#pragma once

class UIComponent
{
private:
	int m_x;
	int m_y;
public:
	UIComponent();
	~UIComponent();
};

class Button : public UIComponent { 

};