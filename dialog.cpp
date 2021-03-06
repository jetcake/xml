#include "dialog.h"
#include "ui_dialog.h"
#include "explorer.h"
#include <QMenu>
#include <QMessageBox>
#include <list>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QStringList locations2 = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);

    foreach (QString var1, locations2) {
        filePath  = var1;
    }

    filePath += "/AppData/LocalLow/Guru Games/";
    fileName = "BlamdownLeaderBoard.bdlb";




    //create the model
    //fileName = "C:/Private/test/BlamdownLeaderBoard.bdlb";
    model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

    winnerCount = 10;

    ReadLeaderboard();
    TableSetup();

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));

    connect(&watcher, SIGNAL(fileChanged(QString)),
             this, SLOT(on_file_updated()));



    //TODO:     Open explorer when starting app






    QImage image;
    bool result = image.load( ":/Data/test.jpg" );

     qDebug() << result << " is img loaded";

     qWarning() << QFile::exists(":/Data/test.jpg");

     QFileInfo fi(":/Data/test.jpg");
     qWarning() << " permis " << fi.permissions();
}

void Dialog::TableSetup()
{
    qDebug() << "Table setup";

    //Set Header Label Texts Here
    model->setHorizontalHeaderLabels(QString("NAME;SCORE;TIME;CHAOS").split(";"));

    if (isTableStylesheet == false) {
        QFont font("Arial", 30, QFont::Bold);
        ui->tableView->horizontalHeader()->setFont( font );
        ui->tableView->setStyleSheet("QHeaderView {font: 24pt; color: blue; border: 3px solid; }");
        ui->tableView->setStyleSheet("font: 30pt bold;"
                                     " color: white;"
                                     " background-color:  qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1,stop: 0 #7892BB, stop: 1 black);"
                                     " selection-background-color: blue;"
                                     "  border-style: inset;"
                                     "  border-width: 10px;   "
                                     "  border-radius: 5px; "
                                     "  border-color: black; "
                                     "  spacing: 10px; "
                                     "  min-width: 10em;     "
                                     "  padding: 50px;");
        isTableStylesheet = true;
    }

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

   // ui->tableView->setStyleSheet( "background-image: url( :/Data/Img_background.png )" );
    //  " background-color: rgb(125,125,64);"
}

