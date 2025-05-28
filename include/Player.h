#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include <cmath>
#include <random>

class Player {
private:
    float x, y;             
    float angle;
    Ball currentBall;
    Ball nextBall;
    Color generateRandomColor();
    float bulletSpeed = 10.f;
    sf::Vector2f lastShotVelocity;

public:
    Player(float x = 650, float y = 500);
    ~Player();

    void updateDirection(float mouseX, float mouseY);
    float getAngle() const;

    const Ball& getCurrentBall() const;
    const Ball& getNextBall() const;
    void swapBalls();

    float getX() const;
    float getY() const;

    Ball shoot(const sf::RenderWindow& window);
    sf::Vector2f getLastShotVelocity() const;
    sf::Vector2f getDirection(const sf::RenderWindow& window) const;
};
#endif