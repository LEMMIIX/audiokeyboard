#include <QRegularExpression>
#include <QKeyEvent>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);

    keys.push_back(Qt::Key_A);
    keys.push_back(Qt::Key_S);
    keys.push_back(Qt::Key_D);
    keys.push_back(Qt::Key_F);
    keys.push_back(Qt::Key_G);
    keys.push_back(Qt::Key_H);
    keys.push_back(Qt::Key_J);
    keys.push_back(Qt::Key_K);
    keys.push_back(Qt::Key_L);
    keys.push_back(Qt::Key_Odiaeresis);
    keys.push_back(Qt::Key_Adiaeresis);
    keys.push_back(Qt::Key_NumberSign);

    int i = 0;
    const QList<QPushButton*> buttons = ui->centralwidget->findChildren<QPushButton*>(QRegularExpression("^key_"));
    for(QPushButton* button : buttons) {
        button->setDisabled(true);
        button->setStyleSheet(style);
        mapKeyButton[keys[i]] = button;
        ++i;
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if(mapKeyButton.contains(event->key())) {
        QPushButton* button = mapKeyButton[event->key()];
        QPalette pal = button->palette();
        pal.setColor(QPalette::Button, QColor(199, 199, 199));
        button->setPalette(pal);
        button->setAutoFillBackground(true);
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    if(mapKeyButton.contains(event->key())) {
        QPushButton* button = mapKeyButton[event->key()];
        button->setPalette(this->palette());
    }
    QWidget::keyPressEvent(event);
}
