#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

#include "Core/Application.h"
#include "Core/Layer.h"

#include "UI/font_awesome.h"

#include "UI/Elements/Slider_1.h"
#include "UI/Elements/Button.h"

#include <vector>
#include <stack>
#include <unordered_map>

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void lockControlPanel();
	void unlockControlPanel();

	void begin();
	void end();

	sf::RenderTexture& getRenderTexture();

	void onUpdate() override;

	bool getFinishState();
	void finishStateClear();
	void finishState();

	Point getViewportWindowPos() const;
	Point getMousePosition(Point);

	void btnExit();
	void btnContinueClicked();
	void btnSetStartBoxClicked();
	void btnSetStopBoxClicked();
	void btnSetBfsClicked();
	void btnSetDfsClicked();
	void btnStartClicked();
	void btnPauseClicked();
	void btnClearClicked();
	void sliderRowsChanged(int);
	void sliderColsChanged(int);

private:
	bool warningBoxState = false;
	bool m_finishState = false;

    ImFont* font;
	
	Point viewportPos;

    sf::RenderTexture rt;
    sf::Clock deltaClock;

	std::unordered_map<std::string, Base_UI*> elements;
    std::string currentAlgorithmName = "BFS";
};
