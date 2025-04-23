#include "Project.hpp"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

Project::Project(const QString& name)
    : m_name(name)
    , m_description("")
    , m_status("New")
    , m_createdDate(QDateTime::currentDateTime())
    , m_modifiedDate(QDateTime::currentDateTime())
{
}

Project::~Project()
{
}

QString Project::name() const
{
    return m_name;
}

void Project::setName(const QString& name)
{
    m_name = name;
    m_modifiedDate = QDateTime::currentDateTime();
}

QString Project::description() const
{
    return m_description;
}

void Project::setDescription(const QString& description)
{
    m_description = description;
    m_modifiedDate = QDateTime::currentDateTime();
}

QString Project::status() const
{
    return m_status;
}

void Project::setStatus(const QString& status)
{
    m_status = status;
    m_modifiedDate = QDateTime::currentDateTime();
}

QDateTime Project::createdDate() const
{
    return m_createdDate;
}

void Project::setCreatedDate(const QDateTime& date)
{
    m_createdDate = date;
}

QDateTime Project::modifiedDate() const
{
    return m_modifiedDate;
}

void Project::setModifiedDate(const QDateTime& date)
{
    m_modifiedDate = date;
}

QVector<QSharedPointer<MarkovModel>> Project::models() const
{
    return m_models;
}

void Project::setModels(const QVector<QSharedPointer<MarkovModel>>>& models)
{
    m_models = models;
    m_modifiedDate = QDateTime::currentDateTime();
}

void Project::addModel(const QSharedPointer<MarkovModel>& model)
{
    // Check if model with same name already exists
    for (int i = 0; i < m_models.size(); ++i) {
        if (m_models[i]->name() == model->name()) {
            m_models[i] = model;
            m_modifiedDate = QDateTime::currentDateTime();
            return;
        }
    }
    
    // Add new model
    m_models.append(model);
    m_modifiedDate = QDateTime::currentDateTime();
}

void Project::removeModel(const QString& modelName)
{
    for (int i = 0; i < m_models.size(); ++i) {
        if (m_models[i]->name() == modelName) {
            m_models.removeAt(i);
            m_modifiedDate = QDateTime::currentDateTime();
            return;
        }
    }
}

QSharedPointer<MarkovModel> Project::getModel(const QString& modelName) const
{
    for (const auto& model : m_models) {
        if (model->name() == modelName) {
            return model;
        }
    }
    
    return nullptr;
}

bool Project::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return false;
    }
    
    QJsonObject json;
    json["name"] = m_name;
    json["description"] = m_description;
    json["status"] = m_status;
    json["createdDate"] = m_createdDate.toString(Qt::ISODate);
    json["modifiedDate"] = m_modifiedDate.toString(Qt::ISODate);
    
    QJsonArray modelsArray;
    for (const auto& model : m_models) {
        QJsonObject modelObj;
        modelObj["name"] = model->name();
        modelObj["description"] = model->description();
        modelObj["type"] = model->type();
        
        // Save model data to a temporary file
        QString tempFilePath = filePath + "." + model->name() + ".temp";
        if (model->saveToFile(tempFilePath)) {
            QFile tempFile(tempFilePath);
            if (tempFile.open(QIODevice::ReadOnly)) {
                QTextStream in(&tempFile);
                modelObj["data"] = in.readAll();
                tempFile.close();
                QFile::remove(tempFilePath);
            }
        }
        
        modelsArray.append(modelObj);
    }
    json["models"] = modelsArray;
    
    QJsonDocument doc(json);
    QTextStream out(&file);
    out << doc.toJson();
    
    file.close();
    return true;
}

bool Project::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << filePath;
        return false;
    }
    
    QTextStream in(&file);
    QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
    file.close();
    
    if (doc.isNull()) {
        qDebug() << "Failed to parse JSON from file:" << filePath;
        return false;
    }
    
    QJsonObject json = doc.object();
    m_name = json["name"].toString();
    m_description = json["description"].toString();
    m_status = json["status"].toString();
    m_createdDate = QDateTime::fromString(json["createdDate"].toString(), Qt::ISODate);
    m_modifiedDate = QDateTime::fromString(json["modifiedDate"].toString(), Qt::ISODate);
    
    m_models.clear();
    QJsonArray modelsArray = json["models"].toArray();
    for (const auto& modelValue : modelsArray) {
        QJsonObject modelObj = modelValue.toObject();
        QString modelName = modelObj["name"].toString();
        
        QSharedPointer<MarkovModel> model = QSharedPointer<MarkovModel>::create(modelName);
        model->setDescription(modelObj["description"].toString());
        model->setType(modelObj["type"].toString());
        
        // Load model data from the embedded data
        QString modelData = modelObj["data"].toString();
        if (!modelData.isEmpty()) {
            QString tempFilePath = filePath + "." + modelName + ".temp";
            QFile tempFile(tempFilePath);
            if (tempFile.open(QIODevice::WriteOnly)) {
                QTextStream out(&tempFile);
                out << modelData;
                tempFile.close();
                
                model->loadFromFile(tempFilePath);
                QFile::remove(tempFilePath);
            }
        }
        
        m_models.append(model);
    }
    
    return true;
} 