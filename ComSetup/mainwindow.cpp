// #include <Windows.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "properties_2.h"

// #include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>

//#include <QMoveEvent>

#include <QPainter>
#include <QtWidgets>

//#include <windows.h>  // scale
#include <QtGlobal>

QSerialPort *serial;

bool Forma, Hand, HandOK;
bool CorrectValH, CorrectValD, CorrectValDI, CorrectValV;
int NumPressRadio1;
bool Reopen;
bool Input;

int NumSendMessage;
QStringList  set_list;
QStringList  get_list;
QStringList  Display;

const char * st_version = "0.91 Beta";
const char * st_manual = "<a href=\http://ks2tools.ru/prop\">ссылке</a>";
const char * st_help = "help@ks2corp.com";
const char * file_param = "Param.txt";

const char * st_btn_search = "Поиск";
const char * st_btn_connect = "Подключить";
const char * st_btn_disconnect = "Отключить";
const char * st_btn_no_connect = "Нет подключения";

const char * st_incorrect = "Несовместимые параметры";


const char * split_str = "\r\n";

const char * st_get_dev = "device";
const char * st_get_did = "did";
const char * st_get_tank = "tank";

const char * st_set_display = "display";   // 1
const char * st_set_wall = "wall";         // 2
const char * st_set_liq = "liquid";
const char * st_set_mix = "mix";           // 3
const char * st_set_deadvol = "deadvol";  // 4
const char * st_set_custom = "custom";

const char * st_set = "set ";
const char * st_get = "get ";


const    QString metkaBublik = "Toroid";
const    QString metkaVatruchka = "Cyl.P";
const    QString metkaBochkaH = "Cyl.H";
const    QString metkaBochkaV = "Cyl.V";
const    QString metkaCustom = "99";
const    QString metkaNone = "None";
const    QString metkaOK = "ok";

const    QString CMDdriverquery = "driverquery";
const    QString CMDfindstr = "findstr \"VID_0483&PID_5740\" *.inf";
const char * st_set_Bublik = "T";
const char * st_set_Vatruchka = "P";
const char * st_set_BochkaH = "H";
const char * st_set_BochkaV = "V";


const char * st_val_invcmd   = "INVCMD";
const quint16 vendor_id = 1155;
const quint16 product_id = 22336;

const quint16 Size_B_min = 100;
const quint16 Size_B_max = 1000;
const quint16 Size_V_min = 70;
const quint16 Size_V_max = 110;

const float DEFAULT_DPI = 100.0;

QByteArray requestData;
QByteArray sendData;

quint8 NumParam;

int Hmain;
int nLastEvent;


// Dynamic widget
QStringList  DW_radio_text;
bool CylPositionV;
QString ReadCMD, WorkPathCMD, CommandCMD;


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

    ui->pb_Add_Parametr->installEventFilter(this);

    Properties = false;

    // ---------------------------------------------------------------
    // Setup images
    QString MesLabel;

        ui->tabWidget->setStyleSheet("padding: 2px 2px 2px 2px");   /* top - right - bottom - left */

        ui->groupBox_2->setStyleSheet("border: 0px;");

        QString green= "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0.2,stop: 0 #11AF50,stop: 0.4 #119F80,stop: 0.7 #319F80,stop: 1 #11AF50 );"
                      "border-radius: 8px;border: 1px solid black;}";
        ui->progressBar->setStyleSheet(green);
        ui->progressBar->setMaximum(0); ui->progressBar->setMinimum(0);

        ui->progressBar->hide();

    // -----------------------------------------------
        ui->pushButton->setStyleSheet(BorderButton);
        ui->pushButton_2->setStyleSheet(BorderButton);
        ui->pushButton_3->setStyleSheet(BorderButton);

    // ---------------------------------------------------------------

    QPixmap pixmap_imL(":/Images/Logo.png");
    QSize scaleSize_logo = ui->label_Logo->size();
    // QSize size(100, 10);
    // qDebug() << scaleSize_logo;
    QPixmap scalePixmap = pixmap_imL.scaled(scaleSize_logo, Qt::KeepAspectRatio);
    QPixmap pixmap_im1(":/Images/tor-narujniy_W130.png");
    QPixmap pixmap_im2(":/Images/tor-polnoteliy_W130.png");
    QPixmap pixmap_im3(":/Images/cilindry_W130.png");

    ui->label_Logo->setPixmap(scalePixmap);
    ui->label_image_1->setPixmap(pixmap_im1);
    ui->label_image_2->setPixmap(pixmap_im2);
    ui->label_image_3->setPixmap(pixmap_im3);

    // search valid port --------------------------------

    NumParam = 6;

    // -------------------------------------

    if (LoadFromFile())
    {
        QStringList labelC;
        DataParam = ParamAnalise(DW_radio_text);
    } // load

    NumPressRadio1 = 0;
  //  DataParam.CurFormaSelect = 1;
    ui->radioButton_F1->click();

    CylPositionV = false;

    RefreshPorts();

    ui->label_prog_1->setText(st_version);
    ui->label_prog_2->setText("KS2");
    ui->label_prog_3->setText(st_help);
    MesLabel = "Если есть вопросы, ознакомьтесь с руководством пользователя по ";
 //   MesLabel.append(split_str);
    MesLabel.append(st_manual);
    MesLabel.append(" или напишите на почту ");
    MesLabel.append(st_help);
    MesLabel.append(".");
    ui->label_prog_4->setText(MesLabel);


    ui->label_prog_4->setTextFormat(Qt::RichText);
    ui->label_prog_4->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_prog_4->setOpenExternalLinks(true);

    // setup UI
    if(true)
    {

    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::black);


    ui->label_ID->setPalette(sample_palette);
    ui->label_dev->setPalette(sample_palette);
    ui->label_ver->setPalette(sample_palette);

    ui->label_prog_1->setPalette(sample_palette);
    ui->label_prog_2->setPalette(sample_palette);
    ui->label_prog_3->setPalette(sample_palette);

    }
    ui->statusBar->addWidget(ui->label_status);

    serial = new QSerialPort(this);

    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    SerialKostil.TimeOut = 100;

    // setup comand
    if(true){
        Display.append(st_get_display1);
        Display.append(st_get_display2);
        Display.append(st_get_display3a);

        set_list.append(st_get_did);   // NumParam

        set_list.append(st_set);
        set_list.last().append(st_set_display);
        set_list.append(st_set);
        set_list.last().append(st_set_wall);
        set_list.append(st_set);
        set_list.last().append(st_set_mix);
        set_list.append(st_set);
        set_list.last().append(st_set_deadvol);
        set_list.append(st_set);
        set_list.last().append(st_set_mix);
        set_list.append(st_set);
        set_list.last().append(st_set_custom);
        set_list.append(st_set);
        set_list.last().append(st_get_tank);

        get_list.append(st_get);
        get_list.last().append(st_get_did);
        get_list.append(st_get);
        get_list.last().append(st_get_dev);
        get_list.append(st_get);
        get_list.last().append(st_get_tank);

        get_list.append(st_get);        
        get_list.last().append(st_set_display);
        get_list.append(st_get);
        get_list.last().append(st_set_wall);
        get_list.append(st_get);
        get_list.last().append(st_set_liq);     // mix
        get_list.append(st_get);
        get_list.last().append(st_set_deadvol);

        // setup get value
        if(!testenable){
           ui->groupBox->setHidden(true);
          // ui->groupBox->hide();
          ui->text_Test->setText("get did");

        }
    }

    // ---------------------------------------------------
    //

    connect(serial,SIGNAL(readyRead()),this, SLOT(serialReceived()));
    connect(&SchwartzPower, &SendlerClass::push, this, &MainWindow::realPush);
    connect(&SerialKostil, &SerialTimer::pop, this, &MainWindow::realPop);
    connect(&SerialKostil, &SerialTimer::status, this, &MainWindow::realStatus);
    connect(this, &MainWindow::ComSendData, this, &MainWindow::writeData);

    ComConnect = false;
    Reopen = true;

    AllValCorrect();

    Forma = false;
 //   ui->widget->hide();
    ui->frame_radio->hide();
    ui->frame_2->hide();
    Hand = false;
    // ui->pb_Add_Parametr->hide();

    ui->pushButton->setFocus();

    if (!SchwartzPower.isRunning())
    {
     SchwartzPower.start();
    }
    if (!SerialKostil.isRunning())
    {
     SerialKostil.start();
    }

   // ui->label_status->setText(st_btn_no_connect);

  //  UpdateSizeWin();




}

