# Save Functionality Test Guide

## Issues Fixed:

1. **Directory Creation**: Fixed the `newFile()` method to use `mkpath()` instead of `mkdir()` to create parent directories recursively.

2. **Error Handling**: Added proper error handling in both `MainWindow::saveFile()` and `ProjectView::save()` methods.

3. **Null Pointer Checks**: Added checks to ensure `projectView` exists before attempting to save.

4. **Directory Existence**: Added checks to ensure the target directory exists before writing files.

5. **Incomplete Implementation**: Completed the "STILL IN PROGRESS" sections in the save method.

## How to Test:

1. **Launch the application**: Run `./LASSIE.app/Contents/MacOS/LASSIE`

2. **Create a new project**:
   - Click "File" → "New Project" or use Ctrl+N
   - Choose a location and name for your project
   - The application should create a project folder and a .dissco file

3. **Test save functionality**:
   - Click "File" → "Save" or use Ctrl+S
   - The project should save successfully
   - Check that the .dissco file was created in the project folder

4. **Test save as functionality**:
   - Click "File" → "Save As"
   - Choose a different location/name
   - Verify the file is saved to the new location

## Expected Behavior:

- No error messages should appear during save operations
- The window title should update to show the saved file name (without the "*" prefix)
- Status bar should show "File saved" message
- The .dissco file should be created with proper XML structure

## Debug Output:

The application will output debug messages to help diagnose issues:
- "Current File: [filepath]" - Shows the file being saved
- "In Main Window Save Function" - Confirms save method is called
- "Project saved successfully to: [filepath]" - Confirms successful save
- Any error messages will show file operation failures

## Common Issues and Solutions:

1. **Permission Denied**: Make sure you have write permissions to the target directory
2. **Directory Not Found**: The application now creates missing directories automatically
3. **No Project to Save**: Create or open a project before attempting to save
4. **File Already Exists**: The application will overwrite existing files (consider adding confirmation dialog)

## File Structure:

The saved .dissco file should contain:
- ProjectConfiguration section with all project properties
- NoteModifiers section with default and custom modifiers
- EnvelopeLibrary section (currently placeholder)
- MarkovModelLibrary section (currently placeholder)
- Events section (currently placeholder) 