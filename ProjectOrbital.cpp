#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

using namespace std;

class Player
{ 
public: 

    int HP;
    int HPmax;
    
    Player(){
        HPmax=10;
        HP=HPmax;
    }

};

class Enemy
{
public: 
     
    int HP;
    int HPmax;
    
    Enemy(){
        HPmax=3;
        HP=HPmax;
    }

};

int main()
{
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({800,800}),"Orbital",sf::Style::Close, sf::State::Windowed, setting);
    window.setFramerateLimit(60);

    //texture
    sf::Texture playerTexture("SpaceShooterShipConstructor/PNG/Example/01.png"); 
    sf::Texture playerBulletTexture("SpaceShooterShipConstructor/PNG/Bullets/01.png");   

    //player
    Player player;
    sf::Sprite playerShape(playerTexture);
    playerShape.setScale({0.5f,0.5f});
    playerShape.setPosition({500,500});

    //player bullet
    sf::Sprite bulletShape(playerBulletTexture);
    std::vector<sf::Sprite> vBullet;

    
    //Player factor
    int shootTimer = 100;

    while (window.isOpen())
    {
        while(std::optional event = window.pollEvent())
        {
            //close
            if(event->is<sf::Event::Closed>()) window.close();

        }

    //update
    
    //movement
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    playerShape.move({0.f,-10.f});
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    playerShape.move({-10.f,0.f});
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    playerShape.move({0.f,10.f});
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    playerShape.move({10.f,0.f});
    
    if(playerShape.getPosition().x <= 0) playerShape.setPosition({0.f,playerShape.getPosition().y});//left
    if(playerShape.getPosition().x + playerShape.getGlobalBounds().size.x  >= window.getSize().x) playerShape.setPosition({window.getSize().x - playerShape.getGlobalBounds().size.x ,playerShape.getPosition().y});//right
    if(playerShape.getPosition().y <= 0) playerShape.setPosition({playerShape.getPosition().x,0.f});//top
    if(playerShape.getPosition().y + playerShape.getGlobalBounds().size.y >= window.getSize().y) playerShape.setPosition({playerShape.getPosition().x,window.getSize().y - playerShape.getGlobalBounds().size.y});//bottom


    //player shoot
    if(shootTimer < 10) shootTimer++;
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shootTimer >=10)
    {
            vBullet.push_back(bulletShape);
            vBullet.back().setScale({0.5,0.5});
            vBullet.back().setPosition(playerShape.getPosition());
            shootTimer = 0;
    }
    
    
    //clear
    window.clear(sf::Color(124,124,124));
    
    //player Bullet
    for (int i = 0; i < vBullet.size() ; i++)
    {
        window.draw(vBullet[i]);
        vBullet[i].move({0.f,-10.f});
        //bullet go out
        if(vBullet[i].getPosition().y <= -10) {vBullet.erase(vBullet.begin() + i); break;}
    
    }

    
    //draw
    window.draw(playerShape);
    
    //show
    window.display();

 
    }
    return 0;
}

