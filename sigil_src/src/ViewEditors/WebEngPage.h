/************************************************************************
**
**  Copyright (C) 2019-2024 Kevin B. Hendricks, Stratford Ontario Canada
**  Copyright (C) 2023- Doug Massay
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

#pragma once
#ifndef WEBENGPAGE_H
#define WEBENGPAGE_H

#include <QObject>
#include <QUrl>
#include <QtWebEngineWidgets>
#include <QtWebEngineCore>
#include <QWebEnginePage>

class QWebEngineProfile;

class WebEngPage : public QWebEnginePage
{
    Q_OBJECT

public:
    WebEngPage(QWebEngineProfile* profile=nullptr, QObject *parent = 0, bool setbackgound = true);

    bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType ntype, bool isMainFrame);
    void javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, 
                                  const QString & message, int lineNumber, const QString & sourceID);

signals:
    void LinkClicked(const QUrl &url);

private slots:
    void EmitLinkClicked();    
  
private:
    QUrl m_url;
};

#endif // WEBENGPAGE_H
