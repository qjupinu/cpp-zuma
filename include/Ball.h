#ifndef BALL_H
#define BALL_H
#include <SFML/Graphics.hpp>
#include "Color.h"
#include <iostream>
#include <memory>

class Ball {
private:
    Color color;
    float x, y;
    float pathIndex;
    sf::Vector2f velocity;

public:
    static constexpr float RADIUS = 24.0f;
    Ball(Color color = Color::Red, float x = 0, float y = 0);
    Ball(const Ball& other);
    Ball& operator=(const Ball& other);

    ~Ball();

    Color getColor() const;
    void setColor(Color c);

    float getX() const;
    float getY() const;
    void setPosition(float newX, float newY);

    friend std::ostream& operator<<(std::ostream& os, const Ball& b);
    friend std::istream& operator>>(std::istream& is, Ball& b);

    void setPathIndex(float index);
    float getPathIndex() const;
    void setVelocity(const sf::Vector2f& v);
    sf::Vector2f getVelocity() const;
};
#endif
