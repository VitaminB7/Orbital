#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;


class Player
{ 
public: 

    int HP;
    int HPmax;
    int Pdmg;
    
    Player(){
        HPmax=5;
        HP=HPmax;
        Pdmg = 1;
    }

};

class Enemy
{
public: 
     
    float bulletSpeed = 1; 

    int HP;
    int HPmax;
    int type;
    int shootTimer;
    float speed;


    Enemy(){
        type = 0;
        HPmax=3;
        HP=HPmax;
        shootTimer = 1000;
        speed = 1;
    }

    Enemy(int a){
        type = a;
        HPmax= 1;
        HP=HPmax;
        speed = 5;
    }

};

int main()
{
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({800,800}),"Orbital", sf::State::Windowed, setting);
    window.setFramerateLimit(120);

    
    //texture
    sf::Texture playerTexture("SpaceShooterShipConstructor/PNG/Example/01.png");
    sf::Texture enemyTexture("SpaceShooterShipConstructor/PNG/Example/05.png");
    sf::Texture followEnemyTexture("SpaceShooterShipConstructor/PNG/Example/14.png");
    sf::Texture playerBulletTexture("SpaceShooterShipConstructor/PNG/Bullets/01.png");
    sf::Texture playerUltimateBulletTexture("SpaceShooterShipConstructor/PNG/Bullets/12.png");
    sf::Texture enemyBulletTexture("SpaceShooterShipConstructor/PNG/Bullets/02.png");
    sf::Texture upgradeWeapon("SpaceShooterShipConstructor/PNG/Addmore/upgrade_icon.png");
    sf::Texture Heal("SpaceShooterShipConstructor/PNG/Addmore/heal.png");
    sf::Texture ShieldIcon("SpaceShooterShipConstructor/PNG/Addmore/shield_icon.png");
    sf::Texture ShieldActive("SpaceShooterShipConstructor/PNG/Addmore/shield_active.png");
    
    //player
    Player player;
    sf::Sprite playerShape(playerTexture);
    playerShape.setScale({0.5f,0.5f});
    sf::Vector2f playerCenter;
    sf::Vector2f mouse(sf::Mouse::getPosition());
    playerShape.setPosition({500,500});

    //upgrade
    std::vector<int> vType;
    sf::Sprite upWeapon(upgradeWeapon);
    upWeapon.setScale({0.2f,0.2f});
    std::vector<sf::Sprite> vUpWeapon;
    sf::Sprite shieldActive(ShieldActive);
    shieldActive.setOrigin({shieldActive.getGlobalBounds().size.x/2-85,shieldActive.getGlobalBounds().size.y/2});
    sf::Sprite shieldIcon(ShieldIcon);
    sf::Sprite heal(Heal);

    //enemy
    Enemy e0;
    Enemy e1(1);
    sf::Sprite enemyShape(enemyTexture);
    enemyShape.setOrigin({enemyShape.getGlobalBounds().size.x/2,enemyShape.getGlobalBounds().size.y/2});
    enemyShape.setRotation(sf::degrees(180.f));
    enemyShape.scale({0.6f,0.6f});
    std::vector<sf::Sprite> vEnemyTexture;
    std::vector<Enemy> vEnemyHP;
    
    sf::Sprite followEnemyShape(followEnemyTexture);
    followEnemyShape.setOrigin({followEnemyShape.getGlobalBounds().size.x/2,followEnemyShape.getGlobalBounds().size.y/2});
    followEnemyShape.setRotation(sf::degrees(180.f));
    followEnemyShape.scale({0.4f,0.4f});
    sf::Vector2f followEnemyPOS;
    sf::Vector2f enemyAimDir;
    sf::Vector2f enemyAimDirNorm;



    //player bullet
    sf::Sprite bulletShape(playerBulletTexture);
    sf::Sprite ultimatebullet(playerUltimateBulletTexture);
    bulletShape.setOrigin({bulletShape.getGlobalBounds().size.x/2-85,bulletShape.getGlobalBounds().size.y/2});
    std::vector<sf::Sprite> vBullet;
    std::vector<sf::Vector2f> bulletDirections;
    
    
    //enemy bullet
    sf::Sprite enemyBulletShape(enemyBulletTexture);
    enemyBulletShape.setOrigin({enemyBulletShape.getGlobalBounds().size.x/2,enemyBulletShape.getGlobalBounds().size.y/2});
    enemyBulletShape.setRotation(sf::degrees(180.f));
    std::vector<sf::Sprite> vEnemyBullet;

    //general factor
    int shootTimer = 100;
    int wave = 1;
    int RandUpgrade = 0;
    int powerup = 0;
    int powerupCD = 0;
    //enemy factor
    int enemySpawnTimer=2000;
    int enemiesToSpawn = 5; 
    int enemiesSpawned = 0;
    int enemiesRemaining = 0;
    //player factor
    int playerHitCD = 0;
    bool isBlinking = false;
    int blinkTimer = 0;
    int blinkDuration = 120;
    bool ultimate = false;
    bool getfirst = false;
    int ultimateTime = 0;
    bool shield = false;
    int shieldtimer = 0;

 //***************************************************************************************** 
    while (window.isOpen())
        {
            while(std::optional event = window.pollEvent())
            {
                //close
                if(event->is<sf::Event::Closed>()) window.close();
            }

        //update

        srand(time(0));
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        playerShape.move({0.f,-5.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        playerShape.move({-5.f,0.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        playerShape.move({0.f,5.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        playerShape.move({5.f,0.f});
        //player move
        if(playerShape.getPosition().x <= 0) playerShape.setPosition({0.f,playerShape.getPosition().y});//left
        if(playerShape.getPosition().x + playerShape.getGlobalBounds().size.x  >= window.getSize().x) playerShape.setPosition({window.getSize().x - playerShape.getGlobalBounds().size.x ,playerShape.getPosition().y});//right
        if(playerShape.getPosition().y <= 0) playerShape.setPosition({playerShape.getPosition().x,0.f});//top
        if(playerShape.getPosition().y + playerShape.getGlobalBounds().size.y >= window.getSize().y) playerShape.setPosition({playerShape.getPosition().x,window.getSize().y - playerShape.getGlobalBounds().size.y});//bottom
        //player cd
        if(shootTimer < 20) shootTimer++;
        if(playerHitCD  > 0) playerHitCD--;

        //player shoot

if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shootTimer >=20)
{
    //Player shoot bullet
    if(!ultimate) {
        vBullet.push_back(bulletShape);
        vBullet.back().setScale({0.5f, 0.5f}); 
    }
    else {
        vBullet.push_back(ultimatebullet);
        vBullet.back().setScale({1.f, 1.f}); 
    }
    vBullet.back().setPosition(playerShape.getPosition());
    bulletDirections.push_back({0.f, -10.f});
    if(powerup >= 1){
        if(!ultimate) {
            vBullet.push_back(bulletShape);
            vBullet.back().setScale({0.5f, 0.5f}); 
        }
        else {
            vBullet.push_back(ultimatebullet);
            vBullet.back().setScale({1.f, 1.f}); 
        }
        vBullet.back().setPosition(playerShape.getPosition());
        bulletDirections.push_back({-3.f, -10.f});

        if(!ultimate) {
            vBullet.push_back(bulletShape);
            vBullet.back().setScale({0.5f, 0.5f});
        }
        else {
            vBullet.push_back(ultimatebullet);
            vBullet.back().setScale({1.f, 1.f});
        }
        vBullet.back().setPosition(playerShape.getPosition());
        bulletDirections.push_back({3.f, -10.f});
    }
    if(powerup >= 2){
        vBullet.push_back(ultimatebullet);
        vBullet.back().setScale({1.f, 1.f}); 
        vBullet.back().setPosition(playerShape.getPosition());
        bulletDirections.push_back({-6.f, -10.f});

        vBullet.push_back(ultimatebullet);
        vBullet.back().setScale({1.f, 1.f});
        vBullet.back().setPosition(playerShape.getPosition());
        bulletDirections.push_back({6.f, -10.f});
    }
    shootTimer = 0;
}


        
        //clear
        window.clear(sf::Color(124,124,124));
        //draw
        window.draw(playerShape);

        //EnemySpawn 
        if (enemySpawnTimer < 50) enemySpawnTimer++;
        if (enemySpawnTimer >= 50 && enemiesSpawned < enemiesToSpawn) 
    {
        enemySpawnTimer = 0;
        int Rand = rand() % 2;
        if (Rand == 0) {
        vEnemyTexture.push_back(enemyShape);
        vEnemyHP.push_back(e0);
        }
        if (Rand == 1) {
            vEnemyTexture.push_back(followEnemyShape);
            vEnemyHP.push_back(e1);
        }
        float R = rand() % (int)window.getSize().x;
        if (R > window.getSize().x / 2) vEnemyTexture.back().setPosition({ R - vEnemyTexture.back().getGlobalBounds().size.x, 0.f });
        else vEnemyTexture.back().setPosition({ R + vEnemyTexture.back().getGlobalBounds().size.x, 0.f });
        //wave operator
        enemiesSpawned++;
        enemiesRemaining++;
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

            //collision to player
            if(vEnemyTexture[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) and playerHitCD <= 0) {
                if(shield){
                    vEnemyTexture.erase(vEnemyTexture.begin()+i); 
                    vEnemyHP.erase(vEnemyHP.begin()+i); 
                    enemiesRemaining--;
                    i--;
                } 
                else{
                    vEnemyTexture.erase(vEnemyTexture.begin()+i); 
                    vEnemyHP.erase(vEnemyHP.begin()+i); 
                    player.HP--;
                    playerHitCD = 120;
                    isBlinking = true; // Blinking
                    blinkTimer = 0; 
                    enemiesRemaining--; 
                    break;
                }   
            }
            if(vEnemyTexture[i].getPosition().y>window.getSize().y){
                vEnemyTexture.erase(vEnemyTexture.begin()+i); 
                vEnemyHP.erase(vEnemyHP.begin()+i); 
                enemiesRemaining--;
                break;
            } //out(bottom)

        }
        //wave operator
        if (enemiesRemaining == 0 && enemiesSpawned >= enemiesToSpawn) {
        wave++;
        enemiesToSpawn += 5; 
        enemiesSpawned = 0;
        e0.speed += 0.25;
        e0.HPmax++;
        e1.speed += 0.5;
        e1.HPmax++;
    }
        
            //player Bullet
        for (int i = 0; i < vBullet.size() ; i++)
        {
            window.draw(vBullet[i]);
            vBullet[i].move(bulletDirections[i]);
            //out
            if(vBullet[i].getPosition().y <= -10) {
                vBullet.erase(vBullet.begin() + i);
                bulletDirections.erase(bulletDirections.begin() + i);
                i--; 
                break;
            }
              
                //enemy collision when got bullet
            
            for (int k = 0; k < vEnemyTexture.size(); k++)
            {
                if(vBullet[i].getGlobalBounds().findIntersection(vEnemyTexture[k].getGlobalBounds()))
                {
                    
                    vEnemyHP[k].HP -= player.Pdmg;//dmg
                    
                    if (vEnemyHP[k].HP <= 0) {
                        if(powerupCD <= 0){
                            int dropChance = rand()%2;
                            if(dropChance == 1){
                                int dropItem = rand()%3;
                                if(dropItem == 0){
                                    vUpWeapon.push_back(sf::Sprite(upgradeWeapon));
                                    vUpWeapon.back().setScale({0.2f, 0.2f});
                                    vUpWeapon.back().setPosition(vEnemyTexture[k].getPosition());
                                    vType.push_back(1);
                                    powerupCD = 120*3;
                                }
                                if(dropItem == 1){
                                    vUpWeapon.push_back(sf::Sprite(heal));
                                    vUpWeapon.back().setScale({0.1f, 0.1f});
                                    vUpWeapon.back().setPosition(vEnemyTexture[k].getPosition());
                                    vType.push_back(2);
                                    powerupCD = 120*3;
                                }
                                if(dropItem == 2){
                                    vUpWeapon.push_back(sf::Sprite(shieldIcon));
                                    vUpWeapon.back().setScale({0.2f, 0.2f});
                                    vUpWeapon.back().setPosition(vEnemyTexture[k].getPosition());
                                    vType.push_back(3);
                                    powerupCD = 120*3;
                                }
                            }
                        }
                        vEnemyTexture.erase(vEnemyTexture.begin() + k);
                        vEnemyHP.erase(vEnemyHP.begin() + k);
                        enemiesRemaining--;
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
            if(vEnemyBullet[i].getPosition().y <= -10) {vEnemyBullet.erase(vEnemyBullet.begin() + i); i--;break;}
              
                //player collision 
                if(vEnemyBullet[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) and playerHitCD <= 0)
                {
                    if(shield){
                        vEnemyBullet.erase(vEnemyBullet.begin() + i);
                        i--;
                    }
                    else{
                        player.HP--;//dmg
                        playerHitCD = 120;
                        isBlinking = true; // Blinking
                        blinkTimer = 0; 
                        vEnemyBullet.erase(vEnemyBullet.begin() + i);
                        i--;
                        break;
                    }
                }
            
        }
        //powerup
        for(int i = 0;i < vUpWeapon.size();i++){
            window.draw(vUpWeapon[i]);
            vUpWeapon[i].move({0.f,2.f});
            if (vUpWeapon[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) ) {
                if (vType[i] == 1) {
                    powerup += 1;
                    getfirst = true;
                    vUpWeapon.erase(vUpWeapon.begin() + i);
                    vType.erase(vType.begin() + i);
                    i--;
                } 
                else if (vType[i] == 2) {
                    player.HP++;
                    if (player.HP > player.HPmax) player.HP = player.HPmax;
                    vUpWeapon.erase(vUpWeapon.begin() + i);
                    vType.erase(vType.begin() + i);
                    i--;
                } 
                else if (vType[i] == 3) {
                    shield = true;
                    shieldtimer = 120*2;
                    vUpWeapon.erase(vUpWeapon.begin() + i);
                    vType.erase(vType.begin() + i);
                    i--;
                }

            
        }
        if (vUpWeapon[i].getPosition().y >= window.getSize().y)
            {
                vUpWeapon.erase(vUpWeapon.begin() + i);
                vType.erase(vType.begin() + i); 
                i--;
            }
    }
    //powerup
    if(powerupCD >= 0) powerupCD--;

    if(powerup >= 2 and !ultimate) {
        ultimate = true;
        ultimateTime = 120*3;
    }
    if(ultimateTime > 0) ultimateTime--;
    if(ultimateTime <= 0) {
        ultimate = false;
        if(getfirst) powerup = 1;
        player.Pdmg = 1;
    }
    if(ultimate) player.Pdmg = 3;
    //shield
    if(shield){
        shieldActive.setPosition(playerShape.getPosition());
        shieldActive.setScale({0.5,0.5});
        window.draw(shieldActive);
    }
    if(shieldtimer <= 0){
        shield = false;
    }
    if(shieldtimer >= 0) shieldtimer--;

                // Blinking
                if (isBlinking) {
                    blinkTimer++;
                    if (blinkTimer % 20 == 0) {
                        if (playerShape.getColor().a == 255) { 
                            playerShape.setColor(sf::Color(255, 255, 255, 100)); 
                        } else {
                            playerShape.setColor(sf::Color(255, 255, 255, 255)); 
                        }
                }

                //stopBlinking
                if (blinkTimer >= blinkDuration) {
                    isBlinking = false; 
                    playerShape.setColor(sf::Color(255, 255, 255, 255)); 
                }
            }

        cout<<"Player HP : "<<player.HP<<'\n';
        // if (player.HP<=0){cout<<"Player die"; }
        cout << "time = " << shieldtimer << endl;

        window.display();
       
        }

    return 0;//code แม่งโคตร KEYES

}
