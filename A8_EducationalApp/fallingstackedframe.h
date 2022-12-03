#ifndef FALLINGSTACKEDFRAME_H
#define FALLINGSTACKEDFRAME_H

#include <QMainWindow>
#include <QObject>
#include "physicsscene.h"
#include <QStackedWidget>


class FallingStackedFrame : public QObject
{
    Q_OBJECT
public:
    explicit FallingStackedFrame(QStackedWidget *core,QObject* parent = nullptr);
    ~FallingStackedFrame();
    void setPhysicsScene(int scene, PhysicsScene* PSptr);
public slots:
    void FallTo(int scene);
private:
    PhysicsScene* BuildScene(int scene);
    QVector<PhysicsScene*> scenes;
    int currentScene = 0;
    QStackedWidget* core;
};

#endif // FALLINGSTACKEDFRAME_H
