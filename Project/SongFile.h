#ifndef SONGFILE
#define SONGFILE

#include <QString>
#include <QPair>
#include <QList>

using Choice_t = QPair<QString,QString>;

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

        QList<Choice_t> getChoices(const QString& separator) const;

        void fill(const QString& author, const QString& title);
};

#endif // SONGFILE

