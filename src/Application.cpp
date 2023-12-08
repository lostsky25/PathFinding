#include "Application.h"

Application::Application(Size windowSize, std::string title) {
    window = new sf::RenderWindow(sf::VideoMode(windowSize.width, windowSize.height), title);

    pathFinding = new PathFinding(Size(10, 10));
    this->size = pathFinding->getAlgorithm().getGridSize();

    ImGui::SFML::Init(*window);
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->Clear();
    //io.Fonts->AddFontDefault();

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    static float fontSize = 18.0f;

    font = io.Fonts->AddFontFromFileTTF("../src/fonts/HedvigLettersSansRegular.ttf", fontSize);

    io.Fonts->AddFontFromFileTTF("../src/fonts/fa-regular-400.ttf", fontSize, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF("../src/fonts/fa-solid-900.ttf", fontSize, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF("../src/fonts/fa-light-300.ttf", fontSize, &config, icon_ranges);

    ImGui::SFML::UpdateFontTexture();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

Point Application::getMousePosOnGrid(const sf::Window& window) {
    const int titleBarHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
    sf::Vector2i position = sf::Mouse::getPosition(window);
    return { (position.x - 1) / (10 + 1) - 1, (position.y - titleBarHeight - 1) / (10 + 1) - 1 };
}

bool Application::isRun() const {
    return _isRun;
}

void Application::showUI() {
    ImGui::SFML::Update(*window, deltaClock.restart());
    //ImGui::ShowStyleEditor();
    {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();

        if (ImGui::Begin("Viewport")) {
            ImVec2 viewportSize = ImGui::GetContentRegionAvail();
            rt.create(viewportSize.x, viewportSize.y);
            ImGui::Image(rt);
        }
        ImGui::End();
    }

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    if (ImGui::Begin("Preferences")) {
        if (start || _continue) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::Text(("Algorithm: " + currentAlgorithmName).c_str());

        if (ImGui::Button("Set BFS")) {
            pathFinding->setAlgorithm(AlgorithmType::BreadthFirstSearch);
            currentAlgorithmName = "BFS";
        }

        if (ImGui::Button("Set DFS")) {
            pathFinding->setAlgorithm(AlgorithmType::DepthFirstSearch);
            currentAlgorithmName = "DFS";
        }

        size = pathFinding->getAlgorithm().getGridSize();

        if (ImGui::SliderInt("Rows", &size.height, 3, 50)) {
            pathFinding->getAlgorithm().setGridSize(Size(size.width, size.height));
        }

        if (ImGui::SliderInt("Cols", &size.width, 3, 50)) {
            pathFinding->getAlgorithm().setGridSize(Size(size.width, size.height));
        }


        if (ImGui::Button(ICON_FA_MAP_MARKER_ALT " Set start box")) {
            choseStartBox = true;
            choseEndBox = false;
        }

        if (ImGui::Button(ICON_FA_MAP_MARKER_ALT " Set end box")) {
            choseStartBox = false;
            choseEndBox = true;
        }

        if (start || _continue) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if (!start && !_continue) {
            if (ImGui::Button(ICON_FA_PLAY " Start")) {
                if (pathFinding->getAlgorithm().hasFromTo()) {
                    start = true;
                }

                choseStartBox = false;
                choseEndBox = false;
            }
        }

        if (start && !finish) {
            if (ImGui::Button(ICON_FA_STOP " Stop")) {

                if (start) {
                    _continue = true;
                }

                start = false;
            }
        }
        else if (!start && _continue) {

            if (ImGui::Button(ICON_FA_PLAY " Continue")) {
                start = true;
            }
        }

        if (ImGui::Button(ICON_FA_SYNC " Clear")) {
            start = false;
            _continue = false;
            stop = false;
            finish = false;

            pathFinding->clear();

            choseStartBox = false;
            choseEndBox = false;
        }

        if (ImGui::Button(ICON_FA_WINDOW_CLOSE " Exit")) {
            _isRun = false;
        }
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
    ImGui::PopFont();
}

void Application::eventHandler() {
    sf::Event event;
    while (window->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(*window, event);

        if (event.type == sf::Event::Closed) {
            window->close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            mouseState.hold = true;
            mouseState.unhold = false;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            mouseState.hold = false;
            mouseState.unhold = true;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (choseStartBox) {
                Point pos = getMousePosOnGrid(*window);

                if (pathFinding->isOnGrid(pos)) {
                    pathFinding->setFrom(pos);

                    choseStartBox = false;
                    choseEndBox = false;
                }
            }
            else if (choseEndBox) {
                Point pos = getMousePosOnGrid(*window);

                if (pathFinding->isOnGrid(pos)) {
                    pathFinding->setTo(pos);

                    choseStartBox = false;
                    choseEndBox = false;
                }
            }
        }
    }
}

void Application::update() {
    Point pos = getMousePosOnGrid(*window);

    if (mouseState.hold && !mouseState.unhold) {
        if (pathFinding->isOnGrid(pos)) {
            if (!choseStartBox && !choseEndBox) {
                pathFinding->getAlgorithm().getGrid().setBoxColor(pos.y, pos.x, COLORS_WALL);
            }
        }
    }

    if (start) {
        if (pathFinding->getAlgorithm().singleStepOfAlg()) {
            _continue = false;
            finish = true;
            //start = false;
        }
    }

    vv<sf::RectangleShape> gridView = pathFinding->getAlgorithm().getGrid().getView();

    window->clear();
    rt.clear();

    for (int i = 0; i < gridView.size(); i++) {
        for (int j = 0; j < gridView[0].size(); j++) {
            rt.draw(gridView[i][j]);
        }
    }

    ImGui::SFML::Render(*window);
    window->display();
}

Application::~Application() {
    ImGui::SFML::Shutdown();
}