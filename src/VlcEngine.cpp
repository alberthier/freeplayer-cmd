#include "VlcEngine.h"
#include <qfileinfo.h>

#define LIBVLC_CATCH(excpt, handler)\
    if (libvlc_exception_raised(excpt)) { \
        qDebug("Exception : %s", libvlc_exception_get_message(excpt)); \
        libvlc_exception_clear(excpt);\
        handler \
    }

VlcEngine::VlcEngine()
    : m_vlc(NULL)
    , m_vlcException(NULL)
    , m_currentItem(0)
{
    m_vlcException = new libvlc_exception_t;
}

VlcEngine::~VlcEngine()
{
    if (m_vlc != NULL) {
        libvlc_destroy(m_vlc);
    }
    delete m_vlcException;
}

bool VlcEngine::init()
{
    m_config.load("share/freeplayer-cmd/freeplayer.xml");
    m_currentItem = 0;
    libvlc_exception_clear(m_vlcException);
    if (m_vlc != NULL) {
        libvlc_destroy(m_vlc);
    }
    m_vlc = libvlc_new(m_config.vlcArgc(), m_config.vlcArgv(), m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        if (m_vlc != NULL) {
            libvlc_destroy(m_vlc);
        }
        return false;
    )
    return true;
}

bool VlcEngine::canDecode(const QString& url) const
{
    QFileInfo fi(url);
    QString ext = fi.extension().lower();
    if ((ext == "mpg")  ||
        (ext == "mpeg") ||
        (ext == "avi")  ||
        (ext == "asf")  ||
        (ext == "wmv")  ||
        (ext == "mp4")  ||
        (ext == "mov")  ||
        (ext == "ra")   ||
        (ext == "flv")  ||
        (ext == "divx") ||
        (ext == "xvid") ||
        (ext == "mp2")  ||
        (ext == "mp3")  ||
        (ext == "wma")  ||
        (ext == "ogg")  ||
        (ext == "ogm")  ||
        (ext == "mkv")  ||
        (ext == "rv")   ||
        (ext == "ram")  ||
        (ext == "wav")  ||
        (ext == "ac3")  ||
        (ext == "aac")  ||
        (ext == "a52")  ||
        (ext == "flac")) {
            return true;
    }
    return false;
}

bool VlcEngine::load(const QString& url)
{
    if (!canDecode(url)) {
        return false;
    }
    libvlc_playlist_clear(m_vlc, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        /* NOP */
    )
    QFileInfo fi(url);
    QPair<uint, char**> opts = m_config.mediaOptions(fi.extension());
    m_currentItem = libvlc_playlist_add_extended(m_vlc, url, "amarok-item", opts.first, (const char**)opts.second, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        m_currentItem = 0;
        return false;
    )
    return true;
}

bool VlcEngine::play(uint ms)
{
    libvlc_playlist_play(m_vlc, m_currentItem, 0, NULL, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        return false;
    )
    if (ms != 0) {
        seek(ms);
    }
    return true;
}

void VlcEngine::stop()
{
    libvlc_playlist_stop(m_vlc, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        /* NOP */
    )
}

void VlcEngine::pause()
{
    libvlc_playlist_pause(m_vlc, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        /* NOP */
    )
}

uint VlcEngine::length() const
{
    libvlc_input_t* input = libvlc_playlist_get_input(m_vlc, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        libvlc_input_free(input);
        return 0;
    )
    uint len = (uint) libvlc_input_get_length(input, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        libvlc_input_free(input);
        return 0;
    )
    libvlc_input_free(input);
    return len;
}

uint VlcEngine::position() const
{
    libvlc_input_t* input = libvlc_playlist_get_input(m_vlc, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        libvlc_input_free(input);
        return 0;
    )
    uint pos = (uint) libvlc_input_get_time(input, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        libvlc_input_free(input);
        return 0;
    )
    libvlc_input_free(input);
    return pos;
}

void VlcEngine::seek(uint ms)
{
    libvlc_input_t* input = libvlc_playlist_get_input(m_vlc, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        libvlc_input_free(input);
        return;
    )
    libvlc_input_set_time(input, ms, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        libvlc_input_free(input);
        return;
    )
}

void VlcEngine::setVolumeSW(uint percent)
{
    // VLC volume range : 0 - 200
    libvlc_audio_set_volume(m_vlc, percent * 2, m_vlcException);
    LIBVLC_CATCH(m_vlcException,
        /* NOP */
    )
}
