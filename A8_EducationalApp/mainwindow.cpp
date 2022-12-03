#include <Box2D/Box2D.h>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QString>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QHBoxLayout>

#include "dragwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont(":/title/fonts/OCRAEXT.TTF");
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(this->mainMenuIndex);
    ui->stackedWidget->setWindowOpacity(0.0);
    connectActions();
    connectTitle();
    this->fallFrame = new FallingStackedFrame(ui->stackedWidget,this);
    connect(this,&MainWindow::fallTo,this->fallFrame,&FallingStackedFrame::FallTo);
    setUpTitleFall();
    connectTools();

    // QGroupBox *drawingTools = new QGroupBox(ui->groupBox);
    // QHBoxLayout *verticalLayout = new QHBoxLayout(ui->drawingWidget);
    ui->toolLayout->addWidget(&dragWidget);
    // ui->toolLayout->addWidget(new DragWidget);
    QTimer::singleShot(1,this,&MainWindow::GoToMainMenue);
    QTimer::singleShot(10000,this,&MainWindow::GoToMainMenue);
    QTimer::singleShot(10000,this,&MainWindow::GoToMainMenue);

    setupMouseIcons();
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

void MainWindow::connectActions()
{
    connect(ui->actiontitle, &QAction::triggered, this, &MainWindow::GoToMainMenue); // TODO replace with more
    connect(&dragWidget, &DragWidget::resetTool, this, &MainWindow::resetTool);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::refreshGameView);
}

void MainWindow::resetTool()
{
    setCursor(Qt::ArrowCursor);
    std::cout << "reset tool" << std::endl;
}

void MainWindow::connectTools()
{
    connect(ui->ANDGateButton, &QPushButton::pressed, this, &MainWindow::ANDGateSelection);
    connect(this, &MainWindow::deleteEvent, &dragWidget, &DragWidget::receiveDeleteKey);
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
    PhysicsScene* ps = new PhysicsScene(this);
    ps->addBody(ui->TitleBackround);
    ps->addBody(ui->TitleForground, 0.35f);
    ps->addBody(ui->helpTitleButton, 0.35);
    ps->addBody(ui->learnTitleButton, 0.35);
    ps->addBody(ui->playTitleButton, 0.4f);
    this->fallFrame->setPhysicsScene(this->mainMenuIndex,ps);
//    emit fallTo(this->mainMenuIndex);
}

void MainWindow::setupMouseIcons()
{
    mouseIcons.insert(AND, QPixmap(":/icons/ANDGate.png"));
    mouseIcons.insert(OR, QPixmap(":/icons/ORGate.png"));
    mouseIcons.insert(NOT, QPixmap(":/icons/NOTGate.png"));
    mouseIcons.insert(NAND, QPixmap(":/icons/NANDGate.png"));
    mouseIcons.insert(NOR, QPixmap(":/icons/NORGate.png"));
    mouseIcons.insert(XOR, QPixmap(":/icons/XORGate.png"));
    mouseIcons.insert(Wire, QPixmap(":/icons/line.png"));
    mouseIcons.insert(Deselect, QPixmap(":/icons/mousePointer.png"));
}

void MainWindow::PressedPlay()
{
    emit fallTo(this->firstLevelIndex);
//    emit titleFallOut();
//    ui->stackedWidget->setCurrentIndex(firstLevelIndex);
}

void MainWindow::PressedInfo()
{
    emit fallTo(this->infoIndex);
//    emit titleFallIn();
//    ui->stackedWidget->setCurrentIndex(infoIndex);
}

void MainWindow::PressedHelp()
{
    emit fallTo(this->helpIndex);
//    emit titleFallIn();
//    ui->stackedWidget->setCurrentIndex(helpIndex);
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
    dragWidget.clearChildren();
    std::cout << "refresh" << std::endl;
}

void MainWindow::ANDGateSelection()
{
    currentTool = AND;
    dragWidget.AddItem(mouseIcons[AND]);
    std::cout << "select AND gate" << std::endl;
}

void MainWindow::NANDGateSelection()
{
    currentTool = NAND;
    dragWidget.AddItem(mouseIcons[NAND]);
    std::cout << "select NAND gate" << std::endl;
}

void MainWindow::ORGateSelection()
{
    currentTool = OR;
    dragWidget.AddItem(mouseIcons[OR]);
    std::cout << "select OR gate" << std::endl;
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
    dragWidget.AddItem(mouseIcons[NOT]);
    std::cout << "select NOT gate" << std::endl;
}

void MainWindow::lineSelection()
{
    currentTool = Wire;
    dragWidget.AddItem(mouseIcons[Wire]);
    std::cout << "select line" << std::endl;
}

void MainWindow::selectToolSelection()
{
    currentTool = Deselect;
    setCursor(Qt::ArrowCursor);
    std::cout << "select tool" << std::endl;
}

void MainWindow::NORGateSelection()
{
    currentTool = NOR;
    dragWidget.AddItem(mouseIcons[NOR]);
    std::cout << "select NOR gate" << std::endl;
}

void MainWindow::XORGateSelection()
{
    currentTool = XOR;
    dragWidget.AddItem(mouseIcons[XOR]);
    std::cout << "select XOR gate" << std::endl;
}
