/************************************************************************
**
**  Copyright (C) 2015-2024 Kevin B. Hendricks, Stratford Ontario Canada
**  Copyright (C) 2011      John Schember <john@nachtimwald.com>
**  Copyright (C) 2011      Grzegorz Wolszczak <grzechu81@gmail.com>
**
**  This file is part of Sigil.
**
**  Sigil is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Sigil is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Sigil.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#include <QtCore/QEvent>
#include <QtCore/QStringList>
#include <QtGui/QKeyEvent>
#include <QDebug>

#include "KeyboardShortcutsWidget.h"
#include "Misc/KeyboardShortcutManager.h"
#include "Misc/SettingsStore.h"

#ifdef Q_OS_WIN32
#include "Windows.h"
#endif

#define DBG if(1)

const int COL_NAME = 0;
const int COL_SHORTCUT = 1;
const int COL_DESCRIPTION = 2;
// This column is not displayed but we still need it so we can reference
// The short cut in the shortcut manager.
const int COL_ID = 3;

KeyboardShortcutsWidget::KeyboardShortcutsWidget()
    : m_EnableAltGr(false)
{
    ui.setupUi(this);
    connectSignalsSlots();
    readSettings();
}

PreferencesWidget::ResultActions KeyboardShortcutsWidget::saveSettings()
{
    PreferencesWidget::ResultActions results = PreferencesWidget::ResultAction_None;

    KeyboardShortcutManager *sm = KeyboardShortcutManager::instance();

    for (int i = 0; i < ui.commandList->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = ui.commandList->topLevelItem(i);
        const QString id = item->text(COL_ID);
        const QKeySequence keySequence = item->text(COL_SHORTCUT);
        sm->setKeySequence(id, keySequence);
    }

    SettingsStore settings;
    settings.setEnableAltGr(ui.EnableAltGr->checkState() == Qt::Checked);
    if (m_EnableAltGr != ui.EnableAltGr->isChecked()) {
        results = results | PreferencesWidget::ResultAction_RestartSigil;
    }
    return results;
}

bool KeyboardShortcutsWidget::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *k = static_cast<QKeyEvent *>(event);
        handleKeyEvent(k);
        return true;
    }

    if (event->type() == QEvent::Shortcut || event->type() == QEvent::KeyRelease) {
        return true;
    }

    if (event->type() == QEvent::ShortcutOverride) {
        // For shortcut overrides, we need to accept as well
        event->accept();
        return true;
    }

    return false;
}

void KeyboardShortcutsWidget::showEvent(QShowEvent *event)
{
    // Fill out the tree view
    readSettings();
    ui.commandList->resizeColumnToContents(COL_NAME);
    ui.commandList->resizeColumnToContents(COL_SHORTCUT);
    QWidget::showEvent(event);
}

void KeyboardShortcutsWidget::treeWidgetItemChangedSlot(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (!current) {
        ui.targetEdit->setText("");
        ui.assignButton->setEnabled(false);
        ui.removeButton->setEnabled(false);
        ui.infoLabel->setText("");
        return;
    }

    const QString shortcut_text = current->text(COL_SHORTCUT);
    ui.targetEdit->setText(shortcut_text);
    ui.assignButton->setEnabled(false);
    ui.removeButton->setEnabled(shortcut_text.length() > 0);
}

void KeyboardShortcutsWidget::removeButtonClicked()
{
    QTreeWidgetItem *item = ui.commandList->currentItem();
    if (item) {
        item->setText(COL_SHORTCUT, "");
    }

    ui.targetEdit->setText("");
    ui.targetEdit->setFocus();
    markSequencesAsDuplicatedIfNeeded();
}

void KeyboardShortcutsWidget::filterEditTextChangedSlot(const QString &text)
{
    const QString newText = text.toUpper();

    // If text is empty - show all items.
    if (text.isEmpty()) {
        for (int i = 0; i < ui.commandList->topLevelItemCount(); i++) {
            ui.commandList->topLevelItem(i)->setHidden(false);
        }
    } else {
        for (int i = 0; i < ui.commandList->topLevelItemCount(); i++) {
            const QString name = ui.commandList->topLevelItem(i)->text(COL_NAME).toUpper();
            const QString description = ui.commandList->topLevelItem(i)->text(COL_DESCRIPTION).toUpper();
            const QString sequence = ui.commandList->topLevelItem(i)->text(COL_SHORTCUT).toUpper();

            if (name.contains(newText) ||
                description.contains(newText) ||
                sequence.contains(newText)
               ) {
                ui.commandList->topLevelItem(i)->setHidden(false);
            } else {
                ui.commandList->topLevelItem(i)->setHidden(true);
            }
        }
    }
}

void KeyboardShortcutsWidget::targetEditTextChangedSlot(const QString &text)
{
    QTreeWidgetItem *item = ui.commandList->currentItem();

    if (item != 0) {
        ui.assignButton->setEnabled(item->text(COL_SHORTCUT) != text);
        ui.removeButton->setEnabled(item->text(COL_SHORTCUT).length() > 0);
    }

    showDuplicatesTextIfNeeded();
}

void KeyboardShortcutsWidget::assignButtonClickedSlot()
{
    const QString new_shortcut = ui.targetEdit->text();

    if (ui.commandList->currentItem() != 0) {
        ui.commandList->currentItem()->setText(COL_SHORTCUT, new_shortcut);
    }

    if (new_shortcut.length() > 0) {
        // Go through all items to remove any conflicting shortcuts
        for (int i = 0; i < ui.commandList->topLevelItemCount(); i++) {
            if (i != ui.commandList->currentIndex().row()) {
                QTreeWidgetItem *item = ui.commandList->topLevelItem(i);

                if (item->text(COL_SHORTCUT) == new_shortcut) {
                    item->setText(COL_SHORTCUT, "");
                }
            }
        }
    }

    ui.infoLabel->clear();
    markSequencesAsDuplicatedIfNeeded();
    ui.assignButton->setEnabled(false);
    ui.removeButton->setEnabled(new_shortcut.length() > 0);
    ui.commandList->setFocus();
}

void KeyboardShortcutsWidget::resetAllButtonClickedSlot()
{
    KeyboardShortcutManager *sm = KeyboardShortcutManager::instance();

    // Go through all items
    for (int i = 0; i < ui.commandList->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = ui.commandList->topLevelItem(i);
        QKeySequence seq = sm->keyboardShortcut(item->text(COL_ID)).defaultKeySequence();
#ifdef Q_OS_MAC
        item->setText(COL_SHORTCUT, seq.toString());
#else
        item->setText(COL_SHORTCUT, seq.toString(QKeySequence::PortableText));
#endif
    }

    markSequencesAsDuplicatedIfNeeded();
}

void KeyboardShortcutsWidget::readSettings()
{
    KeyboardShortcutManager *sm = KeyboardShortcutManager::instance();
    ui.commandList->clear();
    QStringList ids = sm->ids();
    foreach(QString id, ids) {
        KeyboardShortcut shortcut = sm->keyboardShortcut(id);

        if (!shortcut.isEmpty()) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(COL_NAME, shortcut.name());
#ifdef Q_OS_MAC
            item->setText(COL_SHORTCUT, shortcut.keySequence().toString());
#else
            item->setText(COL_SHORTCUT, shortcut.keySequence().toString(QKeySequence::PortableText));
#endif
            item->setText(COL_DESCRIPTION, shortcut.description());
            item->setToolTip(COL_DESCRIPTION, shortcut.toolTip());
            item->setText(COL_ID, id);
            ui.commandList->addTopLevelItem(item);
        }
    }
    ui.commandList->sortItems(0, Qt::AscendingOrder);
    ui.commandList->setAlternatingRowColors(true);
    markSequencesAsDuplicatedIfNeeded();

#if defined(Q_OS_WIN32)
    SettingsStore settings;
    ui.EnableAltGr->setChecked(settings.enableAltGr());
#endif
#if !defined(Q_OS_WIN32)
    ui.EnableAltGr->setVisible(false);
    ui.EnableAltGr->setChecked(false);
#endif
    m_EnableAltGr = ui.EnableAltGr->isChecked();
}

void KeyboardShortcutsWidget::connectSignalsSlots()
{
    connect(ui.commandList, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(treeWidgetItemChangedSlot(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui.removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
    connect(ui.filterEdit, SIGNAL(textChanged(QString)), this, SLOT(filterEditTextChangedSlot(QString)));
    connect(ui.targetEdit, SIGNAL(textChanged(QString)), this, SLOT(targetEditTextChangedSlot(QString)));
    connect(ui.assignButton, SIGNAL(clicked()), this, SLOT(assignButtonClickedSlot()));
    connect(ui.resetAllButton, SIGNAL(clicked()), this, SLOT(resetAllButtonClickedSlot()));
    ui.targetEdit->installEventFilter(this);
}

void KeyboardShortcutsWidget::handleKeyEvent(QKeyEvent *event)
{
    int nextKey = event->key();

    if (nextKey == Qt::Key_Control    ||
        nextKey == Qt::Key_Shift      ||
        nextKey == Qt::Key_Meta       ||
        nextKey == Qt::Key_Alt        ||
        nextKey == Qt::Key_AltGr      ||
        nextKey == Qt::Key_CapsLock   ||
        nextKey == Qt::Key_NumLock    ||
        nextKey == Qt::Key_ScrollLock ||
        nextKey == 0                  ||
        nextKey == Qt::Key_Tab        ||
        nextKey == Qt::Key_Backtab    ||
        nextKey == Qt::Key_unknown    ||
        nextKey == Qt::Key_Backspace  || // This button cannot be assigned, because we want to 'clear' shortcut after backspace push
        ui.commandList->currentItem() == 0 // Do not allow writting in shortcut line edit if no item is selected
       ) {
        // If key was Qt::Key_Backspace additionaly clear sequence dialog
        if (nextKey == Qt::Key_Backspace) {
            removeButtonClicked();
        }
        // allow tab key to shift focus since using it in shortcuts is ill advised
        // on Windows and other platforms
        if (nextKey == Qt::Key_Tab || nextKey == Qt::Key_Backtab) {
            QWidget * upnext = nullptr;
            if (nextKey == Qt::Key_Tab) {
                if (ui.assignButton->isEnabled()) {
                    upnext = ui.assignButton;
                } else if (ui.removeButton->isEnabled()) {
                    upnext = ui.removeButton;
                } else {
                    upnext = nextInFocusChain();
                }
            } else {
                upnext = previousInFocusChain();
            }
            if (upnext) {
                upnext->setFocus(Qt::TabFocusReason);
            }
            return;
        }
        return;
    }

    QString letter = QKeySequence(nextKey).toString(QKeySequence::PortableText);
    Qt::KeyboardModifiers state = event->modifiers();

    DBG qDebug() << "key(): " << QString::number(nextKey) << "  " << QChar(nextKey);
    DBG qDebug() << "text(): " << event->text();
    DBG qDebug() << "nativeVirtualKey(): " << event->nativeVirtualKey();
    DBG qDebug() << "letter: " << letter;
    DBG qDebug() << "modifiers: " << state;

    // Key event generation for shortcuts is one of the most
    // non-crossplatform things in all of Qt so lots of ifdefs

#ifdef Q_OS_WIN32
    if ((state & Qt::GroupSwitchModifier)) {
        // The GroupSwitchModifier via windows altgr option on command line
        // indicates this is AltGr which should not be used for Keyboard Shortcuts.
        // it messes with QKeySequence.toString so fixup letter and turn off
        // the GroupSwitchModifier
        // letter = "" + QChar(nextKey);
        // state = state & ~((int)Qt::GroupSwitchModifier);
        return;
    }
    // try using the Windows call MapVirtualKeyW
    const qint32 vk = event->nativeVirtualKey();
    UINT  result = MapVirtualKeyW(vk, MAPVK_VK_TO_CHAR);
    bool isDeadKey = (result & 0x80000000) == 0x80000000;
    result = result & 0x0000FFFF;
    DBG qDebug() << "MapVK_VK_TO_CHAR: " << QString::number(result) << " " << QChar(result) << " " << isDeadKey;
    if (result != 0) {
        // Dead keys (ie. diacritics should not be used in Keyboard Shortcuts
        if (isDeadKey) return;
        ui.targetEdit->setText(QKeySequence(result | state).toString(QKeySequence::PortableText));
    } else {
        if ((state & Qt::ShiftModifier) && letter.toUpper() == letter.toLower()) {
            // remove the shift since it is included in the keycode
            state = state & ~Qt::SHIFT;
        }
        ui.targetEdit->setText(QKeySequence(nextKey | state).toString(QKeySequence::PortableText));
    }
#else  /* linux and macos */

