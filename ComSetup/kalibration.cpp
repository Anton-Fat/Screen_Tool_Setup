/*********************************%
% Kalibration                     %
%                                 %
% Version 1.0                     %
% Date: 23.12.2019                %
% Autor: Anton Fateyev            %
% Compile: Qt 5.3				  %
% mail: antonfat@gmail.com        %
%*********************************/
#include "kalibration.h"
#include "ui_kalibration.h"
#include "properties_2.h"
#include <QDebug>


int KalPressButton;
int verHW;
const int verHW_kal = 407;
bool KalUiEnable;
bool PingPong;
int fi;
int strike;

QString ParamFull;
QString ParamEmpty;
int  ParamKalVal[kal_num_param+1];


Kalibration::Kalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kalibration)
{
    ui->setupUi(this);

   // ui->label->setText(mes11);

    ui->pushButton->setStyleSheet(BorderButton);
    ui->pushButton_2->setStyleSheet(BorderButton);


}

Kalibration::~Kalibration()
{
    delete ui;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Kalibration::on_pushButton_clicked()
{
    // full
    qDebug() << "Press full";
    NumPressButton = btn_full;
    strike = 1;
    PingPong = true;
    hide();
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Kalibration::on_pushButton_2_clicked()
{
    // empty
    qDebug() << "Press empty";
    NumPressButton = btn_empty;
    strike = 2;
    PingPong = true;
    hide();
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
