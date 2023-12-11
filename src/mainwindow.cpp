#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <string.h>
#include <iostream>
//#include "calc.c"
//#include "lexem.c"
//#include "queue.c"
//#include "parser.c"
//extern "C" {
//#include "calc.c"
//#include "parser.c"
//#include "queue.c"
//#include "lexem.c"
//}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("0.1");
    ui->lineEdit_2->setText("-10");
    ui->lineEdit_3->setText("10");
    ui->lineEdit_Ymin->setText("-3");
    ui->lineEdit_Ymax->setText("3");
    ui->radioButton_annuitet->setChecked(true);
    ui->radioButton_perDay->setChecked(true);
    ui->tabWidget->setCurrentIndex(0);
    ui->lineEdit_creditSum->setText("100000");
    ui->lineEdit_date->setText("10");
    ui->lineEdit_persent->setText("10");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearVectors()
{
    if (!x.empty()) {
        x.clear();
    }
    if (!y.empty()) {
        y.clear();
    }
}

QString btos(bool flag) {
    return flag ? "not empty" : "empty";
}

void MainWindow::on_btnEqalSign_clicked()
{
    char str[255] = "";
    double num = 0;
    double x = 0;
    x = QString::number(ui->lineEdit_word->text().toDouble(), 'g', 7).toDouble();

        std::strcpy(str, ui->lineEdit_math_expression->text().toLocal8Bit().data());
        QMessageBox msg;
        if (!ui->lineEdit_math_expression->displayText().isEmpty()) {
            if (!get_expression_and_calculate(str, x, &num)) {
                QString string(str);
                msg.setText(string + "=" + QString::number(num, 'g', 7));
            } else {
                QString err_string("error: incorrect expression");
                msg.setText(err_string);
            }
        }
        msg.exec();
}

void MainWindow::on_btnDrawGraph_clicked()
{
    MainWindow::clearVectors();
    QMessageBox msg;
    dx = QString::number(ui->lineEdit->text().toDouble(), 'g', 7).toDouble();
    xBegin = QString::number(ui->lineEdit_2->text().toDouble(), 'g', 7).toDouble();
    xEnd = QString::number(ui->lineEdit_3->text().toDouble(), 'g', 7).toDouble();
    char str[255] = "";
    std::strcpy(str, ui->lineEdit_math_expression->text().toLocal8Bit().data());
    double num = 0;
    if (!ui->lineEdit_math_expression->displayText().isEmpty()
            && !ui->lineEdit->displayText().isEmpty()
            && !ui->lineEdit_2->displayText().isEmpty()
            && !ui->lineEdit_3->displayText().isEmpty()) {
        if (dx > 0) {
            for (tmpX = xBegin; tmpX <= xEnd; tmpX += dx) {
                if (!get_expression_and_calculate(str, tmpX, &num)) {
                    x.push_back(tmpX);
                    y.push_back(num);
                } else {
                    msg.setText("Error");
                    msg.exec();
                }
            }
        }
    }

    ui->widget->xAxis->setRange(xBegin, xEnd);
    double yMax = QString::number(ui->lineEdit_Ymax->text().toDouble(), 'g', 2).toDouble();
    double yMin = QString::number(ui->lineEdit_Ymin->text().toDouble(), 'g', 2).toDouble();
    ui->widget->yAxis->setRange(yMin, yMax);
    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();
    ui->widget->clearGraphs();
}

void MainWindow::calcAnnuitetCredit()
{
    QMessageBox msg;

    std::string creditSum = ui->lineEdit_creditSum->text().toStdString();
    std::string percent = ui->lineEdit_persent->text().toStdString();
    std::string date = ui->lineEdit_date->text().toStdString();
    std::string str(creditSum + " * " + percent + " / 1200" + " * " + "(1 + " + percent + " / 1200" + ") ^ " + date + " / ((1 + " + percent + " / 1200" + ") ^ " + date + " - 1)");

    char mathExp[255] = "";
    std::strcpy(mathExp, str.c_str());
    double payment = 0;
    if (!get_expression_and_calculate(mathExp, 0, &payment)) {
        ui->label_monthPay->setText(QString::number(payment, 'f', 2));
    } else {
        msg.setText("Error");
        msg.exec();
    }
    double fullPay = payment * std::stod(date);
    ui->label_fullPay->setText(QString::number(fullPay, 'f', 2));
    ui->label_overPay->setText(QString::number(fullPay - atof(creditSum.c_str()), 'f', 2));
}

