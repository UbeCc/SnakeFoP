#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QWidget>
#include "game.h"

class GameCanvas : public QWidget {
    Q_OBJECT

private:
    const static QColor portalColors[];
    const Game *game{};
    std::function<void(int, int)> onMouseSelect;

public:
    explicit GameCanvas(QWidget *parent = nullptr);

    void SetGame(const Game *_game);

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void SetOnMouseSelect(std::function<void(int, int)>);
};

#endif // GAMECANVAS_H
