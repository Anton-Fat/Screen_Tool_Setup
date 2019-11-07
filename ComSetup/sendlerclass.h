/*********************************%
% KS2 Screen tool	              %
%                                 %
% Version 1.00                    %
% Date: 12.08.2019                %
% Device: Propane screen		  %
% Autor: Anton Fateyev            %
% Compile: Qt 5.3   	  		  %
% mail: antonfat@gmail.com        %
%*********************************/
#ifndef SENDLERCLASS_H
#define SENDLERCLASS_H

#include <QThread>
#include <QWaitCondition>

#define      waitRePush    5   // time = v * 100 msek


class SendlerClass : public QThread // public QObject
{
    Q_OBJECT

public:    

    explicit SendlerClass(QObject *parent = nullptr);
    ~SendlerClass();

    bool m_loop = false;

  //  bool m_loop_dop = false;
  //  bool m_correct = false;

signals:
    void push(const QString &s, int res);

private:
    void run() override;


    QWaitCondition m_cond;
    bool m_quit = false;


};

#endif // SENDLERCLASS_H
