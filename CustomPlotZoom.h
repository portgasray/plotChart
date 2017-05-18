
#include <QPoint>
#include "qcustomplot.h"

class QRubberBand;
class QMouseEvent;
class QWidget;

class CustomPlotZoom : public QCustomPlot
{
    Q_OBJECT
public:
    CustomPlotZoom(QWidget *parent = 0);
    virtual ~CustomPlotZoom();
    void setZoomMode(bool mode);

private slots:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    bool mZoomMode;
    QRubberBand *mRubberBand;
    QPoint mOrigin;
};
