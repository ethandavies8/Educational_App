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
    this->fallFrame = new FallingStackedFrame(ui->stackedWidget, this);
    connect(this, &MainWindow::fallTo, this->fallFrame, &FallingStackedFrame::FallTo);
    setUpTitleFall();
    connectTools();

    scene = new GraphicScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 250, 250));

    connectScene();

    QHBoxLayout *layout = new QHBoxLayout;
    view = new QGraphicsView(scene);
    QColor backgroundColor(46, 46, 46);
    QBrush backgroundBrush(backgroundColor);
    view->setBackgroundBrush(backgroundBrush);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    ui->toolLayout->addWidget(widget);

    truthTableSetup();

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
    connect(ui->learnButton, &QPushButton::clicked, this, &MainWindow::PressedInfo);
    connect(ui->helpButton, &QPushButton::clicked, this, &MainWindow::PressedHelp);
    connect(ui->helpMenuStartGame, &QPushButton::clicked, this, &MainWindow::PressedPlay);
    connect(ui->levelSelectButton, &QPushButton::clicked, this, &MainWindow::PressedLevels);
    connect(ui->helpLevelSelect, &QPushButton::clicked, this, &MainWindow::PressedLevels);
    connect(ui->helpLevelSelect2, &QPushButton::clicked, this, &MainWindow::PressedLevels);
    connect(ui->helpLearn, &QPushButton::clicked, this, &MainWindow::PressedInfo);
    connect(ui->helpLearn2, &QPushButton::clicked, this, &MainWindow::PressedInfo);
    connect(ui->helpHome2, &QPushButton::clicked, this, &MainWindow::GoToMainMenue);
    connect(ui->learnHelp, &QPushButton::clicked, this, &MainWindow::PressedHelp);
    connect(ui->learnLevelSelect, &QPushButton::clicked, this, &MainWindow::PressedLevels);
    connect(ui->levelSelectLearn, &QPushButton::clicked, this, &MainWindow::PressedInfo);
    connect(ui->levelSelectHelp, &QPushButton::clicked, this, &MainWindow::PressedHelp);
    connect(ui->gameOverHome, &QPushButton::clicked, this, &MainWindow::GoToMainMenue);

}

void MainWindow::connectScene()
{
    connect(this, &MainWindow::changeSelectedGate, scene, &GraphicScene::setGateImage);
    connect(this, &MainWindow::changeItemType, scene, &GraphicScene::setItemType);
    connect(this, &MainWindow::checkTwoGateLevel, scene, &GraphicScene::testTwoGates);
    connect(this, &MainWindow::checkThreeGateLevel, scene, &GraphicScene::testThreeGateLevel);
    connect(this, &MainWindow::checkFourGateLevel, scene, &GraphicScene::testFourGateLevel);
    connect(this, &MainWindow::checkChallengeLevel, scene, &GraphicScene::testChallengeLevel);
    connect(scene, &GraphicScene::rowCorrect, this, &MainWindow::truthTableRowCorrect);
    connect(ui->unlockAllLevelsCheck, &QCheckBox::clicked, this, &MainWindow::unlockAllLevels);
}

void MainWindow::connectActions()
{
    connect(ui->actiontitle, &QAction::triggered, this, &MainWindow::GoToMainMenue); // TODO replace with more
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::refreshGameView);
    connect(ui->testResultsButton, &QPushButton::clicked, this, &MainWindow::testCircuit);
    connect(ui->levelCompleteButton, &QPushButton::clicked, this, &MainWindow::nextLevel);
}

