#include <QDebug>
#include "playpage.h"
#include "ui_playpage.h"
#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QKeyEvent>
#include <QMessageBox>
#include "game.h"
#include "status.h"
#include <cstring>

using namespace std;

// extern string gameConfigPath, gameMapPath;

PlayPage::PlayPage(QWidget *parent): ui(new Ui::PlayPage)
{
    ui->setupUi(this);
}

PlayPage::~PlayPage()
{
    delete ui;
}

void PlayPage::on_btn_clicked() {
    gameOver();
}

void PlayPage::gameOver() {
    qDebug() << "test" << Qt::endl;
    this->done(0);
    resultPage->show();
}

void PlayPage::initPlay() {
    game = new Game(MapManager::LoadMap(gameMapPath), ConfigManager::LoadConfig(gameConfigPath));
    auto &status = game->GetStatus();
    int col = status.mapDefinition.height, row = status.mapDefinition.width;
    resize(MARGIN * 4 + (col + 3) * BLOCK_SIZE, MARGIN * 2 + row * BLOCK_SIZE);
    game->GenerateFood();
    QTimer *gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(Step()));
    gameTimer->start(TIME_INTERVAL * 2);
}

void PlayPage::paintEvent(QPaintEvent *event) {
    auto &status = game->GetStatus();
    int col = status.mapDefinition.height, row = status.mapDefinition.width;
    QPainter painter(this);

    // draw playground
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::blue);
    painter.drawRect(MARGIN, MARGIN, col * BLOCK_SIZE, row * BLOCK_SIZE);

    // draw snake
    painter.setBrush(Qt::red);
    painter.setPen(Qt::green);
    Point tail = status.tail;
    Point curPoint = tail;
    while(curPoint.x >= 0) {
        painter.drawRect(MARGIN + curPoint.x * BLOCK_SIZE, MARGIN + curPoint.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
        curPoint = status.map[curPoint.x][curPoint.y];
    }

    // draw foods
    auto foods = status.foods;
    for(auto i = foods.begin(); i != foods.end(); ++i) {
        painter.setBrush(Qt::white);
        painter.drawEllipse(MARGIN + i->x * BLOCK_SIZE, MARGIN + i->y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
        painter.setBrush(Qt::black);
        int x = MARGIN + i->x * BLOCK_SIZE + BLOCK_SIZE / 2;
        int y = MARGIN + i->y * BLOCK_SIZE + BLOCK_SIZE / 2;
        int xOffset = -5;
        int yOffset = 5;
        painter.drawText(x + xOffset, y + yOffset, QString::number(status.map[i->x][i->y].y));
    }

    // draw obstacles
    painter.setBrush(Qt::black);
    auto obstacles = status.mapDefinition.obstacles;
    for(auto i = obstacles.begin(); i != obstacles.end(); ++i) {
        painter.drawRect(MARGIN + i->x * BLOCK_SIZE, MARGIN + i->y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    }

    // draw portals
    auto portals = status.mapDefinition.portals;
    for(auto i = portals.begin(); i != portals.end(); ++i) {
        Point start = (*i)[0], end = (*i)[1];
        painter.setBrush(QColorConstants::Svg::darkblue);
        painter.drawEllipse(MARGIN + start.x * BLOCK_SIZE, MARGIN + start.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
        painter.setBrush(QColorConstants::Svg::midnightblue);
        painter.drawEllipse(MARGIN + end.x * BLOCK_SIZE, MARGIN + end.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(MARGIN * 3 + col * BLOCK_SIZE, MARGIN + 2 * BLOCK_SIZE, "Score " + QString::number(status.score));
}

void PlayPage::Step() {
    qDebug() << "step\n";
    game->Step();
    if(game->GetStatus().state == Game::Dead) {
        gameOver();
    }
    update();
}

void PlayPage::keyPressEvent(QKeyEvent *event) {
    bool flag = false;
    Game::Direction direction = game->GetStatus().direction;
    switch(event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        if(direction != Game::Down) game->ChangeDirection(Game::Up);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        if(direction != Game::Up) game->ChangeDirection(Game::Down);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        if(direction != Game::Right) game->ChangeDirection(Game::Left);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        if(direction != Game::Left) game->ChangeDirection(Game::Right);
        break;
    default:
        break;
    }
}
