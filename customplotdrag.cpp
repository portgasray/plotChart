#include "customplotdrag.h"

CustomPlotDrag::CustomPlotDrag(QWidget * parent)
       : QCustomPlot(parent)
       , mDragMode(false)
       , mouse_hold(false)
{}
CustomPlotDrag::~CustomPlotDrag()
{}

void CustomPlotDrag::setDragMode(bool mode)
{
    mDragMode = mode;
}

void CustomPlotDrag::mousePressEvent(QMouseEvent *event)
{
    if(mDragMode)
    {
        if(event->button()==Qt::RightButton)
        {
            this->setCursor(Qt::ClosedHandCursor);
            mDragStart = event->pos();
            mouse_hold = true;
            DragStartHorzRange = axisRect()->rangeDragAxis(Qt::Horizontal)->range();
            DragStartVertRange = axisRect()->rangeDragAxis(Qt::Vertical)->range();
        }
    }
    QCustomPlot::mousePress(event);
}

void CustomPlotDrag::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        mouse_hold = false;
    }
    QCustomPlot::mouseRelease(event);
}

void CustomPlotDrag::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_hold)   //drag
    {
        QCPAxis* Haxis = this->axisRect()->rangeDragAxis(Qt::Horizontal);
        QCPAxis* Vaxis = this->axisRect()->rangeDragAxis(Qt::Vertical);
        //      double diff = rangeDragHorzAxis->pixelToCoord(mDragStart.x()) / rangeDragHorzAxis->pixelToCoord(event->pos().x());
        //      rangeDragHorzAxis->setRange(mDragStartHorzRange.lower*diff, mDragStartHorzRange.upper*diff);
        double diff=0;
        if(xAxis->scaleType() == QCPAxis::stLinear)
        {
            diff = Haxis->pixelToCoord(mDragStart.x()) - Haxis->pixelToCoord(event->pos().x());
            Haxis->setRange(DragStartHorzRange.lower+diff, DragStartHorzRange.upper+diff);
        }
        else if(xAxis->scaleType() == QCPAxis::stLogarithmic)
        {
            diff = Haxis->pixelToCoord(mDragStart.x()) / Haxis->pixelToCoord(event->pos().x());
            Haxis->setRange(DragStartHorzRange.lower*diff, DragStartHorzRange.upper*diff);
        }
        if(yAxis->scaleType() == QCPAxis::stLinear)
        {
            diff = Vaxis->pixelToCoord(mDragStart.y()) - Vaxis->pixelToCoord(event->pos().y());
            Vaxis->setRange(DragStartVertRange.lower+diff, DragStartVertRange.upper+diff);
        }
        else if(yAxis->scaleType() == QCPAxis::stLogarithmic)
        {
            diff = Vaxis->pixelToCoord(mDragStart.y()) / Vaxis->pixelToCoord(event->pos().y());
            Vaxis->setRange(DragStartVertRange.lower*diff, DragStartVertRange.upper*diff);
        }
        replot();
    }
    QCustomPlot::mouseMove(event);
}

//void CustomPlotDrag::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button()==Qt::RightButton)                //drag
//    {
//        this->setCursor(Qt::ClosedHandCursor);
//        mDragStart = event->pos();
//        mouse_hold = true;
//        DragStartHorzRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Horizontal)->range();
//        DragStartVertRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Vertical)->range();
//    }
//    QCustomPlot::mousePressEvent(event);
//}

//void CustomPlotDrag::mouseMoveEvent(QMouseEvent *event)
//{
//    if(mouse_hold)   //drag
//    {
//        QCPAxis* Haxis = ui->customPlot->axisRect()->rangeDragAxis(Qt::Horizontal);
//        QCPAxis* Vaxis = ui->customPlot->axisRect()->rangeDragAxis(Qt::Vertical);
//        //      double diff = rangeDragHorzAxis->pixelToCoord(mDragStart.x()) / rangeDragHorzAxis->pixelToCoord(event->pos().x());
//        //      rangeDragHorzAxis->setRange(mDragStartHorzRange.lower*diff, mDragStartHorzRange.upper*diff);
//        double diff=0;
//        if(ui->customPlot->xAxis->scaleType() == QCPAxis::stLinear)
//        {
//            diff = Haxis->pixelToCoord(mDragStart.x()) - Haxis->pixelToCoord(event->pos().x());
//            Haxis->setRange(DragStartHorzRange.lower+diff, DragStartHorzRange.upper+diff);
//        }
//        else if(ui->customPlot->xAxis->scaleType() == QCPAxis::stLogarithmic)
//        {
//            diff = Haxis->pixelToCoord(mDragStart.x()) / Haxis->pixelToCoord(event->pos().x());
//            Haxis->setRange(DragStartHorzRange.lower*diff, DragStartHorzRange.upper*diff);
//        }
//        if(ui->customPlot->yAxis->scaleType() == QCPAxis::stLinear)
//        {
//            diff = Vaxis->pixelToCoord(mDragStart.y()) - Vaxis->pixelToCoord(event->pos().y());
//            Vaxis->setRange(DragStartVertRange.lower+diff, DragStartVertRange.upper+diff);
//        }
//        else if(ui->customPlot->yAxis->scaleType() == QCPAxis::stLogarithmic)
//        {
//            diff = Vaxis->pixelToCoord(mDragStart.y()) / Vaxis->pixelToCoord(event->pos().y());
//            Vaxis->setRange(DragStartVertRange.lower*diff, DragStartVertRange.upper*diff);
//        }
//        ui->customPlot->replot();
//    }
//    QCustomPlot::mouseMoveEvent(event);
//}

