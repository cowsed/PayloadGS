#ifndef TELEMETRYLOGPARSER_H
#define TELEMETRYLOGPARSER_H
#include "QObject"

class TelemetryLogParser : public QObject
{
    Q_OBJECT
public:
    explicit TelemetryLogParser(QObject *parent = nullptr);

signals:

public slots:
    void logUpdated(const QString &path);
};

#endif // TELEMETRYLOGPARSER_H
