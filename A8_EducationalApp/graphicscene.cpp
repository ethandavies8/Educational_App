
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

void GraphicScene::setUpTwoGates(){
    sourceOne = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceOne);
    sourceOne->setPos(-200, 0);

    output = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(output);
    output->setPos(300, 0);
}

void GraphicScene::setUpThreeGates(){

    sourceOne = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceOne);
    sourceOne->setPos(-200, -100);

    sourceTwo = new SourceGate(Gate::Source, myItemMenu, currentGate);
    addItem(sourceTwo);
    sourceTwo->setPos(-200, 100);

    output = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(output);
    output->setPos(300, 0);
}

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
    output->setPos(300, 100);
}

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
    output->setPos(300, 100);

    sum = new OutputGate(Gate::Output, myItemMenu, currentGate);
    addItem(sum);
    sum->setPos(300, 300);
}
void GraphicScene::setMode(Mode mode)
{
    myMode = mode;
}

void GraphicScene::setItemType(Gate::GateType type)
{
    myItemType = type;
}

void GraphicScene::setGateImage(QPixmap gateImage){
    currentGate = gateImage;
}

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

       case MoveItem:
            //move logic

        case RemoveItem:
        //removeItem(item)
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicScene::insertItem(QGraphicsSceneMouseEvent *mouseEvent){
    Gate *item;

    switch(myItemType){
        case Gate::AND:
        item = new ANDGate(myItemType, myItemMenu, currentGate);
        std::cout << "created and gate" <<std::endl;
         break;
    case Gate::OR:
        item = new ORGate(myItemType, myItemMenu, currentGate);
        std::cout << "created or gate" <<std::endl;
         break;
    case Gate::NOT:
        item = new NOTGate(myItemType, myItemMenu, currentGate);
        std::cout << "created not gate" <<std::endl;
         break;
    case Gate::NOR:
        item = new NORGate(myItemType, myItemMenu, currentGate);
        std::cout << "created nor gate" <<std::endl;
         break;
    case Gate::NAND:
        item = new NANDGate(myItemType, myItemMenu, currentGate);
        std::cout << "created nand gate" <<std::endl;
         break;
    case Gate::XOR:
        item = new XORGate(myItemType, myItemMenu, currentGate);
        std::cout << "created xor gate" <<std::endl;
        break;
    default:
        item = new Gate(myItemType, myItemMenu, currentGate);
        std::cout << "created default gate" <<std::endl;
        break;
    }

    addItem(item);
    item->setPos(mouseEvent->scenePos());
    //emit itemInserted(item);

}

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

               if(startItem->hasOutput() == false) {
                   Wire *wire = new Wire(startItem, endItem);
                   wire->setColor(myLineColor);

                   wire->setZValue(-1000.0);
                   addItem(wire);
                   wire->updatePosition();
               }else {
                   Wire *wire = startItem->outputWire;
                   wire->setColor(myLineColor);

                   wire->connect(endItem);
                   wire->setZValue(-1000.0);
                   addItem(wire);
                   wire->updatePosition();
               }
           }
       }
       this->update();
       line = nullptr;
       QGraphicsScene::mouseReleaseEvent(mouseEvent);
   }

/*
bool GraphicScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}
*/

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


