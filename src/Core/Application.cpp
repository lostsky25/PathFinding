#include "Core/Application.h"
#include "UI/ImGuiLayer.h"

Application* Application::s_Instance = nullptr;

Application::Application() {
    s_Instance = this;

    PF_CORE_ASSERT(s_Instance, "Application already exists!");

    m_Window = Window::create();
    m_Window->setEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    pathFinding = new PathFinding(Size(10, 10));

    m_ImGuiLayer = new ImGuiLayer();
    pushLayer(m_ImGuiLayer);
}

Application::~Application() {
    delete m_ImGuiLayer;
}

void Application::startAlgorithm() {
    startSearch = true;
}

void Application::stopAlgorithm() {
    startSearch = false;
}

bool Application::getStateAlgorithm() const {
    return startSearch;
}

PathFinding& Application::getPathFinding() { 
    return *pathFinding;
}

Application& Application::get() { 
    return *s_Instance;
}

Window* Application::getWindow() { 
    return m_Window;
}

void Application::setApplicationState(ApplicationState applicationState) {
    this->applicationState = applicationState;
}

void Application::pushLayer(Layer* layer) {
    m_LayerStack.pushLayer(layer);
    layer->onAttach();
}

void Application::pushOverlay(Layer* overlay) {
    m_LayerStack.pushOverlay(overlay);
    overlay->onAttach();
}

void Application::close() {
    m_Running = false;
    m_Window->SFML_data()->close();
}

void Application::run() {
    while (m_Running) {
		m_Window->SFML_data()->clear(sf::Color(0, 0, 1));

        m_ImGuiLayer->begin();

        for (Layer* layer : m_LayerStack)
            layer->onUpdate();

        switch (applicationState) {
            case ApplicationState::NONE: {
                m_ImGuiLayer->finishStateClear();
                break;
            }
            case ApplicationState::RUNNIG: {
                if (pathFinding->getAlgorithm().singleStepOfAlg())
                    setApplicationState(ApplicationState::FINISH);
                break;
            }
            case ApplicationState::PAUSE: {

                break;
            }
            case ApplicationState::FINISH: {
                pathFinding->getAlgorithm().drawShortPath();
                m_ImGuiLayer->finishState();
                break;
            }
        }

        vv<sf::RectangleShape> gridView = pathFinding->getAlgorithm().getGrid().getView();

        for (int i = 0; i < gridView.size(); i++) {
            for (int j = 0; j < gridView[0].size(); j++) {
                m_ImGuiLayer->getRenderTexture().draw(gridView[i][j]);
            }
        }

        m_ImGuiLayer->end();
        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
    dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(onMousePressed));
    dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(onMouseRelesed));
    dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMoved));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->onEvent(e);
        if (e.Handled) {
            break;
        }
    }
}

bool Application::onMouseRelesed(MouseButtonReleasedEvent& e) {
    if (e.getMouseButton() == sf::Mouse::Left) {
        mouseState.hold = false;
        mouseState.unhold = true;
    }

    return true;
}

bool Application::onMousePressed(MouseButtonPressedEvent& e) {
    if (e.getMouseButton() == sf::Mouse::Left) {
        mouseState.hold = true;
        mouseState.unhold = false;

        auto p = m_ImGuiLayer->getMousePosition(currentCursorPossition);
        if (pathFinding->isOnGrid(p) && !m_ImGuiLayer->getFinishState()) {
            if (waitSetFrom) {
                pathFinding->getAlgorithm().setFrom(p);
                waitSetFrom = false;
            }
            
            if (waitSetTo) {
                pathFinding->getAlgorithm().setTo(p);
                waitSetTo = false;
            } 
            
            if (p != pathFinding->getAlgorithm().getTo() && p != pathFinding->getAlgorithm().getFrom()) {
                pathFinding->getAlgorithm().getGrid().setBoxColor(p.y, p.x, COLORS_WALL);
            }
        }
    }

    return true;
}

bool Application::onMouseMoved(MouseMovedEvent& e) {
    currentCursorPossition.x = static_cast<int>(e.getX());
    currentCursorPossition.y = static_cast<int>(e.getY());
    
    if (mouseState.hold && !mouseState.unhold) {
        auto p = m_ImGuiLayer->getMousePosition(currentCursorPossition);
        if (pathFinding->isOnGrid(p) && !m_ImGuiLayer->getFinishState()) {
            
            if (waitSetFrom) {
                pathFinding->getAlgorithm().setFrom(p);
                waitSetFrom = false;
            }
            
            if (waitSetTo) {
                pathFinding->getAlgorithm().setTo(p);
                waitSetTo = false;
            }

            if (p != pathFinding->getAlgorithm().getTo() && p != pathFinding->getAlgorithm().getFrom()) {
                pathFinding->getAlgorithm().getGrid().setBoxColor(p.y, p.x, COLORS_WALL);
            }
        }
    }
    
    return true;
}

bool Application::onWindowClose(WindowCloseEvent& e)
{
    close();
    return true;
}