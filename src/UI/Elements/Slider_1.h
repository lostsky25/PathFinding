#include "UI/Base_UI.h"

template <class T>
class Slider_1 : public Base_UI {
public:
	Slider_1(std::string text, Range<T> range) 
		: text(text), 
			value(range.min),
			range(range),
			visibleState(true) {}

	void setCallback(std::function<void(T)> func) {
		callback = func;
	}

	void update(T value) {
		this->value = value;
	}

	void show() override {
		if (!visibleState) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

			if (ImGui::SliderInt(text.c_str(), &value, range.min, range.max)) {
				callback(value);
			}

			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		} else {
			if (ImGui::SliderInt(text.c_str(), &value, range.min, range.max)) {
				callback(value);
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
	T value;
	std::function<void(T)> callback;
	Range<T> range;
	std::string text;
	bool visibleState;
};