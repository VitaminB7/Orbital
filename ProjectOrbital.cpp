#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <ctime>
#include "MainMenu.h"
#include "effect.h"

using namespace std;


class Player
{ 
public: 

    int HP;
    int HPmax;
    int Pdmg;
    int score;
    
    Player(){
        HPmax= 1000;
        HP=HPmax;
        Pdmg = 1;
        score = 0;
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
        shootTimer = -30;
        speed = 0.5;
    }

    Enemy(int a){
        if(a == 1){
            type = 1;
            HPmax= 1;
            HP=HPmax;
            speed = 5;
        }
        else if(a == 2){
            type = 2;
            HPmax= 30;
            HP=HPmax;
            speed = 1;
            shootTimer = -10;
        }
    }

};

class Bullet
{
    public:
    
    char type;
    sf::Sprite bulletSprite;
    sf::Vector2f bulletDirection;
    Bullet(sf::Texture &BulletTex,char C):bulletSprite(BulletTex),type(C){
        switch (type)
        {
        case 'P':
            bulletSprite.setOrigin({bulletSprite.getGlobalBounds().size.x/2-85,bulletSprite.getGlobalBounds().size.y/2});
            bulletSprite.setScale({0.5f, 0.5f}); 
            break;

        case 'U':
            bulletSprite.setScale({1.f, 1.f});
            break;
        
        case 'e':
            bulletSprite.setOrigin({bulletSprite.getGlobalBounds().size.x/2,bulletSprite.getGlobalBounds().size.y/2});
            bulletSprite.setRotation(sf::degrees(180.f));
            bulletSprite.setScale({0.5,0.5});
            break;

        case 'E':
            bulletSprite.setOrigin({bulletSprite.getGlobalBounds().size.x/2-40,bulletSprite.getGlobalBounds().size.y/2+15});
            bulletSprite.setRotation(sf::degrees(180.f));
            break;

        }
    };
};

int main()
{   
    srand(time(0));
    const int framerate = 120;
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({1000,900}),"Orbital", sf::Style::Close , sf::State::Windowed, setting);
    MainMenu mainMenu({800,600});
    window.setFramerateLimit(framerate);

    
    //texture
    sf::Texture playerTexture("SpaceShooterShipConstructor/PNG/Example/01.png");
    sf::Texture enemyTexture("SpaceShooterShipConstructor/PNG/Example/05.png");
    sf::Texture enemyElite01("SpaceShooterShipConstructor/PNG/Example/07.png");
    sf::Texture enemyElite02("SpaceShooterShipConstructor/PNG/Example/10.png");
    sf::Texture followEnemyTexture("SpaceShooterShipConstructor/PNG/Example/14.png");
    sf::Texture playerBulletTexture("SpaceShooterShipConstructor/PNG/Bullets/01.png");
    sf::Texture playerUltimateBulletTexture("SpaceShooterShipConstructor/PNG/Bullets/12.png");
    sf::Texture enemyBulletTexture("SpaceShooterShipConstructor/PNG/Bullets/02.png");
    sf::Texture elite01bulletTexture("SpaceShooterShipConstructor/PNG/Bullets/05.png");
    sf::Texture missileTexture("SpaceShooterShipConstructor/PNG/Addmore/missile.png");
    sf::Texture upgradeWeapon("SpaceShooterShipConstructor/PNG/Addmore/upgrade_icon.png");
    sf::Texture Heal("SpaceShooterShipConstructor/PNG/Addmore/heal.png");
    sf::Texture ShieldIcon("SpaceShooterShipConstructor/PNG/Addmore/shield_icon.png");
    sf::Texture ShieldActive("SpaceShooterShipConstructor/PNG/Addmore/shield_active.png");
    
    sf::Texture effectTexture("Effect/effect.png");
    sf::IntRect frame({0,0},{48,48});
    sf::Sprite showEffect(effectTexture,frame);

    // sound
    sf::Music theme1("Sound/Theme.mp3");
    theme1.setVolume(25);
    theme1.setLooping(true);
    theme1.play();

    sf::SoundBuffer mExplode("Sound/mExplode.mp3");
    sf::Sound effSound1(mExplode);
    effSound1.setVolume(60);

    sf::SoundBuffer bBullet("Sound/bullet.wav");
    sf::Sound bulletSound(bBullet);
 
    Effect effect;
    std::vector<Effect> vEffect;

