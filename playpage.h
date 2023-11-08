#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QDialog>
#include "resultpage.h"
#include "game.h"
#include <QElapsedTimer>
#include "widget.h"

namespace Ui
{
    class PlayPage;
}

class Widget;

class ResultPage;

class PlayPage : public QDialog
{
Q_OBJECT

public:
    explicit PlayPage(QWidget *parent = nullptr);

    ~PlayPage() override;

    bool initPlay();

    void keyPressEvent(QKeyEvent *) override;

    int getScore();

    int getLength();

    void Step();

private:
    int steps;
    Widget *widget;
    Game *game;
    Ui::PlayPage *ui;
    QTimer *gameTimer;
    ResultPage *resultPage;

    void gameOver();
};

#endif // PLAYPAGE_H
