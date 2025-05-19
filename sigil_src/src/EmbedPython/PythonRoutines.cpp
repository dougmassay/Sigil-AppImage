/************************************************************************
**
**  Copyright (C) 2016-2025 Kevin B. Hendricks, Stratford Ontario Canada
**  Copyright (C) 2016-2022 Doug Massay
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
#include "EmbedPython/EmbeddedPython.h"

#include <QString>
#include <QList>
#include <QVariant>

#include <QDebug>
#include "sigil_constants.h"
#include "Misc/Utility.h"
#include "EmbedPython/PythonRoutines.h"


QString PythonRoutines::GenerateNcxInPython(const QString &navdata, const QString &navbkpath, 
                                            const QString &ncxdir, const QString &doctitle, 
                                            const QString &mainid)
{
    QString results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(navdata));
    args.append(QVariant(navbkpath));
    args.append(QVariant(ncxdir));
    args.append(QVariant(doctitle));
    args.append(QVariant(mainid));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("ncxgenerator"),
                                         QString("generateNCX"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toString();
    }
    return results;
}


MetadataPieces PythonRoutines::GetMetadataInPython(const QString& opfdata, const QString& version) 
{
    int rv = 0;
    QString traceback;
    MetadataPieces mdp;

    QString module = "metaproc2";
    if (version.startsWith('3')) module = "metaproc3";
    QList<QVariant> args;
    args.append(QVariant(opfdata));
    EmbeddedPython* epp = EmbeddedPython::instance();
    QVariant res = epp->runInPython(module, QString("process_metadata"), args, &rv, traceback, true);
    if (rv) {
        fprintf(stderr, "process_meta error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    PyObjectPtr mpo = PyObjectPtr(res);
    args.clear();
    res = epp->callPyObjMethod(mpo, QString("get_recognized_metadata"), args, &rv, traceback);
    if (rv) {
        fprintf(stderr, "get_recognized_metadata error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    mdp.data = res.toString();
    args.clear();
    res = epp->callPyObjMethod(mpo, QString("get_other_meta_xml"), args, &rv, traceback);
    if (rv) {
        fprintf(stderr, "get_other_meta_xml error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    mdp.otherxml = res.toString();
    args.clear();
    res = epp->callPyObjMethod(mpo, QString("get_id_list"), args, &rv, traceback);
    if (rv) {
        fprintf(stderr, "get_id_list error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    mdp.idlist = res.toStringList();
    args.clear();
    res = epp->callPyObjMethod(mpo, QString("get_metadata_tag"), args, &rv, traceback);
    if (rv) {
        fprintf(stderr, "get_metadata_tag error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    mdp.metatag = res.toString();
    return mdp;
}


QString PythonRoutines::SetNewMetadataInPython(const MetadataPieces& mdp, const QString& opfdata, const QString& version) 
{
    int rv = 0;
    QString traceback;
    QString newopfdata= opfdata;
    QString module = "metaproc2";
    if (version.startsWith('3')) module = "metaproc3";
    QList<QVariant> args;
    args.append(QVariant(mdp.data));
    args.append(QVariant(mdp.otherxml));
    args.append(QVariant(mdp.idlist));
    args.append(QVariant(mdp.metatag));
    args.append(QVariant(opfdata));
    EmbeddedPython* epp = EmbeddedPython::instance();
    QVariant res = epp->runInPython(module, QString("set_new_metadata"), args, &rv, traceback, true);
    if (rv) {
        fprintf(stderr, "set_new_metadata error %d traceback %s\n",rv, traceback.toStdString().c_str());
        return newopfdata;
    }
    newopfdata = res.toString();
    return newopfdata;
}


QString PythonRoutines::PerformRepoCommitInPython(const QString &localRepo, 
                                                  const QString &bookid, 
                                                  const QStringList &bookinfo,
                                                  const QString &bookroot, 
                                                  const QStringList &bookfiles) 
{
    QString results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));
    args.append(QVariant(bookinfo));
    args.append(QVariant(bookroot));
    args.append(QVariant(bookfiles));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("performCommit"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toString();
    }
    return results;
}


bool PythonRoutines::PerformRepoEraseInPython(const QString& localRepo, const QString& bookid)
{
    bool results = false;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("eraseRepo"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = (res.toInt() > 0);
    }
    return results;
}

QStringList PythonRoutines::GetRepoTagsInPython(const QString& localRepo, const QString& bookid)
{
    QStringList results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("get_tag_list"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toStringList();
    }
    return results;
}

bool PythonRoutines::ChangeRepoTagMsgInPython(const QString& localRepo, const QString& bookid, const QString& tagname, const QString& newmessage)
{
    bool results = false;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));
    args.append(QVariant(tagname));
    args.append(QVariant(newmessage));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("update_annotated_tag_message"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = (res.toInt() > 0);
    }
    return results;
}


QString PythonRoutines::GenerateEpubFromTagInPython(const QString& localRepo,
                                                    const QString& bookid,
                                                    const QString& tagname,
                                                    const QString& filename,
                                                    const QString& destpath)
{
    QString results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));
    args.append(QVariant(tagname));
    args.append(QVariant(filename));
    args.append(QVariant(destpath));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("generate_epub_from_tag"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toString();
    }
    return results;
}


QString PythonRoutines::GenerateDiffFromCheckPoints(const QString& localRepo,
                                    const QString& bookid,
                                    const QString& leftchkpoint,
                                    const QString& rightchkpoint)
{
    QString results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));
    args.append(QVariant(leftchkpoint));
    args.append(QVariant(rightchkpoint));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("generate_diff_from_checkpoints"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toString();
    }
    return results;
}

QString PythonRoutines::GenerateRepoLogSummaryInPython(const QString& localRepo,
                                                       const QString& bookid)
{
    QString results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("generate_log_summary"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toString();
    }
    return results;
}


QList<DiffRecord::DiffRec> PythonRoutines::GenerateParsedNDiffInPython(const QString& path1,
                                                                       const QString& path2)
{
    QList<DiffRecord::DiffRec> results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(path1));
    args.append(QVariant(path2));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("generate_parsed_ndiff"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        QVariantList vlist = res.toList();
        foreach(QVariant qv, vlist) {
            QStringList fields = qv.toStringList();
            DiffRecord::DiffRec dr;
            dr.code = fields.at(0);
            dr.line = fields.at(1);
            dr.newline = fields.at(2);
            dr.leftchanges = fields.at(3);
            dr.rightchanges = fields.at(4);
            results << dr;
        }
    }
    return results;
}



QString PythonRoutines::GenerateUnifiedDiffInPython(const QString& path1, const QString& path2)
{
    QString results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(path1));
    args.append(QVariant(path2));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("generate_unified_diff"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toString();
    }
    return results;
}


// returns 3 string lists: deleted, added, and modified (in that order)
QList<QStringList> PythonRoutines::GetCurrentStatusVsDestDirInPython(const QString&bookroot,
                                                                     const QStringList& bookfiles,
                                                                     const QString& destdir)
{
    QList<QStringList> results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(bookroot));
    args.append(QVariant(bookfiles));
    args.append(QVariant(destdir));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("get_current_status_vs_destdir"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        QVariantList vlist = res.toList();
        foreach(QVariant qv, vlist) {
            results << qv.toStringList();
        }
    }
    return results;
}


QString PythonRoutines::CopyTagToDestDirInPython(const QString& localRepo,
                                                 const QString& bookid,
                                                 const QString& tagname,
                                                 const QString& destdir)
{
    QString results;
    int rv = -1;
    QString error_traceback;
    QList<QVariant> args;
    args.append(QVariant(localRepo));
    args.append(QVariant(bookid));
    args.append(QVariant(tagname));
    args.append(QVariant(destdir));

    EmbeddedPython * epython  = EmbeddedPython::instance();

    QVariant res = epython->runInPython( QString("repomanager"),
                                         QString("copy_tag_to_destdir"),
                                         args,
                                         &rv,
                                         error_traceback);
    if (rv == 0) {
        results = res.toString();
    }
    return results;
}


QString PythonRoutines::RebaseManifestIDsInPython(const QString& opfdata) 
{
    int rv = 0;
    QString traceback;
    QString newopfdata= opfdata;
    QString module = "fix_opf_ids";
    QList<QVariant> args;
    args.append(QVariant(opfdata));
    EmbeddedPython* epp = EmbeddedPython::instance();
    QVariant res = epp->runInPython(module, QString("rebase_manifest_ids"), args, &rv, traceback, true);
    if (rv) {
        fprintf(stderr, "rebase_manifest_ids error %d traceback %s\n",rv, traceback.toStdString().c_str());
        return newopfdata;
    }
    newopfdata = res.toString();
    return newopfdata;
}


PyObjectPtr PythonRoutines::SetupInitialFunctionSearchEnvInPython(const QString& function_name)
{
    int rv = 0;
    PyObjectPtr FSO;
    QString traceback;
    QString jsonpath = Utility::DefinePrefsDir() + "/" + SIGIL_FUNCTION_REPLACE_JSON_FILE;
    QString metaxml = Utility::GetMainWindowMetadata();
    QString module = "functionsearch";
    QList<QVariant> args;
    args.append(QVariant(metaxml));
    args.append(QVariant(function_name));
    args.append(QVariant(jsonpath));
    EmbeddedPython* epp = EmbeddedPython::instance();
    QVariant res = epp->runInPython(module, QString("getFunctionSearchEnv"), args, &rv, traceback, true);
    if (rv) {
        fprintf(stderr, "getFunctionSearchEnv error %d traceback %s\n",rv, traceback.toStdString().c_str());
	return FSO;
    }
    FSO = PyObjectPtr(res);
    args.clear();
    return FSO;
}


QString PythonRoutines::DoFunctionSearchTextReplacementsInPython(PyObjectPtr FSO,
                                                                 const QString& pattern,
                                                                 const QString& bookpath, const QString& text)
{
    if (FSO.isNull()) {
       fprintf(stderr, "do_text_replacements error - null Search Environment\n");
       return text;
    }
    int rv = 0;
    QString traceback;
    QList<QVariant> args;
    args.append(QVariant(pattern));
    args.append(QVariant(bookpath));
    args.append(QVariant(text));
    EmbeddedPython* epp = EmbeddedPython::instance();
    QVariant res = epp->callPyObjMethod(FSO, QString("do_text_replacements"), args, &rv, traceback);
    if (rv) {
        fprintf(stderr, "do_text_replacements error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    args.clear();
    return res.toString();
}


int PythonRoutines::GetCurrentReplacementCountInPython(PyObjectPtr FSO)
{
    if (FSO.isNull()) {
       fprintf(stderr, "get_current_replacement_count error - null Search Environment\n");
       return 0;
    }
    int rv = 0;
    QString traceback;
    QList<QVariant> args;
    EmbeddedPython* epp = EmbeddedPython::instance();
    QVariant res = epp->callPyObjMethod(FSO, QString("get_current_replacement_count"), args, &rv, traceback);
    if (rv) {
        fprintf(stderr, "get_current_replacement_count error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    return res.toInt();
}


QString PythonRoutines::GetSingleReplacementByFunction(PyObjectPtr FSO,
                                                       const QString& text,
                                                       const QList<std::pair<int,int> > capture_groups)
{
    if (FSO.isNull()) {
        fprintf(stderr, "get_single_replacement_by_function error - null Search Environment\n");
        return text;
    }
    int rv = 0;
    QString traceback;
    QList<QVariant> args;
    EmbeddedPython* epp = EmbeddedPython::instance();
    args.append(QVariant(text));
    QList<QVariant> groups;
    for (int i=0; i < capture_groups.size(); i++) {
        std::pair<int, int> pi = capture_groups.at(i);
        QVariant v = QVariant::fromValue(pi);
        groups.append(v);
    }
    args.append(QVariant(groups));

    QVariant res = epp->callPyObjMethod(FSO, QString("get_single_replacement_by_function"), args, &rv, traceback);
    if (rv) {
        fprintf(stderr, "get_single_replacement_by_function error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    return res.toString();
}


bool PythonRoutines::CreateUserJsonFileInPython()
{
    int rv = 0;
    QString traceback;
    QString jsonpath = Utility::DefinePrefsDir() + "/" + SIGIL_FUNCTION_REPLACE_JSON_FILE;
    QString module = "functionsearch";
    QList<QVariant> args;
    args.append(QVariant(jsonpath));
    EmbeddedPython* epp = EmbeddedPython::instance();
    QVariant res = epp->runInPython(module, QString("createJsonFile"), args, &rv, traceback, true);
    if (rv) {
        fprintf(stderr, "createJsonFile error %d traceback %s\n",rv, traceback.toStdString().c_str());
    }
    int v = res.toInt();
    args.clear();
    if (v) return true;
    return false;
}
