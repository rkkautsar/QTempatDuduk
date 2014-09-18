#include "dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    init();
}

Dialog::~Dialog()
{

}

//TODO: add custom shuffle, possibly using textfile

void Dialog::shuffle_animated(){
    //lock button
    btnRandomize->setDown(true);
    disconnect(btnRandomize,SIGNAL(clicked()),this,SLOT(shuffle_animated()));

    for(int i=0;i<animation_time;++i){
        shuffle();
        dieTime=QTime::currentTime().addMSecs(per_frame_ms);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::ExcludeSocketNotifiers, per_frame_ms);
    }

    //enable button
    btnRandomize->setDown(false);
    connect(btnRandomize,SIGNAL(clicked()),this,SLOT(shuffle_animated()));
}

void Dialog::shuffle_no_gender_animated(){
    //lock button
    btnRandomizeCombine->setDown(true);
    disconnect(btnRandomizeCombine,SIGNAL(clicked()),this,SLOT(shuffle_no_gender_animated()));

    for(int i=0;i<animation_time;++i){
        shuffle_no_gender();
        dieTime=QTime::currentTime().addMSecs(per_frame_ms);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::ExcludeSocketNotifiers, per_frame_ms);
    }

    //enable button
    btnRandomizeCombine->setDown(false);
    connect(btnRandomizeCombine,SIGNAL(clicked()),this,SLOT(shuffle_no_gender_animated()));
}

void Dialog::shuffle(){
    //shuffle
    std::random_shuffle(gents.begin(),gents.end());
    std::random_shuffle(lads.begin(),lads.end());

    //set text
    int gcount=0,lcount=0,mcount=0;
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            if(i==3 && j==2){
                seating[mcount++]->setText(gents[gcount++]);
                seating[mcount++]->setText(gents[gcount++]);
                continue;
            } else if((i+j)&1){
                seating[mcount++]->setText(lads[lcount++]);
                seating[mcount++]->setText(lads[lcount++]);
            } else {
                seating[mcount++]->setText(gents[gcount++]);
                seating[mcount++]->setText(gents[gcount++]);
            }
        }
    }
    for(int i=lads.size()-3;i<lads.size();++i)
        seating[mcount++]->setText(lads[i]);
}

void Dialog::shuffle_no_gender(){
    //shuffle
    std::random_shuffle(gents.begin(),gents.end());
    std::random_shuffle(lads.begin(),lads.end());

    //set text
    int gcount=0,lcount=0,mcount=0;
    for(int i=0;i<seating.size();++i){
        if((i&1)==0 && gcount<gents.size())
            seating[mcount++]->setText(gents[gcount++]);
        else
            seating[mcount++]->setText(lads[lcount++]);
    }
}

void Dialog::init(){
    this->setWindowTitle("QTempatDuduk v1.2");
    this->resize(1130,500);

    btnRandomize=new QPushButton(tr("Shuffle! (&gender)"),this);
    btnRandomizeCombine=new QPushButton(tr("Shuffle! (&normal)"),this);
    btnExit=new QPushButton(tr("E&xit"),this);

    mainLayout=new QVBoxLayout(this);
    buttonLayout=new QHBoxLayout(this);
    seatLayout=new QGridLayout(this);

    init_seat();

    buttonLayout->addStretch();
    buttonLayout->addWidget(btnRandomize);
    buttonLayout->addWidget(btnRandomizeCombine);
    buttonLayout->addWidget(btnExit);

    connect(btnExit,SIGNAL(clicked()),this,SLOT(accept()));
    connect(btnRandomize,SIGNAL(clicked()),this,SLOT(shuffle_animated()));
    connect(btnRandomizeCombine,SIGNAL(clicked()),this,SLOT(shuffle_no_gender_animated()));


    mainLayout->addLayout(seatLayout);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);

    init_data();

    srand(QTime::currentTime().msec());
}

void Dialog::init_seat(){
    QBrush fontBrush(QColor(255, 255, 255, 255));
    fontBrush.setStyle(Qt::SolidPattern);

    QBrush seatBrush(QColor(200, 100, 40, 255));
    seatBrush.setStyle(Qt::SolidPattern);

    palette.setBrush(QPalette::Active, QPalette::WindowText, fontBrush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, fontBrush);

    palette.setBrush(QPalette::Active, QPalette::Window, seatBrush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, seatBrush);

    font.setPointSize(16);
    font.setBold(true);
    font.setFamily("Open Sans"); //change this if you don't have Open Sans

    //TODO: add dynamic seating layout, possibly using textfile

    for(int i=0;i<2*4;i+=2)
        for(int j=0;j<3*4;j+=3){
            for(int k=0;k<2;++k){
                seat=new QLabel(this);
                seat->setPalette(palette);
                seat->setMinimumSize(18,90);
                seat->setFont(font);
                seat->setAutoFillBackground(true);
                seat->setText("-");
                seat->setAlignment(Qt::AlignCenter);
                seatLayout->addWidget(seat,i,j+k);
                seating.push_back(seat);
            }
            space=new QSpacerItem(10,0);
            if(j<(3*4-1))seatLayout->addItem(space,i,j+1);
            space=new QSpacerItem(0,10);
            if(i<(2*4-1))seatLayout->addItem(space,i+1,j,1,3);
        }

    for(int i=0;i<3;++i){
        seat=new QLabel(this);
        seat->setText("-");
        seat->setPalette(palette);
        seat->setMinimumSize(18,90);
        seat->setFont(font);
        seat->setAutoFillBackground(true);
        seat->setAlignment(Qt::AlignCenter);
        seating.push_back(seat);
        seatLayout->addWidget(seat,2*4,4+i);
    }

}

void Dialog::init_data(){
    //get data
    QString s;
    QFile db_male(":/database/male.txt"),db_female(":/database/female.txt");
    if(!db_male.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", db_male.errorString());
    }
    if(!db_female.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", db_female.errorString());
    }

    QTextStream in_male(&db_male),in_female(&db_female);
    int male_count, female_count;

    in_male>>male_count;
    in_male.readLine();
    for(int i=0;i<male_count;++i){
        s = in_male.readLine();
        if(s.size()>1)gents.push_back(s);
    }
    db_male.close();

    in_female>>female_count;
    in_female.readLine();
    for(int i=0;i<female_count;++i) {
        s = in_female.readLine();
        if(s.size()>1)lads.push_back(s);
    }
    db_female.close();
}
