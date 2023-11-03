#include "resultpage.h"
#include "ui_resultpage.h"

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
