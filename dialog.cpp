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
            QCoreApplication::processEvents(QEventLoop::AllEvents, per_frame_ms);
    }

    //enable button
    btnRandomize->setDown(false);
    connect(btnRandomize,SIGNAL(clicked()),this,SLOT(shuffle_animated()));
}

void Dialog::shuffle(){
    //shuffle
    srand(QTime::currentTime().msec());
    std::random_shuffle(male.begin(),male.end());
    std::random_shuffle(female.begin(),female.end());

    int male_iterator=0,female_iterator=0;
    for(int i=0;i<male_count+female_count;++i){
        if(seating_rule[i]==MALE) seating[i]->setText(male[male_iterator++]);
        else seating[i]->setText(female[female_iterator++]);
    }
}

void Dialog::init(){
    this->setWindowTitle("QTempatDuduk v1.3");
    this->resize(1130,500);

    btnRandomize=new QPushButton(tr("&Shuffle!"),this);
    btnExit=new QPushButton(tr("E&xit"),this);

    mainLayout=new QVBoxLayout(this);
    buttonLayout=new QHBoxLayout(this);
    seatLayout=new QGridLayout(this);

    init_data();
    init_seat();

    buttonLayout->addStretch();
    buttonLayout->addWidget(btnRandomize);
    buttonLayout->addWidget(btnExit);

    connect(btnExit,SIGNAL(clicked()),this,SLOT(accept()));
    connect(btnRandomize,SIGNAL(clicked()),this,SLOT(shuffle_animated()));


    mainLayout->addLayout(seatLayout);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);
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

    for(int i=0;i<rule_row;++i){
        for(int j=0;j<rule_column;++j){
            if(rule[i][j]=='.') seatLayout->addItem(new QSpacerItem(15,10),i,j);
            else{
                init_seat_aux();
                seatLayout->addWidget(seat,i,j);
                seating.push_back(seat);
                seating_rule.push_back((rule[i][j]=='m'? MALE : FEMALE));
            }
        }
    }

}

void Dialog::init_seat_aux(){
    seat=new QLabel("-",this);
    seat->setPalette(palette);
    seat->setFont(font);
    seat->setAutoFillBackground(true);
    seat->setAlignment(Qt::AlignCenter);
    seat->setMinimumSize(15,80);
}

void Dialog::init_data(){
    //get data
    QString s;
    QFile db_male(":/database/male.txt"),db_female(":/database/female.txt"),db_rule(":/database/custom.seating.txt");
    if(!db_male.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", db_male.errorString());
    }
    if(!db_female.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", db_female.errorString());
    }
    if(!db_rule.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", db_rule.errorString());
    }

    QTextStream in_male(&db_male),in_female(&db_female),in_rule(&db_rule);

    in_male>>male_count;
    in_male.readLine();
    for(int i=0;i<male_count;++i){
        s = in_male.readLine();
        if(s.size()>1)male.push_back(s);
    }
    db_male.close();

    in_female>>female_count;
    in_female.readLine();
    for(int i=0;i<female_count;++i) {
        s = in_female.readLine();
        if(s.size()>1)female.push_back(s);
    }
    db_female.close();

    in_rule>>rule_male>>rule_female;
    if(rule_male!=male_count || rule_female!=female_count)
        QMessageBox::information(0, "Error", "Mismatch number of person");

    in_rule>>rule_row>>rule_column;
    for(int i=0;i<rule_row;++i){
        in_rule>>s;
        rule.push_back(s);
    }

}
