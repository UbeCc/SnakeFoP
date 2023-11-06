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

PlayPage::PlayPage(QWidget *parent) :
        widget(dynamic_cast<Widget *>(parent)),
        game(nullptr),
        ui(new Ui::PlayPage),
        gameTimer(new QTimer(this)),
        resultPage(new ResultPage(this)) {
    ui->setupUi(this);
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
    ui->Canvas->SetGame(game);
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
        gameOver();
    }

    ui->ScoreLabel->setText(QString::number(status.score));
    ui->LengthLabel->setText(QString::number(status.length));

    ui->Canvas->update();
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
                Widget::UpdateRecordMovement('D');
            }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            if (direction != Game::Left) {
                game->ChangeDirection(Game::Right);
                Widget::UpdateRecordMovement('A');
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
