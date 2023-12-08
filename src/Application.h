#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>

#include "PathFinding.h"

#include "font_awesome.h"

class Application
{
public:
    static Application& getInstance()
    {
        static Application instance(Size(1000, 600), "Path Finding");
        return instance;
    }

    bool isRun() const;
    void showUI();
    void eventHandler();
    void update();

    ~Application();

private:
    Application() = delete;

    Application(Size windowSize, std::string title);
    Point getMousePosOnGrid(const sf::Window& window);

    Application(Application const&);
    void operator=(Application const&);

private:
    Size size;
    ImFont* font;
    sf::RenderWindow* window;
    sf::RenderTexture rt;

    sf::Clock deltaClock;

    PathFinding* pathFinding;

    std::string currentAlgorithmName = "BFS";

    bool start = false;
    bool _continue = false;
    bool finish = false;
    bool stop = false;
    bool searchState = true;

    bool _isRun = true;

    bool choseStartBox = false;
    bool choseEndBox = false;
    bool clearGridView = false;

    struct MouseState {
        bool hold = false;
        bool unhold = true;
    } mouseState;
};

