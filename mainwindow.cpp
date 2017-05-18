#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mRubberBand = new QRubberBand(QRubberBand::Rectangle, ui->customPlot);
    this->crossLineCount = 0;  //control two line on board
    this->left_mouse_hold = false;
    this->right_mouse_hold = false;
    this->greenXSelected = false, greenYSelected = false, greenXYSelected = false;
    this->redXSelected = false,redYSelected = false,redXYSelected = false;
    this->moveSignal = 0;
    this->gx_move_state = true,this->gy_move_state = true, this->gcp_move_state = true,
    this->rx_move_state = true,this->ry_move_state = true, this->rcp_move_state = true;
    this->setWindowTitle("PlotChart");
    this->penWidth = 1;
    this->chartName = "凸轮曲线";
    filePath = QDir::currentPath();
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, chartName));
    ui->nowDateEdit->setDate(QDate::currentDate());   //set deadline time with current system time
    MainWindow::showMaximized();
    initSetCustomPlot(ui->customPlot);

    calcu_text(true);   // show value change of text

//    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(pressDrag(QMouseEvent*))); //drag
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseSignal(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)),this, SLOT(mousePressSignal(QMouseEvent*)));

      //Tips:
//    connect(&a1,SIGNAL(DataChanged(int)),&b1,SLOT(SetData(int)));
//    ui->statusBar->showMessage(tr("a1.Data=%1 b1.Data=%2").arg(a1.Data).arg(b1.Data));
    if(dbmanage.connectDB("DB\iFdbTL.db3")){
        qDebug() << "Database Create Sucessfully!";
    }
    else{
        qDebug() << "Database Create Failed!";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSetCustomPlot(QCustomPlot *customPlot)
{

//    customPlot->graph()->rescaleKeyAxis();
//    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
//                                      QCP::iSelectLegend | QCP::iSelectPlottables);    
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->yAxis->setAutoTickStep(false);
    customPlot->xAxis->setAutoSubTicks(false);
    customPlot->yAxis->setAutoSubTicks(false);
    customPlot->xAxis->setRange(0,3000);  //X: 0~3000
    customPlot->xAxis->setTickStep(200);  // tick :200
    customPlot->yAxis->setRange(-30000,30000);
    customPlot->yAxis->setTickStep(2000);
    customPlot->xAxis->setSubTickCount(4);
    customPlot->yAxis->setSubTickCount(4);
    customPlot->xAxis->setAutoTickCount(15);
    customPlot->yAxis->setAutoTickCount(20);

    // set title of plot:
//    ui->customPlot->plotLayout()->insertRow(0);
//    QCPTextElement *title = new QCPTextElement(ui->customPlot, chartName, QFont("sans", 17, QFont::Bold));
//    customPlot->plotLayout()->addElement(0, 0, title);

    // configure right and top axis to show labels but no ticks:
//    customPlot->xAxis2->setVisible(true);
//    customPlot->xAxis2->setTicks(false);
//    customPlot->xAxis2->setTickLabels(false);
//    customPlot->yAxis->setSubTickCount(5);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    // make top right axes clones of bottom left axes. Looks prettier:
//    customPlot->axisRect()->setupFullAxesBox();
    customPlot->xAxis->setLabel("X(degree)");
    customPlot->yAxis->setLabel("Y(um)");
    customPlot->replot();

//    emit AxisRangeChanged();   //emit axis changed
}

void MainWindow::setResolutionRatio(double xAxis_MinValue, double xAxis_MaxValue, double yAxis_MinValue, double yAxis_MaxValue, int width)
{
    double xMinValue = xAxis_MinValue;
    double xMaxValue = xAxis_MaxValue;
    double yMinValue = yAxis_MinValue;
    double yMaxValue = yAxis_MaxValue;
    penWidth = width;
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setAutoTickStep(true);
    ui->customPlot->xAxis->setAutoSubTicks(false);
    ui->customPlot->yAxis->setAutoSubTicks(false);
    ui->customPlot->xAxis->setRange(xMinValue, xMaxValue);    
    ui->customPlot->yAxis->setRange(yMinValue, yMaxValue);
    ui->customPlot->xAxis->setSubTickCount(4);
    ui->customPlot->yAxis->setSubTickCount(4);
    ui->customPlot->replot();
}

//show dash line
void MainWindow::showGreenCrossLine()
{
    greenGraph = ui->customPlot->addGraph();
    mGCenterTracer = new QCPItemTracer(ui->customPlot);
    greenGraph->setPen(Qt::NoPen);
    ui->customPlot->addItem(mGCenterTracer);
    mGCenterTracer->setGraph(greenGraph);
    QPen pen;
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(1);
    pen.setBrush(Qt::green);
    mGCenterTracer->setPen(pen);
    greenLine_x = ui->customPlot->xAxis->range().center();
    greenLine_y = ui->customPlot->yAxis->range().center();
    greenGraph->addData(greenLine_x,greenLine_y);
    ui->customPlot->replot();
    showTextText1(greenLine_x,greenLine_y);
}

void MainWindow::showRedCrossLine()
{
    redGraph = ui->customPlot->addGraph();
    mRCenterTracer = new QCPItemTracer(ui->customPlot);
    redGraph->setPen(Qt::NoPen);  //
    ui->customPlot->addItem(mRCenterTracer);
    mRCenterTracer->setGraph(redGraph);
    QPen pen;
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(1);
    pen.setBrush(Qt::red);
    mRCenterTracer->setPen(pen);
    redLine_x = ui->customPlot->xAxis->range().center();
    redLine_y = ui->customPlot->yAxis->range().center();
    redGraph->addData(redLine_x,redLine_y);
    ui->customPlot->replot();
    showTextText2(redLine_x,redLine_y);    
}

void  MainWindow::hideCrossLine()
{
    if(crossLineCount == 2)
    {
        ui->customPlot->removeGraph(currentGraph);
        ui->customPlot->removeItem(mRCenterTracer);
        crossLineCount --;
        clearText2();
    }
    else if(crossLineCount == 1)
    {
        ui->customPlot->removeGraph(currentGraph);
        ui->customPlot->removeItem(mGCenterTracer);
        clearText1();
        crossLineCount --;
    }
//    if(crossLineCount == 0)
//    {
//        con_DashLine(false);
//    }
    ui->customPlot->replot();
}

void MainWindow::showTextText1(double x,double y)
{
    QString Str_x = QString::number(x,'f',2);
    QString Str_y = QString::number(y,'f',2);
    ui->x1Text->setText(Str_x);
    ui->y1Text->setText(Str_y);
}

void MainWindow::showTextText2(double x,double y)
{
    QString Str_x = QString::number(x,'f',2);
    QString Str_y = QString::number(y,'f',2);
    ui->x2Text->setText(Str_x);
    ui->y2Text->setText(Str_y);
}

void MainWindow::clearText1()
{
    ui->x1Text->setText("0");
    ui->y1Text->setText("0");
}

void MainWindow::clearText2()
{
    ui->x2Text->setText("0");
    ui->y2Text->setText("0");
}

void MainWindow::on_resolutionRatioBtn_clicked()
{

    ratioDialog = new ResolutionRatioDialog(this);
    ratioDialog->setModal(true);
    ratioDialog->show();

    //#2 anyother way to get data from dialog
//    if(ratioDialog->exec())
//    {
//        double xAxis_MinValue,xAxis_MaxValue,yAxis_MinValue,yAxis_MaxValue;
//        ratioDialog->getResolutionRiato(xAxis_MinValue,xAxis_MaxValue,yAxis_MinValue,yAxis_MaxValue);
//        setResolutionRatio(xAxis_MinValue,xAxis_MaxValue,yAxis_MinValue,yAxis_MaxValue);
//    }
}

void MainWindow::on_showCursorBtn_clicked()
{   
    if(crossLineCount == 0)  //
    {
       showGreenCrossLine();
       crossLineCount ++; 
    }
    else if (crossLineCount == 1)
    {
        showRedCrossLine();    
        crossLineCount ++;
    }
    if(crossLineCount > 0)
    {             
//        con_DashLine(true);  //connect signal & slot to listen mouse event
        connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(changeCursorType(QMouseEvent*)));
    }
}

