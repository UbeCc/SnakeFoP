#include "gamecanvas.h"
#include <QtWidgets>
#include <QPainter>

GameCanvas::GameCanvas(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(300, 300);
}

void GameCanvas::SetGame(const Game *_game) {
    this->game = _game;
}

void GameCanvas::paintEvent(QPaintEvent *event) {
    auto &status = game->GetStatus();
    int col = status.mapDefinition.height, row = status.mapDefinition.width;

    auto size = event->rect().size();
    int width = size.width(), height = size.height();

    const int blockSize = min(width / col, height / row);

    width = blockSize * col;
    height = blockSize * row;

    const int xOffset = (size.width() - width) / 2;
    const int yOffset = (size.height() - height) / 2;

    constexpr double BLOCK_MARGIN = 0.05;
    const int margin = (int) (blockSize * BLOCK_MARGIN);

    QPainter painter(this);

    // draw playground
    painter.setBrush(Qt::gray);
    painter.setPen(Qt::transparent);
    painter.drawRect(xOffset, yOffset, col * blockSize, row * blockSize);

    // draw snake
    painter.setBrush(Qt::green);
    Point head = status.head;
    painter.drawRect(xOffset + margin + head.x * blockSize, yOffset + margin + head.y * blockSize,
                     blockSize - 2 * margin, blockSize - 2 * margin);

    Point tail = status.tail;
    Point curPoint = tail;
    int l = -status.length;
    while (curPoint != head) {
        painter.setBrush(QColor::fromHsv(0, max(32, 255 + 8 * ++l), 255));
        painter.drawRect(xOffset + margin + curPoint.x * blockSize, yOffset + margin + curPoint.y * blockSize,
                         blockSize - 2 * margin, blockSize - 2 * margin);
        curPoint = status.map[curPoint.x][curPoint.y];
    }

    // draw foods

    painter.setBrush(Qt::white);
    painter.setPen(Qt::transparent);
    auto &foods = status.foods;
    for (const auto &food: foods) {
        int sizeFactor = (3 + 2 * (3 - status.map[food.x][food.y].y)) * margin;
        painter.drawEllipse(xOffset + sizeFactor + food.x * blockSize, yOffset + sizeFactor + food.y * blockSize,
                            blockSize - 2 * sizeFactor, blockSize - 2 * sizeFactor);
    }

    // draw obstacles
    painter.setBrush(Qt::black);
    painter.setPen(Qt::transparent);
    auto obstacles = status.mapDefinition.obstacles;
    for (const auto &obstacle: obstacles) {
        painter.drawRect(xOffset + margin + obstacle.x * blockSize, yOffset + margin + obstacle.y * blockSize,
                         blockSize - 2 * margin, blockSize - 2 * margin);
    }

    // draw portals
    auto portals = status.mapDefinition.portals;
    for (const auto &portal: portals) {
        Point start = portal[0], end = portal[1];
        painter.setBrush(QColorConstants::Svg::darkblue);
        painter.drawRect(xOffset + 6 * margin + start.x * blockSize, yOffset + 6 * margin + start.y * blockSize,
                         blockSize - 12 * margin, blockSize - 12 * margin);
        painter.drawRect(xOffset + 6 * margin + end.x * blockSize, yOffset + 6 * margin + end.y * blockSize,
                         blockSize - 12 * margin, blockSize - 12 * margin);
    }
}