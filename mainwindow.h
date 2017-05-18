#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "resolutionratiodialog.h"
#include "qcustomplot.h"
#include "qrubberband.h"
#include "qpoint.h"
#include "CustomPlotZoom.h"
#include "customplotdrag.h"
#include "dbmanager.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CustomPlotZoom *customZoom;
    CustomPlotDrag *customDrag;

    DbManager dbmanage;

    void initSetCustomPlot(QCustomPlot *customPlot);
    void setResolutionRatio(double xAxis_MinValue,double xAxis_MaxValue,
                            double yAxis_MinValue,double yAxis_MaxValue,int width);
//    void setXYLabelValue();

    void showCrossLine();
    void hideCrossLine();    
    void showTextText1(double x, double y);
    void showTextText2(double x, double y);
    void clearText1();
    void clearText2();
    void showGreenCrossLine();
    void showRedCrossLine();    

    void moveGreenLine(double x, double y, int signal);
    void moveRedLine(double x, double y, int signal);
    void con_DashLine(bool mode);
    void calcu_text(bool mode);

    void startMovingGreen();
    void startMovingRed();
    void startZoom(const QPoint &mousePos);
    void startDrag();
//    void setDragMode(bool mode);


protected:
//    void mousePressEvent(QMouseEvent *);
//    void mouseMoveEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);

signals:
    // can use as state
//    void AxisRangeChanged();
    void activated(); //emitted on mouse over
    void disactivated(); //emitted when cursor leave
    void moved();


private slots:
    void on_resolutionRatioBtn_clicked();
    void on_actionOpen_triggered();
    void on_showCursorBtn_clicked();
    void on_hideCrosslineBtn_clicked();      

    void changeCursorType(QMouseEvent *event);

    void mousePressSignal(QMouseEvent *event);    
    void mouseReleaseSignal(QMouseEvent *event);

    void setDvalueText();
    void on_actionZoom_triggered();
    void on_reset_Chart_Btn_clicked();

    void onMouseMoveGreen(QMouseEvent *event);
    void stoppedMovingGreen();

    void onMouseMoveRed(QMouseEvent *event);
    void stoppedMovingRed();

    void onMouseMoveZoom(QMouseEvent *event);
    void stoppedZoom();

    void pressDrag(QMouseEvent *event);
    void onMouseMoveDrag(QMouseEvent *event);
    void stoppedDrag(QMouseEvent *event);


    void on_deleteLineBtn_clicked();

    void on_saveLineBtn_clicked();

    void on_search_pBtn_clicked();

private:
    Ui::MainWindow *ui;
    int moveSignal;
    const int cursorValue = 5; //coefficient of cursor change range
    void move(double factor, Qt::Orientation orientation);
    void zoom(double factor, int center, Qt::Orientation orientation);
    ResolutionRatioDialog *ratioDialog;
    QCPGraph *redGraph, *greenGraph;
    QCPItemTracer *mRCenterTracer;
    QCPItemTracer *mGCenterTracer;

    int crossLineCount,currentGraph,greenCrossLineGraph,redCrossLineGraph;    //graph id
    double greenLine_x,greenLine_y, redLine_x,redLine_y;                      //center point of  line pixel to coordinate
    bool left_mouse_hold;                                                     //mouse hold
    bool greenXSelected, greenYSelected, greenXYSelected,redXSelected, redYSelected, redXYSelected;  //which line has selected
    double cursor_click_x, cursor_click_y;                                     // cursor coordinate when mouse click
    double cursorMoveX, cursorMoveY;                                           // cursor coordinate when mouse move
    double greenline_dis_redline_in_xAxis, greenline_dis_redline_in_yAxis,
           cursor_dis_redline_in_xAxis, cursor_dis_redline_in_yAxis,
           cursor_dis_green_in_xAxis, cursor_dis_green_in_yAxis;
    double moving_dis_gc_x, moving_dis_gc_y, moving_dis_rc_x, moving_dis_rc_y;
    bool gx_move_state,gy_move_state,gcp_move_state,
         rx_move_state,ry_move_state,rcp_move_state,
         zoom_move_state;
    bool mPlotMode;
    bool mDragMode;
    bool right_mouse_hold;
    QPoint mDragStart;
    QCPRange DragStartHorzRange;
    QCPRange DragStartVertRange;   
    QRubberBand *mRubberBand;
    QPoint mOrigin;
    QPoint mEnd;
    int penWidth;
    QString chartName;
    QString filePath;
};

#endif // MAINWINDOW_H
