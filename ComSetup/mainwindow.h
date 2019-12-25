#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sendlerclass.h"
#include "serialtimer.h"
#include "properties_2.h"
#include "kalibration.h"


#include <QMainWindow>
#include <QSerialPort>

#include <QProcess>
//#include <QMoveEvent>


#define      base_num_param 6

#define      res_none     0
#define      res_display  1
#define      res_wall     2
#define      res_mix      3
#define      res_deadvol  4
#define      res_custom   6
#define      res_tank     7
#define      res_kal      8

#define PERCENT_OF_SCREEN_H 90 / 100
// #define PERCENT_OF_SCREEN_W 40 / 100

#define      testenable   false

extern QSerialPort *serial;

extern QStringList  set_list;
extern QStringList  get_list;


extern QByteArray requestData;
extern QByteArray sendData;


extern bool Input;

extern quint8 NumParam;


namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void ComSendData(const QByteArray &data);


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SendlerClass SchwartzPower;
    SerialTimer  SerialKostil;

protected:
    void moveEvent(QMoveEvent *event);

private slots:
    void on_pushButton_clicked();

private slots:
    void serialReceived();

    void writeData(const QByteArray &data);

    void realPush(const QString &s1, int res);

    void realPop(const QString &s1, int res);

    void realStatus(int res);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_test_clicked();

    void on_pb_Add_Parametr_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_radioButton_F1_clicked();

    void on_radioButton_F2_clicked();

    void on_radioButton_F3_clicked();

    void on_radioButton_H_clicked();

    void on_radioButton_V_clicked();


    void on_checkBox_toggled(bool checked);

    void on_pb_Kalibrovka_clicked();

    void on_pb_Kalibrovka_help_clicked();

    void on_pushButton_Reset_clicked();

private:
    Ui::MainWindow *ui;


    void reOpenPort();

    bool eventFilter(QObject *obj, QEvent *evt);

    void StartWaitBar();
    void StopWaitBar();
    bool UpdateSizeWin();
    void FormaShow(bool *Forma);
    void HandShow(bool *Hand);
    bool MBtext(QString mes);
    bool MBtextS(QString mes, QString mes2);
    bool MBtextSimple(QString mes0, QString mes1, QString mesYes, QString mesNo);
    void MBtextErr(QString mes1, QString mes2, QString mes3);
    void MBtextInfo(QString mes1, QString mes2, QString mes3);
    bool MBok();
    void AllValCorrect();
    bool CorrectValAll();
    bool AllValCheck();

    bool LoadFromFile();
    bool SaveToFile(QString name, QString text);
    Stringi ParamAnalise(QStringList  Input_text);
    QString FindChislo(QString Input, int Index);
    QString FindChisloEx(QString Input, int Index, QString Type);
    void CylPositionShow();
    bool DiagnosticsDriver();

    bool removeEcho(QString *Input);
    bool removeEnd(QString *Input);

    void on_radioButton_F1_func(void);
    void on_radioButton_F2_func(void);
    void on_radioButton_F3_func(void);
    QString getSetupTank();
    bool RefreshPorts();
    void SuperCmdRun(QString command, int exeCode);
    void on_process_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_process_readyReadStandardOutput();
    bool CMDAnalise(QString command);
    void KalibrationView(bool Run);
    QString GetKalString();
    bool CheckKal();

   // float windowsDpiScale();

    Properties_2 *ad;
    Kalibration *kal;

    QProcess p;

};

#endif // MAINWINDOW_H
