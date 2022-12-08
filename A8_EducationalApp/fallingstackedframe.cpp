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
    }
    for (int i = 0; i < core->count(); ++i) {
        PhysicsScene* s = this->BuildScene(i);
        this->scenes.append(s);
        scenes.at(i)->setOut();
    }
    scenes.at(0)->fallIn();
}

/// @brief The only heap space allocated by FallingStackedFrame are the instances of PhysicsScene stored in scenes
FallingStackedFrame::~FallingStackedFrame(){
    for (int i = 0; i < this->scenes.count(); ++i) {
        delete this->scenes.at(i);
    }
}

/// @brief This method allows for manual replacement of a PhysicsScene, this allows for specific bounce values 
/// @param scene Index of the stacked frame to replace.
/// @param PSptr pointer to the new PhysicsScene, this class takes full responsibilty for deleting PSptr from the heap
void FallingStackedFrame::setPhysicsScene(int scene, PhysicsScene* PSptr){
    PhysicsScene* s = this->scenes.at(scene);
    this->scenes.replace(scene,PSptr);
    delete s; // clean up old scene 
}

/// @brief triggers to fall to a specific scene in the stacked widget.
/// @param scene index of the desired scene
void FallingStackedFrame::FallTo(int scene){
    scenes.at(scene)->setVisible(true);
    scenes.at(this->currentScene)->fallOut();
    scenes.at(scene)->fallIn();
    this->corelayout->setCurrentIndex(scene);
    this->currentScene = scene;
}

/// @brief This creates a default Physics scene for the given scene.
/// @param scene index to the scene that needs a connected physics scene
/// @return A pointer to the new physics scene.
PhysicsScene* FallingStackedFrame::BuildScene(int scene){
        QWidget* frame = this->core->widget(scene);
        PhysicsScene* ps = new PhysicsScene(frame);
        QList children = frame->children();
        frame->setWindowOpacity(0.0);
        frame->setAttribute(Qt::WA_NoSystemBackground);
        frame->setAttribute(Qt::WA_TranslucentBackground);
        for (int i = 0; i < children.count(); ++i) {
            if( QWidget* widget = dynamic_cast<QWidget*>(children.at(i)) )
            {
                ps->addBody(widget,((rand()%10)/100.0)+0.20);
            }else{
                std::cout<<"Non QWidget in "<<frame->accessibleName().toStdString()<<std::endl;
            }
        }
        return ps;
}
