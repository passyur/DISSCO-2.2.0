#ifndef MARKOVCHAIN_HPP
#define MARKOVCHAIN_HPP

#include <QString>
#include <QVector>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>

class MarkovChain
{
public:
    explicit MarkovChain(const QString &name);
    MarkovChain(const MarkovChain &other); // Copy constructor

    // Basic operations
    void setName(const QString &name);
    QString getName() const;
    
    // State management
    void addState(const QString &state);
    void removeState(const QString &state);
    void setTransitionProbability(const QString &fromState, const QString &toState, double probability);
    double getTransitionProbability(const QString &fromState, const QString &toState) const;
    QStringList getStates() const;
    
    // Matrix operations
    void normalizeTransitions();
    QString getNextState(const QString &currentState) const;
    
    // XML serialization
    void saveToXml(QDomDocument &doc, QDomElement &root) const;
    static MarkovChain* loadFromXml(const QDomElement &element);

private:
    QString name;
    QStringList states;
    QMap<QString, QMap<QString, double>> transitionMatrix;
};

#endif // MARKOVCHAIN_HPP 