#ifndef DIALOG_H
#define DIALOG_H

#include "config.h"

#include <QDialog>
#include <QtGui>
#include <QApplication>
#include <QList>

#include <cstdlib>
#include <algorithm>

#define MALE true
#define FEMALE false
const int animation_time=animation_time_ms/per_frame_ms;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected slots:
    void shuffle();
    void shuffle_animated();
    void change_people();
    void change_rule();
    void update();

private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QGridLayout* seatLayout;
    QList<bool> seating_rule;
    QList<QLabel*> seating;
    QList<QString> male,female,rule;
    QPushButton *btnChangePeople, *btnChangeRule, *btnUpdate, *btnRandomize, *btnExit;
    QString db_people_filename, db_rule_filename;
    QLabel* seat;
    QPalette palette;
    QFont font;
    QTime dieTime;
    int male_count,female_count,rule_male,rule_female,rule_row,rule_column;

    void init();
    void init_seat();
    void init_seat_aux();
    void init_data();

};

#endif // DIALOG_H
