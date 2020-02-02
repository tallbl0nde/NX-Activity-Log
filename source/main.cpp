#include "Application.hpp"

int main() {

#if _NXLINK_
    socketInitializeDefault();
    nxlinkStdio();
#endif

    Main::Application * app = new Main::Application();
    app->run();
    delete app;

#if _NXLINK_
    socketExit();
#endif

    return 0;
}