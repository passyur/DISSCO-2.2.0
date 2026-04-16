#ifndef PARTIALS_HPP
#define PARTIALS_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QString>

#include "../core/event_struct.hpp"
#include "FunctionEntryRow.hpp"

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
        m_row->setLabel("Partial " + QString::number(m_partialIndex+1) + ":");
        m_row->setIndex(m_partialIndex);
    }

    // Flush the partial entry text to the backend Layer::by_layer.
    // \todo this is tremendously chud and should be somehow delegated to ProjectManager, but for now is fine
    void saveWeightToBackend() {
        getBackendLayer().partials[m_partialIndex] = m_row->getText();
    }
    void setPartialText(const QString& text) {
        m_row->setText(text);
    }

signals:
    void deleteRequested(Partials* self);

private:
    // Identity of the spectrum partial in the backend
    // Eventtype m_eventType = 5
    Spectrum& getBackendLayer();

    unsigned  m_eventIndex;
    int       m_partialIndex;

    // UI elements
    QVBoxLayout*        m_mainLayout = nullptr;
    FunctionEntryRow*   m_row        = nullptr;
};

#endif
