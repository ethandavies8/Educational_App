
#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE

//class Wire;

class SceneItem : public QGraphicsPixmapItem
{
public:
    enum ItemType {         AND,
                            OR,
                            NOT,
                            NAND,
                            NOR,
                            XOR,
                            Wire,
                            NoSelection};

    SceneItem(ItemType itemType, QMenu *contextMenu, QPixmap gateImage, QGraphicsItem *parent = nullptr);

    //void removeWire(Wire *wire);
    //void addWire(Wire *wire);
    //void removeWires();
    ItemType itemType() const { return myType; }
    QPolygonF polygon() const { return myPolygon; }
    QImage gateImage() const { return myGateImage;};
    QPixmap image() const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:
    ItemType myType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QImage myGateImage;
    QRect *imageBounds();
    //QList<Wire *> wires;
};

#endif // SCENEITEM_H
