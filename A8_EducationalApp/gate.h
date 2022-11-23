#ifndef GATE_H
#define GATE_H

#include <vector>

class Wire;

class Gate
{
protected:
    std::vector<Wire*> inputs;
    Wire* outputWire;

public:
    Gate();
    void addInput(Wire* wire);
    void removeInput(Wire* wire);
    virtual void setOutput() = 0;
};

//A source gate's output is true;
class SourceGate : Gate{
    SourceGate();
};

//Not gate can only have 1 input.
class NOTGate : public Gate{
    void setOutput();
    void addInput(Wire* wire); //Overwrite addInput
};

class ANDGate : public Gate{
    void setOutput();
};

class NANDGate : public Gate{
    void setOutput();
};

class ORGate : public Gate{
    void setOutput();
};

class NORGate : public Gate{
    void setOutput();
};

class XORGate : public Gate{
    void setOutput();
};

class XNORGate : public Gate{
    void setOutput();
};



#endif // GATE_H
