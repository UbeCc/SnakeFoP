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

PlayPage::PlayPage(QWidget *parent) :
    steps(0),
    widget(dynamic_cast<Widget *>(parent)),
    game(nullptr),
    ui(new Ui::PlayPage),
    gameTimer(new QTimer(this)),
    resultPage(new ResultPage(this)) {
    ui->setupUi(this);    
    connect(gameTimer, &QTimer::timeout, this, [&]() {
        ++steps;
        game->SetPreDirection(game->GetStatus().direction);
        Step();
    });
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
    RecordManager::SaveRecord(formattedDateTime.toStdString() + ".rec", Widget::GetRecord());
    resultPage->exec();
    this->done(0);
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
}

void PlayPage::Step() {
    int tot = game->Step();
    for(int i = 1; i <= tot; ++i) Widget::UpdateTime(steps), qDebug()<<steps;
    const auto &status = game->GetStatus();

    if (status.state == Game::Dead) {
        // use UpdateRecordMovement to memorize `endtime`
        Widget::UpdateTime(steps);
        qDebug() << steps;
        Widget::UpdateRecordMovement('W');
        gameOver();
    }

    ui->ScoreLabel->setText(QString::number(status.score));
    ui->LengthLabel->setText(QString::number(status.length));

    ui->Canvas->update();
}

void PlayPage::keyPressEvent(QKeyEvent *event) {
    Game::Direction direction = game->GetStatus().preDirection;
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        if (direction != Game::Down) {
            game->ChangeDirection(Game::Up);
            Widget::UpdateTime(steps);
            qDebug() << steps;
            Widget::UpdateRecordMovement('W');
        }
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        if (direction != Game::Up) {
            game->ChangeDirection(Game::Down);
            Widget::UpdateTime(steps);
            qDebug() << steps;
            Widget::UpdateRecordMovement('S');
        }
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        if (direction != Game::Right) {
            game->ChangeDirection(Game::Left);
            Widget::UpdateTime(steps);
            qDebug() << steps;
            Widget::UpdateRecordMovement('A');
        }
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        if (direction != Game::Left) {
            game->ChangeDirection(Game::Right);
            Widget::UpdateTime(steps);
            qDebug() << steps;
            Widget::UpdateRecordMovement('D');
        }
        break;
    case Qt::Key_Space:
        gameTimer->stop();
        QMessageBox::information(this, "Paused", "Press OK to continue");
        gameTimer->start();
        break;
    default:
        break;
    }
}

int PlayPage::getScore() {
    return game->GetStatus().score;
}

int PlayPage::getLength() {
    return game->GetStatus().length;
}
