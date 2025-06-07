#include "BallChain.h"
#include <cmath>
#include <iostream>
#include "Notification.h"  // pentru funcții virtuale
#include "Logger.h"        // pentru template

BallChain::BallChain() : movementIndex(0) {}
BallChain::BallChain(const BallChain& other) = default;
BallChain& BallChain::operator=(const BallChain& other) = default;
BallChain::~BallChain() {}

float spacingPerBall = 9.5f;

ScoreNotification scoreNotifier;
Logger<std::string>& getChainLogger() {
    static Logger<std::string> logger;
    return logger;
}

const std::vector<Ball>& BallChain::getBalls() const {
    return balls;
}

void BallChain::addBall(const Ball& ball) {
    float baseIndex = (balls.empty() ? movementIndex : balls.back().getPathIndex());
    Ball b = ball;
    b.setPathIndex(baseIndex + spacingPerBall);
    balls.push_back(b);
    getChainLogger().log("Ball added at end of chain");}

void BallChain::insertBall(size_t index, const Ball& b) {
    if (index > balls.size())
        throw std::out_of_range("can't insert at this index");

    Ball inserted = b;

    if (balls.empty()) {
        inserted.setPathIndex(static_cast<float>(movementIndex));
    } else if (index == 0) {
        inserted.setPathIndex(balls[0].getPathIndex() - spacingPerBall);
    } else {
        inserted.setPathIndex(balls[index - 1].getPathIndex() + spacingPerBall);
    }

    balls.insert(balls.begin() + index, inserted);

    for (size_t i = index + 1; i < balls.size(); ++i) {
        balls[i].setPathIndex(balls[i].getPathIndex() + spacingPerBall);
    }

    updatePositions();
    getChainLogger().log("Ball inserted at index " + std::to_string(index));
    scoreNotifier.send(0); // pentru demonstrație, 0 puncte
}

void BallChain::updatePositions() {
    for (Ball& b : balls) {
        float idx = b.getPathIndex();
        int i0 = static_cast<int>(idx);
        int i1 = i0 + 1;

        if (i0 >= 0 && i1 < static_cast<int>(path.size())) {
            float t = idx - i0;
            sf::Vector2f p0 = path[i0];
            sf::Vector2f p1 = path[i1];

            float x = (1 - t) * p0.x + t * p1.x;
            float y = (1 - t) * p0.y + t * p1.y;

            b.setPosition(x, y);
        }
    }
}

int BallChain::checkAndEliminate() {
    int totalScore = 0;
    bool chainReaction = true;
    const float maxSpacing = spacingPerBall * 1.5f;

    while (chainReaction) {
        chainReaction = false;
        if (balls.size() < 3) return totalScore;

        size_t i = 0;
        while (i < balls.size()) {
            size_t j = i + 1;
            while (j < balls.size() &&
                   balls[j].getColor() == balls[i].getColor() &&
                   std::abs(balls[j].getPathIndex() - balls[j - 1].getPathIndex()) <= maxSpacing) {
                ++j;
            }

            if (j - i >= 3) {
                balls.erase(balls.begin() + i, balls.begin() + j);
                totalScore += 50;
                chainReaction = true;
                updatePositions();
                compactChain();
                scoreNotifier.send(50); // notificare scor
                break;
            }

            i = j;
        }
    }

    return totalScore;
}

void BallChain::compactChain() {
    if (balls.empty()) return;
    balls[0].setPathIndex(balls[0].getPathIndex());

    for (size_t i = 1; i < balls.size(); ++i) {
        float previous = balls[i - 1].getPathIndex();
        balls[i].setPathIndex(previous + spacingPerBall);
    }

    updatePositions();
}

void BallChain::clear() {
    balls.clear();
    path.clear();
    movementIndex = 0;
}

void BallChain::generatePath(int maxPoints, float spacing) {
    path.clear();
    float startX = 100.0f, startY = 150.0f;
    float minX = 50.0f, minY = 50.0f;
    float maxX = 1200.0f, maxY = 900.0f;

    float x = startX, y = startY;
    int dir = 0;
    int horizontalSteps = (maxX - minX) / spacing;
    int verticalSteps = (maxY - minY) / spacing;

    while (static_cast<int>(path.size()) < maxPoints) {
        int steps = (dir % 2 == 0) ? horizontalSteps : verticalSteps;

        for (int i = 0; i < steps && static_cast<int>(path.size()) < maxPoints; ++i) {
            path.emplace_back(x, y);
            switch (dir) {
                case 0: x += spacing; break;
                case 1: y += spacing; break;
                case 2: x -= spacing; break;
                case 3: y -= spacing; break;
            }
        }

        if (dir == 1 || dir == 3) {
            float wallSpacing = 110.0f;
            minX += wallSpacing;
            minY += wallSpacing;
            maxX -= wallSpacing;
            maxY -= wallSpacing;
            horizontalSteps = (maxX - minX) / spacing;
            verticalSteps = (maxY - minY) / spacing;
        }
        dir = (dir + 1) % 4;
    }

    movementIndex = 0;
}

int BallChain::getPathLength() const {
    return static_cast<int>(path.size());
}

void BallChain::addBallAtStart(const Ball& ball) {
    float baseIndex = (balls.empty() ? movementIndex : balls.front().getPathIndex());
    Ball b = ball;
    b.setPathIndex(baseIndex - spacingPerBall);
    balls.insert(balls.begin(), b);
    updatePositions();
    getChainLogger().log("Ball added at start of chain");
}

bool intersectsSegmentCircle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f center, float radius) {
    sf::Vector2f d = p2 - p1;
    sf::Vector2f f = p1 - center;

    float a = d.x * d.x + d.y * d.y;
    float b = 2 * (f.x * d.x + f.y * d.y);
    float c = (f.x * f.x + f.y * f.y) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;

    discriminant = sqrt(discriminant);
    float t1 = (-b - discriminant) / (2 * a);
    float t2 = (-b + discriminant) / (2 * a);

    return (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1);
}

int BallChain::findCollisionIndexAdvanced(sf::Vector2f position, sf::Vector2f velocity) {
    const float radius = Ball::RADIUS;
    const float insertionThreshold = 2.0f * radius;

    for (size_t i = 0; i < balls.size(); ++i) {
        float dx = balls[i].getX() - position.x;
        float dy = balls[i].getY() - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < insertionThreshold) {
            sf::Vector2f toBall = sf::Vector2f(balls[i].getX(), balls[i].getY()) - position;
            float dot = toBall.x * velocity.x + toBall.y * velocity.y;
            return (dot > 0 && i + 1 < balls.size()) ? i + 1 : i;
        }
    }
    return -1;
}

sf::Vector2f BallChain::getLastPathPoint() const {
    return path.empty() ? sf::Vector2f(0.f, 0.f) : path.back();
}

void BallChain::moveForward(float speed) {
    for (Ball& b : balls) {
        float newIndex = b.getPathIndex() + speed;
        b.setPathIndex(newIndex);
    }
}

std::ostream& operator<<(std::ostream& os, const BallChain& chain) {
    os << "BallChain: ";
    for (const auto& ball : chain.balls) {
        os << ball << " ";
    }
    return os;
}
