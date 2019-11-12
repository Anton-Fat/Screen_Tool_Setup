#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sendlerclass.h"
#include "serialtimer.h"
#include "properties_2.h"


#include <QMainWindow>
#include <QSerialPort>



#define      res_none     0
#define      res_display  1
#define      res_wall     2
#define      res_mix      3
#define      res_deadvol  4
#define      res_custom   6
#define      res_tank     7

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


private:
    Ui::MainWindow *ui;


    void reOpenPort();

    bool eventFilter(QObject *obj, QEvent *evt);

    void FormaShow(bool *Forma);
    void HandShow(bool *Hand);
    bool MBtext(QString mes);
    bool MBtextS(QString mes, QString mes2);
    void MBtextErr(QString mes1, QString mes2);
    bool MBok();
    void AllValCorrect();
    bool CorrectValAll();
    bool AllValCheck();

    bool LoadFromFile();
    bool SaveToFile(QString name, QString text);
    Stringi ParamAnalise(QStringList  Input_text);
    QString FindChislo(QString Input, int Index);
    void CylPositionShow();

    bool removeEcho(QString *Input);
    bool removeEnd(QString *Input);

    void on_radioButton_F1_func(void);
    void on_radioButton_F2_func(void);
    void on_radioButton_F3_func(void);
    QString getSetupTank();
    void RefreshPorts();

    Properties_2 *ad;

};

#endif // MAINWINDOW_H
