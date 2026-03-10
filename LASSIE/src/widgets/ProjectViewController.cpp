/* The 'project' object that keeps track of project details to deploy
 * in the associated window (currently, the project view).
 *
 * Copyright (c) 2025, DISSCO authors */

#include <QFile>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QDebug>

#include "ProjectViewController.hpp"
#include "../windows/MainWindow.hpp"
#include "../ui/ui_mainwindow.h"
#include "../inst.hpp"
#include "../dialogs/ProjectPropertiesDialog.hpp"
#include "../ui/ui_ProjectPropertiesDialog.h"
#include "../dialogs/FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"
#include "PaletteViewController.hpp"
#include "../core/event_struct.hpp"
#include "EventAttributesViewController.hpp"
#include "../dialogs/FileNewObject.hpp"
#include "../ui/ui_FileNewObject.h"

#include "../utilities.hpp"
#include "../core/xmlwriter.hpp"

/* ProjectView constructor initializing values for XML file*/
ProjectView::ProjectView(MainWindow* _mainWindow, QString _pathAndName) {

    const ProjectManager *pm = Inst::get_project_manager();
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

    paletteView->populate();
}

/* Function that creates and saves the xml .dissco file */
void ProjectView::save(){
    qDebug() << "In Project View Save Function";

    eventAttributesView->saveCurrentShownEventData();

    ProjectManager *pm = Inst::get_project_manager();
    modifiedButNotSaved = false; // changes bool value because file is being saved

    // ensure directory exists before creating file
    const QFileInfo fileInfo = pm->fileinfo();
    if (const QDir dir = fileInfo.absoluteDir(); !dir.exists()) {
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

    const ProjectManager *pm = Inst::get_project_manager();

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
        const QString new_title = projectPropertiesDialog->ui->titleEntry->text();
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
        projectPropertiesDialog = nullptr;
    }
}

void ProjectView::propertiesInsertFunction() const {
    if (!projectPropertiesDialog) return;
    ProjectManager *pm = Inst::get_project_manager();
    auto* generator = new FunctionGenerator(mainWindow, functionReturnFloat, pm->duration());
    if (generator->exec() == QDialog::Accepted) {
        if (const QString result = generator->getResultString(); !result.isEmpty()) {
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
        const QString nameStr = newObject->ui->objNameEntry->text();
        Eventtype etype;

        if      (newObject->ui->buttonHigh->isChecked())     etype = high;
        else if (newObject->ui->buttonMid->isChecked())      etype = mid;
        else if (newObject->ui->buttonLow->isChecked())      etype = low;
        else if (newObject->ui->buttonBottom->isChecked())   etype = bottom;
        else if (newObject->ui->buttonSpectrum->isChecked()) etype = sound;
        else if (newObject->ui->buttonNote->isChecked())     etype = note;
        else if (newObject->ui->buttonEnv->isChecked())      etype = env;
        else if (newObject->ui->buttonSiv->isChecked())      etype = sieve;
        else if (newObject->ui->buttonSpa->isChecked())      etype = spa;
        else if (newObject->ui->buttonPat->isChecked())      etype = pattern;
        else if (newObject->ui->buttonRev->isChecked())      etype = reverb;
        else if (newObject->ui->buttonFil->isChecked())      etype = filter;
        else {
            delete newObject;
            newObject = nullptr;
            return;
        }

        pm->addEvent(etype, nameStr);
        paletteView->appendEventRow(etype, nameStr);

        modifiedButNotSaved = true;
        MUtilities::modified();
        delete newObject;
        newObject = nullptr;
    }
}


void ProjectView::showAttributes(const QString &eventType, const int index) const {
    qDebug() << "Showing attributes for event:" << eventType << "at index" << index;

    if (const Eventtype type = string_to_eventtype[eventType]; type == top)
        eventAttributesView->showAttributesOfEvent(top, 0);
    else
        eventAttributesView->showAttributesOfEvent(type, index);

    mainWindow->ui->eventsScrollArea->widget()->adjustSize();
    mainWindow->ui->eventsScrollArea->updateGeometry();
}

void ProjectView::deleteEvent(const QString& typeStr, const int index) const {
    const ProjectManager* pm = Inst::get_project_manager();
    const Eventtype etype = string_to_eventtype[typeStr];

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
    if (QStandardItem* folder = paletteView->string_to_folder(typeStr)) folder->removeRow(index);
}

void ProjectView::duplicateEvent(const QString& typeStr, int index) const {
    ProjectManager* pm = Inst::get_project_manager();

    const Eventtype etype = string_to_eventtype[typeStr];

    // Copies element at index, appends " (copy)" to .name, appends to list, returns new name
    auto dup = [index](auto& list) -> QString {
        auto copy = list[index];
        copy.name += " (copy)";
        list.append(copy);
        return copy.name;
    };

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

    paletteView->appendEventRow(etype, newName);
}

void ProjectView::updatePaletteItemName(const QString& typeStr, const int index, const QString& name) const {
    if (paletteView) paletteView->updateItemName(typeStr, index, name);
}

void ProjectView::updateAttributesNameEntry(const QString& typeStr, const int index, const QString& name) const {
    if (eventAttributesView) eventAttributesView->updateNameEntryIfShowing(string_to_eventtype[typeStr], index, name);
}
