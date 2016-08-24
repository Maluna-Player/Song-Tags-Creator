#include "TagsManager.h"
#include "ChoiceDialog.h"
#include <taglib/fileref.h>


TagsManager::TagsManager() : m_processedSongsCount(0)
{

}

TagsManager::~TagsManager()
{

}

void TagsManager::fillSong(SongFile& song, const QString& author, const QString& title)
{
    song.fill(author, title);

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
        if (song.getFilename().contains(" - "))
        {
            int count = song.getFilename().count(" - ");
            if (count == 1)
                fillSong(song, " - ");
            else
                nonDeducedSongs.append(&song);
        }
        else if (song.getFilename().contains("-"))
        {
            int count = song.getFilename().count("-");
            if (count == 1)
                fillSong(song, "-");
            else
                nonDeducedSongs.append(&song);
        }
        else
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

        const QString separator = (currentSong->getFilename().contains(" - ") ? " - " : "-");

        auto choices = currentSong->getChoices(separator);
        for (auto choice : choices)
        {
            unsigned int cpt{0};

            for (const auto& song : songs)
            {
                if (choice.first == song.getAuthor())
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

        for (const auto& song : songs)
        {
            if (songTitle == song.getAuthor())
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

        if (!song.getAuthor().isEmpty() || !song.getTitle().isEmpty())
        {
            const QString songFilePath = song.getFilepath();
            TagLib::FileRef file(songFilePath.toStdString().c_str());

            if (!file.isNull() && file.tag())
            {
                TagLib::Tag *tag = file.tag();

                tag->setArtist(song.getAuthor().toStdString());
                tag->setTitle(song.getTitle().toStdString());

                success = file.save();
            }
        }

        result.append(success);
    }

    return result;
}
