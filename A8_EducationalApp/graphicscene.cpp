
#include "graphicscene.h"

#include <QGraphicsSceneMouseEvent>
#include <iostream>



GraphicScene::GraphicScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = Gate::NoSelection;
    line = nullptr;
    myLineColor = Qt::white;
    setUpTwoGates();
}

//sets gates positions and adds to the screen
void GraphicScene::setUpTwoGates(){
    sourceOne = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceOne);
    sourceOne->setPos(-200, 0);

    output = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(output);
    output->setPos(400, 0);
}

//sets gate positions for 2 inputs
void GraphicScene::setUpThreeGates(){

    sourceOne = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceOne);
    sourceOne->setPos(-200, -100);

    sourceTwo = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceTwo);
    sourceTwo->setPos(-200, 100);

    output = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(output);
    output->setPos(400, 0);
}

//sets gate positions for 3 inputs
void GraphicScene::setUpFourGates(){

    sourceOne = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceOne);
    sourceOne->setPos(-200, -100);

    sourceTwo = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceTwo);
    sourceTwo->setPos(-200, 100);

    sourceThree = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceThree);
    sourceThree->setPos(-200, 300);

    output = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(output);
    output->setPos(400, 100);
}

//sets gate positions for 3 inputs and 2 outputs
void GraphicScene::setUpChallengeGates(){
    sourceOne = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceOne);
    sourceOne->setPos(-200, -100);

    sourceTwo = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceTwo);
    sourceTwo->setPos(-200, 100);

    sourceThree = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceThree);
    sourceThree->setPos(-200, 300);

    output = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(output);
    output->setToolTip("Sum (Output)");
    output->setPos(400, 0);

    sum = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(sum);
    sum->setToolTip("Carry out");
    sum->setPos(400, 200);
}
//update mode slot
void GraphicScene::setMode(Mode mode)
{
    myMode = mode;
}
//update gateType slot
void GraphicScene::setItemType(Gate::GateType type)
{
    myItemType = type;
}
//update pixmap slot
void GraphicScene::setGateImage(QPixmap gateImage){
    currentGate = gateImage;
}

//checks for inserting item or line on mouse press
void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    switch (myMode) {
        case InsertItem:
        insertItem(mouseEvent);
        break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

//inserts gate based on gateType
void GraphicScene::insertItem(QGraphicsSceneMouseEvent *mouseEvent){
    Gate *item;
    switch(myItemType){
        case Gate::AND:
        item = new ANDGate(myItemType, myItemMenu, currentGate);
         break;
    case Gate::OR:
        item = new ORGate(myItemType, myItemMenu, currentGate);
         break;
    case Gate::NOT:
        item = new NOTGate(myItemType, myItemMenu, currentGate);
         break;
    case Gate::NOR:
        item = new NORGate(myItemType, myItemMenu, currentGate);
         break;
    case Gate::NAND:
        item = new NANDGate(myItemType, myItemMenu, currentGate);
         break;
    case Gate::XOR:
        item = new XORGate(myItemType, myItemMenu, currentGate);
        break;
    default:
        item = new Gate(myItemType, myItemMenu, currentGate);
        break;
    }
    addItem(item);
    item->setPos(mouseEvent->scenePos());
}

//draws line with mouse or moves item with mouse as it moves
void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    this->update();
}

//checks for ability to draw wire between two objects
void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != nullptr && myMode == InsertLine) {
           QList<QGraphicsItem *> startItems = items(line->line().p1());
           if (startItems.count() && startItems.first() == line)
               startItems.removeFirst();
           QList<QGraphicsItem *> endItems = items(line->line().p2());
           if (endItems.count() && endItems.first() == line)
               endItems.removeFirst();

           removeItem(line);
           delete line;

           if (startItems.count() > 0 && endItems.count() > 0 &&
               startItems.first()->type() == Gate::Type &&
               endItems.first()->type() == Gate::Type &&
               startItems.first() != endItems.first()) {
               Gate *startItem = qgraphicsitem_cast<Gate *>(startItems.first());
               Gate *endItem = qgraphicsitem_cast<Gate *>(endItems.first());

               if(startItem->getType() != Gate::Output && endItem->getType() != Gate::Source) {
                   if(startItem->hasOutput() == false) {
                       if(endItem->hasOutput())
                       {
                           if(endItem->outputWire->containGate(startItem) == false) {
                               Wire *wire = new Wire(startItem, endItem);
                               wire->setColor(myLineColor);

                               wire->setZValue(-1000.0);
                               addItem(wire);
                               wire->updatePosition();
                           }
                       } else {
                           Wire *wire = new Wire(startItem, endItem);
                           wire->setColor(myLineColor);

                           wire->setZValue(-1000.0);
                           addItem(wire);
                           wire->updatePosition();
                       }
                   }else {
                       Wire *wire = startItem->outputWire;
                       if(wire->containGate(endItem) == false)
                       {
                           wire->setColor(myLineColor);

                           wire->connect(endItem);
                           wire->setZValue(-1000.0);
                           addItem(wire);
                           wire->updatePosition();
                       }
                   }
               }
           }
       }
       this->update();
       line = nullptr;
       QGraphicsScene::mouseReleaseEvent(mouseEvent);
   }

