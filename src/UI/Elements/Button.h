#pragma once

#include "UI/Base_UI.h"

class Button : public Base_UI {
public:
	Button(std::string text, std::string icon = "") 
	: text(text), 
		visibleState(true),
		icon(icon) {}

	void setCallback(std::function<void(void)> func) {
		callback = func;
	}

	void show() override {
		if (!visibleState) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			
			if (ImGui::Button((icon + text).c_str())) {
				callback();
			}

			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		} else {
			if (ImGui::Button((icon + text).c_str())) {
				callback();
			}
		}
	}

	void setVisible(bool visibleState) override {
		this->visibleState = visibleState;
	}

	bool getVisible() const override {
		return visibleState;
	}

private:
	std::function<void(void)> callback;
	std::string icon;
	std::string text;
	bool visibleState;
};