void MainWindow::connectTools()
{
    connect(ui->ANDGateButton, &QPushButton::pressed, this, &MainWindow::ANDGateSelection);
    // connect(this, &MainWindow::deleteEvent, this, &GraphicScene::setMode(GraphicScene::Del));
    connect(ui->NANDGateButton, &QPushButton::pressed, this, &MainWindow::NANDGateSelection);
    connect(ui->ORGateButton, &QPushButton::pressed, this, &MainWindow::ORGateSelection);
    connect(ui->NOTGateButton, &QPushButton::pressed, this, &MainWindow::NOTGateSelection);
    connect(ui->lineButton, &QPushButton::pressed, this, &MainWindow::lineSelection);
    connect(ui->selectionButton, &QPushButton::pressed, this, &MainWindow::selectToolSelection);
    connect(ui->NORGateButton, &QPushButton::pressed, this, &MainWindow::NORGateSelection);
    connect(ui->XORGateButton, &QPushButton::pressed, this, &MainWindow::XORGateSelection);
}

void MainWindow::truthTableSetup()
{
    ui->levelCompleteButton->hide();

    ui->truthTable->setStyleSheet("QTableView::item:selected { color:white; background:darkGreen;}"
                                  "QTableCornerButton::section { background-color:gray; }"
                                  "QHeaderView::section { color:white; background-color:rgb(100, 100, 100);}"
                                  "QTableView::item {color:black; background-color:darkGray;}");

    ui->truthTable_2->setStyleSheet("QTableView::item:selected { color:white; background:darkGreen;}"
                                    "QTableCornerButton::section { background-color:gray; }"
                                    "QHeaderView::section { color:white; background-color:rgb(100, 100, 100);}"
                                    "QTableView::item {color:black; background-color:darkGray;}");
    for (int i = 2; i < ui->truthTable->rowCount(); i++)
    {
        ui->truthTable->hideRow(i);
    }
    ui->truthTable->hideColumn(1);
    ui->truthTable->hideColumn(2);
    ui->truthTable->hideColumn(4);

    ui->truthTableLabel->setText("Use a NOT gate between the input and output, a NOT gate negates the input value.");
}

