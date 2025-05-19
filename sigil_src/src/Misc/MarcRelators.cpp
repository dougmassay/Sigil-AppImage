/************************************************************************
**
**  Copyright (C) 2016  Kevin B. Hendricks, Stratford, Ontario, Canada
**  Copyright (C) 2011  John Schember <john@nachtimwald.com>
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

#include <QString>
#include <QStringList>
#include <QHash>

#include "Misc/MarcRelators.h"

MarcRelators *MarcRelators::m_instance = 0;

MarcRelators *MarcRelators::instance()
{
    if (m_instance == 0) {
        m_instance = new MarcRelators();
    }

    return m_instance;
}


QString MarcRelators::GetName(QString code)
{
    DescriptiveInfo rel = m_CodeMap.value(code, DescriptiveInfo() );
    return rel.name;
}


QString MarcRelators::GetDescriptionByCode(QString code)
{
    DescriptiveInfo rel = m_CodeMap.value(code, DescriptiveInfo());
    return rel.description;
}


QString MarcRelators::GetDescriptionByName(QString name)
{
    QString code = m_NameMap.value(name, QString());
    return GetDescriptionByCode(code);
}


QString MarcRelators::GetCode(QString name)
{
    return m_NameMap.value(name, QString());
}

bool MarcRelators::isRelatorCode(QString code)
{
    return m_CodeMap.contains(code);
}

bool MarcRelators::isRelatorName(QString name)
{
    return m_NameMap.contains(name);
}

QStringList MarcRelators::GetSortedNames()
{
    if (!m_sortedNames.isEmpty()) {
        return m_sortedNames;
    }

    foreach(QString code, m_CodeMap.keys()) {
        DescriptiveInfo rel = m_CodeMap.value(code);
        m_sortedNames.append(rel.name);
    }
    m_sortedNames.sort();
    return m_sortedNames;
}

const QHash<QString, DescriptiveInfo> & MarcRelators::GetCodeMap()
{
    return m_CodeMap;
}

MarcRelators::MarcRelators()
{
    SetRelatorsMap();
}

void MarcRelators::SetRelatorsMap()
{
    if (!m_CodeMap.isEmpty()) {
        return;
    }

    // For the authoritiative relator list and descriptive definitions see http://www.loc.gov/marc/relators/
    // Names and codes must be unique between basic and advanced (except Publisher).
    QStringList data;
    data <<
         tr("Abridger") << "abr" << tr("A person, family, or organization contributing to a resource by shortening or condensing the original work but leaving the nature and content of the original work substantially unchanged. For substantial modifications that result in the creation of a new work, see Author.") <<
         tr("Actor") << "act" << tr("Use for a person or organization who principally exhibits acting skills in a musical or dramatic presentation or entertainment.") <<
         tr("Adapter") << "adp" << tr("Use for a person or organization who 1) reworks a musical composition, usually for a different medium, or 2) rewrites novels or stories for motion pictures or other audiovisual medium.") <<
         tr("Analyst") << "anl" << tr("Use for a person or organization that reviews, examines and interprets data or information in a specific area.") <<
         tr("Animator") << "anm" << tr("Use for a person or organization who draws the two-dimensional figures, manipulates the three dimensional objects and/or also programs the computer to move objects and images for the purpose of animated film processing. Animation cameras, stands, celluloid screens, transparencies and inks are some of the tools of the animator.") <<
         tr("Annotator") << "ann" << tr("Use for a person who writes manuscript annotations on a printed item.") <<
         tr("Appellant") << "apl" << tr("A person or organization who appeals a lower court's decision.") <<
         tr("Appellee") << "ape" << tr("A person or organization against whom an appeal is taken.") <<
         tr("Applicant") << "app" << tr("Use for a person or organization responsible for the submission of an application or who is named as eligible for the results of the processing of the application (e.g., bestowing of rights, reward, title, position).") <<
         tr("Architect") << "arc" << tr("Use for a person or organization who designs structures or oversees their construction.") <<
         tr("Arranger") << "arr" << tr("Use for a person or organization who transcribes a musical composition, usually for a different medium from that of the original; in an arrangement the musical substance remains essentially unchanged.") <<
         tr("Art copyist") << "acp" << tr("Use for a person (e.g., a painter or sculptor) who makes copies of works of visual art.") <<
         tr("Art director") << "adi" << tr("A person contributing to a motion picture or television production by overseeing the artists and craftspeople who build the sets.") <<
         tr("Artist") << "art" << tr("Use for a person (e.g., a painter) or organization who conceives, and perhaps also implements, an original graphic design or work of art, if specific codes (e.g., [egr], [etr]) are not desired. For book illustrators, prefer Illustrator [ill]. ") <<
         tr("Artistic director") << "ard" << tr("Use for a person responsible for controlling the development of the artistic style of an entire production, including the choice of works to be presented and selection of senior production staff.") <<
         tr("Assignee") << "asg" << tr("Use for a person or organization to whom a license for printing or publishing has been transferred.") <<
         tr("Associated name") << "asn" << tr("Use for a person or organization associated with or found in an item or collection, which cannot be determined to be that of a Former owner [fmo] or other designated relator indicative of provenance.") <<
         tr("Attributed name") << "att" << tr("Use for an author, artist, etc., relating him/her to a work for which there is or once was substantial authority for designating that person as author, creator, etc. of the work. ") <<
         tr("Auctioneer") << "auc" << tr("Use for a person or organization in charge of the estimation and public auctioning of goods, particularly books, artistic works, etc.") <<
         tr("Author") << "aut" << tr("Use for a person or organization chiefly responsible for the intellectual or artistic content of a work, usually printed text. This term may also be used when more than one person or body bears such responsibility. ") <<
         tr("Author in quotations or text extracts") << "aqt" << tr("Use for a person or organization whose work is largely quoted or extracted in works to which he or she did not contribute directly. Such quotations are found particularly in exhibition catalogs, collections of photographs, etc.") <<
         tr("Author of afterword, colophon, etc.") << "aft" << tr("Use for a person or organization responsible for an afterword, postface, colophon, etc. but who is not the chief author of a work.") <<
         tr("Author of dialog") << "aud" << tr("Use for a person or organization responsible for the dialog or spoken commentary for a screenplay or sound recording.") <<
         tr("Author of introduction, etc.") << "aui" << tr("Use for a person or organization responsible for an introduction, preface, foreword, or other critical introductory matter, but who is not the chief author.") <<
         tr("Author of screenplay, etc.") << "aus" << tr("Use for a person or organization responsible for a motion picture screenplay, dialog, spoken commentary, etc.") <<
         tr("Autographer") << "ato" << tr("A person whose manuscript signature appears on an item.") <<
         tr("Bibliographic antecedent") << "ant" << tr("Use for a person or organization responsible for a work upon which the work represented by the catalog record is based. This may be appropriate for adaptations, sequels, continuations, indexes, etc.") <<
         tr("Binder") << "bnd" << tr("Use for a person or organization responsible for the binding of printed or manuscript materials.") <<
         tr("Binding designer") << "bdd" << tr("Use for a person or organization responsible for the binding design of a book, including the type of binding, the type of materials used, and any decorative aspects of the binding. ") <<
         tr("Blurb writer") << "blw" << tr("A person or organization responsible for writing a commendation or testimonial for a work, which appears on or within the publication itself, frequently on the back or dust jacket of print publications or on advertising material for all media.") <<
         tr("Book designer") << "bkd" << tr("Use for a person or organization responsible for the entire graphic design of a book, including arrangement of type and illustration, choice of materials, and process used. ") <<
         tr("Book producer") << "bkp" << tr("Use for a person or organization responsible for the production of books and other print media, if specific codes (e.g., [bkd], [egr], [tyd], [prt]) are not desired. ") <<
         tr("Bookjacket designer") << "bjd" << tr("Use for a person or organization responsible for the design of flexible covers designed for or published with a book, including the type of materials used, and any decorative aspects of the bookjacket. ") <<
         tr("Bookplate designer") << "bpd" << tr("Use for a person or organization responsible for the design of a book owner's identification label that is most commonly pasted to the inside front cover of a book. ") <<
         tr("Bookseller") << "bsl" << tr("Use for a person or organization who makes books and other bibliographic materials available for purchase. Interest in the materials is primarily lucrative.") <<
         tr("Braille embosser") << "brl" << tr("A person, family, or organization involved in manufacturing a resource by embossing Braille cells using a stylus, special embossing printer, or other device.") <<
         tr("Broadcaster") << "brd" << tr("A person, family, or organization involved in broadcasting a resource to an audience via radio, television, webcast, etc.") <<
         tr("Calligrapher") << "cll" << tr("Use for a person or organization who writes in an artistic hand, usually as a copyist and or engrosser.") <<
         tr("Cartographer") << "ctg" << tr("Use for a person or organization responsible for the creation of maps and other cartographic materials.") <<
         tr("Caster") << "cas" << tr("A person, family, or organization involved in manufacturing a resource by pouring a liquid or molten substance into a mold and leaving it to solidify to take the shape of the mold.") <<
         tr("Censor") << "cns" << tr("Use for a censor, bowdlerizer, expurgator, etc., official or private. ") <<
         tr("Choreographer") << "chr" << tr("Use for a person or organization who composes or arranges dances or other movements (e.g., \"master of swords\") for a musical or dramatic presentation or entertainment.") <<
         tr("Cinematographer") << "cng" << tr("Use for a person or organization who is in charge of the images captured for a motion picture film. The cinematographer works under the supervision of a director, and may also be referred to as director of photography. Do not confuse with videographer.") <<
         tr("Client") << "cli" << tr("Use for a person or organization for whom another person or organization is acting.") <<
         tr("Collection registrar") << "cor" << tr("A curator who lists or inventories the items in an aggregate work such as a collection of items or works.") <<
         tr("Collector") << "col" << tr("Use for a person or organization who has brought together material from various sources that has been arranged, described, and cataloged as a collection. A collector is neither the creator of the material nor a person to whom manuscripts in the collection may have been addressed.") <<
         tr("Collotyper") << "clt" << tr("Use for a person or organization responsible for the production of photographic prints from film or other colloid that has ink-receptive and ink-repellent surfaces.") <<
         tr("Colorist") << "clr" << tr("A person or organization responsible for applying color to drawings, prints, photographs, maps, moving images, etc.") <<
         tr("Commentator") << "cmm" << tr("Use for a person or organization who provides interpretation, analysis, or a discussion of the subject matter on a recording, motion picture, or other audiovisual medium.") <<
         tr("Commentator for written text") << "cwt" << tr("Use for a person or organization responsible for the commentary or explanatory notes about a text. For the writer of manuscript annotations in a printed book, use Annotator [ann].") <<
         tr("Compiler") << "com" << tr("Use for a person or organization who produces a work or publication by selecting and putting together material from the works of various persons or bodies.") <<
         tr("Complainant") << "cpl" << tr("Use for the party who applies to the courts for redress, usually in an equity proceeding.") <<
         tr("Complainant-appellant") << "cpt" << tr("Use for a complainant who takes an appeal from one court or jurisdiction to another to reverse the judgment, usually in an equity proceeding.") <<
         tr("Complainant-appellee") << "cpe" << tr("Use for a complainant against whom an appeal is taken from one court or jurisdiction to another to reverse the judgment, usually in an equity proceeding.") <<
         tr("Composer") << "cmp" << tr("Use for a person or organization who creates a musical work, usually a piece of music in manuscript or printed form.") <<
         tr("Compositor") << "cmt" << tr("Use for a person or organization responsible for the creation of metal slug, or molds made of other materials, used to produce the text and images in printed matter. ") <<
         tr("Conceptor") << "ccp" << tr("Use for a person or organization responsible for the original idea on which a work is based, this includes the scientific author of an audio-visual item and the conceptor of an advertisement.") <<
         tr("Conductor") << "cnd" << tr("Use for a person who directs a performing group (orchestra, chorus, opera, etc.) in a musical or dramatic presentation or entertainment.") <<
         tr("Conservator") << "con" << tr("A person or organization responsible for documenting, preserving, or treating printed or manuscript material, works of art, artifacts, or other media.") <<
         tr("Consultant") << "csl" << tr("Use for a person or organization relevant to a resource, who is called upon for professional advice or services in a specialized field of knowledge or training.") <<
         tr("Consultant to a project") << "csp" << tr("Use for a person or organization relevant to a resource, who is engaged specifically to provide an intellectual overview of a strategic or operational task and by analysis, specification, or instruction, to create or propose a cost-effective course of action or solution.") <<
         tr("Contestant") << "cos" << tr("Use for the party who opposes, resists, or disputes, in a court of law, a claim, decision, result, etc.") <<
         tr("Contestant-appellant") << "cot" << tr("Use for a contestant who takes an appeal from one court of law or jurisdiction to another to reverse the judgment.") <<
         tr("Contestant-appellee") << "coe" << tr("Use for a contestant against whom an appeal is taken from one court of law or jurisdiction to another to reverse the judgment.") <<
         tr("Contestee") << "cts" << tr("Use for the party defending a claim, decision, result, etc. being opposed, resisted, or disputed in a court of law.") <<
         tr("Contestee-appellant") << "ctt" << tr("Use for a contestee who takes an appeal from one court or jurisdiction to another to reverse the judgment.") <<
         tr("Contestee-appellee") << "cte" << tr("Use for a contestee against whom an appeal is taken from one court or jurisdiction to another to reverse the judgment.") <<
         tr("Contractor") << "ctr" << tr("Use for a person or organization relevant to a resource, who enters into a contract with another person or organization to perform a specific task.") <<
         tr("Contributor") << "ctb" << tr("Use for a person or organization one whose work has been contributed to a larger work, such as an anthology, serial publication, or other compilation of individual works. Do not use if the sole function in relation to a work is as author, editor, compiler or translator.") <<
         tr("Copyright claimant") << "cpc" << tr("Use for a person or organization listed as a copyright owner at the time of registration. Copyright can be granted or later transferred to another person or organization, at which time the claimant becomes the copyright holder.") <<
         tr("Copyright holder") << "cph" << tr("Use for a person or organization to whom copy and legal rights have been granted or transferred for the intellectual content of a work. The copyright holder, although not necessarily the creator of the work, usually has the exclusive right to benefit financially from the sale and use of the work to which the associated copyright protection applies.") <<
         tr("Corrector") << "crr" << tr("Use for a person or organization who is a corrector of manuscripts, such as the scriptorium official who corrected the work of a scribe. For printed matter, use Proofreader.") <<
         tr("Correspondent") << "crp" << tr("Use for a person or organization who was either the writer or recipient of a letter or other communication.") <<
         tr("Costume designer") << "cst" << tr("Use for a person or organization who designs or makes costumes, fixes hair, etc., for a musical or dramatic presentation or entertainment.") <<
         tr("Court governed") << "cou" << tr("A court governed by court rules, regardless of their official nature (e.g., laws, administrative regulations.)") <<
         tr("Court reporter") << "crt" << tr("A person, family, or organization contributing to a resource by preparing a court's opinions for publication.") <<
         tr("Cover designer") << "cov" << tr("Use for a person or organization responsible for the graphic design of a book cover, album cover, slipcase, box, container, etc. For a person or organization responsible for the graphic design of an entire book, use Book designer; for book jackets, use Bookjacket designer.") <<
         tr("Creator") << "cre" << tr("Use for a person or organization responsible for the intellectual or artistic content of a work.") <<
         tr("Curator of an exhibition") << "cur" << tr("Use for a person or organization responsible for conceiving and organizing an exhibition.") <<
         tr("Dancer") << "dnc" << tr("Use for a person or organization who principally exhibits dancing skills in a musical or dramatic presentation or entertainment.") <<
         tr("Data contributor") << "dtc" << tr("Use for a person or organization that submits data for inclusion in a database or other collection of data.") <<
         tr("Data manager") << "dtm" << tr("Use for a person or organization responsible for managing databases or other data sources.") <<
         tr("Dedicatee") << "dte" << tr("Use for a person or organization to whom a book, manuscript, etc., is dedicated (not the recipient of a gift).") <<
         tr("Dedicator") << "dto" << tr("Use for the author of a dedication, which may be a formal statement or in epistolary or verse form.") <<
         tr("Defendant") << "dfd" << tr("Use for the party defending or denying allegations made in a suit and against whom relief or recovery is sought in the courts, usually in a legal action.") <<
         tr("Defendant-appellant") << "dft" << tr("Use for a defendant who takes an appeal from one court or jurisdiction to another to reverse the judgment, usually in a legal action.") <<
         tr("Defendant-appellee") << "dfe" << tr("Use for a defendant against whom an appeal is taken from one court or jurisdiction to another to reverse the judgment, usually in a legal action.") <<
         tr("Degree grantor") << "dgg" << tr("Use for the organization granting a degree for which the thesis or dissertation described was presented.") <<
         tr("Delineator") << "dln" << tr("Use for a person or organization executing technical drawings from others' designs.") <<
         tr("Depicted") << "dpc" << tr("Use for an entity depicted or portrayed in a work, particularly in a work of art.") <<
         tr("Depositor") << "dpt" << tr("Use for a person or organization placing material in the physical custody of a library or repository without transferring the legal title.") <<
         tr("Designer") << "dsr" << tr("Use for a person or organization responsible for the design if more specific codes (e.g., [bkd], [tyd]) are not desired.") <<
         tr("Director") << "drt" << tr("Use for a person or organization who is responsible for the general management of a work or who supervises the production of a performance for stage, screen, or sound recording.") <<
         tr("Dissertant") << "dis" << tr("Use for a person who presents a thesis for a university or higher-level educational degree.") <<
         tr("Distribution place") << "dbp" << tr("A place from which a resource, e.g., a serial, is distributed.") <<
         tr("Distributor") << "dst" << tr("Use for a person or organization that has exclusive or shared marketing rights for an item.") <<
         tr("Donor") << "dnr" << tr("Use for a person or organization who is the donor of a book, manuscript, etc., to its present owner. Donors to previous owners are designated as Former owner [fmo] or Inscriber [ins].") <<
         tr("Draftsman") << "drm" << tr("Use for a person or organization who prepares artistic or technical drawings. ") <<
         tr("Dubious author") << "dub" << tr("Use for a person or organization to which authorship has been dubiously or incorrectly ascribed.") <<
         tr("Editor") << "edt" << tr("Use for a person or organization who prepares for publication a work not primarily his/her own, such as by elucidating text, adding introductory or other critical matter, or technically directing an editorial staff.") <<
         tr("Editor of compilation") << "edc" << tr("A person, family, or organization contributing to a collective or aggregate work by selecting and putting together works, or parts of works, by one or more creators. For compilations of data, information, etc., that result in new works, see compiler.") <<
         tr("Editor of moving image work") << "edm" << tr("A person, family, or organization responsible for assembling, arranging, and trimming film, video, or other moving image formats, including both visual and audio aspects.") <<
         tr("Electrician") << "elg" << tr("Use for a person responsible for setting up a lighting rig and focusing the lights for a production, and running the lighting at a performance.") <<
         tr("Electrotyper") << "elt" << tr("Use for a person or organization who creates a duplicate printing surface by pressure molding and electrodepositing of metal that is then backed up with lead for printing.") <<
         tr("Engineer") << "eng" << tr("Use for a person or organization that is responsible for technical planning and design, particularly with construction.") <<
         tr("Engraver") << "egr" << tr("Use for a person or organization who cuts letters, figures, etc. on a surface, such as a wooden or metal plate, for printing.") <<
         tr("Etcher") << "etr" << tr("Use for a person or organization who produces text or images for printing by subjecting metal, glass, or some other surface to acid or the corrosive action of some other substance.") <<
         tr("Event place") << "evp" << tr("A place where an event such as a conference or a concert took place.") <<
         tr("Expert") << "exp" << tr("Use for a person or organization in charge of the description and appraisal of the value of goods, particularly rare items, works of art, etc. ") <<
         tr("Facsimilist") << "fac" << tr("Use for a person or organization that executed the facsimile.") <<
         tr("Field director") << "fld" << tr("Use for a person or organization that manages or supervises the work done to collect raw data or do research in an actual setting or environment (typically applies to the natural and social sciences).") <<
         tr("Film director") << "fmd" << tr("A director responsible for the general management and supervision of a filmed performance.") <<
         tr("Film distributor") << "fds" << tr("A person, family, or organization involved in distributing a moving image resource to theatres or other distribution channels.") <<
         tr("Film editor") << "flm" << tr("Use for a person or organization who is an editor of a motion picture film. This term is used regardless of the medium upon which the motion picture is produced or manufactured (e.g., acetate film, video tape). ") <<
         tr("Film producer") << "fmp" << tr("A producer responsible for most of the business aspects of a film.") <<
         tr("Filmmaker") << "fmk" << tr("A person, family or organization responsible for creating an independent or personal film. A filmmaker is individually responsible for the conception and execution of all aspects of the film.") <<
         tr("First party") << "fpy" << tr("Use for a person or organization who is identified as the only party or the party of the first part. In the case of transfer of right, this is the assignor, transferor, licensor, grantor, etc. Multiple parties can be named jointly as the first party.") <<
         tr("Forger") << "frg" << tr("Use for a person or organization who makes or imitates something of value or importance, especially with the intent to defraud. ") <<
         tr("Former owner") << "fmo" << tr("Use for a person or organization who owned an item at any time in the past. Includes those to whom the material was once presented. A person or organization giving the item to the present owner is designated as Donor [dnr].") <<
         tr("Funder") << "fnd" << tr("Use for a person or organization that furnished financial support for the production of the work.") <<
         tr("Geographic information specialist") << "gis" << tr("Use for a person responsible for geographic information system (GIS) development and integration with global positioning system data.") <<
         tr("Honoree") << "hnr" << tr("Use for a person or organization in memory or honor of whom a book, manuscript, etc. is donated. ") <<
         tr("Host") << "hst" << tr("Use for a person who is invited or regularly leads a program (often broadcast) that includes other guests, performers, etc. (e.g., talk show host).") <<
         tr("Host institution") << "his" << tr("An organization hosting the event, exhibit, conference, etc., which gave rise to a resource, but having little or no responsibility for the content of the resource.") <<
         tr("Illuminator") << "ilu" << tr("Use for a person or organization responsible for the decoration of a work (especially manuscript material) with precious metals or color, usually with elaborate designs and motifs.") <<
         tr("Illustrator") << "ill" << tr("Use for a person or organization who conceives, and perhaps also implements, a design or illustration, usually to accompany a written text.") <<
         tr("Inscriber") << "ins" << tr("Use for a person who has written a statement of dedication or gift.") <<
         tr("Instrumentalist") << "itr" << tr("Use for a person or organization who principally plays an instrument in a musical or dramatic presentation or entertainment.") <<
         tr("Interviewee") << "ive" << tr("Use for a person or organization who is interviewed at a consultation or meeting, usually by a reporter, pollster, or some other information gathering agent.") <<
         tr("Interviewer") << "ivr" << tr("Use for a person or organization who acts as a reporter, pollster, or other information gathering agent in a consultation or meeting involving one or more individuals.") <<
         tr("Inventor") << "inv" << tr("Use for a person or organization who first produces a particular useful item, or develops a new process for obtaining a known item or result.") <<
         tr("Issuing body") << "isb" << tr("A person, family or organization issuing a work, such as an official organ of the body.") <<
         tr("Judge") << "jud" << tr("A person who hears and decides on legal matters in court.") <<
         tr("Jurisdiction governed") << "jug" << tr("A jurisdiction governed by a law, regulation, etc., that was enacted by another jurisdiction.") <<
         tr("Laboratory") << "lbr" << tr("Use for an institution that provides scientific analyses of material samples.") <<
         tr("Laboratory director") << "ldr" << tr("Use for a person or organization that manages or supervises work done in a controlled setting or environment. ") <<
         tr("Landscape architect") << "lsa" << tr("Use for a person or organization whose work involves coordinating the arrangement of existing and proposed land features and structures.") <<
         tr("Lead") << "led" << tr("Use to indicate that a person or organization takes primary responsibility for a particular activity or endeavor. Use with another relator term or code to show the greater importance this person or organization has regarding that particular role. If more than one relator is assigned to a heading, use the Lead relator only if it applies to all the relators.") <<
         tr("Lender") << "len" << tr("Use for a person or organization permitting the temporary use of a book, manuscript, etc., such as for photocopying or microfilming.") <<
         tr("Libelant") << "lil" << tr("Use for the party who files a libel in an ecclesiastical or admiralty case.") <<
         tr("Libelant-appellant") << "lit" << tr("Use for a libelant who takes an appeal from one ecclesiastical court or admiralty to another to reverse the judgment.") <<
         tr("Libelant-appellee") << "lie" << tr("Use for a libelant against whom an appeal is taken from one ecclesiastical court or admiralty to another to reverse the judgment.") <<
         tr("Libelee") << "lel" << tr("Use for a party against whom a libel has been filed in an ecclesiastical court or admiralty.") <<
         tr("Libelee-appellant") << "let" << tr("Use for a libelee who takes an appeal from one ecclesiastical court or admiralty to another to reverse the judgment.") <<
         tr("Libelee-appellee") << "lee" << tr("Use for a libelee against whom an appeal is taken from one ecclesiastical court or admiralty to another to reverse the judgment.") <<
         tr("Librettist") << "lbt" << tr("Use for a person or organization who is a writer of the text of an opera, oratorio, etc.") <<
         tr("Licensee") << "lse" << tr("Use for a person or organization who is an original recipient of the right to print or publish.") <<
         tr("Licensor") << "lso" << tr("Use for person or organization who is a signer of the license, imprimatur, etc. ") <<
         tr("Lighting designer") << "lgd" << tr("Use for a person or organization who designs the lighting scheme for a theatrical presentation, entertainment, motion picture, etc.") <<
         tr("Lithographer") << "ltg" << tr("Use for a person or organization who prepares the stone or plate for lithographic printing, including a graphic artist creating a design directly on the surface from which printing will be done.") <<
         tr("Lyricist") << "lyr" << tr("Use for a person or organization who is the a writer of the text of a song.") <<
         tr("Manufacture place") << "mfp" << tr("The place of manufacture (e.g., printing, duplicating, casting, etc.) of a resource in a published form.") <<
         tr("Manufacturer") << "mfr" << tr("Use for a person or organization that makes an artifactual work (an object made or modified by one or more persons). Examples of artifactual works include vases, cannons or pieces of furniture.") <<
         tr("Marbler") << "mrb" << tr("The entity responsible for marbling paper, cloth, leather, etc. used in construction of a resource.") <<
         tr("Markup editor") << "mrk" << tr("Use for a person or organization performing the coding of SGML, HTML, or XML markup of metadata, text, etc.") <<
         tr("Medium") << "med" << tr("A person held to be a channel of communication between the earthly world and a different world.") <<
         tr("Metadata contact") << "mdc" << tr("Use for a person or organization primarily responsible for compiling and maintaining the original description of a metadata set (e.g., geospatial metadata set).") <<
         tr("Metal-engraver") << "mte" << tr("Use for a person or organization responsible for decorations, illustrations, letters, etc. cut on a metal surface for printing or decoration.") <<
         tr("Minute taker") << "mtk" << tr("A person, family, or organization responsible for recording the minutes of a meeting.") <<
         tr("Moderator") << "mod" << tr("Use for a person who leads a program (often broadcast) where topics are discussed, usually with participation of experts in fields related to the discussion.") <<
         tr("Monitor") << "mon" << tr("Use for a person or organization that supervises compliance with the contract and is responsible for the report and controls its distribution. Sometimes referred to as the grantee, or controlling agency.") <<
         tr("Music copyist") << "mcp" << tr("Use for a person who transcribes or copies musical notation") <<
         tr("Musical director") << "msd" << tr("Use for a person responsible for basic music decisions about a production, including coordinating the work of the composer, the sound editor, and sound mixers, selecting musicians, and organizing and/or conducting sound for rehearsals and performances.") <<
         tr("Musician") << "mus" << tr("Use for a person or organization who performs music or contributes to the musical content of a work when it is not possible or desirable to identify the function more precisely.") <<
         tr("Narrator") << "nrt" << tr("Use for a person who is a speaker relating the particulars of an act, occurrence, or course of events.") <<
         tr("Onscreen presenter") << "osp" << tr("A performer contributing to an expression of a work by appearing on screen in nonfiction moving image materials or introductions to fiction moving image materials to provide contextual or background information. Use when another term (e.g., Narrator, Host) is either not applicable or not desired.") <<
         tr("Opponent") << "opn" << tr("Use for a person or organization responsible for opposing a thesis or dissertation.") <<
         tr("Organizer of meeting") << "orm" << tr("Use for a person or organization responsible for organizing a meeting for which an item is the report or proceedings.") <<
         tr("Originator") << "org" << tr("Use for a person or organization performing the work, i.e., the name of a person or organization associated with the intellectual content of the work. This category does not include the publisher or personal affiliation, or sponsor except where it is also the corporate author.") <<
         tr("Other") << "oth" << tr("Use for relator codes from other lists which have no equivalent in the MARC list or for terms which have not been assigned a code.") <<
         tr("Owner") << "own" << tr("Use for a person or organization that currently owns an item or collection.") <<
         tr("Panelist") << "pan" << tr(" performer contributing to a resource by participating in a program (often broadcast) where topics are discussed, usually with participation of experts in fields related to the discussion.") <<
         tr("Papermaker") << "ppm" << tr("Use for a person or organization responsible for the production of paper, usually from wood, cloth, or other fibrous material.") <<
         tr("Patent applicant") << "pta" << tr("Use for a person or organization that applied for a patent.") <<
         tr("Patent holder") << "pth" << tr("Use for a person or organization that was granted the patent referred to by the item. ") <<
         tr("Patron") << "pat" << tr("Use for a person or organization responsible for commissioning a work. Usually a patron uses his or her means or influence to support the work of artists, writers, etc. This includes those who commission and pay for individual works.") <<
         tr("Performer") << "prf" << tr("Use for a person or organization who exhibits musical or acting skills in a musical or dramatic presentation or entertainment, if specific codes for those functions ([act], [dnc], [itr], [voc], etc.) are not used. If specific codes are used, [prf] is used for a person whose principal skill is not known or specified.") <<
         tr("Permitting agency") << "pma" << tr("Use for an authority (usually a government agency) that issues permits under which work is accomplished.") <<
         tr("Photographer") << "pht" << tr("Use for a person or organization responsible for taking photographs, whether they are used in their original form or as reproductions.") <<
         tr("Plaintiff") << "ptf" << tr("Use for the party who complains or sues in court in a personal action, usually in a legal proceeding.") <<
         tr("Plaintiff-appellant") << "ptt" << tr("Use for a plaintiff who takes an appeal from one court or jurisdiction to another to reverse the judgment, usually in a legal proceeding.") <<
         tr("Plaintiff-appellee") << "pte" << tr("Use for a plaintiff against whom an appeal is taken from one court or jurisdiction to another to reverse the judgment, usually in a legal proceeding.") <<
         tr("Platemaker") << "plt" << tr("Use for a person or organization responsible for the production of plates, usually for the production of printed images and/or text.") <<
         tr("Praeses") << "pra" << tr("A person who is the faculty moderator of an academic disputation, normally proposing a thesis and participating in the ensuing disputation.") <<
         tr("Presenter") << "pre" << tr("A person or organization mentioned in an 'X presents' credit for moving image materials and who is associated with production, finance, or distribution in some way. A vanity credit; in early years, normally the head of a studio.") <<
         tr("Printer") << "prt" << tr("Use for a person or organization who prints texts, whether from type or plates.") <<
         tr("Printer of plates") << "pop" << tr("Use for a person or organization who prints illustrations from plates. ") <<
         tr("Printmaker") << "prm" << tr("Use for a person or organization who makes a relief, intaglio, or planographic printing surface.") <<
         tr("Process contact") << "prc" << tr("Use for a person or organization primarily responsible for performing or initiating a process, such as is done with the collection of metadata sets.") <<
         tr("Producer") << "pro" << tr("Use for a person or organization responsible for the making of a motion picture, including business aspects, management of the productions, and the commercial success of the work.") <<
         tr("Production company") << "prn" << tr("An organization that is responsible for financial, technical, and organizational management of a production for stage, screen, audio recording, television, webcast, etc.") <<
         tr("Production designer") << "prs" << tr("A person or organization responsible for designing the overall visual appearance of a moving image production.") <<
         tr("Production manager") << "pmn" << tr("Use for a person responsible for all technical and business matters in a production.") <<
         tr("Production personnel") << "prd" << tr("Use for a person or organization associated with the production (props, lighting, special effects, etc.) of a musical or dramatic presentation or entertainment.") <<
         tr("Production place") << "prp" << tr("The place of production (e.g., inscription, fabrication, construction, etc.) of a resource in an unpublished form.") <<
         tr("Programmer") << "prg" << tr("Use for a person or organization responsible for the creation and/or maintenance of computer program design documents, source code, and machine-executable digital files and supporting documentation.") <<
         tr("Project director") << "pdr" << tr("Use for a person or organization with primary responsibility for all essential aspects of a project, or that manages a very large project that demands senior level responsibility, or that has overall responsibility for managing projects, or provides overall direction to a project manager.") <<
         tr("Proofreader") << "pfr" << tr("Use for a person who corrects printed matter. For manuscripts, use Corrector [crr].") <<
         tr("Provider") << "prv" << tr("A person or organization who produces, publishes, manufactures, or distributes a resource if specific codes are not desired (e.g. [mfr], [pbl].)") <<
         tr("Publication place ") << "pup" << tr("The place where a resource is published.") <<
         tr("Publisher") << "pbl" << tr("Use for a person or organization that makes printed matter, often text, but also printed music, artwork, etc. available to the public.") <<
         tr("Publishing director") << "pbd" << tr("Use for a person or organization who presides over the elaboration of a collective work to ensure its coherence or continuity. This includes editors-in-chief, literary editors, editors of series, etc.") <<
         tr("Puppeteer") << "ppt" << tr("Use for a person or organization who manipulates, controls, or directs puppets or marionettes in a musical or dramatic presentation or entertainment.") <<
         tr("Radio director") << "rdd" << tr("A director responsible for the general management and supervision of a radio program.") <<
         tr("Radio producer") << "rpc" << tr("A producer responsible for most of the business aspects of a radio program.") <<
         tr("Recipient") << "rcp" << tr("Use for a person or organization to whom correspondence is addressed.") <<
         tr("Recording engineer") << "rce" << tr("Use for a person or organization who supervises the technical aspects of a sound or video recording session.") <<
         tr("Recording engineer") << "rce" << tr("A person contributing to a resource by supervising the technical aspects of a sound or video recording session.") <<
         tr("Redactor") << "red" << tr("Use for a person or organization who writes or develops the framework for an item without being intellectually responsible for its content.") <<
         tr("Renderer") << "ren" << tr("Use for a person or organization who prepares drawings of architectural designs (i.e., renderings) in accurate, representational perspective to show what the project will look like when completed.") <<
         tr("Reporter") << "rpt" << tr("Use for a person or organization who writes or presents reports of news or current events on air or in print.") <<
         tr("Repository") << "rps" << tr("Use for an agency that hosts data or material culture objects and provides services to promote long term, consistent and shared use of those data or objects.") <<
         tr("Research team head") << "rth" << tr("Use for a person who directed or managed a research project.") <<
         tr("Research team member") << "rtm" << tr("Use for a person who participated in a research project but whose role did not involve direction or management of it.") <<
         tr("Researcher") << "res" << tr("Use for a person or organization responsible for performing research. ") <<
         tr("Respondent") << "rsp" << tr("Use for the party who makes an answer to the courts pursuant to an application for redress, usually in an equity proceeding.") <<
         tr("Respondent-appellant") << "rst" << tr("Use for a respondent who takes an appeal from one court or jurisdiction to another to reverse the judgment, usually in an equity proceeding.") <<
         tr("Respondent-appellee") << "rse" << tr("Use for a respondent against whom an appeal is taken from one court or jurisdiction to another to reverse the judgment, usually in an equity proceeding.") <<
         tr("Responsible party") << "rpy" << tr("Use for a person or organization legally responsible for the content of the published material.") <<
         tr("Restager") << "rsg" << tr("Use for a person or organization, other than the original choreographer or director, responsible for restaging a choreographic or dramatic work and who contributes minimal new content.") <<
         tr("Restorationist") << "rsr" << tr("A person, family, or organization responsible for the set of technical, editorial, and intellectual procedures aimed at compensating for the degradation of an item by bringing it back to a state as close as possible to its original condition.") <<
         tr("Reviewer") << "rev" << tr("Use for a person or organization responsible for the review of a book, motion picture, performance, etc.") <<
         tr("Rubricator") << "rbr" << tr("Use for a person or organization responsible for parts of a work, often headings or opening parts of a manuscript, that appear in a distinctive color, usually red.") <<
         tr("Scenarist") << "sce" << tr("Use for a person or organization who is the author of a motion picture screenplay.") <<
         tr("Scientific advisor") << "sad" << tr("Use for a person or organization who brings scientific, pedagogical, or historical competence to the conception and realization on a work, particularly in the case of audio-visual items.") <<
         tr("Scribe") << "scr" << tr("Use for a person who is an amanuensis and for a writer of manuscripts proper. For a person who makes pen-facsimiles, use Facsimilist [fac].") <<
         tr("Sculptor") << "scl" << tr("Use for a person or organization who models or carves figures that are three-dimensional representations.") <<
         tr("Second party") << "spy" << tr("Use for a person or organization who is identified as the party of the second part. In the case of transfer of right, this is the assignee, transferee, licensee, grantee, etc. Multiple parties can be named jointly as the second party.") <<
         tr("Secretary") << "sec" << tr("Use for a person or organization who is a recorder, redactor, or other person responsible for expressing the views of a organization.") <<
         tr("Seller") << "sll" << tr("A former owner of an item who sold that item to another owner.") <<
         tr("Set designer") << "std" << tr("Use for a person or organization who translates the rough sketches of the art director into actual architectural structures for a theatrical presentation, entertainment, motion picture, etc. Set designers draw the detailed guides and specifications for building the set.") <<
         tr("Setting") << "stg" << tr("An entity in which the activity or plot of a work takes place, e.g. a geographic place, a time period, a building, an event.") <<
         tr("Signer") << "sgn" << tr("Use for a person whose signature appears without a presentation or other statement indicative of provenance. When there is a presentation statement, use Inscriber [ins].") <<
         tr("Singer") << "sng" << tr("Use for a person or organization who uses his/her/their voice with or without instrumental accompaniment to produce music. A performance may or may not include actual words.") <<
         tr("Sound designer") << "sds" << tr("Use for a person who produces and reproduces the sound score (both live and recorded), the installation of microphones, the setting of sound levels, and the coordination of sources of sound for a production.") <<
         tr("Speaker") << "spk" << tr("Use for a person who participates in a program (often broadcast) and makes a formalized contribution or presentation generally prepared in advance.") <<
         tr("Sponsor") << "spn" << tr("Use for a person or organization that issued a contract or under the auspices of which a work has been written, printed, published, etc.") <<
         tr("Stage director") << "sgd" << tr("A person or organization contributing to a stage resource through the overall management and supervision of a performance.") <<
         tr("Stage manager") << "stm" << tr("Use for a person who is in charge of everything that occurs on a performance stage, and who acts as chief of all crews and assistant to a director during rehearsals.") <<
         tr("Standards body") << "stn" << tr("Use for an organization responsible for the development or enforcement of a standard.") <<
         tr("Stereotyper") << "str" << tr("Use for a person or organization who creates a new plate for printing by molding or copying another printing surface.") <<
         tr("Storyteller") << "stl" << tr("Use for a person relaying a story with creative and/or theatrical interpretation.") <<
         tr("Supporting host") << "sht" << tr("Use for a person or organization that supports (by allocating facilities, staff, or other resources) a project, program, meeting, event, data objects, material culture objects, or other entities capable of support. ") <<
         tr("Surveyor") << "srv" << tr("Use for a person or organization who does measurements of tracts of land, etc. to determine location, forms, and boundaries.") <<
         tr("Teacher") << "tch" << tr("Use for a person who, in the context of a resource, gives instruction in an intellectual subject or demonstrates while teaching physical skills. ") <<
         tr("Technical director") << "tcd" << tr("Use for a person who is ultimately in charge of scenery, props, lights and sound for a production.") <<
         tr("Television director") << "tld" << tr("A director responsible for the general management and supervision of a television program.") <<
         tr("Television producer") << "tlp" << tr("A producer responsible for most of the business aspects of a television program.") <<
         tr("Thesis advisor") << "ths" << tr("Use for a person under whose supervision a degree candidate develops and presents a thesis, mémoire, or text of a dissertation. ") <<
         tr("Transcriber") << "trc" << tr("Use for a person who prepares a handwritten or typewritten copy from original material, including from dictated or orally recorded material. For makers of pen-facsimiles, use Facsimilist [fac].") <<
         tr("Translator") << "trl" << tr("Use for a person or organization who renders a text from one language into another, or from an older form of a language into the modern form.") <<
         tr("Type designer") << "tyd" << tr("Use for a person or organization who designed the type face used in a particular item. ") <<
         tr("Typographer") << "tyg" << tr("Use for a person or organization primarily responsible for choice and arrangement of type used in an item. If the typographer is also responsible for other aspects of the graphic design of a book (e.g., Book designer [bkd]), codes for both functions may be needed.") <<
         tr("University place") << "uvp" << tr("A place where a university that is associated with a resource is located, for example, a university where an academic dissertation or thesis was presented.") <<
         tr("Videographer") << "vdg" << tr("Use for a person or organization in charge of a video production, e.g. the video recording of a stage production as opposed to a commercial motion picture. The videographer may be the camera operator or may supervise one or more camera operators. Do not confuse with cinematographer.") <<
         tr("Voice actor") << "vac" << tr("An actor contributing to a resource by providing the voice for characters in radio and audio productions and for animated characters in moving image works, as well as by providing voice overs in radio and television commercials, dubbed resources, etc.") <<
         tr("Witness") << "wit" << tr("Use for a person who verifies the truthfulness of an event or action. ") <<
         tr("Wood-engraver") << "wde" << tr("Use for a person or organization who makes prints by cutting the image in relief on the end-grain of a wood block.") <<
         tr("Woodcutter") << "wdc" << tr("Use for a person or organization who makes prints by cutting the image in relief on the plank side of a wood block.") <<
         tr("Writer of accompanying material") << "wam" << tr("Use for a person or organization who writes significant material which accompanies a sound recording or other audiovisual material.") <<
         tr("Writer of added commentary") << "wac" << tr("A person, family, or organization contributing to an expression of a work by providing an interpretation or critical explanation of the original work.") <<
         tr("Writer of added lyrics") << "wal" << tr("A writer of words added to an expression of a musical work. For lyric writing in collaboration with a composer to form an original work, see lyricist.") <<
         tr("Writer of added text") << "wat" << tr("A person, family, or organization contributing to a non-textual resource by providing text for the non-textual work (e.g., writing captions for photographs, descriptions of maps.)") <<
         tr("Writer of introduction") << "win" << tr("A person, family, or organization contributing to a resource by providing an introduction to the original work.") <<
         tr("Writer of preface") << "wpr" << tr("A person, family, or organization contributing to a resource by providing a preface to the original work.") <<
         tr("Writer of supplementary textual content") << "wst" << tr("A person, family, or organization contributing to a resource by providing supplementary textual content (e.g., an introduction, a preface) to the original work.");

    for (int i = 0; i < data.count(); i++) {
        QString name = data.at(i++);
        QString code = data.at(i++);
        QString description = data.at(i);
        DescriptiveInfo rel;
        rel.name = name;
        rel.description  = description;
        m_CodeMap.insert(code, rel);
        m_NameMap.insert(name, code);
    }
}
