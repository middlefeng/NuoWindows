
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
	_items.push_back(item);
}


void NuoMenu::Update()
{
	int count = GetMenuItemCount(_hMenu);
	for (int i = 0; i < count; ++i)
		RemoveMenu(_hMenu, i, MF_BYPOSITION);

	for (PNuoMenuItem item : _items)
	{
		switch (item->MenuType())
		{
		case NuoMenuItem::kMenuItem_String:
		{
			std::wstring wtext = StringToUTF16(item->Text());
			AppendMenu(_hMenu, MF_STRING, item->ID(), wtext.c_str());
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