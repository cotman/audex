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
 
#ifndef MUSICBRAINZJOB_HEADER
#define MUSICBRAINZJOB_HEADER

#include <QMap>
#include <QObject>
#include <QString>
#include "musicbrainz5/Query.h"
#include "musicbrainz5/Release.h"
#include "coverart/CoverArt.h"
#include "coverart/Image.h"
#include "coverart/ImageList.h"
#include "coverart/Thumbnails.h"

class MusicBrainzJob : public QObject {
  Q_OBJECT

public:
  MusicBrainzJob(const QString& search_artist, const QString& search_album, const int cover_art_limit);
  void fetchCoverArtURLs();
  QMap<QString, QString> cover_art_urls;

private:
  QString search_artist;
  QString search_album;
  int cover_art_limit;

signals:
  void finished();

};

#endif