void MainWindow::on_hideCrosslineBtn_clicked()
{
    hideCrossLine();
}

//cursor change
void MainWindow::changeCursorType(QMouseEvent *event)
{
    double cursor_move_x = event->pos().x();
    double cursor_move_y = event->pos().y();
    //coordToPixel
    int glineXPixel = ui->customPlot->xAxis->coordToPixel (greenLine_x);
    int glineYPixel = ui->customPlot->yAxis->coordToPixel (greenLine_y);

    int rlineXPixel = ui->customPlot->xAxis->coordToPixel (redLine_x);
    int rlineYPixel = ui->customPlot->yAxis->coordToPixel (redLine_y);

    moving_dis_gc_x = qAbs(glineXPixel - cursor_move_x);
    moving_dis_gc_y = qAbs(glineYPixel - cursor_move_y);
    moving_dis_rc_x = qAbs(rlineXPixel - cursor_move_x);
    moving_dis_rc_y = qAbs(rlineYPixel - cursor_move_y);

    if(moving_dis_gc_x < cursorValue || moving_dis_gc_y < cursorValue || moving_dis_rc_x < cursorValue || moving_dis_rc_y < cursorValue)
    {
        if(moving_dis_gc_x < cursorValue || moving_dis_rc_x < cursorValue )
        {
            this->setCursor(Qt::SplitHCursor);
        }
        if(moving_dis_gc_y < cursorValue || moving_dis_rc_y < cursorValue)
        {
            this->setCursor(Qt::SplitVCursor);
        }
        if((moving_dis_gc_x < cursorValue && moving_dis_gc_y < cursorValue ) || (moving_dis_rc_x < cursorValue && moving_dis_rc_y < cursorValue))
        {
            this->setCursor(Qt::SizeAllCursor);
        }
    }
    else
    {
       this->setCursor(Qt::ArrowCursor);
    }
}

