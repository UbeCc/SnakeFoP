#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QDialog>
#include "resultpage.h"
#include "mapmanager.h"
#include "configmanager.h"

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
    ResultPage *resultPage = new ResultPage();
    void gameOver();
    void initPlay();
};

#endif // PLAYPAGE_H
