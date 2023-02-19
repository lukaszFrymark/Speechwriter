#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customlistmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addTextButton_clicked();

    void on_removeTextButton_clicked();

    void on_importButton_clicked();

    void on_exportButton_clicked();

private:
    Ui::MainWindow *ui;
    CustomListModel* model;

    void createListModelView();
    void GetEditText();
};
#endif // MAINWINDOW_H
