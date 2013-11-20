#include "VlcEngineConfig.h"
#include <qfile.h>
#include <qdom.h>
#include <qregexp.h>

VlcEngineConfig::VlcEngineConfig()
    : m_argv(NULL)
    , m_argc(0)
{
}

VlcEngineConfig::~VlcEngineConfig()
{
    deleteOptions(m_argc, m_argv);
}

bool VlcEngineConfig::load(const QString& filename)
{
    deleteOptions(m_argc, m_argv);
    m_mediaTypes.clear();
    clearMediaOptions();
    m_name = "";
    QFile f(filename);
    if (!f.open(IO_ReadOnly)) {
        return false;
    }
    QDomDocument doc;
    if (!doc.setContent(&f)) {
        return false;
    }
    if (doc.doctype().name() != "amarok-vlc-config") {
        return false;
    }
    QDomElement e = doc.documentElement();
    if (e.tagName() != "config") {
        return false;
    }
    loadConfig(e);
    return true;
}

char** VlcEngineConfig::vlcArgv() const
{
    return m_argv;
}

int VlcEngineConfig::vlcArgc() const
{
    return m_argc;
}

QPair<uint, char**> VlcEngineConfig::mediaOptions(const QString& mediaType) const
{
    QMap<QString, uint>::const_iterator it = m_mediaTypes.find(mediaType);
    if (it != m_mediaTypes.end()) {
        VlcMediaOptionsList::const_iterator optIt = m_mediaOptions.at(it.data());
        if (optIt != m_mediaOptions.end()) {
            return *optIt;
        }
    }
    return QPair<uint, char**>(0, NULL);
}

QString VlcEngineConfig::configName() const
{
    return m_name;
}

void VlcEngineConfig::clearMediaOptions()
{
    VlcMediaOptionsList::iterator it = m_mediaOptions.begin();
    VlcMediaOptionsList::iterator itEnd = m_mediaOptions.end();
    for (; it != itEnd; ++it) {
        deleteOptions((*it).first, (*it).second);
    }
    m_mediaOptions.clear();
}

void VlcEngineConfig::deleteOptions(uint& argc, char**& argv) const
{
    for (uint i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
    argc = 0;
    argv = NULL;
}

QPair<uint, char**> VlcEngineConfig::splitOptionsString(const QString& s) const
{
    QStringList argv = QStringList::split(QRegExp("\\s"), s);
    QPair<uint, char**> args;
    args.first = argv.count();
    args.second = new char*[args.first];
    uint i = 0;
    for (QStringList::Iterator it = argv.begin(); it != argv.end(); ++it) {
        char *opt = new char[(*it).length() + 1];
        const char* cstr = (*it).latin1();
        qCopy(cstr, cstr + (*it).length() + 1, opt); // +1 adds the trailing \0
        args.second[i] = opt;
       ++i; 
    }
    return args;
}

void VlcEngineConfig::loadConfig(const QDomElement& e)
{
    m_name = e.attribute("name");
    for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling()) {
        if (n.isElement()) {
            QDomElement c = n.toElement();
            if (c.tagName() == "vlc-options") {
                loadVlcOptions(c);
            } else if (c.tagName() == "media-options") {
                loadMediaOptions(c);
            }
        }
    }
}

void VlcEngineConfig::loadVlcOptions(const QDomElement& e)
{
    // argv[0] is the program name but as we are using a library we have to provide a dummy entry for argv[0] : amarok-vlc
    QPair<uint, char**> opts = splitOptionsString(QString("amarok-vlc ") + e.text());
    m_argc = opts.first;
    m_argv = opts.second;
}

void VlcEngineConfig::loadMediaOptions(const QDomElement& e)
{
    for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling()) {
        if (n.isElement()) {
            QDomElement c = n.toElement();
            if (c.tagName() == "media") {
                loadMedia(c);
            }
        }
    }
}

void VlcEngineConfig::loadMedia(const QDomElement& e)
{
    uint i = m_mediaOptions.count();
    m_mediaOptions.append(splitOptionsString(e.text()));
    QStringList medias = QStringList::split(QRegExp("\\s"), e.attribute("types"));
    for (QStringList::Iterator it = medias.begin(); it != medias.end(); ++it) {
        m_mediaTypes[*it] = i;
    }
}
