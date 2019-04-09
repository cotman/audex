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

/* Some of this code is inspired by amarok 1.4.7
 * (C) 2004 Mark Kretschmann <markey@web.de>
 * (C) 2004 Stefan Bogner <bochi@online.ms>
 * (C) 2004 Max Howell
*/

#include "coverfetcher.h"
#include "musicbrainzjob.h"
#include <algorithm>
#include <QDebug>
#include <QScriptEngine>
#include <QScriptValueIterator>

CoverFetcher::CoverFetcher(QObject *parent) : QObject(parent) {
  Q_UNUSED(parent);
  _status = NOS;
  f_i = 0;
}

CoverFetcher::~CoverFetcher() {
  clear();
}

void CoverFetcher::startFetchThumbnails(const QString& searchArtist, const QString& searchAlbum, const int fetchNo) {

  qDebug() << "Fetch Thumbs ...";
  if (_status != NOS || fetchNo == 0)
  {
      emit nothingFetched();
      return;
  }

  fetch_no = fetchNo;

  _status = SEARCHING;
  emit statusChanged(SEARCHING);
  
  musicbrainz_job = new MusicBrainzJob(searchArtist, searchAlbum, fetchNo);
  connect(musicbrainz_job, SIGNAL(finished()), SLOT(fetched_musicbrainz_cover_art_urls()));
  musicbrainz_job->fetchCoverArtURLs();

}

void CoverFetcher::fetched_musicbrainz_cover_art_urls() {

  switch (_status) {

    case SEARCHING : {
        qDebug() << "searching finished.";
        
        QMap<QString, QString>::iterator it;
        
        int cover_name = 0;
        
        for (it = musicbrainz_job->cover_art_urls.begin(); it != musicbrainz_job->cover_art_urls.end(); it++)
        {
            cover_urls_thumbnails << it.key();
            cover_urls << it.value();
            cover_name++;
            cover_names << QString::number(cover_name);
        }

        _status = NOS; emit statusChanged(NOS);
        fetch_cover_thumbnail();
    } break;

    case NOS : break;

    default : break;
  }

}

void CoverFetcher::stopFetchThumbnails() {

  if ((_status != FETCHING_THUMBNAIL) && (_status != SEARCHING)) return;

  if (job) job->kill();

  _status = NOS; emit statusChanged(NOS);

}


void CoverFetcher::startFetchCover(const int no) {

  if (_status != NOS) return;

  if ((cover_urls.count()==0) || (no >= cover_urls.count()) || (no < 0)) {
    emit nothingFetched();
    return;
  }

  qDebug() << "fetching cover...";
  _status = FETCHING_COVER; emit statusChanged(FETCHING_COVER);

  job = KIO::storedGet(QUrl(cover_urls[no]));
  connect(job, SIGNAL(result(KJob*)), SLOT(fetched_html_data(KJob*)));

}

const QByteArray CoverFetcher::thumbnail(int index) {
  if ((index < 0) || (index >= cover_thumbnails.count())) return QByteArray();
  return cover_thumbnails[index];
}

const QString CoverFetcher::caption(int index) {
  if ((index < 0) || (index >= cover_names.count())) return QString();
  return cover_names[index];
}

void CoverFetcher::fetched_html_data(KJob* job) {

  QByteArray buffer;

  if (job && job->error()) {
    qDebug() << "There was an error communicating with MusicBrainz. "<< job->errorString();
    emit error(i18n("There was an error communicating with MusicBrainz."), i18n("Try again later. Otherwise make a bug report."));
    _status = NOS; emit statusChanged(NOS);
    emit nothingFetched();
    return;
  }
  if (job) {
    KIO::StoredTransferJob* const storedJob = static_cast<KIO::StoredTransferJob*>(job);
    buffer = storedJob->data();
  }

  if (buffer.count() == 0) {
    qDebug() << "MusicBrainz server: empty response";
    emit error(i18n("MusicBrainz server: Empty response."),
               i18n("Try again later. Make a bug report."));
    _status = NOS;  emit statusChanged(NOS);
    return;
  }

  switch (_status) {

      case FETCHING_THUMBNAIL : {
        qDebug() << "cover thumbnail fetched.";
        cover_thumbnails.append(buffer);
        emit fetchedThumbnail(buffer, cover_names[f_i], f_i+1);
        ++f_i;
        if (((fetch_no > -1) && (f_i == fetch_no)) || (cover_urls_thumbnails.count() == 0)) {
          _status = NOS; emit statusChanged(NOS);
          f_i = 0;
          emit allCoverThumbnailsFetched();
        } else {
          fetch_cover_thumbnail();
        }
      } break;

      case FETCHING_COVER : {
        qDebug() << "cover fetched.";
  _status = NOS; emit statusChanged(NOS);
        emit fetchedCover(buffer);
      } break;

      case NOS : break;

      default : break;

  }


}


bool CoverFetcher::fetch_cover_thumbnail() {

  if (cover_urls_thumbnails.count() == 0) {
    qDebug() << "nothing fetched.";
    emit nothingFetched();
    return false;
  }

  _status = FETCHING_THUMBNAIL; emit statusChanged(FETCHING_THUMBNAIL);

  job = KIO::storedGet(QUrl(cover_urls_thumbnails.takeFirst()));
  connect(job, SIGNAL(result(KJob*)), SLOT(fetched_html_data(KJob*)));

  return true;

}
