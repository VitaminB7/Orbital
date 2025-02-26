#pragma once
#include <SFML\Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class MainMenu
{

public:
    
    MainMenu(float width, float height) {
    if (!fonts.openFromFile("C:/CPLUSPLUS/Project/Ui/Fonts/TypeLightSans-KV84p.otf")) {
        std::cerr << "Error: Failed to load font\n";
        exit(1);  // ปิดโปรแกรมทันที
    }

    //Play
    sf::Text playText(fonts);
    playText.setFillColor(sf::Color::Blue);
    playText.setString("Play");
    playText.setCharacterSize(100);
    playText.setPosition(sf::Vector2f(70, 300));
    mainMenu.push_back(playText);
    
    //Setting
    sf::Text settingText(fonts);
    settingText.setFillColor(sf::Color::White);
    settingText.setString("Setting");
    settingText.setCharacterSize(100);
    settingText.setPosition(sf::Vector2f(70, 450));
    mainMenu.push_back(settingText);

    //Exit
    sf::Text exitText(fonts);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("Exit");
    exitText.setCharacterSize(100);
    exitText.setPosition(sf::Vector2f(70, 600));
    mainMenu.push_back(exitText);

    mainMenuSelected = 0;
}

    //Draw MainMenu
void draw(sf::RenderWindow& window)
{
    for (int i = 0; i < 3; i++)
    {
        window.draw(mainMenu[i]);
    }
}

//move up
void moveUp()
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
void moveDown()
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
    void ismainMenu();

    int mainMenuPressed() {
        return mainMenuSelected;
    }
    ~MainMenu(){

    }

private:
   int mainMenuSelected;
   sf::Font fonts;
   std::vector<sf::Text> mainMenu;
   //sf::Text mainMenu1(font);
};


class ButtonSlide 
{
public:
    ButtonSlide(float x, float y, float width){
        track.setSize(sf::Vector2f(width, 20));
        track.setPosition({x, y});
        track.setFillColor(sf::Color::White);

        float center = x + width / 2;
        fillColor.setSize({center-x, 20});
        fillColor.setPosition({x, y});
        fillColor.setFillColor(sf::Color::Blue);

        button.setRadius(30);
        button.setOrigin({30, 30}); 
        button.setPosition({center, y + 10});
        button.setFillColor(sf::Color::Red);

        min = x;
        max = x + width;
    }
    

    void handle(sf::RenderWindow& window) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (button.getGlobalBounds().contains(mousePos)) {
                isDragging = true;
                wasClicked = true; 
            }
        }
        else{
            isDragging = false;
            wasClicked = false;
        }
    
        if (isDragging) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            float newX = std::clamp(mousePos.x, min, max);
            button.setPosition({newX, button.getPosition().y});

            float fillWidth = newX - min;
            fillColor.setSize({fillWidth, 20});
            
            float volume = ((newX - min) / (max - min)) * 100;
            updateSound(volume);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(track);
        window.draw(fillColor);
        window.draw(button);
    }

    void setUpdateFunction(std::function<void(float)> func) {
        updateSound = func;
    }
    
private:
    sf::RectangleShape track;
    sf::RectangleShape fillColor;
    sf::CircleShape button;
    float min, max;
    bool isDragging = false;
    bool wasClicked = false;
    
    std::function<void(float)> updateSound = [](float) {};
};
