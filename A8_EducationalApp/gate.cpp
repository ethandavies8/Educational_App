#include "gate.h"

//Constructors for gates.
Gate::Gate(){
    output = false;
}

bool Gate::getOutput(){
    return output;
}
//Normal gates can have any number of inputs
void Gate::addInput(bool wire){
    inputs.push_back(wire);
}

//Not gates can only have one input
void NOTGate::addInput(bool wire){
    if(inputs.size() == 0)
        inputs.push_back(wire);
}


//CONSTRUCTOR FOR SOURCE GATE, OUTPUT ALWAYS TRUE
SourceGate::SourceGate(){
    output = true;
}

//SET OUTPUTS FOR GATE TYPES<<<<<
void NOTGate::setOutput(){
    output = !inputs[0];
}
void ANDGate::setOutput(){
    if(inputs.size() >= 2){
        output = inputs[0] && inputs[1];
        for(int i = 2; i < (int)inputs.size(); i++)
            output = output & inputs[i];
    }
    else
        output = false;
}
void ORGate::setOutput(){
    if(inputs.size() >= 2)
        for(int i = 0; i < (int)inputs.size(); i++)
            output = output | inputs[i];
    else
        output = false;
}
void XORGate::setOutput(){
    if(inputs.size() >= 2)
        for(int i = 0; i < (int)inputs.size(); i++)
            output = output ^ inputs[i];
    else
        output = false;
}
