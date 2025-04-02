# DISSCO-2.2.0

## MainWindow
mainWindow()
setSavedTitle()
setUnsavedTitle()

**menuProjectConfigureNoteModifiers()**

**menuProjectRun()**

## menuFile
**menuFileNewProject()**
- Creates a new project using FileOperations::newProject
- Sets toolbar actions to sensitive
- Pushes current project back onto project vector
- Changes current project view to current project
- Sets the display title " - LASSIE"
- Calls newProject->setProperties()

**menuFileNewObject()**
- Null check projectView, then call projectview->insertObject()

**menuFileOpen()**
- Open project

**menuFileOpenXML()**

**menuFileSave()**
- Null check projectView, then call projectView->save()

**menuProject**
- 
