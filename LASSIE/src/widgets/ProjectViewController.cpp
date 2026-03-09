/* The 'project' object that keeps track of project details to deploy
 * in the associated window (currently, the project view).
 *
 * Copyright (c) 2025, DISSCO authors */

#include <QFile>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QTextStream>
#include <QDomDocument>

#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>
#include <QScrollArea>
#include <QTreeView>
#include <QStandardItem>

#include "ProjectViewController.hpp"
#include "../windows/MainWindow.hpp"
#include "../ui/ui_mainwindow.h"
#include "../inst.hpp"
#include "../core/EnvelopeLibraryEntry.hpp"
#include "../dialogs/ProjectPropertiesDialog.hpp"
#include "../ui/ui_ProjectPropertiesDialog.h"
#include "../dialogs/FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"
#include "PaletteViewController.hpp"
#include "../core/event_struct.hpp"
#include "EventAttributesViewController.hpp"
#include "../dialogs/FileNewObject.hpp"
#include "../ui/ui_FileNewObject.h"
// #include "../core/IEvent.h"
// #include "muParser.h"

#include "../utilities.hpp"
#include "../core/xmlwriter.hpp"

namespace PVCHelper {
    QList<QStandardItem*> make_child_palette_tuple(const QString& type, const QString& name) {
        auto* typeItem = new QStandardItem(type);
        auto* nameItem = new QStandardItem(name);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        typeItem->setData(type, Qt::UserRole + 1);
        typeItem->setData(name, Qt::UserRole + 2);
        nameItem->setData(type, Qt::UserRole + 1);
        nameItem->setData(name, Qt::UserRole + 2);
        return {typeItem, nameItem};
    }
}
/* ProjectView constructor initializing values for XML file*/
ProjectView::ProjectView(MainWindow* _mainWindow, QString _pathAndName) {

    ProjectManager *pm = Inst::get_project_manager();
    qDebug() << "In PV Constructor p:" << pm->get_curr_project();

    mainWindow = _mainWindow;
    modifiedButNotSaved = true;

    // // Initialize PaletteViewController
    paletteView = new PaletteViewController(this);
    mainWindow->ui->paletteLayout->insertWidget(0, paletteView);

    // Initialize Events Attribute View Controller
    mainWindow->ui->eventsScrollArea->setWidgetResizable(true);
    eventAttributesView = new EventAttributesViewController(this);
    mainWindow->ui->eventsScrollArea->setWidget(eventAttributesView);

    updatePaletteView();
}

/* Function that creates and saves the xml .dissco file */
void ProjectView::save(){
    qDebug() << "In Project View Save Function";

    eventAttributesView->saveCurrentShownEventData();

    ProjectManager *pm = Inst::get_project_manager();
    modifiedButNotSaved = false; // changes bool value because file is being saved

    // ensure directory exists before creating file
    QFileInfo fileInfo = pm->fileinfo();
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create directory:" << dir.absolutePath();
            return;
        }
    }

    // creates the file with the specified /path/name.dissco
    QFile file(pm->fileinfo().absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.fileName();
        qDebug() << "Error reason:" << file.errorString();
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    XmlWriter::writeProject(xmlWriter, pm, default_note_modifiers);
    file.close();
}

