#include "gate.h"
#include "wire.h"
#include <algorithm>

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

    // Operator template to avoid a lot of repeated code.
    // Does the generic op on input values and inverts if nott is true.
    template <typename op>
    void Gate::applyOperator(bool nott)
    {
        hasBeenChecked = true;

        bool tempLogic;
        if (inputs.size() >= 2 && outputWire != nullptr)
        {
            tempLogic = op()(inputs[0]->getValue(), inputs[1]->getValue());
            for (int i = 2; i < (int)inputs.size(); i++)
                tempLogic = op()(tempLogic, inputs[i]->getValue());

            if (nott)
                outputWire->updateValue(!tempLogic);
            else
                outputWire->updateValue(tempLogic);
        }
    }

    // Constructors for gates.
    Gate::Gate(GateType type, QMenu * contextMenu, QPixmap gateImage,
               QGraphicsItem * parent)
        : QGraphicsPixmapItem(parent), myType(type), myContextMenu(contextMenu)
    {
        QPixmap *map;
        switch(type){
        case(Source):
            map = new QPixmap(":/icons/line.png");
            setPixmap(*map);
        break;
        case(Output):
            map = new QPixmap(":/icons/line.png");
            setPixmap(*map);
            break;
        default:
            setPixmap(gateImage.scaled(90,55));
            setFlag(QGraphicsItem::ItemIsMovable, true);
            break;
        }        
        height = 55;
        width = 90;
        setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }

    Gate::GateType Gate::getType()
    {
        return myType;
    }

    Gate::Gate()
    {
    }

    int Gate::returnWirePlace(Wire *wire)
    {
        for(int i = 0; i < (int)inputs.size(); i++) {
            if(wire == inputs[i])
                return i+1;
        }
        return 1;
    }

    void Gate::setOutput(){}

    // Get rid of a wire.
    void Gate::removeOutput()
    {
        outputWire = nullptr;
    }
    bool Gate::hasOutput()
    {
        if (outputWire == nullptr)
            return false;
        else
            return true;
    }
    // Normal gates can have any number of inputs
    void Gate::addInput(Wire * wire)
    {
        inputs.push_back(wire);
    }
    // Set outputWire to a wire.
    void Gate::addOutput(Wire * wire)
    {
        outputWire = wire;
    }

    // Clear the flag of this gate and previous gates
    void Gate::clearFlag()
    {

        hasBeenChecked = false;

        for (Wire *wire : inputs)
        {
            if (wire)
            {
                Gate *gateInput = wire->getBeginningOfWire();
                if (gateInput->hasBeenChecked)
                    gateInput->clearFlag();
            }
        }
    }

    // Not gates and output gates can only have one input
    void NOTGate::addInput(Wire * wire)
    {
        if (inputs.size() == 0)
            inputs.push_back(wire);
        else
            inputs[0] = wire;
    }

    // Remove a certain wire from the inputs list.
    void Gate::removeInput(Wire * wire)
    {
        inputs.erase(std::remove(inputs.begin(), inputs.end(), wire), inputs.end());
    }

    // SET OUTPUTS FOR GATE TYPES<<<<<
    void NOTGate::setOutput()
    {

        hasBeenChecked = true;

        if (outputWire != nullptr)
        {
            if (inputs.size() > 0)
                outputWire->updateValue(!inputs[0]->getValue());
        }
    }
    void SourceGate::setOutput() {}
    void SourceGate::setOutput(bool output)
    {
        if (outputWire != nullptr)
            outputWire->updateValue(output);
    }
    void OutputGate::setOutput()
    {
        // When the output gate has been reached, no circular dependency has been found
        clearFlag();
    }
    // Using template that uses "std::bit_and/or/xor" as the generic value,
    // and a bool value that tell if the result should be inverted.
    void ANDGate::setOutput()
    {
        // Apply a bit and on gate inputs
        applyOperator<std::bit_and<bool>>(false);
    }
    void NANDGate::setOutput()
    {
        // Apply a bit and on gate inputs then invert it because parameter is "true"
        applyOperator<std::bit_and<bool>>(true);
    }
    void ORGate::setOutput()
    {
        // Apply a bit or on gate inputs
        applyOperator<std::bit_or<bool>>(false);
    }
    void NORGate::setOutput()
    {
        // Apply a bit or on gate inputs and invert it.
        applyOperator<std::bit_or<bool>>(true);
    }
    void XORGate::setOutput()
    {
        // Apply a bit xor on gate inputs
        applyOperator<std::bit_xor<bool>>(false);
    }
    void XNORGate::setOutput()
    {
        // Apply a bit xor on gate inputs and invert it.
        applyOperator<std::bit_xor<bool>>(true);
    }

    // SourceGate special methods
    void SourceGate::addInput() {}

    void SourceGate::clearFlag() {}

    // OutputGate special methods
    bool OutputGate::getOutput()
    {
        if (inputs.size() == 1)
            return inputs[0]->getValue();
        else
            return false;
    }
    void OutputGate::addInput(Wire * wire)
    {
        if (inputs.size() == 0)
            inputs.push_back(wire);
        else
            inputs[0] = wire;
    }
    void OutputGate::addOutput() {}
