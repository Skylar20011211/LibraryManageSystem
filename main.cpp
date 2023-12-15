#include "mainwindow.h"

#include <QApplication>
#include<QSqlDatabase>
#include"login.h"
#include<QMessageBox>
#include<QDebug>
#include<QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("/Users/skylar/Qt5.14.2/5.14.2/clang_64/plugins");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("Library");
    db.setUserName("root");
    db.setPassword("20011211");
    bool ok = db.open();
      if (ok){
         qDebug()<<" open database";
      }
      else {

          qDebug()<<"error open database";
      }

    //db.exec("SET NAMES 'GBK'");
    QApplication a(argc, argv);
    login w;
    w.show();
    return a.exec();
}
