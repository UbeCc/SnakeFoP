#ifndef REPLAYPAGE_H
#define REPLAYPAGE_H

#include <QDialog>
#include "playpage.h"
#include "resultpage.h"
#include "game.h"
#include "widget.h"

namespace Ui {
class RePlayPage;
}

class Widget;
class ResultPage;
class GameCanvas;

class RePlayPage : public QDialog
{
    Q_OBJECT

public:
    explicit RePlayPage(QWidget *parent = nullptr);
    ~RePlayPage();
    void initPlay(QFileInfo);
    int getScore();
    int getLength();
    virtual void showEvent(QShowEvent *event);

private slots:
    void Step();
    void recordButton_clicked();
    void playButton_clicked();
    void exitButton_clicked();

private:
    bool initFlag, playFlag;
    Widget *widget;
    Game *game;
    Ui::RePlayPage *ui;
    QTimer *gameTimer;
    GameCanvas *gameCanvas;
    Record record;
    void gameOver();
};

#endif // REPLAYPAGE_H
