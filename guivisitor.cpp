#include "guivisitor.h"

#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCoreApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QSlider>
#include <QFileInfo>
#include <QObject>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>

GUIVisitor::GUIVisitor()
    : resultWidget(new QWidget()),
      mainLayout(new QVBoxLayout())
{
    resultWidget->setLayout(mainLayout);
}

void GUIVisitor::clear() {
    QLayoutItem* child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
}

void GUIVisitor::visit(const Library::Book& book) {
    clear();

    QHBoxLayout* hLayout = new QHBoxLayout();

    QString path = QCoreApplication::applicationDirPath() + "/Resources/images/" +
                   QString::fromStdString(book.getImage());
    QLabel* imageLabel = new QLabel;
    qDebug() << "Percorso immagine (dal DB):" << path;
    QPixmap image(path);
    if (!image.isNull()){
        imageLabel->setPixmap(image.scaled(250, 250, Qt::KeepAspectRatio));
    }
    else
        imageLabel->setText("Image not found");
    hLayout->addWidget(imageLabel);
    hLayout->setAlignment(imageLabel, Qt::AlignLeft | Qt::AlignTop);

    QFormLayout* form = new QFormLayout;
    form->addRow("Title:", new QLabel(QString::fromStdString(book.getTitle())));
    form->addRow("Author:", new QLabel(QString::fromStdString(book.getAuthor())));
    form->addRow("Year:", new QLabel(QString::number(book.getYear())));
    form->addRow("Genre:", new QLabel(QString::fromStdString(book.getGenre())));
    form->addRow("Publisher:", new QLabel(QString::fromStdString(book.getPublisher())));
    form->addRow("Translator:", new QLabel(QString::fromStdString(book.getTranslator())));
    form->addRow("Pages:", new QLabel(QString::number(book.getPages())));

    QWidget* formWidget = new QWidget;
    formWidget->setLayout(form);
    hLayout->addWidget(formWidget);

    mainLayout->addLayout(hLayout);
}

void GUIVisitor::visit(const Library::Movie& movie) {
    clear();

    QHBoxLayout* hLayout = new QHBoxLayout();
    QString path = QCoreApplication::applicationDirPath() + "/Resources/images/" +
                   QString::fromStdString(movie.getImage());
    QLabel* imageLabel = new QLabel;
    QPixmap image(path);
    if (!image.isNull())
        imageLabel->setPixmap(image.scaled(150, 200, Qt::KeepAspectRatio));
    else
        imageLabel->setText("Image not found");
    hLayout->addWidget(imageLabel);

    QFormLayout* form = new QFormLayout;
    form->addRow("Title:", new QLabel(QString::fromStdString(movie.getTitle())));
    form->addRow("Author:", new QLabel(QString::fromStdString(movie.getAuthor())));
    form->addRow("Year:", new QLabel(QString::number(movie.getYear())));
    form->addRow("Genre:", new QLabel(QString::fromStdString(movie.getGenre())));
    form->addRow("Language:", new QLabel(QString::fromStdString(movie.getLanguage())));
    form->addRow("Min Age:", new QLabel(QString::number(movie.getMinAge())));
    form->addRow("Duration:", new QLabel(QString::number(movie.getDuration())));
    form->addRow("Oscar:", new QLabel(movie.getOscar() ? "Yes" : "No"));

    QWidget* formWidget = new QWidget;
    formWidget->setLayout(form);
    hLayout->addWidget(formWidget);

    // Aggiungi il player video solo se il film ha un trailer
    if (!movie.getTrailer().empty()) {
        QVBoxLayout* videoLayout = new QVBoxLayout();

        // Crea il player multimediale per video
        QMediaPlayer* player = new QMediaPlayer(resultWidget);
        QVideoWidget* videoWidget = new QVideoWidget(resultWidget);
        player->setVideoOutput(videoWidget);

        // Costruisci il percorso del file video
        QString videoPath = QCoreApplication::applicationDirPath() + "/Resources/trailer/" +
                           QString::fromStdString(movie.getTrailer());

        // Verifica se il file esiste
        if (QFileInfo::exists(videoPath)) {
            player->setSource(QUrl::fromLocalFile(videoPath));

            // Crea i controlli del player
            QSlider* progressSlider = new QSlider(Qt::Horizontal);
            progressSlider->setRange(0, 100);

            QPushButton* playButton = new QPushButton("Play");
            QPushButton* pauseButton = new QPushButton("Pause");
            QPushButton* stopButton = new QPushButton("Stop");

            // Layout per i pulsanti
            QHBoxLayout* buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(playButton);
            buttonLayout->addWidget(pauseButton);
            buttonLayout->addWidget(stopButton);

            // Connetti i pulsanti alle funzioni del player
            connect(playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
            connect(pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);
            connect(stopButton, &QPushButton::clicked, [player, progressSlider]() {
                player->stop();
                player->setPosition(0);
                progressSlider->setValue(0);
            });

            // Aggiorna lo slider durante la riproduzione
            connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 position) {
                if (player->duration() > 0) {
                    progressSlider->setValue(static_cast<int>((position * 100) / player->duration()));
                }
            });

            // Permetti di cercare una posizione con lo slider
            connect(progressSlider, &QSlider::sliderMoved, player, [=](int value) {
                if (player->duration() > 0) {
                    player->setPosition((value * player->duration()) / 100);
                }
            });

            // Aggiungi i controlli al layout
            videoLayout->addWidget(new QLabel("Trailer:"));
            videoLayout->addWidget(videoWidget);
            videoLayout->addWidget(progressSlider);
            videoLayout->addLayout(buttonLayout);

            // Imposta una dimensione fissa per il video
            videoWidget->setMinimumSize(320, 240);
        } else {
            videoLayout->addWidget(new QLabel("File video non trovato: " + videoPath));
        }

        hLayout->addLayout(videoLayout);
    }

    mainLayout->addLayout(hLayout);
}

