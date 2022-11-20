#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <QWidget>
#include "Box2D/Box2D.h"

class PhysicsObject : public QWidget
{
    Q_OBJECT
public:
    explicit PhysicsObject(QWidget *parent,b2World* world,int worldChannel);
    ~PhysicsObject();
public slots:
    void UpdateGuiPosition();
    void fallIn();
    void fallOut();
private:
    QPoint    worldToGuiPos(QPointF pos);
    QPointF   guiToWorldPos(QPoint pos);
    void      setWorldChannel(int channel);

    QWidget*  widget;
    QPoint    home;
    b2Body*   body;
    QPoint    offset;
};
#endif // PHYSICSOBJECT_H
