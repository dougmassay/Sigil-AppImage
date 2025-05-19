/************************************************************************
**
**  Copyright (C) 2012 John Schember <john@nachtimwald.com>
**  Copyright (C) 2012 Dave Heiland
**  Copyright (C) 2012 Grant Drake
**  Copyright (C) 2009, 2010, 2011  Strahinja Markovic  <strahinja.markovic@gmail.com>
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

#include <QtGui/QDesktopServices>

#include "Misc/Utility.h"
#include "Misc/SettingsStore.h"
#include "ResourceObjects/CSSResource.h"

static const QString W3C_HTML_FORM = "<html>"
                                     " <body>"
                                     "  <p>%1</p>"
                                     "  <p><b>%2</b></p>"
                                     "  <p>%3</p>"
                                     "  <p><input id='button' form='form' type='submit' value='Check' /></p>"
                                     "  <div>"
                                     "   <form id='form' enctype='multipart/form-data' action='http://jigsaw.w3.org/css-validator/validator' method='post'>"
                                     "    <p><textarea name='text' rows='12' cols='70'>%4</textarea></p>"
                                     "    <input type='hidden' name='lang' value='en' />"
                                     "    <input type='hidden' name='profile' value='%5' />"
                                     "   </form>"
                                     "  </div>"
                                     "  <script type='text/javascript'>"
                                     "   function mySubmit() { var frm=document.getElementById('form'); frm.submit(); }"
                                     "   window.onload = function() { window.setTimeout(function() { mySubmit(); }, 3000); };"
                                     "  </script>"
                                     " </body>"
                                     "</html>";

CSSResource::CSSResource(const QString &mainfolder, const QString &fullfilepath, QObject *parent)
    : TextResource(mainfolder, fullfilepath, parent),
      m_TemporaryValidationFiles(QList<QString>())
{
}

CSSResource::~CSSResource()
{
    foreach(QString filepath, m_TemporaryValidationFiles) {
        Utility::SDeleteFile(filepath);
    }
}

bool CSSResource::DeleteCSStyles(QList<CSSInfo::CSSSelector *> css_selectors)
{
    CSSInfo css_info(GetText());
    // Search for selectors with the same definition and line and remove from text
    const QString &new_resource_text = css_info.removeMatchingSelectors(css_selectors);

    if (!new_resource_text.isNull()) {
        // At least one of the selector(s) was removed.
        SetText(new_resource_text);
        emit Modified();
        return true;
    }

    return false;
}

Resource::ResourceType CSSResource::Type() const
{
    return Resource::CSSResourceType;
}

void CSSResource::ValidateStylesheetWithW3C()
{
    SettingsStore settings;
    QString w3c_spec;

    // check user prefs for EPUB stylesheet validation level
    if (GetEpubVersion().startsWith('3')) {
        w3c_spec = settings.cssEpub3ValidationSpec();
    }
    else {
        w3c_spec = settings.cssEpub2ValidationSpec();
    }

    const QString &post_form_html = W3C_HTML_FORM
                                    .arg(tr("Sigil will send your stylesheet data to the <a href='http://jigsaw.w3.org/css-validator/'>W3C Validation Service</a>."))
                                    .arg(tr("This page should disappear once loaded after 3 seconds."))
                                    .arg(tr("If your browser does not have javascript enabled, click on the button below."))
                                    .arg(GetText())
                                    .arg(w3c_spec);
    const QString &temp_file_path = Utility::GetTemporaryFileNameWithExtension(".html");
    Utility::WriteUnicodeTextFile(post_form_html, temp_file_path);
    m_TemporaryValidationFiles.append(temp_file_path);
    QDesktopServices::openUrl(QUrl::fromLocalFile(temp_file_path));
}


bool CSSResource::ReformatCSS(bool multiple_line_format)
{
    QString original_text = GetText();
    CSSInfo css_info(original_text);
    const QString new_text = css_info.getReformattedCSSText(multiple_line_format);

    if (original_text != new_text) {
        SetText(new_text);
	    emit Modified();
    }

    return true;
}
