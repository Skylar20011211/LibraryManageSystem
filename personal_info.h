#ifndef PERSONAL_INFO_H
#define PERSONAL_INFO_H

#include <QWidget>
#include <QtSql/QSqlTableModel>
#include <QButtonGroup>

namespace Ui {
class personal_info;
}

class personal_info : public QWidget
{
    Q_OBJECT

public:
    explicit personal_info(QWidget *parent = nullptr);
    ~personal_info();

private slots:
    void on_borrowButton_clicked();

    void on_returnButton_clicked();

    void on_quitButton_clicked();

    void on_update_infor_Button_clicked();

private:
    Ui::personal_info *ui;
    QSqlTableModel *model;
//    QButtonGroup *BG;
    void setTitle();
};

#endif // PERSONAL_INFO_H
