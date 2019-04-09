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
 
#ifndef COVERFETCHER_HEADER
#define COVERFETCHER_HEADER

#include <QObject>
#include <QByteArray>
#include <QRegExp>

#include <KLocalizedString>
#include <KIO/Job>
#include <KIO/SimpleJob>
#include <KIO/TransferJob>

#include "preferences.h"
#include "musicbrainzjob.h"

class CoverFetcher : public QObject {
  Q_OBJECT
public:
  CoverFetcher(QObject *parent = 0);
  ~CoverFetcher();

  void fetchMusicBrainzCoverArtURLs(const QString& searchArtist, const QString& searchAlbum, const int fetchNo = 8);
  void startFetchThumbnails(const QString& searchArtist, const QString& searchAlbum, const int fetchNo = 8);
  void stopFetchThumbnails();
  void startFetchCover(const int no);
  
  const QByteArray thumbnail(int index);
  const QString caption(int index);
  inline int count() { return cover_names.count(); }

  enum Status {
    NOS,
    SEARCHING,
    FETCHING_THUMBNAIL,
    FETCHING_COVER
  };
  
  inline Status status() const { return _status; }

signals:
  void fetchedThumbnail(const QByteArray& thumbnail, const QString& caption, int no);
  void allCoverThumbnailsFetched();
  void fetchedCover(const QByteArray& cover);
  void nothingFetched();

  void statusChanged(Status status);

  void error(const QString& description, const QString& solution = QString());
  void warning(const QString& description);
  void info(const QString& description);

private slots:
  void fetched_musicbrainz_cover_art_urls();
  void fetched_html_data(KJob* job);

private:
  int fetch_no;
  QStringList cover_urls_thumbnails;
  QStringList cover_urls;
  QStringList cover_names;
  QList<QByteArray> cover_thumbnails;
  void clear() { cover_thumbnails.clear(); }

  KIO::TransferJob* job;
  MusicBrainzJob* musicbrainz_job;

  Status _status;

  int f_i;
  QString external_ip;
  QString search_string;

  bool fetch_cover_thumbnail();
  bool fetch_cover(const int no);

};

#endif
