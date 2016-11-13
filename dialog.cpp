#include "dialog.h"
#include "ui_dialog.h"
#include <QMenu>
#include <list>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //create the model
    fileName = "C:/Private/test/BlamdownLeaderBoard.bdlb";
    model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

    winnerCount = 10;

    //ReadFile();

    ReadLeaderboard();
    TableSetup();

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));



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

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::ShowContextMenu(const QPoint &pos)
{
    qDebug() << " QPos: " << pos ;

    QAction *newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&New"), this);
    newAct->setStatusTip(tr("new sth"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(on_new_clicked()));

    QAction *saveAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Save"), this);
    saveAct->setStatusTip(tr("save sth"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(on_save_clicked()));

    QAction *exitAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Exit"), this);
    exitAct->setStatusTip(tr("exit sth"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(on_exit_clicked()));

    QMenu menu(this);
    menu.addAction(newAct);
    menu.addAction(saveAct);
    menu.addAction(exitAct);

    QPoint pt(pos);
    menu.exec( QCursor::pos() );
}

void Dialog::on_new_clicked()
{
    qDebug() << "Clicked on new! ";

    model->clear();
    ReadLeaderboard();
    TableSetup();

}

void Dialog::on_save_clicked()
{
     qDebug() << "Clicked on save! ";

}

void Dialog::on_exit_clicked()
{
     qDebug() << "Clicked on exit! ";
     QApplication::quit();
}


void Dialog::ReadFile()
{
    QStandardItem *root = new QStandardItem("Books");
    model->appendRow(root);


    //Load XML file
    QFile file(fileName);
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
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Failed to write file";
    }

    QTextStream stream(&file);
    stream << document.toString();
    file.close();
    qDebug() << "Save filed";

}

void Dialog::LoadXMLFile(QDomDocument doc)
{
    //Load XML file
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bool isO = document.setContent(&file);
        file.close();

        file.open(QIODevice::ReadOnly | QIODevice::Text);
        bool isC = tempDoc.setContent(&file);

        qDebug() << isO << " is Document open ";
        qDebug() << isC << " is CLONE Document open ";
    }
}

void Dialog::ReadLeaderboard()
{
    LoadXMLFile(document);

    int elementCount =  document.elementsByTagName("score").count();
    qDebug() << "elemnt count: " << elementCount;

    int* scoreList = new int[elementCount];

    int elementId;
    QDomNode winner;
    QDomNode scoreNode;
    QList<int> winnerList;

    //  LoadXMLFile(tempDoc);
    QDomNodeList entryList = tempDoc.elementsByTagName("score");

    while (winnerList.length() < winnerCount) {
        int temp = 0;

        for (int x = 0; x < elementCount; ++x) {
            scoreNode = entryList.at(x).firstChild();

            scoreList[x] = scoreNode.nodeValue().toInt();
            // qDebug() << scoreList[x];

            if (scoreList[x] >  temp ) {
                qDebug() << scoreList[x] << " mellan score och list ";
                temp = scoreList[x];
                elementId = x;

                winner = scoreNode;
                qDebug() << "new big value: " << temp;
            }
        }

        winnerList.append(elementId);
        //Clears entry from list
        scoreList[elementId] = 0;

        winner.setNodeValue("0");
        AddEntryToModel(elementId);
    }

    //Debug
    qDebug() << elementId << " highscore";
    qDebug() << GetNodeValue("playerName", 4) << " winner name";
    qDebug() << GetNodeValue("score", 4) << " winner score";
    qDebug() << GetNodeValue("time", elementId) << " winner name";
    qDebug() << GetNodeValue("chaos", elementId) << " winner name";

    //Adding to model
      foreach (int winnerID, winnerList) {
          qDebug() << winnerID;
      }

       qDebug() << model->item(1,0);
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


