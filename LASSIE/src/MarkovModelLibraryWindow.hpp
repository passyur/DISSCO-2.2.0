#ifndef MARKOVMODELLIBRARYWINDOW_HPP
#define MARKOVMODELLIBRARYWINDOW_HPP

#include <QMainWindow>
#include <QMap>
#include <QSharedPointer>
#include "MarkovModel.hpp"

namespace Ui {
class MarkovModelLibraryWindow;
}

class QStandardItemModel;
class QStandardItem;

class MarkovModelLibraryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MarkovModelLibraryWindow(QWidget *parent = nullptr);
    ~MarkovModelLibraryWindow();

private slots:
    void onNewButtonClicked();
    void onOpenButtonClicked();
    void onSaveButtonClicked();
    void onCloseButtonClicked();
    void onSearchTextChanged(const QString &text);
    void onModelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    
    // Model editing
    void onAddStateClicked();
    void onRemoveStateClicked();
    void onAddTransitionClicked();
    void onRemoveTransitionClicked();
    void onGenerateSequenceClicked();

private:
    void setupConnections();
    void initializeModel();
    void updateModelView(const QSharedPointer<MarkovModel>& model);
    void clearModelView();
    void addModelToTree(const QSharedPointer<MarkovModel>& model);
    void removeModelFromTree(const QString& modelName);
    void filterModels(const QString& searchText);
    
    Ui::MarkovModelLibraryWindow *ui;
    QStandardItemModel *model;
    QMap<QString, QSharedPointer<MarkovModel>> models;
    QSharedPointer<MarkovModel> currentModel;
};

#endif // MARKOVMODELLIBRARYWINDOW_HPP 