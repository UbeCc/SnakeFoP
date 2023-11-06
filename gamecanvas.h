#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QWidget>
#include "game.h"

class GameCanvas : public QWidget {
    Q_OBJECT

private:
    const Game *game{};

public:
    explicit GameCanvas(QWidget *parent = nullptr);

    void SetGame(const Game *_game);

    void paintEvent(QPaintEvent *event) override;
};

#endif // GAMECANVAS_H