void ProjectView::setProperties() {
    if (!projectPropertiesDialog) {
        projectPropertiesDialog = new ProjectPropertiesDialog(mainWindow);
        connect(projectPropertiesDialog->ui->insertFunctionButton, &QPushButton::clicked, this, &ProjectView::propertiesInsertFunction);
    }

    ProjectManager *pm = Inst::get_project_manager();

    projectPropertiesDialog->ui->titleEntry->setText(pm->title());
    projectPropertiesDialog->ui->flagEntry->setText(pm->fileflag());
    projectPropertiesDialog->ui->numChannelsEntry->setText(pm->numchannels());
    projectPropertiesDialog->ui->rateEntry->setText(pm->samplerate());
    projectPropertiesDialog->ui->sizeEntry->setText(pm->samplesize());
    projectPropertiesDialog->ui->numThreadsEntry->setText(pm->numthreads());
    projectPropertiesDialog->ui->synthesisCheckBox->setCheckState(pm->synthesis() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->scoreCheckBox->setCheckState(pm->score() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->staffCheckBox->setCheckState(pm->grandstaff() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->numStaffEntry->setText(pm->numstaffs());
    projectPropertiesDialog->ui->particelBox->setCheckState(pm->outputparticel() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->topEventEntry->setText(pm->topevent().name);
    projectPropertiesDialog->ui->topEventEntry->setEnabled(false);
    projectPropertiesDialog->ui->durationEntry->setText(pm->duration());

 
    if (projectPropertiesDialog->exec() == QDialog::Accepted) {
        QString new_title = projectPropertiesDialog->ui->titleEntry->text();
        pm->fileflag() = projectPropertiesDialog->ui->flagEntry->text();
        pm->numchannels() = projectPropertiesDialog->ui->numChannelsEntry->text();
        pm->samplerate() = projectPropertiesDialog->ui->rateEntry->text();
        pm->samplesize() = projectPropertiesDialog->ui->sizeEntry->text();
        pm->numthreads() = projectPropertiesDialog->ui->numThreadsEntry->text();
        pm->synthesis() = projectPropertiesDialog->ui->synthesisCheckBox->isChecked();
        pm->score() = projectPropertiesDialog->ui->scoreCheckBox->isChecked();
        pm->grandstaff() = projectPropertiesDialog->ui->staffCheckBox->isChecked();
        pm->numstaffs() = projectPropertiesDialog->ui->numStaffEntry->text();
        pm->outputparticel() = projectPropertiesDialog->ui->particelBox->isChecked();
        QString new_topevent = projectPropertiesDialog->ui->topEventEntry->text();
        pm->duration() = projectPropertiesDialog->ui->durationEntry->text();

        if (new_title != pm->title()) {
            QString old_pathAndName = pm->fileinfo().absoluteFilePath();
            QString new_pathAndName = pm->fileinfo().absolutePath() + "/" + new_title + ".dissco";
            if (QFile::rename(old_pathAndName, new_pathAndName)) {
                pm->title() = new_title;
            }
        }

        MUtilities::modified();
        delete projectPropertiesDialog;
        projectPropertiesDialog = NULL;
    }
}

void ProjectView::propertiesInsertFunction() {
    if (!projectPropertiesDialog) return;
    ProjectManager *pm = Inst::get_project_manager();
    auto* generator = new FunctionGenerator(mainWindow, functionReturnFloat, pm->duration());
    if (generator->exec() == QDialog::Accepted) {
        QString result = generator->getResultString();
        if (!result.isEmpty()) {
            pm->duration() = result;
            projectPropertiesDialog->ui->durationEntry->setText(pm->duration());
        }
    }
    delete generator;
}

void ProjectView::insertObject() {
    if (!newObject)
        newObject = new FileNewObject(mainWindow);

    if (newObject->exec() == QDialog::Accepted) {
        ProjectManager *pm = Inst::get_project_manager();
        QString nameStr = newObject->ui->objNameEntry->text();
        QString typeStr;
        QStandardItem* folder = nullptr;

        // Appends a new HEvent (High/Mid/Low) and sets typeStr/folder
        auto addHEvent = [&](QList<HEvent>& list, Eventtype t, const QString& ts, QStandardItem* f) {
            HEvent obj = {};
            obj.type = t;
            obj.name = nameStr;
            list.push_back(obj);
            typeStr = ts;
            folder = f;
        };

        // Appends a new simple event (with .orderinpalette/.name) and sets typeStr/folder
        auto addSimple = [&](auto& list, const QString& ts, QStandardItem* f) {
            using T = typename std::decay_t<decltype(list)>::value_type;
            T obj = {};
            obj.orderinpalette = QString::number(list.size() + 1);
            obj.name = nameStr;
            list.push_back(obj);
            typeStr = ts;
            folder = f;
        };

        if      (newObject->ui->buttonHigh->isChecked())
            addHEvent(pm->highevents(), high, "High", paletteView->folderHigh);
        else if (newObject->ui->buttonMid->isChecked())
            addHEvent(pm->midevents(),  mid,  "Mid",  paletteView->folderMid);
        else if (newObject->ui->buttonLow->isChecked())
            addHEvent(pm->lowevents(),  low,  "Low",  paletteView->folderLow);
        else if (newObject->ui->buttonBottom->isChecked()) {
            BottomEvent obj = {};
            obj.event.type = bottom;
            obj.event.name = nameStr;
            pm->bottomevents().push_back(obj);
            typeStr = "Bottom";
            folder = paletteView->folderBottom;
        }
        else if (newObject->ui->buttonSpectrum->isChecked())
            addSimple(pm->spectrumevents(),  "Spectrum",       paletteView->folderSpectrum);
        else if (newObject->ui->buttonNote->isChecked())
            addSimple(pm->noteevents(),      "Note",           paletteView->folderNote);
        else if (newObject->ui->buttonEnv->isChecked())
            addSimple(pm->envelopeevents(),  "Envelope",       paletteView->folderEnv);
        else if (newObject->ui->buttonSiv->isChecked())
            addSimple(pm->sieveevents(),     "Sieve",          paletteView->folderSiv);
        else if (newObject->ui->buttonSpa->isChecked())
            addSimple(pm->spaevents(),       "Spatialization", paletteView->folderSpa);
        else if (newObject->ui->buttonPat->isChecked())
            addSimple(pm->patternevents(),   "Pattern",        paletteView->folderPat);
        else if (newObject->ui->buttonRev->isChecked())
            addSimple(pm->reverbevents(),    "Reverb",         paletteView->folderRev);
        else if (newObject->ui->buttonFil->isChecked())
            addSimple(pm->filterevents(),    "Filter",         paletteView->folderFil);

        if (!folder) {
            qWarning() << "insertObject: No folder found for type" << typeStr;
            delete newObject;
            newObject = nullptr;
            return;
        }

        // Create QStandardItems with proper user roles
        folder->appendRow(PVCHelper::make_child_palette_tuple(typeStr, nameStr));

        // Commented out because no structs for mea event yet
        /* else if (newObject->ui->buttonMea->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Measurement");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderMea->appendRow({newObjectType, newObjectName});
        } */
        
        modifiedButNotSaved = true;
        MUtilities::modified();
        delete newObject;
        newObject = nullptr;
    }
}

static Eventtype eventtypeFromString(const QString& s) {
    if (s == "High")          return high;
    if (s == "Mid")           return mid;
    if (s == "Low")           return low;
    if (s == "Bottom")        return bottom;
    if (s == "Spectrum")      return sound;
    if (s == "Note")          return note;
    if (s == "Envelope")      return env;
    if (s == "Sieve")         return sieve;
    if (s == "Spatialization") return spa;
    if (s == "Pattern")       return pattern;
    if (s == "Reverb")        return reverb;
    if (s == "Filter")        return filter;
    if (s == "Measurement")   return mea;
    return top; // fallback (should not happen for deletable types)
}

void ProjectView::updatePaletteView() {
    ProjectManager *pm = Inst::get_project_manager();

    auto makeItems = [](const QString& typeStr, const QString& nameStr,
                        QStandardItem*& outType, QStandardItem*& outName) {
        outType = new QStandardItem(typeStr);
        outName = new QStandardItem(nameStr);
        outType->setFlags(outType->flags() & ~Qt::ItemIsEditable);
        outType->setData(typeStr, Qt::UserRole + 1);
        outType->setData(nameStr, Qt::UserRole + 2);
        outName->setData(typeStr, Qt::UserRole + 1);
        outName->setData(nameStr, Qt::UserRole + 2);
    };

    // Top event is singular — add directly without a loop
    {
        QStandardItem *t, *n;
        makeItems("Top", pm->topevent().name, t, n);
        t->setFlags(t->flags() & ~Qt::ItemIsDragEnabled);
        n->setFlags(n->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
        paletteView->folderTop->appendRow({t, n});
    }

    for (const HEvent& item : pm->highevents()) {
        QStandardItem *t, *n;
        makeItems("High", item.name, t, n);
        paletteView->folderHigh->appendRow({t, n});
    }

    for (const HEvent& item : pm->midevents()) {
        QStandardItem *t, *n;
        makeItems("Mid", item.name, t, n);
        paletteView->folderMid->appendRow({t, n});
    }

    for (const HEvent& item : pm->lowevents()) {
        QStandardItem *t, *n;
        makeItems("Low", item.name, t, n);
        paletteView->folderLow->appendRow({t, n});
    }

    for (const BottomEvent& item : pm->bottomevents()) {
        QStandardItem *t, *n;
        makeItems("Bottom", item.event.name, t, n);
        paletteView->folderBottom->appendRow({t, n});
    }

    for (const SpectrumEvent& item : pm->spectrumevents()) {
        QStandardItem *t, *n;
        makeItems("Spectrum", item.name, t, n);
        paletteView->folderSpectrum->appendRow({t, n});
    }

    for (const NoteEvent& item : pm->noteevents()) {
        QStandardItem *t, *n;
        makeItems("Note", item.name, t, n);
        paletteView->folderNote->appendRow({t, n});
    }

    for (const EnvelopeEvent& item : pm->envelopeevents()) {
        QStandardItem *t, *n;
        makeItems("Envelope", item.name, t, n);
        paletteView->folderEnv->appendRow({t, n});
    }

    for (const SieveEvent& item : pm->sieveevents()) {
        QStandardItem *t, *n;
        makeItems("Sieve", item.name, t, n);
        paletteView->folderSiv->appendRow({t, n});
    }

    for (const SpaEvent& item : pm->spaevents()) {
        QStandardItem *t, *n;
        makeItems("Spatialization", item.name, t, n);
        paletteView->folderSpa->appendRow({t, n});
    }

    for (const PatternEvent& item : pm->patternevents()) {
        QStandardItem *t, *n;
        makeItems("Pattern", item.name, t, n);
        paletteView->folderPat->appendRow({t, n});
    }

    for (const ReverbEvent& item : pm->reverbevents()) {
        QStandardItem *t, *n;
        makeItems("Reverb", item.name, t, n);
        paletteView->folderRev->appendRow({t, n});
    }

    for (const FilterEvent& item : pm->filterevents()) {
        QStandardItem *t, *n;
        makeItems("Filter", item.name, t, n);
        paletteView->folderFil->appendRow({t, n});
    }
}

void ProjectView::showAttributes(QString eventType, int index) {
    qDebug() << "Showing attributes for event:" << eventType << "at index" << index;

    Eventtype type = eventtypeFromString(eventType);
    if (type == top)
        eventAttributesView->showAttributesOfEvent(top, 0);
    else
        eventAttributesView->showAttributesOfEvent(type, index);

    mainWindow->ui->eventsScrollArea->widget()->adjustSize();
    mainWindow->ui->eventsScrollArea->updateGeometry();
}

void ProjectView::deleteEvent(const QString& typeStr, int index)
{
    ProjectManager* pm = Inst::get_project_manager();
    Eventtype etype = eventtypeFromString(typeStr);

    // Notify the attributes view before touching the backend
    eventAttributesView->onEventDeleted(etype, index);

    // Remove from the backend list
    if      (etype == high)    pm->highevents().removeAt(index);
    else if (etype == mid)     pm->midevents().removeAt(index);
    else if (etype == low)     pm->lowevents().removeAt(index);
    else if (etype == bottom)  pm->bottomevents().removeAt(index);
    else if (etype == sound)   pm->spectrumevents().removeAt(index);
    else if (etype == note)    pm->noteevents().removeAt(index);
    else if (etype == env)     pm->envelopeevents().removeAt(index);
    else if (etype == sieve)   pm->sieveevents().removeAt(index);
    else if (etype == spa)     pm->spaevents().removeAt(index);
    else if (etype == pattern) pm->patternevents().removeAt(index);
    else if (etype == reverb)  pm->reverbevents().removeAt(index);
    else if (etype == filter)  pm->filterevents().removeAt(index);

    // Remove from the palette model
    QStandardItem* folder = paletteView->folderForType(typeStr);
    if (folder) folder->removeRow(index);
}

void ProjectView::duplicateEvent(const QString& typeStr, int index)
{
    ProjectManager* pm = Inst::get_project_manager();

    QStandardItem* folder = paletteView->folderForType(typeStr);
    if (!folder) return;

    // Copies element at index, appends " (copy)" to .name, appends to list, returns new name
    auto dup = [index](auto& list) -> QString {
        auto copy = list[index];
        copy.name += " (copy)";
        list.append(copy);
        return copy.name;
    };

    Eventtype etype = eventtypeFromString(typeStr);
    QString newName;
    if      (etype == high)    newName = dup(pm->highevents());
    else if (etype == mid)     newName = dup(pm->midevents());
    else if (etype == low)     newName = dup(pm->lowevents());
    else if (etype == bottom) {
        BottomEvent copy = pm->bottomevents()[index];
        copy.event.name += " (copy)";
        newName = copy.event.name;
        pm->bottomevents().append(copy);
    }
    else if (etype == sound)   newName = dup(pm->spectrumevents());
    else if (etype == note)    newName = dup(pm->noteevents());
    else if (etype == env)     newName = dup(pm->envelopeevents());
    else if (etype == sieve)   newName = dup(pm->sieveevents());
    else if (etype == spa)     newName = dup(pm->spaevents());
    else if (etype == pattern) newName = dup(pm->patternevents());
    else if (etype == reverb)  newName = dup(pm->reverbevents());
    else if (etype == filter)  newName = dup(pm->filterevents());
    else return;

    folder->appendRow(PVCHelper::make_child_palette_tuple(typeStr, newName));
}

void ProjectView::updatePaletteItemName(const QString& typeStr, int index, const QString& name)
{
    if (paletteView) paletteView->updateItemName(typeStr, index, name);
}

void ProjectView::updateAttributesNameEntry(const QString& typeStr, int index, const QString& name)
{
    if (eventAttributesView) eventAttributesView->updateNameEntryIfShowing(typeStr, index, name);
}
