#pragma once

#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>

#include <string>
#include <functional>

class Base_UI {
public:
	virtual void show() = 0;
	virtual void setVisible(bool) = 0;
	virtual bool getVisible() const = 0;
};

template <class T>
struct Range {
	T min;
	T max;
};