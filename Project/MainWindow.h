#ifndef MAINWINDOW
#define MAINWINDOW

#include <QFileInfoList>
#include <QStackedLayout>
#include <QTableWidget>
#include <QProgressBar>
#include <QLineEdit>
#include "SongFile.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    private:

        enum ResultColumn { FILENAME, AUTHOR, TITLE, REVERSE, COLUMNS_COUNT };

        static const int minimumRefs { 3 };

        QVector<SongFile> m_songs;

        QStackedLayout *mp_layout;

        QTableWidget *mp_resultTable;

        QProgressBar *mp_progressBar;

        QLineEdit *mp_dirPath;


        QWidget* createStartPage();

        QWidget* createResultTable();

        QFileInfoList loadFiles(const QString& dirname) const;

        void fillSong(SongFile& song, const QString& separator);

        void fillSong(SongFile& song, const QString& author, const QString& title);

        void displayResults();

    private slots:

        void computeTags();

        void openDir();

        void reverse(int row);

    signals:

        void processedSongs(int songsNb);

    public:

        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();
};

#endif // MAINWINDOW

