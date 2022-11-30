
#include "sceneitem.h"
//#include "arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QLabel>
#include <QPainter>

#include <iostream>

SceneItem::SceneItem(ItemType sceneItemType, QMenu *contextMenu,QPixmap gateImage,
                         QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), myType(sceneItemType)
    , myContextMenu(contextMenu)
{

    switch (myType) {
        case AND:

            break;
        default:

            break;
    }
    setPixmap(gateImage);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}



/*
void SceneItem::removeWire(Wire *arrow)
{
    arrows.removeAll(arrow);
}

void SceneItem::removeWires()
{

    const auto wiresCopy = wires;
    for (Wire *wire : wiresCopy) {
        wire->startItem()->removeWire(wire);
        wire->endItem()->removeWire(wire);
        scene()->removeItem(wire);
        delete wire;
    }
}

void SceneItem::addWire(Wire *wire)
{
    wires.append(wire);
}

*/

void SceneItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant SceneItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
       // for (Wire *wire : qAsConst(wire))
           // wire->updatePosition();
    }

    return value;
}

