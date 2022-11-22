#include <Box2D/Box2D.h>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QString>
#include <QFontDatabase>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QFontDatabase::addApplicationFont(":/title/fonts/OCRAEXT.TTF");
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connectActions();
    connectTitle();
    setUpTitle();
    connectTools();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->physicsScene;
}

void MainWindow::connectTitle(){
    connect(ui->playTitleButton,&QPushButton::clicked, this,&MainWindow::PressedPlay);
    connect(ui->learnTitleButton,&QPushButton::clicked,this,&MainWindow::PressedInfo);
    connect(ui->helpTitleButton,&QPushButton::clicked,this,&MainWindow::PressedHelp);
}

void MainWindow::connectActions(){
    connect(ui->actiontitle,&QAction::triggered,this,&MainWindow::GoToMainMenue); // TODO replace with more
}

void MainWindow::connectTools(){
    connect(ui->ANDGateButton, &QPushButton::pressed, this, &MainWindow::ANDGateSelection);
    connect(ui->NANDGateButton, &QPushButton::pressed, this, &MainWindow::NANDGateSelection);
    connect(ui->ORGateButton, &QPushButton::pressed, this, &MainWindow::ORGateSelection);
    connect(ui->NOTGateButton, &QPushButton::pressed, this, &MainWindow::NOTGateSelection);
    connect(ui->lineButton, &QPushButton::pressed, this, &MainWindow::lineSelection);
}

void MainWindow::setUpTitle(){
    this->physicsScene = new PhysicsScene(this);
    this->physicsScene->addBody(ui->TitleBackround);
    this->physicsScene->addBody(ui->TitleForground,0.35f);
    this->physicsScene->addBody(ui->helpTitleButton,0.35);
    this->physicsScene->addBody(ui->learnTitleButton,0.35);
    this->physicsScene->addBody(ui->playTitleButton,0.4f);

    connect(this,&MainWindow::titleFallIn,this->physicsScene,&PhysicsScene::fallIn);
    connect(this,&MainWindow::titleFallOut,this->physicsScene,&PhysicsScene::fallOut);

    emit titleFallIn();
}

void MainWindow::PressedPlay(){
    emit titleFallOut();
    ui->stackedWidget->setCurrentIndex(firstLevelIndex);
}

void MainWindow::PressedInfo(){
    emit titleFallIn();
    ui->stackedWidget->setCurrentIndex(infoIndex);
}

void MainWindow::PressedHelp(){
    emit titleFallIn();
    ui->stackedWidget->setCurrentIndex(helpIndex);
}

void MainWindow::GoToMainMenue(){
    emit titleFallIn();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    std::cout << "resize" << std::endl;

}

void MainWindow::ANDGateSelection(){
        currentTool = AND;
        QPixmap p = QPixmap(":/icons/ANDGate.png");
        p.scaled(50, 50);
        QCursor c = QCursor(p, 0, 0);
        setCursor(c);
        std::cout <<"select AND gate" <<std::endl;
}

void MainWindow::NANDGateSelection(){
    currentTool = NAND;
    QPixmap p = QPixmap(":/icons/NANDGate.png");
    p.scaled(50, 50);
    QCursor c = QCursor(p, 0, 0);
    setCursor(c);
    std::cout <<"select NAND gate" <<std::endl;
}

void MainWindow::ORGateSelection(){
    currentTool = OR;
    QPixmap p = QPixmap(":/icons/ORGate.png");
    p.scaled(50, 50, Qt::IgnoreAspectRatio);
    QCursor c = QCursor(p, 0, 0);
    setCursor(c);
    std::cout <<"select OR gate" <<std::endl;

}

void MainWindow::NOTGateSelection(){
    currentTool = NOT;
    QPixmap p = QPixmap(":/icons/NOTGate.png");
    p.scaled(50, 50);
    QCursor c = QCursor(p, 0, 0);
    setCursor(c);
    std::cout <<"select NOT gate" <<std::endl;

}

void MainWindow::lineSelection(){
    currentTool = Wire;
    QPixmap p = QPixmap(":/icons/line.png");
    p.scaled(50, 50);
    QCursor c = QCursor(p, 0, 0);
    setCursor(c);
    std::cout <<"select line" <<std::endl;

}

