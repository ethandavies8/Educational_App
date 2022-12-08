#include <Box2D/Box2D.h>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QString>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QtWidgets>

#include "graphicscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont(":/title/fonts/OCRAEXT.TTF");
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(this->mainMenuIndex);
    connectActions();
    connectTitle();
    this->fallFrame = new FallingStackedFrame(ui->stackedWidget,this);
    connect(this,&MainWindow::fallTo,this->fallFrame,&FallingStackedFrame::FallTo);
    setUpTitleFall();
    connectTools();

    scene = new GraphicScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 250, 250));

    connectScene();

    QHBoxLayout *layout = new QHBoxLayout;
    //layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    QColor backgroundColor(46, 46, 46);
    QBrush backgroundBrush(backgroundColor);
    view->setBackgroundBrush(backgroundBrush);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);


    ui->toolLayout->addWidget(widget);
    ui->levelCompleteButton->hide();

    ui->truthTable->setStyleSheet("QTableView::item:selected { color:white; background:#000000;}"
                                  "QTableCornerButton::section { background-color:#232326; }"
                                  "QHeaderView::section { color:white; background-color:#232326; }"
                                  "QTableView::item {color:white;}");
    ui->truthTableLabel->setText("A NOT Gate will take an input and reverse it in the output.");

    QColor truthTablecolor(46, 46, 46);
    QBrush truthTablebrush(truthTablecolor);
    truthTableBackground = truthTablebrush;
    for(int i = 2; i < ui->truthTable->rowCount(); i++){
        ui->truthTable->hideRow(i);
    }
    ui->truthTable->hideColumn(1);
    ui->truthTable->hideColumn(2);
    ui->truthTable->hideColumn(4);
    setupMouseIcons();
//    qApp->setStyleSheet("QWidget { border: 1px solid red; }");
    this->GoToMainMenue();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->fallFrame;
}

void MainWindow::connectTitle()
{
    connect(ui->playTitleButton, &QPushButton::clicked, this, &MainWindow::PressedPlay);
    connect(ui->learnTitleButton, &QPushButton::clicked, this, &MainWindow::PressedInfo);
    connect(ui->helpTitleButton, &QPushButton::clicked, this, &MainWindow::PressedHelp);
}

void MainWindow::connectScene(){
    connect(this, &MainWindow::changeSelectedGate, scene, &GraphicScene::setGateImage);
    connect(this, &MainWindow::changeItemType, scene, &GraphicScene::setItemType);
    connect(this, &MainWindow::checkTwoGateLevel, scene, &GraphicScene::testTwoGates);
    connect(this, &MainWindow::checkThreeGateLevel, scene, &GraphicScene::testThreeGateLevel);
    connect(this, &MainWindow::checkFourGateLevel, scene, &GraphicScene::testFourGateLevel);
    connect(this, &MainWindow::checkChallengeLevel, scene, &GraphicScene::testChallengeLevel);
    connect(scene, &GraphicScene::rowCorrect, this, &MainWindow::truthTableRowCorrect);
    connect(ui->levelCompleteButton, &QPushButton::clicked, this, &MainWindow::nextLevel);
}

void MainWindow::connectActions()
{
    connect(ui->actiontitle, &QAction::triggered, this, &MainWindow::GoToMainMenue); // TODO replace with more
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::refreshGameView);
    connect(ui->testResultsButton, &QPushButton::clicked, this, &MainWindow::testCircuit);
}

void MainWindow::resetTool()
{
    setCursor(Qt::ArrowCursor);
    std::cout << "reset tool" << std::endl;
}

void MainWindow::connectTools()
{
    connect(ui->ANDGateButton, &QPushButton::pressed, this, &MainWindow::ANDGateSelection);
    //connect(this, &MainWindow::deleteEvent, this, &GraphicScene::setMode(GraphicScene::Del));
    connect(ui->NANDGateButton, &QPushButton::pressed, this, &MainWindow::NANDGateSelection);
    connect(ui->ORGateButton, &QPushButton::pressed, this, &MainWindow::ORGateSelection);
    connect(ui->NOTGateButton, &QPushButton::pressed, this, &MainWindow::NOTGateSelection);
    connect(ui->lineButton, &QPushButton::pressed, this, &MainWindow::lineSelection);
    connect(ui->selectionButton, &QPushButton::pressed, this, &MainWindow::selectToolSelection);
    connect(ui->NORGateButton, &QPushButton::pressed, this, &MainWindow::NORGateSelection);
    connect(ui->XORGateButton, &QPushButton::pressed, this, &MainWindow::XORGateSelection);

}

