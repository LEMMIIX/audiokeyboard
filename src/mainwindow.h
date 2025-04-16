#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    std::vector<int> keys;	// Tastatur-Tasten die gemappt werden sollen
    QHash<int, QPushButton*> mapKeyButton;	// Map von Taste zu Knopf
    QString style = "QPushButton { min-width: 50px; font: 12pt \"Courier New\"; max-width: 50px; min-height: 50px; max-height: 50px; border-color: rgb(0, 0, 0); border: 2px solid #000000; border-radius: 19px; }";

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
