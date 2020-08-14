#include "Application.hpp"

int main() {
    Main::Application * app = new Main::Application();
    app->run();
    delete app;
    return 0;
}