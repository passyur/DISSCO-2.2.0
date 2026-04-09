#ifndef PARTIALS_HPP
#define PARTIALS_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
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
        m_partialLabel->setText("Partial " + QString::number(m_partialIndex+1) + ":");
    }

    // Flush the partial entry text to the backend Layer::by_layer.
    // \todo this is tremendously chud and should be somehow delegated to ProjectManager, but for now is fine
    void saveWeightToBackend() {
        // qDebug() << "in saveWeightToBackend partials at " << m_partialIndex ;
        // qDebug() << "partials size: " << getBackendLayer().partials.size();
        getBackendLayer().partials[m_partialIndex] = m_partialEntry->text();
    }
    void setPartialText(const QString& text) {
        if (m_partialEntry) {
            m_partialEntry->setText(text);
        }
    }

signals:
    void deleteRequested(Partials* self);

private slots:
    void onInsertFunctionClicked();
    void onRemovePartialClicked();
    void onPartialChanged(const QString& text);

private:
    // Identity of the spectrum partial in the backend
    // Eventtype m_eventType = 5
    Spectrum& getBackendLayer();

    unsigned  m_eventIndex;
    int       m_partialIndex;

    // UI elements
    QVBoxLayout*        m_mainLayout = nullptr;
    QPushButton*        m_insertFuncButton = nullptr;
    QLabel*             m_partialLabel = nullptr;
    QLineEdit*          m_partialEntry = nullptr;
    QPushButton*        m_removePartialButton = nullptr;
};

#endif