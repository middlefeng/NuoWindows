#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>



class NuoMenuItem;
typedef std::shared_ptr<NuoMenuItem> PNuoMenuItem;

class NuoMenu;
typedef std::shared_ptr<NuoMenu> PNuoMenu;


class NuoMenuBar
{
	HMENU _hMenu;

	std::vector<PNuoMenu> _menus;

public:

	NuoMenuBar();
	~NuoMenuBar();

	void AppendMenu(const PNuoMenu& menu);
	void Update();
	HMENU Handle() const;
};



class NuoMenu
{
	HMENU _hMenu;

	std::vector<PNuoMenuItem> _items;
	std::string _title;

public:
	NuoMenu(const std::string& title);
	~NuoMenu();

	void AppenMenuItem(const PNuoMenuItem& item);
	void Update();

	std::string Title() const;
	HMENU Handle() const;
};



class NuoMenuItem : public std::enable_shared_from_this<NuoMenuItem>
{
	std::string _text;

public:
	enum Type
	{
		kMenuItem_String,
		kMenuItem_Separator
	};

	NuoMenuItem(int id, const Type type);
	NuoMenuItem(int id, const std::string& text);

	Type MenuType() const;
	std::string Text() const;
	int ID() const;

private:

	Type _type;
	int _id;
};