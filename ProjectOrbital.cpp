#include "MainMenu.h"
#include "effect.h"
#include "function.h"
using namespace std;

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
    sf::Texture superEliteTexture("SpaceShooterShipConstructor/PNG/Example/02.png");
    sf::Texture asteroidTexture("Prop/Asteroid.png");
    sf::Texture warningTexture("Prop/warning.png");

    sf::Texture effectTexture("Effect/effect.png");
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

    //asteroid
    asteroid sad(asteroidTexture,warningTexture);
    std::vector<asteroid> vAsteroid;
    sf::Clock cAs;

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
    sf::Texture musicText("Prop/Music-0.png");
    sf::Sprite music(musicText);
    music.setOrigin({64/2,64/2});
    music.setPosition({285, 415});
    sf::Texture soundText("Prop/Speaker-0.png");
    sf::Sprite sound(soundText);
    sound.setOrigin({64/2,64/2});
    sound.setPosition({285, 515});
    sf::Texture backText("Prop/Back.png");
    sf::Sprite back(backText);
    back.setScale({1.5,1.4});
    back.setPosition({20,20});
    //Background
    sf::Sprite backgroundGame(background);
    sf::Sprite bgMenu(bgMainmenu);
    //ability cooldown
    nukeCooldownText.setCharacterSize(25);
    nukeCooldownText.setFillColor(sf::Color::Yellow);
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
    sf::Texture pauseTex("Prop/Pause.png");
    sf::Sprite Pause(pauseTex);
    Pause.setScale({6,6});
    Pause.setPosition({1000 - 111,15});
    sf::Texture resumeTexture("Prop/Play.png");
    sf::Sprite resume(resumeTexture);
    resume.setPosition({centerX - 96 - 10, centerY - 96 - 10});
    resume.setScale({6,6});
    sf::Texture restartTexture("Prop/Redo.png");
    sf::Sprite restart(restartTexture);
    restart.setPosition({centerX + 96 + 10, centerY - 96 - 10});
    restart.setScale({6,6});
    sf::Texture exitTexture("Prop/Exit.png");
    sf::Sprite exit(exitTexture);
    exit.setPosition({centerX - 96 - 10, centerY + 96 + 10});
    exit.setScale({6,6});
    sf::Texture homeText("Prop/Home.png");
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
    //upWeapon.setScale({0.1f,0.1f});
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
    Enemy superelite(5);

    sf::Sprite enemyShape(enemyTexture);
    enemyShape.setOrigin({enemyShape.getGlobalBounds().size.x/2,enemyShape.getGlobalBounds().size.y/2-10});
    enemyShape.setRotation(sf::degrees(180.f));
    enemyShape.scale({0.6f,0.6f});
    std::vector<sf::Sprite> vEnemyTexture;
    std::vector<Enemy> vEnemyHP;
    
    sf::Sprite followEnemyShape(followEnemyTexture);
    followEnemyShape.setOrigin({followEnemyShape.getGlobalBounds().size.x/2-10,followEnemyShape.getGlobalBounds().size.y/2-10});
    followEnemyShape.setRotation(sf::degrees(180.f));
    followEnemyShape.scale({0.4f,0.4f});
    sf::Vector2f followEnemyPOS;
    sf::Vector2f enemyAimDir;
    sf::Vector2f enemyAimDirNorm;

    sf::Sprite elite01shape(enemyElite01);
    elite01shape.setOrigin({elite01shape.getGlobalBounds().size.x/2-30,elite01shape.getGlobalBounds().size.y/2-30});
    elite01shape.setRotation(sf::degrees(180.f));
    elite01shape.scale({0.8f,0.8f});
    sf::Sprite elite01bulletshape(elite01bulletTexture);
     std::vector<sf::Vector2f> enemybulletDirections;
    std::vector<sf::Sprite> enemybulletEtlite;

    sf::Sprite elite02shape(enemyElite02);
    elite02shape.setOrigin({elite02shape.getGlobalBounds().size.x/2-30,elite02shape.getGlobalBounds().size.y/2-30});
    elite02shape.setRotation(sf::degrees(180.f));
    elite02shape.scale({0.8f,0.8f});
    sf::Sprite missileShape(missileTexture);
    missileShape.setOrigin({missileShape.getGlobalBounds().size.x/2-10,missileShape.getGlobalBounds().size.y/2-10});
    missileShape.setRotation(sf::degrees(270.f));
    missileShape.scale({0.2f,0.2f});

    sf::Sprite superEliteshape(superEliteTexture);
    superEliteshape.setOrigin({superEliteshape.getGlobalBounds().size.x/2,superEliteshape.getGlobalBounds().size.y/2});
    superEliteshape.setRotation(sf::degrees(180.f));
    superEliteshape.scale({1.5f,1.5f});


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
    nukeEffectSprite.setColor(sf::Color(255, 255, 255, 190));

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
    float AsSpawn = 7;
    int wave = 1;
    int enemySpawnTimer=2000;
    int enemiesToSpawn = 10; 
    int enemiesSpawned = 0;
    int wavePattern = 1;
    int currentPatterns = 1;
    float direction = -1;
    bool superelitespawn = false;
    bool pull = false;
    int pulltime = 300;
    int pullcd = 0;
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
                vEnemyTexture.clear(); vEnemyHP.clear(); vBullet.clear(); bulletDirections.clear(); vAsteroid.clear();vUpWeapon.clear();vType.clear();
                ultimateTime = 0;
                e0.HPmax = 3; e0.HP = e0.HPmax;
                e1.HPmax = 1; e1.HP = e1.HPmax;
                elite01.HPmax = 20; elite01.HP = elite01.HPmax;
                elite02.HPmax = 30; elite02.HP = elite01.HPmax;
                missile.HP = 1; missile.HP = missile.HPmax;
                missile.speed = 3; e0.speed = 1; e1.speed = 3;
                superelite.HPmax = 200;
                superelite.HP = superelite.HPmax;
                currentPatterns = 1;
                ultimate = false;
                getfirst = false;
                enemiesToSpawn = 10; enemiesSpawned = 0; enemySpawnTimer = 0;
                shield = false;
                checkpow = 0;
                wave = 1;
                powerup = 0;
                AsSpawn = 7;
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
            nuke(vEnemyTexture, vEnemyHP, enemybulletEtlite, enemybulletDirections, vBullet,vEffect,effect);
            nukeCooldown = 120 * 45;
            nukeActive = true;
            nukeTimer = 120 ;
            effsound2.play();
            }
            //instakill//
            if (instakillCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
                instakill = true;
                instakillTimer = 120 * 10;
                instakillCooldown = 120 * 30;
                effsound3.play();
            }
            if (instakillTimer > 0)
            instakillTimer--;
            else instakill = false;
            //reset cooldown//
            if (resetCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
                nukeCooldown = 0;
                instakillCooldown = 0;
                resetCooldown = 120 * 90;
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
            nukeCooldownText.setString("Nuke Cooldown: " + std::to_string(nukeCooldown / 120) + "s [1]");
            instakillCooldownText.setString("Instakill Cooldown: " + std::to_string(instakillCooldown / 120) + "s [2]");
            resetCooldownText.setString("Reset Cooldown: " + std::to_string(resetCooldown / 120) + "s [3]");
    
            if (instakill) {
                instakillDurationBar.setSize(sf::Vector2f((instakillTimer / float(120 * 10)) * 200, 15));
            } else {
                instakillDurationBar.setSize(sf::Vector2f(0, 20));
            }

            //asteroid
            if(cAs.getElapsedTime().asSeconds()>=AsSpawn){
                vAsteroid.push_back(sad);
                cAs.restart();
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
        if (enemySpawnTimer >= framerate *2 && enemiesSpawned < enemiesToSpawn* currentPatterns) 
    {
        enemySpawnTimer = 0;
        int Rand;
        if(wave < 7) Rand = rand()%2;
        else Rand = rand()%4;
        int pattern = rand()%3+1;
        int hellwave = rand()%100;
        int spawnCount = 0;
        if(wave %10 == 0 && superelitespawn){
            SpawnSuperEnemyElite(window, vEnemyTexture,vEnemyHP, superEliteshape,superelite);
            superelitespawn = false;
        }
        if(Rand == 1){
            SpawnEnemyPattern(pattern, 10, window, vEnemyTexture, vEnemyHP, enemyShape, e0);
            spawnCount += 10;
        }
        if(Rand == 0){ 
            SpawnEnemyfollow(10, window, vEnemyTexture, vEnemyHP, followEnemyShape, e1);
            spawnCount += 10;
        }
        if(Rand == 2){
            if(wave > 15){
                SpawnEnemyElite(6, window, vEnemyTexture,vEnemyHP,elite01shape,elite01);
                spawnCount += 6;
            }else {
                SpawnEnemyElite(4, window, vEnemyTexture,vEnemyHP,elite01shape,elite01);
                spawnCount += 4;
            }
        }
        if(Rand == 3){
            if(wave < 15){
                SpawnEnemyEliteMissile(3,window, vEnemyTexture,vEnemyHP,elite02shape,elite02);
                spawnCount += 3;
            }else{
                SpawnEnemyEliteMissile(5,window, vEnemyTexture,vEnemyHP,elite02shape,elite02);
                spawnCount += 5;
            }
        }
        if(hellwave < 10 and wave > 10){
            SpawnEnemyEliteMissile(5,window, vEnemyTexture,vEnemyHP,elite02shape,elite02);
            SpawnEnemyElite(7, window, vEnemyTexture,vEnemyHP,elite01shape,elite01);
            SpawnEnemyfollow(20, window, vEnemyTexture, vEnemyHP, followEnemyShape, e1);
            SpawnEnemyPattern(pattern, 10, window, vEnemyTexture, vEnemyHP, enemyShape, e0);
        }
        enemiesSpawned += spawnCount;
    }
            //wave operator

        if (vEnemyTexture.size() == 0 && enemiesSpawned >= enemiesToSpawn* currentPatterns && enemySpawnTimer >= framerate*2) {
            wave++;
            if(AsSpawn>=1)AsSpawn-=0.25;
            if(wave% 10 == 0)superelitespawn =true;
            currentPatterns++;
            enemiesSpawned = 0;
            if(e0.speed <= e0.speedmax and wave % 2 == 0) e0.speed += 0.25;
            if(e0.HPmax < e0.limithp and wave % 3 == 0){
                e0.HPmax += 1;
                e0.HP = e0.HPmax;
            }
           if(e1.speed <= e1.speedmax and wave % 2 == 0) e1.speed += 0.5;
           if(e1.HPmax < e1.limithp and wave % 3 == 0){
            e1.HPmax += 1;
            e1.HP = e1.HPmax;
           }
            if(missile.speed <= missile.speedmax and wave % 3 == 0 && wave >7) missile.speed += 0.25;
            if(missile.HPmax <= missile.limithp){
                missile.HPmax += 1;
                missile.HP = missile.HPmax;
            }
           if(elite01.HPmax < elite01.limithp && wave >7  and wave % 3 == 0){
            elite01.HPmax += 1;
            elite01.HP = elite01.HPmax;
        }
        if(elite02.HPmax < elite02.limithp && wave >7 and wave % 3 == 0){
            elite02.HPmax += 1;
            elite02.HP = elite02.HPmax;
        }
        if(wave > 10){
            superelite.HPmax +=  10;
            superelite.HP = superelite.HPmax;
        }
        }
        
            //Enemy
        for (int i = 0; i < vEnemyTexture.size() ; i++)
        {
           
            //follow enemy
            if(vEnemyHP[i].type == 1 or vEnemyHP[i].type == 3){followEnemyPOS = (vEnemyTexture[i].getPosition());}
            playerCenter = {playerShape.getPosition().x + playerShape.getGlobalBounds().size.x/2 , playerShape.getPosition().y + playerShape.getGlobalBounds().size.y/2};
            enemyAimDir = (playerCenter-followEnemyPOS);
            if(enemyAimDir.x != 0.f and enemyAimDir.y != 0.f) enemyAimDirNorm =  enemyAimDir / (float)sqrt(pow(enemyAimDir.x,2)+pow(enemyAimDir.y,2));
            else enemyAimDirNorm = {0.f,0.f};
            
            //draw+move
            window.draw(vEnemyTexture[i]);//draw
            if(vEnemyHP[i].type == 0)vEnemyTexture[i].move({0.f,2.f*vEnemyHP[i].speed});//move
            if(vEnemyHP[i].type == 1)vEnemyTexture[i].move(enemyAimDirNorm*vEnemyHP[i].speed);//move
            if(vEnemyHP[i].type == 2)vEnemyTexture[i].move({0.f,0.5f*vEnemyHP[i].speed});
            if(vEnemyHP[i].type == 3)vEnemyTexture[i].move(enemyAimDirNorm*vEnemyHP[i].speed);
            if(vEnemyHP[i].type == 4)vEnemyTexture[i].move({0.f,0.5f*vEnemyHP[i].speed});
            if(vEnemyHP[i].type == 5){
                if ( vEnemyTexture[i].getPosition().x >= 800) {
                    direction = -1;
                }else if(vEnemyTexture[i].getPosition().x <= 100)direction = 1;
                vEnemyTexture[i].move({direction*1.f, 0.f});
            }

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
                SpawnEliteBullets(vEnemyTexture[i].getPosition(),elite01bullet,vBullet,1);
                vEnemyHP[i].shootTimer = 0;
            }
            if(vEnemyHP[i].type == 4 && vEnemyHP[i].shootTimer <= 150) vEnemyHP[i].shootTimer++;
            if(vEnemyHP[i].type == 4 && vEnemyHP[i].shootTimer >= 150){
                SpawnMissile(vEnemyTexture[i].getPosition(), vEnemyTexture,vEnemyHP,missileShape,missile,1);
                vEnemyHP[i].shootTimer = 0;
            }
            if(vEnemyHP[i].type == 5 && vEnemyHP[i].shootTimer <= 50) vEnemyHP[i].shootTimer++;
            if(vEnemyHP[i].type == 5 ){
                if (vEnemyHP[i].shootTimer >= 50){
                    int Rand = rand()%100;
                    if(Rand <10) {
                        pull = true;
                    }
                    if(Rand > 60) SpawnMissile(vEnemyTexture[i].getPosition(), vEnemyTexture,vEnemyHP,missileShape,missile,2);
                    SpawnEliteBullets(vEnemyTexture[i].getPosition(),elite01bullet,vBullet,2);
                    vEnemyHP[i].shootTimer = 0;
                }if(pull && pullcd <= 0) {
                    playerShape.move(0.01f*(vEnemyTexture[i].getPosition()-playerShape.getPosition()));
                    
                }
                if(pulltime > 0 && pull ) pulltime--;
                if(pullcd > 0) pullcd--;
                if(pulltime <= 0) {
                    pulltime = 300;
                    pull = false;
                }
            }
            
            //collision to player
            if(vEnemyTexture[i].getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) and playerHitCD <= 0 and vEnemyHP[i].type != 5) {
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
                    if(instakill && vEnemyHP[k].type != 5){vEnemyHP[k].HP = 0;}
                    else
                    vEnemyHP[k].HP -= player.Pdmg;//dmg

                    vEnemyHP[k].isHit = true;
                    vEnemyHP[k].hitTimer = 15;
                    
                    if (vEnemyHP[k].HP <= 0) {
                        if(vEnemyHP[k].type == 5){
                            nukeActive = true;
                            nukeTimer = 120 ;
                            effsound2.play();
                            }
                        if(powerupCD <= 0){
                            int dropChance = rand()%2;
                            if(dropChance == 1){
                                int dropItem = rand()%3;
                                if(dropItem == 0){
                                    vUpWeapon.push_back(sf::Sprite(upgradeWeapon));
                                    vUpWeapon.back().setScale({0.1f, 0.1f});
                                    vUpWeapon.back().setPosition(vEnemyTexture[k].getPosition());
                                    vType.push_back(1);
                                    powerupCD = 120;
                                }
                                if(dropItem == 1){
                                    vUpWeapon.push_back(sf::Sprite(heal));
                                    vUpWeapon.back().setScale({0.1f, 0.1f});
                                    vUpWeapon.back().setPosition(vEnemyTexture[k].getPosition());
                                    vType.push_back(2);
                                    powerupCD = 120;
                                }
                                if(dropItem == 2){
                                    vUpWeapon.push_back(sf::Sprite(shieldIcon));
                                    vUpWeapon.back().setScale({0.2f, 0.2f});
                                    vUpWeapon.back().setPosition(vEnemyTexture[k].getPosition());
                                    vType.push_back(3);
                                    powerupCD = 120;
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
        //check wave
        cout << wave<< " " << currentPatterns << " " << enemiesToSpawn << " " << currentPatterns<<endl;
        //asteroid
        if(player.HP>0){
        for (int i = vAsteroid.size()-1 ; i >= 0 ; i--)
            {
                vAsteroid[i].asteroidMove(window,pause);
                if(vAsteroid[i].asteroidSprite.getGlobalBounds().findIntersection(playerShape.getGlobalBounds()) )
                {   if(!shield && playerHitCD <= 0)player.HP -= 5;
                    vAsteroid.erase(vAsteroid.begin()+i);  
                    break;}
                if(vAsteroid[i].spritePOS().y>800){vAsteroid.erase(vAsteroid.begin()+i);  break;} 
            }
        }
        //effect
        for (int i = 0; i < vEffect.size(); i++)
            {
                vEffect[i].effectChange(showEffect,effSound1,vEffect.back().pos,window,pause);
                if(vEffect[i].state>=8) {vEffect.erase(vEffect.begin()+i); i--; break;}
            }

        window.display();
       
        }

    return 0;//code แม่งโคตร KEYES

}
