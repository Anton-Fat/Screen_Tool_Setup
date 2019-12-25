#ifndef KALIBRATION_H
#define KALIBRATION_H

#include <QDialog>

#define   kal_num_param  3
#define   excellent      7

extern int KalPressButton;
extern int verHW;
extern const int verHW_kal;
extern bool KalUiEnable;
extern bool PingPong;
extern int fi;
extern int strike;

extern QString ParamFull;
extern QString ParamEmpty;
extern int  ParamKalVal[kal_num_param+1];


namespace Ui {
class Kalibration;
}

class Kalibration : public QDialog
{
    Q_OBJECT

public:
    explicit Kalibration(QWidget *parent = nullptr);
    ~Kalibration();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Kalibration *ui;
};

#endif // KALIBRATION_H
