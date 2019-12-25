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
    int gad;

     while (!m_quit) {

      QThread::msleep(100);

      if (m_loop && ComConnect) {

            for (gad = NumDopParam+2; gad < NumDopParam+4; gad++)
            {

                str = set_list.at(gad);
                emit this->push(str, gad);        // ---

                timeOut();

                if (!(m_loop && ComConnect)) {
                break;
                }
            } // for

            emit this->push("end", -1);
            m_loop = false;

      } // m_loop
      if (Properties && ComConnect) {

          for (int i = 0; i < NumDopParam; i++) {

              if(SetupParamDopB[i] == true)
              {
                  gad = i+1;

                  str = set_list.at(gad);
                  emit this->push(str, gad);        // ---

                  timeOut();

                  if (!(Properties && ComConnect)) {
                  break;
                  }

              }

          } // for
          emit this->push("end", -2);
          Properties = false;

      } // Properties
      if (PingPong && ComConnect) {

          for (int i = 0; i < 1; i++) {
          gad = excellent;
          fi = 1;

          str = get_list.at(gad);
          qDebug() << str;

          emit this->push(str, res_kal);        // ---

          timeOut();
          if (!(PingPong && ComConnect)) {
          break;
          }

          gad = excellent + strike;
          fi = 0;
          str = set_list.at(gad);
          qDebug() << str;
          emit this->push(str, res_kal);        // ---

          timeOut();
          if (!(PingPong && ComConnect)) {
          break;
          }

          gad = excellent + strike;
          fi = 2;
          str = get_list.at(gad);
          qDebug() << str;
          emit this->push(str, res_kal);        // ---

          PingPong = false;
        }
      } // PingPong

     }
     qDebug() << "=============================================";
}
void SendlerClass::timeOut()
{
int countTime;

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
}
