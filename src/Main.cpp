#include "CommandLineIface.h"

int main(int argc, char** argv)
{
    CommandLineIface cmd;
    if (argc != 2) {
        return -1;
    }
    if (cmd.load(argv[1])) {
        return cmd.run();
    }
    return -2;
}