//move dash line
void MainWindow::mousePressSignal(QMouseEvent *event)
{   
    //where click happened
    cursor_click_x = event->pos().x();
    cursor_click_y = event->pos().y();

    //get Pixel position
    int glCenterPixel_x = ui->customPlot->xAxis->coordToPixel (greenLine_x); //green
    int glCenterPixel_y = ui->customPlot->yAxis->coordToPixel (greenLine_y);

    int rlCenterPixel_x = ui->customPlot->xAxis->coordToPixel (redLine_x);   //red
    int rlCenterPixel_y = ui->customPlot->yAxis->coordToPixel (redLine_y);

    greenline_dis_redline_in_xAxis = qAbs(rlCenterPixel_x - glCenterPixel_x);  // red & green distance in xAxis
    greenline_dis_redline_in_yAxis = qAbs(rlCenterPixel_y - glCenterPixel_y);  // red & green distance in yAxis


    if(event->button()== Qt::LeftButton)                  //zoom
    {
        left_mouse_hold = true;
    }

    if(event->button()== Qt::RightButton)                //drag
    {
        this->setCursor(Qt::ClosedHandCursor);
        mDragStart = event->pos();
        right_mouse_hold = true;
        DragStartHorzRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Horizontal)->range();
        DragStartVertRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Vertical)->range();
        startDrag();
    }
    if(left_mouse_hold)  //when left button click
    {
        cursor_dis_green_in_xAxis = qAbs(glCenterPixel_x - cursor_click_x);
        cursor_dis_green_in_yAxis = qAbs(glCenterPixel_y - cursor_click_y);
        //redline shown
        if(crossLineCount == 2)
        {
            cursor_dis_redline_in_xAxis = qAbs(rlCenterPixel_x - cursor_click_x);
            cursor_dis_redline_in_yAxis = qAbs(rlCenterPixel_y - cursor_click_y);
        }
    }
    //DistributeAction();
    if(crossLineCount == 0)
    {
        if(left_mouse_hold)
        {
            startZoom(event->pos());  //start point
        }
    }
    //green line move action
    if(crossLineCount == 1)
    {
        if(cursor_dis_green_in_xAxis < cursorValue || cursor_dis_green_in_yAxis < cursorValue)
        {
            if(left_mouse_hold)
            {
                startMovingGreen();
                if(cursor_dis_green_in_xAxis < cursorValue && cursor_dis_green_in_yAxis < cursorValue)
                {
                    //move green center point signal 1
                    moveSignal = 1;    
                }
                else if(cursor_dis_green_in_yAxis < cursorValue)
                {
                    //move green x  signal 2
                    moveSignal = 2;
    //                startMovingGreen();
                }
                else if(cursor_dis_green_in_xAxis < cursorValue)
                {
                    //move green y  signal 3
                     moveSignal = 3;
    //                 startMovingGreen();
                }
            }
        }
        else  //zoom
        {
            if(left_mouse_hold)
            {
                startZoom(event->pos());  //start point
            }
        }
    }
    // two line
    if(crossLineCount == 2)
    {
        //in line area
        if(cursor_dis_green_in_xAxis < cursorValue || cursor_dis_green_in_yAxis < cursorValue || cursor_dis_redline_in_xAxis < cursorValue || cursor_dis_redline_in_yAxis < cursorValue)
        {
            if(left_mouse_hold)
            {
                startMovingGreen();
                startMovingRed();
                //overlapping in x aex or overlapping in y aex or both
                if(greenline_dis_redline_in_xAxis < 2*cursorValue || greenline_dis_redline_in_yAxis < 2*cursorValue)
                {
                    //overlapping in centerpoint
                    if(greenline_dis_redline_in_xAxis < 2*cursorValue && greenline_dis_redline_in_yAxis < 2*cursorValue)
                    {
                        stoppedMovingRed();
                    }
                    if(greenline_dis_redline_in_yAxis < 2*cursorValue) //overlapping in x aex
                    {
                        //prohibit  red line x aex  use signal 2 replace signal 5
                        //two center point frist
                        if(cursor_dis_green_in_xAxis < cursorValue && cursor_dis_green_in_yAxis < cursorValue)
                        {
                            //move green center point signal 1
                            moveSignal = 1;
                        }
                        else if(cursor_dis_redline_in_xAxis < cursorValue && cursor_dis_redline_in_yAxis < cursorValue)
                        {
                            //move red center point signal 4
                            moveSignal = 4;
                        }
                        else if(cursor_dis_green_in_yAxis < cursorValue)
                        {
                            //move green x  signal 2
                            moveSignal = 2;
                        }
                        else if(cursor_dis_green_in_xAxis < cursorValue)
                        {
                            //move green y  signal 3
                             moveSignal = 3;
                        }
                        else if(cursor_dis_redline_in_yAxis < cursorValue)
                        {
                            //overlapping in x aex
                            moveSignal = 2;           //use signal 2 replace signal 5
                        }
                        else if(cursor_dis_redline_in_xAxis < cursorValue)
                        {
                            //move red x  signal 6
                            moveSignal = 6;
                        }
                    }
                    else if(greenline_dis_redline_in_xAxis < 2*cursorValue) //overlapping in y aex
                    {
                        //prohibit  red line y aex  use signal 3 replace signal 6
                        //two centerpoint first
                        if(cursor_dis_green_in_xAxis < cursorValue && cursor_dis_green_in_yAxis < cursorValue)
                        {
                            //move green center point signal 1
                            moveSignal = 1;
                        }
                        else if(cursor_dis_redline_in_xAxis < cursorValue && cursor_dis_redline_in_yAxis < cursorValue)
                        {
                            //move red center point signal 4
                            moveSignal = 4;
                        }
                        else if(cursor_dis_green_in_yAxis < cursorValue)
                        {
                            //move green x  signal 2
                            moveSignal = 2;
                        }
                        else if(cursor_dis_green_in_xAxis < cursorValue)
                        {
                            //move green y  signal 3
                             moveSignal = 3;
                        }
                        else if(cursor_dis_redline_in_yAxis < cursorValue)
                        {
                            //move red x  signal 5
                            moveSignal = 5;
                        }
                        else if(cursor_dis_redline_in_xAxis < cursorValue)
                        {
                            //move green  y  use signal 3 replace signal 6
                            moveSignal = 3;
                        }

                    }
                }
                else // different
                {
                    if(left_mouse_hold)
                    {
                        if(cursor_dis_green_in_xAxis < cursorValue && cursor_dis_green_in_yAxis < cursorValue)
                        {
                            //move green center point signal 1
                            moveSignal = 1;
                        }
                        else if(cursor_dis_green_in_yAxis < cursorValue)
                        {
                            //move green x  signal 2
                            moveSignal = 2;
                        }
                        else if(cursor_dis_green_in_xAxis < cursorValue)
                        {
                            //move green y  signal 3
                             moveSignal = 3;
                        }
                        else if(cursor_dis_redline_in_xAxis < cursorValue && cursor_dis_redline_in_yAxis < cursorValue)
                        {
                            //move red center point signal 4
                            moveSignal = 4;
                        }
                        else if(cursor_dis_redline_in_yAxis < cursorValue)
                        {
                            //move red x  signal 5
                            moveSignal = 5;
                        }
                        else if(cursor_dis_redline_in_xAxis < cursorValue)
                        {
                            //move red x  signal 6
                            moveSignal = 6;
                        }
                    }
                }
            }
        }
        else
        {
            //zoom
            if(left_mouse_hold)
            {
                startZoom(event->pos());
            }
        }
    }
}

