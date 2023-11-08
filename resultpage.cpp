#include "resultpage.h"
#include "ui_resultpage.h"
#include "widget.h"

ResultPage::ResultPage(QWidget *parent) :
    QDialog(parent),
    playPage(dynamic_cast<PlayPage *>(parent)),
    ui(new Ui::ResultPage)
{
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(ui->replayButton, &QPushButton::clicked, this, &ResultPage::OnReplayButtonClicked);
}

ResultPage::~ResultPage()
{
    delete ui;
}

void ResultPage::OnReplayButtonClicked()
{
        foreach(QWidget *widget, QApplication::topLevelWidgets())
        {
            if (dynamic_cast<Widget *>(widget) != nullptr)
            {
                this->close();
                widget->show();
                return;
            }
        }
}


void ResultPage::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->scoreLabel->setText("您的最终得分是: " + QString::number(playPage->getScore()));
    ui->lengthLabel->setText("您的最终长度是: " + QString::number(playPage->getLength()));
}
