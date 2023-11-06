#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QDialog>
#include "resultpage.h"
#include "game.h"
#include <QElapsedTimer>
#include "widget.h"
#include "replaypage.h"
#include "gamecanvas.h"

namespace Ui {
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
    void initPlay();
    void keyPressEvent(QKeyEvent*) override;
    int getScore();
    int getLength();

private slots:
    void Step();

private:
    long long startTime;
    int cnt;
    Widget *widget;
    Game *game;
    Ui::PlayPage *ui;
    QElapsedTimer gameElapsedTimer;
    QTimer *gameTimer;
    ResultPage *resultPage;
    GameCanvas *gameCanvas;
    void gameOver();
};

#endif // PLAYPAGE_H
