#include "resultpage.h"
#include "ui_resultpage.h"
#include "status.h"

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
    foreach(QWidget *widget, QApplication::allWidgets()) {
        qDebug() << widget->windowTitle();
        QString title = widget->windowTitle();
        if(title == "Widget" && widget->isWindow()) {
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
