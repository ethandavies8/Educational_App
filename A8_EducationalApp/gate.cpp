#include "gate.h"
#include "wire.h"
//#include "arrow.h"
#include <algorithm>

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

//Operator template to avoid a lot of repeated code.
template <typename op> bool applyOperator(std::vector<Wire*> inputs){
    bool tempLogic;
    if(inputs.size() >= 2){
        tempLogic = op()(inputs[1]->getValue(), inputs[1]->getValue());
        for(int i = 2; i < (int)inputs.size(); i++)
            tempLogic = op()(tempLogic, inputs[i]->getValue());
        return tempLogic;
    }
    else
        return false;
}

//Constructors for gates.
/*
Gate::Gate(GateType type, QPixmap gateMap, QGraphicsItem *parent) : QGraphicsPixmapItem(parent){
    myType = type;
    setPixmap(gateMap);
}
*/
Gate::Gate(GateType type, QMenu *contextMenu,QPixmap gateImage,
                         QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), myType(type)
    , myContextMenu(contextMenu)
{

    switch (myType) {
        case AND:

            break;
        default:

            break;
    }
    setPixmap(gateImage);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void Gate::removeWire(Wire *wire)
{
    wires.removeAll(wire);
}

void Gate::removeWires()
{

    const auto wiresCopy = wires;
    for (Wire *wire : wiresCopy) {
        wire->startItem()->removeWire(wire);
        wire->endItem()->removeWire(wire);
        scene()->removeItem(wire);
        delete wire;
    }
}

void Gate::addWire(Wire *wire)
{
    wires.append(wire);
}


//Normal gates can have any number of inputs
void Gate::addInput(Wire* wire){
    inputs.push_back(wire);
    setOutput();
}
void Gate::addOutput(Wire* wire){
    outputWire = wire;
}

void Gate::setOutput(){

}

//Not gates and output gates can only have one input
void NOTGate::addInput(Wire* wire){
    if(inputs.size() == 0)
        inputs.push_back(wire);
    setOutput();
}

//Remove a certain wire from the inputs list.
void Gate::removeInput(Wire* wire){
    inputs.erase(std::remove(inputs.begin(), inputs.end(), wire), inputs.end());
}

//SET OUTPUTS FOR GATE TYPES<<<<<
void NOTGate::setOutput(){
    if(outputWire != nullptr){
        if(inputs.size() > 0)
            outputWire->updateValue(!inputs[0]->getValue());
        else
            outputWire->updateValue(false);
    }
}
//SourceGate overwritten methods
void SourceGate::setOutput(){}
void SourceGate::setOutput(bool output){
    outputWire->updateValue(output);
}
void SourceGate::addInput(){}

//OutputGate overwritten methods
bool OutputGate::getOutput(){
    if(inputs.size() == 1)
        return inputs[0]->getValue();
    else return false;
}
void OutputGate::addInput(Wire* wire){
    if(inputs.size() == 0)
        inputs.push_back(wire);
}
void OutputGate::addOutput(){}
void OutputGate::setOutput(){}

//Using template that uses "std::bit_and/or/xor" as the generic value.
void ANDGate::setOutput(){
    bool _and = applyOperator<std::bit_and<bool>>(inputs);
    outputWire->updateValue(_and);
}
void NANDGate::setOutput(){
    bool nand = !applyOperator<std::bit_and<bool>>(inputs);
    outputWire->updateValue(nand);
}
void ORGate::setOutput(){
    bool _or = applyOperator<std::bit_or<bool>>(inputs);
    outputWire->updateValue(_or);
}
void NORGate::setOutput(){
    bool nor = !applyOperator<std::bit_or<bool>>(inputs);
    outputWire->updateValue(nor);
}
void XORGate::setOutput(){
    bool _xor = applyOperator<std::bit_xor<bool>>(inputs);
    outputWire->updateValue(_xor);
}
void XNORGate::setOutput(){
    bool xnor = !applyOperator<std::bit_xor<bool>>(inputs);
    outputWire->updateValue(xnor);
}


void Gate::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   // scene()->clearSelection();
    //setSelected(true);
    //myContextMenu->exec(event->screenPos());
}

QVariant Gate::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //if (change == QGraphicsItem::ItemPositionChange) {
      //  for (Arrow *arrow : qAsConst(wires))
        //    arrow->updatePosition();
    //}

    return value;
}