#ifdef Q_OS_MAC
    // macOS treats some META+SHIFT+key sequences incorrectly so try to fix it up
    if ((state & Qt::MetaModifier) && (letter.toUpper() == letter.toLower())) {
        state = state & ~Qt::SHIFT;
    }
#endif

    nextKey |= translateModifiers(state, event->text());
    ui.targetEdit->setText(QKeySequence(nextKey).toString(QKeySequence::PortableText));

#endif

    DBG qDebug() << "\n";

    event->accept();
}


int KeyboardShortcutsWidget::translateModifiers(Qt::KeyboardModifiers state, const QString &text)
{
    int result = 0;

    // The shift modifier only counts when it is not used to type a symbol
    // that is only reachable using the shift key anyway
    if ((state & Qt::ShiftModifier) && (text.size() == 0
                                        || !text.at(0).isPrint()
                                        || text.at(0).isLetterOrNumber()
                                        || text.at(0).isSpace()
                                       )
       ) {
        result |= Qt::SHIFT;
    }

    if (state & Qt::ControlModifier) {
        result |= Qt::CTRL;
    }

    if (state & Qt::MetaModifier) {
        result |= Qt::META;
    }

    if (state & Qt::AltModifier) {
        result |= Qt::ALT;
    }

    return result;
}

