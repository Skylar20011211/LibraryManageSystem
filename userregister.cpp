#include "userregister.h"
#include "ui_userregister.h"
#include <QMessageBox>
#include<QtDebug>
UserRegister::UserRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegister)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("注册"));
     BG=new QButtonGroup(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_7->setEchoMode(QLineEdit::Password);
}
UserRegister::~UserRegister()
{
    delete ui;
}
void UserRegister::on_RegisterResultButton_clicked()
{

    QString password=ui->lineEdit_2->text();
    QString password2=ui->lineEdit_7->text();
    QString name=ui->lineEdit_3->text();
    QString type;
    QString department=ui->comboBox->currentText();
    QSqlQuery query;
    QString n;
    BG->addButton(ui->radioButton_3,2);
    BG->addButton(ui->radioButton_4,3);
    switch (BG->checkedId()) {
    case 2:
        type="T";
        break;
    case 3:
        type="S";
        break;
    }

   if(QString::compare(password,password2)!=0){
        QMessageBox::warning(NULL,"注册失败","两次密码输入不相同！");
   }
   else{
        QString InsertStr=QString("insert into card(type,pw,name,department)"
                                  "values('%1','%2','%3','%4')").arg(type)
                        .arg(password).arg(name).arg(department);
                qDebug()<<InsertStr;
                if(type!=NULL&&password!=NULL&&name!=NULL&&department!=NULL)
                {
                    if(isonly(name))
                    {
                        if(query.exec(InsertStr)){
                            QMessageBox::information(NULL,"注册成功","注册成功！",QMessageBox::Ok);
                            this->close();
                        }
                        else
                            QMessageBox::warning(NULL,"注册失败","注册失败，请稍后重试！");
                    }
                    else
                    {
                        QMessageBox::warning(NULL,"注册失败","该用户名已存在！");
                    }
                }
                else
                {
                    QMessageBox::warning(NULL,"注册失败","信息不能为空！");
                }
     }


}


bool UserRegister::isonly(QString str)
{
    QString sql=QString("select * from users where name='%1'").arg(str);
    QString n;
    QSqlQuery query;
    bool result;
    query.exec(sql);
    while(query.next())
    {
        n=query.value(1).toString();
    }
    if(n==NULL)
        result=true;
    else
        result=false;
    return result;
}
void UserRegister::on_quitButton_clicked()
{
    if (!(QMessageBox::information(this,tr("退出"),tr("确定退出吗?"),tr("Yes"),tr("No"))))
    {
        this->close();
    }
}






