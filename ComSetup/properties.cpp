#include "properties.h"
#include "ui_properties.h"

#include <QDebug>
#include <QThread>



Properties::Properties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}
// ======================================================================
Properties::~Properties()
{
    delete ui;


}


// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB




