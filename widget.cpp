#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
        : QWidget(parent),
          ui(new Ui::MainPage),
          settingPage(new SettingPage(this)),
          replayPage(new RePlayPage(this)),
          playPage(new PlayPage(this)),
          gameConfigPath("./config/default.txt"),
          gameMapPath("./maps/test_map.txt") {
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_replayButton_clicked() {
    this->hide();
    QString recordFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(recordFilePath);
    replayPage->initPlay(fileInfo);
    replayPage->show();
}

void Widget::on_enterButton_clicked() {
    if (settingPage->exec() == QDialog::Accepted) {
        this->hide();
        playPage->initPlay();
        playPage->show();
    }
}

void Widget::SetGameConfigPath(const QFileInfo &path) {
    gameConfigPath = path;
}

void Widget::SetGameMapPath(const QFileInfo &path) {
    gameMapPath = path;
}

QFileInfo Widget::GetGameConfigPath() const {
    return gameConfigPath;
}

QFileInfo Widget::GetGameMapPath() const {
    return gameMapPath;
}

void Widget::ResetRecord(const Map& map, Config config) {
    foodPtr = 0;
    gameRecord.reset(map, config);
}

void Widget::ResetRecord(const Record& record) {
    foodPtr = 0;
    gameRecord.reset(record);
}

void Widget::UpdateRecordFood(int x, int y, int v) {
    gameRecord.sequence += "F";
    gameRecord.foodSequence.emplace_back(Point{x, y}, v);
}

void Widget::UpdateRecordMovement(char movement) {
    gameRecord.sequence += "M";
    gameRecord.moveSequence += movement;
}

Record Widget::GetRecord() {
    return gameRecord;
}

int Widget::foodPtr = 0;
Record Widget::gameRecord;

void Widget::PrintFood() {
    for(const auto & i : gameRecord.foodSequence) {
        qDebug() << i.first.x << " " << i.first.y << "\n";
    }
}

pair<Point, int> Widget::GetNextFood() {
    return gameRecord.foodSequence[foodPtr++];
}

Map Widget::GetMap() {
    return gameRecord.map;
}

Config Widget::GetConfig() {
    return gameRecord.config;
}
