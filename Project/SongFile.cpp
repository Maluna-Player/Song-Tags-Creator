#include "SongFile.h"
#include <QFileInfo>
#include <QDir>


SongFile::SongFile(const QString &filepath)
{
    QFileInfo fileInfo(filepath);

    m_filepath = fileInfo.filePath();
    m_filename = fileInfo.completeBaseName();
}

SongFile::~SongFile()
{

}

QString SongFile::getFilepath() const
{
    return m_filepath;
}

QString SongFile::getFilename() const
{
    return m_filename;
}

QString SongFile::getTitle() const
{
    return m_title;
}

QStringList SongFile::getAuthors() const
{
    return m_authors;
}

QString SongFile::getComputedFilename() const
{
    QString filename;

    if (m_authors.isEmpty())
        filename = m_title;
    else
    {
        filename = m_authors[0] + " - " + m_title;

        if (m_authors.size() > 1)
            filename += " feat. " + m_authors.mid(1).join(" & ");
    }

    return filename;
}

QList<Choice_t> SongFile::getChoices(const QString& separator) const
{
    QList<Choice_t> choices;
    int count = m_filename.count(separator);

    for (int i = 0; i < count; ++i)
        choices.append(qMakePair(m_filename.section(separator, 0, i), m_filename.section(separator, i+1, count)));

    return choices;
}

void SongFile::fill(const QStringList& authors, const QString& title)
{
    m_authors = authors;
    m_title = title;
}

void SongFile::reverse()
{
    if (m_authors.isEmpty())
    {
        m_authors.append(m_title);
        m_title.clear();
    }
    else
    {
        QString title{m_authors[0]};

        m_authors[0] = m_title;
        m_title = title;
    }
}

bool SongFile::rename()
{
    QFileInfo fileInfo(m_filepath);

    const QString newFilename = getComputedFilename();
    const QString newFilepath = fileInfo.path() + QDir::separator() + newFilename + "." + fileInfo.suffix();

    if (QFile::rename(fileInfo.filePath(), newFilepath))
    {
        m_filepath = newFilepath;
        m_filename = newFilename;

        return true;
    }

    return false;
}
