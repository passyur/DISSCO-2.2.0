#ifndef INST_HPP
#define INST_HPP

#include <QObject>
#include <QAction>

#include "core/project_struct.hpp"

class Inst : public QObject {
    Q_OBJECT

    public:
        static Inst* instance(void){
            static Inst *that = new Inst;
            return that;
        }

        static ProjectManager* get_project_manager() { return instance()->project_manager_; }
        
    private:
        Inst(void);
        
        ProjectManager *project_manager_;

};

#endif