void MainWindow::mouseReleaseSignal(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        left_mouse_hold = false;
        this->setCursor(Qt::ArrowCursor);

        //pass parameter to zoom
        mEnd = event->pos();
        //disable zoom move
//           disconnect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveDrag(QMouseEvent*)));
//           disconnect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedDrag(QMouseEvent*)));
    }

    if(event->button()==Qt::RightButton)
       {
           right_mouse_hold = false;
           this->setCursor(Qt::ArrowCursor);
           //disable drag
//           disconnect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveDrag(QMouseEvent*)));
//           disconnect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedDrag(QMouseEvent*)));
       }
}

void MainWindow::startMovingGreen()
{
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveGreen(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedMovingGreen()));
}

void MainWindow::onMouseMoveGreen(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    cursorMoveX = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    cursorMoveY = ui->customPlot->yAxis->pixelToCoord(event->pos().y());
    moveGreenLine(cursorMoveX, cursorMoveY, moveSignal);
}

void MainWindow::stoppedMovingGreen()
{
    disconnect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveGreen(QMouseEvent*)));
    disconnect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedMovingGreen()));
}

void MainWindow::startMovingRed()
{
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveRed(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedMovingRed()));
}

void MainWindow::onMouseMoveRed(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    cursorMoveX = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    cursorMoveY = ui->customPlot->yAxis->pixelToCoord(event->pos().y());
    moveRedLine(cursorMoveX, cursorMoveY, moveSignal);
}