MainWindow::~MainWindow()
{
    delete ui;
    if(serial->isOpen())
    {
    serial->close();
    }

}

void MainWindow::moveEvent(QMoveEvent *event)
{

    // const QPoint & QMoveEvent::oldPos() const
    QMoveEvent *moveEvent = static_cast<QMoveEvent*>(event);

    QPoint pos = moveEvent->oldPos();

qDebug()<< "pos" << pos;

    UpdateSizeWin();

    qDebug()<<"Move detected";

    event->accept();
}
// FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
void MainWindow::StartWaitBar()
{
    ui->progressBar->show();
}
// FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
void MainWindow::StopWaitBar()
{
    ui->progressBar->hide();
}

// FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
bool MainWindow::UpdateSizeWin()
{

    // Size scale SSSSS
    int Hcalc;


    HDC screen = GetDC( 0 );
    qDebug() << "HDC screen = " << screen;


    QRect r = QApplication::desktop()->screenGeometry();
    qDebug() << "r = " << r;

    adjustSize();
    resize(width(),height()+1);  // Magic!

    Hmain = height();
    Hcalc = r.size().height() * PERCENT_OF_SCREEN_H;
    if (Hcalc < Hmain)
    {
    resize(width(),Hcalc);
    }


    qDebug() << "W = " << width() << ", H = " << height();
    qDebug() << "Hcalc = " << r.size().height() * PERCENT_OF_SCREEN_H;

    return true;
}
void MainWindow::realPush(const QString &s1, int res)   // >>>
{
    QString comp_str;
    QString str = s1;

    QString comboBox_f;
     if(res < 0)
     {
        if(res == -1)
        {
            ui->pushButton_2->setText("Применить");
            if(HandOK == true)
            {
             MBok(); //

            } else {
                ui->label_status->setText(st_incorrect);
                MBtextErr(st_incorrect, mes01);
            }
        }
    if(res == -2)
    {

    MBok(); //

    ad->hide();
  //  delete ad;

    }
    } else {
        if (res != res_none)
        {

            reOpenPort();

            switch (res) {
            case res_display:
            str.append(' ');
            str.append(Display.at(SetupParamDopVal[0]));

            break;
            case res_wall:

            str.append(' ');
            str.append(QString::number(SetupParamDopVal[1]));

            break;
            case res_mix:
            str.append(' ');
            str.append(QString::number(SetupParamDopVal[2]));

            break;
            case res_deadvol:
                str.append(' ');
                str.append(QString::number(SetupParamDopVal[3]));

            break;
            case res_custom:
                str.append(' ');
                str.append(DataParam.Custom);

            break;
            case res_tank:
                str.append(' ');
                str.append(metkaCustom);

            break;

                // NumParam
            } // switch

            str.append('\r');
        }

        sendData = str.toLocal8Bit();

        if(!serial->isOpen())
        {
        qDebug() << "Serial is not open !";
        serial->open(QIODevice::ReadWrite);
        }

        if((HandOK == true)||(Hand == false)||(NumPressButton == btn_setup_dop))
        {
        qDebug() << "Send --------------R";
        qDebug() << sendData;

        writeData(sendData);
        }
    } // res

}
// CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
void MainWindow::writeData(const QByteArray &data)
{
    SerialKostil.Count_time = SerialKostil.TimeOut;
    SerialKostil.on_timer = true;
    SerialKostil.Count = 0;
    SerialKostil.DataInput.clear();
    serial->write(data);
}
void MainWindow::realStatus(int res)
{
    bool delete_port = true;

    if(res == 1)
    {
  //  qDebug()<<"Check Port ";

    if(serial->error() == QSerialPort::NoError)
    {
    delete_port = false;

    } else {
    qDebug() << serial->error();
    }

    if(delete_port)
    {
    qDebug() << "Serial is not open !";
    ui->pushButton->click();
    MBtextErr("Датчик отключен. Проверьте подключение датчика к компьютеру.", "Ошибка");
    RefreshPorts();
    }

    } // res 1
    if(res == 2)
    {
     RefreshPorts();
    }  // res 2

}
void MainWindow::realPop(const QString &s1, int res)   // <<<
{

    int sizeArr;
    QByteArray requestData2;
    QString outputData;
    QStringList list2;
    QString Input = s1;
    QString Setup;

    qDebug()<<"Input All --- Count = "+QString::number(res);

    removeEnd(&Input);
    qDebug()<<Input;

    removeEcho(&Input);

    switch (NumPressButton) {
    case btn_setup:

        if(Input == metkaOK)
        {
            if(!Hand) {
            MBok(); //
            }

        } else {
            if(Hand)
            {
            HandOK = false;
            qDebug() << "HandOK = false";
            }
}

    break;
    case btn_cansel:

        qDebug()<<"btn cansel";

    break;

    }

    if( NumPressButton == btn_connect)
    {

        int Num;
        QString str;
      //  QString tmp = requestData2;

        QString comp_str2;
        comp_str2 = st_val_invcmd;
        if (Input == comp_str2)
        {
        ui->label_status->setText("Неверная команда");
        }

        if(NumSendMessage == 1)
        {

            list2 = Input.split(split_str); // "\r\n"
            qDebug() << "List 2";
            qDebug() << list2;

            ui->label_ID->setText(list2.at(0));
            // get infooo (2)

             outputData = get_list.at(1);
             outputData.append('\r');
            qDebug() << "Send --------------1";
            writeData(outputData.toLocal8Bit());
        } else if (NumSendMessage == 2) {

          //  QStringList list;
            QString dev, ver;


            list2 = Input.split(' ');

            if(list2.size() > 1)
            {
                dev = list2.at(0);
                list2.removeFirst();
                ver = list2.join(' ');
             ui->label_dev->setText(dev);
             ui->label_ver->setText(ver);
            }
            } else if (NumSendMessage == 3) {
            // tank analize


            list2 = Input.split(split_str);

            DataParam = ParamAnalise(list2);

         //   qDebug() << "Custom "+DataParam.Custom;

            Setup = FindChislo(DataParam.Custom, 0);
            if(Setup != metkaNone)
            ui->spinBox_D->setValue(Setup.toInt());
            Setup = FindChislo(DataParam.Custom, 1);
            if(Setup != metkaNone)
            ui->spinBox_H->setValue(Setup.toInt());
            Setup = FindChislo(DataParam.Custom, 2);
            if(Setup != metkaNone)
            ui->spinBox_V->setValue(Setup.toInt());

            NumPressRadio1 = 0;
            if(DataParam.CurForma == 1)
            {
                qDebug() << "CurForma = 1";
            ui->radioButton_F1->click();
            //on_radioButton_F1_func();
            }
            if(DataParam.CurForma == 2)
            {
                qDebug() << "CurForma = 2";
            ui->radioButton_F2->click();
            //on_radioButton_F2_func();
            }
            if(DataParam.CurForma == 3)
            {
                qDebug() << "CurForma = 3";
            ui->radioButton_F3->click();
            ui->radioButton_V->click();

            }
            if(DataParam.CurForma == 4)
            {
                qDebug() << "CurForma = 4";
            ui->radioButton_F3->click();
            ui->radioButton_H->click();

            }

            } else if (NumSendMessage == 4) {
            // +++++++++++++++++++++++++++++++++++++++++++++++++++
            // +++++++++++++++++++++++++++++++++++++++++++++++++++

            // display   litres/percents

            if(Input == st_get_display1)
            SetupParamDopVal[0] = 0;
            if(Input == st_get_display2)
            SetupParamDopVal[0] = 1;
            if(Input == st_get_display3)
            SetupParamDopVal[0] = 2;
            qDebug() << SetupParamDopVal[0];

           // ui->spinBox_H->setValue(Input.toInt());
            } else if (NumSendMessage == 5) {
            // wall


            Input = FindChislo(Input, 0);
            qDebug() << "wall";
            qDebug() << Input;

            SetupParamDopVal[1] = Input.toInt();

          //  ui->spinBox_D->setValue(Input.toInt());

            } else if (NumSendMessage == 6) {
            // liquid

            Input = FindChislo(Input, 0);
            qDebug() << "liquid";
            qDebug() << Input;

            SetupParamDopVal[2] = Input.toInt();
            } else if (NumSendMessage == 7) {
        // deadvol

            Input = FindChislo(Input, 0);
            qDebug() << Input;

            SetupParamDopVal[3] = Input.toInt();

            NumSendMessage = 0;

            } else if (NumSendMessage == 8) {
            NumSendMessage = 0;
            }

        // NumParam

        if ((NumSendMessage >= 2) && (NumSendMessage <= NumParam ))
        {
            // -------------------------
            Num = NumSendMessage;
            str = get_list.at(Num);
        //    hjghkhj
            str.append('\r');
            sendData = str.toLocal8Bit();
            qDebug() << "Send --------------9";
            writeData(sendData);
            NumSendMessage +=1;
           // NumSendMessage = NumSendMessage;
        } else if(NumSendMessage == 1)
        {
            NumSendMessage +=1;
        }
    }

    qDebug()<<"Receive data !";

}
void MainWindow::serialReceived()
{

   Input = true;
   SerialKostil.Input_K = true;
   SerialKostil.Count++;
   SerialKostil.DataInput += serial->readAll();;

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_pushButton_clicked()
{
    NumPressButton = btn_connect;
    QString  outputData;
    bool Output;

if(ui->comboBox->count() == 0)
{

qDebug() << "New find";

// search valid port --------------------------------

 if(RefreshPorts() == false)
 {
qDebug() << "Message Diagnostiks";


    Output = MBtextSimple(" ", mes7, "Диагностика", "Отмена");
    if(Output)
    {
    if(RefreshPorts() == false)
    {
    qDebug() << "Diagnostiks is run";
    DiagnosticsDriver();
    }

    }

 }


} else {

       if(!ComConnect)
        {
           serial->setPortName(ui->comboBox->currentText());
           Input = false;

           if (serial->open(QIODevice::ReadWrite))
           {
             ComConnect = true;
             qDebug()<<"Port connected";   // ---------------------

             ui->label_status->setText("Датчик подключен");
             QThread::msleep(200);
             NumSendMessage = 1;
             // get infooo (1)

            // Num += NumParam;

             outputData = get_list.at(0);
             outputData.append('\r');

             writeData(outputData.toLocal8Bit());

             ui->pushButton->setText(st_btn_disconnect);
           }


        } else {
        if(serial->isOpen())
        {
        ComConnect = false;
        serial->close();
        qDebug()<<"Port close";
        ui->label_status->setText(st_btn_no_connect);
        ui->pushButton->setText(st_btn_connect);
        }

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
bool MainWindow::DiagnosticsDriver()
{
  bool output, res;
  output = true;
  QStringList  Drive;
  QString TmpFileName;
  QString Path, Path2;
  QString temp;

  QProcess gitProcess;
  QDir binDir(Path);

  StartWaitBar();

  Path = binDir.absolutePath();

  Path = binDir.rootPath();

  QStringList env_list(QProcess::systemEnvironment());

  int idx = env_list.indexOf(QRegExp("^WINDIR=.*", Qt::CaseInsensitive));
  if (idx > -1)
  {
      QStringList windir = env_list[idx].split('=');
      qDebug() << "Var : " << windir[0];
      qDebug() << "Path: " << windir[1];   // <<<<<
      Path2 = windir[1];
  }

  Path2.append("\\");
  Path2.append("inf");

   qDebug() << "1333 " << Path;
   qDebug() << "Path2 = " << Path2;

    temp = '\\';


  foreach( QFileInfo drive, QDir::drives() )
  {
  Drive.append(drive.absolutePath());
  }

  qDebug() << "Drive: " <<Drive.at(0);

  WorkPathCMD = Path2;
  CommandCMD = CMDfindstr;
  SuperCmdRun(CommandCMD, 1);

return output;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::SuperCmdRun(QString command, int exeCode)
{
    ReadCMD.clear();
    p.setWorkingDirectory(WorkPathCMD);
    p.connect(&p, &QProcess::readyReadStandardOutput,
                  this, &MainWindow::on_process_readyReadStandardOutput);
    p.connect(&p, (void (QProcess::*)(int,QProcess::ExitStatus))&QProcess::finished,
                  this, &MainWindow::on_process_finished);

    p.start(command);

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_process_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString perr = p.readAllStandardError();
    if (perr.length())
        qDebug() << "Error during download.\n" << perr;
    else
    {

        qDebug() << "Download completed!";
        // Analize
        StopWaitBar();
        if(CMDAnalise(CommandCMD))
        {
        qDebug() << "Driver Found !!!";
        MBtextErr("Ошибки не обнаружены, подключите датчик KS2 PROP1 к компьютеру.", mes02);

        } else {
        qDebug() << "Driver Not Found.";
        MBtextErr("Драйвер Virtual COM Port не найден, попробуйте переустановить программу.", mes02);
        }
    }

}
bool MainWindow::CMDAnalise(QString command)
{
    QString metka1, metka2;
    int metkaPos1, metkaPos2;

    if (command == CMDdriverquery)
    {

    metka1 = "STM Virtual COM Port";
    metka2 = "usbser";
    if (ReadCMD.contains(metka1, Qt::CaseInsensitive))
    {
        metkaPos1 = ReadCMD.indexOf(metka1);
         qDebug() << "metkaPos = " << metkaPos1;
        if (ReadCMD.contains(metka2, Qt::CaseInsensitive))
        {
             metkaPos2 = ReadCMD.indexOf(metka2);
                if(metkaPos2 - metkaPos1 < 50)
                {
                return true;
                }
        }
    }
    } // command
    // -----------------------------
    if (command == CMDfindstr)
    {
    metka1 = "VID_0483&PID_5740";
    if (ReadCMD.contains(metka1, Qt::CaseInsensitive))
    {
        qDebug() << "find VID_0483&PID_5740 ";
    return true;
    }
    } // command
return false;
}
void MainWindow::on_process_readyReadStandardOutput()
{
    QString Output;
    Output.clear();
    p.setReadChannel(QProcess::StandardOutput);
    QTextStream stream(&p);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        // extract progress info from line and etc.
        ReadCMD.append(line);
    }
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_pushButton_2_clicked()
{

    QString Hval = ui->spinBox_H->text();
    QString Dval = ui->spinBox_D->text();
    QString mes;

    QString outputData;

    NumPressButton = btn_setup;
    qDebug() << "Press setup";

    if(ComConnect)
     {

        requestData.clear();

        if(Hand == false)
        {
        //

            if(ui->comboBox_Param->currentIndex() != -1)
            {
            outputData = "";
            qDebug() <<  DataParam.CurFormaSelect;

            outputData = getSetupTank();

                  outputData.append('\r');
                  outputData.prepend(' ');
                  outputData.prepend(st_get_tank);
                  outputData.prepend(st_set);

                  qDebug() <<  "Hand false ------------------------";
                  qDebug() <<  outputData;

                  reOpenPort();
                  writeData(outputData.toLocal8Bit());

                  }  // cur index

        } else {  // Hand == true


            if(AllValCheck())
            {
              qDebug() << "Correct";

              outputData = "";

              QString TPHV = "F";
              TPHV.append(st_set_Bublik);
              TPHV.append(st_set_Vatruchka);
              TPHV.append(st_set_BochkaH);
              TPHV.append(st_set_BochkaV);

              outputData.append(TPHV.at(DataParam.CurFormaSelect));
              outputData.append(' ');

              outputData.append(ui->spinBox_D->text());
              outputData.append(' ');
              outputData.append(ui->spinBox_H->text());
              outputData.append(' ');
              outputData.append(ui->spinBox_V->text());
              if(DataParam.CurFormaSelect == 1){
              outputData.append(' ');
              outputData.append(ui->spinBox_DI->text());
              }

              qDebug() <<  "Hand true ------------------------";
              qDebug() <<  outputData;

              DataParam.Custom = outputData;

           //   reOpenPort();
           //   writeData(outputData.toLocal8Bit());

                 HandOK = true;
                 SchwartzPower.m_loop = true;
                 ui->pushButton_2->setText("Ожидайте");

            } // end correct

        }

     } else {

        if(ui->pushButton->text() == st_btn_search)
        {

            QMessageBox msgBox;
            msgBox.setWindowTitle(" ");
            msgBox.setText(mes8+ui->pushButton->text()+".");
            msgBox.exec();

        }
        if(ui->pushButton->text() == st_btn_connect)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(" ");
            msgBox.setText(mes9+ui->pushButton->text()+".");
            msgBox.exec();

        }
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
      ui->pushButton->setText(st_btn_search);
}

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
// TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
// TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
void MainWindow::on_pushButton_test_clicked()
{
    QString str;
    int Num;

    qDebug() << "Press test";
    if(ComConnect)
    {
    Num = ui->spinBox_test->value();
    if(Num == 0)
    {
     str = ui->text_Test->toPlainText();
    } else {
    Num += NumParam;

    str = set_list.at(Num);
    }
    if (ui->checkBox_Test->checkState() == Qt::Checked)  // Qt::Unchecked
    {
    str.append('\r');
    }
     // get infooo (T)
    sendData = str.toLocal8Bit();
    qDebug() << sendData;
    qDebug() << "Send --------------T";
    writeData(sendData);

    }
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_pb_Add_Parametr_clicked()
{

    ad = new Properties_2(); // this
    ad->show();
    // position

    int x, y, offset_x, offset_y;

    x = this->geometry().x();
    y = this->geometry().y();

    offset_x = 25;
    offset_y = 200;

    // QApplication::desktop()->screenGeometry();

    ad->move(x+offset_x, y+offset_y);

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{

if (obj == ui->pb_Add_Parametr)
{

    if (evt->type() == QEvent::Paint)
      {

       //   return true;
      }
}
if (nLastEvent == QEvent::Move && evt->type() == 173)
   {

    qDebug()<<"Move detected Filter";
   }
  return false;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::FormaShow(bool *Forma)
{
    if(*Forma == false)
    {
    *Forma = true;

    qDebug()<<"Add forma";
    }  // Forma

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::CylPositionShow()
{
    if(CylPositionV)
    {
    ui->comboBox_Param->clear();
    for (int i = 0; i < DataParam.Num_BochkaV; i++) {
    ui->comboBox_Param->addItem(DataParam.Str_BochkaV.at(i));
  //  ui->comboBox_Param->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    } // for
    if(DataParam.CurForma == 3)
    {
    ui->comboBox_Param->setCurrentIndex(DataParam.CurParam);
    }
    } else {
        ui->comboBox_Param->clear();
        for (int i = 0; i < DataParam.Num_BochkaH; i++) {
        ui->comboBox_Param->addItem(DataParam.Str_BochkaH.at(i));
      //  ui->comboBox_Param->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
        } // for
        if(DataParam.CurForma == 4)
        {
        ui->comboBox_Param->setCurrentIndex(DataParam.CurParam);
        }
    } // CylPosition

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::HandShow(bool *Hand)
{
    if(*Hand == true)
    {
    *Hand = false;
    ui->frame_2->hide();

    qDebug()<<"down hand";
    }
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::MBtext(QString mes)
{

    QString Hval = ui->spinBox_H->text();
    QString Dval = ui->spinBox_D->text();

    QString mes2 = "Вы ввели нестандартные параметры бака: ширина "+Dval+" мм, высота "+Hval+" мм";

    return MBtextS(mes, mes2);  // mes5

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::MBtextS(QString mes1, QString mes2)
{
bool Output;

    Output = MBtextSimple(mes0, mes1, "Да, параметры введены верно", "Исправить");

    if(Output)
    {
        // Yes button was pressed
        QMessageBox msgBox;
        msgBox.setWindowTitle(" ");
        msgBox.setText(mes2);
        msgBox.exec();
    }

   return Output;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::MBtextSimple(QString mes0, QString mes1, QString mesYes, QString mesNo)
{
     QMessageBox* pmbx =
               new QMessageBox(mes0,
                               mes1, // "<i>Simple</i> <u>Message</u>",
                               QMessageBox::Warning,
                               QMessageBox::Yes,
                               QMessageBox::No,
                               QMessageBox::NoButton //QMessageBox::Cancel | QMessageBox::Escape
                              );
             pmbx->setButtonText(QMessageBox::Yes,mesYes);
             pmbx->setButtonText(QMessageBox::No,mesNo);
           int n = pmbx->exec();
           delete pmbx;
           if (n == QMessageBox::Yes) {
           return true;
           }
           if (n == QMessageBox::No) {
            return false;
           }
   return true;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::MBtextErr(QString mes1, QString mes2)
{

     QMessageBox* pmbx =
               new QMessageBox(mes2,
                               mes1, // "<i>Simple</i> <u>Message</u>",
                               QMessageBox::Warning,
                               QMessageBox::NoButton,
                               QMessageBox::No,
                               QMessageBox::NoButton
                              );

             pmbx->setButtonText(QMessageBox::No,"Исправить");
           int n = pmbx->exec();
           delete pmbx;

           if (n == QMessageBox::No) {
           }

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::MBok()
{
QString mes2 = "Ультразвуковой датчик уровня KS2 Prop 1 настроен на заданные значения";

QMessageBox msgBox;

msgBox.setWindowTitle(" ");
msgBox.setText(mes2);
msgBox.exec();
}

// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::AllValCheck()
{
QString Hval = ui->spinBox_H->text();
QString Dval = ui->spinBox_D->text();
QString mes;

QString st_H = "высот";
QString st_D = "ширин";

bool error, furst_string = true;

qDebug()<<"Change val 1 ------------";

QString Vval = ui->spinBox_V->text();
    QString mes1A, mes2A;
int H, D, DI, V, Vcalc;

H = ui->spinBox_H->value();
D = ui->spinBox_D->value();
DI = ui->spinBox_DI->value();
V = ui->spinBox_V->value();

error = false;


//=============================================
if(!ui->spinBox_D->isHidden())
{
    qDebug() << "Check inside diametr";

    if(DI > D/2)
    {
        mes1A = "Внутрений диаметр должен быть меньше, чем половина внешнего.";
        furst_string = false;
        error = true;
    }

}
//=============================================
if(error)
{
    MBtextErr(mes1A, mes01);
    return false;
}

furst_string = true;
mes1A.clear();
error = false;

//=============================================
Vcalc = D*D*H*M_PI_4/1000000;
qDebug() << "Vcalc = ";
qDebug() << Vcalc;

        if (V*100 < Vcalc*Size_V_min)
        {

            mes1A.append(mes5);
            mes1A.append("мало");

            error = true;
        }
        if (V*100 > Vcalc*Size_V_max)
        {

            mes1A.append(mes5);
            mes1A.append("велико");

            error = true;
        }
        if(V < 10 || V > 250)
        {
          mes1A = "Введено недопустимое значение объема";
          error = true;
        }

        if(error)
        {
            MBtextErr(mes1A, mes01);
            return false;
        }

        furst_string = true;
        mes1A.clear();
        error = false;
//=============================================
        if (H<140)
        {
            mes1A = "Высота бака слишком мала";

            MBtextErr(mes1A, mes01);
            return false;
        }
        furst_string = true;
        mes1A.clear();
        error = false;
//=============================================
                if (D<140)
                {
                    mes1A = "Внешний диаметр бака слишком мал";

                    MBtextErr(mes1A, mes01);
                    return false;
                }
                furst_string = true;
                mes1A.clear();
                error = false;
//=============================================
                        if (D>1200)
                        {
                            mes1A = "Внешний диаметр бака слишком велик";

                            MBtextErr(mes1A, mes01);
                            return false;
                        }
                        furst_string = true;
                        mes1A.clear();
                        error = false;
//=============================================

if (H<Size_B_min)
{
       mes1A.append(mes1);
       mes1A.append(st_H);
       mes1A.append("а бака ");
       mes1A.append(Hval);
       mes1A.append(" мм?");

       error = true;

} //
if (H>Size_B_max)
{
    mes1A.append("Введены слишком большие параметры ");
    mes1A.append(st_H);
    mes1A.append("ы бака. Вы уверены, что ");
    mes1A.append(st_H);
    mes1A.append("а бака ");
    mes1A.append(Hval);
    mes1A.append(" мм?");
    error = true;
} //
//=============================================
qDebug()<<"Change val 2 ------------";

if (D<Size_B_min)
{
if(!furst_string) mes1A.append(split_str);

mes1A.append(mes1);
mes1A.append(st_D);
mes1A.append("а бака ");
mes1A.append(Dval);
mes1A.append(" мм?");
error = true;
} //
if (D>Size_B_max)
{
    if(!furst_string) mes1A.append(split_str);

    mes1A.append("Введены слишком большие параметры ");
    mes1A.append(st_D);
    mes1A.append("ы бака. Вы уверены, что ");
    mes1A.append(st_D);
    mes1A.append("а бака ");
    mes1A.append(Dval);
    mes1A.append(" мм?");
    error = true;
} //

if(error)
{

    return MBtext(mes1A);
}

return true;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::AllValCorrect()
{
CorrectValH = true;
CorrectValD = true;
CorrectValV = true;
CorrectValDI = true;
}

// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::CorrectValAll()
{
    if(!CorrectValH) return false;
    if(!CorrectValD) return false;
    if(!CorrectValV) return false;
    if(!CorrectValDI) return false;

  return true;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_checkBox_stateChanged(int arg1)
{

    if (arg1 == Qt::Unchecked)
    {
        qDebug()<<"press 1 parametr";
        HandShow(&Hand);
        ui->widget_combo->show();
    }
    if (arg1 == Qt::Checked)
    {
        qDebug()<<"press H parametr";
        if(Hand == false){
            Hand = true;
            ui->frame_2->show();
            ui->widget_combo->hide();
        }

    }
    UpdateSizeWin();
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::SaveToFile(QString name, QString text)
{
    // SaveToFile("ComOut.txt",);

    QFile file2(name);
    bool yes = false;

    if (file2.open(QIODevice::WriteOnly))
    {
        qDebug() << "File is open";

        file2.write(text.toLocal8Bit()); //  text);
        file2.close();
        yes = true;
    }
    //-----------------------
    return yes;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::LoadFromFile()
{
    QString fileName = file_param;

    QFile file(fileName);
    bool yes = false;

    QString str="";
    QString str_1="";

    if((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
   // qDebug() << "Файл существует";
    if(file.isOpen())
    {
    qDebug() << "File is open";
    }
    while(!file.atEnd())
    {
        str_1 = file.readLine();
        DW_radio_text.append(str_1);
        str += str_1;
        }
        file.close();
        yes = true;
    }
return yes;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
Stringi MainWindow::ParamAnalise(QStringList  Input_text) // DW_radio_text
{
    Stringi DataStr;

    QString str_1="";
    QString metka = ")";
    QString curVal = "=>";
    QString number; // metkaCustom
    QString subStr;

    int metkaPos;

    bool numfind = false;

    DataStr.CurForma = 0;
    DataStr.Num_Bublik = 0;
    DataStr.Num_Vatruchka = 0;
    DataStr.Num_BochkaV = 0;
    DataStr.Num_BochkaH = 0;

    DataStr.Chislo_Bublik.clear();
    DataStr.Str_Bublik.clear();
    DataStr.Chislo_Vatruchka.clear();
    DataStr.Str_Vatruchka.clear();
    DataStr.Chislo_BochkaV.clear();
    DataStr.Str_BochkaV.clear();
    DataStr.Chislo_BochkaH.clear();
    DataStr.Str_BochkaH.clear();
    DataStr.Custom.clear();

    for (quint8 i = 0; i < Input_text.size(); i++) {
    str_1 = Input_text.at(i);


    if (str_1.contains(metka, Qt::CaseInsensitive))
    {

        metkaPos = str_1.indexOf(metka) + 1;
        subStr = str_1.left(metkaPos);
        number = FindChislo(subStr, 0);
        if(number == metkaCustom)
        {
          subStr = str_1.mid(metkaPos);
          subStr = subStr.trimmed();
          DataStr.Custom =   subStr;
        }

    if (str_1.contains(metkaBublik, Qt::CaseInsensitive))
    {
        DataStr.Chislo_Bublik.append(number);

        metkaPos = str_1.indexOf(metkaBublik) + metkaBublik.size();
        subStr = str_1.mid(metkaPos);

        subStr = subStr.trimmed();
        DataStr.Str_Bublik.append(subStr);
        //qDebug() << subStr;                             // <<<

        if (str_1.contains(curVal, Qt::CaseInsensitive))
        {
        DataStr.CurForma = 1;
        DataStr.CurParam = DataStr.Num_Bublik;
        numfind = true;
        } // ---
        DataStr.Num_Bublik += 1;

    } // metkaBublik
    // -------------------------------------
    if (str_1.contains(metkaVatruchka, Qt::CaseInsensitive))
    {
        DataStr.Chislo_Vatruchka.append(number);

        metkaPos = str_1.indexOf(metkaVatruchka) + metkaVatruchka.size();
        subStr = str_1.mid(metkaPos);

        subStr = subStr.trimmed();
        DataStr.Str_Vatruchka.append(subStr);
        //qDebug() << subStr;                             // <<<

        if (str_1.contains(curVal, Qt::CaseInsensitive))
        {
        DataStr.CurForma = 2;
        DataStr.CurParam = DataStr.Num_Vatruchka;
        numfind = true;
        } // ---
        DataStr.Num_Vatruchka += 1;

    } // metkaBublik
    // -------------------------------------
    if (str_1.contains(metkaBochkaV, Qt::CaseInsensitive))
    {
        DataStr.Chislo_BochkaV.append(number);

        metkaPos = str_1.indexOf(metkaBochkaV) + metkaBochkaV.size();
        subStr = str_1.mid(metkaPos);

        subStr = subStr.trimmed();
        DataStr.Str_BochkaV.append(subStr);
        //qDebug() << subStr;                             // <<<

        if (str_1.contains(curVal, Qt::CaseInsensitive))
        {
        DataStr.CurForma = 3;
        DataStr.CurParam = DataStr.Num_BochkaV;
        numfind = true;
        } // ---
        DataStr.Num_BochkaV += 1;

    } // metkaBublik
    // -------------------------------------
    if (str_1.contains(metkaBochkaH, Qt::CaseInsensitive))
    {
        DataStr.Chislo_BochkaH.append(number);

        metkaPos = str_1.indexOf(metkaBochkaH) + metkaBochkaH.size();
        subStr = str_1.mid(metkaPos);

        subStr = subStr.trimmed();
        DataStr.Str_BochkaH.append(subStr);
        //qDebug() << subStr;                             // <<<

        if (str_1.contains(curVal, Qt::CaseInsensitive))
        {
        DataStr.CurForma = 4;
        DataStr.CurParam = DataStr.Num_BochkaH;
        numfind = true;
        } // ---
        DataStr.Num_BochkaH += 1;

    } // metkaBublik
    // -------------------------------------

    }
    } // for
    DataStr.init = true;
    return DataStr;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
QString MainWindow::FindChislo(QString Input, int Index)
{
QString Output;
QRegExp rx("(\\d+)");
QStringList listNum;
int pos = 0;
while ((pos = rx.indexIn(Input, pos)) != -1) {
    listNum << rx.cap(1);
    pos += rx.matchedLength();
}
if(listNum.size()>Index){
Output = listNum.at(Index);
} else {
    Output = metkaNone;
}
return Output;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
QString MainWindow::getSetupTank()
{
    QString outputData = metkaNone;
    if(DataParam.CurFormaSelect == 1){
        if(ui->comboBox_Param->currentIndex() < DataParam.Num_Bublik)
        {
        outputData = DataParam.Chislo_Bublik.at(ui->comboBox_Param->currentIndex());
        } else {
        qDebug() <<  "Error 1";
        }
    }
    if(DataParam.CurFormaSelect == 2){
        if(ui->comboBox_Param->currentIndex() < DataParam.Num_Vatruchka)
        {
        outputData = DataParam.Chislo_Vatruchka.at(ui->comboBox_Param->currentIndex());
        } else {
        qDebug() <<  "Error 2";
        }
    }
    if(DataParam.CurFormaSelect == 3){
        if(ui->comboBox_Param->currentIndex() < DataParam.Num_BochkaH)
        {
        outputData = DataParam.Chislo_BochkaH.at(ui->comboBox_Param->currentIndex());
        } else {
        qDebug() <<  "Error 3";
        }
    }
    if(DataParam.CurFormaSelect == 4){
        if(ui->comboBox_Param->currentIndex() < DataParam.Num_BochkaV)
        {
        outputData = DataParam.Chislo_BochkaV.at(ui->comboBox_Param->currentIndex());
        } else {
        qDebug() <<  "Error 4";
        }
    }
    return outputData;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::removeEcho(QString *Input)
{
    QStringList list;

    list = Input->split(split_str); // "\r\n"

    if(list.size() > 1)
    {
        list.removeFirst();
        *Input = list.join(split_str);

    } else {
    return false;
    }
    return true;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::removeEnd(QString *Input)
{
  int   sizeArr;
  bool ret = true;
  sizeArr = Input->size();

// Remove "\r\n" simbol
if(sizeArr>2)
{
    sizeArr-=1;
    if (Input->at(sizeArr) == '\n')
    {
        Input->remove(sizeArr,1);
         sizeArr-=1;
    } else {
    ret = false;
    }
    if (Input->at(sizeArr) == '\r')
    {
        Input->chop(1);
        sizeArr-=1;
    }else {
    ret = false;
    }
}
return ret;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_F1_clicked()
{
    on_radioButton_F1_func();
    ui->label_DI->show();
    ui->spinBox_DI->show();
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_F1_func(void)
{
    if(NumPressRadio1 != 1)
    {
    NumPressRadio1 = 1;
    DataParam.CurFormaSelect = 1;
    qDebug()<<"Radio Button 1 press";
    FormaShow(&Forma);

    ui->frame_radio->hide();

    if(DataParam.init)
    {
        ui->comboBox_Param->clear();
        for (int i = 0; i < DataParam.Num_Bublik; i++) {
        ui->comboBox_Param->addItem(DataParam.Str_Bublik.at(i));
     //   ui->comboBox_Param->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
        } // for
        if(DataParam.CurForma == 1)
        {
        ui->comboBox_Param->setCurrentIndex(DataParam.CurParam);
        }
    } // init
    }
    // read num param
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_F2_clicked()
{
    on_radioButton_F2_func();
    ui->label_DI->hide();
    ui->spinBox_DI->hide();

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_F2_func(void)
{
    if(NumPressRadio1 != 2)
    {
     NumPressRadio1 = 2;
     DataParam.CurFormaSelect = 2;
     qDebug()<<"Radio Button 2 press";
     FormaShow(&Forma);

     ui->frame_radio->hide();


     if(DataParam.init)
     {
         ui->comboBox_Param->clear();
         for (int i = 0; i < DataParam.Num_Vatruchka; i++) {
         ui->comboBox_Param->addItem(DataParam.Str_Vatruchka.at(i));
     //    ui->comboBox_Param->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
         } // for
         if(DataParam.CurForma == 2)
         {
         ui->comboBox_Param->setCurrentIndex(DataParam.CurParam);
         }
     } // init

    }

}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_F3_clicked()
{
    on_radioButton_F3_func();
    ui->label_DI->hide();
    ui->spinBox_DI->hide();
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_F3_func()
{
    if(NumPressRadio1 != 3)
    {
    NumPressRadio1 = 3;
    qDebug()<<"Radio Button 3 press";
    FormaShow(&Forma);

    ui->frame_radio->show();

    DataParam.CurFormaSelect = 3;
    if(CylPositionV) DataParam.CurFormaSelect++;

    if(DataParam.init)
    {
        CylPositionShow();
    } // init

    }
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_H_clicked()
{
    CylPositionV = false;
    DataParam.CurFormaSelect = 3;
     CylPositionShow();
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_radioButton_V_clicked()
{
    CylPositionV = true;
    DataParam.CurFormaSelect = 4;
     CylPositionShow();


}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
bool MainWindow::RefreshPorts()
{
    bool exist_vendor;
    quint16 vendor_id2;
    quint16 product_id2;

    ui->comboBox->clear();

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {

        exist_vendor = true;
        if(info.hasVendorIdentifier()){
            vendor_id2 = info.vendorIdentifier();
        } else {
        exist_vendor = false;
        }
                qDebug() << "Has Product id" << info.hasProductIdentifier();
                if(info.hasProductIdentifier()){
                    product_id2 = info.productIdentifier();
                }else {
                exist_vendor = false;
                }
        if((exist_vendor)&&(vendor_id == vendor_id2)&&(product_id == product_id2))
        {
           ui->comboBox->addItem(info.portName());
        }
    }
    // end search port

    if(ui->comboBox->count() == 0)
    {
    ui->pushButton->setText(st_btn_search);
    } else {
    ui->pushButton->setText(st_btn_connect);
    return true;
    }
return false;
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
void MainWindow::on_checkBox_toggled(bool checked)
{
   UpdateSizeWin();
}
// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
