#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QDialog>
#include "resultpage.h"
#include "game.h"

namespace Ui {
class PlayPage;
}

class PlayPage : public QDialog
{
    Q_OBJECT

public:
    explicit PlayPage(QWidget *parent = nullptr);
    ~PlayPage();
    void initPlay();
    virtual void paintEvent(QPaintEvent*);
    virtual void keyPressEvent(QKeyEvent*);

private slots:
    void Step();

private:
    Game *game;
    Ui::PlayPage *ui;
    QTimer *gameTimer;
    ResultPage *resultPage = new ResultPage();
    void gameOver();
};

#endif // PLAYPAGE_H
