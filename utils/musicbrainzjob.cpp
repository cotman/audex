/* AUDEX CDDA EXTRACTOR
 * Copyright (C) 2007-2015 Marco Nelles (audex@maniatek.com)
 * <http://userbase.kde.org/Audex>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include "musicbrainzjob.h"


MusicBrainzJob::MusicBrainzJob(const QString& search_artist, const QString& search_album, const int cover_art_limit) {

    this->search_artist = search_artist;
    this->search_album = search_album;
    this->cover_art_limit = cover_art_limit;
    
}

void MusicBrainzJob::fetchCoverArtURLs() {

    // Format: '"<ALBUM_NAME>" artist:"<ARTIST_NAME>"'
    QString search_string = "'" + this->search_album + "'" + " artist:'" + this->search_artist + "'";
    qDebug() << "MusicBrainz search string:  \'" << search_string;

    MusicBrainz5::CQuery Query("audex-query");
    MusicBrainz5::CQuery::tParamMap Params;
    Params["query"] = search_string.toUtf8().constData();
    Params["limit"] = 25;

    CoverArtArchive::CCoverArt CoverArt("audex-query");

    // First get a list of Releases that match the search criteria
    MusicBrainz5::CMetadata Metadata = Query.Query("release", "", "", Params);
    MusicBrainz5::CReleaseList *ReleaseList = Metadata.ReleaseList();

    int cover_art_found = 0;

    // Iterate the list of Releases and, for each, pluck out the unique Release ID
    for (int release_count = 0; release_count < ReleaseList->NumItems(); release_count++)
    {
        if (cover_art_found == this->cover_art_limit)
        {
            break;
        }

        MusicBrainz5::CRelease *Release = ReleaseList->Item(release_count);

        try
        {
            CoverArtArchive::CReleaseInfo ReleaseInfo = CoverArt.ReleaseInfo(Release->ID());
            CoverArtArchive::CImageList* ImageList = ReleaseInfo.ImageList();

            for (int image_count = 0; image_count < ImageList->NumItems(); image_count++)
            {
                if (cover_art_found == this->cover_art_limit)
                {
                    break;
                }

                CoverArtArchive::CImage* Image = ImageList->Item(image_count);
                CoverArtArchive::CThumbnails* Thumbnails = Image->Thumbnails();

                if (!Image->Front() || Thumbnails->Small().empty())
                {
                    continue;
                }

                QString image_thumbnail_url = QString::fromStdString(Thumbnails->Small());
                QString image_full_url = QString::fromStdString(Image->Image());
                
                this->cover_art_urls.insert(image_thumbnail_url, image_full_url);
                cover_art_found++;

                qDebug() << "Thumbnail:  " << image_thumbnail_url;
                qDebug() << "Full:  " << image_full_url;
            }
        }
        catch (...)
        {
            // No appropriate images were found for the Release ID, but we continue to check any others,
            // provided we haven't exceeded the past in limit
        }
    }
    
    emit finished();
    qDebug() << "MusicBrainz fetch finished";
}
