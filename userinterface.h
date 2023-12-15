#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include <QWidget>
#include <QtSql/QSqlTableModel>
namespace Ui {
class userInterface;
}
class userInterface : public QWidget
{
    Q_OBJECT
public:
    explicit userInterface(QWidget *parent = 0);
    ~userInterface();
//private slots:
//    void on_myinformationButton_clicked();
//    void on_searchButton_clicked();
//    void on_search_exist_Button_clicked();
//    void on_showAllButton_clicked();
//void on_tableView_doubleClicked(const QModelIndex &index);



//void on_exitButton_clicked();

private slots:
    void on_myinformationButton_clicked();

    void on_exitSearchButton_clicked();

    void on_searchButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::userInterface *ui;
    QSqlTableModel *model;

};
#endif // USERINTERFACE_H
