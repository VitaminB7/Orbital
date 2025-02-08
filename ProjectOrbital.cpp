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
    int type;
    int shootTimer;
    float speed;

    float bulletSpeed = 1; 

    Enemy(int a){
        type = a;
        HPmax=3;
        HP=HPmax;
        speed = 2;
    }

    Enemy(){
        type = 0;
        HPmax=3;
        HP=HPmax;
        shootTimer = 2000;
        speed = 1;
    }

};

int main()
{
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({800,800}),"Orbital",sf::Style::Close, sf::State::Windowed, setting);
    window.setFramerateLimit(144);

    //texture
    sf::Texture playerTexture("C:/CPLUSPLUS/Project/Ui/Player/SpaceShooterShipConstructor/PNG/Example/01.png"); 
    sf::Texture playerBulletTexture("C:/CPLUSPLUS/Project/Ui/Player/SpaceShooterShipConstructor/PNG/Bullets/01.png");   
    sf::Texture followEnemyTexture("C:/CPLUSPLUS/Project/Ui/Player/SpaceShooterShipConstructor/PNG/Example/14.png");
    sf::Texture enemyTexture("C:/CPLUSPLUS/Project/Ui/Player/SpaceShooterShipConstructor/PNG/Example/01.png");
    sf::Texture enemyBulletTexture("C:/CPLUSPLUS/Project/Ui/Player/SpaceShooterShipConstructor/PNG/Bullets/02.png");

    //player
    Player player;
    sf::Sprite playerShape(playerTexture);
    playerShape.setScale({0.5f,0.5f});
    playerShape.setPosition({500,500});
    sf::Vector2f playerCenter;

    //enemy
    Enemy e0;
    Enemy e1(1);
    sf::Sprite enemyShape(enemyTexture);
    enemyShape.setOrigin({enemyShape.getGlobalBounds().size.x/2,enemyShape.getGlobalBounds().size.y/2});
    enemyShape.setRotation(sf::degrees(180.f));
    enemyShape.scale({0.8,0.8});
    std::vector<sf::Sprite> vEnemyTexture;
    std::vector<Enemy> vEnemyHP;

    sf::Sprite followEnemyShape(followEnemyTexture);
    followEnemyShape.setOrigin({followEnemyShape.getGlobalBounds().size.x/2,followEnemyShape.getGlobalBounds().size.y/2});
    followEnemyShape.setRotation(sf::degrees(180.f));
    followEnemyShape.scale({0.8f,0.8f});
    sf::Vector2f followEnemyPOS;
    sf::Vector2f enemyAimDir;
    sf::Vector2f enemyAimDirNorm;

    //player bullet
    sf::Sprite bulletShape(playerBulletTexture);
    std::vector<sf::Sprite> vBullet;
    std::vector<sf::Vector2f> bulletDirections;

    //enemy bullet
    sf::Sprite enemyBulletShape(enemyBulletTexture);
    enemyBulletShape.setOrigin({enemyBulletShape.getGlobalBounds().size.x/2,enemyBulletShape.getGlobalBounds().size.y/2});
    enemyBulletShape.setRotation(sf::degrees(180.f));
    std::vector<sf::Sprite> vEnemyBullet;

    
    //Player factor
    int shootTimer = 100;

    //Enemy factor
    int enemySpawnTimer=2000;

