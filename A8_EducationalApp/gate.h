#ifndef GATE_H
#define GATE_H

#include <vector>

class Gate
{
protected:
    std::vector<bool> inputs;
    bool output;
    virtual void setOutput() = 0;

public:
    Gate();
    bool getOutput();
    void addInput(bool wire);
};

//A source gate doesn't inherit from gate and output is true;
class SourceGate{
    bool output;
    SourceGate();
};

//Not gate can only have 1 input.
class NOTGate : public Gate{
    void setOutput();
    void addInput(bool wire); //Overwrite output
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

class XORGate : public Gate{
    void setOutput();
};



#endif // GATE_H
