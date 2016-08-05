#include "SongFile.h"


SongFile::SongFile(const QString &filename)
    : m_filename(filename)
{

}

SongFile::~SongFile()
{

}

QString SongFile::getFilename() const
{
    return m_filename;
}

QString SongFile::getTitle() const
{
    return m_title;
}

QString SongFile::getAuthor() const
{
    return m_author;
}

QList<Choice_t> SongFile::getChoices(const QString& separator) const
{
    QList<Choice_t> choices;
    int count = m_filename.count(separator);

    for (int i = 0; i < count; ++i)
        choices.append(qMakePair(m_filename.section(separator, 0, i), m_filename.section(separator, i+1, count)));

    return choices;
}

void SongFile::fill(const QString& author, const QString& title)
{
    m_author = author;
    m_title = title;
}
