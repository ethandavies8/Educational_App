/*
* The Cuties
* CS 3505
* A8-An-Educational-App
*
* Header file for Wire object
*/

#ifndef WIRE_H
#define WIRE_H

#include <QPainter>
#include <QGraphicsLineItem>
#include "gate.h"

class Wire : public QGraphicsLineItem
{
public:
    Wire(Gate *startItem, Gate *endItem, QGraphicsItem *parent = nullptr);

    void setColor(const QColor &color) { myColor = color; }
    void removeConnections();
    Gate* getBeginningOfWire() { return startGate; }

    void updateValue(bool newValue);                        // Updates this wire's value, then updates the next gates
    bool getValue() const { return value; }                 // Returns value currently on the wire
    int getConnectionCount() const { return ends.count(); } // Returns number of connections
    void connect(Gate *gate);
    void disconnect(Gate *gate);

    QRectF boundingRect() const override;
    void updatePosition();
    bool containGate(Gate*);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    bool value; // Each wire has a value of true or false (1 or 0)
    QColor myColor = Qt::black;
    Gate *startGate;
    QVector<Gate *> ends;
};

#endif // WIRE_H
