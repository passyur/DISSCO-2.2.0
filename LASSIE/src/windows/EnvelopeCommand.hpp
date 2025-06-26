#ifndef ENVELOPECOMMAND_HPP
#define ENVELOPECOMMAND_HPP

#include <QUndoCommand>
#include <QPointF>
#include "EnvelopeLibraryEntry.hpp"

// Base class for envelope commands
class EnvelopeCommand : public QUndoCommand
{
public:
    explicit EnvelopeCommand(EnvelopeLibraryEntry* envelope, QUndoCommand* parent = nullptr);
    
protected:
    EnvelopeLibraryEntry* envelope;
};

// Command for adding a point
class AddPointCommand : public EnvelopeCommand
{
public:
    AddPointCommand(EnvelopeLibraryEntry* envelope, const QPointF& point);
    
    void undo() override;
    void redo() override;
    
private:
    QPointF point;
};

// Command for removing a point
class RemovePointCommand : public EnvelopeCommand
{
public:
    RemovePointCommand(EnvelopeLibraryEntry* envelope, int index);
    
    void undo() override;
    void redo() override;
    
private:
    int index;
    QPointF removedPoint;
};

// Command for moving a point
class MovePointCommand : public EnvelopeCommand
{
public:
    MovePointCommand(EnvelopeLibraryEntry* envelope, int index, const QPointF& newPos);
    
    void undo() override;
    void redo() override;
    
private:
    int index;
    QPointF oldPos;
    QPointF newPos;
};

#endif // ENVELOPECOMMAND_HPP 