#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //create the model
    fileName = "C:/Private/test/BlamdownLeaderBoard.bdlb";
    model = new QStandardItemModel(1,0,this);

    ReadFile();

    ui->treeView->setModel(model);
}

Dialog::~Dialog()
{
    delete ui;
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

    int elementCount =  document.elementsByTagName("score").count();
    qDebug() << "elemnt count: " << elementCount;

    int* scoreList = new int[elementCount];
    int temp = 0;
    int elementId;
    QDomNode winner;
    QDomNode scoreNode;

    for (int x = 0; x < elementCount; ++x) {
        scoreNode = document.elementsByTagName("score").at(x).firstChild();

        scoreList[x] = scoreNode.nodeValue().toInt();
        qDebug() << scoreList[x];

        if (scoreList[x] >  temp ) {
            temp = scoreList[x];
            qDebug() << "new big value: " << temp;
            elementId = x;

           winner = scoreNode;
        }
    }

   qDebug() << elementId << " highscore";
   qDebug() << GetNodeValue("playerName", elementId) << " winner name";
   qDebug() << GetNodeValue("score", elementId) << " winner name";
   qDebug() << GetNodeValue("time", elementId) << " winner name";
   qDebug() << GetNodeValue("chaos", elementId) << " winner name";


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

QString Dialog::GetNodeValue(QString element, int i)
{
    return document.elementsByTagName(element).at(i).firstChild().nodeValue();
}

void Dialog::on_pushButton_clicked()
{
    //Save document
    WriteFile();
}
