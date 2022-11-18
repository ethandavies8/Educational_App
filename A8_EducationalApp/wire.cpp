#include "wire.h"

#include <QPen>

// Constructor for wire class
Wire::Wire(QGraphicsItem *parent) : QGraphicsLineItem(parent) {
    value = 0; // Preset wire's value to 0, won't care until update time
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

// Updates value held on wire and updates entire circuit
void Wire::updateValue(bool newValue) {

    value = newValue;

    // When this wire's value changes, all subsequent gate's outputs will change
    // LOOP THROUGH ENDS AND UPDATE THOSE GATES
}