//tests for correct not gate output
void GraphicScene::testTwoGates(){
    sourceOne->setOutput(false);
    if(output->getOutput()){
        emit rowCorrect(0);
    }
    sourceOne->setOutput(true);
    if(!output->getOutput()){
        emit rowCorrect(1);
    }
}

//tests for correct output for levels with 2 inputs
void GraphicScene::testThreeGateLevel(int levelIndex){
    //if true for input 0,0
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    if(output->getOutput()){
        if(levelIndex == 4 || levelIndex == 5){
            emit rowCorrect(0);
        }
    }
    else{
        if(levelIndex == 1 || levelIndex == 2 || levelIndex == 3)
            emit rowCorrect(0);
    }
    //if true for input 1, 0
    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    if(output->getOutput()){
        if(levelIndex == 2 || levelIndex == 3 || levelIndex == 4)
            emit rowCorrect(1);
    }
    else{
        if(levelIndex == 1 || levelIndex == 5)
            emit rowCorrect(1);
    }
    //if true for input 0, 1
    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    if(output->getOutput()){
        if(levelIndex == 2 || levelIndex == 3 || levelIndex == 4)
            emit rowCorrect(2);
    }
    else{
        if(levelIndex == 1 || levelIndex == 5)
            emit rowCorrect(2);
    }
    //if true for input 1, 1
    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    if(output->getOutput()){
        if(levelIndex == 1 || levelIndex == 2)
            emit rowCorrect(3);
    }
    else{
        if(levelIndex == 3 || levelIndex == 4|| levelIndex == 5){
            emit rowCorrect(3);
        }
    }
}

//test for levels with 3 inputs
void GraphicScene::testFourGateLevel(int levelIndex){
    //if true for input 0,0,0
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    if(output->getOutput()){
        if(levelIndex == 7 || levelIndex == 8)
            emit rowCorrect(0);
    }
    else{
        if(levelIndex == 6)
            emit rowCorrect(0);
    }
    //if true for input 0,0, 1
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    if(output->getOutput()){
        if(levelIndex == 6)
            emit rowCorrect(1);
    }
    else{
        if(levelIndex == 7 || levelIndex == 8)
            emit rowCorrect(1);
    }
    //if true for input 0,1,0
    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    if(output->getOutput()){
        if(levelIndex == 6 || levelIndex == 8)
            emit rowCorrect(2);
    }
    else{
        if(levelIndex == 7)
            emit rowCorrect(2);
    }
    //if true for input 0,1,1
    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    if(output->getOutput()){
        if(levelIndex == 7 || levelIndex == 8)
            emit rowCorrect(3);
    }
    else{
        if(levelIndex == 6)
            emit rowCorrect(3);
    }
    //if true for input 1,0,0
    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    if(output->getOutput()){
        if(levelIndex == 6 || levelIndex == 8)
            emit rowCorrect(4);
    }
    else{
        if(levelIndex == 7)
            emit rowCorrect(4);
    }
    //if true for input 1,0,1
    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    if(!output->getOutput()){
        if(levelIndex == 6 || levelIndex == 7 || levelIndex == 8)
            emit rowCorrect(5);
    }
    //if true for input 1,1,0
    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    if(output->getOutput()){
        if(levelIndex == 7)
            emit rowCorrect(6);
    }
    else{
        if(levelIndex == 6 || levelIndex == 8)
            emit rowCorrect(6);
    }
    //if true for input 1,1,1
    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    if(output->getOutput()){
        if(levelIndex == 6 || levelIndex == 7)
            emit rowCorrect(7);
    }
    else{
        if(levelIndex == 8)
            emit rowCorrect(7);
    }
}

//test final level for both outputs
void GraphicScene::testChallengeLevel(){
    //if true for input 0,0,0
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    if(!output->getOutput() && !sum->getOutput())
        emit rowCorrect(0);
    //if true for input 0,0,1
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    if(output->getOutput() && !sum->getOutput())
        emit rowCorrect(1);
    //if true for input 0,1,0
    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    if(output->getOutput() && !sum->getOutput())
        emit rowCorrect(2);
    //if true for input 0,1,1
    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    if(!output->getOutput() && sum->getOutput())
        emit rowCorrect(3);
    //if true for input 1,0,0
    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    if(output->getOutput() && !sum->getOutput())
        emit rowCorrect(4);
    //if true for input 1,0,1
    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    if(!output->getOutput() && sum->getOutput())
        emit rowCorrect(5);
    //if true for input 1,1,0
    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    if(!output->getOutput() && sum->getOutput())
        emit rowCorrect(6);
    //if true for input 1,1,1
    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    if(output->getOutput() && sum->getOutput())
        emit rowCorrect(7);
}
