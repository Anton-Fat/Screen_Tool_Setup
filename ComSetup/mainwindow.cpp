#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>

QSerialPort *serial;

bool ComConnect;
bool Reopen;
bool Input;
int NumPressButton;
int NumSendMessage;
QStringList  set_list;

const char * st_version = "Version 0.2";

const char * st_get_dev = "get_device\r";
const char * st_get_did = "get_did\r";

const char * st_set_shape = "shape";
const char * st_set_height = "height";
// const char * st_set_lenght = "lenght";
const char * st_set_diam = "diameter";
const char * st_set_liq = "liquid";
const char * st_set_mix = "mix";

const char * st_set = "set_";
const char * st_get = "get_";


const char * st_val_shape1 = "Cylindrical";
const char * st_val_shape2 = "Toroidal";

const char * st_val_liquid1 = "water";
const char * st_val_liquid2 = "propane";
const char * st_val_liquid3 = "butane";
const char * st_val_liquid4 = "propane-butane";

const char * st_val_shape1r = "цилиндрическая";
const char * st_val_shape2r = "тороидальная";

const char * st_val_liquid1r = "вода";
const char * st_val_liquid2r = "пропан";
const char * st_val_liquid3r = "бутан";
const char * st_val_liquid4r = "пропан-бутан";


QByteArray requestData;
QByteArray sendData;