void MainWindow::stoppedMovingRed()
{
    disconnect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveRed(QMouseEvent*)));
    disconnect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedMovingRed()));
}

void MainWindow::moveGreenLine(double x, double y, int signal)
{
    switch(signal)
    {
        case 1:                 //move center point
            greenLine_x = x;
            greenLine_y = y;
            break;
        case 2:                 //move x aex
            greenLine_y = y;
            break;
        case 3:
            greenLine_x = x;    //move y aex
            break;
    }
    greenGraph->addData(greenLine_x,greenLine_y);
    mGCenterTracer->setGraphKey(greenLine_x);
    ui->customPlot->replot();
    showTextText1(greenLine_x,greenLine_y);
}

void MainWindow::moveRedLine(double x, double y, int signal)
{
    switch(signal)
    {
        case 4:
            redLine_x = x;
            redLine_y = y;
            break;
        case 5:
            redLine_y = y;
            break;
        case 6:
            redLine_x = x;
            break;
    }
    redGraph->addData(redLine_x,redLine_y);
    mRCenterTracer->setGraphKey(redLine_x);
    ui->customPlot->replot();
    showTextText2(redLine_x,redLine_y);
}

//zoom
void MainWindow::startZoom(const QPoint &mousePos)
{
    this->setCursor(Qt::CrossCursor);
    mOrigin = mousePos;
    mRubberBand->setGeometry(QRect(mOrigin, QSize()));
    mRubberBand->show();
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveZoom(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedZoom()));
}

void MainWindow::onMouseMoveZoom(QMouseEvent *event)
{
    if (mRubberBand->isVisible())
    {
        mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
    }
}

void MainWindow::stoppedZoom()
{
    if (mRubberBand->isVisible())
    {
        const QRect & zoomRect = mRubberBand->geometry();
        int xp1, yp1, xp2, yp2;
        zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
        double x1 = ui->customPlot->xAxis->pixelToCoord(xp1);
        double x2 = ui->customPlot->xAxis->pixelToCoord(xp2);
        double y1 = ui->customPlot->yAxis->pixelToCoord(yp1);
        double y2 = ui->customPlot->yAxis->pixelToCoord(yp2);
        //positive direction
        if((mEnd.x() - mOrigin.x()) > 10)
        {
            //a property rect 20*20
            if((qAbs(xp1-xp2) >20) && (qAbs(yp1-yp2) > 20))
            {
                //auto set ticks
                ui->customPlot->xAxis->setRange(x1, x2);
                ui->customPlot->yAxis->setRange(y1, y2);

                ui->customPlot->xAxis->setAutoTicks(true);
                ui->customPlot->yAxis->setAutoTicks(true);

                ui->customPlot->xAxis->setTickLabels(true);
                ui->customPlot->yAxis->setTickLabels(true);

                ui->customPlot->xAxis->setAutoTickStep(true);
                ui->customPlot->yAxis->setAutoTickStep(true);

                ui->customPlot->xAxis->setAutoSubTicks(true);
                ui->customPlot->yAxis->setAutoSubTicks(true);
            }
        }

        //    negative direction
        if(((mOrigin.x() - mEnd.x()) > 20) &&(mOrigin.y() - mEnd.y() > 20))
        {
            initSetCustomPlot(ui->customPlot);  //reset plot
        }
        mRubberBand->hide();
        ui->customPlot->replot();
    }

    disconnect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveZoom(QMouseEvent*)));
    disconnect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedZoom()));
}

void MainWindow::con_DashLine(bool mode)
{
    mPlotMode = mode;
    if(mPlotMode)
    {
        connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)),this, SLOT(mousePressSignal(QMouseEvent*)));
    }
}

void MainWindow::move(double factor, Qt::Orientation orientation)
{
    QCPAxis* axis = ui->customPlot->axisRect()->rangeDragAxis(orientation);
    //  double rg = (axis->range().upper - axis->range().lower)*(factor);
    //  axis->setRange(axis->range().lower+(rg), axis->range().upper+(rg));
    double rg =0.0;
    DragStartHorzRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Horizontal)->range();
    DragStartVertRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Vertical)->range();
    if(ui->customPlot->xAxis->scaleType() == QCPAxis::stLinear)
    {
        rg = (axis->range().upper - axis->range().lower)*(factor/10);
        axis->setRange(axis->range().lower+(rg), axis->range().upper+(rg));
    }
    else if(ui->customPlot->xAxis->scaleType() == QCPAxis::stLogarithmic)
    {
        //      rg = (axis->range().upper / axis->range().lower)*(factor/100);
        int start,stop;
        double diff;
        if(factor>0.0)
        {
            stop =this->width()*factor/10;
            start = 2*this->width()*factor/10;
        }
        if(factor<0.0)
        {
            factor*=-1.0;
            start =this->width()*factor/10;
            stop = 2*this->width()*factor/10;
        }
        diff = axis->pixelToCoord(start) / axis->pixelToCoord(stop);
        axis->setRange(ui->customPlot->axisRect()->rangeDragAxis(orientation)->range().lower*diff, ui->customPlot->axisRect()->rangeDragAxis(orientation)->range().upper*diff);
    }
    ui->customPlot->replot();
}

void MainWindow::zoom(double factor, int center, Qt::Orientation orientation)
{
    QCPAxis* axis = ui->customPlot->axisRect()->rangeZoomAxis(orientation);
    axis->scaleRange(factor, axis->pixelToCoord(center));
    ui->customPlot->replot();
}

//drag
void MainWindow::startDrag()
{
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveDrag(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedDrag(QMouseEvent*)));
}

void MainWindow::pressDrag(QMouseEvent *event)
{
//    if(event->button()== Qt::RightButton)                //drag
//    {
//        this->setCursor(Qt::ClosedHandCursor);
//        mDragStart = event->pos();
//        right_mouse_hold = true;
//        DragStartHorzRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Horizontal)->range();
//        DragStartVertRange = ui->customPlot->axisRect()->rangeDragAxis(Qt::Vertical)->range();
//        startDrag();
//    }
//    QCustomPlot::mousePressEvent(event);
}

void MainWindow::onMouseMoveDrag(QMouseEvent *event)
{
    if(right_mouse_hold)   //drag
    {
        QCPAxis* Haxis = ui->customPlot->axisRect()->rangeDragAxis(Qt::Horizontal);
        QCPAxis* Vaxis = ui->customPlot->axisRect()->rangeDragAxis(Qt::Vertical);
        //      double diff = rangeDragHorzAxis->pixelToCoord(mDragStart.x()) / rangeDragHorzAxis->pixelToCoord(event->pos().x());
        //      rangeDragHorzAxis->setRange(mDragStartHorzRange.lower*diff, mDragStartHorzRange.upper*diff);
        double diff=0;
        if(ui->customPlot->xAxis->scaleType() == QCPAxis::stLinear)
        {
            diff = Haxis->pixelToCoord(mDragStart.x()) - Haxis->pixelToCoord(event->pos().x());
            Haxis->setRange(DragStartHorzRange.lower+diff, DragStartHorzRange.upper+diff);
        }
        else if(ui->customPlot->xAxis->scaleType() == QCPAxis::stLogarithmic)
        {
            diff = Haxis->pixelToCoord(mDragStart.x()) / Haxis->pixelToCoord(event->pos().x());
            Haxis->setRange(DragStartHorzRange.lower*diff, DragStartHorzRange.upper*diff);
        }
        if(ui->customPlot->yAxis->scaleType() == QCPAxis::stLinear)
        {
            diff = Vaxis->pixelToCoord(mDragStart.y()) - Vaxis->pixelToCoord(event->pos().y());
            Vaxis->setRange(DragStartVertRange.lower+diff, DragStartVertRange.upper+diff);
        }
        else if(ui->customPlot->yAxis->scaleType() == QCPAxis::stLogarithmic)
        {
            diff = Vaxis->pixelToCoord(mDragStart.y()) / Vaxis->pixelToCoord(event->pos().y());
            Vaxis->setRange(DragStartVertRange.lower*diff, DragStartVertRange.upper*diff);
        }
        ui->customPlot->replot();
    }
//    QCustomPlot::mouseMoveEvent(event);
}

void MainWindow::stoppedDrag(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        right_mouse_hold = false;
        this->setCursor(Qt::ArrowCursor);
        disconnect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMoveDrag(QMouseEvent*)));
        disconnect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(stoppedDrag(QMouseEvent*)));
    }
//    QCustomPlot::mouseRelease(event);
}

//calculate value of x2-x1 and y2-y1
void MainWindow::setDvalueText()
{
    double x1 = ui->x1Text->text().toDouble();
    double x2 = ui->x2Text->text().toDouble();
    double y1 = ui->y1Text->text().toDouble();
    double y2 = ui->y2Text->text().toDouble();
    double d_dalue_x = x2 - x1;
    double d_cursorValue = y2 - y1;

    QString str_x = QString::number(d_dalue_x,'f',2);
    QString str_y = QString::number(d_cursorValue,'f',2);
    ui->DValueXText->setText(str_x);
    ui->DValueYText->setText(str_y);
}

void MainWindow::calcu_text(bool mode)
{
    if(mode)
    {
        connect(ui->x1Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
        connect(ui->y1Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
        connect(ui->x2Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
        connect(ui->y2Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
    }
    else
    {
        disconnect(ui->x1Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
        disconnect(ui->y1Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
        disconnect(ui->x2Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
        disconnect(ui->y2Text,SIGNAL(textChanged(QString)),this,SLOT(setDvalueText()));
    }
}

void MainWindow::on_actionZoom_triggered()
{

}

//open
void MainWindow::on_actionOpen_triggered()
{
    QVector <double> pointX,pointY1,pointY2;
//    int lineCount = 2;
//    QTextStream cout(stdout, QIODevice::ReadOnly);
           QString filename = QFileDialog::getOpenFileName(
              this,
              "Open Document",
              filePath,     //QDir::currentPath()
              "Document files (*.csv);;All files(*.*)");
           if (!filename.isNull()) {
              QFile file(filename);
              QStringList pathfields = filename.split('/');
              QString temp = pathfields[pathfields.length()-1];
              int pathLength = filename.length();
              int fileLength = temp.length();
              filePath = filename.remove(pathLength-fileLength,fileLength);
              if(file.open(QIODevice::ReadOnly))
              {
                  QTextStream in(&file);
                  while (!in.atEnd())
                  {
                      QString line =in.readLine();  // read first line and so on
                      QStringList fields = line.split(','); //split the string
                      pointX.append(fields.at(0).toDouble());  // take first value and stored into x
                      pointY1.append(fields.at(1).toDouble()); // take second value and stored into y1
                      pointY2.append(fields.at(2).toDouble()); // take third value and stored into y2
                  }
              }
           }
           // create graph and assign data to it:
           ui->customPlot->clearGraphs();

           QCPGraph *firstGraph = ui->customPlot->addGraph();
           firstGraph->setPen(QPen(Qt::blue,penWidth,Qt::SolidLine));
           firstGraph->setData(pointX, pointY1);

           QCPGraph *secondGraph = ui->customPlot->addGraph();
           secondGraph->setPen(QPen(Qt::red,penWidth,Qt::SolidLine));
           secondGraph->setData(pointX, pointY2);
           ui->customPlot->replot();
}

//reset
void MainWindow::on_reset_Chart_Btn_clicked()
{    
    initSetCustomPlot(ui->customPlot);  //reset plot
}


void MainWindow::on_deleteLineBtn_clicked()
{
    //after open line then clean QCPGraph
}

void MainWindow::on_saveLineBtn_clicked()
{
    QString pictureName = chartName + ".png";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               pictureName,
                               tr("Images (*.png)"));
    ui->customPlot->grab().save(fileName);
//    QPixmap pixmap = QPixmap.loadFromData((const uchar *) imageBuffer_pointer,
//        (sizeof(imageRows) * sizeof(imageCols));
//    pixmap.save(fileName, "JPG");
}

void MainWindow::on_search_pBtn_clicked()
{

}
