#ifndef LOGIN_H
#define LOGIN_H
#include <QWidget>
namespace Ui {
class login;
}
class login : public QWidget
{
    Q_OBJECT
public:
    explicit login(QWidget *parent = 0);
    static QString name;
    static int card_id;


    ~login();
private slots:
    void on_loginButton_clicked();
    void on_RegisterButton_clicked();
    void on_exitButton_clicked();


private:
    Ui::login *ui;
};
#endif // LOGIN_H
