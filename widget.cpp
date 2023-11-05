#include "widget.h"
#include "ui_widget.h"

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

void Widget::ResetRecord(Map map, Config config) {
    foodPtr = 0;
    gameRecord.reset(map, config);
}

void Widget::UpdateRecordFood(int x, int y, int v) {
    gameRecord.sequence += "F";
    gameRecord.foodSequence.push_back(make_pair(Point{x, y}, v));
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

pair<Point, int> Widget::GetNextFood() {
    return gameRecord.foodSequence[foodPtr++];
}
