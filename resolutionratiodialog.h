#ifndef RESOLUTIONRATIODIALOG_H
#define RESOLUTIONRATIODIALOG_H

#include <QDialog>

namespace Ui {
class ResolutionRatioDialog;
}

class ResolutionRatioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResolutionRatioDialog(QWidget *parent = 0);  // 0 null point, no parent object
    ~ResolutionRatioDialog();
    // #2
//    void getResolutionRiato(double xAxis_MinValue, double xAxis_MaxValue, double yAxis_MinValue, double yAxis_MaxValue);


private slots:
    void on_okBtn_clicked();
    void on_cancleBtn_clicked();
    //#2
//    void on_okBtn_clicked(){this->setResult(QDialog::Accepted);this->close();}
//    void on_cancleBtn_clicked(){this->setResult(QDialog::Rejected);this->close();}

private:
    Ui::ResolutionRatioDialog *ui;
};

#endif // RESOLUTIONRATIODIALOG_H
