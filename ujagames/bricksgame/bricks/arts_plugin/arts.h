///////////////////////////////////////////////////////////////////////////////
//
// Copyright: Kevin Krammer <kevin.krammer@gmx.at>, 2004
//
///////////////////////////////////////////////////////////////////////////////
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as
//   published by the Free Software Foundation; either version 2 of the
//   License, or (at your option) any later version.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ARTS_H
#define ARTS_H

// Qt includes
#include <qobject.h>
#include <qsound.h>

// forward declarations
class QAuBucketARTS;
class QAuServerARTSPrivate;

class QAuServerARTS : public QAuServer {
    Q_OBJECT

public:
    QAuServerARTS(QObject* parent);
    ~QAuServerARTS();

    void init(QSound*);
    void play(const QString& filename);
    void play(QSound*);
    void stop(QSound*);
    bool okay();
    void setDone(QSound*);

private:
    QAuBucketARTS* bucket(QSound* s);

private slots:
    void slotCheckObjects();

private:
    QAuServerARTSPrivate* m_private;
} ;

#endif

// end of file
