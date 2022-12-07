#include "wire.h"
#include "gate.h"

#include <QPen>
#include <QPainter>
#include <QtMath>
#include <ostream>

// Constructor for wire class


Wire::Wire(Gate& beginGate, Gate& endGate, QGraphicsItem *parent) : QGraphicsLineItem(parent), startGate(&beginGate) {
    value = 0; // Preset wire's value to 0, won't care until update time
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    ends.push_back(&endGate);
    beginGate.addOutput(this);
    endGate.addInput(this);

}


Wire::Wire(Gate *startGate, Gate *endGate, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startGate(startGate), endGate(endGate)
{

    value = 0;
    startGate->addOutput(this);
    connect(endGate);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setUpOffset();
}

void Wire::setUpOffset(){
    Gate::GateType startType = startGate->getType();
    Gate::GateType endType = endGate->getType();

    switch(startType){

    case Gate::AND:
        xStartOffset = 95;
        yStartOffset = 25;
     break;
    case Gate::OR:
        xStartOffset = 75;
        yStartOffset = 35;
     break;
    case Gate::NOT:
        xStartOffset = 95;
        yStartOffset = 25;
     break;
    case Gate::NOR:
        xStartOffset = 95;
        yStartOffset = 25;
     break;
    case Gate::NAND:
        xStartOffset = 95;
        yStartOffset = 25;
     break;
    case Gate::XOR:
        xStartOffset = 95;
        yStartOffset = 25;
    break;
    default:
        xStartOffset = 50;
        yStartOffset = 25;
    break;
    }

    switch(endType){

    case Gate::AND:
        xEndOffset = 30;
        yEndOffset = 25;
     break;
    case Gate::OR:
        xEndOffset = 24;
        yEndOffset = 40;
     break;
    case Gate::NOT:
        xEndOffset = 30;
        yEndOffset = 25;
     break;
    case Gate::NOR:
        xEndOffset = 30;
        yEndOffset = 25;
     break;
    case Gate::NAND:
        xEndOffset = 30;
        yEndOffset = 25;
     break;
    case Gate::XOR:
        xEndOffset = 30;
        yEndOffset = 25;
    break;
    default:
        xStartOffset = 50;
        yStartOffset = 25;
    break;
    }


}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{
    QPen pen(myColor);
      painter->setPen(pen);

       // set the start to be the output place

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
          //QLineF centerLine(startPoint, ends.at(0)->pos());

          QLineF centerLine(startPoint, endPoint);

          setLine(centerLine);
          painter->drawLine(line());
      }
}

// Updates value held on wire and updates entire circuit
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
    for(int i = 0; i < ends.size(); i++){
        QLineF line(mapFromItem(startGate, 0, 0), mapFromItem(ends[i], 0, 0));
        setLine(line);
    }
}

