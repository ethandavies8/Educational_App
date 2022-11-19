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


};
//A source gate always has an output of true;
class SourceGate : public Gate{
    SourceGate();
};

//Not gate can only have 1 input.
class NOTGate : public Gate{
    void setOutput();

};

class ANDGate : public Gate{
    void setOutput();
};

#endif // GATE_H
