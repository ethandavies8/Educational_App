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

/// @brief  No memory responsibilities
PhysicsObject::~PhysicsObject(){
    // Both of the pointers in the class are managed elsewhere, the body's memory is cleared with the world. and the widget is removed with the parent(main Window).
}

/// @brief Reads the bodies position and updates the gui to reflect any changes.
void PhysicsObject::UpdateGuiPosition(){
    if(this->body->IsAwake()){
        b2Vec2 pos = this->body->GetPosition();

        QPoint guiPos = worldToGuiPos(QPointF(pos.x,pos.y));
        guiPos.setX(home.x());
        QRect geometry = this->widget->geometry();
        geometry.moveTo(guiPos);
        this->widget->setGeometry(geometry);
    }

}

/// @brief Converts a world relative position to a gui relative position
/// @param pos world position
/// @return    gui position 
QPoint PhysicsObject::worldToGuiPos(QPointF pos){
    int x = (int)(pos.x()*100) - this->offset.x();
    int y = (int)(pos.y()*100) - this->offset.y();

    return QPoint(x,y);
}

/// @brief Converts a gui relative position to a world relative position
/// @param pos gui position
/// @return    world position 
QPointF PhysicsObject::guiToWorldPos(QPoint pos){
    qreal x = ((pos.x() + this->offset.x())/100.0);
    qreal y = ((pos.y() + this->offset.y())/100.0);

    return QPointF(x,y);
}

/// @brief sets the bodies channel in the world Used to avoid collisions between widgets
/// @param channel an unused channel
void PhysicsObject::setWorldChannel(int channel){
    this->offset.setX(channel*5*100);
}

/// @brief Starts the fall in processes for the body
void PhysicsObject::fallIn(){
    this->fallingIn = true;
    b2Transform transform = this->body->GetTransform();
    transform.p.y = -10;
    this->body->SetTransform(transform.p,transform.q.GetAngle());
    this->body->SetActive(true);
    this->body->SetAwake(true);
    this->offset.setY(-this->home.y());
}

/// @brief Starts the fall out processes for the body
void PhysicsObject::fallOut(){
    this->fallingIn = false;
    b2Transform transform = this->body->GetTransform();
    transform.p.y = -10;
    this->body->SetTransform(transform.p,transform.q.GetAngle());
    this->body->SetActive(true);
    this->body->SetAwake(true);
    this->offset.setY(-this->home.y()-1000);
}

/// @brief sets the body out of view hidden from the user.
void PhysicsObject::setOut(){
    this->fallingIn = false;
    b2Transform transform = this->body->GetTransform();
    transform.p.y = 0;
    this->body->SetTransform(transform.p,transform.q.GetAngle());
    this->body->SetActive(true);
    this->body->SetAwake(true);
    this->offset.setY(-this->home.y()-1000);
}

/// @brief Check if the body is falling in or out.
/// @return true if the body is falling in.
bool PhysicsObject::isfallingIn(){
    return this->fallingIn;
}
