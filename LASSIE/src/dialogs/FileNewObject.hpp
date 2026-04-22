#ifndef FILENEWOBJECT_HPP
#define FILENEWOBJECT_HPP

#include <QDialog>
#include <QMap>
#include <QStringList>

namespace Ui {
class FileNewObject;
}

class FileNewObject : public QDialog
{
    Q_OBJECT

public:
    /*Constructor to create the New Object Dialog Interface*/
    explicit FileNewObject(QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~FileNewObject();

    Ui::FileNewObject *ui;

    // Pre-select the radio button matching typeStr and clear the name field.
    // If typeStr is "Top", clears all selections. Assumes non-empty string.
    void setDefaultType(const QString& typeStr);

    // Provide existing event names per type so accept() can reject duplicates.
    void setExistingNames(const QMap<QString, QStringList>& names);

public slots:
    void accept() override;

private slots:
    void validateInput();

private:
    QMap<QString, QStringList> m_existingNames;
};

#endif // FILENEWOBJECT_HPP
