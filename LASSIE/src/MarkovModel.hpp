#ifndef MARKOVMODEL_HPP
#define MARKOVMODEL_HPP

#include <QString>
#include <QMap>
#include <QVector>
#include <QSharedPointer>

class MarkovModel
{
public:
    MarkovModel(const QString& name = QString());
    ~MarkovModel();

    // Getters and setters
    QString name() const;
    void setName(const QString& name);
    
    QString description() const;
    void setDescription(const QString& description);
    
    QString type() const;
    void setType(const QString& type);
    
    // Model data
    QMap<QString, QVector<QPair<QString, double>>> transitions() const;
    void setTransitions(const QMap<QString, QVector<QPair<QString, double>>>& transitions);
    
    // Model operations
    void addState(const QString& state);
    void removeState(const QString& state);
    void addTransition(const QString& fromState, const QString& toState, double probability);
    void removeTransition(const QString& fromState, const QString& toState);
    
    // Serialization
    bool saveToFile(const QString& filePath) const;
    bool loadFromFile(const QString& filePath);
    
    // Model generation
    QString generateSequence(int length) const;
    
private:
    QString m_name;
    QString m_description;
    QString m_type;
    QMap<QString, QVector<QPair<QString, double>>> m_transitions;
};

typedef QSharedPointer<MarkovModel> MarkovModelPtr;

#endif // MARKOVMODEL_HPP 