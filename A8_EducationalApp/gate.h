#ifndef GATE_H
#define GATE_H

#include <vector>
#include <QGraphicsPolygonItem>

class Wire;

class Gate : public QGraphicsItem
{

protected:
    std::vector<Wire*> inputs;
    Wire* outputWire;

public:
    enum GateType {
            AND,
            OR,
            NOT,
            NAND,
            NOR,
            XOR,
    };
    Gate(GateType, QGraphicsItem *parent);
    void addInput(Wire* wire);
    void addOutput(Wire* wire);
    void removeInput(Wire* wire);
    virtual void setOutput() = 0;
private:
    GateType myType;
};

//A source gate's output is true;
class SourceGate : public Gate{
    void addInput();
    void setOutput(bool);
    void setOutput();
};

class OutputGate : public Gate{
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
