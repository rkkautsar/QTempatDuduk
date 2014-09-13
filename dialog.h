#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtGui>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected slots:
    void acakv2();
    void acak();
    void acak_campur();
    void acak_campur_animated();
};

#endif // DIALOG_H
