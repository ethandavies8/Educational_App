#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Box2D/Box2D.h>

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
    void Pressed();
private slots:
    void UpdateWorld();
private:
    Ui::MainWindow *ui;
    void titleFallOut();
    void setUpTitle();
    void titleFallIn();
    b2World* world;
    b2Body* body;
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;
};
#endif // MAINWINDOW_H
