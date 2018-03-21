#pragma once

#include <vector>
#include <string>

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

/*class UIList : UIComponent {
public:
	UIList(std::vector<std::string>);
	~UIList();

};*/