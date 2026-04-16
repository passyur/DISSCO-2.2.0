#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "inst.hpp"

/* many of these are temporarily here until someone wants to go through and break up these functions according to the roles of each Class
   e.g., Nhi's MarkovModel methods should go into ProjectManager in place of direct reference access to the QList<MarkovModel<float>> */
namespace MUtilities {

    /* Function called when project modified to mark the window as having unsaved changes */
    inline void modified(){
        Inst::get_project_manager()->markModified();
    }

}

#endif /* UTILITIES_HPP */