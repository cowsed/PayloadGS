#ifndef FRONTBACKDATAHOLDER_H
#define FRONTBACKDATAHOLDER_H

#include <QAbstractSeries>
#include <QDateTime>
#include <QObject>
#include <QQmlEngine>
class FrontBackDataHolder : public QObject

{
    Q_OBJECT

    Q_PROPERTY(double latestValue READ latestValue NOTIFY valueChanged FINAL)
    Q_PROPERTY(QDateTime latestTime READ latestTime FINAL)
    Q_PROPERTY(QDateTime earliestTime READ earliestTime FINAL)
    Q_PROPERTY(size_t count READ count NOTIFY countChanged FINAL)
    QML_ELEMENT
    QML_UNCREATABLE("tied to C++ data source")

public:
    FrontBackDataHolder(size_t max_in_mem, QObject *parent = nullptr);

    Q_INVOKABLE void newValue(QDateTime ts, double value);
    Q_INVOKABLE void fillXYSeries(QAbstractSeries *series);

    double latestValue();
    QDateTime latestTime();
    QDateTime earliestTime();
    size_t count();
signals:
    void valueChanged();
    void countChanged();

private:
    QList<QPointF> m_data;
    size_t m_max_in_mem;
};

#endif // FRONTBACKDATAHOLDER_H
