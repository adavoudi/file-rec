#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QStandardItem>
#include <QFileDialog>
#include <QMessageBox>

#include "Core.h"
#include "about.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnBrowse_clicked();

    void on_btnAnalyze_clicked();

    void on_btnImage_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    Core *detector;
    about *frmAbout;
};

#endif // MAINWINDOW_H