void Dialog::OpenMessageBox()
{
   QMessageBox msgBox;
    msgBox.setText("No Leaderboard file");

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Error", "No leaderboard file was found", QMessageBox::Ok|QMessageBox::Abort);

    if (reply == QMessageBox::Ok) {
        qDebug() << "Yes Was clicked";
        QApplication::quit();
    }
    else {
         qDebug() << "Yes Was !!NOT!! clicked";
         on_exit_clicked();
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::ShowContextMenu(const QPoint &pos)
{
    qDebug() << " QPos: " << pos ;

    QAction *newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&New"), this);
    newAct->setStatusTip(tr("new sth"));
    connect(newAct, SIGNAL(triggered()),
            this, SLOT(on_new_clicked()));

    QAction *openAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Open"), this);
    openAct->setStatusTip(tr("open sth"));
    connect(openAct, SIGNAL(triggered()),
            this, SLOT(on_open_clicked()));

    QAction *saveAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Save"), this);
    saveAct->setStatusTip(tr("save sth"));
    connect(saveAct, SIGNAL(triggered()),
            this, SLOT(on_save_clicked()));

    QAction *exitAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Exit"), this);
    exitAct->setStatusTip(tr("exit sth"));
    connect(exitAct, SIGNAL(triggered()),
            this, SLOT(on_exit_clicked()));

    QMenu menu(this);
    menu.addAction(newAct);
    menu.addAction(openAct);
    menu.addAction(saveAct);
    menu.addAction(exitAct);

    QPoint pt(pos);
    menu.exec( QCursor::pos() );
}

void Dialog::on_new_clicked()
{
    qDebug() << "Clicked on new! " << fileName;
    QString dayOfWeek = QDate::currentDate().dayOfWeek();
    QDate date(QDate::currentDate());


    QFile file(filePath + fileName);
    qDebug() << "copying new file: " << filePath + fileName << " to : " << filePath + date.toString("dddd") + fileName;

    if (file.copy(filePath + date.toString("dddd") + fileName)) qDebug() << "file copying sucsecsfull";
    else{
        qDebug() << "file already exist";
        return;
    }

    file.remove();


    // TODO:    Reset table function
    model->clear();
    ReadLeaderboard();
    TableSetup();

}

void Dialog::on_open_clicked()
{
    qDebug() << "Clicked on Open! ";

    Explorer e(this);
    //Connect Explorer signal and dialog slot
    connect(&e, SIGNAL(FileChoosed(QString)), this, SLOT(on_file_choosed(QString)));

    e.exec();
}

void Dialog::on_save_clicked()
{
    //Change to export email.
     qDebug() << "Clicked on save! ";

}

void Dialog::on_exit_clicked()
{
     qDebug() << "Clicked on exit! ";
     QApplication::quit();
}

void Dialog::on_file_choosed(QString path)
{
    //FIXME:    Need to recheck path
    QFileInfo fi(path);
    fileName = fi.fileName();
    filePath = fi.dir().path() + "/";

    qDebug() << "File path recived: " << fi.dir().path() << " fileName:  " << fi.fileName();
    qDebug() << "watcher.addPath: " << path;

    model->clear();
    ReadLeaderboard();
    TableSetup();
}

void Dialog::on_file_updated()
{
    qDebug() << "Leaderboard File updated";

    model->clear();
    ReadLeaderboard();
    TableSetup();
}

void Dialog::ReadFile()
{
    QStandardItem *root = new QStandardItem("Books");
    model->appendRow(root);


    //Load XML file
    QFile file(filePath + fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bool isO = document.setContent(&file);
        file.close();
        qDebug() << isO << " is Document open ";
    }

    //get XML root element
    QDomElement xmlRoot = document.firstChildElement();

    //read the books
    QDomNodeList books = xmlRoot.elementsByTagName("Book");
    qDebug() << "hbellp" << books.count();


    for(int i = 0; i < books.count(); i++){

        QDomElement book = books.at(i).toElement();
        QStandardItem *item = new QStandardItem(book.attribute("Name"));

        //Read chapters of the book
        QDomNodeList chapters = book.elementsByTagName("Chapter");
        for(int h = 0; h < chapters.count(); h++){
            QDomElement chapter = chapters.at(h).toElement();
            QStandardItem *chapterItem = new QStandardItem(chapter.attribute("Name"));

            item->appendRow(chapterItem);
        }
        root->appendRow(item);
    }
}

void Dialog::WriteFile()
{
    QDomDocument document;

    //Make a root node
    QDomElement xmlRoot = document.createElement("Books");
    document.appendChild(xmlRoot);

    QStandardItem *root = model->item(0,0);

    for(int i = 0; i < root->rowCount(); i++){
        QStandardItem *book = root->child(i,0);

        QDomElement xmlBook = document.createElement("Book");
        xmlBook.setAttribute("Name", book->text());
        xmlBook.setAttribute("ID", i);
        xmlRoot.appendChild(xmlBook);

        for(int h = 0; h < book->rowCount(); h++){
            QStandardItem *chapter = book->child(h,0);

            QDomElement xmlChapter = document.createElement("Chapter");
            xmlChapter.setAttribute("Name", chapter->text());
            xmlChapter.setAttribute("ID", h);
            xmlRoot.appendChild(xmlChapter);
        }
    }

    //Save to disk
    QFile file(filePath + fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Failed to write file";
    }

    QTextStream stream(&file);
    stream << document.toString();
    file.close();
    qDebug() << "Save filed";

}

//TODO:     Remove parameter
bool Dialog::LoadXMLFile(QDomDocument doc)
{
     bool isO = false;

    //Load XML file
    QFile file(filePath + fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        isO = document.setContent(&file);
        file.close();

        file.open(QIODevice::ReadOnly | QIODevice::Text);
        bool isC = tempDoc.setContent(&file);

        qDebug() << isO << " is Document open ";
        qDebug() << isC << " is CLONE Document open ";

        //File watcher looking when xml updates by other applications
        watcher.addPath(filePath + fileName);
    }
    else {
         qDebug() << "file dosent exist: " << fileName;
         OpenMessageBox();
         isO = false;
    }
    return isO;
}

void Dialog::ReadLeaderboard()
{
    // TODO:   Dont repeat last entry

    if (!LoadXMLFile(document)) return;

    int elementCount =  document.elementsByTagName("score").count();
    qDebug() << "elemnt count: " << elementCount;

    int* scoreList = new int[elementCount];

    int elementId;
    QDomNode winner;
    QDomNode scoreNode;
    QList<int> winnerList;

    QDomNodeList entryList = tempDoc.elementsByTagName("score");

    //Sorting entries by higest score, and adding them to view
    while (winnerList.length() < winnerCount && winnerList.length() < elementCount) {
        int temp = 0;

        for (int x = 0; x < elementCount; ++x) {
            scoreNode = entryList.at(x).firstChild();

            scoreList[x] = scoreNode.nodeValue().toInt();

            if (scoreList[x] >  temp ) {

                temp = scoreList[x];
                elementId = x;

                winner = scoreNode;
            }
        }
        winnerList.append(elementId);

        //Clears entry from list
        scoreList[elementId] = 0;

        winner.setNodeValue("0");
        AddEntryToModel(elementId);
    }

    //Adding to model
      foreach (int winnerID, winnerList) {
          qDebug() << "Adding entry to view: "<< winnerID << " " << GetNodeValue("playerName", winnerID);
      }

}

void Dialog::AddEntryToModel(int elementId)
{
    qDebug() << "addinge entry to model. ID:  " << elementId;
    QStandardItem *root = new QStandardItem(GetNodeValue("playerName", elementId));
    QStandardItem *scoreItem = new QStandardItem(GetNodeValue("score", elementId));
    QStandardItem *timeItem = new QStandardItem(GetNodeValue("time", elementId));
    QStandardItem *chaosItem = new QStandardItem(GetNodeValue("chaos", elementId));

    QList<QStandardItem*> *items = new QList<QStandardItem*>();
    items->append(root);
    items->append(scoreItem);
    items->append(timeItem);
    items->append(chaosItem);

    model->appendRow(*items);
}

QString Dialog::GetNodeValue(QString element, int i)
{
    return document.elementsByTagName(element).at(i).firstChild().nodeValue();
}


