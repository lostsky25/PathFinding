#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Log.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

struct WindowProps {
    std::string title;
    unsigned int width;
    unsigned int height;

    WindowProps(const std::string& title = "Window",
        unsigned int width = 1280,
        unsigned int height = 720)
        : title(title), width(width), height(height) {}
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void OnUpdate() = 0;

    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual void setEventCallback(const EventCallbackFn& callback) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSync() const = 0;

    sf::RenderWindow* SFML_data() { return window; }

    static Window* create(const WindowProps& props = WindowProps());

protected:
    sf::Clock deltaClock;
    sf::RenderWindow* window;
};

