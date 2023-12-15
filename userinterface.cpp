#include "userinterface.h"
#include "ui_userinterface.h"
#include"personal_info.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QTableView>
#include "userregister.h"
#include <QDateTime>

userInterface::userInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userInterface)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("图书管理系统"));
    model = new QSqlTableModel(this);
    model->setTable("book");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置模式成手动提交，表明需要管理员手动点button才能修改数据库
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);//setVisible(bool visible)函数用于设置垂直表头是否可见
    ui->tableView->setColumnWidth(9,150);


}

userInterface::~userInterface()
{
    delete ui;
}


void userInterface::on_myinformationButton_clicked()
{
    personal_info*w =new personal_info();
    w->show();
}

void userInterface::on_exitSearchButton_clicked()
{
    ui->tableView->setModel(model);
}

void userInterface::on_searchButton_clicked()
{
    QString title=ui->booknameEdit->text();
    QString author=ui->authorEdit->text();
    QString category=ui->categoryEdit->text();
    QString press=ui->publisherEdit->text();
    float lowPrice=ui->lowPriceEdit->text().toFloat();
    float highPrice=ui->highPriceEdit->text().toFloat();
    int earlyYear=ui->earlyYearEdit->text().toInt();
    int lateYear=ui->lateYearEdit->text().toInt();
    QString sql = "SELECT * FROM book WHERE 1=1"; // 1=1是一个无意义的条件，但是可以用于拼接查询条件
    QList<QVariant> params; // 用于保存查询条件的列表

    if (!author.isEmpty()) {
        sql += " AND author = ?";
        params.append(author);
    }

    if (!title.isEmpty()) {
        sql += " AND title LIKE ?";
        params.append("%" + title + "%");
    }

    if (!category.isEmpty()) {
        sql += " AND category = ?";
        params.append(category);
    }

    if (!press.isEmpty()) {
        sql += " AND press LIKE ?";
        params.append("%" + press + "%");
    }
     //两者要么同时有，要么同时没有
    if (earlyYear!= 0&&lateYear!=0) {
        if(lateYear>=earlyYear){
            sql += " AND (publish_year BETWEEN ? AND ?)";
            params.append(earlyYear);
            params.append(lateYear);
        }
        else{
            QMessageBox::warning(this, tr("条件无效"),
                                 tr("条件无效！"));
        }

    }
    else if(earlyYear!=0){
        sql+= " AND publish_year > ?";//一定要记得！开头是空格！
        params.append(earlyYear);
    }
    else if(lateYear!=0){
        sql+=" AND publish_year < ?";
        params.append(lateYear);
    }


    if(lowPrice!=0.0&&highPrice!=0.0){
        if(highPrice>=lowPrice){
            sql += " AND (price BETWEEN ? AND ?)";
            params.append(lowPrice);
            params.append(highPrice);
        }
        else{
            QMessageBox::warning(this, tr("条件无效"),
                                 tr("条件无效！"));
        }
    }
    else if(lowPrice!=0.0){
        sql+=" AND price > ?";
        params.append(lowPrice);
    }
    else if(highPrice!=0.0){
        sql+=" AND price < ?";
        params.append(highPrice);
    }


    QSqlQuery query;
    query.prepare(sql);

    for (int i = 0; i < params.size(); i++) {
        query.addBindValue(params.at(i));
    }
    query.exec();
    QSqlQueryModel *t_model = new QSqlQueryModel;
    t_model->setQuery(query);
    if (t_model->lastError().isValid()) {
        qDebug() << "查询出错 " << t_model->lastError().text();
    }
    else {
        ui->tableView->setModel(t_model);
    }

}

void userInterface::on_exitButton_clicked()
{
    if (!(QMessageBox::information(this,tr("退出"),tr("确定退出吗?"),tr("Yes"),tr("No"))))
    {
        this->close();
    }
}
