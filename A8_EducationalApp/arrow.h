
#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

//class SceneItem;
class Gate;

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(Gate *startItem, Gate *endItem,
          QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { myColor = color; }
    Gate *startItem() const { return myStartItem; }
    Gate *endItem() const { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    Gate *myStartItem;
    Gate *myEndItem;
    QPolygonF arrowHead;
    QColor myColor = Qt::black;
    QPointF spriteOffset;
};
#endif // ARROW_H
