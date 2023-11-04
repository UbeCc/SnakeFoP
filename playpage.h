#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QDialog>
#include "resultpage.h"
#include "game.h"
#include "widget.h"

namespace Ui {
class PlayPage;
}

class Widget;
class ResultPage;
class GameCanvas;


class PlayPage : public QDialog
{
    Q_OBJECT

public:
    explicit PlayPage(QWidget *parent = nullptr);
    ~PlayPage();
    void initPlay();
    virtual void keyPressEvent(QKeyEvent*);
    int getScore();
    int getLength();

private slots:
    void Step();

private:
    Widget *widget;
    Game *game;
    Ui::PlayPage *ui;
    QTimer *gameTimer;
    ResultPage *resultPage;
    GameCanvas *gameCanvas;
    void gameOver();
};

#endif // PLAYPAGE_H
