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
#include "mainwindow.h"
#include "properties_2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

  //  Properties_2 ad; //


 //   ad.show();



    return a.exec();
}