void MainWindow::setUpTitleFall()
{
    removeBackground(ui->TitleBackround);
    removeBackground(ui->TitleForground);

    PhysicsScene *ps = new PhysicsScene(this);
    ps->addBody(ui->TitleBackround);
    ps->addBody(ui->TitleForground, 0.35f);
    ps->addBody(ui->helpTitleButton, 0.35);
    ps->addBody(ui->learnTitleButton, 0.35);
    ps->addBody(ui->playTitleButton, 0.4f);
    this->fallFrame->setPhysicsScene(this->mainMenuIndex, ps);
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

void MainWindow::PressedPlay()
{
    if (!seenHelpMenu)
    {
        emit fallTo(this->helpIndex);
        seenHelpMenu = true;
    }
    else
        emit fallTo(this->firstLevelIndex);
}

void MainWindow::PressedInfo()
{
    emit fallTo(this->infoIndex);
}

void MainWindow::PressedHelp()
{
    emit fallTo(this->helpIndex);
    seenHelpMenu = true;
}

void MainWindow::PressedLevels()
{
    emit fallTo(this->levelSelectIndex);
    refreshGameView();
    emit changeItemType(Gate::NoSelection);
    scene->setMode(GraphicScene::MoveItem);
    for (int i = 0; i <= currentLevelIndex; i++)
    {
        levelUnlocked(i);
    }
}


void MainWindow::removeBackground(QWidget *widget)
{
    widget->setAttribute(Qt::WA_NoSystemBackground);
    widget->setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::resetTool()
{
    setCursor(Qt::ArrowCursor);
    std::cout << "reset tool" << std::endl;
}

void MainWindow::testCircuit()
{
    for (int i = 0; i < currentRowCount; i++)
    {
        ui->truthTable->item(i, 3)->setSelected(false);
        if (currentLevelIndex == 9)
            ui->truthTable->item(i, 4)->setSelected(false);
    }

    if (currentLevelIndex == 0)
    {
        emit checkTwoGateLevel();
    }
    else if (currentLevelIndex > 0 && currentLevelIndex < 6)
    {
        emit checkThreeGateLevel(currentLevelIndex);
    }
    else if (currentLevelIndex >= 6 && currentLevelIndex < 9)
        emit checkFourGateLevel(currentLevelIndex);
    else
        emit checkChallengeLevel();
}

void MainWindow::truthTableRowCorrect(int row)
{
    switch (row)
    {
    case (0):
        ui->truthTable->item(0,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(0,4)->setSelected(true);
        break;
    case (1):
        ui->truthTable->item(1,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(1,4)->setSelected(true);
        break;
    case (2):
        ui->truthTable->item(2,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(2,3)->setSelected(true);
        break;
    case (3):
        ui->truthTable->item(3,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(3,4)->setSelected(true);
        break;
    case (4):
        ui->truthTable->item(4,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(4, 4)->setSelected(true);
        break;
    case (5):
        ui->truthTable->item(5,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(5,4)->setSelected(true);
        break;
    case (6):
        ui->truthTable->item(6,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(6,4)->setSelected(true);
        break;
    case (7):
        ui->truthTable->item(7,3)->setSelected(true);
        if (currentLevelIndex == 9)
            ui->truthTable->item(7, 4)->setSelected(true);
        break;
    }

    // check for all boxes selected for 1 input
    if (currentLevelIndex == 0 && ui->truthTable->item(0, 3)->isSelected() &&
        ui->truthTable->item(1, 3)->isSelected())
    {
        ui->levelCompleteButton->show();
        ui->levelCompleteButton->setEnabled(true);
    }
    // check for all boxes selected for 2 input
    if (currentLevelIndex > 0 && currentLevelIndex < 6 &&
        ui->truthTable->item(0, 3)->isSelected() &&
        ui->truthTable->item(1, 3)->isSelected() &&
        ui->truthTable->item(2, 3)->isSelected() &&
        ui->truthTable->item(3, 3)->isSelected())
    {
        ui->levelCompleteButton->show();
        ui->levelCompleteButton->setEnabled(true);
    }
    // check for all boxes selected for 3 input
    else if (ui->truthTable->item(0, 3)->isSelected() &&
             ui->truthTable->item(1, 3)->isSelected() &&
             ui->truthTable->item(2, 3)->isSelected() &&
             ui->truthTable->item(3, 3)->isSelected() &&
             ui->truthTable->item(4, 3)->isSelected() &&
             ui->truthTable->item(5, 3)->isSelected() &&
             ui->truthTable->item(6, 3)->isSelected() &&
             ui->truthTable->item(7, 3)->isSelected())
    {
        ui->levelCompleteButton->show();
        ui->levelCompleteButton->setEnabled(true);
        if (currentLevelIndex == 9)
        {
            ui->levelCompleteButton->setText("Congratulations! you have won the game, click again for end screen.");
        }
    }
}

void MainWindow::nextLevel()
{
    currentLevelIndex++;
    switch (currentLevelIndex)
    {
    case (1):
        makeLevelTwo();
        break;
    case (2):
        makeLevelThree();
        break;
    case (3):
        makeLevelFour();
        break;
    case (4):
        makeLevelFive();
        break;
    case (5):
        makeLevelSix();
        break;
    case (6):
        makeLevelSeven();
        break;
    case (7):
        makeLevelEight();
        break;
    case (8):
        makeLevelNine();
        break;
    case (9):
        makeChallengeLevel();
        break;
    case (10):
        emit fallTo(gameOverIndex);
        currentLevelIndex = 0;
        break;
    default:
        break;
    }
}
void MainWindow::clearPreviousLevel()
{
    ui->levelCompleteButton->hide();
    ui->levelCompleteButton->setEnabled(false);
    refreshGameView();
}

void MainWindow::makeLevelOne()
{
    ui->truthTable->item(0, 0)->setText("0");
    ui->truthTable->item(1, 1)->setText("1");
    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("0");
    ui->levelLabel->setText("Level 1");

    ui->truthTableLabel->setText("Use a NOT gate between the input and output, a NOT gate negates the input value.");
    clearPreviousLevel();
}

void MainWindow::makeLevelTwo()
{
    // setup for 2 gates
    ui->truthTable->showRow(2);
    ui->truthTable->showRow(3);
    ui->truthTable->showColumn(1);

    currentRowCount = 4;
    fillFourRowTruthTable();

    // update outputs
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("0");
    ui->truthTable->item(2, 3)->setText("0");
    ui->truthTable->item(3, 3)->setText("1");
    ui->levelLabel->setText("Level 2");

    // update level description and unlock next gate
    ui->truthTableLabel->setText("Here we will use an AND gate which will take two inputs and only output 1 if both inputs are 1.");
    ui->ANDGateButton->setEnabled(true);

    clearPreviousLevel();
}

void MainWindow::makeLevelThree()
{
    fillFourRowTruthTable();
    // update outputs
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("1");
    ui->levelLabel->setText("Level 3");

    // update level description and unlock next gate
    ui->truthTableLabel->setText(
        "Here we will use an OR gate which will take inputs and only output 1 if either or all inputs are 1.");
    ui->ORGateButton->setEnabled(true);
    clearPreviousLevel();
}

void MainWindow::makeLevelFour()
{
    fillFourRowTruthTable();
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("0");
    ui->levelLabel->setText("Level 4");

    // update level description and unlock next gate
    ui->truthTableLabel->setText(
        "Here we will use an XOR gate which will will take inputs and only output 1 if only one input is 1.");
    ui->XORGateButton->setEnabled(true);
    clearPreviousLevel();
}

void MainWindow::makeLevelFive()
{
    fillFourRowTruthTable();

    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("0");
    ui->levelLabel->setText("Level 5");

    ui->truthTableLabel->setText("The NAND and NOR Gates are opposites of their respective Gates. "
                                 "The NAND Gate will only output 1 if both inputs aren't 1. The NOR Gate will "
                                 "only output 1 if both inputs are 0."
                                 " Using this information, try to match the not version of the gate to the truth table below.");
    // unlock remaining gates
    ui->NANDGateButton->setEnabled(true);
    ui->NORGateButton->setEnabled(true);
    clearPreviousLevel();
}

void MainWindow::makeLevelSix()
{

    fillFourRowTruthTable();

    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("0");
    ui->truthTable->item(2, 3)->setText("0");
    ui->truthTable->item(3, 3)->setText("0");
    ui->levelLabel->setText("Level 6");

    ui->truthTableLabel->setText("The NAND and NOR Gates are opposites of their respective Gates. "
                                 "The NAND Gate will only output 1 if both inputs aren't 1. The NOR Gate will "
                                 "only output 1 if both inputs are 0."
                                 " Using this information, try to match the not version of the gate to the truth table below.");

    clearPreviousLevel();
}

void MainWindow::makeLevelSeven()
{
    // setup for 3 gates
    for (int i = 4; i < ui->truthTable->rowCount(); i++)
    {
        ui->truthTable->showRow(i);
    }
    ui->truthTable->showColumn(2);
    currentRowCount = 8;

    fillEightRowTruthTable();

    // update outputs
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("0");
    ui->truthTable->item(4, 3)->setText("1");
    ui->truthTable->item(5, 3)->setText("0");
    ui->truthTable->item(6, 3)->setText("0");
    ui->truthTable->item(7, 3)->setText("1");
    ui->levelLabel->setText("Level 7");

    // update level description and unlock next gate
    ui->truthTableLabel->setText("A Gate can have more than two inputs. "
                                 "See how an XOR Gate behaves with 3 inputs! "
                                 "It will perform the XOR operation on each input separately.");

    clearPreviousLevel();
}

void MainWindow::makeLevelEight()
{
    fillEightRowTruthTable();

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

void MainWindow::makeLevelNine()
{
    fillEightRowTruthTable();

    ui->truthTable->item(0, 3)->setText("1");
    ui->truthTable->item(1, 3)->setText("0");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("1");
    ui->truthTable->item(4, 3)->setText("1");
    ui->truthTable->item(5, 3)->setText("0");
    ui->truthTable->item(6, 3)->setText("0");
    ui->truthTable->item(7, 3)->setText("0");
    ui->levelLabel->setText("Level 9");

    ui->truthTableLabel->setText("Here is an example truth table. Connect the Gates to reach the desired output.");
    clearPreviousLevel();
}

void MainWindow::makeChallengeLevel()
{

    ui->truthTable->showColumn(4);
    fillEightRowTruthTable();

    // set first output column
    ui->truthTable->item(0, 3)->setText("0");
    ui->truthTable->item(1, 3)->setText("1");
    ui->truthTable->item(2, 3)->setText("1");
    ui->truthTable->item(3, 3)->setText("0");
    ui->truthTable->item(4, 3)->setText("1");
    ui->truthTable->item(5, 3)->setText("0");
    ui->truthTable->item(6, 3)->setText("0");
    ui->truthTable->item(7, 3)->setText("1");

    // set sum output column
    ui->truthTable->item(0, 4)->setText("0");
    ui->truthTable->item(1, 4)->setText("0");
    ui->truthTable->item(2, 4)->setText("0");
    ui->truthTable->item(3, 4)->setText("1");
    ui->truthTable->item(4, 4)->setText("0");
    ui->truthTable->item(5, 4)->setText("1");
    ui->truthTable->item(6, 4)->setText("1");
    ui->truthTable->item(7, 4)->setText("1");

    ui->levelLabel->setText("Challenge Level!");
    clearPreviousLevel();
}

void MainWindow::unlockAllLevels(){
       for (int i = 0; i <= 9; i++)
       {
           levelUnlocked(i);
       }
       ui->unlockAllLevelsCheck->setEnabled(false);
}

void MainWindow::levelUnlocked(int level)
{
    switch (level)
    {
    case (0):
        ui->level1Button->setEnabled(true);
        ui->level1Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (1):
        ui->level2Button->setEnabled(true);
        ui->level2Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (2):
        ui->level3Button->setEnabled(true);
        ui->level3Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (3):
        ui->level4Button->setEnabled(true);
        ui->level4Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (4):
        ui->level5Button->setEnabled(true);
        ui->level5Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (5):
        ui->level6Button->setEnabled(true);
        ui->level6Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (6):
        ui->level7Button->setEnabled(true);
        ui->level7Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (7):
        ui->level8Button->setEnabled(true);
        ui->level8Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (8):
        ui->level9Button->setEnabled(true);
        ui->level9Button->setStyleSheet("color: green;"
                                        "background-color: rgb(60, 60, 60);");
        break;
    case (9):
        ui->level10Button->setEnabled(true);
        ui->level10Button->setStyleSheet("color: green;"
                                         "background-color: rgb(60, 60, 60);");
        break;
    default:
        break;
    }
}

void MainWindow::GoToMainMenue()
{
    emit fallTo(this->mainMenuIndex);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    std::cout << "resize" << std::endl;
}

void MainWindow::refreshGameView()
{
    for (int i = 0; i < currentRowCount; i++)
    {
        ui->truthTable->item(i, 3)->setSelected(false);
        if (currentLevelIndex == 9)
            ui->truthTable->item(i, 4)->setSelected(false);
    }
    scene->clear();
    scene->update();

    if (currentLevelIndex == 0)
    {
        scene->setUpTwoGates();
    }
    else if (currentLevelIndex > 0 && currentLevelIndex < 6)
    {
        scene->setUpThreeGates();
    }
    else if (currentLevelIndex >= 6 && currentLevelIndex < 9)
        scene->setUpFourGates();
    else if (currentLevelIndex == 9)
        scene->setUpChallengeGates();
    else
    {
        // game over
    }
}

void MainWindow::ANDGateSelection()
{
    currentTool = AND;
    scene->setMode(GraphicScene::InsertItem);
    emit changeItemType(Gate::AND);
    emit changeSelectedGate(mouseIcons[AND]);
}

void MainWindow::NANDGateSelection()
{
    currentTool = NAND;
    scene->setMode(GraphicScene::InsertItem);
    emit changeItemType(Gate::NAND);
    emit changeSelectedGate(mouseIcons[NAND]);
}

void MainWindow::ORGateSelection()
{
    currentTool = OR;
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
    scene->setMode(GraphicScene::InsertItem);
    emit changeItemType(Gate::XOR);
    emit changeSelectedGate(mouseIcons[XOR]);
}

void MainWindow::cleanSlate(){

    refreshGameView();
    emit changeItemType(Gate::NoSelection);
    scene->setMode(GraphicScene::MoveItem);
}

void MainWindow::on_HelpHome_clicked()
{
    GoToMainMenue();
    cleanSlate();
}

void MainWindow::on_learnHome_clicked()
{
    GoToMainMenue();
    cleanSlate();
}

void MainWindow::on_Level1Home_clicked()
{
    GoToMainMenue();
    cleanSlate();
}

void MainWindow::on_levelSelectHome_clicked()
{
    GoToMainMenue();
    cleanSlate();
}

void MainWindow::on_level1Button_clicked()
{
    currentLevelIndex = 0;
    makeLevelOne();
    refreshGameView();
    PressedPlay();
    ui->ANDGateButton->setEnabled(false);
    ui->ORGateButton->setEnabled(false);
    ui->XORGateButton->setEnabled(false);
    ui->NANDGateButton->setEnabled(false);
    ui->NORGateButton->setEnabled(false);

    for (int i = 2; i < ui->truthTable->rowCount(); i++)
    {
        ui->truthTable->hideRow(i);
    }
    ui->truthTable->hideColumn(1);
    ui->truthTable->hideColumn(2);
    ui->truthTable->hideColumn(4);
    currentRowCount = 2;
}

void MainWindow::on_level2Button_clicked()
{
    currentLevelIndex = 1;
    makeLevelTwo();
    refreshGameView();
    PressedPlay();
    ui->ANDGateButton->setEnabled(true);
    ui->ORGateButton->setEnabled(false);
    ui->XORGateButton->setEnabled(false);
    ui->NANDGateButton->setEnabled(false);
    ui->NORGateButton->setEnabled(false);

    threeGateUiUpdate();
}

void MainWindow::on_level3Button_clicked()
{
    currentLevelIndex = 2;
    makeLevelThree();
    refreshGameView();
    PressedPlay();
    ui->ANDGateButton->setEnabled(true);
    ui->ORGateButton->setEnabled(true);
    ui->XORGateButton->setEnabled(false);
    ui->NANDGateButton->setEnabled(false);
    ui->NORGateButton->setEnabled(false);
    threeGateUiUpdate();
}

void MainWindow::on_level4Button_clicked()
{
    currentLevelIndex = 3;
    makeLevelFour();
    refreshGameView();
    PressedPlay();
    ui->ANDGateButton->setEnabled(true);
    ui->ORGateButton->setEnabled(true);
    ui->XORGateButton->setEnabled(true);
    ui->NANDGateButton->setEnabled(false);
    ui->NORGateButton->setEnabled(false);
    threeGateUiUpdate();
}

void MainWindow::on_level5Button_clicked()
{
    currentLevelIndex = 4;
    makeLevelFive();
    refreshGameView();
    PressedPlay();
    ui->ANDGateButton->setEnabled(true);
    ui->ORGateButton->setEnabled(true);
    ui->XORGateButton->setEnabled(true);
    ui->NANDGateButton->setEnabled(true);
    ui->NORGateButton->setEnabled(true);
    threeGateUiUpdate();
}

void MainWindow::on_level6Button_clicked()
{
    currentLevelIndex = 5;
    makeLevelSix();
    refreshGameView();
    PressedPlay();
    enableAllGateButtons();
    threeGateUiUpdate();
}

void MainWindow::on_level7Button_clicked()
{
    currentLevelIndex = 6;
    makeLevelSeven();
    refreshGameView();
    PressedPlay();
    enableAllGateButtons();
    fourGateGuiUpdate();
}

void MainWindow::on_level8Button_clicked()
{
    currentLevelIndex = 7;
    makeLevelEight();
    refreshGameView();
    PressedPlay();
    enableAllGateButtons();
    fourGateGuiUpdate();
}

void MainWindow::on_level9Button_clicked()
{
    currentLevelIndex = 8;
    makeLevelNine();
    refreshGameView();
    PressedPlay();
    enableAllGateButtons();
    fourGateGuiUpdate();
}

void MainWindow::on_level10Button_clicked()
{
    currentLevelIndex = 9;
    makeChallengeLevel();
    refreshGameView();
    PressedPlay();
    enableAllGateButtons();
    ui->truthTable->showColumn(1);
    ui->truthTable->showColumn(2);
    ui->truthTable->showColumn(4);

    for (int i = 2; i < ui->truthTable->rowCount(); i++)
    {
        ui->truthTable->showRow(i);
    }
    currentRowCount = 8;
}

void MainWindow::enableAllGateButtons()
{

    ui->ANDGateButton->setEnabled(true);
    ui->ORGateButton->setEnabled(true);
    ui->XORGateButton->setEnabled(true);
    ui->NANDGateButton->setEnabled(true);
    ui->NORGateButton->setEnabled(true);
}

void MainWindow::threeGateUiUpdate()
{

    ui->truthTable->showRow(2);
    ui->truthTable->showRow(3);
    ui->truthTable->showColumn(1);
    ui->truthTable->hideColumn(2);
    ui->truthTable->hideColumn(4);

    for (int i = 4; i < ui->truthTable->rowCount(); i++)
    {
        ui->truthTable->hideRow(i);
    }
    currentRowCount = 4;
}

void MainWindow::fourGateGuiUpdate()
{

    ui->truthTable->showColumn(1);
    ui->truthTable->showColumn(2);
    ui->truthTable->hideColumn(4);

    for (int i = 2; i < ui->truthTable->rowCount(); i++)
    {
        ui->truthTable->showRow(i);
    }

    currentRowCount = 8;
}

void MainWindow::fillFourRowTruthTable()
{
    // iniialize values for columns and rows

    ui->truthTable->item(0, 0)->setText("0");
    ui->truthTable->item(1, 0)->setText("1");
    ui->truthTable->item(2, 0)->setText("0");
    ui->truthTable->item(3, 0)->setText("1");

    ui->truthTable->item(0, 1)->setText("0");
    ui->truthTable->item(1, 1)->setText("0");
    ui->truthTable->item(2, 1)->setText("1");
    ui->truthTable->item(3, 1)->setText("1");
}

void MainWindow::fillEightRowTruthTable()
{

    // input 1
    ui->truthTable->item(0, 0)->setText("0");
    ui->truthTable->item(1, 0)->setText("0");
    ui->truthTable->item(2, 0)->setText("0");
    ui->truthTable->item(3, 0)->setText("0");
    ui->truthTable->item(4, 0)->setText("1");
    ui->truthTable->item(5, 0)->setText("1");
    ui->truthTable->item(6, 0)->setText("1");
    ui->truthTable->item(7, 0)->setText("1");
    // input 2
    ui->truthTable->item(0, 1)->setText("0");
    ui->truthTable->item(1, 1)->setText("0");
    ui->truthTable->item(2, 1)->setText("1");
    ui->truthTable->item(3, 1)->setText("1");
    ui->truthTable->item(4, 1)->setText("0");
    ui->truthTable->item(5, 1)->setText("0");
    ui->truthTable->item(6, 1)->setText("1");
    ui->truthTable->item(7, 1)->setText("1");
    // input 3

    ui->truthTable->item(0, 2)->setText("0");
    ui->truthTable->item(1, 2)->setText("1");
    ui->truthTable->item(2, 2)->setText("0");
    ui->truthTable->item(3, 2)->setText("1");
    ui->truthTable->item(4, 2)->setText("0");
    ui->truthTable->item(5, 2)->setText("1");
    ui->truthTable->item(6, 2)->setText("0");
    ui->truthTable->item(7, 2)->setText("1");
}



