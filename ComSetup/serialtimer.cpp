#include "serialtimer.h"
#include "mainwindow.h"


#include <QDebug>
#include <QSerialPort>
#include <QTime>

#define      timeStatus     1000


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
       int countTime, countStatus;
       QString str;
       countStatus = 0;

       while (!m_quit) {

        QThread::msleep(100);

        if (on_timer && ComConnect) {
        end = false;
        countStatus = 0;
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
        }   // on_timer
        if (!on_timer && ComConnect) {
            countStatus++;
            if(countStatus >= timeStatus/100)
            {
            emit this->status(1);
            countStatus = 0;
            }

        }
        if (!ComConnect) {
            countStatus++;
            if(countStatus >= timeStatus * 3/100)
            {
            emit this->status(2);
            countStatus = 0;
            }

        }

       } // while
}
