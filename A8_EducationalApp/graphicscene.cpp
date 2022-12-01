
#include "graphicscene.h"

#include <QGraphicsSceneMouseEvent>
#include <iostream>



GraphicScene::GraphicScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = Gate::NoSelection;
    line = nullptr;
    myLineColor = Qt::black;
}

void GraphicScene::setMode(Mode mode)
{
    myMode = mode;
}

void GraphicScene::setItemType(Gate::GateType type)
{
    myItemType = type;
}

void GraphicScene::setGateImage(QPixmap gateImage){
    currentGate = gateImage;
}

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    switch (myMode) {
        case InsertItem:
        insertItem(mouseEvent);
        break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

       case MoveItem:
            //move logic

        case RemoveItem:
        //removeItem(item)
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicScene::insertItem(QGraphicsSceneMouseEvent *mouseEvent){
    Gate *item;

    switch(myItemType){
        case Gate::AND:
        item = new ANDGate(myItemType, myItemMenu, currentGate);
        std::cout << "created and gate" <<std::endl;
         break;
    case Gate::OR:
        item = new ORGate(myItemType, myItemMenu, currentGate);
        std::cout << "created or gate" <<std::endl;
         break;
    case Gate::NOT:
        item = new NOTGate(myItemType, myItemMenu, currentGate);
        std::cout << "created not gate" <<std::endl;
         break;
    case Gate::NOR:
        item = new NOTGate(myItemType, myItemMenu, currentGate);
        std::cout << "created nor gate" <<std::endl;
         break;
    case Gate::NAND:
        item = new NOTGate(myItemType, myItemMenu, currentGate);
        std::cout << "created nand gate" <<std::endl;
         break;
    case Gate::XOR:
        item = new NOTGate(myItemType, myItemMenu, currentGate);
        std::cout << "created xor gate" <<std::endl;
        break;
    default:
        item = new Gate(myItemType, myItemMenu, currentGate);
        std::cout << "created default gate" <<std::endl;
        break;
    }

    addItem(item);
    item->setPos(mouseEvent->scenePos());
    emit itemInserted(item);

}

void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != nullptr && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;


        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == Gate::Type &&
            endItems.first()->type() == Gate::Type &&
            startItems.first() != endItems.first()) {
            Gate *startItem = qgraphicsitem_cast<Gate *>(startItems.first());
            Gate *endItem = qgraphicsitem_cast<Gate *>(endItems.first());


            Wire *wire = new Wire(startItem, endItem);
            wire->setColor(myLineColor);

            startItem->addOutput(wire);
            endItem->addInput(wire);
            wire->setZValue(-1000.0);
            addItem(wire);
            wire->updatePosition();
        }
    }

    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


bool GraphicScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}



