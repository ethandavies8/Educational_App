#include <QObject>
#include <Box2D/Box2D.h>
#include "physicsobject.h"
#include "physicsscene.h"
#include <QTimer>



PhysicsScene::PhysicsScene(QWidget *parent)
    : QWidget{parent}
{
    this->bodies = QVector<PhysicsObject*>();
    // Define the gravity vector.
    b2Vec2 gravity(0.0f, 10.0f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    world = new b2World(gravity);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 1.01497f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50000000.0f, 0.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    timeStep = 1.0f / 60.0f;
    velocityIterations = 6;
    positionIterations = 2;


    QTimer *worldCalc = new QTimer(this);
    connect(worldCalc, &QTimer::timeout, this, &PhysicsScene::updateWorld);
    worldCalc->start(20);
    // Initialize bodies
    setOut();
}
/// @brief Each physics scene is responsible for the physics world and any physics bodies it created.
PhysicsScene::~PhysicsScene()
{
    int count = this->bodies.count();
    for (int i = 0; i < count; ++i) {
        delete this->bodies.at(i);
    }
    delete this->world;
}

/// @brief This should be called on a regular interval to update all physics bodies positions on the
///             gui according to the location in the physics world.
void PhysicsScene::updateWorld(){
    world->Step(timeStep, velocityIterations, positionIterations);
    emit postSimulation();
}

/// @brief This adds a new physics body that is connected to a specific widget.
/// @param widget The widget to be simulated.
/// @param bounce The coefficient of restoration for the new widget, determines how much "bounce" the
///                 the new widget has.
void PhysicsScene::addBody(QWidget* widget,float bounce){
    int bodyCount = this->bodies.count();
    // add the body to the world
    this->bodies.append(new PhysicsObject(widget,this->world,bodyCount,bounce));
    //add the signal so it will update its postion after each simulation cycle.
    connect(this,&PhysicsScene::postSimulation,this->bodies.value(bodyCount),&PhysicsObject::UpdateGuiPosition);
    connect(this,&PhysicsScene::TriggerFallIn,this->bodies.value(bodyCount),&PhysicsObject::fallIn);
    connect(this,&PhysicsScene::TriggerFallOut,this->bodies.value(bodyCount),&PhysicsObject::fallOut);
    connect(this,&PhysicsScene::TriggersetOut,this->bodies.value(bodyCount),&PhysicsObject::setOut);
}

/// @brief  Triggers all bodies in the scene to begin a fall in
void PhysicsScene::fallIn(){
    emit TriggerFallIn();
}

/// @brief  Triggers all bodies in the scene to begin a fall out
void PhysicsScene::fallOut(){
    emit TriggerFallOut();
}

/// @brief tells all the bodies in the scene to Move themselves off screen so they are not visible.
void PhysicsScene::setOut(){
    emit TriggersetOut();
}
