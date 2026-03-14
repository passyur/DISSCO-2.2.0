#ifndef PARTIALS_HPP
#define PARTIALS_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
// #include <QTreeView>
// #include <QStandardItemModel>
// #include <QAbstractItemView>
#include <QLabel>
#include <QString>

#include "../core/event_struct.hpp"

class EventAttributesViewController;
class ProjectView;

class Partials : public QFrame {
    Q_OBJECT

public:
    Partials(unsigned eventIndex, int partialIndex, QWidget* parent);
    ~Partials() override;

    // Called by EventAttributesViewController when partials above this one are deleted
    void setPartialIndex(int partialIndex) {
        m_partialIndex = partialIndex;
        m_partialLabel->setText("Partial " + QString::number(partialIndex + 1) + ":");
    }

    QLineEdit*          m_partialEntry;

signals:
    void deleteRequested(Partials* self);

private slots:
    void onInsertFunctionClicked() {};
    void onRemovePartialClicked();
    void onPartialChanged(const QString& text);

private:
    // Identity of the spectrum partial in the backend
    // Eventtype m_eventType = 5
    Spectrum& getBackendLayer();

    unsigned  m_eventIndex;
    int       m_partialIndex;

    // UI elements
    QVBoxLayout*        m_mainLayout;
    QLabel*             m_partialLabel;
    QPushButton*        m_insertFuncButton;
    QPushButton*        m_removePartialButton;
};

#endif