enum Liquid
{
    liq_water = 0,
    liq_propane,
    liq_butane,
    liq_prop_but
} liquid;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->geometry().width(),this->geometry().height());

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    ui->comboBox->addItem(info.portName());

    if(ui->comboBox->count() == 0)
    {
    ui->pushButton->setText("Поиск");
    }

    ui->label_status->setText(st_version);

    // setup UI
    if(true)
    {
    ui->comboBox_shape->addItem(st_val_shape1r); //  "cylindrical"
    ui->comboBox_shape->addItem(st_val_shape2r);  // "toroidal"
    // ui->comboBox_shape->setLocale(Qt::TextAlignmentRole);
    for (int i = 0 ; i < ui->comboBox_shape->count() ; ++i) {
    ui->comboBox_shape->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    ui->comboBox_fuel->addItem(st_val_liquid1r);
    ui->comboBox_fuel->addItem(st_val_liquid2r);
    ui->comboBox_fuel->addItem(st_val_liquid3r);
    ui->comboBox_fuel->addItem(st_val_liquid4r);
    for (int i = 0 ; i < ui->comboBox_fuel->count() ; ++i) {
    ui->comboBox_fuel->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::black);


    ui->label_ID->setPalette(sample_palette);
    ui->label_dev->setPalette(sample_palette);
    ui->label_ver->setPalette(sample_palette);
    ui->comboBox_shape->setPalette(sample_palette);
    ui->comboBox_fuel->setPalette(sample_palette);

    ui->comboBox_shape->setStyleSheet("background-color: rgb(255, 255, 255) ; color: rgb(0, 0, 0);");
    ui->comboBox_fuel->setStyleSheet("background-color: rgb(255, 255, 255) ; color: rgb(0, 0, 0);");
    }
    ui->statusBar->addWidget(ui->label_status);

    serial = new QSerialPort(this);


    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // setup comand
    if(true){
        set_list.append(st_get_did);

        set_list.append(st_set);
        set_list.last().append(st_set_shape);
        set_list.append(st_set);
        set_list.last().append(st_set_height);
        set_list.append(st_set);
        set_list.last().append(st_set_diam);
        set_list.append(st_set);
        set_list.last().append(st_set_liq);
        set_list.append(st_set);
        set_list.last().append(st_set_mix);

        set_list.append(st_get);
        set_list.last().append(st_set_shape);
        set_list.append(st_get);
        set_list.last().append(st_set_height);
        set_list.append(st_get);
        set_list.last().append(st_set_diam);
        set_list.append(st_get);
        set_list.last().append(st_set_liq);
        set_list.append(st_get);
        set_list.last().append(st_set_mix);

        // setup get value
        if(!testenable){
           ui->groupBox->setHidden(true);
        }
    }


    connect(serial,SIGNAL(readyRead()),this, SLOT(serialReceived()));

    connect(&SchwartzPower, &SendlerClass::push, this, &MainWindow::realPush);

    ComConnect = false;
    Reopen = true;

    ui->pushButton->setFocus();

    if (!SchwartzPower.isRunning())
    {
     SchwartzPower.start();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    if(serial->isOpen())
    {
    serial->close();
    }

}
void MainWindow::realPush(const QString &s1, int res)
{
    QString comp_str;
    QString str = s1;

    if(res == -1)
    {
    ui->pushButton_2->setText("Применить");
    } else {
        if (res != res_none)
        {

            reOpenPort();

            switch (res) {
            case res_shape:
            str.append(' ');
            comp_str = st_val_shape1r;
            if (ui->comboBox_shape->currentText() == comp_str)
            {
            str.append(st_val_shape1);
            }
            comp_str = st_val_shape2r;
            if (ui->comboBox_shape->currentText() == comp_str)
            {
            str.append(st_val_shape2);
            }

            break;
            case res_height:

            str.append(' ');
            str.append(ui->spinBox_H->text());
            break;
            case res_lenght:
            str.append(' ');
            str.append(ui->spinBox_D->text());
            break;
            case res_liquid:
                // replase
                comp_str = st_val_liquid1r;
                if (ui->comboBox_fuel->currentText() == comp_str)
                {
                liquid = liq_water;
                }
                comp_str = st_val_liquid2r;
                if (ui->comboBox_fuel->currentText() == comp_str)
                {
                liquid = liq_propane;
                }
                comp_str = st_val_liquid3r;
                if (ui->comboBox_fuel->currentText() == comp_str)
                {
                liquid = liq_butane;
                }
                comp_str = st_val_liquid4r;
                if (ui->comboBox_fuel->currentText() == comp_str)
                {
                liquid = liq_prop_but;
                }
            str.append(' ');
            str.append(QString::number(liquid));

            break;
            case res_mix:
            str.append(' ');
            str.append(ui->spinBox_Proc->text());
            // str.append('%');
            break;

            } // switch

            str.append('\r');
        }

        qDebug() << "Push <<<";
        qDebug() << str;

        sendData = str.toLocal8Bit();

        if(!serial->isOpen())
        {
        qDebug() << "Serial is not open !";
        serial->open(QIODevice::ReadWrite);
        }

        serial->write(sendData);
    } // res

}

void MainWindow::serialReceived()
{

   QByteArray requestData2;
   int sizeArr;
   Input = true;

    requestData2 = serial->readAll();

    sizeArr = requestData2.size();
    // Remove "\r\n" simbol
    if(sizeArr>2)
    {
        sizeArr-=1;
        if (requestData2.at(sizeArr) == '\n')
        {
            requestData2.remove(sizeArr,1);
             sizeArr-=1;
        }
        if (requestData2.at(sizeArr) == '\r')
        {
            requestData2.chop(1);
            sizeArr-=1;
        }
    }

    switch (NumPressButton) {
    case btn_setup:


    break;
    case btn_cansel:

        qDebug()<<"btn cansel";

    break;

    }

    if( NumPressButton == btn_connect)
    {

        int Num = 4;
        QString str;
        QString tmp = requestData2;

        if(NumSendMessage == 1)
        {
            sizeArr = requestData2.size();
            if(requestData2.size() > 24)
            {
              requestData2.remove(0,sizeArr-24);
            }
            ui->label_ID->setText(requestData2);
            serial->write(st_get_dev);
        } else if (NumSendMessage == 2) {
            QStringList list;
            QString dev, ver;
            list = tmp.split(' ');

            if(list.size() > 1)
            {
                dev = list.at(0);
                list.removeFirst();
                ver = list.join(' ');
             ui->label_dev->setText(dev);
             ui->label_ver->setText(ver);

            }
            } else if (NumSendMessage == 3) {
            QString comp_str;
            comp_str = st_val_shape1;
            if (tmp == comp_str)
            {
            ui->comboBox_shape->setCurrentIndex(0);
            }
            comp_str = st_val_shape2;
            if (tmp == comp_str)
            {
            ui->comboBox_shape->setCurrentIndex(1);
            }
            } else if (NumSendMessage == 4) {
            ui->spinBox_H->setValue(tmp.toInt());
            } else if (NumSendMessage == 5) {
            ui->spinBox_D->setValue(tmp.toInt());
            } else if (NumSendMessage == 6) {
            if((tmp.toInt() >= 0) && (tmp.toInt() < ui->comboBox_fuel->count()))
            {
             ui->comboBox_fuel->setCurrentIndex(tmp.toInt());
            }
            } else if (NumSendMessage == 7) {
            ui->spinBox_Proc->setValue(tmp.toInt());
            NumSendMessage = 0;
            }
        if ((NumSendMessage >= 2) && (NumSendMessage <= 6))
        {
            // -------------------------
            Num += NumSendMessage;
            str = set_list.at(Num);
            str.append('\r');
            sendData = str.toLocal8Bit();
            serial->write(sendData);
            NumSendMessage +=1;
        } else if(NumSendMessage == 1)
        {
            NumSendMessage +=1;
        }
    }

    requestData += requestData2;
    qDebug()<<"Receive data !";
    qDebug()<<requestData2;

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_pushButton_clicked()
{
    NumPressButton = btn_connect;

if(ui->comboBox->count() == 0)
{
qDebug() << "New find";
const auto infos = QSerialPortInfo::availablePorts();
for (const QSerialPortInfo &info : infos)
{
ui->comboBox->addItem(info.portName());
}
if(ui->comboBox->count() != 0)
ui->pushButton->setText("Подключение");

} else {

       if(!ComConnect)
        {
           serial->setPortName(ui->comboBox->currentText());
           Input = false;

           if (serial->open(QIODevice::ReadWrite))
           {
             ComConnect = true;
             qDebug()<<"Port connected";
             ui->label_status->setText("link open");
             QThread::msleep(200);
             NumSendMessage = 1;

             serial->write(st_get_did);
           }

        ui->pushButton->setText("Отключение");
        } else {
        if(serial->isOpen())
        {
        ComConnect = false;
        serial->close();
        qDebug()<<"Port close";
        ui->label_status->setText("link close");
        }
        ui->pushButton->setText("Подключение");
        }
       } // count com

}
void MainWindow::reOpenPort()
{
    if (Reopen)
    {
    if(serial->isOpen())
    {
    serial->close();
    if (serial->open(QIODevice::ReadWrite))
    {
     qDebug() << "New Open";
    }
    }
   }
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_pushButton_2_clicked()
{
    NumPressButton = btn_setup;
    qDebug() << "Press setup";

    if(ComConnect)
     {

        requestData.clear();

        SchwartzPower.m_loop = true;
        ui->pushButton_2->setText("Ожидайте");

     }

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_pushButton_3_clicked()
{
    NumPressButton = btn_cansel;
    qDebug() << "Press cansel";

    if(ComConnect)
    {
        if (SchwartzPower.isRunning())
        {

            SchwartzPower.m_loop = false;          
            ui->pushButton_2->setText("Применить");

        }
    } else {

      ui->comboBox->clear();
      ui->pushButton->setText("Поиск");
}

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_pushButton_test_clicked()
{
    QString str;
    int Num;

    qDebug() << "Press test";
    if(ComConnect)
    {

   // reOpenPort();

    Num = ui->spinBox_test->value();
    Num += 5;

    str = set_list.at(Num);
    str.append('\r');

    qDebug() << str;
    sendData = str.toLocal8Bit();

    serial->write(sendData);

    }
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB


