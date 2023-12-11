#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>

//extern "C" {
//#include "calc.h"
//#include "parser.h"
//#include "queue.h"
//#include "lexem.h"
//}

#ifdef __cplusplus
extern "C" {
#endif
#include "calc.h"
#include "parser.h"
#include "queue.h"
#include "lexem.h"
#ifdef __cplusplus
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clearVectors();

private slots:
    void on_btnEqalSign_clicked();
    void on_btnDrawGraph_clicked();
    void calcAnnuitetCredit();
    void calcDifferentCredit();
    void on_btnCalcCredit_clicked();

    void on_btnCalcDeposit_clicked();

private:
    Ui::MainWindow *ui;
    double xBegin, xEnd, dx, tmpX;
    int numPoint;
    QVector<double> x, y;
};
#endif // MAINWINDOW_H
