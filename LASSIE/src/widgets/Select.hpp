#ifndef SELECT_HPP
#define SELECT_HPP

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

class Select : public QFrame {
    Q_OBJECT

public:
    Select(int selectIndex,  QWidget *parent = nullptr, QTextEdit* resultEntry = nullptr);
    ~Select() override;

    int getSelectIndex() const { return m_selectIndex; }
    void setSelectIndex(int newIndex) { m_selectIndex = newIndex; }
    QString getNodeText();

    void setValText(const QString& text) {
        if (m_valEntry) { m_valEntry->setText(text); }
    }

signals:
    void deleteRequested(Select* self);
    void nodeTextChanged(Select* self);
    void editFocused(QLineEdit* edit);

private slots:
    void onRemoveNodeClicked();
    void onNodeTextChanged();

private:
    int       m_selectIndex;

    // UI elements
    QVBoxLayout*        m_mainLayout = nullptr;
    QLineEdit*          m_valEntry = nullptr;
    QPushButton*        m_removeSelectButton = nullptr;
};

#endif