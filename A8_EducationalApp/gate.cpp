#include "gate.h"
#include "wire.h"
#include <algorithm>

//Operator template to avoid a lot of repeated code.
template <typename op> bool applyOperator(std::vector<Wire*> inputs){
    bool tempLogic;
    if(inputs.size() >= 2){
        tempLogic = op()(inputs[0]->getValue(), inputs[1]->getValue());
        for(int i = 2; i < (int)inputs.size(); i++)
            tempLogic = op()(tempLogic, inputs[i]->getValue());
        return tempLogic;
    }
    else
        return false;
}

//Constructors for gates.
Gate::Gate(){
}
void Gate::removeOutput(){
    outputWire = nullptr;
}

//Normal gates can have any number of inputs
void Gate::addInput(Wire* wire){
    inputs.push_back(wire);
}
void Gate::addOutput(Wire* wire){
    outputWire = wire;
}


//Not gates and output gates can only have one input
void NOTGate::addInput(Wire* wire){
    if(inputs.size() == 0)
        inputs.push_back(wire);
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

