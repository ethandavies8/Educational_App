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
    void resizeEvent(QResizeEvent *event) override;

public slots:
    //pressed button slots
    void PressedPlay();
    void PressedInfo();
    void PressedHelp();
    void PressedLevels();
    void GoToMainMenue();
    //tool and gate selection slots
    void refreshGameView();
    void ANDGateSelection();
    void NANDGateSelection();
    void ORGateSelection();
    void NOTGateSelection();
    void NORGateSelection();
    void XORGateSelection();
    void resetTool();
    void lineSelection();
    void selectToolSelection();
    //circuit and level slots
    void testCircuit();
    void nextLevel();
    void unlockAllLevels();
    void checkTruthTable(QVector<bool>);
signals:
    //ui signals
    void fallTo(int frame);
    void titleFallOut();
    void titleFallIn();
    void mainWindowResized();
    void deleteEvent();
    //backend data signals
    void changeSelectedGate(QPixmap map);
    void changeItemType(Gate::GateType);
    void checkTwoGateLevel();
    void checkThreeGateLevel(int);
    void checkFourGateLevel(int);
    void checkChallengeLevel();

private:
    FallingStackedFrame* fallFrame;
    Ui::MainWindow *ui;
    void setUpTitleFall();
    GraphicScene *scene;
    QGraphicsView *view;
    //prviate setup helper methods
    void setUpTitle();
    void connectTitle();
    void connectActions();
    void connectTools();
    void connectScene();
    void connectLevelSelect();
    void setupMouseIcons();
    void truthTableSetup();

    //level methods
    void clearPreviousLevel();
    void cleanSlate();
    void setOutputs(QString);
    void makeLevelOne();
    void makeLevelTwo();
    void makeLevelThree();
    void makeLevelFour();
    void makeLevelFive();
    void makeLevelSix();
    void makeLevelSeven();
    void makeLevelEight();
    void makeLevelNine();
    void makeChallengeLevel();
    void levelUnlocked(int);
    void enableAllGateButtons();
    void endLevelLoad();
    void threeGateUiUpdate();
    void fourGateGuiUpdate();
    void fillFourRowTruthTable();
    void fillEightRowTruthTable();

    void removeBackground(QWidget* widget);

    //global variables
    b2World *world;
    int mainMenuIndex = 0;
    int currentLevelIndex = 0;
    int infoIndex = 1;
    int helpIndex = 2;
    int firstLevelIndex = 3;
    int levelSelectIndex = 4;
    int gameOverIndex = 5;
    int currentRowCount = 2;
    QMenu *itemMenu;
    bool seenHelpMenu = false;

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

    void ShowANDialog();
    void ShowORDialog();
    void ShowNOTDialog();
    void ShowXORDialog();
    void ShowNANDDialog();
    void ShowNORDialog();

protected:
    QHash<Tool, QPixmap> mouseIcons;
private slots:
    void on_HelpHome_clicked();
    void on_learnHome_clicked();
    void on_Level1Home_clicked();
    void on_levelSelectHome_clicked();
    void on_level1Button_clicked();
    void on_level2Button_clicked();
    void on_level3Button_clicked();
    void on_level4Button_clicked();
    void on_level5Button_clicked();
    void on_level6Button_clicked();
    void on_level7Button_clicked();
    void on_level8Button_clicked();
    void on_level9Button_clicked();
    void on_level10Button_clicked();
};
#endif // MAINWINDOW_H
