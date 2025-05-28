#include "Ball.h"

Ball::Ball(Color color, float x, float y)
    : color(color), x(x), y(y), pathIndex(0.0f) {}
Ball::Ball(const Ball& other) = default;
Ball& Ball::operator=(const Ball& other) = default;
Ball::~Ball() {}

Color Ball::getColor() const { return color; }
void Ball::setColor(Color c) { color = c; }
float Ball::getX() const { return x; }
float Ball::getY() const { return y; }
void Ball::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Ball::setPathIndex(float index) {
    pathIndex = index;
}

float Ball::getPathIndex() const {
    return pathIndex;
}

void Ball::setVelocity(const sf::Vector2f& v) {
    velocity = v;
}

sf::Vector2f Ball::getVelocity() const {
    return velocity;
}

std::ostream& operator<<(std::ostream& os, const Ball& b) {
    os << "Ball(" << static_cast<int>(b.color) << ", x=" << b.x << ", y=" << b.y << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Ball& b) {
    int c;
    float x, y;
    is >> c >> x >> y;
    b.color = static_cast<Color>(c);
    b.x = x;
    b.y = y;
    return is;
}
