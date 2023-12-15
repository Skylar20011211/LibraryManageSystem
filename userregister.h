#ifndef EDIT_H
#define EDIT_H
#include <QWidget>
#include <QtSql/QSqlTableModel>
#include <QSqlQuery>
#include <QObject>
#include <QButtonGroup>
#include <QComboBox>
namespace Ui {
class UserRegister;
}
class UserRegister : public QWidget
{
    Q_OBJECT
public:
    explicit UserRegister(QWidget *parent = 0);
    static bool isonly(QString str);
    ~UserRegister();
private slots:
    void on_RegisterResultButton_clicked();
    void on_quitButton_clicked();

private:
    Ui::UserRegister *ui;
    QButtonGroup *BG;
};
#endif // EDIT_H
