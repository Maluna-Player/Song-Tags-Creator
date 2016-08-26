#include "TagsManager.h"
#include "ChoiceDialog.h"
#include <taglib/fileref.h>


const QList<QString> TagsManager::elementsSeparators { " - ", "-" };

const QList<QString> TagsManager::authorsSeparators { " feat. ", " Feat. ", " feat ", " Feat ", " ft. ", " Ft. ", " ft ", " Ft " };


TagsManager::TagsManager() : m_processedSongsCount(0)
{

}

TagsManager::~TagsManager()
{

}

void TagsManager::fillSong(SongFile& song, const QString& author, const QString& title)
{
    QList<QString> songAuthors;
    QString songTitle = title;

    bool processed{false};

    for (auto const& separator : authorsSeparators)
    {
        if (author.contains(separator) || title.contains(separator))
        {
            // Split of author
            auto authorElements = author.split(separator);
            for (const QString& authorElement : authorElements)
            {
                if (!authorElement.isEmpty())
                    songAuthors.append(authorElement);
            }


            // Split of title
            auto titleElements = title.split(separator);
            songTitle = titleElements[0];

            for (int i = 1; i < titleElements.size(); ++i)
            {
                if (!titleElements[i].isEmpty())
                    songAuthors.append(titleElements[i]);
            }

            processed = true;
            break;
        }
    }

    if (!processed && !author.isEmpty())
        songAuthors.append(author);

    song.fill(songAuthors, songTitle);

    ++m_processedSongsCount;
    emit processedSongs(m_processedSongsCount);
}

void TagsManager::fillSong(SongFile& song, const QString& separator)
{
    auto elements = song.getFilename().split(separator);
    fillSong(song, elements[0], elements[1]);
}

QList<SongFile*> TagsManager::deduceSongs(QVector<SongFile>& songs)
{
    QList<SongFile*> nonDeducedSongs;

    for (SongFile& song : songs)
    {
        bool separatorFound{false};

        for (auto const& separator : elementsSeparators)
        {
            if (song.getFilename().contains(separator))
            {
                int count = song.getFilename().count(separator);
                if (count == 1)
                    fillSong(song, separator);
                else
                    nonDeducedSongs.append(&song);

                separatorFound = true;
                break;
            }
        }

        if (!separatorFound)
            fillSong(song, "", song.getFilename());
    }

    return nonDeducedSongs;
}

bool TagsManager::askSongs(QList<SongFile*> songsToAsk, const QVector<SongFile>& songs)
{
    for (SongFile *currentSong : songsToAsk)
    {
        unsigned int maxRefs{0};
        Choice_t bestChoice;

        QString separator;
        for (auto const& currentSeparator : elementsSeparators)
        {
            if (currentSong->getFilename().contains(currentSeparator))
            {
                separator = currentSeparator;
                break;
            }
        }

        auto choices = currentSong->getChoices(separator);
        for (auto choice : choices)
        {
            unsigned int cpt{0};

            for (const SongFile& song : songs)
            {
                if (song.getAuthors().contains(choice.first))
                    ++cpt;
            }

            if (cpt > maxRefs)
            {
                maxRefs = cpt;
                bestChoice = choice;
            }
        }

        if (maxRefs >= minimumRefs)
            fillSong(*currentSong, bestChoice.first, bestChoice.second);
        else
        {
            ChoiceDialog dialog(*currentSong, separator);

            if (dialog.aborted())
                return false;

            if (dialog.allIgnored())
                break;

            auto choice = dialog.getSelectedChoice();
            fillSong(*currentSong, choice.first, choice.second);
        }
    }

    return true;
}

void TagsManager::reverseSongs(QVector<SongFile> &songs)
{
    for (auto& currentSong : songs)
    {
        unsigned int count{0};
        const auto& songTitle = currentSong.getTitle();

        for (const SongFile& song : songs)
        {
            if (song.getAuthors().contains(songTitle))
                ++count;
        }

        if (count >= minimumRefs)
            currentSong.reverse();
    }
}

bool TagsManager::compute(QVector<SongFile>& songs)
{
    m_processedSongsCount = 0;

    auto songsToAsk = deduceSongs(songs);
    if (!askSongs(songsToAsk, songs))
        return false;

    reverseSongs(songs);

    return true;
}

QVector<bool> TagsManager::writeTags(const QVector<SongFile>& songs)
{
    QVector<bool> result;

    for (SongFile const& song : songs)
    {
        bool success = false;

        if (!song.getAuthors().isEmpty() || !song.getTitle().isEmpty())
        {
            const QString songFilePath = song.getFilepath();
            TagLib::FileRef file(songFilePath.toStdString().c_str());

            if (!file.isNull() && file.tag())
            {
                TagLib::Tag *tag = file.tag();

                tag->setArtist(song.getAuthors().join(";").toStdString());
                tag->setTitle(song.getTitle().toStdString());

                success = file.save();
            }
        }

        result.append(success);
    }

    return result;
}
