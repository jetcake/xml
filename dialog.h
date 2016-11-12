#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtXml>
#include <QtDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    QStandardItemModel *model;
    QString fileName;
    QDomDocument document;
    void ReadFile();
    void WriteFile();
    void LoadXMLFile();
    void ReadLeaderboard();
    void AddEntryToModel(int);
    QString GetNodeValue(QString, int);
    int winnerCount;
};

#endif // DIALOG_H
