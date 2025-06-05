#include "EnvelopeCommand.hpp"

EnvelopeCommand::EnvelopeCommand(EnvelopeLibraryEntry* env, QUndoCommand* parent)
    : QUndoCommand(parent)
    , envelope(env)
{
}

AddPointCommand::AddPointCommand(EnvelopeLibraryEntry* envelope, const QPointF& pt)
    : EnvelopeCommand(envelope)
    , point(pt)
{
    setText(QObject::tr("Add Point"));
}

void AddPointCommand::undo()
{
    envelope->removePoint(envelope->findPoint(point));
}

void AddPointCommand::redo()
{
    envelope->addPoint(point);
}

RemovePointCommand::RemovePointCommand(EnvelopeLibraryEntry* envelope, int idx)
    : EnvelopeCommand(envelope)
    , index(idx)
{
    setText(QObject::tr("Remove Point"));
    removedPoint = envelope->getPoint(index);
}

void RemovePointCommand::undo()
{
    envelope->insertPoint(index, removedPoint);
}

void RemovePointCommand::redo()
{
    envelope->removePoint(index);
}

MovePointCommand::MovePointCommand(EnvelopeLibraryEntry* envelope, int idx, const QPointF& newPosition)
    : EnvelopeCommand(envelope)
    , index(idx)
    , oldPos(envelope->getPoint(idx))
    , newPos(newPosition)
{
    setText(QObject::tr("Move Point"));
}

void MovePointCommand::undo()
{
    envelope->movePoint(index, oldPos);
}

void MovePointCommand::redo()
{
    envelope->movePoint(index, newPos);
} 