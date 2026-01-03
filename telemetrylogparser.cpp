#include "telemetrylogparser.h"
#include <QtLogging>

TelemetryLogParser::TelemetryLogParser(QObject *parent)
    : QObject{parent}
{}

void TelemetryLogParser::logUpdated(const QString &path)
{
    qDebug("Telem log updated: %s", qPrintable(path.toUtf8()));
    printf("Updated\n");
}
