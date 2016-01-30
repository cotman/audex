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

#include "replaygainwrapper.h"

ReplayGainWrapper::ReplayGainWrapper(QObject* parent, const QString& commandPattern) : QObject(parent) {

  command_pattern = commandPattern;

  connect(&proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
  connect(&proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

  proc.setOutputChannelMode(KProcess::SeparateChannels);
  proc.setReadChannel(KProcess::StandardError);

  termination = FALSE;
  
}

ReplayGainWrapper::~ReplayGainWrapper() {

}

bool ReplayGainWrapper::calculate(const QString& suffix, const QString& target_dir) {

  if (command_pattern.isEmpty()) return FALSE;

  PatternParser patternparser;
  QString command = QString(patternparser.parseReplayGainCommandPattern(command_pattern, suffix))
    .arg(QString(target_dir).replace(" ", "\\ "));

  kDebug() << "executing command " << command;
  proc.setShellCommand(command);
  proc.start();
  proc.waitForStarted();
  emit info(i18n("Applying ReplayGain to all tracks..."));
  return proc.waitForFinished();

}

void ReplayGainWrapper::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {

  if ((exitStatus == QProcess::NormalExit) && (exitCode==0)) {
    emit info(i18n("ReplayGain tags written."));
  } else {
    emit error(i18n("An error occurred while calculating ReplayGain."),
	i18n("Please check your profile."));
  }

  kDebug() << "replaygain finished.";
}

void ReplayGainWrapper::processError(QProcess::ProcessError err) {

  if (termination) return;

  switch (err) {
    case QProcess::FailedToStart :
      emit error(i18n("ReplayGain tool failed to start."), i18n("Either it is missing, or you may have insufficient permissions to invoke the program.")); break;
    case QProcess::Crashed :
      emit error(i18n("ReplayGain tool crashed some time after starting successfully."), i18n("Please check your profile.")); break;
    case QProcess::Timedout :
      emit error(i18n("ReplayGain tool timed out. This should not happen."), i18n("Please check your profile.")); break;
    case QProcess::WriteError :
      emit error(i18n("An error occurred when attempting to write with the ReplayGain tool."), i18n("For example, the process may not be running, or it may have closed its input channel.")); break;
    case QProcess::ReadError :
      emit error(i18n("An error occurred when attempting to read with the ReplayGain tool."), i18n("For example, the process may not be running.")); break;
    case QProcess::UnknownError :
      emit error(i18n("An unknown error occurred with the ReplayGain tool. This should not happen."), i18n("Please check your profile.")); break;
  }

  kDebug() << "replaygain finished.";
}
