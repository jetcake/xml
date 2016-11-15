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
    void ShowContextMenu(const QPoint &pos);
    void on_new_clicked();
    void on_open_clicked();
    void on_save_clicked();
    void on_exit_clicked();
    void on_file_choosed(QString);

private:
    Ui::Dialog *ui;
    QStandardItemModel *model;
    QString fileName;
    QDomDocument document;
    QDomDocument tempDoc;
    QFileSystemWatcher watcher;
    void ReadFile();
    void WriteFile();
    void LoadXMLFile(QDomDocument);
    void ReadLeaderboard();
    void AddEntryToModel(int);
    void TableSetup();

    QString GetNodeValue(QString, int);
    int winnerCount;
    bool isTableStylesheet;
};

#endif // DIALOG_H
