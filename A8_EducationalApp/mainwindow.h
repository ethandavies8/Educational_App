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
    void resizeEvent(QResizeEvent* event);

public slots:
    void PressedPlay();
    void PressedInfo();
    void PressedHelp();
    void GoToMainMenue();
    void ANDGateSelection();
    void NANDGateSelection();
    void ORGateSelection();
    void NOTGateSelection();
    void NORGateSelection();
    void XORGateSelection();
    void lineSelection();
    void selectToolSelection();
signals:
    void titleFallOut();
    void titleFallIn();
    void mainWindowResized();
private:
    Ui::MainWindow *ui;
    void setUpTitle();
    void connectTitle();
    void connectActions();
    void connectTools();
    void setupMouseIcons();
    PhysicsScene* physicsScene;
    b2World* world;
    int mainMenuIndex = 0;
    int infoIndex = 1;
    int helpIndex = 2;
    int firstLevelIndex = 3;
    enum Tool {AND, OR, NOT, NAND, NOR, XOR, Wire, Deselect};
    Tool currentTool;
    QHash<Tool, QPixmap> mouseIcons;
};
#endif // MAINWINDOW_H
