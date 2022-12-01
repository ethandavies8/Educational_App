#include "wire.h"
#include "gate.h"

#include <QPen>
#include <QPainter>
#include <QtMath>

// Constructor for wire class
/*
Wire::Wire(Gate& beginGate, Gate& endGate, QGraphicsItem *parent) : QGraphicsLineItem(parent), begin(&beginGate) {
    value = 0; // Preset wire's value to 0, won't care until update time
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    ends.push_back(&endGate);

    // waiting gate class inherit from GraphicsItems
    //startPoint = beginGate->pos();
}
*/

Wire::Wire(Gate *startGate, Gate *endGate, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), myStartItem(startGate), myEndItem(endGate)
{
    value = 0;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    ends.push_back(endGate);
    spriteOffset.setX(25);
    spriteOffset.setY(25);
}


/*
QPainterPath Wire::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
*/

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{
    QPen pen(myColor);
      painter->setPen(pen);

       // set the start to be the output place
      QPointF startPoint = myStartItem->pos();
       startPoint.setX(startPoint.x() + 100);
       startPoint.setY(startPoint.y() + 25);
      QPointF endPoint = myEndItem->pos();
      endPoint.setX(endPoint.x() + 30);
      endPoint.setY(endPoint.y() + 25);
       //QLineF centerLine(startPoint, ends.at(0)->pos());
       QLineF centerLine(startPoint, endPoint);

       setLine(centerLine);
       painter->drawLine(line());
}

// Updates value held on wire and updates entire circuit
void Wire::updateValue(bool newValue) {

    value = newValue;

    // When this wire's value changes, all subsequent gate's outputs will change
    for (Gate* gate : ends) {
        gate->setOutput();
    }
}

// Add a gate to the end of this wire
void Wire::connect(Gate& gate) {
    ends.push_back(&gate);
    gate.addInput(this);
}

// Remove a connection between this wire and a gate
void Wire::disconnect(Gate& gate) {
    ends.erase(std::remove(ends.begin(), ends.end(), &gate), ends.end()); // Remove gate from vector
    gate.removeInput(this);
}

// Telling the scene, how big is the wire
QRectF Wire::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
            .adjusted(-extra, -extra, extra, extra);
}



// Update the wire when gate position is changed.
void Wire::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}

// Add the sub wire that is connecting to a new gate
void Wire::addSubLineStarting(QPointF newSubLineStarting)
{
    subLineStarting.append(newSubLineStarting);
}

// Drawing on the gamescene, comment out because gate class is not inherit GraphicsItems
// so is not happy with begin->pos();
//void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    QPen pen(myColor);
//    painter->setPen(pen);

//    // set the start to be the output place
//    QPointF startPoint = begin->pos();
//    startPoint.setX(startPoint.x() + 100);
//    startPoint.setY(startPoint.y() + 50);

//    QLineF centerLine(startPoint, ends.at(0)->pos());

//    setLine(centerLine);
//    painter->drawLine(line());

//    for(int i = 0; i < subLineStarting.count(); i++){
//        QLineF subLine(subLineStarting[i], ends.at(i+1)->pos());
//        setLine(subLine);
//        painter->drawLine(line());
//    }
//}