//******************************************************************UI****************************************************************/
    //Font Text
    sf::Font font("Fonts/TypeLightSans-KV84p.otf");
    sf::Text scoreText(font);
    sf::Text gameOverText(font);
    sf::Text restartText(font);
    sf::Text yourScoreText(font);
    sf::Text resumeText(font);
    sf::Text obritalText(font);
    //Obrital
    obritalText.setString("Obrital");
    obritalText.setFillColor(sf::Color::Red);
    obritalText.setCharacterSize(100);
    obritalText.setPosition(sf::Vector2f(70, 170));
    //Pause Game
    sf::Texture button("Prop/button UI.png");
    sf::IntRect pause1(sf::Vector2i(16*11, 16*9), sf::Vector2i(16, 16));
    sf::Sprite Pause(button);
    Pause.setTextureRect(pause1);
    Pause.setPosition({721,15});
    Pause.setScale({4,4});
    sf::IntRect restart1(sf::Vector2i(16*9, 16*9), sf::Vector2i(16, 16));
    sf::Sprite restart(button);
    restart.setTextureRect(restart1);
    restart.setPosition({380,355});
    restart.setScale({4,4});
    sf::IntRect resume1(sf::Vector2i(16*7, 16*9), sf::Vector2i(16, 16));
    sf::Sprite resume(button);
    resume.setTextureRect(resume1);
    resume.setPosition({280,355});
    resume.setScale({4,4});
    sf::IntRect exit1(sf::Vector2i(16*10, 16*10), sf::Vector2i(16, 16));
    sf::Sprite exit(button);
    exit.setTextureRect(exit1);
    exit.setPosition({480,355});
    exit.setScale({4,4});
    //Text GameOver
    std::stringstream got,rt;
    got << " Game Over ";
    gameOverText.setString(got.str());
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setCharacterSize(100);
    gameOverText.setPosition({400,300});
    gameOverText.setOrigin({gameOverText.getGlobalBounds().size.x / 2, gameOverText.getGlobalBounds().size.y / 2});
    sf::RectangleShape gameOverBg({800,800});
    gameOverBg.setFillColor(sf::Color(0, 0, 0, 130));
    //Blink String
    sf::Clock clock;
    float blinkSpeed = 3.0f;
    bool blink = true;
    //Text Restart
    rt << "Press R to restart";
    restartText.setString(rt.str());
    restartText.setFillColor(sf::Color::White);
    restartText.setCharacterSize(40);
    restartText.setPosition({400,480});
    restartText.setOrigin({restartText.getGlobalBounds().size.x / 2, restartText.getGlobalBounds().size.y / 2});
    //Close Game
    sf::Texture homeGame("Prop/home.png");
    sf::Sprite home(homeGame);
    home.setPosition({721,15});
    home.setScale({4,4});

