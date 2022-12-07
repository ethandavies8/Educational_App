#ifndef WIRE_H
#define WIRE_H

#include <QPainter>
#include <QGraphicsLineItem>
#include "gate.h"

class Wire : public QGraphicsLineItem
{
public:
    Wire(Gate& beginGate, Gate& endGate, QGraphicsItem *parent = nullptr); // Need to include gate classes in constructor (begin, end)
    Wire(Gate *startItem, Gate *endItem, QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    void setColor(const QColor &color) { myColor = color; }
    Gate *startItem() const { return startGate; }
    Gate *endItem() const { return endGate; }
    void removeConnections();
    Gate* getBeginningOfWire() { return startGate; }

    void updateValue(bool newValue);                        // Updates this wire's value, then updates the next gates
    bool getValue() const { return value; }                 // Returns value currently on the wire
    int getConnectionCount() const { return ends.count(); } // Returns number of connections
    void connect(Gate *gate);
    void disconnect(Gate *gate);

    QRectF boundingRect() const override;
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    QPointF startPoint;
    QVector<Gate *> ends;
    bool value; // Each wire has a value of true or false (1 or 0)
    QPolygonF beginNode, endNode;
    QColor myColor = Qt::black;
    Gate *startGate;
    Gate *endGate;
    void setUpOffset();
    // QPointF spriteOffset;x
    double xStartOffset;
    double yStartOffset;
    double xEndOffset;
    double yEndOffset;
};

#endif // WIRE_H
