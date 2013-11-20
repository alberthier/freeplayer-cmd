#ifndef VLCENGINE_H
#define VLCENGINE_H

#include <qobject.h>
#include <vlc/libvlc.h>
#include "VlcEngineConfig.h"

class VlcEngine : public QObject
{
    Q_OBJECT
public:
    VlcEngine();
    virtual ~VlcEngine();

    virtual bool init();
    virtual bool canDecode(const QString& url) const;
    virtual uint position() const;
//    virtual bool load( const KURL&, bool );
    virtual bool load(const QString& url);
    virtual bool play(uint ms);
    virtual void stop();
    virtual void pause();
    virtual uint length() const;
    virtual void seek(uint ms);
//    virtual Engine::State state() const { return Engine::Empty; }
//protected:
    virtual void setVolumeSW(uint percent);

private:
    libvlc_instance_t* m_vlc;
    libvlc_exception_t* m_vlcException;
    int m_currentItem;
    VlcEngineConfig m_config;
};

#endif // VLCENGINE_H
