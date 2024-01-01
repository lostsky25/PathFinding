#include "Core/MainWindow.h"
#include <iostream>

static bool s_SFMLInitialized = false;

Window* Window::create(const WindowProps& props) {
    return new MainWindow(props);
}

MainWindow::MainWindow(const WindowProps& props) {
    init(props);
}

MainWindow::~MainWindow() {
    shutdown();
}

void MainWindow::init(const WindowProps& props) {
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;

    PF_CORE_TRACE("Creating window {0} ({1} {2}).", props.title, props.width, props.height);

    if (!s_SFMLInitialized) {
        s_SFMLInitialized = true;
    }

    window = new sf::RenderWindow(sf::VideoMode(props.width, props.height), props.title);
}

void MainWindow::shutdown() {
    delete window;
}

void MainWindow::OnUpdate() {

    sf::Event event;
    while (window->pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(*window, event);
        if (event.type == sf::Event::Closed) {
            WindowCloseEvent event;
            m_Data.eventCallback(event);
        }
        if (event.type == sf::Event::Resized) {
            m_Data.height = event.size.height;
            m_Data.width = event.size.width;
            WindowResizeEvent event(m_Data.width, m_Data.height);
            m_Data.eventCallback(event);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Right) {
                MouseButtonPressedEvent event(sf::Mouse::Right);
                m_Data.eventCallback(event);
            }
            if (event.mouseButton.button == sf::Mouse::Left) {
                MouseButtonPressedEvent event(sf::Mouse::Left);
                m_Data.eventCallback(event);
            }
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Right) {
                MouseButtonReleasedEvent event(sf::Mouse::Right);
                m_Data.eventCallback(event);
            }
            if (event.mouseButton.button == sf::Mouse::Left) {
                MouseButtonReleasedEvent event(sf::Mouse::Left);
                m_Data.eventCallback(event);
            }
        }
        if (event.type == sf::Event::MouseMoved) {
            MouseMovedEvent event(event.mouseMove.x, event.mouseMove.y);
            m_Data.eventCallback(event);
        }
    }
}

void MainWindow::setVSync(bool) {

}

bool MainWindow::isVSync() const {
    return false;
}