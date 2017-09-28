#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frmAbout = new about(this);

    QFile dataFile(":/files/DATA.dat");
    detector = new Core(&dataFile);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose a file");

    if(path.isEmpty())
        return;

    ui->txtBrowse->setText(path);

    on_btnAnalyze_clicked();
}

void MainWindow::on_btnAnalyze_clicked()
{
    QString path = ui->txtBrowse->text();

    if(path.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter a valid address.");
        return;
    }

    ui->progressBar->setValue(0);

    QVector<QStringList> output;
    QFile file(path);

    ui->progressBar->setValue(10);

    detector->detect(&file, output);

    ui->progressBar->setValue(70);

    model = new QStandardItemModel(output.size(), 4, this);

    QStringList header;
    header << "Extension" << "Description" << "Point" << "Match";

    model->setHorizontalHeaderLabels(header);

    for(int row = 0; row < output.size(); row++)
    {
        for(int col = 0; col < 4; col++)
        {
            QStandardItem *item = new QStandardItem(output[row][col]);
            model->setItem(row, col, item);
        }
    }

    ui->progressBar->setValue(100);

    ui->tblResult->setModel(model);
}

void MainWindow::on_btnImage_clicked()
{

    frmAbout->show();
}
