#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "sendlerclass.h"

#include <QDialog>


namespace Ui {
class MainWindow;
}

// ======================================================================
class Properties : public QDialog
{
    Q_OBJECT

public:
    Properties(QWidget *parent = nullptr);

    ~Properties();

private slots:


private:
  //  Dialog *ui;



};

#endif // PROPERTIES_H