void MainWindow::calcDifferentCredit()
{
    std::string creditSum = ui->lineEdit_creditSum->text().toStdString();
    std::string percent = ui->lineEdit_persent->text().toStdString();
    std::string date = ui->lineEdit_date->text().toStdString();
    double S = QString::number(ui->lineEdit_creditSum->text().toDouble(), 'g', 7).toDouble();
    double n = QString::number(ui->lineEdit_date->text().toDouble(), 'g', 7).toDouble();
    double p = QString::number(ui->lineEdit_persent->text().toDouble(), 'g', 7).toDouble();
    double b = S / n; // main pay

    ui->tableWidget->setRowCount(n);
    ui->tableWidget->setColumnCount(3);

    QMessageBox msg;
    double payment = 0;
    QVector<double> paymentI;

    for (int i = 0; i < n; i++) {
        double Sn = S - (b * i);
        double pros = Sn * p / 1200;
        double perMonth = b + (S - (b * i)) * p / 1200;
        std::string str_perMounth(creditSum + "/" + date + "+" + "(" + creditSum + "-(" + creditSum + "/" + date + "*x))*" + percent + "/1200");
        char mathExp[255] = "";
        std::strcpy(mathExp, str_perMounth.c_str());
        if (!get_expression_and_calculate(mathExp, i, &payment)) {

        } else {
            msg.setText("Error");
            msg.exec();
        }

        paymentI.append(payment);
        QTableWidgetItem *newItem [] = {
            new QTableWidgetItem(QString::number(payment, 'f', 2)),
            new QTableWidgetItem(QString::number(pros, 'f', 2)),
            new QTableWidgetItem(QString::number(Sn - perMonth, 'f', 2)),
        };
        for (size_t col = 0; col < 3; col++) {
            ui->tableWidget->setItem(i, col, newItem[col]);
        }
    }

    // main pay --
    QVectorIterator<double> i(paymentI);
    QString expMainPay;
    while (i.hasNext()) {
        expMainPay.append("+" + QString::number(i.next(), 'f', 2));
    }
    double dexpMainPay = 0;
    char mathExpMain[255] = "";
    std::strcpy(mathExpMain, expMainPay.toStdString().c_str());
    if (!get_expression_and_calculate(mathExpMain, 0, &dexpMainPay)) {
        ui->label_fullPay->setText(QString::number(dexpMainPay, 'f', 2));
    } else {
        msg.setText("Error");
        msg.exec();
    }
    // -- main pay

    // overPay --
    double dexpOverPay = 0;
    char mathExpOver[255] = "";
    std::string strOver(ui->label_fullPay->text().toStdString() + "-" + ui->lineEdit_creditSum->text().toStdString());
    std::strcpy(mathExpOver, strOver.c_str());

    if (!get_expression_and_calculate(mathExpOver, 0, &payment)) {
        ui->label_overPay->setText(QString::number(payment, 'f', 2));
    } else {
        msg.setText("Error");
        msg.exec();
    }
    // -- overPay

    // pay per month --
    ui->label_monthPay->setText(QString::number(paymentI.takeFirst(), 'f', 2) + " - " + QString::number(paymentI.takeLast()));
    // -- pay per month
}

void MainWindow::on_btnCalcCredit_clicked()
{
    if (ui->lineEdit_creditSum->text().isEmpty()) {
        ui->lineEdit_creditSum->setText("0");
    }
    if (ui->lineEdit_date->text().isEmpty()) {
        ui->lineEdit_date->setText("0");
    }
    if (ui->lineEdit_persent->text().isEmpty()) {
        ui->lineEdit_persent->setText("0");
    }
    if (ui->radioButton_annuitet->isChecked()) {
        MainWindow::calcAnnuitetCredit();
    } else {
        MainWindow::calcDifferentCredit();
    }
}

void MainWindow::on_btnCalcDeposit_clicked()
{
    double P = QString::number(ui->lineEdit_SumDebit->text().toDouble(), 'g', 7).toDouble();
    double N = QString::number(ui->lineEdit_procentDebit->text().toDouble(), 'g', 7).toDouble();
    double T = QString::number(ui->lineEdit_dateDebit->text().toDouble(), 'g', 7).toDouble();
    double nalogProc = QString::number(ui->lineEdit_nalogProc->text().toDouble(), 'g', 7).toDouble();
    double Db = 0;
    if (ui->radioButton_perDay->isChecked()) {
        Db = P * pow((1 + N / 100 / 365), T * 30);
    } else {
        Db = P * pow((1 + N / 100 / 12), T);
    }
    double diff = Db - P - (nalogProc / 100) * 1000000;
    double nalog = 0;
    if (diff > 0) {
        nalog = diff * 0.13;
    }
    ui->label_nalogSum->setText(QString::number(nalog, 'f', 2));
    ui->label_totalSum->setText(QString::number(Db, 'f', 2));
    ui->label_sumPercent->setText(QString::number(Db - P, 'f', 2));
}
