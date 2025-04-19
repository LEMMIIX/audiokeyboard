#include <QRegularExpression>
#include <QKeyEvent>
#include <QObject>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "soundwave.h"

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

    activeTones.resize(keys.size());
    for(int i = 0; i < keys.size(); ++i) {
        activeTones[i].active = false;
        activeTones[i].frequency = FREQUENCY + 10.0 * i;
        activeTones[i].phase = 0.0;
    }

    setup_combobox();

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::set_audiodevice);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::get_index_of_key(int key) {
    int index = 0;

    for(int i = 0; i <= keys.size() && (key != keys[i]); ++i) {
        ++index;
    }

    return index;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if(mapKeyButton.contains(event->key())) {
        QPushButton* button = mapKeyButton[event->key()];
        QPalette pal = button->palette();
        pal.setColor(QPalette::Button, QColor(199, 199, 199));
        button->setPalette(pal);
        button->setAutoFillBackground(true);

        int keyIndex = get_index_of_key(event->key());
        activeTones[keyIndex].active = true;
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    if(mapKeyButton.contains(event->key())) {
        QPushButton* button = mapKeyButton[event->key()];
        button->setPalette(this->palette());

        int keyIndex = get_index_of_key(event->key());
        activeTones[keyIndex].active = false;
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::setup_combobox(void) {
    QStringList available_devices;
    std::vector<unsigned int> device_ids = adc.getDeviceIds();
    if(device_ids.size() == 0) {
        available_devices << "Keine Audiogeräte gefunden";
    } else {
        std::cout << "Device ids: " << std::endl;
        for(unsigned int i : device_ids) {
            std::cout << i << "\t" << adc.getDeviceInfo(i).name << std::endl;
            available_devices << QString::fromUtf8((adc.getDeviceInfo(i).name).c_str());	// Gerätenamen die erkannt wurden werden der Liste inzugefügt
        }
    }

    ui->comboBox->insertItems(0, available_devices);
}

void MainWindow::set_audiodevice(void) {
    // vorherige Audiostreams löschen
    if(adc.isStreamRunning()) adc.stopStream();
    if(adc.isStreamOpen()) adc.closeStream();

    std::vector<unsigned int> device_ids = adc.getDeviceIds();
    o_params.deviceId = device_ids[ui->comboBox->currentIndex()];
    std::cout << ui->comboBox->currentIndex() << "\t gewählte device Id: " << device_ids[ui->comboBox->currentIndex()]<<  std::endl;
    std::cout << "o_params ID: " << o_params.deviceId << std::endl;
    o_params.nChannels = 2;
    o_params.firstChannel = 0;
    sampleRate = 44100;
    bufferFrames = 256;

    if(adc.openStream(&o_params, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &keyboard, &activeTones)) {
        std::cout << adc.getErrorText() << std::endl;
    }

    if(adc.startStream()) {
        std::cout << adc.getErrorText() << std::endl;
    }
}


int keyboard(void* outputBuffer, void* InputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void* userData) {
    return single_tone(outputBuffer, InputBuffer, nBufferFrames, streamTime, status, userData);
}
