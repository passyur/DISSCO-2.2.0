#ifndef STOCHOS_HPP
#define STOCHOS_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "../core/event_struct.hpp"
#include "FunctionEntryRow.hpp"

class FunctionGenerator;
class EventAttributesViewController;
class ProjectView;

class Stochos : public QFrame {
    Q_OBJECT

public:
    // methodType 0 = range dist, 1 = functions
    Stochos(int methodType, int stochosIndex, QWidget *parent = nullptr);
    ~Stochos() override;

    int getStochosIndex() const { return m_stochosIndex; }
    void setStochosIndex(int newIndex) {
        m_stochosIndex = newIndex;
        if (m_row) m_row->setIndex(newIndex);
    }
    QString getNodeText();

    void setMinText(const QString& text) {
        if (m_minEntry) { m_minEntry->setText(text); }
    }

    void setMaxText(const QString& text) {
        if (m_maxEntry) { m_maxEntry->setText(text); }
    }

    void setDistText(const QString& text) {
        if (m_distEntry) { m_distEntry->setText(text); }
    }

    void setValText(const QString& text) {
        if (m_row) { m_row->setText(text); }
    }

signals:
    void deleteRequested(Stochos* self);
    void nodeTextChanged(Stochos* self);
    void editFocused(QLineEdit* edit);

private slots:
    void onRemoveNodeClicked();
    void onNodeTextChanged();

private:
    int       m_stochosIndex;
    int       m_methodType;

    // UI elements
    QVBoxLayout*        m_mainLayout = nullptr;
    // methodType 0 entries
    QLineEdit*          m_minEntry   = nullptr;
    QLineEdit*          m_maxEntry   = nullptr;
    QLineEdit*          m_distEntry  = nullptr;
    QPushButton*        m_removeStochosButton = nullptr;
    // methodType 1 entry
    FunctionEntryRow*   m_row        = nullptr;
};

#endif
