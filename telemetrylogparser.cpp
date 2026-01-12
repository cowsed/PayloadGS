#include "telemetrylogparser.h"
#include <QFile>
#include <QtLogging>

TelemetryLogParser::TelemetryLogParser(QObject *parent)
    : QObject{parent}
{}

void TelemetryLogParser::handleTemps(QDateTime time, const QStringList &list)
{
    printf("Parsing temps");
    if (list.length() < 4) {
        qDebug("not enough parts to parse temp line");
    }
    bool ok = false;
    double t1 = list[2].toDouble(&ok);
    if (!ok) {
        printf("Failed to parse t1\n");
        return;
    }
    double t2 = list[3].toDouble(&ok);
    if (!ok) {
        printf("Failed to parse t2\n");
        return;
    }
    printf("temps: %s %f %f", qPrintable(time.toString()), t1, t2);
    emit tempsUpdated(time, t1, t2);
}

void TelemetryLogParser::handleLine(const QString &str)
{
    QStringList parts = str.split("\t");
    if (parts.length() < 2) {
        printf("not enough parts to parse line\n");
        return;
    }
    bool ok = false;
    uint64_t msec = parts[0].toUInt(&ok);
    if (!ok) {
        printf("failed to parse epoch ms\n");
        return;
    }
    QDateTime date = QDateTime::fromMSecsSinceEpoch(msec);
    const QString &typ = parts[1];
    if (typ == "Temps") {
        handleTemps(date, parts);
    }
}
void TelemetryLogParser::logUpdated(const QString &path)
{
    printf("Telem log updated: %s", qPrintable(path.toUtf8()));
    printf("Updated\n");
    QFile file{path};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("Failed to open file");
        return;
    }

    file.seek(last_read);
    printf("seekd to %lld \n", last_read);

    while (file.canReadLine()) {
        QString line = file.readLine();
        last_read = file.pos();
        printf("Line: %s\n", qPrintable(line));
        handleLine(line);
    }
    printf("Done\n");
}