//******************************************************************************************************************************************* */
    
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
    Enemy elite01(2);

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

    sf::Sprite elite01shape(enemyElite01);
    elite01shape.setOrigin({enemyShape.getGlobalBounds().size.x/2,enemyShape.getGlobalBounds().size.y/2});
    elite01shape.setRotation(sf::degrees(180.f));
    elite01shape.scale({0.8f,0.8f});
    sf::Sprite elite01bulletshape(elite01bulletTexture);

     std::vector<sf::Vector2f> enemybulletDirections;
    std::vector<sf::Sprite> enemybulletEtlite;

    //Bullet
    Bullet playerBullet(playerBulletTexture,'P');
    Bullet ultimateBullet(playerUltimateBulletTexture,'U');
    Bullet enemyBullet(enemyBulletTexture,'e');
    Bullet elite01bullet(elite01bulletTexture,'E');

    sf::Sprite ultimatebullet(playerUltimateBulletTexture);

    std::vector<Bullet> vBullet;
    std::vector<sf::Vector2f> bulletDirections;


    //Split 
    bool pause = false;
    bool play = false;
    bool wasClickedPause = false;
    bool wasClickedPlay = false;
    bool ismainMenu = true;
    bool restartGame = false;
    //general factor
    int shootTimer = 100;
    int RandUpgrade = 0;
    int powerup = 0;
    int powerupCD = 0;
    //enemy factor
    int wave = 1;
    int enemySpawnTimer=2000;
    int enemiesToSpawn = 7; 
    int enemiesSpawned = 0;
    int wavePattern = 1;
    int currentPatterns = 1;
    //player factor
    int playerHitCD = 0;
    bool isBlinking = false;
    int blinkTimer = 0;
    int blinkDuration = framerate;
    bool ultimate = false;
    bool getfirst = false;
    int ultimateTime = 0;
    bool shield = false;
    int shieldtimer = 0;
    int checkpow = 0;

    void SpawnEnemyPattern(int pattern, int numEnemies, sf::RenderWindow &window, 
        vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
        sf::Sprite enemyShape, Enemy e0);
    void SpawnEnemyfollow(int numEnemies, sf::RenderWindow &window, 
        vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
        sf::Sprite followEnemyShape, Enemy e1);
    void SpawnEnemyElite(int numEnemies, sf::RenderWindow &window, 
            vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
            sf::Sprite elite01shape,Enemy elite01);
    void SpawnEliteBullets(sf::Vector2f  enemyPos,Bullet elite01bullet,std::vector<Bullet> &vBullet);

 //***************************************************************************************** 
    while (window.isOpen())
        {
            while(std::optional event = window.pollEvent())
            {
                //close
                if(event->is<sf::Event::Closed>()) window.close();
            }
            if (ismainMenu) {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && clock.getElapsedTime().asSeconds() > 0.2f)
            {
                mainMenu.moveUp();
                clock.restart();
            }
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && clock.getElapsedTime().asSeconds() > 0.2f)
            {
                mainMenu.moveDown();
                clock.restart();
            }
                   
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                int x = mainMenu.mainMenuPressed();
                if (x == 0) 
                {
                    std::cout << "Play"; play = true; ismainMenu = false;  
                }   
                if (x == 1) 
                    std::cout << "Option";
                if (x == 2) 
                {
                    std::cout << "Exit"; window.close();
                }      
            } 
                //window.clear();
                mainMenu.draw(window);
                window.draw(obritalText);
                //window.display();
            }
            //Blink String
            if (blink) 
            {
                float time = clock.getElapsedTime().asSeconds(); //นับเวลา(เวลาเพิ่มขึ้นเรื่อยๆ)
                int alpha = static_cast<int>((std::sin(time * blinkSpeed) * 0.5f + 0.5f) * 255); //กะพริบแบบสมูทๆ
                restartText.setFillColor(sf::Color(255,255,255, alpha));
            }

        //Show Score & Life
        std::stringstream lifePoint, yst;
        lifePoint << "Score " << player.score << " HP " << player.HP;
        scoreText.setString(lifePoint.str());
        scoreText.setCharacterSize(50);
        scoreText.setPosition({15,3});
        //Show Your Score
        yst << "Score " << player.score;
        yourScoreText.setString(yst.str());
        yourScoreText.setFillColor(sf::Color::Blue);
        yourScoreText.setCharacterSize(60);
        yourScoreText.setPosition({400,400});
        yourScoreText.setOrigin({yourScoreText.getGlobalBounds().size.x / 2, yourScoreText.getGlobalBounds().size.y / 2});

        //update
