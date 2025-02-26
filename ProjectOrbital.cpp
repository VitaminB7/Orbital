#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
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
        HPmax= 10;
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
    int limithp;
    int type;
    int shootTimer;
    float speed;
    float speedmax;
    bool isHit;
    int hitTimer;


     Enemy(){
        type = 0;
        limithp = 6;
        HPmax=3;
        HP=HPmax;
        shootTimer = -30;
        speed = 0.5;
        speedmax = 5;
        isHit = false;
        hitTimer = 0;
    }

    Enemy(int a){
        if(a == 1){//follow Enemy
            type = 1;
            limithp = 2;
            HPmax= 1;
            HP=HPmax;
            speed = 2;
            speedmax = 5;
        }
        else if(a == 2){//Elite 1
            type = 2;
            limithp = 30;
            HPmax= 20;
            HP=HPmax;
            speed = 1;
            shootTimer = -30;
        }
        else if(a == 3){//missile
            type = 3;
            HPmax= 1;
            limithp = 2;
            HP=HPmax;
            speed = 3;
            speedmax = 6;
        }
        else if(a == 4){//Elite 2
            type = 4;
            HPmax= 30;
            limithp = 40;
            HP=HPmax;
            speed = 1;
            shootTimer = 0;
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

//ability
void clearEnemyBullets(std::vector<Bullet>& vBullet) {
    for (int i = vBullet.size() - 1; i >= 0; i--) {
        if (vBullet[i].type == 'e' || vBullet[i].type == 'E') {
            vBullet.erase(vBullet.begin() + i);
        }
    }
}

void nuke(std::vector<sf::Sprite>& vEnemyTexture, std::vector<Enemy>& vEnemyHP,
    std::vector<sf::Sprite>& enemybulletEtlite, std::vector<sf::Vector2f>& enemybulletDirections,
    std::vector<Bullet>& vBullet) {

    vEnemyTexture.clear();
    vEnemyHP.clear();
    enemybulletEtlite.clear();
    enemybulletDirections.clear();
    clearEnemyBullets(vBullet);
}

bool nukeActive = false;
int nukeTimer = 0;

//instakill//
bool instakill = false;
int instakillTimer = 0;

//ability cooldown//
int nukeCooldown = 0;
int instakillCooldown = 0;
int resetCooldown = 0;

//ability duration bar
sf::RectangleShape instakillDurationBar;
sf::RectangleShape instakillDurationBarBorder;

int main()
{   
    srand(time(0));
    const int framerate = 120;
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({1000,900}),"Orbital", sf::Style::Close , sf::State::Windowed, setting);
    MainMenu mainMenu({800,600});
    float x = 350;
    ButtonSlide ButtonSlide1({x, 400, 400});
    ButtonSlide ButtonSlide2({x, 500, 400});
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
    sf::Texture background("Scene/Space Background (2).png");
    sf::Texture bgMainmenu("Scene/Space Background (1).png");
    sf::Texture nukeIcon("SpaceShooterShipConstructor/PNG/Addmore/nukeicon.png");
    sf::Texture instakillIcon("SpaceShooterShipConstructor/PNG/Addmore/instakill.png");
    sf::Texture resetCooldownIcon("SpaceShooterShipConstructor/PNG/Addmore/resetcooldown.png");
    sf::Texture nukeExplosion("SpaceShooterShipConstructor/PNG/Addmore/explosion.png");

    sf::Texture effectTexture("Effect/effect.png");
    sf::Texture nukeIcon("SpaceShooterShipConstructor/PNG/Addmore/nukeicon.png");
    sf::Texture instakillIcon("SpaceShooterShipConstructor/PNG/Addmore/instakill.png");
    sf::Texture resetCooldownIcon("SpaceShooterShipConstructor/PNG/Addmore/resetcooldown.png");
    sf::Texture nukeExplosion("SpaceShooterShipConstructor/PNG/Addmore/explosion.png");
    sf::IntRect frame({0,0},{48,48});
    sf::Sprite showEffect(effectTexture,frame);

    // sound
    sf::Music theme1("Sound/Theme.mp3");
    //theme1.setVolume(25);
    theme1.setLooping(true);
    theme1.play();

    sf::SoundBuffer mExplode("Sound/mExplode.mp3");
    sf::Sound effSound1(mExplode);
    //effSound1.setVolume(60);

    sf::SoundBuffer nExplosion("Sound/nExplosion.wav");
    sf::Sound effsound2(nExplosion);

    sf::SoundBuffer instakillSound("Sound/instakill2.wav");
    sf::Sound effsound3(instakillSound);
    //effsound3.setVolume(25);

    sf::SoundBuffer reset("Sound/reset.wav");
    sf::Sound effsound4(reset);
    //effsound4.setVolume(50);

    sf::SoundBuffer nExplosion("Sound/nExplosion.wav");
    sf::Sound effsound2(nExplosion);

    sf::SoundBuffer instakillSound("Sound/instakill2.wav");
    sf::Sound effsound3(instakillSound);
    effsound3.setVolume(30);

    sf::SoundBuffer reset("Sound/reset.wav");
    sf::Sound effsound4(reset);
    effsound4.setVolume(60);

    sf::SoundBuffer bBullet("Sound/bullet.wav");
    sf::Sound bulletSound(bBullet);

    // music sound
    ButtonSlide1.setUpdateFunction([&theme1](float volume) 
    {
        theme1.setVolume(volume);
    });

    // effectSound
    ButtonSlide2.setUpdateFunction([&](float volume) {
        effSound1.setVolume(volume);
        effsound2.setVolume(volume);
        effsound3.setVolume(volume);
        effsound4.setVolume(volume);
        bulletSound.setVolume(volume);
    });
 
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
    sf::Text nukeCooldownText(font);
    sf::Text instakillCooldownText(font);
    sf::Text resetCooldownText(font);
    //Obrital
    sf::Text obritalText(font);
    sf::Text shadowText(font);
    obritalText.setString("Orbital");
    obritalText.setFillColor(sf::Color::Red);
    obritalText.setCharacterSize(170);
    obritalText.setPosition(sf::Vector2f(70, 70));
    shadowText = obritalText;
    shadowText.setFillColor(sf::Color(0, 0, 0, 150)); 
    shadowText.move({10, 10});
    sf::Texture musicText("Prop/Shadow/64/Music-0.png");
    sf::Sprite music(musicText);
    music.setOrigin({64/2,64/2});
    music.setPosition({285, 415});
    sf::Texture soundText("Prop/Shadow/64/Speaker-0.png");
    sf::Sprite sound(soundText);
    sound.setOrigin({64/2,64/2});
    sound.setPosition({285, 515});
    sf::Texture backText("Prop/Shadow/64/Back.png");
    sf::Sprite back(backText);
    back.setScale({1.5,1.4});
    back.setPosition({20,20});
    //Background
    sf::Sprite backgroundGame(background);
    sf::Sprite bgMenu(bgMainmenu);
  
    //ability cooldown
    nukeCooldownText.setCharacterSize(25);
    nukeCooldownText.setFillColor(sf::Color::Black);
    nukeCooldownText.setPosition(sf::Vector2f(50, 60));

    instakillCooldownText.setCharacterSize(25);
    instakillCooldownText.setFillColor(sf::Color::Red);
    instakillCooldownText.setPosition(sf::Vector2f(50, 90));

    resetCooldownText.setCharacterSize(25);
    resetCooldownText.setFillColor(sf::Color::White);
    resetCooldownText.setPosition(sf::Vector2f(50, 120));
    ////duration bars
    instakillDurationBar.setSize(sf::Vector2f(200, 15));
    instakillDurationBar.setFillColor(sf::Color::Yellow);
    instakillDurationBar.setPosition(sf::Vector2f(20, 170));
    instakillDurationBarBorder.setSize(sf::Vector2f(200, 15));
    instakillDurationBarBorder.setFillColor(sf::Color::Transparent);
    instakillDurationBarBorder.setOutlineThickness(2);
    instakillDurationBarBorder.setOutlineColor(sf::Color::Black);
    instakillDurationBarBorder.setPosition({20, 170});
    //Pause Game
    float centerX = 1000/2 - 96/2;
    float centerY = 900/2 - 96/2;
    sf::Texture pauseTex("Prop/Shadow/16/Pause.png");
    sf::Sprite Pause(pauseTex);
    Pause.setScale({6,6});
    Pause.setPosition({1000 - 111,15});
    sf::Texture resumeTexture("Prop/Shadow/16/Play.png");
    sf::Sprite resume(resumeTexture);
    resume.setPosition({centerX - 96 - 10, centerY - 96 - 10});
    resume.setScale({6,6});
    sf::Texture restartTexture("Prop/Shadow/16/Redo.png");
    sf::Sprite restart(restartTexture);
    restart.setPosition({centerX + 96 + 10, centerY - 96 - 10});
    restart.setScale({6,6});
    sf::Texture exitTexture("Prop/Shadow/16/Exit.png");
    sf::Sprite exit(exitTexture);
    exit.setPosition({centerX - 96 - 10, centerY + 96 + 10});
    exit.setScale({6,6});
    sf::Texture homeText("Prop/Shadow/16/Home.png");
    sf::Sprite home(homeText);
    home.setPosition({centerX + 96 + 10, centerY + 96 + 10});
    home.setScale({6,6});
    //Text GameOver
    std::stringstream got,rt;
    got << " Game Over ";
    gameOverText.setString(got.str());
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setCharacterSize(100);
    gameOverText.setPosition({500,300});
    gameOverText.setOrigin({gameOverText.getGlobalBounds().size.x / 2, gameOverText.getGlobalBounds().size.y / 2});
    sf::RectangleShape gameOverBg({1000,900});
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
    restartText.setPosition({500,480});
    restartText.setOrigin({restartText.getGlobalBounds().size.x / 2, restartText.getGlobalBounds().size.y / 2});

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
    Enemy elite02(4);
    Enemy missile(3);

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

    sf::Sprite elite02shape(enemyElite02);
    elite02shape.setOrigin({enemyShape.getGlobalBounds().size.x/2,enemyShape.getGlobalBounds().size.y/2});
    elite02shape.setRotation(sf::degrees(180.f));
    elite02shape.scale({0.8f,0.8f});
    sf::Sprite missileShape(missileTexture);
    missileShape.setOrigin({elite01bulletshape.getGlobalBounds().size.x/2-40,elite01bulletshape.getGlobalBounds().size.y/2+15});
    missileShape.setRotation(sf::degrees(270.f));
    missileShape.scale({0.2f,0.2f});

    //Bullet
    Bullet playerBullet(playerBulletTexture,'P');
    Bullet ultimateBullet(playerUltimateBulletTexture,'U');
    Bullet enemyBullet(enemyBulletTexture,'e');
    Bullet elite01bullet(elite01bulletTexture,'E');

     
    //player bullet
    sf::Sprite ultimatebullet(playerUltimateBulletTexture);
    std::vector<Bullet> vBullet;
    std::vector<sf::Vector2f> bulletDirections;
    
    //ability icon
    sf::Sprite nukeIconSprite(nukeIcon);
    nukeIconSprite.setPosition(sf::Vector2f(12.5, 60));
    nukeIconSprite.setScale({0.15f,0.15f});

    sf::Sprite instakillIconSprite(instakillIcon);
    instakillIconSprite.setPosition(sf::Vector2f(-7, 70));
    instakillIconSprite.setScale({0.2f,0.2f});
    
    sf::Sprite resetCooldownIconSprite(resetCooldownIcon);
    resetCooldownIconSprite.setPosition(sf::Vector2f(15, 122));
    resetCooldownIconSprite.setScale({0.08f,0.08f});

    sf::Sprite nukeEffectSprite(nukeExplosion);
    nukeEffectSprite.setPosition({-50,-300});
    nukeEffectSprite.setScale({3,3});
    nukeEffectSprite.setColor(sf::Color(255, 255, 255, 150));
    
    //Split 
    bool pause = false;
    bool play = false;
    bool wasClickedPause = false;
    bool wasClickedPlay = false;
    bool ismainMenu = true;
    bool restartGame = false;
    bool homeGame = false;
    bool resetGame = false;
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
    void SpawnEnemyEliteMissile(int numEnemies, sf::RenderWindow &window, 
        vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
        sf::Sprite elite02shape,Enemy elite02);       
    void SpawnMissile( sf::Vector2f  enemyPos, 
        vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
        sf::Sprite missileShape, Enemy e3);
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
                {
                    ismainMenu = false;
                }
                if (x == 2) 
                {
                    std::cout << "Exit"; window.close();
                }      
            } 
                //window.clear();
                window.draw(bgMenu);
                mainMenu.draw(window);
                window.draw(shadowText);
                window.draw(obritalText);
                //window.display();
            }
            if (!ismainMenu && !play) { //หน้าsetting
                window.draw(bgMenu); window.draw(gameOverBg); window.draw(sound); window.draw(music); window.draw(back);
                ButtonSlide1.draw(window);
                ButtonSlide2.draw(window);
                ButtonSlide1.handle(window);
                ButtonSlide2.handle(window);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                    if (back.getGlobalBounds().contains(mousePos)) {
                        ismainMenu = true;
                    }
                } 
                
            } 

            //resetGame
            if(resetGame){
                restartGame = true;
                pause = false;
                vEnemyTexture.clear(); vEnemyHP.clear(); vBullet.clear(); bulletDirections.clear();
                ultimateTime = 0;
                e0.HPmax = 3; e0.HP = e0.HPmax;
                e1.HPmax = 1; e1.HP = e1.HPmax;
                elite01.HPmax = 20; elite01.HP = elite01.HPmax;
                elite02.HPmax = 30; elite02.HP = elite01.HPmax;
                missile.HP = 1; missile.HP = missile.HPmax;
                missile.speed = 3; e0.speed = 1; e1.speed = 3;
                ultimate = false;
                getfirst = false;
                enemiesToSpawn = 0; enemiesSpawned = 0; enemySpawnTimer = 0;
                checkpow = 0;
                wave = 0;
                powerup = 0;
                nukeCooldown = 0; nukeActive = false;
                instakillCooldown = 0; instakill = false;
                resetCooldown = 0; 
                playerShape.setPosition({500,850});
                player.score = 0;
                player.HP = player.HPmax;
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
        yourScoreText.setPosition({500,400});
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

        //abilities//
        //nuke//
        if(nukeCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)){
        nuke(vEnemyTexture, vEnemyHP, enemybulletEtlite, enemybulletDirections, vBullet);
        nukeCooldown = 120 * 30;
        nukeActive = true;
        nukeTimer = 120 * 5;
        effsound2.play();
        }
        //instakill//
        if (instakillCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
            instakill = true;
            instakillTimer = 120 * 10;
            instakillCooldown = 120 * 20;
            effsound3.play();
        }
        if (instakillTimer > 0)
        instakillTimer--;
        else instakill = false;
        //reset cooldown//
        if (resetCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
            nukeCooldown = 0;
            instakillCooldown = 0;
            resetCooldown = 120 * 60;
            effsound4.play();
        }

        //nuke timer//
        if (nukeTimer > 0) {
            nukeTimer--;
        } else {
            nukeActive = false;
        }

        //ability cooldown//
        if (nukeCooldown > 0) {
            nukeCooldown--;
        }
        if (instakillCooldown > 0) {
            instakillCooldown--;
        }
        if (resetCooldown > 0) {
            resetCooldown--;
        }

        //Update ability text
        nukeCooldownText.setString("Nuke Cooldown: " + std::to_string(nukeCooldown / 120) + "s");
        instakillCooldownText.setString("Instakill Cooldown: " + std::to_string(instakillCooldown / 120) + "s");
        resetCooldownText.setString("Reset Cooldown: " + std::to_string(resetCooldown / 120) + "s");

        if (instakill) {
            instakillDurationBar.setSize(sf::Vector2f((instakillTimer / float(120 * 10)) * 200, 15));
        } else {
            instakillDurationBar.setSize(sf::Vector2f(0, 20));
        }

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
        //draw
        window.draw(backgroundGame);
        window.draw(Pause); 
        window.draw(scoreText);
        window.draw(playerShape);
        // Draw cooldown text
        window.draw(nukeCooldownText);
        window.draw(instakillCooldownText);
        window.draw(resetCooldownText);
        //draw ability icon
        window.draw(nukeIconSprite);
        window.draw(instakillIconSprite);
        window.draw(resetCooldownIconSprite);
        if(nukeActive)
        window.draw(nukeEffectSprite);
        window.draw(instakillDurationBar);
        window.draw(instakillDurationBarBorder);    

        //EnemySpawn 
        if (enemySpawnTimer < framerate *2) enemySpawnTimer++;
        if (enemySpawnTimer >= framerate *2 && enemiesSpawned <= enemiesToSpawn* currentPatterns) 
    {
        enemySpawnTimer = 0;
        int Rand;
        if(wave < 7) Rand = rand()%2;
        else Rand = rand()%4;
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
        if(Rand == 3){
            SpawnEnemyEliteMissile(3,window, vEnemyTexture,vEnemyHP,elite02shape,elite02);
            spawnCount += 3;
        }
        enemiesSpawned += spawnCount;
    }
            //wave operator

        if (vEnemyTexture.size() == 0 && enemiesSpawned >= enemiesToSpawn* currentPatterns && enemySpawnTimer >= framerate*2) {
            wave++;
            currentPatterns++;
            enemiesSpawned = 0;
            if(e0.speed <= e0.speedmax) e0.speed += 0.25;
            if(e0.HPmax < e0.limithp){
                e0.HPmax += 1;
                e0.HP = e0.HPmax;
            }
           if(e1.speed <= e1.speedmax) e1.speed += 0.5;
           if(e1.HPmax < e1.limithp){
            e1.HPmax += 1;
            e1.HP = e1.HPmax;
           }
            if(missile.speed <= missile.speedmax) missile.speed += 0.25;
            if(missile.HPmax <= missile.limithp){
                missile.HPmax += 1;
                missile.HP = missile.HPmax;
            }
           if(elite01.HPmax < elite01.limithp && wave >7){
            elite01.HPmax += 1;
            elite01.HP = elite01.HPmax;
        }
        if(elite02.HPmax < elite02.limithp && wave >7){
            elite02.HPmax += 1;
            elite02.HP = elite02.HPmax;
        }

        }
        
            //Enemy
        for (int i = 0; i < vEnemyTexture.size() ; i++)
        {
           
            //follow enemy
            if(vEnemyHP[i].type == 1 or vEnemyHP[i].type == 3){followEnemyPOS = (vEnemyTexture[i].getPosition());}
            playerCenter = {playerShape.getPosition().x + playerShape.getGlobalBounds().size.x/2 , playerShape.getPosition().y + playerShape.getGlobalBounds().size.y/2};
            enemyAimDir = (playerCenter-followEnemyPOS);
            enemyAimDirNorm =  enemyAimDir / (float)sqrt(pow(enemyAimDir.x,2)+pow(enemyAimDir.y,2));
            
            //draw+move
            window.draw(vEnemyTexture[i]);//draw
            if(vEnemyHP[i].type == 0)vEnemyTexture[i].move({0.f,2.f*vEnemyHP[i].speed});//move
            if(vEnemyHP[i].type == 1)vEnemyTexture[i].move(enemyAimDirNorm*vEnemyHP[i].speed);//move
            if(vEnemyHP[i].type == 2)vEnemyTexture[i].move({0.f,0.5f*vEnemyHP[i].speed});
            if(vEnemyHP[i].type == 3)vEnemyTexture[i].move(enemyAimDirNorm*vEnemyHP[i].speed);
            if(vEnemyHP[i].type == 4)vEnemyTexture[i].move({0.f,0.5f*vEnemyHP[i].speed});

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
            if(vEnemyHP[i].type == 4 && vEnemyHP[i].shootTimer <= 50) vEnemyHP[i].shootTimer++;
            if(vEnemyHP[i].type == 4 && vEnemyHP[i].shootTimer >= 50){
                SpawnMissile(vEnemyTexture[i].getPosition(), vEnemyTexture,vEnemyHP,missileShape,missile);
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

            // enemy is hit 
            if (vEnemyHP[i].isHit) {
                vEnemyTexture[i].setColor(sf::Color(255, 90, 90));
                vEnemyHP[i].hitTimer--;
                if (vEnemyHP[i].hitTimer <= 0) {
                    vEnemyHP[i].isHit = false;
                    vEnemyTexture[i].setColor(sf::Color::White);
                }
            }

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
                    if (instakill) {//instakill ability//
                        // หาก ability active อยู่ ให้ทำลายศัตรูทันที
                        vEnemyHP[k].HP = 0;
                    } else {
                        // หากไม่ active ให้ลด HP ตามปกติ
                        vEnemyHP[k].HP -= player.Pdmg;
                    }//dmg

                    vEnemyHP[k].isHit = true;
                    vEnemyHP[k].hitTimer = 15;
                    
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
                if(vBullet[i].type != 'P' &&vBullet[i].type != 'U' && vBullet[i].bulletSprite.getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) and playerHitCD <= 0)
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
                window.draw(backgroundGame);
                window.draw(gameOverBg);
                window.draw(resume);
                window.draw(restart);
                window.draw(exit);
                window.draw(home);
                if (resume.getGlobalBounds().contains(mousePos)) pause = !pause;  
                if (restart.getGlobalBounds().contains(mousePos)) resetGame = true;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
            {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window)); 
                if (home.getGlobalBounds().contains(mousePos)) {
                    play = false;
                    resetGame = true;
                    ismainMenu = true;
                }
            }
                if (exit.getGlobalBounds().contains(mousePos)) window.close();
            }
        } else {
            wasClickedPause = false; resetGame = false;
        }
            
        if (player.HP <= 0) //Game Over
        {   
            window.draw(backgroundGame);
            window.draw(gameOverBg);
            window.draw(gameOverText);
            window.draw(restartText);
            window.draw(yourScoreText);
            window.draw(back);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) //Restart Game
            { 
                resetGame = true; 
            } 
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) //Back to mainmenu
            {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window)); 
                if (back.getGlobalBounds().contains(mousePos)) {
                    ismainMenu = true;
                    play = false;
                    resetGame = true;
                }
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

        sf::Vector2f direction({cos(radian), sin(radian)});
        vBullet.back().bulletDirection = direction;
    }
}
void SpawnEnemyEliteMissile(int numEnemies, sf::RenderWindow &window, 
    vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
    sf::Sprite elite02shape,Enemy elite02)
    {
        for (int i = 0; i < numEnemies; i++) {
            float startX = window.getSize().x/2;
            float startY = 150; 
            float spacing = 300;
            float offsetX = (i - numEnemies / 2) * spacing; 
            float offsetY = abs(i - numEnemies / 2) * 20; 
            vEnemyTexture.push_back(elite02shape);
            vEnemyHP.push_back(elite02);
            vEnemyTexture.back().setPosition({startX + offsetX, startY + offsetY});
    }
}
void SpawnMissile( sf::Vector2f  enemyPos, 
    vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
    sf::Sprite missileShape, Enemy missile)
    {
            vEnemyTexture.push_back(missileShape);
            vEnemyHP.push_back(missile);
            vEnemyTexture.back().setPosition({enemyPos});
    }
