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
    const auto &mapDef = _game->GetStatus().mapDefinition;
    setMinimumSize(mapDef.width * 25, mapDef.height * 25);
    update();
}

void GameCanvas::CalculateBlockSizeAndOffset(int &blockSize, int &row, int &col, int &xOffset, int &yOffset) const
{
    auto size = this->rect().size();
    int width = size.width(), height = size.height();

    const auto &map = game->GetStatus().mapDefinition;
    col = map.height, row = map.width;

    int visualCol = col, visualRow = row;

    // HBorder is the top and bottom border (horizontal), VBorder is the left and right border
    bool hBorder = map.borderIsObstacle[0] || map.borderIsObstacle[1],
        vBorder = map.borderIsObstacle[2] || map.borderIsObstacle[3];

    if (hBorder)
    {
        visualCol += 2;
    }

    if (vBorder)
    {
        visualRow += 2;
    }

    blockSize = min(width / visualRow, height / visualCol);

    width = blockSize * visualRow;
    height = blockSize * visualCol;

    xOffset = (size.width() - width) / 2 + (vBorder ? blockSize : 0);
    yOffset = (size.height() - height) / 2 + (hBorder ? blockSize : 0);
}

void GameCanvas::paintEvent(QPaintEvent *event)
{
    if (!game)
    {
        return;
    }

    auto &status = game->GetStatus();
    int col, row, blockSize, xOffset, yOffset;
    CalculateBlockSizeAndOffset(blockSize, row, col, xOffset, yOffset);

    constexpr double BLOCK_MARGIN = 0.05;
    const double margin = blockSize * BLOCK_MARGIN;

    QPainter painter(this);

    bool hBorder = status.mapDefinition.borderIsObstacle[0] || status.mapDefinition.borderIsObstacle[1],
        vBorder = status.mapDefinition.borderIsObstacle[2] || status.mapDefinition.borderIsObstacle[3];

    // draw playground
    painter.setBrush(Qt::gray);
    painter.setPen(Qt::transparent);
    painter.drawRect((int) (xOffset - (vBorder ? blockSize : 0) - margin),
        (int) (yOffset - (hBorder ? blockSize : 0) - margin),
        (int) (row * blockSize + (vBorder ? 2 : 0) * blockSize + 2 * margin),
        (int) (col * blockSize + (hBorder ? 2 : 0) * blockSize + 2 * margin));

    // draw border
    const static QBrush borderBrush(QColor::fromHsv(0, 0, 72));
    painter.setBrush(borderBrush);

    if (hBorder)
    {
        for (int i = 0; i < status.mapDefinition.width; ++i)
        {
            painter.drawRect((int) (xOffset + i * blockSize + margin),
                (int) (yOffset - blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
            painter.drawRect((int) (xOffset + i * blockSize + margin),
                (int) (yOffset + status.mapDefinition.height * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
        }
    }

    if (vBorder)
    {
        for (int i = 0; i < status.mapDefinition.height; ++i)
        {
            painter.drawRect((int) (xOffset - blockSize + margin),
                (int) (yOffset + i * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
            painter.drawRect((int) (xOffset + status.mapDefinition.width * blockSize + margin),
                (int) (yOffset + i * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
        }
    }

    if (hBorder && vBorder)
    {
        painter.drawRect((int) (xOffset - blockSize + margin),
            (int) (yOffset - blockSize + margin),
            (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
        painter.drawRect((int) (xOffset + status.mapDefinition.width * blockSize + margin),
            (int) (yOffset - blockSize + margin),
            (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
        painter.drawRect((int) (xOffset - blockSize + margin),
            (int) (yOffset + status.mapDefinition.height * blockSize + margin),
            (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
        painter.drawRect((int) (xOffset + status.mapDefinition.width * blockSize + margin),
            (int) (yOffset + status.mapDefinition.height * blockSize + margin),
            (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
    }

    // draw snake head
    const static QBrush headBrush(QColor::fromHsv(91, 255, 56));
    painter.setBrush(headBrush);
    painter.setPen(Qt::transparent);
    Point head = status.head;

    switch (status.directionMap[head.x][head.y])
    {
        case Game::Direction::Up:
            painter.drawRect((int) (xOffset + head.x * blockSize + margin),
                (int) (yOffset + head.y * blockSize + blockSize / 2. - margin),
                (int) (blockSize - 2 * margin), (int) (blockSize / 2.));
            painter.drawPie((int) (xOffset + head.x * blockSize + margin),
                (int) (yOffset + head.y * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin),
                0 * 16, 180 * 16);
            break;
        case Game::Direction::Down:
            painter.drawRect((int) (xOffset + head.x * blockSize + margin),
                (int) (yOffset + head.y * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize / 2.));
            painter.drawPie((int) (xOffset + head.x * blockSize + margin),
                (int) (yOffset + head.y * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin),
                180 * 16, 180 * 16);
            break;
        case Game::Direction::Left:
            painter.drawRect((int) (xOffset + head.x * blockSize + blockSize / 2. - margin),
                (int) (yOffset + head.y * blockSize + margin),
                (int) (blockSize / 2.), (int) (blockSize - 2 * margin));
            painter.drawPie((int) (xOffset + head.x * blockSize + margin),
                (int) (yOffset + head.y * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin),
                90 * 16, 180 * 16);
            break;
        case Game::Direction::Right:
            painter.drawRect((int) (xOffset + head.x * blockSize + margin),
                (int) (yOffset + head.y * blockSize + margin),
                (int) (blockSize / 2.), (int) (blockSize - 2 * margin));
            painter.drawPie((int) (xOffset + head.x * blockSize + margin),
                (int) (yOffset + head.y * blockSize + margin),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin),
                270 * 16, 180 * 16);
            break;
    }

    Point tail = status.tail;
    Point curPoint = tail;
    Point previousPoint = tail;
    int l = -status.length;

    // Draw snake tail if any
    if (curPoint != head)
    {
        painter.setBrush(QColor::fromHsv(90, max(32, 255 + 8 * ++l), 104));
        switch (status.directionMap[curPoint.x][curPoint.y])
        {
            case Game::Direction::Up:
                painter.drawPolygon(QPolygon({
                    QPoint((int) (xOffset + curPoint.x * blockSize + margin),
                        (int) (yOffset + curPoint.y * blockSize + margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize - margin),
                        (int) (yOffset + curPoint.y * blockSize + margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize / 2.),
                        (int) (yOffset + curPoint.y * blockSize + blockSize / 4. * 3 - margin)),
                }));
                break;
            case Game::Direction::Down:
                painter.drawPolygon(QPolygon({
                    QPoint((int) (xOffset + curPoint.x * blockSize + margin),
                        (int) (yOffset + curPoint.y * blockSize + blockSize - margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize - margin),
                        (int) (yOffset + curPoint.y * blockSize + blockSize - margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize / 2.),
                        (int) (yOffset + curPoint.y * blockSize + blockSize / 4. * 1 + margin)),
                }));
                break;
            case Game::Direction::Left:
                painter.drawPolygon(QPolygon({
                    QPoint((int) (xOffset + curPoint.x * blockSize + margin),
                        (int) (yOffset + curPoint.y * blockSize + margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + margin),
                        (int) (yOffset + curPoint.y * blockSize + blockSize - margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize / 4. * 3 - margin),
                        (int) (yOffset + curPoint.y * blockSize + blockSize / 2.)),
                }));
                break;
            case Game::Direction::Right:
                painter.drawPolygon(QPolygon({
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize - margin),
                        (int) (yOffset + curPoint.y * blockSize + margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize - margin),
                        (int) (yOffset + curPoint.y * blockSize + blockSize - margin)),
                    QPoint((int) (xOffset + curPoint.x * blockSize + blockSize / 4. * 1 + margin),
                        (int) (yOffset + curPoint.y * blockSize + blockSize / 2.)),
                }));
                break;
        }

        curPoint = status.map[curPoint.x][curPoint.y];
    }

    while (curPoint != head)
    {
        painter.setBrush(QColor::fromHsv(90, max(32, 255 + 8 * ++l), 104));

        const Game::Direction previousDirection = -status.directionMap[previousPoint.x][previousPoint.y];
        const Game::Direction nextDirection = status.directionMap[curPoint.x][curPoint.y];

        if ((previousDirection == Game::Direction::Up && nextDirection == Game::Direction::Right) ||
            (previousDirection == Game::Direction::Right && nextDirection == Game::Direction::Up))
        {
            painter.drawPie((int) (xOffset + curPoint.x * blockSize + margin),
                (int) (yOffset + curPoint.y * blockSize - blockSize + 3 * margin),
                (int) (2 * blockSize - 4 * margin), (int) (2 * blockSize - 4 * margin),
                180 * 16, 90 * 16);
        }
        else if ((previousDirection == Game::Direction::Up && nextDirection == Game::Direction::Left) ||
            (previousDirection == Game::Direction::Left && nextDirection == Game::Direction::Up))
        {
            painter.drawPie((int) (xOffset + curPoint.x * blockSize - blockSize + 3 * margin),
                (int) (yOffset + curPoint.y * blockSize - blockSize + 3 * margin),
                (int) (2 * blockSize - 4 * margin), (int) (2 * blockSize - 4 * margin),
                270 * 16, 90 * 16);
        }
        else if ((previousDirection == Game::Direction::Down && nextDirection == Game::Direction::Right) ||
            (previousDirection == Game::Direction::Right && nextDirection == Game::Direction::Down))
        {
            painter.drawPie((int) (xOffset + curPoint.x * blockSize + margin),
                (int) (yOffset + curPoint.y * blockSize + margin),
                (int) (2 * blockSize - 4 * margin), (int) (2 * blockSize - 4 * margin),
                90 * 16, 90 * 16);
        }
        else if ((previousDirection == Game::Direction::Down && nextDirection == Game::Direction::Left) ||
            (previousDirection == Game::Direction::Left && nextDirection == Game::Direction::Down))
        {
            painter.drawPie((int) (xOffset + curPoint.x * blockSize - blockSize + 3 * margin),
                (int) (yOffset + curPoint.y * blockSize + margin),
                (int) (2 * blockSize - 4 * margin), (int) (2 * blockSize - 4 * margin),
                0 * 16, 90 * 16);
        }
        else
        {
            painter.drawRect((int) (xOffset + margin + curPoint.x * blockSize),
                (int) (yOffset + margin + curPoint.y * blockSize),
                (int) (blockSize - 2 * margin), (int) (blockSize - 2 * margin));
        }

        previousPoint = curPoint;
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
            painter.drawPolygon(QPolygon({
                QPoint((int) (xOffset + p.x * blockSize + 3 * margin),
                    (int) (yOffset + p.y * blockSize + blockSize / 2.)),
                QPoint((int) (xOffset + p.x * blockSize + blockSize / 2.),
                    (int) (yOffset + p.y * blockSize + 3 * margin)),
                QPoint((int) (xOffset + (p.x + 1) * blockSize - 3 * margin),
                    (int) (yOffset + p.y * blockSize + blockSize / 2.)),
                QPoint((int) (xOffset + p.x * blockSize + blockSize / 2.),
                    (int) (yOffset + (p.y + 1) * blockSize - 3 * margin)),
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

    int col, row, blockSize, xOffset, yOffset;
    CalculateBlockSizeAndOffset(blockSize, row, col, xOffset, yOffset);

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
