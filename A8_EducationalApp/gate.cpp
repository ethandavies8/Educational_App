#include "gate.h"

//Constructors for gates.
Gate::Gate(){
    output = false;
}
SourceGate::SourceGate(){
    output = true;
}

bool Gate::getOutput(){
    return output;
}


//SET OUTPUTS FOR GATE TYPES<<<
void NOTGate::setOutput(){
    output = !inputs[0];
}
void ANDGate::setOutput(){
    if(inputs.size() >= 2){
        output = inputs[0] && inputs[1];
        for(int i = 2; i < (int)inputs.size(); i++)
            output = output & inputs[i];
    }
}
