#include "MarkovChain.hpp"
#include <QRandomGenerator>

MarkovChain::MarkovChain(const QString &name)
    : name(name)
{
}

MarkovChain::MarkovChain(const MarkovChain &other)
    : name(other.name)
    , states(other.states)
    , transitionMatrix(other.transitionMatrix)
{
}

void MarkovChain::setName(const QString &newName)
{
    name = newName;
}

QString MarkovChain::getName() const
{
    return name;
}

void MarkovChain::addState(const QString &state)
{
    if (!states.contains(state)) {
        states.append(state);
        // Initialize transition probabilities to 0
        for (const QString &fromState : states) {
            transitionMatrix[fromState][state] = 0.0;
            transitionMatrix[state][fromState] = 0.0;
        }
    }
}

void MarkovChain::removeState(const QString &state)
{
    if (states.contains(state)) {
        states.removeOne(state);
        // Remove all transitions involving this state
        transitionMatrix.remove(state);
        for (auto &transitions : transitionMatrix) {
            transitions.remove(state);
        }
    }
}

void MarkovChain::setTransitionProbability(const QString &fromState, const QString &toState, double probability)
{
    if (states.contains(fromState) && states.contains(toState)) {
        transitionMatrix[fromState][toState] = qBound(0.0, probability, 1.0);
    }
}

double MarkovChain::getTransitionProbability(const QString &fromState, const QString &toState) const
{
    if (states.contains(fromState) && states.contains(toState)) {
        return transitionMatrix[fromState][toState];
    }
    return 0.0;
}

QStringList MarkovChain::getStates() const
{
    return states;
}

void MarkovChain::normalizeTransitions()
{
    for (const QString &fromState : states) {
        double sum = 0.0;
        for (const QString &toState : states) {
            sum += transitionMatrix[fromState][toState];
        }
        if (sum > 0.0) {
            for (const QString &toState : states) {
                transitionMatrix[fromState][toState] /= sum;
            }
        }
    }
}

QString MarkovChain::getNextState(const QString &currentState) const
{
    if (!states.contains(currentState) || states.isEmpty()) {
        return QString();
    }

    double random = QRandomGenerator::global()->generateDouble();
    double sum = 0.0;

    for (const QString &toState : states) {
        sum += transitionMatrix[currentState][toState];
        if (random <= sum) {
            return toState;
        }
    }

    return states.last(); // Fallback to last state if no transition found
}

void MarkovChain::saveToXml(QDomDocument &doc, QDomElement &root) const
{
    root.setAttribute("name", name);

    // Save states
    QDomElement statesElement = doc.createElement("states");
    for (const QString &state : states) {
        QDomElement stateElement = doc.createElement("state");
        stateElement.setAttribute("name", state);
        statesElement.appendChild(stateElement);
    }
    root.appendChild(statesElement);

    // Save transitions
    QDomElement transitionsElement = doc.createElement("transitions");
    for (const QString &fromState : states) {
        for (const QString &toState : states) {
            double probability = transitionMatrix[fromState][toState];
            if (probability > 0.0) {
                QDomElement transitionElement = doc.createElement("transition");
                transitionElement.setAttribute("from", fromState);
                transitionElement.setAttribute("to", toState);
                transitionElement.setAttribute("probability", QString::number(probability));
                transitionsElement.appendChild(transitionElement);
            }
        }
    }
    root.appendChild(transitionsElement);
}

MarkovChain* MarkovChain::loadFromXml(const QDomElement &element)
{
    QString name = element.attribute("name");
    MarkovChain *chain = new MarkovChain(name);

    // Load states
    QDomElement statesElement = element.firstChildElement("states");
    QDomElement stateElement = statesElement.firstChildElement("state");
    while (!stateElement.isNull()) {
        chain->addState(stateElement.attribute("name"));
        stateElement = stateElement.nextSiblingElement("state");
    }

    // Load transitions
    QDomElement transitionsElement = element.firstChildElement("transitions");
    QDomElement transitionElement = transitionsElement.firstChildElement("transition");
    while (!transitionElement.isNull()) {
        QString fromState = transitionElement.attribute("from");
        QString toState = transitionElement.attribute("to");
        double probability = transitionElement.attribute("probability").toDouble();
        chain->setTransitionProbability(fromState, toState, probability);
        transitionElement = transitionElement.nextSiblingElement("transition");
    }

    return chain;
} 