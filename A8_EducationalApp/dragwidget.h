
#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QFrame>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE


class DragWidget : public QFrame
{
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = nullptr);
    //DragWidget& operator=(const DragWidget& other);
    void clearChildren();
    void AddItem(QPixmap map);
public slots:
    //void receiveMouse(QMouseEvent *event);
    void receiveDeleteKey();
signals:
    void resetTool();
private:
    QHash<int, QLabel*> gates;
    int lastLabelClickedIndex;
   // QLabel *trashIcon = new QLabel(this);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DRAGWIDGET_H
