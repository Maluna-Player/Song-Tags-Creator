#ifndef TAGSMANAGER_H
#define TAGSMANAGER_H

#include <QVector>
#include <QObject>
#include "SongFile.h"

class TagsManager : public QObject
{
    Q_OBJECT

    private:

        unsigned int m_processedSongsCount;

        static const unsigned int minimumRefs { 3 };

        static const QList<QString> elementsSeparators;

        static const QList<QString> authorsSeparators;


        void fillSong(SongFile& song, const QString& author, const QString& title);

        void fillSong(SongFile& song, const QString& separator);


        QList<SongFile*> deduceSongs(QVector<SongFile>& songs);

        bool askSongs(QList<SongFile*> songsToAsk, const QVector<SongFile>& songs);

        void reverseSongs(QVector<SongFile>& songs);

    signals:

        void processedSongs(int songsCount);

    public:

        TagsManager();
        virtual ~TagsManager();

        bool compute(QVector<SongFile>& songs);

        QVector<bool> writeTags(const QVector<SongFile>& songs);
};

#endif // TAGSMANAGER_H
