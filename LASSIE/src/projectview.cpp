/* The 'project' object that keeps track of project details to deploy
 * in the associated window (currently, the project view).
 *
 * Copyright (c) 2025, DISSCO authors */

#include <QFile>

// #include "Define.h"
#include "projectview.hpp"
#include "inst.hpp"
// #include "muParser.h"

namespace PVCHelper {
    /* called before appearing (see immediately succeeding f'n) the checkboxes; init'd when a user brings up the configure note modifier menu */
    // void initializeNoteModifierButton(QMap<QString, bool>& default_note_modifiers, QCheckBox& checkbox, QString key_name, QString button_name){
    //     note_modifiers_configuration_dialog_ref_builder->get_widget(button_name, checkbox);
    //     /* let the compiler do this, branchless */
    //     checkbox->set_active(default_note_modifiers[key_name]);
    // }

    // void showNoteModifierButton(QMap<QString, bool>& default_note_modifiers, QCheckBox& checkbox, QString key_name, QString button_name){
    //     note_modifiers_configuration_dialog_ref_builder->get_widget(button_name, checkbox);
    //     default_note_modifiers[key_name] = checkbox->get_active();
    // }
}
 
// unsigned int counter = 1;
/** the empty constructor for a NEW project, will return a ProjectView  **/
// ProjectView::ProjectView(){
//     std::string top_name = "0";
    
    // filepath = "";
    // project_title = "Untitled-" + std::to_string(counter);
    // file_flag = "";
    // duration = "";
    // num_channels = "2";
    // sample_rate = std::to_string(SAMPLING_RATE);
    // sample_size = "16";
    // num_threads = "1";
    // num_staffs = "1";
    // top_event_num = "0";

    // synthesis = true;
    // score = false;
    // grand_staff = false;

    // topwin = NULL;
    // highwin = NULL;
    // midwin = NULL;
    // lowwin = NULL;
    // bottomwin = NULL;
    // spectrumwin = NULL;
    // envwin = NULL;
    // sievewin = NULL;
    // spatialwin = NULL;
    // patternwin = NULL;
    // reverbwin = NULL;
    // notewin = NULL;
    // filterwin = NULL;
    // measurewin = NULL;
    // env_lib_entries = NULL;
// 
    // 
// }

ProjectView::ProjectView(Project *proj){
    Inst::instance()->get_project_manager();

    empty_project = false;

    seed = "";

    // initializeModifiers();
    // add(leftTwoPlusAttributes);
}
