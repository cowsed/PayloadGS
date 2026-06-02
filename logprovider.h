#ifndef LOGPROVIDER_H
#define LOGPROVIDER_H

#include <QObject>
#include <QQmlEngine>
#include <QString>

class LogProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString logText READ logText NOTIFY logTextChanged)
    QML_ELEMENT
    QML_SINGLETON
public:
    LogProvider(QObject *parent = nullptr)
        : QObject(parent)
    {}

    Q_INVOKABLE const QString &logText() const { return m_logText; }

    void appendLog(const QString &message)
    {
        m_logText.append(message + "\n");
        emit logTextChanged();
    }

signals:
    void logTextChanged();

private:
    QString m_logText;
};

#endif // LOGPROVIDER_H
