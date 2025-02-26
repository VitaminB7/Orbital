#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <ctime>


class Player
{ 
public: 

    int HP;
    int HPmax;
    int Pdmg;
    int score;
    
    Player(){
        HPmax= 50;
        HP=HPmax;
        Pdmg = 1;
        score = 0;
    }

};

class Enemy
{
public: 
     
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
        speedmax = 2.5;
        isHit = false;
        hitTimer = 0;
    }

    Enemy(int a){
        if(a == 1){//follow Enemy
            type = 1;
            limithp = 3;
            HPmax= 1;
            HP=HPmax;
            speed = 2;
            speedmax = 7;
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
        else if(a== 5){//super elite
            type = 5;
            HPmax= 200;
            HP=HPmax;
            speed = 1;
            shootTimer = 30;
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
void SpawnEliteBullets(sf::Vector2f enemyPos,Bullet elite01bullet,std::vector<Bullet> &vBullet,int pattern)
{   
    if(pattern == 1){
        const int numBullets = 7; // จำนวนกระสุน
        const float spreadAngle = 180.f; // กระจายครึ่งวงกลม (180 องศา)
        float startAngle = 0; // เริ่มยิงจาก -180 องศาไป 0 องศา
    
        for (int j = 0; j < numBullets; j++)
        {
            float angle = startAngle + (j * spreadAngle / (numBullets - 1)); // คำนวณมุมของกระสุนแต่ละอัน
            float radian = angle * (M_PI / 180.f); // แปลงเป็นเรเดียน
            
            vBullet.push_back(elite01bullet);
            vBullet.back().bulletSprite.setPosition(enemyPos); // ตั้งกระสุนให้ออกจากตำแหน่งศัตรูa
    
            sf::Vector2f direction({cos(radian), sin(radian)});
            vBullet.back().bulletDirection = direction * 2.f;
        }
    }
    if(pattern == 2){
        const int numBullets = 18; 
        const float spreadAngle = 360.f; 
        float startAngle = 0; // เริ่มยิงจาก -180 องศาไป 0 องศา

        for (int j = 0; j < numBullets; j++)
        {
            float angle = startAngle + (j * spreadAngle / (numBullets - 1)); // คำนวณมุมของกระสุนแต่ละอัน
            float radian = angle * (M_PI / 180.f); // แปลงเป็นเรเดียน
            
            vBullet.push_back(elite01bullet);
            vBullet.back().bulletSprite.setPosition(enemyPos); // ตั้งกระสุนให้ออกจากตำแหน่งศัตรูa
    
            sf::Vector2f direction({cos(radian), sin(radian)});
            vBullet.back().bulletDirection = direction * 5.f;
        }
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
    sf::Sprite missileShape, Enemy missile,int pattern)
    {       
        if(pattern == 1){
            for (int i = 0; i < 2; i++) {
                vEnemyTexture.push_back(missileShape);
                vEnemyHP.push_back(missile);
                
                if (i < 1) 
                vEnemyTexture.back().setPosition({enemyPos.x - 150, enemyPos.y}); // left
                else 
                vEnemyTexture.back().setPosition({enemyPos.x + 50, enemyPos.y}); // right
                }
        }

            if(pattern == 2){
                for (int i = 0; i < 4; i++) {
                    vEnemyTexture.push_back(missileShape);
                    vEnemyHP.push_back(missile);
                    if (i == 0) 
                    vEnemyTexture.back().setPosition({enemyPos.x - 150, enemyPos.y}); 
                    else if(i == 1)
                    vEnemyTexture.back().setPosition({enemyPos.x - 250, enemyPos.y}); 
                    else if(i == 2)
                    vEnemyTexture.back().setPosition({enemyPos.x , enemyPos.y}); 
                    else 
                    vEnemyTexture.back().setPosition({enemyPos.x +100, enemyPos.y}); 
                    }
            }

    }
void SpawnSuperEnemyElite(sf::RenderWindow &window, 
        vector<sf::Sprite> &vEnemyTexture, vector<Enemy> &vEnemyHP, 
        sf::Sprite superEliteshape,Enemy superElite)
        {
            float startX = window.getSize().x/2+100;
            float startY = 150;
            vEnemyTexture.push_back(superEliteshape);
            vEnemyHP.push_back(superElite);
            vEnemyTexture.back().setPosition({startX, startY});
        }


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
    std::vector<Bullet>& vBullet,std::vector<Effect> &vEffect,Effect effect) {

    for (int i = 0; i < vEnemyHP.size(); i++)
    {
        if(vEnemyHP[i].type != 5) 
        vEnemyHP[i].HP = 0;
        if (vEnemyHP[i].HP <= 0) {
            vEffect.push_back(effect);
            vEffect.back().pos={vEnemyTexture[i].getPosition()};
            vEnemyTexture.erase(vEnemyTexture.begin() + i);
            vEnemyHP.erase(vEnemyHP.begin() + i);
            i--;
        }    
    }
    
    enemybulletEtlite.clear();
    enemybulletDirections.clear();
    clearEnemyBullets(vBullet);
}

class asteroid {

    sf::Sprite warningSprite; 
    sf::Clock clock;
    sf::Clock clockBlink;
    sf::RectangleShape warningArea;
    int state = 0;
    int blink;
    float R;
    sf::Vector2f pos;

public:
    sf::Sprite asteroidSprite;
    asteroid(sf::Texture &asTex, sf::Texture &warnTex);
    void asteroidMove(sf::RenderWindow& window,bool &pause); 
    void warnBlink();
    sf::Vector2f spritePOS();
    void update();
};

asteroid::asteroid(sf::Texture &asTex, sf::Texture &warnTex)
: asteroidSprite(asTex) , warningSprite(warnTex)
{
    asteroidSprite.setScale({0.1,0.1});
    warningSprite.setScale({0.1,0.1});
    warningArea.setSize({asteroidSprite.getGlobalBounds().size.x, 1000.f});
    warningArea.setFillColor(sf::Color(255,0,0,80));
}


void asteroid::warnBlink() {
    if(state<=4){
        if (clockBlink.getElapsedTime().asSeconds() < 0.5f) {
            blink = 255;
        } else if (clockBlink.getElapsedTime().asSeconds() < 1.f) {
            blink = 50;
        }
        else if (clockBlink.getElapsedTime().asSeconds() > 1.f)
        {
        clockBlink.restart();
        state++;
        }
    }
}

void asteroid::asteroidMove(sf::RenderWindow& window,bool &pause) {
    
    if(!pause){
        if(state==0)
        {
            update();
            state++;
        }
        if(state<=4)
        {
            warnBlink();
            warningSprite.setColor(sf::Color(255,0,0,blink));
            window.draw(warningSprite);
            window.draw(warningArea);
        }
        else 
        {
            asteroidSprite.move({0,10});
            window.draw(asteroidSprite);
        }
    }
}

sf::Vector2f asteroid::spritePOS()
{
    return asteroidSprite.getPosition();
}

void asteroid::update()
{
    R = rand() % 800 + asteroidSprite.getGlobalBounds().size.x;
    if (R > 800 - asteroidSprite.getGlobalBounds().size.x) R = 800 - asteroidSprite.getGlobalBounds().size.x;
    pos = {R,0};
    warningArea.setPosition(pos);
    warningSprite.setPosition({pos.x+13,0});
    asteroidSprite.setPosition(pos);
}
