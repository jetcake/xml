#ifndef EXPLORER_H
#define EXPLORER_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>

namespace Ui {
class Explorer;
}

class Explorer : public QDialog
{
    Q_OBJECT

public:
    explicit Explorer(QWidget *parent = 0);
    ~Explorer();

signals:
    QString FileChoosed(QString);

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::Explorer *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
};

#endif // EXPLORER_H
