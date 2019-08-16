#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sendlerclass.h"

#include <QMainWindow>
#include <QSerialPort>

#define     btn_connect    1
#define     btn_setup      2
#define     btn_cansel     3

#define      res_none     0
#define      res_shape    1
#define      res_height   2
#define      res_lenght   3
#define      res_liquid   4
#define      res_mix      5

#define      testenable   false

extern QSerialPort *serial;

extern QStringList  set_list;
extern const char * st_set_shape;
extern const char * st_set_mix;
extern const char * st_set_liq;

extern QByteArray requestData;
extern QByteArray sendData;

extern bool ComConnect;
extern bool Input;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private slots:
    void serialReceived();

    void realPush(const QString &s, int res);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_test_clicked();

private:
    Ui::MainWindow *ui;

    SendlerClass SchwartzPower;
    void reOpenPort();

};

#endif // MAINWINDOW_H
