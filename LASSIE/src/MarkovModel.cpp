#include "MarkovModel.hpp"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QDebug>

MarkovModel::MarkovModel(const QString& name)
    : m_name(name)
    , m_description("")
    , m_type("First-Order")
{
}

MarkovModel::~MarkovModel()
{
}

QString MarkovModel::name() const
{
    return m_name;
}

void MarkovModel::setName(const QString& name)
{
    m_name = name;
}

QString MarkovModel::description() const
{
    return m_description;
}

void MarkovModel::setDescription(const QString& description)
{
    m_description = description;
}

QString MarkovModel::type() const
{
    return m_type;
}

void MarkovModel::setType(const QString& type)
{
    m_type = type;
}

QMap<QString, QVector<QPair<QString, double>>> MarkovModel::transitions() const
{
    return m_transitions;
}

void MarkovModel::setTransitions(const QMap<QString, QVector<QPair<QString, double>>>& transitions)
{
    m_transitions = transitions;
}

void MarkovModel::addState(const QString& state)
{
    if (!m_transitions.contains(state)) {
        m_transitions[state] = QVector<QPair<QString, double>>();
    }
}

void MarkovModel::removeState(const QString& state)
{
    // Remove the state itself
    m_transitions.remove(state);
    
    // Remove transitions to this state
    for (auto it = m_transitions.begin(); it != m_transitions.end(); ++it) {
        QVector<QPair<QString, double>>& transitions = it.value();
        for (int i = transitions.size() - 1; i >= 0; --i) {
            if (transitions[i].first == state) {
                transitions.removeAt(i);
            }
        }
    }
}

void MarkovModel::addTransition(const QString& fromState, const QString& toState, double probability)
{
    // Ensure the states exist
    addState(fromState);
    addState(toState);
    
    // Check if the transition already exists
    QVector<QPair<QString, double>>& transitions = m_transitions[fromState];
    for (int i = 0; i < transitions.size(); ++i) {
        if (transitions[i].first == toState) {
            transitions[i].second = probability;
            return;
        }
    }
    
    // Add the new transition
    transitions.append(qMakePair(toState, probability));
}

void MarkovModel::removeTransition(const QString& fromState, const QString& toState)
{
    if (!m_transitions.contains(fromState)) {
        return;
    }
    
    QVector<QPair<QString, double>>& transitions = m_transitions[fromState];
    for (int i = transitions.size() - 1; i >= 0; --i) {
        if (transitions[i].first == toState) {
            transitions.removeAt(i);
            break;
        }
    }
}

bool MarkovModel::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return false;
    }
    
    QJsonObject json;
    json["name"] = m_name;
    json["description"] = m_description;
    json["type"] = m_type;
    
    QJsonObject transitionsJson;
    for (auto it = m_transitions.begin(); it != m_transitions.end(); ++it) {
        QJsonArray stateTransitions;
        for (const auto& transition : it.value()) {
            QJsonObject transitionObj;
            transitionObj["to"] = transition.first;
            transitionObj["probability"] = transition.second;
            stateTransitions.append(transitionObj);
        }
        transitionsJson[it.key()] = stateTransitions;
    }
    json["transitions"] = transitionsJson;
    
    QJsonDocument doc(json);
    QTextStream out(&file);
    out << doc.toJson();
    
    file.close();
    return true;
}

bool MarkovModel::loadFromFile(const QString& filePath)
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
    m_type = json["type"].toString();
    
    m_transitions.clear();
    QJsonObject transitionsJson = json["transitions"].toObject();
    for (auto it = transitionsJson.begin(); it != transitionsJson.end(); ++it) {
        QString fromState = it.key();
        QJsonArray stateTransitions = it.value().toArray();
        
        QVector<QPair<QString, double>> transitions;
        for (const auto& transitionValue : stateTransitions) {
            QJsonObject transitionObj = transitionValue.toObject();
            QString toState = transitionObj["to"].toString();
            double probability = transitionObj["probability"].toDouble();
            transitions.append(qMakePair(toState, probability));
        }
        
        m_transitions[fromState] = transitions;
    }
    
    return true;
}

QString MarkovModel::generateSequence(int length) const
{
    if (m_transitions.isEmpty()) {
        return QString();
    }
    
    // Start with a random state
    QVector<QString> states = m_transitions.keys();
    QString currentState = states[QRandomGenerator::global()->bounded(states.size())];
    
    QString result = currentState;
    
    // Generate the sequence
    for (int i = 1; i < length; ++i) {
        if (!m_transitions.contains(currentState) || m_transitions[currentState].isEmpty()) {
            // If no transitions, pick a random state
            currentState = states[QRandomGenerator::global()->bounded(states.size())];
        } else {
            // Choose next state based on probabilities
            const QVector<QPair<QString, double>>& transitions = m_transitions[currentState];
            
            // Calculate cumulative probabilities
            QVector<double> cumProbs;
            double cumProb = 0.0;
            for (const auto& transition : transitions) {
                cumProb += transition.second;
                cumProbs.append(cumProb);
            }
            
            // Normalize if needed
            if (cumProb > 0.0) {
                for (int j = 0; j < cumProbs.size(); ++j) {
                    cumProbs[j] /= cumProb;
                }
            }
            
            // Choose next state
            double r = QRandomGenerator::global()->generateDouble();
            int nextIndex = 0;
            while (nextIndex < cumProbs.size() && r > cumProbs[nextIndex]) {
                nextIndex++;
            }
            
            if (nextIndex < transitions.size()) {
                currentState = transitions[nextIndex].first;
            } else {
                // Fallback to random state
                currentState = states[QRandomGenerator::global()->bounded(states.size())];
            }
        }
        
        result += " " + currentState;
    }
    
    return result;
} 