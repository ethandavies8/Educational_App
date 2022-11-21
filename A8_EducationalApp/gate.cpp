#include "gate.h"
#include "wire.h"

//Operator template to avoid a lot of repeated code.
template <typename op> bool applyOperator(std::vector<Wire> inputs){
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
Gate::Gate(){
}

//Normal gates can have any number of inputs
void Gate::addInput(Wire* wire){
    inputs.push_back(wire);
}

//Not gates can only have one input
void NOTGate::addInput(Wire* wire){
    if(inputs.size() == 0)
        inputs.push_back(wire);
}


//CONSTRUCTOR FOR SOURCE GATE, OUTPUT ALWAYS TRUE
SourceGate::SourceGate(){
    outputWire->updateValue(true);
}

//SET OUTPUTS FOR GATE TYPES<<<<<
void NOTGate::setOutput(){
    outputWire->updateValue(!inputs[0]->getValue());
}
//Using template that uses "std::bit_and" as the generic value.
void ANDGate::setOutput(){
    bool _and = applyOperator<std::bit_and<bool>>(inputs);
    outputWire.updateValue(_and);
}
void NANDGate::setOutput(){
    bool nand = !applyOperator<std::bit_and<bool>>(inputs);
    outputWire.updateValue(nand);
}
void ORGate::setOutput(){
    bool _or = applyOperator<std::bit_or<bool>>(inputs);
    outputWire.updateValue(_or);
}
void XORGate::setOutput(){
    bool _xor = applyOperator<std::bit_xor<bool>>(inputs);
    outputWire.updateValue(_xor);
}

