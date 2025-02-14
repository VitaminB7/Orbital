#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#define MAX_Menu 3

class MainMenu
{

public:
    MainMenu(float width, float height);

    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    void ismainMenu();

    int mainMenuPressed() {
        return mainMenuSelected;
    }
    ~MainMenu();

private:
   int mainMenuSelected;
   sf::Font fonts;
   std::vector<sf::Text> mainMenu;
   //sf::Text mainMenu1(font);
};

MainMenu::MainMenu(float width, float height)
{
    if (!fonts.openFromFile("Fonts/TypeLightSans-KV84p.otf")) {
        std::cerr << "Error: Failed to load font\n";
        exit(1);  
    }

    //Play
    sf::Text playText(fonts);
    playText.setFillColor(sf::Color::Blue);
    playText.setString("Play");
    playText.setCharacterSize(70);
    playText.setPosition(sf::Vector2f(70, 300));
    mainMenu.push_back(playText);
    
    //Option
    sf::Text optionText(fonts);
    optionText.setFillColor(sf::Color::White);
    optionText.setString("Option");
    optionText.setCharacterSize(70);
    optionText.setPosition(sf::Vector2f(70, 400));
    mainMenu.push_back(optionText);

    //Exit
    sf::Text exitText(fonts);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("Exit");
    exitText.setCharacterSize(70);
    exitText.setPosition(sf::Vector2f(70, 500));
    mainMenu.push_back(exitText);

    mainMenuSelected = 0;
}

MainMenu::~MainMenu()
{

}

//Draw MainMenu
void MainMenu::draw(sf::RenderWindow& window)
{
    for (int i = 0; i < 3; i++)
    {
        window.draw(mainMenu[i]);
    }
}

//move up
void MainMenu::moveUp()
{
    if (mainMenuSelected - 1 >= -1)
    {
        mainMenu[mainMenuSelected].setFillColor(sf::Color::White);

        mainMenuSelected--;
        if (mainMenuSelected == -1)
        {
            mainMenuSelected = 2;
        }
        mainMenu[mainMenuSelected].setFillColor(sf::Color::Blue);
    }
}

//move down
void MainMenu::moveDown()
{
    if (mainMenuSelected + 1 <= 3)
    {
        mainMenu[mainMenuSelected].setFillColor(sf::Color::White);

        mainMenuSelected++;
        if (mainMenuSelected == 3)
        {
            mainMenuSelected = 0;
        }
        mainMenu[mainMenuSelected].setFillColor(sf::Color::Blue);
    }
}
