#ifndef LORAWRAPPER_H
#define LORAWRAPPER_H

#include <qobject.h>

class SFWrapper
{
    Q_GADGET
public:
    enum SpreadingFactor {
        Null,
        Ready,
        Loading,
        Error

    };

    SFWrapper();
};

#endif // LORAWRAPPER_H
