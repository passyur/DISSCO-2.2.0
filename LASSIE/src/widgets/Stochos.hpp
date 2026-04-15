#ifndef STOCHOS_HPP
#define STOCHOS_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "../core/event_struct.hpp"

class FunctionGenerator;
class EventAttributesViewController;
class ProjectView;

class Stochos : public QFrame {
    Q_OBJECT

public:
    // methodType 0 = range dist, 1 = functions
    Stochos(int methodType, int stochosIndex,  QWidget *parent = nullptr, QTextEdit* resultEntry = nullptr);
    ~Stochos() override;

    int getStochosIndex() const { return m_stochosIndex; }
    void setStochosIndex(int newIndex) { m_stochosIndex = newIndex; }
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
        if (m_valEntry) { m_valEntry->setText(text); }
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
    QLineEdit*          m_minEntry = nullptr;
    QLineEdit*          m_maxEntry = nullptr;
    QLineEdit*          m_distEntry = nullptr;
    QLineEdit*          m_valEntry = nullptr;
    QPushButton*        m_removeStochosButton = nullptr;
};

#endif