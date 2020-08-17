
#include "NuoMenu.h"
#include "NuoStrings.h"



NuoMenuBar::NuoMenuBar()
{
	_hMenu = CreateMenu();
}


NuoMenuBar::~NuoMenuBar()
{
	DestroyMenu(_hMenu);
}


void NuoMenuBar::AppendMenu(const PNuoMenu& menu)
{
	_menus.push_back(menu);
}


void NuoMenuBar::Update()
{
	int count = GetMenuItemCount(_hMenu);
	for (int i = 0; i < count; ++i)
		RemoveMenu(_hMenu, i, MF_BYPOSITION);

	for (PNuoMenu item : _menus)
	{
		std::wstring wtitle = StringToUTF16(item->Title());
		::AppendMenu(_hMenu, MF_POPUP, (UINT_PTR)item->Handle(), wtitle.c_str());
	}
}


HMENU NuoMenuBar::Handle() const
{
	return _hMenu;
}


bool NuoMenuBar::DoAction(int id)
{
	for (PNuoMenu menu : _menus)
	{
		if (menu->HasID(id))
		{
			menu->DoActionForID(id);
			return true;
		}

		return false;
	}
}


NuoMenu::NuoMenu(const std::string& title)
	: _title(title)
{
	_hMenu = CreateMenu();
}


NuoMenu::~NuoMenu()
{
	DestroyMenu(_hMenu);
}


void NuoMenu::AppenMenuItem(const PNuoMenuItem& item)
{
	_items.insert(std::make_pair(item->ID(), item));
}


void NuoMenu::Update()
{
	int count = GetMenuItemCount(_hMenu);
	for (int i = 0; i < count; ++i)
		RemoveMenu(_hMenu, i, MF_BYPOSITION);

	for (auto item : _items)
	{
		PNuoMenuItem menuItem = item.second;

		switch (menuItem->MenuType())
		{
		case NuoMenuItem::kMenuItem_String:
		{
			std::wstring wtext = StringToUTF16(menuItem->Text());
			AppendMenu(_hMenu, MF_STRING, menuItem->ID(), wtext.c_str());
			break;
		}
		case NuoMenuItem::kMenuItem_Separator:
		{
			AppendMenu(_hMenu, MF_SEPARATOR, 0, nullptr);
			break;
		}
		default:
			break;
		}
	}
}


std::string NuoMenu::Title() const
{
	return _title;
}


HMENU NuoMenu::Handle() const
{
	return _hMenu;
}


bool NuoMenu::HasID(int id) const
{
	return _items.find(id) != _items.end();
}


void NuoMenu::DoActionForID(int id)
{
	auto item = _items.find(id);

	if (item != _items.end())
	{
		PNuoMenuItem menuItem = item->second;
		menuItem->DoAction();
	}
}



NuoMenuItem::NuoMenuItem(int id, const Type type)
	: _id(id), _type(type)
{
}


NuoMenuItem::NuoMenuItem(int id, const std::string& text)
	: _id(id),
	  _type(kMenuItem_String),
	  _text(text)
{
}


NuoMenuItem::Type NuoMenuItem::MenuType() const
{
	return _type;
}


std::string NuoMenuItem::Text() const
{
	return _text;
}


int NuoMenuItem::ID() const
{
	return _id;
}


void NuoMenuItem::SetAction(MenuAction action)
{
	_action = action;
}



void NuoMenuItem::DoAction()
{
	_action(this->shared_from_this());
}