if (play) {
    if (!pause) {
        if (player.HP > 0 ) {
        
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
    bulletSound.play();
    if(!ultimate) {
        vBullet.push_back(playerBullet);
    }
    else {
        vBullet.push_back(ultimateBullet); 
    }
    vBullet.back().bulletSprite.setPosition(playerShape.getPosition());
    vBullet.back().bulletDirection = {0.f, -10.f};
    if(powerup >= 1){
        if(!ultimate) {
            vBullet.push_back(playerBullet);
        }
        else {
            vBullet.push_back(ultimateBullet);
        }
        vBullet.back().bulletSprite.setPosition(playerShape.getPosition());
        vBullet.back().bulletDirection = {-3.f, -10.f};

        if(!ultimate) {
            vBullet.push_back(playerBullet);
        }
        else {
            vBullet.push_back(ultimateBullet);
        }
        vBullet.back().bulletSprite.setPosition(playerShape.getPosition());
        vBullet.back().bulletDirection = {3.f, -10.f};
    }
    if(powerup >= 2){
        if(!ultimate) {
            vBullet.push_back(playerBullet);
        }
        else {
            vBullet.push_back(ultimateBullet);
        }
        vBullet.back().bulletSprite.setPosition(playerShape.getPosition());
        vBullet.back().bulletDirection = {-6.f, -10.f};

        if(!ultimate) {
            vBullet.push_back(playerBullet);
        }
        else {
            vBullet.push_back(ultimateBullet);
        }
        vBullet.back().bulletSprite.setPosition(playerShape.getPosition());
        vBullet.back().bulletDirection = {6.f, -10.f};

    }
    shootTimer = 0;
}


        
        //clear
        window.clear(sf::Color(124,124,124));
        
        //draw
        window.draw(Pause); 
        window.draw(scoreText);
        window.draw(playerShape);

        //EnemySpawn 
        if (enemySpawnTimer < framerate *2) enemySpawnTimer++;
        if (enemySpawnTimer >= framerate *2 && enemiesSpawned < enemiesToSpawn* currentPatterns) 
    {
        enemySpawnTimer = 0;
        int Rand;
        if(wave < 7) Rand = rand()%2;
        else Rand = rand()%3;
        int pattern = rand()%3+1;
        int spawnCount = 0;
        if(Rand == 1){
            SpawnEnemyPattern(pattern, 10, window, vEnemyTexture, vEnemyHP, enemyShape, e0);
            spawnCount += 10;
        }
        if(Rand == 0){ 
            SpawnEnemyfollow(10, window, vEnemyTexture, vEnemyHP, followEnemyShape, e1);
            spawnCount += 10;
        }
        if(Rand == 2){
            SpawnEnemyElite(4, window, vEnemyTexture,vEnemyHP,elite01shape,elite01);
            spawnCount += 4;
        }
        enemiesSpawned += spawnCount;
    }
            //wave operator
        if (vEnemyTexture.size() == 0 && enemiesSpawned >= enemiesToSpawn* currentPatterns) {
            wave++;
            currentPatterns++;
            enemiesSpawned = 0;
            e0.speed += 0.25;
            e0.HPmax+=1;
            e1.speed += 0.5;
            e1.HPmax+=3;
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
            if(vEnemyHP[i].type == 2)vEnemyTexture[i].move({0.f,0.5f*vEnemyHP[i].speed});

            //enemy shoot
            
            if(vEnemyHP[i].type == 0 && vEnemyHP[i].shootTimer < 50) vEnemyHP[i].shootTimer++;
            if(vEnemyHP[i].type == 0 && vEnemyHP[i].shootTimer >= 50) 
        {
            vBullet.push_back(enemyBullet);
            vBullet.back().bulletDirection = {0.f,10.f};
            vBullet.back().bulletSprite.setPosition(vEnemyTexture[i].getPosition());
            vEnemyHP[i].shootTimer = 0;
        }
            if(vEnemyHP[i].type == 2&& vEnemyHP[i].shootTimer < 100) vEnemyHP[i].shootTimer++;
            if(vEnemyHP[i].type == 2 && vEnemyHP[i].shootTimer >= 100){
                SpawnEliteBullets(vEnemyTexture[i].getPosition(),elite01bullet,vBullet);
                vEnemyHP[i].shootTimer = 0;
            }
            
            //collision to player
            if(vEnemyTexture[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) and playerHitCD <= 0) {
                if(shield){
                    vEffect.push_back(effect);
                    vEffect.back().pos={vEnemyTexture[i].getPosition()};
                    vEnemyTexture.erase(vEnemyTexture.begin()+i); 
                    vEnemyHP.erase(vEnemyHP.begin()+i); 
                    i--;
                    break;
                } 
                else{
                    vEffect.push_back(effect);
                    vEffect.back().pos={vEnemyTexture[i].getPosition()}; 
                    vEnemyTexture.erase(vEnemyTexture.begin()+i); 
                    vEnemyHP.erase(vEnemyHP.begin()+i);
                    player.HP--;
                    playerHitCD = 120;
                    isBlinking = true; // Blinking
                    blinkTimer = 0; 
                    i--;
                    break;
                }   
            }
            if(vEnemyTexture[i].getPosition().y>window.getSize().y){
                vEnemyTexture.erase(vEnemyTexture.begin()+i); 
                vEnemyHP.erase(vEnemyHP.begin()+i); 
                i--;
                break;
            } //out(bottom)

        }

        
            //Bullet
        for (int i = vBullet.size() - 1; i >= 0; i--) 
        {
            window.draw(vBullet[i].bulletSprite);
            vBullet[i].bulletSprite.move(vBullet[i].bulletDirection);
            //out
            if(vBullet[i].bulletSprite.getPosition().y <= -10) {
                vBullet.erase(vBullet.begin() + i);
                break;
            }
              
                //enemy collision when got bullet
            
            for (int k = vEnemyTexture.size() - 1; k >= 0; k--) 
            {
                if((vBullet[i].type == 'P' || vBullet[i].type == 'U' )&& vBullet[i].bulletSprite.getGlobalBounds().findIntersection(vEnemyTexture[k].getGlobalBounds()))
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
                        vEffect.push_back(effect);
                        vEffect.back().pos={vEnemyTexture[k].getPosition()};
                        vEnemyTexture.erase(vEnemyTexture.begin() + k);
                        vEnemyHP.erase(vEnemyHP.begin() + k);
                        player.score++;
                    }
        
                    vBullet.erase(vBullet.begin() + i);
                    break;
                }
            }

                //player collision 
                if(vBullet[i].type != 'P' && vBullet[i].bulletSprite.getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) and playerHitCD <= 0)
                {
                    if(shield){
                        vBullet.erase(vBullet.begin() + i);
                        i--;
                        break;
                    }
                    else{
                        player.HP--;//dmg
                        playerHitCD = 120;
                        isBlinking = true; // Blinking
                        blinkTimer = 0; 
                        vBullet.erase(vBullet.begin() + i);
                        i--;
                        break;
                    }
                }    

                //out    
                if(vBullet[i].bulletSprite.getPosition().y <= -10 || vBullet[i].bulletSprite.getPosition().y < 0 || vBullet[i].bulletSprite.getPosition().x < 0 || vBullet[i].bulletSprite.getPosition().x > 1000) {vBullet.erase(vBullet.begin() + i); i--;break;}

            
        }

        //powerup
        for(int i = 0;i < vUpWeapon.size();i++){
            window.draw(vUpWeapon[i]);
            vUpWeapon[i].move({0.f,2.f});
            if (vUpWeapon[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) ) {
                if (vType[i] == 1) {
                    powerup += 1;
                    if(checkpow >= 1) getfirst = true;
                    vUpWeapon.erase(vUpWeapon.begin() + i);
                    vType.erase(vType.begin() + i);
                    checkpow++;
                    i--;
                    break;
                } 
                else if (vType[i] == 2) {
                    player.HP++;
                    if (player.HP > player.HPmax) player.HP = player.HPmax;
                    vUpWeapon.erase(vUpWeapon.begin() + i);
                    vType.erase(vType.begin() + i);
                    i--;
                    break;
                } 
                else if (vType[i] == 3) {
                    shield = true;
                    shieldtimer = 120*2;
                    vUpWeapon.erase(vUpWeapon.begin() + i);
                    vType.erase(vType.begin() + i);
                    i--;
                    break;
                }

            
        }
        if (vUpWeapon[i].getPosition().y >= window.getSize().y)
            {
                vUpWeapon.erase(vUpWeapon.begin() + i);
                vType.erase(vType.begin() + i); 
                i--;
                break;
            }
    }
    //powerup
    if(powerupCD >= 0) powerupCD--;

    if(powerup >= 3 and !ultimate) {
        ultimate = true;
        ultimateTime = 120*10;
    }
    if(ultimateTime > 0) ultimateTime--;
    if(ultimateTime <= 0) {
        ultimate = false;
        if(getfirst) powerup = 2;
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

                } //Play
            } //Pause
        } //Player

        cout<<"wave : "<< wave <<'\n';
        // if (player.HP<=0){cout<<"Player die"; }
        cout << "time = " << shieldtimer << endl;

        //Pause
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
            if (Pause.getGlobalBounds().contains(mousePos) && !wasClickedPause){
                pause = !pause;  
                wasClickedPause = !wasClickedPause;
                
            }    
            if (pause) {
                window.clear(sf::Color(124,124,124));
                window.draw(resume);
                window.draw(restart);
                window.draw(exit);
                if (resume.getGlobalBounds().contains(mousePos)) pause = !pause;  
                if (restart.getGlobalBounds().contains(mousePos)) 
                {
                    cout << "restart was clicked";
                    restartGame = true;
                    pause = false;
                    vEnemyTexture.clear(); vEnemyHP.clear(); vBullet.clear();  
                    bulletDirections.clear();
                    ultimateTime = 0;
                    ultimate = false;
                    getfirst = false;
                    enemiesToSpawn = 0; 
                    enemiesSpawned = 0;
                    checkpow = 0;
                    wave = 0;
                    powerup = 0;
                    playerShape.setPosition({370,700});
                    player.score = 0;
                    player.HP = player.HPmax;
                }  
                if (exit.getGlobalBounds().contains(mousePos)) window.close();
            }
        } else {
            wasClickedPause = false; 
        }
            
        if (player.HP <= 0 || restartGame) //Game Over
        {   
            if (player.HP <= 0) {
            window.clear(sf::Color(124,124,124));
            window.draw(gameOverBg);
            window.draw(gameOverText);
            window.draw(restartText);
            window.draw(yourScoreText);
            window.draw(home);
            }
            restartGame = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) //Restart Game
            {
                bulletDirections.clear();
                vEnemyTexture.clear(); vEnemyHP.clear(); vBullet.clear(); 
                ultimateTime = 0;
                ultimate = false;
                getfirst = false;
                enemiesToSpawn = 0; 
                enemiesSpawned = 0;
                wave = 0;
                powerup = 0;
                playerShape.setPosition({370,700});
                player.score = 0;
                player.HP = player.HPmax;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) //Close Game
            {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window)); 
                if (home.getGlobalBounds().contains(mousePos)) ismainMenu = true;
            }
        } 

        for (int i = 0; i < vEffect.size(); i++)
            {
                vEffect[i].effectChange(showEffect,effSound1,vEffect.back().pos,window,pause);
                if(vEffect[i].state>=8) {vEffect.erase(vEffect.begin()+i); i--; break;}
            }

        window.display();
       
        }

    return 0;//code แม่งโคตร KEYES

}
void SpawnEnemyPattern(int pattern, int numEnemies, sf::RenderWindow &window, 
    vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
    sf::Sprite enemyShape, Enemy e0){
    float startX = window.getSize().x / 2 + 30; 
    float startY = 0; 
    float spacing = 100; 

    for (int i = 0; i < numEnemies; i++) {
        vEnemyTexture.push_back(enemyShape);
        vEnemyHP.push_back(e0);
        if (pattern == 1) { // **Line Pattern**
            vEnemyTexture.back().setPosition({startX - (numEnemies * spacing / 2) + i * spacing, startY});
        } else if (pattern == 2) { // **V-Shape Pattern**
            float offsetX = (i - numEnemies / 2) * spacing; 
            float offsetY = abs(i - numEnemies / 2) * 20; 
            vEnemyTexture.back().setPosition({startX + offsetX, startY + offsetY});
        } else if (pattern == 3) { // **Circle Pattern**
            float angle = i * (360.0 / numEnemies) * (M_PI / 180.0); 
            float radius = 250; 
            vEnemyTexture.back().setPosition({startX + cos(angle) * radius, -200 + sin(angle) * radius});
        }
    }
}
void SpawnEnemyfollow(int numEnemies, sf::RenderWindow &window, 
        vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
        sf::Sprite followEnemyShape, Enemy e1)
        {
            for(int j =0;j<10;j++){
                float startX = rand()%window.getSize().x;
                float startY = rand()%10; 
                vEnemyTexture.push_back(followEnemyShape);
                vEnemyHP.push_back(e1);
                vEnemyTexture.back().setPosition({startX,startY});
            }
        }
