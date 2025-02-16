#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <ctime>



using namespace std;
class Effect{
    
    
    sf::Clock clock;
    sf::IntRect frame;
    
    public:

    sf::Vector2f pos;
    int state=0;
    void effectChange(sf::Sprite showEffect,sf::Sound &sound,sf::Vector2f position,sf::RenderWindow &window,bool &pause);   
    
    Effect()
    :frame(sf::IntRect({0,0},{48,48})){}

};

void Effect::effectChange(sf::Sprite effectSprite,sf::Sound &sound,sf::Vector2f position,sf::RenderWindow &window,bool &pause)
{   
    
    if(clock.getElapsedTime().asSeconds() > 0.1f){
        frame.position.x = state * 48;
        clock.restart();
        if(state==0) {sound.play();}
        if(state<=7) state++;
    }
    effectSprite.setTextureRect(frame);
    effectSprite.setPosition(pos);
    if(!pause)
    window.draw(effectSprite);
}




