#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <Box2D/Box2D.h>
#include "fallingstackedframe.h"
#include "dragwidget.h"

QT_BEGIN_NAMESPACE
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
    DragWidget dragWidget;
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
signals:
    void fallTo(int frame);
    void titleFallOut();
    void titleFallIn();
    void mainWindowResized();
    void deleteEvent();

private:
    FallingStackedFrame* fallFrame;
    Ui::MainWindow *ui;
    void setUpTitleFall();
    void connectTitle();
    void connectActions();
    void connectTools();
    void setupMouseIcons();
    b2World *world;
    int mainMenuIndex = 0;
    int infoIndex = 1;
    int helpIndex = 2;
    int firstLevelIndex = 3;
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
