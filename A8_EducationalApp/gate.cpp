#include "gate.h"

//Constructors for gates.
Gate::Gate(){
}

//Normal gates can have any number of inputs
void Gate::addInput(Wire wire){
    inputs.push_back(wire);
}

//Not gates can only have one input
void NOTGate::addInput(Wire wire){
    if(inputs.size() == 0)
        inputs.push_back(wire);
}


//CONSTRUCTOR FOR SOURCE GATE, OUTPUT ALWAYS TRUE
SourceGate::SourceGate(){
    outputWire.updateValue(true);
}

//SET OUTPUTS FOR GATE TYPES<<<<<
void NOTGate::setOutput(){
    outputWire.updateValue(!inputs[0].getValue());
}
void ANDGate::setOutput(){
    bool tempLogic;
    if(inputs.size() >= 2){
        tempLogic = inputs[0].getValue() && inputs[1].getValue();
        for(int i = 2; i < (int)inputs.size(); i++)
            tempLogic = tempLogic & inputs[i].getValue();
        outputWire.updateValue(tempLogic);
    }
    else
        outputWire.updateValue(false);
}
//The only difference between the NANDGate setOutput is the "!" after the for loop
void NANDGate::setOutput(){
    bool tempLogic;
    if(inputs.size() >= 2){
        tempLogic = inputs[0].getValue() && inputs[1].getValue();
        for(int i = 2; i < (int)inputs.size(); i++)
            tempLogic = tempLogic & inputs[i].getValue();
        outputWire.updateValue(!tempLogic);
    }
    else
        outputWire.updateValue(false);
}
void ORGate::setOutput(){
    bool tempLogic;
    if(inputs.size() >= 2){
        tempLogic = inputs[0].getValue() || inputs[1].getValue();
        for(int i = 2; i < (int)inputs.size(); i++)
            tempLogic = tempLogic | inputs[i].getValue();
        outputWire.updateValue(tempLogic);
    }
    else
        outputWire.updateValue(false);
}
void XORGate::setOutput(){
    bool tempLogic;
    if(inputs.size() >= 2){
        tempLogic = inputs[0].getValue() ^ inputs[1].getValue();
        for(int i = 2; i < (int)inputs.size(); i++)
            tempLogic = tempLogic ^ inputs[i].getValue();
        outputWire.updateValue(tempLogic);
    }
    else
        outputWire.updateValue(false);
}