//=============================================================================================//

    while (window.isOpen())
    {
        while(std::optional event = window.pollEvent())
        {
            //close
            if(event->is<sf::Event::Closed>()) window.close();

        }

    //update

    srand(time(0));

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

    //EnemySpawn 
        if (enemySpawnTimer < 200) enemySpawnTimer++;
        if (enemySpawnTimer >= 200) 
    {
        enemySpawnTimer = 0;
        int Rand = rand()%2;
        if (Rand == 0)
        {  
            vEnemyTexture.push_back(enemyShape);
            vEnemyHP.push_back(e0);
        }
        if (Rand == 1)
        {
            vEnemyTexture.push_back(followEnemyShape);
            vEnemyHP.push_back(e1);
        }
        float R = rand() % (int)window.getSize().x;
        if (R > window.getSize().x / 2) 
        vEnemyTexture.back().setPosition({ R - vEnemyTexture.back().getGlobalBounds().size.x, 0.f });
        else vEnemyTexture.back().setPosition({ R + vEnemyTexture.back().getGlobalBounds().size.x, 0.f });


    }

    //Enemy
        for (int i = 0; i < vEnemyTexture.size() ; i++)
        {
           
            //follow enemy
            if(vEnemyHP[i].type == 1){followEnemyPOS = (vEnemyTexture[i].getPosition());}
            playerCenter = {playerShape.getPosition().x + playerShape.getGlobalBounds().size.x/2 , playerShape.getPosition().y + playerShape.getGlobalBounds().size.y/2};
            enemyAimDir = (playerCenter-followEnemyPOS);
            enemyAimDirNorm =  enemyAimDir / (float)sqrt(pow(enemyAimDir.x,2)+pow(enemyAimDir.y,2));
            
            //draw+move
            window.draw(vEnemyTexture[i]);//draw
            if(vEnemyHP[i].type == 0)vEnemyTexture[i].move({0.f,2.f*vEnemyHP[i].speed});//move
            if(vEnemyHP[i].type == 1)vEnemyTexture[i].move(enemyAimDirNorm*vEnemyHP[i].speed);//move

            //enemy shoot
            
            if(vEnemyHP[i].type == 0 && vEnemyHP[i].shootTimer < 50) vEnemyHP[i].shootTimer++;
            if(vEnemyHP[i].type == 0 && vEnemyHP[i].shootTimer >= 50) 
        {
            vEnemyBullet.push_back(enemyBulletShape);
            vEnemyBullet.back().setScale({0.5,0.5});
            vEnemyBullet.back().setPosition(vEnemyTexture[i].getPosition());
            vEnemyHP[i].shootTimer = 0;
        }

            //collision
            if(vEnemyTexture[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds())) {vEnemyTexture.erase(vEnemyTexture.begin()+i); 
            vEnemyHP.erase(vEnemyHP.begin()+i); 
            player.HP--;

            break;} //player
            if(vEnemyTexture[i].getPosition().y>window.getSize().y){
                vEnemyTexture.erase(vEnemyTexture.begin()+i); 
                vEnemyHP.erase(vEnemyHP.begin()+i); 
                break;
            } //out(bottom)
        }

    //player Bullet
    for (int i = 0; i < vBullet.size() ; i++)
    {
        window.draw(vBullet[i]);
        vBullet[i].move({0.f,-10.f});
        //bullet go out
        if(vBullet[i].getPosition().y <= -10) {vBullet.erase(vBullet.begin() + i); break;}
        
        //enemy collision
        for (int k = 0; k < vEnemyTexture.size(); k++)
            {
                if(vBullet[i].getGlobalBounds().findIntersection(vEnemyTexture[k].getGlobalBounds()))
                {
                            
                    vEnemyHP[k].HP--;//dmg
                            
                    if (vEnemyHP[k].HP <= 0) {
                    vEnemyTexture.erase(vEnemyTexture.begin() + k);
                    vEnemyHP.erase(vEnemyHP.begin() + k);

                    }
                    
                    vBullet.erase(vBullet.begin() + i);
                    bulletDirections.erase(bulletDirections.begin() + i);
                    i--;
                    break;
                }
            }
    }

    //enemy Bullet
    for (int i = 0; i < vEnemyBullet.size() ; i++)
    {
        window.draw(vEnemyBullet[i]);
        vEnemyBullet[i].move({0.f,10.f});
        //out
        if(vEnemyBullet[i].getPosition().y <= -10) {vEnemyBullet.erase(vEnemyBullet.begin() + i); break;}
          
            //player collision 
            if(vEnemyBullet[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds()))
            {
                
                player.HP--;//dmg
                
                vEnemyBullet.erase(vEnemyBullet.begin() + i);
                break;
            }
        
    }

    
    //draw
    window.draw(playerShape);
    
    //show
    window.display();

 
    }
    return 0;
}
