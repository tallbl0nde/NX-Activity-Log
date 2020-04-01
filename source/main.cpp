#include "Application.hpp"

int main() {

#if _NXLINK_
    nxlinkStdio();
#endif

    Main::Application * app = new Main::Application();
    app->run();
    delete app;

    return 0;
}