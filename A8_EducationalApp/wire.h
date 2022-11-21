#ifndef WIRE_H
#define WIRE_H

#include <QGraphicsLineItem>
#include "gate.h"

class Wire : public QGraphicsLineItem
{
public:
    Wire(Gate& beginGate, Gate& endGate, QGraphicsItem *parent = nullptr); // Need to include gate classes in constructor (begin, end)

    void updateValue(bool newValue); // Updates this wire's value, then updates the next gates
    bool getValue() const { return value; }
    void connect(Gate& gate);
    void disconnect(Gate& gate);

private:
    Gate *begin;
    QVector<Gate*> ends;
    bool value; // Each wire has a value of true or false (1 or 0)
    QPolygonF beginNode, endNode;
    QColor myColor = Qt::black;
};

#endif // WIRE_H
