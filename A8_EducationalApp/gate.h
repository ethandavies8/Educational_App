#ifndef GATE_H
#define GATE_H

#include <vector>
#include <QGraphicsPolygonItem>

class Wire;

class Gate : public QGraphicsPixmapItem
{

protected:
    std::vector<Wire *> inputs;
    Wire *outputWire = nullptr;
    bool hasBeenChecked = false;

public:
    enum GateType
    {
        AND,
        OR,
        NOT,
        NAND,
        NOR,
        XOR,
        NoSelection,
    };
    // Gate(GateType, QPixmap, QGraphicsItem *parent);
    Gate(GateType itemType, QMenu *contextMenu, QPixmap gateImage, QGraphicsItem *parent = nullptr);
    template <typename op>
    void applyOperator(bool);
    Gate();
    void addInput(Wire *wire);
    void addOutput(Wire *wire);
    bool hasOutput();
    void removeOutput();
    void removeInput(Wire *wire);
    void setOutput();
    GateType getType();

private:
    GateType myType;
    QMenu *myContextMenu;
    QList<Wire *> wires;
    void removeInput(Wire *wire);
    void clearFlag();
    bool gateHasBeenChecked() { return hasBeenChecked; }
    virtual void setOutput() = 0;
};

// A source gate's output is true;
class SourceGate : public Gate
{

public:
    using Gate::Gate;
    void addInput();
    void setOutput();
    void setOutput(bool);
    void clearFlag();
};

class OutputGate : public Gate
{

public:
    using Gate::Gate;
    void addOutput();
    void addInput(Wire *wire);
    void setOutput();
    bool getOutput();
};

// Not gate can only have 1 input.
class NOTGate : public Gate
{
    using Gate::Gate;
    void setOutput();
    void addInput(Wire *wire); // Overwrite addInput
};

class ANDGate : public Gate
{
    using Gate::Gate;
    void setOutput();
};

class NANDGate : public Gate
{
    using Gate::Gate;
    void setOutput();
};

class ORGate : public Gate
{
    using Gate::Gate;
    void setOutput();
};

class NORGate : public Gate
{
    using Gate::Gate;
    void setOutput();
};

class XORGate : public Gate
{
    using Gate::Gate;
    void setOutput();
};

class XNORGate : public Gate
{
    using Gate::Gate;
    void setOutput();
};

#endif // GATE_H
