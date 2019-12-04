#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "winpcap.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

    Winpcap *pcap;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void functionHandle(QString key);
    void choseDevice(QAction*);
    void on_btn_start_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

