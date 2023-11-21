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
    setMinimumSize(mapDef.width * 25 + 10, mapDef.height * 25 + 10);
    update();
}

void GameCanvas::CalculateBlockSizeAndOffset(int &blockSize, int &row, int &col, int &xOffset, int &yOffset) const
{
    auto size = this->rect().size();
    int width = size.width() - 10, height = size.height() - 10;

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
    const int margin = (int)std::ceil(blockSize * BLOCK_MARGIN);

    QPainter painter(this);

    bool hBorder = status.mapDefinition.borderIsObstacle[0] || status.mapDefinition.borderIsObstacle[1],
        vBorder = status.mapDefinition.borderIsObstacle[2] || status.mapDefinition.borderIsObstacle[3];

    // draw playground
    const static QPen canvasBorderPen(Qt::gray, 2);
    painter.setPen(canvasBorderPen);
    painter.setBrush(Qt::transparent);
    painter.drawRect(xOffset - (vBorder ? blockSize : 0) - 3,
        yOffset - (hBorder ? blockSize : 0) - 3,
        row * blockSize + (vBorder ? 2 : 0) * blockSize + 6,
        col * blockSize + (hBorder ? 2 : 0) * blockSize + 6);

    painter.setBrush(Qt::gray);
    painter.setPen(Qt::transparent);
    painter.drawRect(xOffset - (vBorder ? blockSize : 0),
        yOffset - (hBorder ? blockSize : 0),
        row * blockSize + (vBorder ? 2 : 0) * blockSize,
        col * blockSize + (hBorder ? 2 : 0) * blockSize);

    // draw border
    const static QBrush borderBrush(QColor::fromHsv(0, 0, 72));
    painter.setBrush(borderBrush);

    if (hBorder)
    {
        for (int i = 0; i < status.mapDefinition.width; ++i)
        {
            painter.drawRect(xOffset + i * blockSize + margin,
                yOffset - blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin);
            painter.drawRect(xOffset + i * blockSize + margin,
                yOffset + status.mapDefinition.height * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin);
        }
    }

    if (vBorder)
    {
        for (int i = 0; i < status.mapDefinition.height; ++i)
        {
            painter.drawRect(xOffset - blockSize + margin,
                yOffset + i * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin);
            painter.drawRect(xOffset + status.mapDefinition.width * blockSize + margin,
                yOffset + i * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin);
        }
    }

    if (hBorder && vBorder)
    {
        painter.drawRect(xOffset - blockSize + margin,
            yOffset - blockSize + margin,
            blockSize - 2 * margin, blockSize - 2 * margin);
        painter.drawRect(xOffset + status.mapDefinition.width * blockSize + margin,
            yOffset - blockSize + margin,
            blockSize - 2 * margin, blockSize - 2 * margin);
        painter.drawRect(xOffset - blockSize + margin,
            yOffset + status.mapDefinition.height * blockSize + margin,
            blockSize - 2 * margin, blockSize - 2 * margin);
        painter.drawRect(xOffset + status.mapDefinition.width * blockSize + margin,
            yOffset + status.mapDefinition.height * blockSize + margin,
            blockSize - 2 * margin, blockSize - 2 * margin);
    }

    // draw snake head
    const static QBrush headBrush(QColor::fromHsv(91, 255, 56));
    painter.setBrush(headBrush);
    painter.setPen(Qt::transparent);
    Point head = status.head;

    switch (status.directionMap[head.x][head.y])
    {
        case Game::Direction::Up:
            painter.drawRect(xOffset + head.x * blockSize + margin,
                yOffset + head.y * blockSize + blockSize / 2 - margin,
                blockSize - 2 * margin, blockSize / 2);
            painter.drawPie(xOffset + head.x * blockSize + margin,
                yOffset + head.y * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin,
                0 * 16, 180 * 16);
            break;
        case Game::Direction::Down:
            painter.drawRect(xOffset + head.x * blockSize + margin,
                yOffset + head.y * blockSize + margin,
                blockSize - 2 * margin, blockSize / 2);
            painter.drawPie(xOffset + head.x * blockSize + margin,
                yOffset + head.y * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin,
                180 * 16, 180 * 16);
            break;
        case Game::Direction::Left:
            painter.drawRect(xOffset + head.x * blockSize + blockSize / 2 - margin,
                yOffset + head.y * blockSize + margin,
                blockSize / 2, blockSize - 2 * margin);
            painter.drawPie(xOffset + head.x * blockSize + margin,
                yOffset + head.y * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin,
                90 * 16, 180 * 16);
            break;
        case Game::Direction::Right:
            painter.drawRect(xOffset + head.x * blockSize + margin,
                yOffset + head.y * blockSize + margin,
                blockSize / 2, blockSize - 2 * margin);
            painter.drawPie(xOffset + head.x * blockSize + margin,
                yOffset + head.y * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin,
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
                    QPoint(xOffset + curPoint.x * blockSize + margin,
                        yOffset + curPoint.y * blockSize + margin),
                    QPoint(xOffset + curPoint.x * blockSize + blockSize - margin,
                        yOffset + curPoint.y * blockSize + margin),
                    QPoint(xOffset + curPoint.x * blockSize + blockSize / 2,
                        yOffset + curPoint.y * blockSize + blockSize * 3 / 4 - margin),
                }));
                break;
            case Game::Direction::Down:
                painter.drawPolygon(QPolygon({
                    QPoint(xOffset + curPoint.x * blockSize + margin,
                        yOffset + curPoint.y * blockSize + blockSize - margin),
                    QPoint(xOffset + curPoint.x * blockSize + blockSize - margin,
                        yOffset + curPoint.y * blockSize + blockSize - margin),
                    QPoint(xOffset + curPoint.x * blockSize + blockSize / 2,
                        yOffset + curPoint.y * blockSize + blockSize / 4 + margin),
                }));
                break;
            case Game::Direction::Left:
                painter.drawPolygon(QPolygon({
                    QPoint(xOffset + curPoint.x * blockSize + margin,
                        yOffset + curPoint.y * blockSize + margin),
                    QPoint(xOffset + curPoint.x * blockSize + margin,
                        yOffset + curPoint.y * blockSize + blockSize - margin),
                    QPoint(xOffset + curPoint.x * blockSize + blockSize * 3 / 4 - margin,
                        yOffset + curPoint.y * blockSize + blockSize / 2),
                }));
                break;
            case Game::Direction::Right:
                painter.drawPolygon(QPolygon({
                    QPoint(xOffset + curPoint.x * blockSize + blockSize - margin,
                        yOffset + curPoint.y * blockSize + margin),
                    QPoint(xOffset + curPoint.x * blockSize + blockSize - margin,
                        yOffset + curPoint.y * blockSize + blockSize - margin),
                    QPoint(xOffset + curPoint.x * blockSize + blockSize / 4 + margin,
                        yOffset + curPoint.y * blockSize + blockSize / 2),
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
            painter.drawPie(xOffset + curPoint.x * blockSize + margin,
                yOffset + curPoint.y * blockSize - blockSize + 3 * margin,
                2 * blockSize - 4 * margin, 2 * blockSize - 4 * margin,
                180 * 16, 90 * 16);
        }
        else if ((previousDirection == Game::Direction::Up && nextDirection == Game::Direction::Left) ||
            (previousDirection == Game::Direction::Left && nextDirection == Game::Direction::Up))
        {
            painter.drawPie(xOffset + curPoint.x * blockSize - blockSize + 3 * margin,
                yOffset + curPoint.y * blockSize - blockSize + 3 * margin,
                2 * blockSize - 4 * margin, 2 * blockSize - 4 * margin,
                270 * 16, 90 * 16);
        }
        else if ((previousDirection == Game::Direction::Down && nextDirection == Game::Direction::Right) ||
            (previousDirection == Game::Direction::Right && nextDirection == Game::Direction::Down))
        {
            painter.drawPie(xOffset + curPoint.x * blockSize + margin,
                yOffset + curPoint.y * blockSize + margin,
                2 * blockSize - 4 * margin, 2 * blockSize - 4 * margin,
                90 * 16, 90 * 16);
        }
        else if ((previousDirection == Game::Direction::Down && nextDirection == Game::Direction::Left) ||
            (previousDirection == Game::Direction::Left && nextDirection == Game::Direction::Down))
        {
            painter.drawPie(xOffset + curPoint.x * blockSize - blockSize + 3 * margin,
                yOffset + curPoint.y * blockSize + margin,
                2 * blockSize - 4 * margin, 2 * blockSize - 4 * margin,
                0 * 16, 90 * 16);
        }
        else
        {
            painter.drawRect(xOffset + curPoint.x * blockSize + margin, yOffset + curPoint.y * blockSize + margin,
                blockSize - 2 * margin, blockSize - 2 * margin);
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
        int sizeFactor = std::min((3 + 2 * (3 - status.map[food.x][food.y].y)) * margin, blockSize / 2 - 1);
        painter.drawEllipse(xOffset + sizeFactor + food.x * blockSize,
            yOffset + sizeFactor + food.y * blockSize,
            blockSize - 2 * sizeFactor, blockSize - 2 * sizeFactor);
    }

    // draw obstacles
    painter.setBrush(Qt::black);
    painter.setPen(Qt::transparent);
    auto obstacles = status.mapDefinition.obstacles;
    for (const auto &obstacle: obstacles)
    {
        painter.drawRect(xOffset + margin + obstacle.x * blockSize,
            yOffset + margin + obstacle.y * blockSize,
            blockSize - 2 * margin, blockSize - 2 * margin);
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
            const int xCenter = xOffset + p.x * blockSize + blockSize / 2,
                yCenter = yOffset + p.y * blockSize + blockSize / 2;

            painter.drawPolygon(QPolygon({
                QPoint(xCenter, yCenter - blockSize / 2 + 2 * margin),
                QPoint(xCenter + blockSize / 2 - 2 * margin, yCenter),
                QPoint(xCenter, yCenter + blockSize / 2 - 2 * margin),
                QPoint(xCenter - blockSize / 2 + 2 * margin, yCenter),
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
