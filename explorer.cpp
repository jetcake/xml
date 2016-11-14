#include "explorer.h"
#include "ui_explorer.h"

Explorer::Explorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Explorer)
{

    ui->setupUi(this);

    QString sPath = "C:/Private/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(sPath);
    ui->treeView->setModel(dirmodel);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    filemodel->setRootPath(sPath);
    ui->listView->setModel(filemodel);

}

Explorer::~Explorer()
{
    delete ui;
}

QString FileChoosed(QString path){

    qDebug() << "File Choosed: " + path;
    return path;
}

void Explorer::on_treeView_clicked(const QModelIndex &index)
{

    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));
}

void Explorer::on_listView_doubleClicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    qDebug() << "Open File: " + sPath;

    FileChoosed(sPath);

    this->close();
    //Opens file with default application
   // QDesktopServices::openUrl(QUrl::fromLocalFile(filemodel->filePath(index)));
}
