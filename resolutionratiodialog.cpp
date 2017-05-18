#include "resolutionratiodialog.h"
#include "ui_resolutionratiodialog.h"
#include "mainwindow.h"

ResolutionRatioDialog::ResolutionRatioDialog(QWidget *parent) :
    QDialog(parent),                       //parent pass to base class
    ui(new Ui::ResolutionRatioDialog)
{
    ui->setupUi(this);
    ui->xAxisMinLe->setText("0");
    ui->xAxisMaxLe->setText("3000");
    ui->yAxisMinLe->setText("-30000");
    ui->yAxisMaxLe->setText("30000");
}

ResolutionRatioDialog::~ResolutionRatioDialog()
{
    delete ui;
}

void ResolutionRatioDialog::on_okBtn_clicked()
{
    double xAxis_minValue = ui->xAxisMinLe->text().toDouble();
    double xAxis_maxValue = ui->xAxisMaxLe->text().toDouble();
    double yAxis_minValue = ui->yAxisMinLe->text().toDouble();
    double yAxis_maxValue = ui->yAxisMaxLe->text().toDouble();
    int lineWidth = ui->lineWidth->text().toInt();
    MainWindow *mainwindow = qobject_cast<MainWindow *>(parent()); //?
    mainwindow->setResolutionRatio(xAxis_minValue,xAxis_maxValue,yAxis_minValue,yAxis_maxValue,lineWidth);
    accept();
}

void ResolutionRatioDialog::on_cancleBtn_clicked()
{
    reject();
}

//#2 send data from dialog to mainwindow
//void ResolutionRatioDialog::getResolutionRiato(double xAxis_MinValue, double xAxis_MaxValue, double yAxis_MinValue, double yAxis_MaxValue)
//{
//    xAxis_MinValue = ui->xAxisMinLe->text().toDouble();
//    xAxis_MaxValue = ui->xAxisMaxLe->text().toDouble();
//    yAxis_MinValue = ui->yAxisMinLe->text().toDouble();
//    yAxis_MaxValue = ui->yAxisMaxLe->text().toDouble();
//}