void KeyboardShortcutsWidget::markSequencesAsDuplicatedIfNeeded()
{
    // This is not optized , but since this will be called rather seldom
    // effort for optimization may not be worth it
    QMap<QKeySequence, QSet<QTreeWidgetItem *>> seqMap;

    // Go through all items
    for (int i = 0; i < ui.commandList->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = ui.commandList->topLevelItem(i);
        const QKeySequence keySequence = item->text(COL_SHORTCUT);
        seqMap[keySequence].insert(item);
    }

    // Now, mark all conflicting sequences
    foreach(QKeySequence sequence, seqMap.keys()) {
        QSet<QTreeWidgetItem *> itemSet = seqMap[sequence];

        if (itemSet.size() > 1) {
            //mark items as conflicted items
            foreach(QTreeWidgetItem * item, itemSet.values()) {
                QFont font = item->font(COL_SHORTCUT);
                font.setBold(true);
                item->setForeground(COL_SHORTCUT, QColor(Qt::red));
                item->setFont(COL_SHORTCUT, font);
            }
        } else {
            // Mark as non-confilicted
            foreach(QTreeWidgetItem * item, itemSet.values()) {
                QFont font = item->font(COL_SHORTCUT);
                font.setBold(false);
                item->setForeground(COL_SHORTCUT, QPalette().color(QPalette::Text));
                item->setFont(COL_SHORTCUT, font);
            }
        }
    }
}

void KeyboardShortcutsWidget::showDuplicatesTextIfNeeded()
{
    ui.infoLabel->clear();
    const QString new_shortcut = ui.targetEdit->text();

    if (new_shortcut.length() > 0) {
        // Display any conflicting shortcuts in the info label
        QStringList *conflicts = new QStringList();

        for (int i = 0; i < ui.commandList->topLevelItemCount(); i++) {
            if (i != ui.commandList->currentIndex().row()) {
                QTreeWidgetItem *item = ui.commandList->topLevelItem(i);

                if (item->text(COL_SHORTCUT) == new_shortcut) {
                    conflicts->append(item->text(COL_NAME));
                }
            }
        }

        if (conflicts->count() > 0) {
            ui.infoLabel->setText(tr("Conflicts with: <b>") + conflicts->join("</b>, <b>") + "</b>");
        }
    }
}
