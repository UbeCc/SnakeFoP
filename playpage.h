#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QDialog>
#include "resultpage.h"
#include "game.h"
#include "widget.h"
#include "replaypage.h"

namespace Ui {
class PlayPage;
}

class Widget;
class ResultPage;
class GameCanvas: public QWidget {
private:
    const Game *game;

public:
    GameCanvas(QWidget *parent = nullptr) : QWidget(parent) {
        setFocusPolicy(Qt::StrongFocus);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMinimumSize(300, 300);
    }
    void SetGame(const Game *game);
    void paintEvent(QPaintEvent *event);
};

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
