#ifndef MAINWINDOW
#define MAINWINDOW

#include <QFileInfoList>
#include <QStackedLayout>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>
#include "SongFile.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    private:

        enum ResultColumn { FILENAME, AUTHOR, TITLE, FILENAME_CHECKBOX, REVERSE, COLUMNS_COUNT };

        QVector<SongFile> m_songs;

        QStackedLayout *mp_layout;

        QTableWidget *mp_resultTable;

        QProgressBar *mp_progressBar;

        QLineEdit *mp_dirPath;

        QLabel *mp_filesCountLabel;

        QListWidget *mp_SongsPreviewList;

        QList<QCheckBox*> mp_renamingChechBoxes;


        QWidget* createStartPage();

        QWidget* createResultTable();

        static QWidget* createCenteredCheckBox(QCheckBox *checkBox);

        static QTableWidgetItem* createEmptyTableItem();

        QFileInfoList loadFiles(const QString& dirPath) const;

        void openDir(const QString& dirPath);

        void displayResults();

    private slots:

        void computeTags();

        void selectDir();

        void reverse(int row) const;

        void checkAll(bool value) const;

    public:

        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();
};

#endif // MAINWINDOW

