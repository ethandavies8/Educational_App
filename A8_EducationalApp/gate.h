#ifndef GATE_H
#define GATE_H

#include <vector>
#include <QGraphicsPolygonItem>

class Wire;
class Arrow;

class Gate : public QGraphicsPixmapItem
{

protected:
    std::vector<Wire*> inputs;
    Wire* outputWire;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
    enum GateType {
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
    void addInput(Wire* wire);
    void addOutput(Wire* wire);
    void removeInput(Wire* wire);
    void setOutput();

    void removeWire(Wire *wire);
    void addWire(Wire *wire);
    void removeWires();
private:
    GateType myType;
    QMenu *myContextMenu;

    QList<Wire *> wires;
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
