#ifndef SONGFILE
#define SONGFILE

#include <QString>
#include <QPair>
#include <QList>

class SongFile
{
    private:

        QString m_filename;

        QString m_title;

        QString m_author;

    public:

        SongFile() {}
        SongFile(const QString& filename);
        virtual ~SongFile();

        QString getFilename() const;

        QString getTitle() const;

        QString getAuthor() const;

        QList<QPair<QString,QString>> getChoices(const QString& separator) const;

        void fill(const QString& author, const QString& title);
};

#endif // SONGFILE

