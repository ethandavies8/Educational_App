#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <Box2D/Box2D.h>
#include "physicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void PressedPlay();
    void PressedInfo();
    void PressedHelp();
    void GoToMainMenue();
signals:
    void titleFallOut();
    void titleFallIn();
private:
    Ui::MainWindow *ui;
    void setUpTitle();
    void connectTitle();
    void connectActions();
    PhysicsScene* physicsScene;
    b2World* world;
};
#endif // MAINWINDOW_H
