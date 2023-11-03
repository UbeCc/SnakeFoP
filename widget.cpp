#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainPage)
{
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_enterButton_clicked() {
    this->hide();
    settingPage->show();
}
