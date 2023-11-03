#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QDialog>
#include "resultpage.h"

namespace Ui {
class PlayPage;
}

class PlayPage : public QDialog
{
    Q_OBJECT

public:
    explicit PlayPage(QWidget *parent = nullptr);
    ~PlayPage();

private slots:
    void on_btn_clicked();

private:
    Ui::PlayPage *ui;
    ResultPage *resultpage = new ResultPage();
    void gameOver();
};

#endif // PLAYPAGE_H
