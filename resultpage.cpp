#include "resultpage.h"
#include "ui_resultpage.h"
#include "status.h"
#include "widget.h"

ResultPage::ResultPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultPage)
{
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
}

ResultPage::~ResultPage()
{
    delete ui;
}

void ResultPage::on_replayButton_clicked() {
    foreach(QWidget *widget, QApplication::topLevelWidgets()) {
        if (dynamic_cast<Widget*>(widget) != nullptr) {
            this->close();
            widget->show();
            return;
        }
    }
}


void ResultPage::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    ui->scoreLabel->setText("您的最终得分是: " + QString::number(gameScore));
    ui->lengthLabel->setText("您的最终长度是: " + QString::number(gameLength));
}
