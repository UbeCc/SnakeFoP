#include "playpage.h"
#include "ui_playpage.h"
#include <QPainter>
#include <QTimer>
#include <QString>
#include <QKeyEvent>
#include <QMessageBox>
#include "game.h"
#include "recordmanager.h"
#include <QDir>

using namespace std;

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

PlayPage::PlayPage(QWidget *parent) :
        widget(dynamic_cast<Widget *>(parent)),
        game(nullptr),
        ui(new Ui::PlayPage),
        gameTimer(new QTimer(this)),
        resultPage(new ResultPage(this)) {
    ui->setupUi(this);
    gameCanvas = new GameCanvas(this);
    ui->horizontalLayout_2->replaceWidget(ui->GameCanvas, gameCanvas);
    delete ui->GameCanvas;
    ui->GameCanvas = gameCanvas;
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(Step()));
}

PlayPage::~PlayPage() {
    delete ui;
}

void PlayPage::gameOver() {
    gameTimer->stop();
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    QDateTime dateTime;
    dateTime.setMSecsSinceEpoch(timestamp);
    QString format = "yyyy-MM-dd-hh-mm-ss";
    QString formattedDateTime = dateTime.toString(format);
    QString currentDirectory = QDir::currentPath();
//    qDebug() << "Current Working Directory: " << currentDirectory << " " << formattedDateTime << "\n";
    RecordManager::SaveRecord(formattedDateTime.toStdString() + ".rec", Widget::GetRecord());
    this->done(0);
    resultPage->show();
}

void PlayPage::initPlay() {
    Widget::mode = false;
    auto map = MapManager::LoadMap(widget->GetGameMapPath().filePath().toStdString());
    auto config = ConfigManager::LoadConfig(widget->GetGameConfigPath().filePath().toStdString());
    Widget::ResetRecord(map, config);
    game = new Game(map, config, 0);
    ui->ConfigLabel->setText(widget->GetGameConfigPath().fileName());
    ui->MapLabel->setText(widget->GetGameMapPath().fileName());
    ui->ScoreLabel->setText("0");
    ui->LengthLabel->setText("1");
    gameCanvas->SetGame(game);
    auto &status = game->GetStatus();
    gameTimer->start((int) (TIME_INTERVAL * (1. / status.config.level)));
    gameElapsedTimer.start();
    startTime = gameElapsedTimer.elapsed();
}

void PlayPage::Step() {
    int tot = game->Step();
    for(int i = 1; i <= tot; ++i) Widget::UpdateTime(gameElapsedTimer.elapsed() - startTime);
    const auto &status = game->GetStatus();

    if (status.state == Game::Dead) {
        // use UpdateRecordMovement to memorize `endtime`
        Widget::UpdateTime(gameElapsedTimer.elapsed() - startTime);
        Widget::UpdateRecordMovement('W');
        gameOver();
    }

    ui->ScoreLabel->setText(QString::number(status.score));
    ui->LengthLabel->setText(QString::number(status.length));

    ui->GameCanvas->update();
}

void PlayPage::keyPressEvent(QKeyEvent *event) {
    Game::Direction direction = game->GetStatus().direction;
    switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_W:
            if (direction != Game::Down) {
                game->ChangeDirection(Game::Up);
                Widget::UpdateRecordMovement('W');
            }
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            if (direction != Game::Up) {
                game->ChangeDirection(Game::Down);
                Widget::UpdateRecordMovement('S');
            }
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            if (direction != Game::Right) {
                game->ChangeDirection(Game::Left);
                Widget::UpdateRecordMovement('A');
            }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            if (direction != Game::Left) {
                game->ChangeDirection(Game::Right);
                Widget::UpdateRecordMovement('D');
            }
            break;
        default:
            break;
    }
    Widget::UpdateTime(gameElapsedTimer.elapsed() - startTime);
}

int PlayPage::getScore() {
    return game->GetStatus().score;
}

int PlayPage::getLength() {
    return game->GetStatus().length;
}
