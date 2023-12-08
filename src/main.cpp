#include <iostream>
#include <vector>
#include <queue>

#include <SFML/Graphics.hpp>

#include "BFS.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>

#include "Application.h"

int main() {
    Application& app = Application::getInstance();

    while (app.isRun()) {
        app.eventHandler();
        app.showUI();
        app.update();
    }

    return 0;
}