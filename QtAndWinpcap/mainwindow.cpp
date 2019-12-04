#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include <QMenu>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    pcap = new Winpcap();
    connect(pcap, SIGNAL(sendMsg(QString)), this, SLOT(functionHandle(QString)));

   if (pcap->init()){
       QMenu*  setting = menuBar()->addMenu(tr("&Setting"));
       QMenu *devChose =  setting->addMenu(tr("&Device"));
       vector<QString> devList = pcap->getDeviceList();
       qDebug()<<devList.size();
       for(uint i=0; i<devList.size(); i++) {
           qDebug()<<devList[i];
           devChose->addAction(devList[i]);
       }
       connect(devChose, SIGNAL(triggered(QAction*)), this, SLOT(choseDevice(QAction*)));
   }
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_btn_start_clicked(){
    if(ui->btn_start->text()=="Start"){
        this->pcap->Start(ui->devName->text());
        this->ui->btn_start->setText("Stop");
        /*
        while(pcap->isRunning){
            int rowConut = ui->mainTable->rowCount();
            if(rowConut==pcap->packCount) continue;
            QString row = pcap->rows_vec[uint(rowConut)];
            QStringList vols = row.split("@");
            if(vols.length()!=3){
                QMessageBox::warning(this, "warn", "Ip message format worng!");
                Sleep(1000);
                continue;
            }
            ui->mainTable->setItem(rowConut, 0, new QTableWidgetItem(vols[0]));
            ui->mainTable->setItem(rowConut, 1, new QTableWidgetItem(vols[1]));
            ui->mainTable->setItem(rowConut, 2, new QTableWidgetItem(vols[2]));
        }
        */
    }
    else if(ui->btn_start->text()=="Stop"){
        this->ui->btn_start->setText("Start");
    }
    return;
}


//functionHandle is a interface for bridge to controll mainwindow
void MainWindow::functionHandle(QString msg){
    int idx = msg.lastIndexOf("@");
    if(idx<=0){
        QMessageBox::information(this, "error", "Unexpect message:"+msg );
        return;
    }
    QString msgType = msg.left(idx);
    if(msgType=="error" || msgType=="warn" || msgType=="success"){
        QMessageBox::information(this, msgType, msg.right(msg.length()-idx-1) );
        return;
    }
    return;
}


void MainWindow::choseDevice(QAction *ac){
    ui->devName->setText(ac->text());
}
