#include "dialog.h"
#include <QApplication>
#include <QtGui>
#include <bits/stdc++.h>
#include <QList>

QVBoxLayout* mainLayout;
QHBoxLayout* buttonLayout;
QGridLayout* mejaLayout;
QList<QLabel*> tempat_duduk;
QList<QString> gents,lads;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("Tempat Duduk v1.1");
    this->resize(1130,500);

    mainLayout=new QVBoxLayout(this);
    buttonLayout=new QHBoxLayout(this);
    mejaLayout=new QGridLayout(this);
    QLabel* meja;
    QSpacerItem* space;

    QPalette palette;

    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);

    QBrush brush1(QColor(220, 160, 100, 255));
    brush1.setStyle(Qt::SolidPattern);

    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);

    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);

    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    font.setFamily("Open Sans");

    for(int i=0;i<2*4;i+=2)
        for(int j=0;j<3*4;j+=3){
            for(int k=0;k<2;++k){
                meja=new QLabel(this);
                meja->setPalette(palette);
                meja->setMinimumSize(18,90);
                meja->setFont(font);
                meja->setAutoFillBackground(true);
                meja->setText("-");
                meja->setAlignment(Qt::AlignCenter);
                mejaLayout->addWidget(meja,i,j+k);
                tempat_duduk.push_back(meja);
            }
            space=new QSpacerItem(10,0);
            if(j<(3*4-1))mejaLayout->addItem(space,i,j+1);
            space=new QSpacerItem(0,10);
            if(i<(2*4-1))mejaLayout->addItem(space,i+1,j,1,3);
        }

    for(int i=0;i<3;++i){
        meja=new QLabel(this);
        meja->setText("-");
        meja->setPalette(palette);
        meja->setMinimumSize(18,90);
        meja->setFont(font);
        meja->setAutoFillBackground(true);
        meja->setAlignment(Qt::AlignCenter);
        tempat_duduk.push_back(meja);
        mejaLayout->addWidget(meja,2*4,4+i);
    }

    QPushButton* btnRandomize=new QPushButton("Acak!",this);
    QPushButton* btnRandomizeCombine=new QPushButton("Acak! (campur)",this);
    QPushButton* btnExit=new QPushButton("Exit",this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnRandomize);
    buttonLayout->addWidget(btnRandomizeCombine);
    buttonLayout->addWidget(btnExit);

    connect(btnExit,SIGNAL(clicked()),this,SLOT(accept()));
    connect(btnRandomize,SIGNAL(clicked()),this,SLOT(acakv2()));
    connect(btnRandomizeCombine,SIGNAL(clicked()),this,SLOT(acak_campur_animated()));


    mainLayout->addLayout(mejaLayout);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);

    //get data
    QString s;
    QFile db1(":/database/male.txt"),db2(":/database/female.txt");
    if(!db1.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", db1.errorString());
    }
    if(!db2.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", db2.errorString());
    }

    QTextStream in1(&db1);
    while(!in1.atEnd()) {
        s = in1.readLine();
        if(s.size()>1)gents.push_back(s);
    }
    db1.close();

    QTextStream in2(&db2);
    while(!in2.atEnd()) {
        s = in2.readLine();
        if(s.size()>1)lads.push_back(s);
    }
    db2.close();

    srand(time(NULL));

}

Dialog::~Dialog()
{

}


void Dialog::acakv2(){
    QTime dieTime;
    for(int i=0;i<10;++i){
        acak();
        //delay 50 ms
        dieTime=QTime::currentTime().addMSecs(50);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void Dialog::acak(){
    //shuffle
    std::random_shuffle(gents.begin(),gents.end());
    std::random_shuffle(lads.begin(),lads.end());

    //set text
    int gcount=0,lcount=0,mcount=0;
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            if(i==3 && j==2){
                tempat_duduk[mcount++]->setText(gents[gcount++]);
                tempat_duduk[mcount++]->setText(gents[gcount++]);
                continue;
            } else if((i+j)&1){
                tempat_duduk[mcount++]->setText(lads[lcount++]);
                tempat_duduk[mcount++]->setText(lads[lcount++]);
            } else {
                tempat_duduk[mcount++]->setText(gents[gcount++]);
                tempat_duduk[mcount++]->setText(gents[gcount++]);
            }
        }
    }
    for(int i=lads.size()-3;i<lads.size();++i)
        tempat_duduk[mcount++]->setText(lads[i]);
}

void Dialog::acak_campur_animated(){
    QTime dieTime;
    for(int i=0;i<10;++i){
        acak_campur();
        //delay 50 ms
        dieTime=QTime::currentTime().addMSecs(50);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void Dialog::acak_campur(){
    //shuffle
    std::random_shuffle(gents.begin(),gents.end());
    std::random_shuffle(lads.begin(),lads.end());

    //set text
    int gcount=0,lcount=0,mcount=0;
    for(int i=0;i<tempat_duduk.size();++i){
        if((i&1)==0 && gcount<gents.size())
            tempat_duduk[mcount++]->setText(gents[gcount++]);
        else
            tempat_duduk[mcount++]->setText(lads[lcount++]);
    }
}
