#include "sendlerclass.h"
#include "mainwindow.h"

#include <QDebug>
#include <QSerialPort>
#include <QTime>
// #include <QTimer>

SendlerClass::SendlerClass(QObject *parent):
   QThread(parent)
{
}

SendlerClass::~SendlerClass()
{

    m_quit = true;
    m_cond.wakeOne();

    wait();

}
void SendlerClass::run()
{

    QString str;
    qDebug() << "Thread run";
    int countTime;


     while (!m_quit) {

      QThread::msleep(100);

      if (m_loop && ComConnect) {

            for (int gad = 1; gad < 7; gad++) // set_list.size()
            {

                str = set_list.at(gad);
                emit this->push(str, gad);        // ---

                countTime = waitRePush;

                while (countTime!=0)
                {
                countTime--;
                QThread::msleep(100);
                if(Input == true)
                {
                Input = false;
                countTime = waitRePush;
                }
                }

                if (!(m_loop && ComConnect)) {
                    break;
                }
            }

            emit this->push("end", -1);
            m_loop = false;

      }
     }   
}

