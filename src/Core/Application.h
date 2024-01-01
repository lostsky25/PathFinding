#pragma once
#include <SFML/Window/Mouse.hpp>

#include "Core/Core.h"
#include "Core/PathFinding.h"

#include "Core/LayerStack.h"
#include "UI/Window.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

enum ApplicationState {
    NONE = 0,
    RUNNIG,
    PAUSE,
    FINISH
};

class ImGuiLayer;

class Application
{
public:
    friend class ImGuiLayer;

    Application();
    Application(const Application&) = delete;
    void operator=(const Application&) = delete;

    ~Application();

    void pushLayer(Layer* layer);
    void pushOverlay(Layer* overlay);

    void run();
    void close();

    void startAlgorithm();
    void stopAlgorithm();

    bool getStateAlgorithm() const;
    void setApplicationState(ApplicationState);

    PathFinding& getPathFinding();

    static Application& get();

    Window* getWindow();

    void OnEvent(Event& e);
    bool onWindowClose(WindowCloseEvent& e);
    bool onMouseMoved(MouseMovedEvent& e);
    bool onMousePressed(MouseButtonPressedEvent& e);
    bool onMouseRelesed(MouseButtonReleasedEvent& e);

private:
    struct MouseState {
        bool hold = false;
        bool unhold = true;
    } mouseState;

    bool waitSetFrom = false;
    bool waitSetTo = false;

    bool m_Running = true;
    bool startSearch = false;

    ApplicationState applicationState = ApplicationState::NONE;
    
    ImGuiLayer* m_ImGuiLayer;

    static Application* s_Instance;

    Window* m_Window;

    LayerStack m_LayerStack;

    PathFinding* pathFinding;

    Point currentCursorPossition;
};

