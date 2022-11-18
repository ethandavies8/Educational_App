#ifndef WIRE_H
#define WIRE_H

#include <QGraphicsLineItem>

class Wire : public QGraphicsLineItem
{
public:
    Wire(QGraphicsItem *parent = nullptr); // Need to include gate classes in constructor (begin, end)

    void updateValue(bool newValue); // Updates this wire's value, then updates the next gates
    bool getValue() const { return value; }
    // Connect method will connect the end of this wire to another gate
    // Disconnect method will disconnect the end of this wire from the desired gate

private:
    // Gate pointer "begin" for what this wire starts from
    // Gate pointer list "ends" for what this wire ends at
    bool value; // Each wire has a value of true or false (1 or 0)
    QPolygonF beginNode, endNode;
    QColor myColor = Qt::black;
};

#endif // WIRE_H
