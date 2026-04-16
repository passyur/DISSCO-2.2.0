#ifndef SELECT_HPP
#define SELECT_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QString>

#include "../core/event_struct.hpp"
#include "FunctionEntryRow.hpp"

class FunctionGenerator;
class EventAttributesViewController;
class ProjectView;

class Select : public QFrame {
    Q_OBJECT

public:
    Select(int selectIndex, QWidget *parent = nullptr);
    ~Select() override;

    int getSelectIndex() const { return m_selectIndex; }
    void setSelectIndex(int newIndex) { m_selectIndex = newIndex; m_row->setIndex(newIndex); }
    QString getNodeText();

    void setValText(const QString& text) {
        m_row->setText(text);
    }

signals:
    void deleteRequested(Select* self);
    void nodeTextChanged(Select* self);
    void editFocused(QLineEdit* edit);

private:
    int       m_selectIndex;

    // UI elements
    QVBoxLayout*        m_mainLayout = nullptr;
    FunctionEntryRow*   m_row        = nullptr;
};

#endif
