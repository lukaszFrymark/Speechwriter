#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createListModelView();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createListModelView()
{
    model = new CustomListModel;
    ui->listView->setModel( model );

//    QStringList strList;
//    strList << "monitor" << "mouse" << "keyboard" << "hard disk drive"
//            << "graphic card" << "sound card" << "memory" << "motherboard";
//    model->setStringList(strList);
}

void MainWindow::on_addTextButton_clicked()
{
    GetEditText();
}

void MainWindow::GetEditText()
{
//    QMessageBox::information(
//        this,
//        tr("Application Name"),
//        ui->sentenceEdit->toPlainText() );

    model->insertRow(model->rowCount());
    QModelIndex index = model->index(model->rowCount()-1);
    model->setData(index, ui->sentenceEdit->toPlainText());

    ui->sentenceEdit->clear();
}

void MainWindow::on_removeTextButton_clicked()
{
    model->removeRow( ui->listView->currentIndex().row() );
}

void MainWindow::on_importButton_clicked()
{
    model->removeRows(0, model->rowCount());
    model->checkedItems.clear();
    QString fileName = QFileDialog::getOpenFileName(this, ("Open File"),
                                                      "",
                                                      ("Text files (*.txt)"));
    QFileInfo fi( fileName );
    if( fi.exists() && fi.isFile() )
    {
        model->import(fileName);
    }
}

