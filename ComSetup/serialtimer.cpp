#include "serialtimer.h"
#include "mainwindow.h"


#include <QDebug>
#include <QSerialPort>
#include <QTime>


SerialTimer::SerialTimer(QObject *parent):
    QThread(parent)
 {
 }

 SerialTimer::~SerialTimer()
 {
     m_quit = true;
     m_conb.wakeOne();
     wait();
 }
 void SerialTimer::run()
 {
       qDebug() << "Kostil run";
       int countTime;
       QString str;

       while (!m_quit) {

        QThread::msleep(100);

        if (on_timer && ComConnect) {
        end = false;
        countTime = Count_time/10;

        while (countTime!=0)
        {
        countTime--;
        QThread::msleep(10);
        if(Input_K == true)
        {
        Input_K = false;
        countTime = Count_time/10;
        }
        }

        str = DataInput;
        emit this->pop(str, Count);        // ---
        on_timer = false;
        end = true;
        }

       } // while
}
