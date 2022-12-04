#ifndef PHYSICSSCENE_H
#define PHYSICSSCENE_H

#include "qwidget.h"
#include <QObject>
#include <Box2D/Box2D.h>
#include "physicsobject.h"
#include <QVector>

class PhysicsScene : public QWidget
{
    Q_OBJECT
public:
    explicit PhysicsScene(QWidget *parent);
    ~PhysicsScene();
    void addBody(QWidget* widget,float bounce=0.3);
public slots:
    void fallIn();
    void fallOut();
    void setOut();
private slots:
    void updateWorld();

signals:
    void postSimulation();
    void TriggerFallIn();
    void TriggerFallOut();
    void TriggersetOut();
private:
    int frameHeight;
    b2World* world;
    QVector<PhysicsObject*> bodies;
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;
};

#endif // PHYSICSSCENE_H
