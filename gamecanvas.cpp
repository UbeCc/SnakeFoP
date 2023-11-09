#include "gamecanvas.h"
#include <QtWidgets>
#include <QPainter>
#include <utility>

using std::min;
using std::max;

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

void GameCanvas::SetGame(const Game *_game)
{
    this->game = _game;
    update();
}

void GameCanvas::paintEvent(QPaintEvent *event)
{
    if (!game)
    {
        return;
    }

    auto &status = game->GetStatus();
    int col = status.mapDefinition.height, row = status.mapDefinition.width;

    auto size = event->rect().size();
    int width = size.width(), height = size.height();

    const int blockSize = min(width / row, height / col);

    width = blockSize * row;
    height = blockSize * col;

    const int xOffset = (size.width() - width) / 2;
    const int yOffset = (size.height() - height) / 2;

    constexpr double BLOCK_MARGIN = 0.05;
    const double margin = blockSize * BLOCK_MARGIN;

    QPainter painter(this);

    // draw playground
    painter.setBrush(Qt::gray);
    painter.setPen(Qt::transparent);
    painter.drawRect(xOffset, yOffset, row * blockSize, col * blockSize);

    // draw border
    const static QPen borderPen(Qt::red, 3);
    if (status.mapDefinition.borderIsObstacle[0])
    {
        painter.setPen(borderPen);
        painter.drawLine(xOffset, yOffset, xOffset + row * blockSize, yOffset);
    }

    if (status.mapDefinition.borderIsObstacle[1])
    {
        painter.setPen(borderPen);
        painter.drawLine(xOffset + row * blockSize, yOffset + col * blockSize, xOffset, yOffset + col * blockSize);
    }

    if (status.mapDefinition.borderIsObstacle[2])
    {
        painter.setPen(borderPen);
        painter.drawLine(xOffset + row * blockSize, yOffset, xOffset + row * blockSize, yOffset + col * blockSize);
    }

    if (status.mapDefinition.borderIsObstacle[3])
    {
        painter.setPen(borderPen);
        painter.drawLine(xOffset, yOffset + col * blockSize, xOffset, yOffset);
    }

    // draw snake
    painter.setBrush(Qt::green);
    painter.setPen(Qt::transparent);
    Point head = status.head;
    painter.drawRect((int) (xOffset + margin + head.x * blockSize), int(yOffset + margin + head.y * blockSize),
        (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));

    Point tail = status.tail;
    Point curPoint = tail;
    int l = -status.length;
    while (curPoint != head)
    {
        painter.setBrush(QColor::fromHsv(0, max(32, 255 + 8 * ++l), 255));
        painter.drawRect((int) (xOffset + margin + curPoint.x * blockSize),
            int(yOffset + margin + curPoint.y * blockSize),
            (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
        curPoint = status.map[curPoint.x][curPoint.y];
    }

    // draw foods
    painter.setBrush(Qt::white);
    painter.setPen(Qt::transparent);
    auto &foods = status.foods;
    for (const auto &food: foods)
    {
        double sizeFactor = (3 + 2 * (3 - status.map[food.x][food.y].y)) * margin;
        painter.drawEllipse((int) (xOffset + sizeFactor + food.x * blockSize),
            (int) (yOffset + sizeFactor + food.y * blockSize),
            (int) (blockSize - 2 * sizeFactor), (int) (blockSize - 2 * sizeFactor));
    }

    // draw obstacles
    painter.setBrush(Qt::black);
    painter.setPen(Qt::transparent);
    auto obstacles = status.mapDefinition.obstacles;
    for (const auto &obstacle: obstacles)
    {
        painter.drawRect((int) (xOffset + margin + obstacle.x * blockSize),
            (int) (yOffset + margin + obstacle.y * blockSize),
            (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
    }

    // draw portals
    auto portals = status.mapDefinition.portals;
    for (int i = 0; i < (int) portals.size(); ++i)
    {
        const auto &portal = portals[i];
        painter.setBrush(portalColors[i % 10]);

        for (const auto &p: portal)
        {
            // Draw a rhombus
            painter.drawPolygon(
                QPolygonF({
                    QPointF(xOffset + p.x * blockSize + margin,
                        yOffset + p.y * blockSize + blockSize / 2.),
                    QPointF(xOffset + p.x * blockSize + blockSize / 2.,
                        yOffset + p.y * blockSize + margin),
                    QPointF(xOffset + (p.x + 1) * blockSize - margin,
                        yOffset + p.y * blockSize + blockSize / 2.),
                    QPointF(xOffset + p.x * blockSize + blockSize / 2.,
                        yOffset + (p.y + 1) * blockSize - margin),
                }));
        }
    }
}

// Forward click event to move event
void GameCanvas::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    mouseMoveEvent(event);
}

void GameCanvas::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (!(event->buttons() & (Qt::LeftButton | Qt::RightButton)))
    {
        return;
    }

    if (!game || !onMouseSelect)
    {
        return;
    }

    auto &status = game->GetStatus();
    int col = status.mapDefinition.height, row = status.mapDefinition.width;

    auto size = this->rect().size();
    int width = size.width(), height = size.height();

    const int blockSize = min(width / row, height / col);

    width = blockSize * row;
    height = blockSize * col;

    const int xOffset = (size.width() - width) / 2;
    const int yOffset = (size.height() - height) / 2;

    const int x = ((int) event->position().x() - xOffset) / blockSize,
        y = ((int) event->position().y() - yOffset) / blockSize;

    if (x < 0 || x >= row || y < 0 || y >= col)
    {
        return;
    }

    onMouseSelect(x, y, (bool) (event->buttons() & Qt::RightButton));
}

void GameCanvas::SetOnMouseSelect(std::function<void(int, int, bool)> select)
{
    this->onMouseSelect = std::move(select);
}
