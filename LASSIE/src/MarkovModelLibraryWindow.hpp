#ifndef MARKOVMODELLIBRARYWINDOW_HPP
#define MARKOVMODELLIBRARYWINDOW_HPP

#include <QWidget>
#include <QString>

class MarkovModelLibraryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MarkovModelLibraryWindow(QWidget *parent = nullptr);
    ~MarkovModelLibraryWindow();

    void *getActiveModel() const { return nullptr; } // TODO: Implement

public slots:
    void onNewModel();
    void onOpenModel();
    void onSaveModel();
    void onCloseModel();

private:
    void setupUi();
};

#endif // MARKOVMODELLIBRARYWINDOW_HPP 