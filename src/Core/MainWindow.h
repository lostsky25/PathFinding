#pragma once
#include <SFML/Window/Event.hpp>

#include "UI/Window.h"

class MainWindow : public Window
{
public:
    MainWindow(const WindowProps& props);
    virtual ~MainWindow();

    void OnUpdate() override;

    inline unsigned int getWidth() const override {
        return m_Data.width;
    }

    inline unsigned int getHeight() const override {
        return m_Data.height;
    }

    inline void setEventCallback(const EventCallbackFn& callback) override {
        m_Data.eventCallback = callback;
    }

    void setVSync(bool enabled) override;
    bool isVSync() const override;

private:
    virtual void init(const WindowProps& props);
    virtual void shutdown();

private:

    struct WindowData {
        std::string title;
        unsigned int width, height;
        bool VSync;

        EventCallbackFn eventCallback;
    };

    WindowData m_Data;
};

