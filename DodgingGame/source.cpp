#include <SFML/Graphics.hpp>
#include<iostream>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(640, 480), "Dodge Balls");
    window.setFramerateLimit(60);

    //enemy
    CircleShape enemy;
    enemy.setRadius(10.f);
    enemy.setFillColor(Color::Yellow);
    enemy.setPosition(window.getSize().x - enemy.getRadius()*2 , window.getSize().y / 2);
    int spawnTime = 0;
    int spawnInterval = 30;
    int velX = -5.f;

    std::vector<CircleShape> enemies;
    enemies.push_back(CircleShape(enemy));

    //player
    RectangleShape player;
    player.setSize(Vector2f(40.f, 40.f));
    player.setFillColor(Color::Green);
    player.setPosition(10.f, window.getSize().y / 2 - player.getSize().y / 2);
    int hp = 10;
    int score = 0;

    RectangleShape hpBar;
    hpBar.setFillColor(Color::Red);
    hpBar.setSize(Vector2f(20.f*hp, 20.f));
    hpBar.setPosition(window.getSize().x/2 - hpBar.getSize().x/2, 10);

    while (window.isOpen() && hp > 0)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == event.KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
        }
        //UPDATE
        window.clear();

        //player updating
        player.setPosition(player.getPosition().x, Mouse::getPosition(window).y);
        if (player.getPosition().y <= 0) {
            player.setPosition(player.getPosition().x, 0);
        }
        if (player.getPosition().y > window.getSize().y - player.getSize().y) {
            player.setPosition(player.getPosition().x, window.getSize().y - player.getSize().y);
        }

        
        //enemies updating
        if (spawnTime < spawnInterval) {
            spawnTime++;
        }
        if (spawnTime >= spawnInterval) {
            enemy.setPosition(window.getSize().x - enemy.getRadius() * 2, rand()% int(window.getSize().y - enemy.getRadius()*2));
            enemies.push_back(CircleShape(enemy));
            spawnTime = 0;
        }

        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].move(velX, 0.f);
            //check collision if he the side of the window
            if (enemies[i].getPosition().x < 0 - enemy.getRadius() * 2) {
                enemies.erase(enemies.begin() + i);
                velX--;
                spawnInterval--;
                score++;
            }
        }
        
        //collision
        for (int i = 0; i < enemies.size(); i++) {
            //collision with the player
            if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds())) {
                enemies.erase(enemies.begin() + i);
                hp--;
                hpBar.setSize(Vector2f(20.f * hp, 20.f));
                if (hp <= 0) {
                    std::cout << score << std::endl;
                    std::cout << "GAME OVER" << std::endl;
                }
            }
        }


        //DRAW
        window.draw(player);
        for (int i = 0; i < enemies.size(); i++) {
            window.draw(enemies[i]);
            
        }
        //UI
        window.draw(hpBar);
        window.display();
    }

    return 0;
}