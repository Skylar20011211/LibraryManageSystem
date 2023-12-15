#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QTableView>
#include "userregister.h"
#include <QDateTime>
#include<QFile>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("图书管理系统"));
    model = new QSqlTableModel(this);
    model->setTable("book");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置模式成手动提交，表明需要管理员手动点button才能修改数据库
    model->select();//使用 select() 函数可以重新从数据库中获取最新的数据，并更新模型中的数据
    model->database().transaction();

    model2 = new QSqlTableModel(this);
    model2->setTable("card");
    model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model2->select();

    setTitle();

    BG=new QButtonGroup(this);
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);//setVisible(bool visible)函数用于设置垂直表头是否可见
    ui->tableView->setColumnWidth(9,150);

    ui->tableView_2->setModel(model2);
    ui->tableView_2->verticalHeader()->setVisible(false);


}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setTitle()
{
    model->setHeaderData(0,Qt::Horizontal, "book_id");
    model->setHeaderData(1,Qt::Horizontal, "category");
    model->setHeaderData(2,Qt::Horizontal, "title");
    model->setHeaderData(3,Qt::Horizontal, "press");
    model->setHeaderData(4,Qt::Horizontal, "publish_year");
    model->setHeaderData(5,Qt::Horizontal, "author");
    model->setHeaderData(6,Qt::Horizontal, "price");
    model->setHeaderData(7,Qt::Horizontal, "stock");
    model->setHeaderData(8,Qt::Horizontal, "total");
    model->setHeaderData(9,Qt::Horizontal, "ISBN");

    model2->setHeaderData(0,Qt::Horizontal, "card_id");
    model2->setHeaderData(1,Qt::Horizontal, "name");
    model2->setHeaderData(2,Qt::Horizontal, "department");
    model2->setHeaderData(3,Qt::Horizontal, "type");
    model2->setHeaderData(4,Qt::Horizontal, "pw");

}




void MainWindow::on_addButton_clicked()
{
    int rowNum = model->rowCount();
    model->insertRows(rowNum,1);//在指定的位置rowNum插入1行数据
    model->setData(model->index(rowNum,0),rowNum+1);//id不需要管理员手动添加，显示界面中自动增加
}


void MainWindow::on_deleteButton_clicked()
{
    int curRow = ui->tableView->currentIndex().row();
    model->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("删除当前行!"),
                                  tr("你确定删除当前行吗？ "),QMessageBox::Yes, QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        model->revertAll();//用于撤销所有未提交的修改
    } else {
        if(model->submitAll()){
            QMessageBox::information(this, "删除成功","删除成功");
        }
        else{
            QMessageBox::warning(this, tr("数据库错误"),tr("数据库错误!"));
        }
    }

}

void MainWindow::on_updateButton_clicked()
{
    if(model->submitAll()){
        model->database().commit();
        QMessageBox::information(NULL,"修改成功","修改成功！");
    }
    else{
        model->database().rollback();
        QMessageBox::warning(this, tr("数据库错误"),
                             tr("数据库错误!"));
    }
}



void MainWindow::on_exitButton_clicked()
{
    if (!(QMessageBox::information(this,tr("退出"),tr("确定退出吗?"),tr("Yes"),tr("No"))))
    {
        this->close();
    }
}



void MainWindow::on_searchButton_clicked()
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

void MainWindow::on_exitSearchButton_clicked()
{
    ui->tableView->setModel(model);

}

void MainWindow::on_searchButton_2_clicked()
{

    QString name=ui->lineEdit_10->text();
    QString department=ui->lineEdit_9->text();

    QString sql = "SELECT * FROM card WHERE 1=1"; // 1=1是一个无意义的条件，但是可以用于拼接查询条件
    QList<QVariant> params; // 用于保存查询条件的列表

    if (!name.isEmpty()) {
        sql += " AND name = ?";
        params.append(name);
    }

    if (!department.isEmpty()) {
        sql += " AND department LIKE ?";
        params.append("%" + department + "%");
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
        ui->tableView_2->setModel(t_model);
    }


}

void MainWindow::on_exitSearchButton_2_clicked()
{
    model2->select();
     ui->tableView_2->setModel(model2);
}

void MainWindow::on_addButton2_clicked()
{
    int rowNum = model2->rowCount();
    model2->insertRows(rowNum,1);//在指定的位置rowNum插入1行数据
    model2->setData(model->index(rowNum,0),rowNum+1);//card_id不需要管理员手动添加，显示界面中自动增加
}

void MainWindow::on_updateButton2_clicked()
{
    if(model2->submitAll()){
        model2->database().commit();
        QMessageBox::information(NULL,"修改成功","修改成功！");
    }
    else{
        model2->database().rollback();
        QMessageBox::warning(this, tr("数据库错误"),
                             tr("数据库错误!"));
    }
}

void MainWindow::on_deleteButton2_clicked()
{

    int curRow = ui->tableView_2->currentIndex().row();
    model2->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("删除当前行!"),
                                  tr("你确定删除当前行吗？ "),QMessageBox::Yes, QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        model2->revertAll();//用于撤销所有未提交的修改
    } else {
        if(model2->submitAll()){
            QMessageBox::information(this, "删除成功","删除成功");
        }
        else{
            QMessageBox::warning(this, tr("数据库错误"),tr("数据库错误!"));
        }
    }
}

void MainWindow::on_addFromFile_clicked()
{
    QFile file("/Users/skylar/Desktop/booklist.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) { // 以只读文本模式打开文件
        QTextStream in(&file);
        QSqlQuery query;
        query.prepare("INSERT INTO book (category,title,press,publish_year,author,price,stock,total,ISBN,book_id) VALUES (?,?,?,?,?,?,?,?,?,?)");
        int num=model->rowCount();
        num++;
        while (!in.atEnd()) { // 循环读取每一行数据
            QString line = in.readLine(); // 读取一行数据
            QStringList fields = line.split(",");
            query.bindValue(0, fields[0]);
            query.bindValue(1, fields[1]);
            query.bindValue(2, fields[2]);
            query.bindValue(3, fields[3].toInt());
            query.bindValue(4, fields[4]);
            query.bindValue(5, fields[5].toFloat());
            query.bindValue(6, fields[6].toInt());
            query.bindValue(7, fields[7].toInt());
            query.bindValue(8, fields[8]);
            query.bindValue(9,num );
            query.exec();
            num++;
            model->database().commit();
            model->select();

        }
        file.close(); // 关闭文件
    }

}
