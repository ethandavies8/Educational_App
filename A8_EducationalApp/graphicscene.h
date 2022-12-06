
#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H


#include "wire.h"
#include "gate.h"

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
    enum Mode { InsertItem, InsertLine, MoveItem, RemoveItem };

    explicit GraphicScene(QMenu *itemMenu, QObject *parent = nullptr);

public slots:
    void setMode(GraphicScene::Mode mode);
    void setItemType(Gate::GateType type);
    void setGateImage(QPixmap gateImage);
    void setUpLevelOne();

signals:
    void itemInserted(Gate *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool isItemChange(int type) const;
    void insertItem(QGraphicsSceneMouseEvent *mouseEvent);
    Gate::GateType myItemType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QColor myLineColor ;
    QPixmap currentGate;
    SourceGate *sourceOne;
    SourceGate *sourceTwo;
    OutputGate *output;
};

#endif // GRAPHICSCENE_H
