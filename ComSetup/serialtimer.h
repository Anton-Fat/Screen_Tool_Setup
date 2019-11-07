#ifndef SERIALTIMER_H
#define SERIALTIMER_H

//--
#include <QThread>
#include <QWaitCondition>

#define      waitRePush    5   // time = v * 100 msek


class SerialTimer : public QThread // public QObject
{
    Q_OBJECT

public:

    explicit SerialTimer(QObject *parent = nullptr);
    ~SerialTimer();

    bool on_timer = false;
    bool end;
    bool Input_K = false;
    int Count_time;
    int TimeOut;
    int Count;
    QByteArray DataInput;

signals:
    void pop(const QString &s, int res);

private:
    void run() override;


    QWaitCondition m_conb;
    bool m_quit = false;


};


#endif // SERIALTIMER_H