void MainWindow::setUpTitleFall()
{
    removeBackground(ui->TitleBackround);
    removeBackground(ui->TitleForground);

    PhysicsScene* ps = new PhysicsScene(this);
    ps->addBody(ui->TitleBackround);
    ps->addBody(ui->TitleForground, 0.35f);
    ps->addBody(ui->helpTitleButton, 0.35);
    ps->addBody(ui->learnTitleButton, 0.35);
    ps->addBody(ui->playTitleButton, 0.4f);
    this->fallFrame->setPhysicsScene(this->mainMenuIndex,ps);
}
void MainWindow::removeBackground(QWidget* widget) {
    widget->setAttribute(Qt::WA_NoSystemBackground);
    widget->setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::setupMouseIcons()
{
    mouseIcons.insert(AND, QPixmap(":/icons/ANDGate.png"));
    mouseIcons.insert(OR, QPixmap(":/icons/ORGate.png"));
    mouseIcons.insert(NOT, QPixmap(":/icons/NOTGate.png"));
    mouseIcons.insert(NAND, QPixmap(":/icons/nand.png"));
    mouseIcons.insert(NOR, QPixmap(":/icons/nor.png"));
    mouseIcons.insert(XOR, QPixmap(":/icons/xor.png"));
    mouseIcons.insert(Wire, QPixmap(":/icons/line.png"));
    mouseIcons.insert(Deselect, QPixmap(":/icons/mousePointer.png"));
}


void MainWindow::testCircuit(){
    if(currentLevelIndex == 0){
        emit checkTwoGateLevel();
    }
    else if(currentLevelIndex > 0 && currentLevelIndex < 6){
        emit checkThreeGateLevel(currentLevelIndex);
    }
    else if(currentLevelIndex >= 6 && currentLevelIndex < 9)
        emit checkFourGateLevel(currentLevelIndex);
    else
        emit checkChallengeLevel();
}

void MainWindow::truthTableRowCorrect(int row){
    switch(row){
    case(0):
        ui->truthTable->item(0, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(0, 4)->setBackground(Qt::darkGreen);
        break;
    case(1):
        ui->truthTable->item(1, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(1, 4)->setBackground(Qt::darkGreen);
        break;
    case(2):
        ui->truthTable->item(2, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(2, 4)->setBackground(Qt::darkGreen);
        break;
    case(3):
        ui->truthTable->item(3, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(3, 4)->setBackground(Qt::darkGreen);
        break;
    case(4):
        ui->truthTable->item(4, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(4, 4)->setBackground(Qt::darkGreen);
        break;
    case(5):
        ui->truthTable->item(5, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(5, 4)->setBackground(Qt::darkGreen);
        break;
    case(6):
        ui->truthTable->item(6, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(6, 4)->setBackground(Qt::darkGreen);
        break;
    case(7):
        ui->truthTable->item(7, 3)->setBackground(Qt::darkGreen);
        if(currentLevelIndex== 9)
            ui->truthTable->item(7, 4)->setBackground(Qt::darkGreen);
        break;
    }

    //check for all boxes green for 1 input
    if(currentLevelIndex == 0 && ui->truthTable->item(0,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(1,3)->background() == Qt::darkGreen){
        ui->levelCompleteButton->show();
        ui->levelCompleteButton->setEnabled(true);
    }
    //check for all boxes green for 2 input
    if(currentLevelIndex > 0 && currentLevelIndex < 6 &&
            ui->truthTable->item(0,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(1,3)->background() == Qt::darkGreen &&
           ui->truthTable->item(2,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(3,3)->background() == Qt::darkGreen){
        ui->levelCompleteButton->show();
        ui->levelCompleteButton->setEnabled(true);
    }
    //check for all boxes green 3 input
    else if(ui->truthTable->item(0,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(1,3)->background() == Qt::darkGreen &&
           ui->truthTable->item(2,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(3,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(4,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(5,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(6,3)->background() == Qt::darkGreen &&
            ui->truthTable->item(7,3)->background() == Qt::darkGreen){

            ui->levelCompleteButton->show();
            ui->levelCompleteButton->setEnabled(true);
            if(currentLevelIndex == 9){
            ui->levelCompleteButton->setText("Congratulations! you have won the game");
            }
            //ui->levelCompleteButton->setText("You have reached the end of the game. Congratualions!");
    }
}

void MainWindow::nextLevel(){
    currentLevelIndex++;
    switch(currentLevelIndex){
    case(1):
        makeLevelTwo();
        break;
    case(2):
        makeLevelThree();
        break;
    case(3):
        makeLevelFour();
        break;
    case(4):
        makeLevelFive();
        break;
    case(5):
        makeLevelSix();
        break;
    case(6):
        makeLevelSeven();
        break;
    case(7):
        makeLevelEight();
        break;
    case(8):
        makeLevelNine();
        break;
    case(9):
        makeChallengeLevel();
        break;
    case(10):
        //end game screen
        //ui->levelCompleteButton->setText("Congratulations! you have won the game");
        break;
    default:
        //final screen
        break;
    }
}
void MainWindow::clearPreviousLevel(){
    ui->levelCompleteButton->hide();
    ui->levelCompleteButton->setEnabled(false);
    refreshGameView();
}

void MainWindow::makeLevelTwo(){
    //setup for 2 gates
    ui->truthTable->showRow(2);
    ui->truthTable->showRow(3);
    ui->truthTable->showColumn(1);

    currentRowCount += 2;

    //iniialize values for 2 new rows
    ui->truthTable->item(0, 1)->setText("0");
    ui->truthTable->item(1, 1)->setText("0");
    ui->truthTable->item(2, 1)->setText("1");
    ui->truthTable->item(3, 1)->setText("1");

    ui->truthTable->item(0, 0)->setText("0");
    ui->truthTable->item(1, 0)->setText("1");
    ui->truthTable->item(2, 0)->setText("0");
    ui->truthTable->item(3, 0)->setText("1");


    //update outputs
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("0");
    ui->truthTable->item(2, 3)->setText("0");
    ui->truthTable->item(3, 3)->setText("1");
    ui->levelLabel->setText("Level 2");

    //update level description and unlock next gate
    ui->truthTableLabel->setText("An AND Gate will take inputs and only output 1 if both inputs are 1.");
    ui->ANDGateButton->setEnabled(true);

    clearPreviousLevel();
}

void MainWindow::makeLevelThree(){
    //update outputs
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("1");
    ui->levelLabel->setText("Level 3");

    //update level description and unlock next gate
    ui->truthTableLabel->setText(
                "An OR Gate will take inputs and only output 1 if either or all inputs are 1.");
    ui->ORGateButton->setEnabled(true);
    clearPreviousLevel();
}

void MainWindow::makeLevelFour(){
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("0");
    ui->levelLabel->setText("Level 4");

    //update level description and unlock next gate
    ui->truthTableLabel->setText(
                "An XOR Gate will take inputs and only output 1 if only one input is 1.");
    ui->XORGateButton->setEnabled(true);
    clearPreviousLevel();

}

void MainWindow::makeLevelFive(){
    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("0");
    ui->levelLabel->setText("Level 5");

    ui->truthTableLabel->setText("The NAND and NOR Gates are opposites of their respective Gates. "
                                 "The NAND Gate will only output 1 if both inputs aren't 1. The NOR Gate will "
                                 "only output 1 if both inputs are 0."
                                 " Using this information, try to match the not version of the gate to the truth table below");
    //unlock remaining gates
    ui->NANDGateButton->setEnabled(true);
    ui->NORGateButton->setEnabled(true);
    clearPreviousLevel();
}

void MainWindow::makeLevelSix(){
    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("0");
    ui->truthTable->item(2, 3)->setText("0");
    ui->truthTable->item(3, 3)->setText("0");
    ui->levelLabel->setText("Level 6");

    clearPreviousLevel();
}

void MainWindow::makeLevelSeven(){
    //setup for 3 gates
    for(int i = 4; i < ui->truthTable->rowCount(); i++){
        ui->truthTable->showRow(i);
    }
    ui->truthTable->showColumn(2);
    currentRowCount += 4;

    //input 1
        ui->truthTable->item(0, 0)->setText("0");
        ui->truthTable->item(1, 0)->setText("0");
        ui->truthTable->item(2, 0)->setText("0");
        ui->truthTable->item(3, 0)->setText("0");
        ui->truthTable->item(4, 0)->setText("1");
        ui->truthTable->item(5, 0)->setText("1");
        ui->truthTable->item(6, 0)->setText("1");
        ui->truthTable->item(7, 0)->setText("1");
    //input 2
        ui->truthTable->item(0, 1)->setText("0");
        ui->truthTable->item(1, 1)->setText("0");
        ui->truthTable->item(2, 1)->setText("1");
        ui->truthTable->item(3, 1)->setText("1");
        ui->truthTable->item(4, 1)->setText("0");
        ui->truthTable->item(5, 1)->setText("0");
        ui->truthTable->item(6, 1)->setText("1");
        ui->truthTable->item(7, 1)->setText("1");
    //input 3

        ui->truthTable->item(0, 2)->setText("0");
        ui->truthTable->item(1, 2)->setText("1");
        ui->truthTable->item(2, 2)->setText("0");
        ui->truthTable->item(3, 2)->setText("1");
        ui->truthTable->item(4, 2)->setText("0");
        ui->truthTable->item(5, 2)->setText("1");
        ui->truthTable->item(6, 2)->setText("0");
        ui->truthTable->item(7, 2)->setText("1");


    //update outputs
        ui->truthTable->item(0, 3)->setText("0");
        ui->truthTable->item(1, 3)->setText("1");
        ui->truthTable->item(2, 3)->setText("1");
        ui->truthTable->item(3, 3)->setText("0");
        ui->truthTable->item(4, 3)->setText("1");
        ui->truthTable->item(5, 3)->setText("0");
        ui->truthTable->item(6, 3)->setText("0");
        ui->truthTable->item(7, 3)->setText("1");
        ui->levelLabel->setText("Level 7");

        //update level description and unlock next gate
        ui->truthTableLabel->setText("A Gate can have multiple inputs. See how an XOR Gate behaves with 3 inputs! It will perform the XOR operation on each input separately");

        clearPreviousLevel();
}

void MainWindow::makeLevelEight(){
    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("0");
    ui->truthTable->item(2, 3)->setText("0");
    ui->truthTable->item(3, 3)->setText("1");
    ui->truthTable->item(4, 3)->setText("0");
    ui->truthTable->item(5, 3)->setText("0");
    ui->truthTable->item(6, 3)->setText("1");
    ui->truthTable->item(7, 3)->setText("1");
    ui->levelLabel->setText("Level 8");

    ui->truthTableLabel->setText("Here is an example truth table. Connect the Gates to reach the desired output.");
    clearPreviousLevel();
}

void MainWindow::makeLevelNine(){
    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("0");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("1");
    ui->truthTable->item(4, 3)->setText("1");
    ui->truthTable->item(5, 3)->setText("0");
    ui->truthTable->item(6, 3)->setText("0");
    ui->truthTable->item(7, 3)->setText("0");
    ui->levelLabel->setText("Level 9");
    clearPreviousLevel();
}

void MainWindow::makeChallengeLevel(){

    ui->truthTable->showColumn(4);

    //set first output column
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("0");
    ui->truthTable->item(4, 3)->setText("1");
    ui->truthTable->item(5, 3)->setText("0");
    ui->truthTable->item(6, 3)->setText("0");
    ui->truthTable->item(7, 3)->setText("1");

    //set sum output column
    ui->truthTable->item(0, 4)->setText("0");
    ui->truthTable->item(1, 4)->setText("0");
    ui->truthTable->item(2, 4)->setText("0");
    ui->truthTable->item(3, 4)->setText("1");
    ui->truthTable->item(4, 4)->setText("0");
    ui->truthTable->item(5, 4)->setText("1");
    ui->truthTable->item(6, 4)->setText("1");
    ui->truthTable->item(7, 4)->setText("1");

    ui->levelLabel->setText("Challenge Level");
    clearPreviousLevel();
}

void MainWindow::PressedPlay()
{
    emit fallTo(this->firstLevelIndex);
}

void MainWindow::PressedInfo()
{
    emit fallTo(this->infoIndex);
}

void MainWindow::PressedHelp()
{
    emit fallTo(this->helpIndex);
}

void MainWindow::GoToMainMenue()
{
    emit fallTo(this->mainMenuIndex);
//    emit titleFallIn();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    std::cout << "resize" << std::endl;
}

void MainWindow::refreshGameView(){
    for(int i = 0; i < currentRowCount; i++){
        ui->truthTable->item(i, 3)->setBackground(truthTableBackground);
        if(currentLevelIndex == 9)
            ui->truthTable->item(i, 4)->setBackground(truthTableBackground);
    }
    scene->clear();
    scene->update();

    if(currentLevelIndex == 0){
        scene->setUpTwoGates();
    }
    else if(currentLevelIndex > 0 && currentLevelIndex < 6){
        scene->setUpThreeGates();
    }
    else if(currentLevelIndex >= 6 && currentLevelIndex < 9)
        scene->setUpFourGates();
    else if(currentLevelIndex == 9)
        scene->setUpChallengeGates();
    else{
        //game over
       // ui->levelCompleteButton->setText("You have reached the end of the game!");
    }
    /*
    switch(currentLevelIndex){
    case(0):
        scene->setUpTwoGates();
        break;
    default:
        scene->setUpThreeGates();
        break;
    }

    std::cout << "refresh" << std::endl;
    */
}

void MainWindow::ANDGateSelection()
{
    currentTool = AND;
    std::cout << "select AND gate" << std::endl;
    scene->setMode(GraphicScene::InsertItem);
    emit changeItemType(Gate::AND);
    emit changeSelectedGate(mouseIcons[AND]);

}

void MainWindow::NANDGateSelection()
{
    currentTool = NAND;
    std::cout << "select NAND gate" << std::endl;
    scene->setMode(GraphicScene::InsertItem);
    emit changeItemType(Gate::NAND);
    emit changeSelectedGate(mouseIcons[NAND]);
}

void MainWindow::ORGateSelection()
{
    currentTool = OR;
    std::cout << "select OR gate" << std::endl;
    scene->setMode(GraphicScene::InsertItem);

    emit changeItemType(Gate::OR);
    emit changeSelectedGate(mouseIcons[OR]);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        emit deleteEvent();
    }
}
void MainWindow::NOTGateSelection()
{
    currentTool = NOT;
    std::cout << "select NOT gate" << std::endl;
    scene->setMode(GraphicScene::InsertItem);

    emit changeItemType(Gate::NOT);
    emit changeSelectedGate(mouseIcons[NOT]);
}

void MainWindow::lineSelection()
{
    currentTool = Wire;
    std::cout << "select line" << std::endl;

    emit changeItemType(Gate::NoSelection);
    scene->setMode(GraphicScene::InsertLine);
}

void MainWindow::selectToolSelection()
{
    currentTool = Deselect;
    setCursor(Qt::ArrowCursor);
    std::cout << "select tool" << std::endl;

    emit changeItemType(Gate::NoSelection);
    scene->setMode(GraphicScene::MoveItem);
}

void MainWindow::NORGateSelection()
{
    currentTool = NOR;
    std::cout << "select NOR gate" << std::endl;
    scene->setMode(GraphicScene::InsertItem);

    emit changeItemType(Gate::NOR);
    emit changeSelectedGate(mouseIcons[NOR]);
}

void MainWindow::XORGateSelection()
{
    currentTool = XOR;
    std::cout << "select XOR gate" << std::endl;
    scene->setMode(GraphicScene::InsertItem);

    emit changeItemType(Gate::XOR);
    emit changeSelectedGate(mouseIcons[XOR]);
}

void MainWindow::on_HelpHome_clicked()
{
    GoToMainMenue();
}


void MainWindow::on_learnHome_clicked()
{
    GoToMainMenue();
}


void MainWindow::on_Level1Home_clicked()
{
    GoToMainMenue();
    refreshGameView();
    emit changeItemType(Gate::NoSelection);
    scene->setMode(GraphicScene::MoveItem);

}

