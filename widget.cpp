#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent),
          ui(new Ui::MainPage),
          settingPage(new SettingPage(this)),
          playPage(new PlayPage(this)),
          gameConfigPath("./config/default.txt"),
          gameMapPath("./maps/test_map.txt") {
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
}

Widget::~Widget() {
    delete ui;
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
