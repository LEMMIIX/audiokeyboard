#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// std import
#include <vector>

// QT import
#include <QMainWindow>
#include <QPushButton>

// lib import
#include "../lib/rtaudio/RtAudio.h"

// eigene Dateien imort

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

    /**
     * @brief keyPressEvent aktiviert und setzte Farbe des entsprechenden Knopfes
     * 		wenn eine bestimmte Taste gedrückt wird
     * @param event Key event bei Tastendruck
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @brief keyReleaseEvent setzt beim loslassen der Taste den entsprechenden Knop
     * 		auf den default Zustand zurück
     * @param event Key event bei Tastendruck
     */
    void keyReleaseEvent(QKeyEvent* event);

    /**
     * @brief setup_combobox Erstellen und definieren der Combobox, zeigt alle
     * 		zur Verfügung stehenden (output) Geräte an
     */
    void setup_combobox(void);

    /**
     * @brief set_audiodevice Setzt das Output Gerät anhand der combobox
     */
    void set_audiodevice(void);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

private:
    Ui::MainWindow *ui;

    /**
     * @brief adc Audio Instanz von RtAudio lib
     */
    RtAudio adc;

    /**
     * @brief get_index_of_key
     * @return index des entsprechendes keys der gedrückt wurde
     */
    int get_index_of_key(int);
};


/**
  * @brief keyboard Logik zum händeln der Tastenanschläge
  * @param outputBuffer
  * @param InputBuffer
  * @param nBufferFrames
  * @param streamTime
  * @param status
  * @param userData
  * @return 0 wenn Funkction erfolgreich läuft
  */
int keyboard(void* outputBuffer, void* InputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void* userData);

#endif // MAINWINDOW_H
