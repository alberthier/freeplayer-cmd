#include "CommandLineIface.h"
#include <qfile.h>
#include <qtextstream.h>
#include "VlcEngine.h"

CommandLineIface::CommandLineIface()
    : m_engine(NULL)
{
    m_engine = new VlcEngine();
}

CommandLineIface::~CommandLineIface()
{
    delete m_engine;
}

bool CommandLineIface::load(const QString& filename)
{
    QTextStream out(stdin, IO_WriteOnly);
    if (!m_engine->init()) {
        out << "Initialization failed.\n";
        return 1;
    }
    return m_engine->load(filename);
}

int CommandLineIface::run()
{
    QTextStream in(stdin, IO_ReadOnly);
    QTextStream out(stdin, IO_WriteOnly);
    QString cmd;
    uint arg = 0;

    while (cmd != "quit") {
        in >> cmd;
        if (cmd == "position") {
            qDebug("%d\n", m_engine->position());
        } else if (cmd == "play") {
            //in >> arg;
            out << m_engine->play(0) << "\n";
        } else if (cmd == "stop") {
            m_engine->stop();
        } else if (cmd == "pause") {
            m_engine->pause();
        } else if (cmd == "length") {
            qDebug("%d\n", m_engine->length());
        } else if (cmd == "seek") {
            in >> arg;
            m_engine->seek(arg);
        } else if (cmd == "volume") {
            in >> arg;
            m_engine->setVolumeSW(arg);
        }
    }

    return 0;
}
