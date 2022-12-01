#ifndef WIRE_H
#define WIRE_H

#include <QPainter>
#include <QGraphicsLineItem>
#include "gate.h"

class Wire : public QGraphicsLineItem
{
public:
    //Wire(Gate& beginGate, Gate& endGate, QGraphicsItem *parent = nullptr); // Need to include gate classes in constructor (begin, end)
    Wire(Gate *startItem, Gate *endItem, QGraphicsItem *parent = nullptr);

int type() const override { return Type; }
//QPainterPath shape() const override;
void setColor(const QColor &color) { myColor = color; }
Gate *startItem() const { return myStartItem; }
Gate *endItem() const { return myEndItem; }


void updateValue(bool newValue); // Updates this wire's value, then updates the next gates
bool getValue() const { return value; } // Returns value currently on the wire
int getConnectionCount() const { return ends.count(); } // Returns number of connections
void connect(Gate& gate);
void disconnect(Gate& gate);

QRectF boundingRect() const;
void updatePosition();
void addSubLineStarting(QPointF);

protected:
void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
         QWidget *widget = nullptr) override;

private:
    Gate *begin;
    QPointF startPoint;
    QVector<Gate*> ends;
    bool value; // Each wire has a value of true or false (1 or 0)
    QPolygonF beginNode, endNode;
    QColor myColor = Qt::black;
    QList<QPointF> subLineStarting;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *widget);

    Gate *myStartItem;
    Gate *myEndItem;
    //QPolygonF arrowHead;
    QPointF spriteOffset;
};

#endif // WIRE_H
