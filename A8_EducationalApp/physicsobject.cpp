#include "physicsobject.h"
#include "Box2D/Box2D.h"
#include <QPointF>
#include <iostream>
#include <math.h>


PhysicsObject::PhysicsObject(QWidget *parent,b2World* world,int worldChannel,float bounce)
    : QWidget{parent}
{
    // Define the dynamic body. We set its position and call the body factory.
    this->widget = parent;
    this->home = parent->pos();
    this->offset = QPoint(0,0); // The world channel sets the x position of the object in the world so they do not interact
    this->setWorldChannel(worldChannel);

    QPointF homef = this->guiToWorldPos(this->home);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(homef.x(), -5);
    body = world->CreateBody(&bodyDef);
//    std::cout<< parent->objectName().toStdString()<<" -> X:"<<home.x() <<" Y:"<<home.y() <<std::endl;
//    std::cout<< parent->objectName().toStdString()<<" -> X:"<<homef.x()<<" Y:"<<homef.y()<<std::endl;


    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = bounce;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);


}


PhysicsObject::~PhysicsObject(){
    // Both of the pointers in the class are managed elsewere, the body's memory is cleared with the world. and the widget is removed with the parent(main Window).
}

void PhysicsObject::UpdateGuiPosition(){
    if(this->body->IsAwake()){
        b2Vec2 pos = this->body->GetPosition();

        QPoint guiPos = worldToGuiPos(QPointF(pos.x,pos.y));
        guiPos.setX(home.x());
//        std::cout<< this->parent()->objectName().toStdString()<<" -> X:"<<guiPos.x()<<" Y:"<<guiPos.y()<<std::endl;
        QRect geometry = this->widget->geometry();
        geometry.moveTo(guiPos);
//        std::cout<<"widget:"<<this->body->IsAwake()<<" X:"<<pos.x<<" Y:"<<pos.y<<" W:"<<geometry.width()<<" H:"<<geometry.height()<<std::endl;

//        if(!this->body->IsAwake() && !this->isfallingIn()){
//            this->setVisible(false);
//        }
        this->widget->setGeometry(geometry);
    }

}
//void PhysicsObject::resize(QRect rect){

//}

QPoint PhysicsObject::worldToGuiPos(QPointF pos){
    int x = (int)(pos.x()*100) - this->offset.x();
    int y = (int)(pos.y()*100) - this->offset.y();

    return QPoint(x,y);
}

QPointF PhysicsObject::guiToWorldPos(QPoint pos){
    qreal x = ((pos.x() + this->offset.x())/100.0);
    qreal y = ((pos.y() + this->offset.y())/100.0);

    return QPointF(x,y);
}

void PhysicsObject::setWorldChannel(int channel){
    this->offset.setX(channel*5*100);
}

void PhysicsObject::fallIn(){
//    std::cout<<"Trigger fall in "<<this->parent()->objectName().toStdString()<<std::endl;
//    this->setVisible(true);
    this->fallingIn = true;
    b2Transform transform = this->body->GetTransform();
    transform.p.y = -10;
    this->body->SetTransform(transform.p,transform.q.GetAngle());
    this->body->SetActive(true);
    this->body->SetAwake(true);
//    this->show();
//    this->activateWindow();
//    this->raise();
    this->offset.setY(-this->home.y());
}

void PhysicsObject::fallOut(){
//    std::cout<<"Trigger fall out "<<std::endl;
    this->fallingIn = false;
    b2Transform transform = this->body->GetTransform();
    transform.p.y = -10;
    this->body->SetTransform(transform.p,transform.q.GetAngle());
    this->body->SetActive(true);
    this->body->SetAwake(true);
    this->offset.setY(-this->home.y()-1000);
}

void PhysicsObject::setOut(){
    this->fallingIn = false;
    b2Transform transform = this->body->GetTransform();
    transform.p.y = 0;
    this->body->SetTransform(transform.p,transform.q.GetAngle());
    this->body->SetActive(true);
    this->body->SetAwake(true);
//    this->show();
//    this->activateWindow();
//    this->raise();
    this->offset.setY(-this->home.y()-1000);
}

bool PhysicsObject::isfallingIn(){
    return this->fallingIn;
}
