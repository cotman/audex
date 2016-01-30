/* AUDEX CDDA EXTRACTOR
 * Copyright (C) 2016 (audex@maniatek.com)
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

#ifndef REPLAYGAINWRAPPER_HEADER
#define REPLAYGAINWRAPPER_HEADER

#include <QString>

#include <KDebug>
#include <KProcess>

#include "utils/patternparser.h"

class ReplayGainWrapper : public QObject {

  Q_OBJECT

public:
  ReplayGainWrapper(QObject* parent = 0,const QString& commandPatten = "");
  ~ReplayGainWrapper();

public slots:
  bool calculate(const QString& suffix, const QString& target_dir);

private slots:
  void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void processError(QProcess::ProcessError err);

signals:
  void finished();

  void error(const QString& message, const QString& details = QString());
  void warning(const QString& message);
  void info(const QString& message);

private:
  QString command_pattern;

  bool termination;

  KProcess proc;

};

#endif
