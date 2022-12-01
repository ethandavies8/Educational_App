#ifndef WIRE_H
#define WIRE_H

#include <QPainter>
#include <QGraphicsLineItem>
#include "gate.h"

class Wire : public QGraphicsLineItem
{
public:
    Wire(Gate& beginGate, Gate& endGate, QGraphicsItem *parent = nullptr); // Need to include gate classes in constructor (begin, end)

    void updateValue(bool newValue); // Updates this wire's value, then updates the next gates
    bool getValue() const { return value; } // Returns value currently on the wire
    int getConnectionCount() const { return ends.count(); } // Returns number of connections
    void connect(Gate& gate);
    void disconnect(Gate& gate);
    void removeConnections();
    Gate* getBeginningOfWire() { return begin; }

    QRectF boundingRect() const;
    void updatePosition();
    void addSubLineStarting(QPointF);

private:
    Gate *begin;
    QPointF startPoint;
    QVector<Gate*> ends;
    bool value; // Each wire has a value of true or false (1 or 0)
    QPolygonF beginNode, endNode;
    QColor myColor = Qt::black;
    QList<QPointF> subLineStarting;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *widget);
};

#endif // WIRE_H