void SpawnEnemyElite(int numEnemies, sf::RenderWindow &window, 
            vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
            sf::Sprite elite01shape,Enemy elite01)
            {
                for (int i = 0; i < numEnemies; i++) {
                float startX = window.getSize().x/2+100;
                float startY = 150; 
                float spacing = 150;
                vEnemyTexture.push_back(elite01shape);
                vEnemyHP.push_back(elite01);
                vEnemyTexture.back().setPosition({startX - (numEnemies * spacing / 2) + i * spacing, startY});
                }
            }
void SpawnEliteBullets(sf::Vector2f enemyPos,Bullet elite01bullet,std::vector<Bullet> &vBullet)
{
    const int numBullets = 5; // จำนวนกระสุน
    const float spreadAngle = 180.f; // กระจายครึ่งวงกลม (180 องศา)
    float startAngle = 0; // เริ่มยิงจาก -180 องศาไป 0 องศา

    for (int j = 0; j < numBullets; j++)
    {
        float angle = startAngle + (j * spreadAngle / (numBullets - 1)); // คำนวณมุมของกระสุนแต่ละอัน
        float radian = angle * (M_PI / 180.f); // แปลงเป็นเรเดียน
        
        vBullet.push_back(elite01bullet);
        vBullet.back().bulletSprite.setPosition(enemyPos); // ตั้งกระสุนให้ออกจากตำแหน่งศัตรูa

        sf::Vector2f direction(cos(radian), sin(radian));
        vBullet.back().bulletDirection = {direction};
    }
}

