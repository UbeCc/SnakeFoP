#include "gamecanvas.h"
#include <QtWidgets>
#include <QPainter>
#include <utility>

const QColor GameCanvas::portalColors[] = {
        QColor::fromString("#aa2e25"),
        QColor::fromString("#a31545"),
        QColor::fromString("#6d1b7b"),
        QColor::fromString("#673ab7"),
        QColor::fromString("#3f51b5"),
        QColor::fromString("#1769aa"),
        QColor::fromString("#03a9f4"),
        QColor::fromString("#00695f"),
        QColor::fromString("#357a38"),
        QColor::fromString("#b26a00"),
};

GameCanvas::GameCanvas(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(300, 300);
}

void GameCanvas::SetGame(const Game *_game) {
    this->game = _game;
    update();
}

void GameCanvas::paintEvent(QPaintEvent *event) {
    if (!game) {
        return;
    }

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

    // draw border
    const static QPen borderPen(Qt::red, 3);
    if (status.mapDefinition.borderIsObstacle[0]) {
        painter.setPen(borderPen);
        painter.drawLine(xOffset, yOffset, xOffset + col * blockSize, yOffset);
    }

    if (status.mapDefinition.borderIsObstacle[1]) {
        painter.setPen(borderPen);
        painter.drawLine(xOffset + col * blockSize, yOffset + row * blockSize, xOffset, yOffset + row * blockSize);
    }

    if (status.mapDefinition.borderIsObstacle[2]) {
        painter.setPen(borderPen);
        painter.drawLine(xOffset + col * blockSize, yOffset, xOffset + col * blockSize, yOffset + row * blockSize);
    }

    if (status.mapDefinition.borderIsObstacle[3]) {
        painter.setPen(borderPen);
        painter.drawLine(xOffset, yOffset + row * blockSize, xOffset, yOffset);
    }

    // draw snake
    painter.setBrush(Qt::green);
    painter.setPen(Qt::transparent);
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
    for (int i = 0; i < (int)portals.size(); ++i) {
        const auto &portal = portals[i];
        Point start = portal[0], end = portal[1];
        painter.setBrush(portalColors[i % 10]);
        painter.drawRect(xOffset + 6 * margin + start.x * blockSize, yOffset + 6 * margin + start.y * blockSize,
                         blockSize - 12 * margin, blockSize - 12 * margin);
        painter.drawRect(xOffset + 6 * margin + end.x * blockSize, yOffset + 6 * margin + end.y * blockSize,
                         blockSize - 12 * margin, blockSize - 12 * margin);
    }
}

// Forward click event to move event
void GameCanvas::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    mouseMoveEvent(event);
}

void GameCanvas::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);

    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    if (!game || !onMouseSelect) {
        return;
    }

    auto &status = game->GetStatus();
    int col = status.mapDefinition.height, row = status.mapDefinition.width;

    auto size = this->rect().size();
    int width = size.width(), height = size.height();

    const int blockSize = min(width / col, height / row);

    width = blockSize * col;
    height = blockSize * row;

    const int xOffset = (size.width() - width) / 2;
    const int yOffset = (size.height() - height) / 2;

    const int x = ((int)event->position().x() - xOffset) / blockSize, y = ((int)event->position().y() - yOffset) / blockSize;

    if (x < 0 || x >= col || y < 0 || y >= row) {
        return;
    }

    onMouseSelect(x, y);
}

void GameCanvas::SetOnMouseSelect(std::function<void(int, int)> select) {
    this->onMouseSelect = std::move(select);
}
