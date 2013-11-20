#ifndef VLCENGINECONFIG_H
#define VLCENGINECONFIG_H

#include <qstring.h>
#include <qmap.h>
#include <qvaluelist.h>
#include <qpair.h>

class QDomElement;

class VlcEngineConfig
{
public:
    VlcEngineConfig();
    virtual ~VlcEngineConfig();

    virtual bool load(const QString& filename);
    virtual char** vlcArgv() const;
    virtual int vlcArgc() const;
    virtual QPair<uint, char**> mediaOptions(const QString& mediaType) const;
    virtual QString configName() const;

private:
    void clearMediaOptions();
    void deleteOptions(uint& argc, char**& argv) const;
    QPair<uint, char**> splitOptionsString(const QString& s) const;
    void loadConfig(const QDomElement& e);
    void loadVlcOptions(const QDomElement& e);
    void loadMediaOptions(const QDomElement& e);
    void loadMedia(const QDomElement& e);

private:
    char** m_argv;
    uint m_argc;
    QMap<QString, uint> m_mediaTypes;
    typedef QValueList<QPair<uint, char**> > VlcMediaOptionsList;
    VlcMediaOptionsList m_mediaOptions;
    QString m_name;
};

#endif // VLCENGINECONFIG_H
