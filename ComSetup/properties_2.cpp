#include "properties_2.h"
#include "ui_properties_2.h"
#include <QDebug>

const QString BorderButton = "QPushButton { border: 2px solid gray;"
                             "border-style: outset;"
                             "padding: 3px;"
                             "background-color: #D1D1D1;"
                             "border-radius: 8px;}";

const  QString mes0 = "Предупреждение о возможной ошибке ввода значений:";
const  QString mes1 = "Обратите внимание, что параметры вводятся не в <b>см</b>, а в <b>мм</b>. Вы уверены, что ширина бака ";
const  QString mes2 = "Введены слишком большие параметры высоты/ширины бака. Вы уверены, что ширина бака ";
const  QString mes4 = "Подключите датчик КS2 PROP 1 к компьютеру. Для этого нажмите кнопку ";
const  QString mes5 = "Значение объема слишком ";


const char * st_get_display3a = "both";
const char * st_get_display3 = "litres/percents";
const char * st_get_display1 = "litres";
const char * st_get_display2 = "percents";

const char * st_val_view1r = "литры";
const char * st_val_view2r = "проценты";
const char * st_val_view3r = "попеременно: литры/проценты";

Stringi DataParam;
bool Properties;
//bool Properties;
bool ComConnect;

bool SetupParamDopB[NumDopParam];    // view wall PropaneVal max
int  SetupParamDopVal[NumDopParam];

int NumPressButton;

bool InitForma;

// int PropaneVal;


Properties_2::Properties_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Properties_2)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);

    //setWindowFlags(Qt::FramelessWindowHint);

    setWindowFlags(Qt::CustomizeWindowHint);

    qDebug() << "Start forma";
    InitForma = false;


    ui->pushButton_PR_setup->setStyleSheet(BorderButton);
    ui->pushButton_PR_cansel->setStyleSheet(BorderButton);
    ui->pushButton_PR_setup->setEnabled(false);


    for (int i = 0; i < NumDopParam; i++) {
     SetupParamDopB[i] = false;

    }



    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::black);

    //  ui->comboBox_shape->setPalette(sample_palette);

      ui->comboBox_view->addItem(st_val_view1r);
      ui->comboBox_view->addItem(st_val_view2r);
      ui->comboBox_view->addItem(st_val_view3r);

    //  for (int i = 0 ; i < ui->comboBox_view->count() ; ++i) {
    //  ui->comboBox_view->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    //  }


      ui->comboBox_view->setPalette(sample_palette);

      ui->comboBox_view->setStyleSheet("background-color: rgb(255, 255, 255) ; color: rgb(0, 0, 0);");

      if(!ComConnect) {
          SetupParamDopVal[0] = 2;
          SetupParamDopVal[1] = 3;
          SetupParamDopVal[2] = 70;
          SetupParamDopVal[3] = 8;
      }

      // ad->firstName(); ui->spinBox_Proc->setValue(tmp.toInt());

      ui->comboBox_view->setCurrentIndex(SetupParamDopVal[0]);
        qDebug() << "CurrentIndex Setup";

      ui->spinBox->setValue(SetupParamDopVal[1]);
      ui->horizontalSlider->setValue(SetupParamDopVal[2]);
      horizontalSlider_setText(SetupParamDopVal[2]);
      ui->spinBox_2->setValue(SetupParamDopVal[3]);

      InitForma = true;

}

Properties_2::~Properties_2()
{
    delete ui;
}


// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::on_pushButton_PR_cansel_clicked()
{
  qDebug() << "Press cansel";
  NumPressButton = btn_cansel_dop;
  //qDebug() << SetupParamDopB;
    Properties = false;
    hide();


}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::on_pushButton_PR_setup_clicked()
{
  qDebug() << "Press setup";

  NumPressButton = btn_setup_dop;

    Properties = true;
   // hide();

  //requestData.clear();


  ui->pushButton_PR_setup->setText("Ожидайте");

}

// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::on_horizontalSlider_valueChanged(int value)
{

    SetupParamDopVal[2] = value;

    if(InitForma)
    {
    if(ComConnect)
    {
      ui->pushButton_PR_setup->setEnabled(true);
      SetupParamDopB[2] = true;
      qDebug() << "SetupParamDopB[2] = true";
    }
    }

    horizontalSlider_setText(value);

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::horizontalSlider_setText(int value)
{
    QString val1, val2;
    val1 = QString::number(value);
    val2 = QString::number(100-value);

    ui->label_Propan->setText("пропан "+val1+"%");
    ui->label_Butan->setText(val2+"% бутан");

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::on_spinBox_editingFinished()
{
qDebug() << "Tol change";



}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::on_comboBox_view_currentIndexChanged(int index)
{
    if(InitForma)
    {
    SetupParamDopVal[0] = index;
    if(ComConnect)
    {
      ui->pushButton_PR_setup->setEnabled(true);
      SetupParamDopB[0] = true;
      qDebug() << "SetupParamDopB[0] = true";
    }
    }
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::on_spinBox_valueChanged(int arg1)
{
    if(InitForma)
    {
    SetupParamDopVal[1] = arg1;
    if(ComConnect)
    {
      ui->pushButton_PR_setup->setEnabled(true);
      SetupParamDopB[1] = true;
      qDebug() << "SetupParamDopB[1] = true";
    }
}

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void Properties_2::on_spinBox_2_valueChanged(int arg1)
{
    if(InitForma)
    {
    SetupParamDopVal[3] = arg1;
    if(ComConnect)
    {
      ui->pushButton_PR_setup->setEnabled(true);
      SetupParamDopB[3] = true;
      qDebug() << "SetupParamDopB[3] = true";
    }
}
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB