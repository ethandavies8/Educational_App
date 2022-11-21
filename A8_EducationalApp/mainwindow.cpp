#include <Box2D/Box2D.h>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectActions();
    connectTitle();
    setUpTitle();
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
}

void MainWindow::PressedInfo(){
    emit titleFallIn();
}

void MainWindow::PressedHelp(){
    emit titleFallIn();
}

void MainWindow::GoToMainMenue(){
    emit titleFallIn();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    std::cout << "resize" << std::endl;

}