void GUIVisitor::visit(const Library::CD& cd) {
    clear();

    QHBoxLayout* hLayout = new QHBoxLayout();

    QString path = QCoreApplication::applicationDirPath() + "/Resources/images/" +
                   QString::fromStdString(cd.getImage());
    QLabel* imageLabel = new QLabel;
    QPixmap image(path);
    if (!image.isNull())
        imageLabel->setPixmap(image.scaled(150, 200, Qt::KeepAspectRatio));
    else
        imageLabel->setText("Image not found");
    hLayout->addWidget(imageLabel);

    QFormLayout* form = new QFormLayout;
    form->addRow("Title:", new QLabel(QString::fromStdString(cd.getTitle())));
    form->addRow("Author:", new QLabel(QString::fromStdString(cd.getAuthor())));
    form->addRow("Year:", new QLabel(QString::number(cd.getYear())));
    form->addRow("Genre:", new QLabel(QString::fromStdString(cd.getGenre())));
    form->addRow("Album:", new QLabel(QString::fromStdString(cd.getAlbum())));
    form->addRow("Language:", new QLabel(QString::fromStdString(cd.getLanguage())));
    form->addRow("Duration:", new QLabel(QString::number(cd.getDuration())));

    QWidget* formWidget = new QWidget;
    formWidget->setLayout(form);
    hLayout->addWidget(formWidget);

    // Aggiungi il player musicale solo se il CD ha una traccia audio
    if (!cd.getaudioTrack().empty()) {
        QVBoxLayout* playerLayout = new QVBoxLayout();

        // Crea il player multimediale
        QMediaPlayer* player = new QMediaPlayer(resultWidget);
        QAudioOutput* audioOutput = new QAudioOutput(resultWidget);
        player->setAudioOutput(audioOutput);

        // Imposta il volume a un livello ragionevole
        audioOutput->setVolume(50);

        // Costruisci il percorso del file audio
        QString audioPath = QCoreApplication::applicationDirPath() + "/Resources/audio/" +
                           QString::fromStdString(cd.getaudioTrack());

        // Verifica se il file esiste
        if (QFileInfo::exists(audioPath)) {
            player->setSource(QUrl::fromLocalFile(audioPath));

            // Crea i controlli del player
            QSlider* progressSlider = new QSlider(Qt::Horizontal);
            progressSlider->setRange(0, 100);

            QPushButton* playButton = new QPushButton("Play");
            QPushButton* pauseButton = new QPushButton("Pause");
            QPushButton* stopButton = new QPushButton("Stop");

            // Layout per i pulsanti
            QHBoxLayout* buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(playButton);
            buttonLayout->addWidget(pauseButton);
            buttonLayout->addWidget(stopButton);

            // Connetti i pulsanti alle funzioni del player
            connect(playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
            connect(pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);
            connect(stopButton, &QPushButton::clicked, player, &QMediaPlayer::stop);

            // Aggiorna lo slider durante la riproduzione
            connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 position) {
                if (player->duration() > 0) {
                    progressSlider->setValue(static_cast<int>((position * 100) / player->duration()));
                }
            });

            // Permetti di cercare una posizione con lo slider
            connect(progressSlider, &QSlider::sliderMoved, player, [=](int value) {
                if (player->duration() > 0) {
                    player->setPosition((value * player->duration()) / 100);
                }
            });

            // Aggiungi i controlli al layout
            playerLayout->addWidget(new QLabel("Riproduci traccia:"));
            playerLayout->addWidget(progressSlider);
            playerLayout->addLayout(buttonLayout);
        } else {
            playerLayout->addWidget(new QLabel("File audio non trovato: " + audioPath));
        }

        hLayout->addLayout(playerLayout);
    }

    mainLayout->addLayout(hLayout);
}
