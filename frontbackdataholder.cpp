#include "frontbackdataholder.h"
#include <QXYSeries>

FrontBackDataHolder::FrontBackDataHolder(size_t max_in_mem, QObject *parent)
    : QObject{parent}
    , m_max_in_mem(max_in_mem)
{}

void FrontBackDataHolder::newValue(QDateTime ts, double value)
{
    size_t old_len = m_data.count();
    m_data.push_back({static_cast<qreal>(ts.toMSecsSinceEpoch()), value});

    if (m_data.length() > m_max_in_mem) {
        m_data.removeFirst();
    }

    if (m_data.count() != old_len) {
        emit countChanged();
    }

    emit valueChanged();
}

double FrontBackDataHolder::latestValue()
{
    if (m_data.isEmpty()) {
        return NAN;
    }

    return m_data.last().y();
}
QDateTime FrontBackDataHolder::latestTime()
{
    if (m_data.isEmpty()) {
        return QDateTime::fromMSecsSinceEpoch(0);
    }

    return QDateTime::fromMSecsSinceEpoch(m_data.last().x());
}
QDateTime FrontBackDataHolder::earliestTime()
{
    if (m_data.isEmpty()) {
        return QDateTime::fromMSecsSinceEpoch(0);
    }

    return QDateTime::fromMSecsSinceEpoch(m_data.first().x());
}
size_t FrontBackDataHolder::count()
{
    return m_data.count();
}

void FrontBackDataHolder::fillXYSeries(QAbstractSeries *series)
{
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);
        // xySeries->replace(radio_temps);
        if (xySeries->count() > m_max_in_mem) {
            xySeries->remove(0);
        }
        if (m_data.count() > 0) {
            xySeries->append(m_data.last());
        }
    }
}
