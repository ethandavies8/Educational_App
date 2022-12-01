
#include "graphicscene.h"

#include <QGraphicsSceneMouseEvent>


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
    //SceneItem *item;
    Gate *item;
    switch (myMode) {
        case InsertItem:
            item = new Gate(myItemType, myItemMenu, currentGate);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;

        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

       case MoveItem:
            //move logic
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
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
            startItem->addWire(wire);
            endItem->addWire(wire);
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



