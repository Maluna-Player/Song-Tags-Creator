#include "MainWindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QHeaderView>
#include "TagsManager.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    mp_layout = new QStackedLayout;

    mp_layout->addWidget(createStartPage());
    mp_layout->addWidget(createResultTable());

    QPushButton *quitButton = new QPushButton("Quit");

    layout->addWidget(new QLabel("<b>Song Tags Creator</b>"));
    layout->addLayout(mp_layout);
    layout->addWidget(quitButton);
    setLayout(layout);

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{

}

QWidget* MainWindow::createStartPage()
{
    QWidget *startPage = new QWidget;
    QVBoxLayout *startLayout = new QVBoxLayout;

    QHBoxLayout *inputDirLayout = new QHBoxLayout;

    mp_dirPath = new QLineEdit;
    mp_dirPath->setReadOnly(true);
    QPushButton *browseButton = new QPushButton("Browse");

    inputDirLayout->addWidget(mp_dirPath);
    inputDirLayout->addWidget(browseButton);

    connect(browseButton, SIGNAL(clicked()), this, SLOT(selectDir()));


    QHBoxLayout *songsLayout = new QHBoxLayout;

    mp_filesCountLabel = new QLabel;
    mp_SongsPreviewList = new QListWidget;

    QPushButton *startButton = new QPushButton("Start");
    connect(startButton, SIGNAL(clicked()), this, SLOT(computeTags()));

    songsLayout->addWidget(mp_SongsPreviewList);
    songsLayout->addWidget(startButton);

    mp_progressBar = new QProgressBar;
    mp_progressBar->hide();

    startLayout->addLayout(inputDirLayout);
    startLayout->addWidget(mp_progressBar);
    startLayout->addWidget(mp_filesCountLabel);
    startLayout->addLayout(songsLayout);
    startPage->setLayout(startLayout);

    return startPage;
}

QWidget* MainWindow::createResultTable()
{
    QCheckBox *checkAllBox = new QCheckBox("Check all");
    connect(checkAllBox, SIGNAL(clicked(bool)), this, SLOT(checkAll(bool)));

    mp_resultTable = new QTableWidget(1, COLUMNS_COUNT);
    mp_resultTable->setItem(0, FILENAME, createEmptyTableItem());
    mp_resultTable->setItem(0, AUTHOR, createEmptyTableItem());
    mp_resultTable->setItem(0, TITLE, createEmptyTableItem());
    mp_resultTable->setItem(0, REVERSE, createEmptyTableItem());
    mp_resultTable->setCellWidget(0, FILENAME_CHECKBOX, createCenteredCheckBox(checkAllBox));

    QStringList headerLabels;
    headerLabels << "Filename" << "Author" << "Title" << "Rename file" << "";
    mp_resultTable->setHorizontalHeaderLabels(headerLabels);

    return mp_resultTable;
}

QFileInfoList MainWindow::loadFiles(const QString& dirPath) const
{
    QDir dir(dirPath);
    if (!dir.exists())
        qDebug() << "Input dir does not exist";

    return dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Files);
}

void MainWindow::openDir(const QString& dirPath)
{
    mp_dirPath->setText(dirPath);
    m_songs.clear();
    mp_SongsPreviewList->clear();

    QFileInfoList files = loadFiles(dirPath);

    for (const QFileInfo& fileInfo : files)
    {
        m_songs.append(SongFile(fileInfo.completeBaseName()));
        mp_SongsPreviewList->addItem(fileInfo.fileName());
    }

    mp_filesCountLabel->setText(QString::number(m_songs.size()) + " file(s)");
    mp_progressBar->setValue(0);
    mp_progressBar->setMaximum(m_songs.size());
    mp_progressBar->hide();
    mp_resultTable->setRowCount(m_songs.size() + 1);

}

void MainWindow::selectDir()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose Directory", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dirPath.isNull())
        openDir(dirPath);
}

void MainWindow::computeTags()
{
    const QString dirPath = mp_dirPath->text();
    if (dirPath.isEmpty())
    {
        QMessageBox::critical(this, "Input dir error", "Input dir path is empty", QMessageBox::Ok);
        return;
    }

    if (m_songs.isEmpty())
    {
        QMessageBox::critical(this, "Input dir error", "Input dir is empty", QMessageBox::Ok);
        return;
    }

    mp_progressBar->show();

    TagsManager manager;
    connect(&manager, SIGNAL(processedSongs(int)), mp_progressBar, SLOT(setValue(int)));

    if (!manager.compute(m_songs))
        openDir(mp_dirPath->text());
    else
        displayResults();
}

QWidget* MainWindow::createCenteredCheckBox(QCheckBox *checkBox)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(checkBox);

    widget->setLayout(layout);

    return widget;
}

QTableWidgetItem* MainWindow::createEmptyTableItem()
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);

    return item;
}

void MainWindow::displayResults()
{
    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(reverse(int)));

    QStringList rowLabels;
    rowLabels << "";

    for (int i = 0; i < m_songs.size(); ++i)
    {
        rowLabels << QString::number(i+1);

        QTableWidgetItem *filenameItem = new QTableWidgetItem(m_songs.at(i).getFilename());
        filenameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        mp_resultTable->setItem(i+1, FILENAME, filenameItem);
        mp_resultTable->setItem(i+1, AUTHOR, new QTableWidgetItem(m_songs.at(i).getAuthor()));
        mp_resultTable->setItem(i+1, TITLE, new QTableWidgetItem(m_songs.at(i).getTitle()));

        mp_renamingChechBoxes.append(new QCheckBox);
        mp_resultTable->setCellWidget(i+1, FILENAME_CHECKBOX, createCenteredCheckBox(mp_renamingChechBoxes.last()));


        QPushButton *reverseButton = new QPushButton(QIcon("../Project/images/reverse.png"), "");
        mp_resultTable->setCellWidget(i+1, REVERSE, reverseButton);

        connect(reverseButton, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(reverseButton, i+1);
    }

    mp_resultTable->resizeColumnsToContents();
    mp_layout->setCurrentIndex(1);
    mp_resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mp_resultTable->setVerticalHeaderLabels(rowLabels);
    resize(800, 600);
}

void MainWindow::reverse(int row) const
{
    const QString newAuthor = mp_resultTable->item(row, TITLE)->text();
    const QString newTitle = mp_resultTable->item(row, AUTHOR)->text();

    mp_resultTable->item(row, AUTHOR)->setText(newAuthor);
    mp_resultTable->item(row, TITLE)->setText(newTitle);
}

void MainWindow::checkAll(bool value) const
{
    for (QCheckBox *checkBox : mp_renamingChechBoxes)
        checkBox->setChecked(value);
}
