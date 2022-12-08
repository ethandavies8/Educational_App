#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <Box2D/Box2D.h>
#include "fallingstackedframe.h"
#include <QGraphicsItem>
#include "physicsscene.h"
#include "gate.h"

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
    void testCircuit();
    void truthTableRowCorrect(int);
    void nextLevel();
signals:
    void fallTo(int frame);
    void titleFallOut();
    void titleFallIn();
    void mainWindowResized();
    void deleteEvent();
    void changeSelectedGate(QPixmap map);
    void changeItemType(Gate::GateType);
    void checkTwoGateLevel();
    void checkThreeGateLevel(int);
    void checkFourGateLevel(int);
    void checkChallengeLevel();
    //void checkLevelTwo();

private:
    FallingStackedFrame* fallFrame;
    Ui::MainWindow *ui;
    void setUpTitleFall();
    GraphicScene *scene;
    QGraphicsView *view;
    void setUpTitle();
    void connectTitle();
    void connectActions();
    void connectTools();
    void connectScene();
    void setupMouseIcons();
    void clearPreviousLevel();

    void removeBackground(QWidget* widget);

    void makeLevelTwo();
    void makeLevelThree();
    void makeLevelFour();
    void makeLevelFive();
    void makeLevelSix();
    void makeLevelSeven();
    void makeLevelEight();
    void makeLevelNine();
    void makeChallengeLevel();


    b2World *world;
    int mainMenuIndex = 0;
    int infoIndex = 1;
    int helpIndex = 2;
    int firstLevelIndex = 3;
    int currentLevelIndex = 0;
    int currentRowCount = 2;
    QMenu *itemMenu;
    QBrush truthTableBackground;
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
private slots:
    void on_HelpHome_clicked();
    void on_learnHome_clicked();
    void on_Level1Home_clicked();
};
#endif // MAINWINDOW_H
