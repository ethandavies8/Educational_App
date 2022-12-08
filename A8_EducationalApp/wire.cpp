/*
* The Cuties
* CS 3505
* A8-An-Educational-App
*
* Implementation file for Wire object
*/

#include "wire.h"
#include "gate.h"

#include <QPen>
#include <QPainter>
#include <QtMath>
#include <ostream>

// Constructor for wire class
Wire::Wire(Gate *startGate, Gate *endGate, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startGate(startGate) {

    value = false; // DEFAULT

    // Establish connections in gates
    startGate->addOutput(this);
    connect(endGate);

    // Drawing settings
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

// Draw wires
void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *) {

    QPen pen(myColor);
      painter->setPen(pen);

      for(int i = 0; i < ends.count(); i++) {
          QPointF startPoint = startGate->pos();
          startPoint.setX(startPoint.x() + startGate->getWidth());
          startPoint.setY(startPoint.y() + startGate->getHeight() / 2);
          QPointF endPoint = ends[i]->pos();
          if(ends[i]->getType() == Gate::OR || ends[i]->getType() == Gate::XOR || ends[i]->getType() == Gate::NOR)
          {
              endPoint.setX(endPoint.x() + 20);
          }
          endPoint.setY(endPoint.y() + ends[i]->getHeight() / (ends[i]->wireConnectedCount() + 1) * ends[i]->returnWirePlace(this));

          QLineF centerLine(startPoint, endPoint);

          setLine(centerLine);
          painter->drawLine(line());
      }
}

// Find if the gate is already connected or not
bool Wire::containGate(Gate *gateToCheck) {
    return std::find(ends.begin(), ends.end(), gateToCheck) != ends.end();
}

// Updates value held on wire and updates the gates this wire is connected to
void Wire::updateValue(bool newValue) {

    value = newValue;

    // When this wire's value changes, all subsequent gate's outputs will change
    for (Gate* gate : ends) {
        if (gate) {
            if (!(gate->gateHasBeenChecked()))
                gate->setOutput();
        }
    }
}

// Add a gate to the end of this wire
void Wire::connect(Gate* gate) {
    ends.push_back(gate);
    gate->addInput(this);
}

// Remove a connection between this wire and a gate
void Wire::disconnect(Gate* gate) {
    ends.erase(std::remove(ends.begin(), ends.end(), gate), ends.end()); // Remove gate from vector
    gate->removeInput(this);
}

// Remove all connections of this wire
void Wire::removeConnections() {

    if (startGate)
        startGate->removeOutput();

    for (Gate* gate : ends) {
        if (gate)
            gate->removeInput(this);
    }
}

// Telling the scene how big the wire is
QRectF Wire::boundingRect() const {

    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

// Update the wire when gate position is changed.
void Wire::updatePosition() {

    for(int i = 0; i < ends.size(); i++) {
        QLineF line(mapFromItem(startGate, 0, 0), mapFromItem(ends[i], 0, 0));
        setLine(line);
    }
}
