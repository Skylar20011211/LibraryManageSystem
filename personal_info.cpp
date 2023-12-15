#include "personal_info.h"
#include "ui_personal_info.h"
#include <QSqlQuery>
#include"login.h"
#include<QMessageBox>
#include<QDebug>
#include<QDateTime>
personal_info::personal_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::personal_info)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("图书管理系统"));
    model = new QSqlTableModel(this);
    model->setTable("borrow");
    setTitle();
    model->setFilter(QString("card_id=%1").arg(login::card_id));
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(3,190);
    ui->tableView->setColumnWidth(4,190);

    QSqlQuery query;
    QString sql=QString("select * from card where card_id=%1").arg(login::card_id);
    query.exec(sql);
    while(query.next())
    {
        ui->cardID->setText(query.value(0).toString());
        ui->name->setText(query.value(1).toString());
        ui->password->setText(query.value(4).toString());
        ui->department->setText(query.value(2).toString());
      }
    ui->cardID->setEnabled(false);
    ui->name->setEnabled(false);
    ui->department->setEnabled(false);
}

personal_info::~personal_info()
{
    delete ui;
}

void personal_info::on_borrowButton_clicked()
{
    int book_id=ui->lineEdit->text().toInt();
    QSqlQuery query;
    QString sql0=QString("select * from book where book_id=%1").arg(book_id);
    query.exec(sql0);
    int stock;
    QString title;//QT中不同于C++,只有QString
        query.next();
        stock=query.value(7).toInt();
        title=query.value(2).toString();

    QDateTime borrow_qtime=QDateTime::currentDateTime();
    QDateTime return_qtime=borrow_qtime.addMonths(1);
    QString borrow_time = borrow_qtime.toString("yyyy-MM-dd HH:mm:ss");
    QString return_time = return_qtime.toString("yyyy-MM-dd HH:mm:ss");
    if(stock>0){
        stock--;
        QString sql1=QString("insert into borrow(card_id,book_id,title,borrow_time,return_time) values(%1,%2,'%3','%4','%5')").arg(login::card_id).arg(book_id).arg(title).arg(borrow_time).arg(return_time);
        QString sql2=QString("update book set stock=%1 where book_id=%2").arg(stock).arg(book_id);
        if(query.exec(sql1)&&query.exec(sql2)){
            model->select();
            QMessageBox::information(NULL,"借书成功","借书成功！");
        }
    }
    else{
            QMessageBox msgBox;
            QString sql3=QString("select * from borrow where book_id=%1 order by return_time desc").arg(book_id);
            query.exec(sql3);
            QString earliest_return;
            while(query.next()){
                earliest_return=query.value(4).toString();
            }
            QDialog *dialog = new QDialog(this);
            dialog->setWindowTitle("借书失败");
            dialog->setFixedSize(300, 150);
            QLabel *label = new QLabel(dialog);
            QString text = "您所借的图书已无库存\n最近的归还时间是:\n %1";
            label->setText(text.arg(earliest_return));
            dialog->show();



    }
}
void personal_info::setTitle(){
    model->setHeaderData(0,Qt::Horizontal, "card_id");
    model->setHeaderData(1,Qt::Horizontal, "book_id");
    model->setHeaderData(2,Qt::Horizontal, "title");
    model->setHeaderData(3,Qt::Horizontal, "borrow_time");
    model->setHeaderData(4,Qt::Horizontal, "return_time");


}


void personal_info::on_returnButton_clicked()
{
    int book_id=ui->lineEdit_2->text().toInt();
    QSqlQuery query;
    QString sql0=QString("update book set stock=stock+1 where book_id=%1").arg(book_id);
    QString sql1=QString("delete from borrow where book_id=%1").arg(book_id);
    if( query.exec(sql0)&&query.exec(sql1)){
        model->select();
        QMessageBox::information(NULL,"还书成功","还书成功！");
    }
    else{
        QMessageBox::information(NULL,"还书失败","还书失败！");
    }

}

void personal_info::on_quitButton_clicked()
{
    if (!(QMessageBox::information(this,tr("退出"),tr("确定退出吗?"),tr("Yes"),tr("No"))))
    {
        this->close();
    }
}

void personal_info::on_update_infor_Button_clicked()
{
    QString password=ui->password->text();
    QSqlQuery query;
    if(password!=NULL)
    {
        QString sql=QString("update card set pw='%1' where card_id=%2").arg(password).arg(login::card_id);
        if(query.exec(sql)){
            QMessageBox::information(this, "修改成功", "修改成功！");
        }
        else{
            QMessageBox::warning(this, "修改失败", "修改失败！");
        }
    }
    else
    {
        QMessageBox::warning(NULL,"修改失败","不能为空！");
    }

}
