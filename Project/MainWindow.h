#ifndef MAINWINDOW
#define MAINWINDOW

#include <QFileInfoList>
#include <QStackedLayout>
#include <QTableWidget>
#include <QProgressBar>
#include "SongFile.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    private:

        static const int minimumRefs { 3 };

        QVector<SongFile> m_songs;

        QStackedLayout *mp_layout;

        QTableWidget *mp_resultTable;

        QProgressBar *mp_progressBar;


        QWidget* createStartPage();

        QWidget* createResultTable();

        QFileInfoList loadFiles(const QString& dirname) const;

        void fillSong(SongFile& song, const QString& separator);

        void fillSong(SongFile& song, const QString& author, const QString& title);

        void displayResults();

    private slots:

        void computeTags();

    signals:

        void processedSongs(int songsNb);

    public:

        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();
};

#endif // MAINWINDOW

