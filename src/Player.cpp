#include "Player.h"

Player::Player(float x, float y)
    : x(x), y(y), angle(0.0f),
      currentBall(generateRandomColor(), x, y),
      nextBall(generateRandomColor(), x, y) {}

Player::~Player() {}

Color Player::generateRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 5);
    return static_cast<Color>(dist(gen));
}

void Player::updateDirection(float mouseX, float mouseY) {
    float dx = mouseX - x;
    float dy = mouseY - y;
    angle = std::atan2(dy, dx);
}

float Player::getAngle() const {
    return angle;
}

const Ball& Player::getCurrentBall() const {
    return currentBall;
}

const Ball& Player::getNextBall() const {
    return nextBall;
}

void Player::swapBalls() {
    std::swap(currentBall, nextBall);
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }
sf::Vector2f Player::getDirection(const sf::RenderWindow& window) const {
    sf::Vector2f center(650, 500);
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    sf::Vector2f dir = mousePos - center;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0)
        dir /= length;

    return dir;
}


Ball Player::shoot(const sf::RenderWindow& window) {
    sf::Vector2f direction = getDirection(window);
    sf::Vector2f velocity = direction * bulletSpeed;

    lastShotVelocity = velocity;

    Ball shotBall(currentBall);
    shotBall.setVelocity(velocity);
    currentBall = nextBall;
    nextBall = Ball(generateRandomColor(), x, y);
    return shotBall;
}

sf::Vector2f Player::getLastShotVelocity() const {
    return lastShotVelocity;
}