void GraphicScene::testThreeGateLevel(int levelIndex){
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    //if true for input 0,0
    if(output->getOutput()){
        if(levelIndex == 4 || levelIndex == 5){
            emit rowCorrect(0);
        }
    }
    else{
        if(levelIndex == 1 || levelIndex == 2 || levelIndex == 3){
            emit rowCorrect(0);
        }
    }
    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    //if true for input 1, 0
    if(output->getOutput()){
        if(levelIndex == 2 || levelIndex == 3 || levelIndex == 4){
            emit rowCorrect(1);
        }
    }
    else{
        if(levelIndex == 1 || levelIndex == 5){
            emit rowCorrect(1);
        }
    }

    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    //if true for input 0, 1
    if(output->getOutput()){
        if(levelIndex == 2 || levelIndex == 3 || levelIndex == 4){
            emit rowCorrect(2);
        }
    }
    else{
        if(levelIndex == 1 || levelIndex == 5)
        emit rowCorrect(2);
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    if(output->getOutput()){
        if(levelIndex == 1 || levelIndex == 2){
            emit rowCorrect(3);
        }
    }
    else{
        if(levelIndex == 3 || levelIndex == 4|| levelIndex == 5){
            emit rowCorrect(3);
        }
    }
}

void GraphicScene::testFourGateLevel(int levelIndex){
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    //if true for input 0,0,0
    if(output->getOutput()){
        if(levelIndex == 7 || levelIndex == 8){
            emit rowCorrect(0);
        }
    }
    else{
        if(levelIndex == 6){
            emit rowCorrect(0);
        }
    }

    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    //if true for input 0,0, 1
    if(output->getOutput()){
        if(levelIndex == 6){
            emit rowCorrect(1);
        }
    }
    else{
        if(levelIndex == 7 || levelIndex == 8){
            emit rowCorrect(1);
        }
    }
    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    //if true for input 0,1,0
    if(output->getOutput()){
        if(levelIndex == 6 || levelIndex == 8){
            emit rowCorrect(2);
        }
    }
    else{
        if(levelIndex == 7){
            emit rowCorrect(2);
        }
    }

    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    //if true for input 0,1,1
    if(output->getOutput()){
        if(levelIndex == 7 || levelIndex == 8){
            emit rowCorrect(3);
        }
    }
    else{
        if(levelIndex == 6){
            emit rowCorrect(3);
        }
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    //if true for input 1,0,0
    if(output->getOutput()){
        if(levelIndex == 6 || levelIndex == 8){
            emit rowCorrect(4);
        }
    }
    else{
        if(levelIndex == 7){
            emit rowCorrect(4);
        }
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    //if true for input 1,0,1
    if(output->getOutput()){
    }
    else{
        if(levelIndex == 6 || levelIndex == 7 || levelIndex == 8){
            emit rowCorrect(5);
        }
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    //if true for input 1,1,0
    if(output->getOutput()){
        if(levelIndex == 7){
            emit rowCorrect(6);
        }
    }
    else{
        if(levelIndex == 6 || levelIndex == 8){
            emit rowCorrect(6);
        }
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    //if true for input 1,1,1
    if(output->getOutput()){
        if(levelIndex == 6 || levelIndex == 7){
            emit rowCorrect(7);
        }
    }
    else{
        if(levelIndex == 8){
            emit rowCorrect(7);
        }
    }
}

void GraphicScene::testChallengeLevel(){
    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    //if true for input 0,0,0
    if(!output->getOutput() && !sum->getOutput()){
        emit rowCorrect(0);
    }

    sourceOne->setOutput(false);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    //if true for input 0,0,1
    if(output->getOutput() && !sum->getOutput()){
        emit rowCorrect(1);
    }

    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    //if true for input 0,1,0
    if(output->getOutput() && !sum->getOutput()){
        emit rowCorrect(2);
    }

    sourceOne->setOutput(false);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    //if true for input 0,1,1
    if(!output->getOutput() && sum->getOutput()){
        emit rowCorrect(3);
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(false);
    //if true for input 1,0,0
    if(output->getOutput() && !sum->getOutput()){
        emit rowCorrect(4);
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(false);
    sourceThree->setOutput(true);
    //if true for input 1,0,1
    if(!output->getOutput() && sum->getOutput()){
        emit rowCorrect(5);
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(false);
    //if true for input 1,1,0
    if(!output->getOutput() && sum->getOutput()){
        emit rowCorrect(6);
    }

    sourceOne->setOutput(true);
    sourceTwo->setOutput(true);
    sourceThree->setOutput(true);
    //if true for input 1,1,1
    if(!output->getOutput() && sum->getOutput()){
        emit rowCorrect(7);
    }
}
