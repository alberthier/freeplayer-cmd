#ifndef COMMANDLINEIFACE_H
#define COMMANDLINEIFACE_H

#include <qstring.h>

class VlcEngine;

class CommandLineIface
{
public:
    CommandLineIface();
    virtual ~CommandLineIface();

    virtual bool load(const QString& filename);
    virtual int run();

private:
    VlcEngine* m_engine;
};

#endif // COMMANDLINEIFACE_H
