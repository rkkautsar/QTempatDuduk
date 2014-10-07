#include "dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    init();
}

Dialog::~Dialog()
{

}

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
    this->setWindowTitle("QTempatDuduk v1.4");
    this->resize(1000,500);

    btnChangePeople=new QPushButton(tr("&People..."),this);
    btnChangeRule=new QPushButton(tr("&Rule..."),this);
    btnUpdate=new QPushButton(tr("&Update!"),this);
    btnRandomize=new QPushButton(tr("&Shuffle!"),this);
    btnExit=new QPushButton(tr("E&xit"),this);

    mainLayout=new QVBoxLayout(this);
    buttonLayout=new QHBoxLayout(this);
    seatLayout=new QGridLayout(this);

    //default files
    db_people_filename="./people.txt";
    db_rule_filename="./seating.txt";

    init_data();
    init_seat();

    buttonLayout->addWidget(btnChangePeople);
    buttonLayout->addWidget(btnChangeRule);
    buttonLayout->addWidget(btnUpdate);
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnRandomize);
    buttonLayout->addWidget(btnExit);

    connect(btnChangePeople,SIGNAL(clicked()),this,SLOT(change_people()));
    connect(btnChangeRule,SIGNAL(clicked()),this,SLOT(change_rule()));
    connect(btnUpdate,SIGNAL(clicked()),this,SLOT(update()));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(accept()));
    connect(btnRandomize,SIGNAL(clicked()),this,SLOT(shuffle_animated()));


    mainLayout->addLayout(seatLayout);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);
}

void Dialog::init_seat(){
    QBrush fontBrush(QColor(FONTCOLOR));
    fontBrush.setStyle(Qt::SolidPattern);

    QBrush seatBrush(QColor(SEATCOLOR));
    seatBrush.setStyle(Qt::SolidPattern);

    palette.setBrush(QPalette::Active, QPalette::WindowText, fontBrush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, fontBrush);

    palette.setBrush(QPalette::Active, QPalette::Window, seatBrush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, seatBrush);

    font.setPointSize(FONTSIZE);
    font.setBold(true);
    font.setFamily(FONTFAMILY);

    for(int i=0;i<rule_row;++i){
        for(int j=0;j<rule_column;++j){
            if(rule[i][j]=='.') seatLayout->addItem(new QSpacerItem(SPACERSIZE),i,j);
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
    seat->setMinimumSize(SEATMINIMUMSIZE);
}

void Dialog::init_data(){
    //get data
    QString s;
    QFile db_people(db_people_filename),db_rule(db_rule_filename);
    if(!db_people.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, tr("Error"), db_people.errorString());
    }
    if(!db_rule.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, tr("Error"), db_rule.errorString());
    }

    QTextStream in_people(&db_people),in_rule(&db_rule);

    in_people>>male_count;
    in_people.readLine(); //clearing
    for(int i=0;i<male_count;++i){
        s = in_people.readLine();
        if(s.size())male.push_back(s);
    }

    in_people>>female_count;
    in_people.readLine();
    for(int i=0;i<female_count;++i) {
        s = in_people.readLine();
        if(s.size())female.push_back(s);
    }
    db_people.close();

    in_rule>>rule_male>>rule_female;
    if(rule_male!=male_count || rule_female!=female_count)
        QMessageBox::information(0, tr("Error"), tr("Mismatch number of person"));

    in_rule>>rule_row>>rule_column;
    for(int i=0;i<rule_row;++i){
        in_rule>>s;
        rule.push_back(s);
    }
    db_rule.close();

}

void Dialog::change_people(){
    db_people_filename=
            QFileDialog::getOpenFileName(this,
                                         tr("Select file..."),
                                         QDir::homePath(),
                                         tr("Text files (*.txt)"));
    if(!QFile(db_people_filename).exists()){
        QMessageBox::information(0, tr("Error"), "File not found");
        db_people_filename="./people.txt";
    }
}

void Dialog::change_rule(){
    db_rule_filename=
            QFileDialog::getOpenFileName(this,
                                         tr("Select file..."),
                                         QDir::homePath(),
                                         tr("Text files (*.txt)"));
    if(!QFile(db_rule_filename).exists()){
        QMessageBox::information(0, tr("Error"), "File not found");
        db_rule_filename="./seating.txt";
    }
}

void Dialog::update(){
    //empty all
    male.clear();
    female.clear();
    seating_rule.clear();
    for(int i=0;i<rule_row;++i)
        for(int j=0;j<rule_column;++j)
            seatLayout->removeItem(seatLayout->itemAtPosition(i,j));
    for(int i=0;i<seating.size();++i)
        delete seating[i];
    seating.clear();
    rule.clear();

    //redo
    init_data();
    init_seat();

}
