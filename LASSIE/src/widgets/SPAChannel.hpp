#ifndef SPACHANNEL_HPP
#define SPACHANNEL_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "generic/FunctionEntryRow.hpp"

class SPAChannel : public QFrame {
    Q_OBJECT
public:
    SPAChannel(int id, bool isPartialMode, QWidget* parent = nullptr);

    void addRow(int index, bool silent = false);
    void removeRow(int index);
    QString getXml();
    void updateLabels();
    int rowCount() const { return m_rows.size(); }
    void setTitle(const QString& text);
    void hideButtons();
    void hideTitle();

signals:
    void textChanged();
    void insertChannelRequested(SPAChannel* self);
    void removeChannelRequested(SPAChannel* self);
    void rowInsertRequested(int index);
    void rowRemoveRequested(int index);

private:
    bool m_isPartialMode;
    QLabel* m_titleLabel;
    QPushButton *m_insBtn, *m_rmBtn;
    QWidget* m_rowsContainer;
    QVBoxLayout* m_rowsLayout;
    QList<FunctionEntryRow*> m_rows;
};

#endif