#include "replaypage.h"
#include "ui_replaypage.h"
#include <QPainter>
#include <QTimer>
#include <QElapsedTimer>
#include <QString>
#include <QKeyEvent>
#include <QMessageBox>
#include "game.h"
#include "playpage.h"
#include "recordmanager.h"
#include <QDir>
#include <QFileDialog>

using namespace std;

void RePlayPage::exitButton_clicked() {
    gameOver();
}

void RePlayPage::recordButton_clicked() {
    QString recordFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(recordFilePath);
    ui->RecordLabel->setText(fileInfo.fileName());
    record = RecordManager::LoadRecord(fileInfo.filePath().toStdString(), Widget::gameRecord);
}

void RePlayPage::playButton_clicked() {
    if(playFlag) {
        playFlag = false;
        gameTimer->stop();
        gameIntervalTimer->stop();
        res = Widget::GetRecord().timestamp[curStep] - Widget::GetRecord().timestamp[curStep - 1] - gameElapsedTimer->elapsed();
    } else {
        playFlag = true;
        gameTimer->start();
        gameIntervalTimer->start(res);
        gameElapsedTimer->restart();
        res = 0;
    }
}

RePlayPage::RePlayPage(QWidget *parent) :
    initFlag(false),
    playFlag(false),
    widget(dynamic_cast<Widget *>(parent)),
    game(nullptr),
    ui(new Ui::RePlayPage),
    gameTimer(nullptr),
    gameIntervalTimer(nullptr),
    gameElapsedTimer(nullptr) {
    ui->setupUi(this);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(1, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::recordButton_clicked);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(2, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::playButton_clicked);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(3, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::exitButton_clicked);
}

RePlayPage::~RePlayPage() {
    delete ui;
}

void RePlayPage::initPlay(const QFileInfo& fileInfo) {
    curStep = 0;
    Widget::mode = true;
    ui->RecordLabel->setText(fileInfo.fileName());
    Widget::ResetRecord(RecordManager::LoadRecord(fileInfo.filePath().toStdString(), Widget::gameRecord));
    game = new Game(Widget::GetMap(), Widget::GetConfig(), 1);
    ui->Canvas->SetGame(game);
    auto &status = game->GetStatus();
    game->SetStatus(Game::Alive);
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [&]() {
        game->Step();
        ui->ScoreLabel->setText(QString::number(status.score));
        ui->LengthLabel->setText(QString::number(status.length));
        ui->Canvas->update();
        if(Widget::IsEnd()) gameOver();
    });
    curStep = Widget::seqPtr;
    gameIntervalTimer = new QTimer(this);
    gameElapsedTimer = new QElapsedTimer();
    gameIntervalTimer->setSingleShot(true);
    connect(gameIntervalTimer, &QTimer::timeout, this, [&]() {
        Step();
        ++curStep;
        if(curStep == (int)Widget::GetRecord().timestamp.size()) return;
        int delta = Widget::GetRecord().timestamp[curStep] - Widget::GetRecord().timestamp[curStep - 1];
        qDebug() << delta << " " << gameElapsedTimer->elapsed();
        gameIntervalTimer->start(max(0, delta));
        gameElapsedTimer->restart();
    });
    gameTimer->start((int)(TIME_INTERVAL * (1. / status.config.level)));
    gameElapsedTimer->start();
    gameIntervalTimer->start(Widget::GetRecord().timestamp[curStep]);
}

void RePlayPage::gameOver() {
    gameTimer->stop();
    gameIntervalTimer->stop();
    gameElapsedTimer->invalidate();
    this->done(0);
    widget->show();
}

void RePlayPage::Step() {
    auto status = game->GetStatus();
    char curOp =Widget::NextAction();
//    qDebug() << curOp << "\n";
    if(curOp == 'M') {
        Game::Direction direction = game->GetStatus().direction;
        char dire = Widget::GetNextMovement();
        //DSAW
        switch (dire) {
        case 'W':
            if (direction != Game::Down) game->ChangeDirection(Game::Up);
            break;
        case 'S':
            if (direction != Game::Up) game->ChangeDirection(Game::Down);
            break;
        case 'A':
            if (direction != Game::Right) game->ChangeDirection(Game::Left);
            break;
        case 'D':
            if (direction != Game::Left) game->ChangeDirection(Game::Right);
            break;
        default:
            break;
        }
    } else if(curOp == 'F') {
        auto food = Widget::GetNextFood();
        int x = food.first.x, y = food.first.y;
        status.map[x][y].y = food.second;
        status.foods.push_back({x, y});
    }
}

int RePlayPage::getScore() {
    return game->GetStatus().score;
}

int RePlayPage::getLength() {
    return game->GetStatus().length;
}
