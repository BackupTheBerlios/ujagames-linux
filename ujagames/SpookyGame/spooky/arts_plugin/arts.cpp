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

// Qt includes
#include <qapplication.h>
#include <qdir.h>
#include <qptrlist.h>
#include <qsound.h>
#include <qtimer.h>

// aRts includes
#include <soundserver.h>

// local includes
#include "arts.h"

///////////////////////////////////////////////////////////////////////////////

class QAuBucketARTS : public QAuBucket
{
public:
    QAuBucketARTS(const std::string& filename) :
        fileName(filename),
        playObject(Arts::PlayObject::null()) { }

    ~QAuBucketARTS() { }

    std::string fileName;
    Arts::PlayObject playObject;
};

///////////////////////////////////////////////////////////////////////////////

class QAuServerARTSPrivate
{
public:
    QAuServerARTSPrivate();

    Arts::SimpleSoundServer arts;
    QTimer timer;
    QPtrList<QSound> playing;

    std::string absFileName(const QString& filename)
    {
        QFileInfo info(filename);
        return static_cast<std::string>(QFile::encodeName(info.absFilePath()));
    }
};

///////////////////////////////////////////////////////////////////////////////

QAuServerARTS::QAuServerARTS(QObject* parent) : QAuServer(parent,"ARTS")
{
    m_private = new QAuServerARTSPrivate();

    QObject::connect(&m_private->timer, SIGNAL(timeout()),
                     this, SLOT(slotCheckObjects()));
}

///////////////////////////////////////////////////////////////////////////////

QAuServerARTS::~QAuServerARTS()
{
    delete m_private;
}

///////////////////////////////////////////////////////////////////////////////

void QAuServerARTS::play(const QString& filename)
{
    if (!m_private->arts.isNull())
    {
        m_private->arts.play(m_private->absFileName(filename));
    }
}

///////////////////////////////////////////////////////////////////////////////

void QAuServerARTS::setDone(QSound* s)
{
    if (!m_private->arts.isNull())
    {
        decLoop(s);
        if (s->loopsRemaining())
        {
            QAuBucketARTS *b = bucket(s);
            b->playObject = Arts::PlayObject::null();
            play(s);
        }
        else
        {
            m_private->playing.remove(s);

            QAuBucketARTS* b = bucket(s);
            if (!b->playObject.isNull())
            {
                b->playObject.halt();
                b->playObject = Arts::PlayObject::null();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

void QAuServerARTS::play(QSound* s)
{
    if (!m_private->arts.isNull())
    {
        if (m_private->playing.findRef(s) == -1)
            m_private->playing.append(s);

        QAuBucketARTS *b = bucket(s);
        if (b->playObject.isNull())
            b->playObject = m_private->arts.createPlayObject(b->fileName);

        if (!m_private->timer.isActive())
	m_private->timer.start(10); 
	// uja 14.02.2005 herabgesetzt von 100 auf 10, um bessere Reaktionszeit zu erhalten
    }
}

///////////////////////////////////////////////////////////////////////////////

void QAuServerARTS::stop(QSound* s)
{
    if (!m_private->arts.isNull())
    {
        int index = m_private->playing.findRef(s);
        if (index != -1)
        {
            m_private->playing.remove(index);

            if (m_private->playing.count() == 0)
                m_private->timer.stop();

            QAuBucketARTS* b = bucket(s);
            if (!b->playObject.isNull())
            {
                b->playObject.halt();
                b->playObject = Arts::PlayObject::null();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

void QAuServerARTS::init(QSound* s)
{
    if (!m_private->arts.isNull())
    {
        setBucket(s, new QAuBucketARTS(m_private->absFileName(s->fileName())));
    }
}

///////////////////////////////////////////////////////////////////////////////

bool QAuServerARTS::okay()
{
    return !m_private->arts.isNull();
}

///////////////////////////////////////////////////////////////////////////////

QAuBucketARTS* QAuServerARTS::bucket(QSound* s)
{
    return static_cast<QAuBucketARTS*>(QAuServer::bucket(s));
}

///////////////////////////////////////////////////////////////////////////////

void QAuServerARTS::slotCheckObjects()
{
    uint i = 0;
    while (i < m_private->playing.count())
    {
        QAuBucketARTS* b = bucket(m_private->playing.at(i));
        if (b->playObject.isNull())
            setDone(m_private->playing.at(i));
        else
        {
            Arts::poTime pos = b->playObject.currentTime();
            Arts::poTime max = b->playObject.overallTime();

            if (pos.seconds >= max.seconds && pos.ms >= max.ms)
                setDone(m_private->playing.at(i));
            else
                ++i;
        }
    }

    if (m_private->playing.count() == 0)
        m_private->timer.stop();
}

///////////////////////////////////////////////////////////////////////////////

QAuServerARTSPrivate::QAuServerARTSPrivate()
    : arts(Arts::Reference("global:Arts_SimpleSoundServer"))
{
}

///////////////////////////////////////////////////////////////////////////////

bool createArtsServer(QObject* parent)
{
    Arts::Dispatcher* dispatcher = new Arts::Dispatcher();
    QAuServerARTS* arts = new QAuServerARTS(parent);
    if (!arts->okay())
    {
        delete arts;
        delete dispatcher;

        return false;
    }
    else
        return true;
}

///////////////////////////////////////////////////////////////////////////////

int init_QSound()
{
    if (createArtsServer(qApp))
        return 1;
    else
        return 0;
}

// end of file
