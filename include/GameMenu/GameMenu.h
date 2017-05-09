/*
*	Copyright (C) 2016 Sidhin S Thomas
*
*	This software is licensed under the MIT License
*/

#ifndef SS_USER_INTERFACE
#define SS_USER_INTERFACE


/*--- Headers ---*/
#include <SFML/Graphics.hpp>
#include <memory>
namespace gmenu {

	
	/* Simple abstract class interface to implement Actions */
	class Action {
	public:
		/* The function that will be called by the menu
		Depending on the return value of start() the menu will close
		- If start() return true : menu will not close
		- If start() return false: menu will close

		usefull when you need to implement a feature after which it should not return to the menu.
		eg. Back/Exit/Game Mode etc.
		*/
		virtual void start() = 0;
	};

	struct MenuItem {
		std::shared_ptr<Action> action;
		std::string title;
	};

	/* BitFlags for Different possible Layouts */
	enum Layout {

		TitleCentre = 1 << 0,
		TitleRight = 1 << 1,
		TitleLeft = 1 << 2,

		ItemCentre = 1 << 3,
		ItemRight = 1 << 4,
		ItemLeft = 1 << 5,

		Default = TitleCentre | ItemCentre,
					  
	};

	/* Defines the style of the menu */
	struct Style {
		sf::Font &TitleFont;
		sf::Font &ItemFont;

		sf::Color TitleColor = sf::Color::Green;;
		sf::Color ItemColor = sf::Color::Red ;
		sf::Color Selected = sf::Color::Blue;

		unsigned int TitleFontSize = 50;
		unsigned int ItemFontSize = 20;

		float MenuTitleScaleFactor = 1.25;
		float MenuItemScaleFactor = 0.25;

		struct {
			float top, left;
		} PaddingItems, PaddingTitle;

		int layout = Layout::Default;
		Style(sf::Font &mf, sf::Font &itmf):
			TitleFont( mf ), ItemFont( itmf ), PaddingTitle {10,0}, PaddingItems {0,0}
		{	
		}
	};


	
	
	class Menu {
		/* Generic Menu - can be instantiated to generate a custom menu as needed over a sf::RenderWindow */

	public:
		
		/* Only available constructor */
		Menu(sf::RenderWindow &wnd, std::string title, std::vector<MenuItem> items, Style &st):
			style( st ), window (wnd) {
			menuTitle = title;
			menuItems = items;
			setMenu();
		}

	    /* Handles the events that occured and takes action accordingly" */
		void handleEvent(sf::Event);

		/* Renders the menu to the window */
		void drawMenu();


		/* In case menu items needs to be changed */
		void setMenuItems( std::vector<MenuItem> );
		
		/* In case the title needs to be changed */
		void setTitle(std::string title);
		

	private:
		

		void writeText( std::string string, sf::Font font, unsigned int size, float x, float y,
			const sf::Color &color);

		void setMenu();



		/*==================================================*
		*				Internal structuers        			*
		*===================================================*/

		std::vector<MenuItem> menuItems;

		sf::Vector2f title_location;
		std::vector<sf::Vector2f> item_location;

		/*==================================================*
		*					Data Members					*
		*===================================================*/

		int currently_selected_item = 0;

		Style &style;
		
		sf::RenderWindow &window;
		std::string menuTitle;		

	}; // Menu


	/*==================================================*
	*				Operator overload					*
	*===================================================*/
	template<class T> inline T operator~ ( T a ) {
		return (T) ~(int) a;
	}
	template<class T> inline T operator| ( T a, T b ) {
		return (T) ((int) a | (int) b);
	}
	template<class T> inline T operator& ( T a, T b ) {
		return (T) ((int) a & (int) b);
	}
	template<class T> inline T operator^ ( T a, T b ) {
		return (T) ((int) a ^ (int) b);
	}
	template<class T> inline T& operator|= ( T& a, T b ) {
		return (T&) ((int&) a |= (int) b);
	}
	template<class T> inline T& operator&= ( T& a, T b ) {
		return (T&) ((int&) a &= (int) b);
	}
	template<class T> inline T& operator^= ( T& a, T b ) {
		return (T&) ((int&) a ^= (int) b);
	}


} // namespace gmenu

#endif