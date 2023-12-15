#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSql/QSqlTableModel>
#include <QSqlQuery>
#include <QItemDelegate>
#include<QButtonGroup>
namespace Ui {
class MainWindow;
//class ReadOnlyDelegate;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();

    void on_updateButton_clicked();



    void on_exitButton_clicked();


    void on_searchButton_clicked();

    void on_exitSearchButton_clicked();



    void on_searchButton_2_clicked();

    void on_exitSearchButton_2_clicked();

    void on_addButton2_clicked();

    void on_updateButton2_clicked();

    void on_deleteButton2_clicked();

    void on_addFromFile_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QSqlTableModel *model2;
    QButtonGroup *BG;

    void setTitle();
};
//class ReadOnlyDelegate: public QItemDelegate
//{
//public:
//    ReadOnlyDelegate(QWidget *parent = NULL):QItemDelegate(parent)
//    {}
//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override //final
//    {
//        Q_UNUSED(parent)
//        Q_UNUSED(option)
//        Q_UNUSED(index)
//        return NULL;
//    }
//};
#endif // MAINWINDOW_H
