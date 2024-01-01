#include "ImGuiLayer.h"

ImGuiLayer::ImGuiLayer() : Layer("ImGui layer") {
    ImGui::SFML::Init(*Application::get().getWindow()->SFML_data());
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    static float fontSize = 18.0f;

    font = io.Fonts->AddFontFromFileTTF("resources/fonts/HedvigLettersSansRegular.ttf", fontSize);

    io.Fonts->AddFontFromFileTTF("resources/fonts/fa-regular-400.ttf", fontSize, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF("resources/fonts/fa-solid-900.ttf", fontSize, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF("resources/fonts/fa-light-300.ttf", fontSize, &config, icon_ranges);

    ImGui::SFML::UpdateFontTexture();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    elements["btnSetBfs"] = new Button(" Set BFS");
    elements["btnSetDfs"] = new Button(" Set DFS");
    elements["btnStart"] = new Button(" Start", ICON_FA_PLAY);
    elements["btnPause"] = new Button(" Stop", ICON_FA_STOP);
    elements["btnClear"] = new Button(" Clear", ICON_FA_SYNC);
    elements["btnSetStartBox"] = new Button(" Set start box", ICON_FA_MAP_MARKER_ALT);
    elements["btnSetEndBox"] = new Button(" Set end box", ICON_FA_MAP_MARKER_ALT);
    elements["btnContinue"] = new Button(" Continue", ICON_FA_PLAY);
    elements["btnExit"] = new Button(" Exit", ICON_FA_WINDOW_CLOSE);

    elements["sliderRows"] = new Slider_1<int>(" Rows", {10, 30});
    elements["sliderColumns"] = new Slider_1<int>(" Cols", {10, 30});

    static_cast<Button*>(elements["btnSetBfs"])->setCallback(std::bind(&ImGuiLayer::btnSetBfsClicked, this));
    static_cast<Button*>(elements["btnSetDfs"])->setCallback(std::bind(&ImGuiLayer::btnSetDfsClicked, this));
    static_cast<Button*>(elements["btnStart"])->setCallback(std::bind(&ImGuiLayer::btnStartClicked, this));
    static_cast<Button*>(elements["btnPause"])->setCallback(std::bind(&ImGuiLayer::btnPauseClicked, this));
    static_cast<Button*>(elements["btnContinue"])->setCallback(std::bind(&ImGuiLayer::btnContinueClicked, this));
    static_cast<Button*>(elements["btnClear"])->setCallback(std::bind(&ImGuiLayer::btnClearClicked, this));
    static_cast<Button*>(elements["btnSetStartBox"])->setCallback(std::bind(&ImGuiLayer::btnSetStartBoxClicked, this));
    static_cast<Button*>(elements["btnSetEndBox"])->setCallback(std::bind(&ImGuiLayer::btnSetStopBoxClicked, this));
    static_cast<Button*>(elements["btnExit"])->setCallback(std::bind(&ImGuiLayer::btnExit, this));
    
    static_cast<Slider_1<int>*>(elements["sliderRows"])->setCallback(std::bind(&ImGuiLayer::sliderRowsChanged, this, std::placeholders::_1));
    static_cast<Slider_1<int>*>(elements["sliderColumns"])->setCallback(std::bind(&ImGuiLayer::sliderColsChanged, this, std::placeholders::_1));
}

ImGuiLayer::~ImGuiLayer() {
    for (auto& [name, elem] : elements) {
        if (elem != nullptr)
            delete elem;
    }
}

void ImGuiLayer::lockControlPanel() {
    for (const auto [name, elem] : elements)
        elem->setVisible(false);
}

void ImGuiLayer::unlockControlPanel() {
    for (const auto [name, elem] : elements)
        elem->setVisible(true);
}

void ImGuiLayer::begin() {
    ImGui::SFML::Update(*Application::get().getWindow()->SFML_data(), deltaClock.restart());
}

void ImGuiLayer::end() {
    ImGui::SFML::Render(*Application::get().getWindow()->SFML_data());
    Application::get().getWindow()->SFML_data()->display();
}

sf::RenderTexture& ImGuiLayer::getRenderTexture() {
    return rt;
}

void ImGuiLayer::onUpdate() {
    if (warningBoxState) {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 pos(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoSavedSettings;

        ImGui::Begin("Warning", nullptr, flags);

        std::string speed_text = "To run the algorithm, you must select a start and finish point.\n";
        ImGui::Text(speed_text.c_str());

        ImGui::Spacing();
        ImGui::NextColumn();

        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::NewLine();

        ImGui::SameLine(270);

        if (ImGui::Button("OK", ImVec2(100, 0))) {
            warningBoxState = false;
        }
        ImGui::End();
    } else {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        
        io.DisplaySize = ImVec2(static_cast<float>(app.getWindow()->getWidth()), static_cast<float>(app.getWindow()->getHeight()));
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

            ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindow");
            ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode | 
                ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
            
            ImGui::End();

            if (ImGui::Begin("Viewport")) {
                auto p = ImGui::GetWindowPos();
                viewportPos = { static_cast<int>(p.x), static_cast<int>(p.y) };
                
                ImVec2 viewportSize = ImGui::GetContentRegionAvail();
                rt.create(static_cast<unsigned>(viewportSize.x), static_cast<unsigned>(viewportSize.y));
                ImGui::Image(rt);
            }
            ImGui::End();
        }

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

        if (ImGui::Begin("Preferences")) {
            ImGui::Text(("Algorithm: " + currentAlgorithmName).c_str());
            
            elements["sliderRows"]->show();
            elements["sliderColumns"]->show();
            
            elements["btnSetBfs"]->show();
            elements["btnSetDfs"]->show();
            
            if (elements["btnStart"]->getVisible()) {
                elements["btnStart"]->show();
            } else {
                if (elements["btnPause"]->getVisible()) {
                    elements["btnPause"]->show();
                } else {
                    elements["btnContinue"]->show();
                }
            }

            elements["btnClear"]->show();
            elements["btnSetStartBox"]->show();
            elements["btnSetEndBox"]->show();

            elements["btnExit"]->show();
        }

        ImGui::End();
        ImGui::PopStyleVar(1);
        ImGui::PopFont();
        rt.clear();
    }
}

bool ImGuiLayer::getFinishState() {
    return m_finishState;
}

void ImGuiLayer::finishStateClear() {
    m_finishState = false;
}

void ImGuiLayer::finishState() {
    m_finishState = true;

    for (const auto [name, elem] : elements)
        if (name == "btnClear")
            elem->setVisible(true);
        else
            elem->setVisible(false);
}

Point ImGuiLayer::getViewportWindowPos() const {
    return Point(viewportPos.x, viewportPos.y);
}

Point ImGuiLayer::getMousePosition(Point absolutePos) {
    absolutePos.x -= getViewportWindowPos().x;
    absolutePos.y -= getViewportWindowPos().y;
    
    const int titleBarHeight = static_cast<int>(ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f);
    absolutePos = { (absolutePos.x - 1) / (10 + 1) - 1, (absolutePos.y - titleBarHeight - 1) / (10 + 1) - 1 };
    return absolutePos;
}

// ==========UI Elements=================

void ImGuiLayer::btnExit() {
    Application& app = Application::get();
    app.close();
}

void ImGuiLayer::btnContinueClicked() {
    btnStartClicked();
}

void ImGuiLayer::btnSetStartBoxClicked() {
    Application& app = Application::get();
    app.waitSetFrom = true;
    app.waitSetTo = false;
}

void ImGuiLayer::btnSetStopBoxClicked() {
    Application& app = Application::get();
    app.waitSetTo = true;
    app.waitSetFrom = false;
}

void ImGuiLayer::btnSetBfsClicked() {
    Application& app = Application::get();
    app.getPathFinding().setAlgorithm(AlgorithmName::BreadthFirstSearch);

    static_cast<Slider_1<int>*>(elements["sliderRows"])->update(app.getPathFinding().getAlgorithm().getGridSize().height);
    static_cast<Slider_1<int>*>(elements["sliderColumns"])->update(app.getPathFinding().getAlgorithm().getGridSize().width);

    currentAlgorithmName = "BFS";
}

void ImGuiLayer::btnSetDfsClicked() {
    Application& app = Application::get();
    app.getPathFinding().setAlgorithm(AlgorithmName::DepthFirstSearch);

    static_cast<Slider_1<int>*>(elements["sliderRows"])->update(app.getPathFinding().getAlgorithm().getGridSize().height);
    static_cast<Slider_1<int>*>(elements["sliderColumns"])->update(app.getPathFinding().getAlgorithm().getGridSize().width);
    
    currentAlgorithmName = "DFS";
}

void ImGuiLayer::btnStartClicked() {
    
    Application& app = Application::get();
    if (app.getPathFinding().getAlgorithm().hasFromTo()) {
        app.setApplicationState(ApplicationState::RUNNIG);

        elements["btnSetBfs"]->setVisible(false);
        elements["btnSetDfs"]->setVisible(false);
        elements["btnStart"]->setVisible(false);
        elements["btnPause"]->setVisible(true);
        elements["btnClear"]->setVisible(false);
        elements["btnSetStartBox"]->setVisible(false);
        elements["btnSetEndBox"]->setVisible(false);
        elements["sliderRows"]->setVisible(false);
        elements["sliderColumns"]->setVisible(false);
    } else {
        warningBoxState = true;
    }
}

void ImGuiLayer::btnPauseClicked() {
    Application& app = Application::get();
    app.setApplicationState(ApplicationState::PAUSE);
    elements["btnPause"]->setVisible(false);
}

void ImGuiLayer::btnClearClicked() {
    Application& app = Application::get();
    app.getPathFinding().clear();
    app.setApplicationState(ApplicationState::NONE);
    unlockControlPanel();
}

void ImGuiLayer::sliderRowsChanged(int rows) {
    Application& app = Application::get();
    int cols = app.getPathFinding().getAlgorithm().getGridSize().width;

    app.getPathFinding().getAlgorithm().setGridSize(Size(cols, rows));
}

void ImGuiLayer::sliderColsChanged(int cols) {
    Application& app = Application::get();
    int rows = app.getPathFinding().getAlgorithm().getGridSize().height;

    app.getPathFinding().getAlgorithm().setGridSize(Size(cols, rows));
}