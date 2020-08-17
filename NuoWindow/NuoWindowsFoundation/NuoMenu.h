#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>



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

	bool DoAction(int id);
};



class NuoMenu
{
	HMENU _hMenu;

	std::map<int, PNuoMenuItem> _items;
	std::string _title;

public:
	NuoMenu(const std::string& title);
	~NuoMenu();

	void AppenMenuItem(const PNuoMenuItem& item);
	void Update();

	std::string Title() const;
	HMENU Handle() const;

	bool HasID(int id) const;
	void DoActionForID(int id);
};


typedef std::function<void(const PNuoMenuItem&)> MenuAction;


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

	void SetAction(MenuAction action);
	void DoAction();

private:

	Type _type;
	int _id;

	MenuAction _action;
};