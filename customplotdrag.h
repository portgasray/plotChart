#ifndef CUSTOMPLOTDRAG_H
#define CUSTOMPLOTDRAG_H
#include <QPoint>
#include "qcustomplot.h"

class QMouseEvent;
class QWidget;

class CustomPlotDrag: public QCustomPlot
{
    Q_OBJECT

public:
    CustomPlotDrag(QWidget * parent = 0);
    virtual ~CustomPlotDrag();
    void setDragMode(bool mode);

private slots:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    bool mDragMode;
    bool mouse_hold;
    QPoint mDragStart;
    QCPRange DragStartHorzRange;
    QCPRange DragStartVertRange;
};

#endif // CUSTOMPLOTDRAG_H
