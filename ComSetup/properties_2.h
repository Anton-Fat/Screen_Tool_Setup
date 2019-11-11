#ifndef PROPERTIES_2_H
#define PROPERTIES_2_H

#include <QDialog>
#include "sendlerclass.h"

#define      NumDopParam   4

#define     btn_connect    1
#define     btn_setup      2
#define     btn_cansel     3
#define     btn_setup_dop  4
#define     btn_cansel_dop 5


extern const QString BorderButton;
extern int NumPressButton;


extern const char * st_get_display3a;
extern const char * st_get_display3;
extern const char * st_get_display1;
extern const char * st_get_display2;


extern const  QString mes0;
extern const  QString mes01;
extern const  QString mes1;
extern const  QString mes8;
extern const  QString mes9;
extern const  QString mes5;

extern bool Properties;
extern bool ComConnect;

extern bool SetupParamDopB[NumDopParam];
extern int  SetupParamDopVal[NumDopParam];



typedef struct
{
    int CurForma;   // None | Num_Bublik | Num_Vatruchka | Num_Bochka = 0 1 2 3
    int CurParam;
    int CurFormaSelect;

    quint8 Num_Bublik;
    quint8 Num_Vatruchka;
    quint8 Num_BochkaV;
    quint8 Num_BochkaH;

    bool init;

  QStringList  Str_Bublik;
  QStringList  Str_Vatruchka;
  QStringList  Str_BochkaV;
  QStringList  Str_BochkaH;

  QStringList  Chislo_Bublik;
  QStringList  Chislo_Vatruchka;
  QStringList  Chislo_BochkaV;
  QStringList  Chislo_BochkaH;

  QString Custom;

} Stringi;

extern Stringi DataParam;

namespace Ui {
class Properties_2;
}

class Properties_2 : public QDialog
{
    Q_OBJECT

public:
    explicit Properties_2(QWidget *parent = nullptr);
    ~Properties_2();


private slots:
    void on_pushButton_PR_cansel_clicked();

    void on_pushButton_PR_setup_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_editingFinished();

    void on_comboBox_view_currentIndexChanged(int index);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

private:
    Ui::Properties_2 *ui;

    void horizontalSlider_setText(int value);

};

#endif // PROPERTIES_2_H
