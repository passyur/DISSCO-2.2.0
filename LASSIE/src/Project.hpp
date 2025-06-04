#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <QString>
#include <QDateTime>
#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include "MarkovModel.hpp"

class Project
{
public:
    Project(const QString& name = QString());
    ~Project();

    // Getters and setters
    QString name() const;
    void setName(const QString& name);
    
    QString description() const;
    void setDescription(const QString& description);
    
    QString status() const;
    void setStatus(const QString& status);
    
    QDateTime createdDate() const;
    void setCreatedDate(const QDateTime& date);
    
    QDateTime modifiedDate() const;
    void setModifiedDate(const QDateTime& date);
    
    // Project items
    QVector<QSharedPointer<MarkovModel>> models() const;
    void setModels(const QVector<QSharedPointer<MarkovModel>>& models);
    
    void addModel(const QSharedPointer<MarkovModel>& model);
    void removeModel(const QString& modelName);
    QSharedPointer<MarkovModel> getModel(const QString& modelName) const;
    
    // Serialization
    bool saveToFile(const QString& filePath) const;
    bool loadFromFile(const QString& filePath);
    
private:
    QString m_name;
    QString m_description;
    QString m_status;
    QDateTime m_createdDate;
    QDateTime m_modifiedDate;
    QVector<QSharedPointer<MarkovModel>> m_models;
};

typedef QSharedPointer<Project> ProjectPtr;

#endif // PROJECT_HPP 