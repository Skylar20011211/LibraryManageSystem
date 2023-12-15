#include "login.h"
#include "ui_login.h"
#include<QSqlQuery>
#include<mainwindow.h>
#include"userinterface.h"
#include"QMessageBox"
#include"userregister.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{

    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("登录"));
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

login::~login()
{
    delete ui;
}
int login::card_id;
QString login::name;//这两行很重要！不添加编译会报错

void login::on_loginButton_clicked()
{

    login::name=ui->lineEdit->text();
    QString password=ui->lineEdit_2->text();
    QSqlQuery query;
    if(login::name!=NULL&&password.length()!=0){
        QString instruction=QString("select card_id, pw from card where name='%1'").arg(login::name);
        query.exec(instruction);
        while(query.next())//其实只循环一次
        {
            if(login::name=="root"&&password=="123456"){//可以自己更改管理员的账号和密码
                this->close();
                MainWindow* w=new MainWindow();
                w->show();
                break;
            }
            else if(login::name!="root"&&query.value(1).toString()==password){
                login::card_id=query.value(0).toInt();
                this->close();
                userInterface* w2=new userInterface();
                w2->show();
                break;
            }
            else{
                QMessageBox::information(this, "登录失败", "用户名或者密码错误");
                break;
            }
        }
    }
    else{
        QMessageBox::warning(this,"登录失败","不能为空！");
    }

}
void login::on_RegisterButton_clicked()
{
    UserRegister *w =new UserRegister();
    w->show();
}
void login::on_exitButton_clicked()
{
    if (!(QMessageBox::information(this,tr("退出"),tr("确定退出吗?"),tr("Yes"),tr("No"))))
    {
        this->close();
    }
}



