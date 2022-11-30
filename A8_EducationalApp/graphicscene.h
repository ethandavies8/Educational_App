
#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include "sceneitem.h"

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
QT_END_NAMESPACE


class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, MoveItem };

    explicit GraphicScene(QMenu *itemMenu, QObject *parent = nullptr);

public slots:
    void setMode(Mode mode);
    void setItemType(SceneItem::ItemType type);
    void setGateImage(QPixmap gateImage);

signals:
    void itemInserted(SceneItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool isItemChange(int type) const;
    SceneItem::ItemType myItemType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QColor myItemColor;
    QColor myLineColor;
    QPixmap currentGate;
};

#endif // GRAPHICSCENE_H
