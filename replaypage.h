#ifndef REPLAYPAGE_H
#define REPLAYPAGE_H

#include <QDialog>
#include "game.h"
#include "widget.h"

namespace Ui
{
    class ReplayPage;
}

class Widget;

class ResultPage;

class GameCanvas;

class ReplayPage : public QDialog
{
Q_OBJECT

public:
    explicit ReplayPage(QWidget *parent = nullptr);

    ~ReplayPage() override;

    bool InitPlay(const QFileInfo &);

    int GetScore();

    int GetLength();

private slots:

    void Step();

    void OnRecordButtonClicked();

    void OnPlayButtonClicked();

    void OnExitButtonClicked();

private:
    bool initFlag, playFlag;
    Widget *widget;
    int curStep;
    Game *game;
    Ui::ReplayPage *ui;
    QTimer *gameTimer;
    Record record;

    void GameOver();
};

#endif // REPLAYPAGE_H
