#ifndef GATE_H
#define GATE_H

#include <vector>

class Wire;

class Gate
{
protected:
    std::vector<Wire*> inputs;
    Wire* outputWire = nullptr;
    bool hasBeenChecked = false;

public:
    template <typename op> void applyOperator(bool);
    Gate();
    void addInput(Wire* wire);
    void addOutput(Wire* wire);
    bool hasOutput();
    void removeOutput();
    void removeInput(Wire* wire);
    void clearFlag();
    bool gateHasBeenChecked() { return hasBeenChecked; }
    virtual void setOutput() = 0;
};

//A source gate's output is true;
class SourceGate : public Gate{
public:
    void addInput();
    void setOutput();
    void setOutput(bool);
    void clearFlag();
};

class OutputGate : public Gate{
public:
    void addOutput();
    void addInput(Wire* wire);
    void setOutput();
    bool getOutput();
};

//Not gate can only have 1 input.
class NOTGate : public Gate{
    void setOutput();
    void addInput(Wire* wire); //Overwrite addInput
};

class ANDGate : public Gate{
public:
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
