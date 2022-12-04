#include "fallingstackedframe.h"
#include <QStackedWidget>
#include <QObject>
#include "physicsscene.h"
#include <QStackedLayout>
#include <iostream>

FallingStackedFrame::FallingStackedFrame(QStackedWidget *core,QObject* parent)
    : QObject{parent}
{
    this->core = core;
    core->show();
    scenes = QVector<PhysicsScene*>();
    if(core == nullptr){
        return;
    }
    if( QStackedLayout* layout = dynamic_cast<QStackedLayout*>(core->layout()) )
    {
        this->corelayout = layout;
        layout->setStackingMode(QStackedLayout::StackAll);
        layout->setCurrentIndex(0);
    }else{
        std::cout<<"Shit"<<std::endl;

    }
    for (int i = 0; i < core->count(); ++i) {
        PhysicsScene* s = this->BuildScene(i);
        this->scenes.append(s);
        scenes.at(i)->setOut();
    }
    scenes.at(0)->fallIn();
}

FallingStackedFrame::~FallingStackedFrame(){
    for (int i = 0; i < this->scenes.count(); ++i) {
        delete this->scenes.at(i);
    }
}


void FallingStackedFrame::setPhysicsScene(int scene, PhysicsScene* PSptr){
    PhysicsScene* s = this->scenes.at(scene);
    this->scenes.replace(scene,PSptr);
    delete s;
}

void FallingStackedFrame::FallTo(int scene){
    scenes.at(scene)->setVisible(true);
    scenes.at(this->currentScene)->fallOut();
    scenes.at(scene)->fallIn();
    this->corelayout->setCurrentIndex(scene);
//    this->core->setCurrentIndex(scene);
//    scenes.at(this->currentScene)->setVisible(false);
//    scenes.at(scene)->setVisible(true);


    this->currentScene = scene;
}

PhysicsScene* FallingStackedFrame::BuildScene(int scene){
        QWidget* frame = this->core->widget(scene);
        PhysicsScene* ps = new PhysicsScene(frame);
        QList children = frame->children();
//        scenes.at(scene)->setVisible(false);
//        frame->setVisible(false);
        frame->setWindowOpacity(0.0);
        frame->setAttribute(Qt::WA_NoSystemBackground);
        frame->setAttribute(Qt::WA_TranslucentBackground);
        std::cout<<"posX:"<<frame->pos().x()<<" posY:" <<frame->pos().y() << std::endl;
        for (int i = 0; i < children.count(); ++i) {
            if( QWidget* widget = dynamic_cast<QWidget*>(children.at(i)) )
            {
                ps->addBody(widget,((rand()%10)/100.0)+0.30);
//                std::cout<<"posX:"<<widget->pos().x()<<" posY:" <<widget->pos().y() << std::endl;
//                widget->show();
//                widget->activateWindow();
//                widget->raise();
            }else{
                std::cout<<"Non QWidget in "<<frame->accessibleName().toStdString()<<std::endl;
            }
        }
        return ps;
}
