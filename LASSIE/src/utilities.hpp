#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "windows/MainWindow.hpp"

/* many of these are temporarily here until someone wants to go through and break up these functions according to the roles of each Class
   e.g., Nhi's MarkovModel methods should go into ProjectManager in place of direct reference access to the QList<MarkovModel<float>> */
namespace MUtilities {

    /* Function called when project modified to mark the window as having unsaved changes */
    inline void modified(){
        MainWindow::instance()->setWindowModified(true);
    }

}

namespace MarkovUtilities {
    // nhi: Markov model methods
    inline int createNewMarkovModel() { 
        ProjectManager *pm = Inst::get_project_manager();
        pm->markovmodels().append(MarkovModel<float>()); 
        return pm->markovmodels().size() - 1; 
    }

    inline int duplicateMarkovModel(int idx) { 
        ProjectManager *pm = Inst::get_project_manager();
        // QList<MarkovModel<float>*> markov_models = ;
        if (idx >= 0 && idx < pm->markovmodels().size()) {
            pm->markovmodels().append(pm->markovmodels()[idx]); 
            return pm->markovmodels().size() - 1;
        }
        return -1;
    }

    inline void removeMarkovModel(int idx) { 
        ProjectManager *pm = Inst::get_project_manager();
        if (idx >= 0 && idx < pm->markovmodels().size()) {
            pm->markovmodels().erase(pm->markovmodels().begin() + idx);
        }
    }
}

#endif /* UTILITIES_HPP */