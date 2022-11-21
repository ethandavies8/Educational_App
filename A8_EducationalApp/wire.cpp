#include "wire.h"

#include <QPen>

// Constructor for wire class
Wire::Wire(Gate& beginGate, Gate& endGate, QGraphicsItem *parent) : QGraphicsLineItem(parent), begin(&beginGate) {
    value = 0; // Preset wire's value to 0, won't care until update time
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    ends.push_back(&endGate);
}

// Updates value held on wire and updates entire circuit
void Wire::updateValue(bool newValue) {

    value = newValue;

    // When this wire's value changes, all subsequent gate's outputs will change
    /*
    for (Gate* gate : ends) {
        gate->setOutput();
    }
    */
}

// Add a gate to the end of this wire
void Wire::connect(Gate& gate) {
    // ends.push_back(&gate);
}

// Remove a connection between this wire and a gate
void Wire::disconnect(Gate& gate) {
    // ends.remove(&gate);
}
