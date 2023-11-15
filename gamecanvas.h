#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QWidget>
#include "game.h"

class GameCanvas : public QWidget
{
Q_OBJECT

private:
    const static QColor portalColors[];
    const Game *game{};
    std::function<void(int, int, bool)> onMouseSelect;

public:
    explicit GameCanvas(QWidget *parent = nullptr) : QWidget(parent) {};
    void SetGame(const Game *_game);

    void SetOnMouseSelect(std::function<void(int, int, bool)>);

public slots:

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void CalculateBlockSizeAndOffset(int &blockSize, int &row, int &col, int &xOffset, int &yOffset) const;
};

#endif // GAMECANVAS_H
