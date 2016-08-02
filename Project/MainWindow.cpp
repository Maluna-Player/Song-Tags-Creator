#include "MainWindow.h"
#include <QVBoxLayout>
#include "ChoiceDialog.h"
#include <QDir>
#include <QDebug>
#include <QPushButton>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QFileInfoList files = loadFiles("../Project/songs");

    for (const QFileInfo& fileInfo : files)
        m_songs.append(SongFile(fileInfo.completeBaseName()));

    QVBoxLayout *layout = new QVBoxLayout;
    mp_layout = new QStackedLayout;

    mp_layout->addWidget(createStartPage());
    mp_layout->addWidget(createResultTable());

    QPushButton *quitButton = new QPushButton("Quit");

    layout->addWidget(new QLabel("<b>Song Tags Creator</b>"));
    layout->addLayout(mp_layout);
    layout->addWidget(quitButton);
    setLayout(layout);

    connect(this, SIGNAL(processedSongs(int)), mp_progressBar, SLOT(setValue(int)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{

}

QWidget* MainWindow::createStartPage()
{
    QWidget *startPage = new QWidget;
    QVBoxLayout *startLayout = new QVBoxLayout;

    QPushButton *startButton = new QPushButton("Start");
    connect(startButton, SIGNAL(clicked()), this, SLOT(computeTags()));

    mp_progressBar = new QProgressBar;
    mp_progressBar->setMaximum(m_songs.size() * 2);
    mp_progressBar->setValue(0);

    startLayout->addWidget(startButton);
    startLayout->addWidget(mp_progressBar);
    startPage->setLayout(startLayout);

    return startPage;
}

QWidget* MainWindow::createResultTable()
{
    mp_resultTable = new QTableWidget(m_songs.size(), 3);

    QStringList headerLabels;
    headerLabels << "Filename" << "Author" << "Title";
    mp_resultTable->setHorizontalHeaderLabels(headerLabels);

    return mp_resultTable;
}

QFileInfoList MainWindow::loadFiles(const QString& dirname) const
{
    QDir dir(dirname);
    if (!dir.exists())
        qDebug() << "Input dir does not exist";

    return dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
}

void MainWindow::fillSong(SongFile& song, const QString& separator)
{
    auto elements = song.getFilename().split(separator);
    song.fill(elements[0], elements[1]);

    emit processedSongs(mp_progressBar->value() + 1);
}

void MainWindow::fillSong(SongFile& song, const QString& author, const QString& title)
{
    song.fill(author, title);

    emit processedSongs(mp_progressBar->value() + 1);
}

void MainWindow::computeTags()
{
    QList<SongFile*> songsToBeAsked;

    for (auto& song : m_songs)
    {
        if (song.getFilename().contains(" - "))
        {
            int count = song.getFilename().count(" - ");
            if (count == 1)
                fillSong(song, " - ");
            else
                songsToBeAsked.append(&song);
        }
        else if (song.getFilename().contains("-"))
        {
            int count = song.getFilename().count("-");
            if (count == 1)
                fillSong(song, "-");
            else
                songsToBeAsked.append(&song);
        }
        else
            fillSong(song, "", song.getFilename());
    }

    // Asked songs
    for (SongFile *currentSong : songsToBeAsked)
    {
        unsigned int maxRefs{0};
        QPair<QString,QString> bestChoice;

        const QString separator = (currentSong->getFilename().contains(" - ") ? " - " : "-");

        auto choices = currentSong->getChoices(separator);
        for (auto choice : choices)
        {
            unsigned int cpt{0};

            for (const auto& song : m_songs)
            {
                if (choice.first == song.getAuthor())
                    ++cpt;
            }

            if (cpt > maxRefs)
            {
                maxRefs = cpt;
                bestChoice = choice;
            }
        }

        if (maxRefs >= minimumRefs)
            fillSong(*currentSong, bestChoice.first, bestChoice.second);
        else
        {
            ChoiceDialog dialog(*currentSong, separator);
            auto choice = dialog.getSelectedChoice();
            fillSong(*currentSong, choice.first, choice.second);
        }
    }

    // Reversed songs
    for (auto& currentSong : m_songs)
    {
        unsigned int count{0};
        const auto& songTitle = currentSong.getTitle();

        for (const auto& song : m_songs)
        {
            if (songTitle == song.getAuthor())
                ++count;
        }

        if (count >= minimumRefs)
            currentSong.fill(currentSong.getTitle(), currentSong.getAuthor());

        emit processedSongs(mp_progressBar->value() + 1);
    }

    displayResults();
}

 void MainWindow::displayResults()
 {
     for (int i = 0; i < m_songs.size(); ++i)
     {
         mp_resultTable->setItem(i, 0, new QTableWidgetItem(m_songs.at(i).getFilename()));
         mp_resultTable->setItem(i, 1, new QTableWidgetItem(m_songs.at(i).getAuthor()));
         mp_resultTable->setItem(i, 2, new QTableWidgetItem(m_songs.at(i).getTitle()));
     }

     mp_resultTable->resizeColumnsToContents();
     mp_layout->setCurrentIndex(1);
 }
