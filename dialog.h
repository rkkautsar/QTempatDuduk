#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtGui>
#include <QApplication>
#include <QList>

#include <cstdlib>
#include <algorithm>

#define animation_time_ms 500 //500 ms
#define per_frame_ms 50 //1 frame per 50 ms

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
    void shuffle_no_gender();
    void shuffle_no_gender_animated();

private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QGridLayout* seatLayout;
    QList<QLabel*> seating;
    QList<QString> gents,lads;
    QPushButton *btnRandomize, *btnRandomizeCombine, *btnExit;
    QLabel* seat;
    QSpacerItem* space;
    QPalette palette;
    QFont font;
    QTime dieTime;

    void init();
    void init_seat();
    void init_data();

};

#endif // DIALOG_H
