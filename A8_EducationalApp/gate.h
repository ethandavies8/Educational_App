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
    void removeWire(Wire *wire);
    void addWire(Wire *wire);
    void removeWires();

private:
    GateType myType;
    QMenu *myContextMenu;
    QList<Wire *> wires;
};

// A source gate's output is true;
class SourceGate : public Gate
{
    using Gate::Gate;

public:
    void addInput();
    void setOutput();
    void setOutput(bool);
};

class OutputGate : public Gate
{
    using Gate::Gate;

public:
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
