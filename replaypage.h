#ifndef REPLAYPAGE_H
#define REPLAYPAGE_H

#include <QDialog>
#include "playpage.h"
#include "resultpage.h"
#include "game.h"
#include "widget.h"

namespace Ui
{
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

    ~RePlayPage() override;

    bool initPlay(const QFileInfo &);

    int getScore();

    int getLength();

private slots:

    void Step();

    void recordButton_clicked();

    void playButton_clicked();

    void exitButton_clicked();

private:
    bool initFlag, playFlag;
    Widget *widget;
    int curStep;
    Game *game;
    Ui::RePlayPage *ui;
    QTimer *gameTimer;
    Record record;

    void gameOver();
};

#endif // REPLAYPAGE_H
