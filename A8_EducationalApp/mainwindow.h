#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <Box2D/Box2D.h>
#include "physicsscene.h"
#include "sceneitem.h"

class GraphicScene;
class Gate;

QT_BEGIN_NAMESPACE
class QGraphicsView;
namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);

public slots:
    void PressedPlay();
    void PressedInfo();
    void PressedHelp();
    void GoToMainMenue();
    void refreshGameView();
    void ANDGateSelection();
    //void mouseClicked();
    void resetTool();
    void NANDGateSelection();
    void ORGateSelection();
    void NOTGateSelection();
    void NORGateSelection();
    void XORGateSelection();
    void lineSelection();
    void selectToolSelection();
    void itemSelected(QGraphicsItem *item);
    void itemInserted(Gate *item);
signals:
    void titleFallOut();
    void titleFallIn();
    void mainWindowResized();
    void deleteEvent();
    void changeSelectedGate(QPixmap map);

private:
    Ui::MainWindow *ui;
    GraphicScene *scene;
    QGraphicsView *view;
    void setUpTitle();
    void connectTitle();
    void connectActions();
    void connectTools();
    void setupMouseIcons();
    PhysicsScene *physicsScene;
    b2World *world;
    int mainMenuIndex = 0;
    int infoIndex = 1;
    int helpIndex = 2;
    int firstLevelIndex = 3;
    QMenu *itemMenu;
    enum Tool
    {
        AND,
        OR,
        NOT,
        NAND,
        NOR,
        XOR,
        Wire,
        Deselect
    };
    Tool currentTool;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    QHash<Tool, QPixmap> mouseIcons;
};
#endif // MAINWINDOW_H
