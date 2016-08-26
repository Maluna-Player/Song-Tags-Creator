#ifndef SONGFILE
#define SONGFILE

#include <QString>
#include <QPair>
#include <QList>
#include <QStringList>

using Choice_t = QPair<QString,QString>;

class SongFile
{
    private:

        QString m_filepath;

        QString m_filename;

        QString m_title;

        QStringList m_authors;


        QString getComputedFilename() const;

    public:

        SongFile() {}
        SongFile(const QString& filepath);
        virtual ~SongFile();

        QString getFilepath() const;

        QString getFilename() const;

        QString getTitle() const;

        QStringList getAuthors() const;

        QList<Choice_t> getChoices(const QString& separator) const;

        void fill(const QStringList& authors, const QString& title);

        void reverse();

        bool rename();
};

#endif // SONGFILE

