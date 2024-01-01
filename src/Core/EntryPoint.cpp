#include "Core/Application.h"

int main() {
    Log::init();
    
    Application* app = new Application();

    app->run();

    delete app;

    return 0;
}