#include "GameManager.h"
#include <cmath>
#include <algorithm>
#include <iostream>

sf::Color convertColor(Color c) {
    switch (c) {
        case Color::Red: return sf::Color::Red;
        case Color::Green: return sf::Color::Green;
        case Color::Blue: return sf::Color::Blue;
        case Color::Yellow: return sf::Color::Yellow;
        case Color::Purple: return sf::Color(128, 0, 128);
        case Color::Gray: return sf::Color(128, 128, 128);
        default: return sf::Color::White;
    }
}
Color generateRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 5);
    return static_cast<Color>(dist(gen));
}

// initialize game
GameManager::GameManager() : player(650, 500) {
    score = 0;
    ballSpawnClock.restart();
    gameClock.restart();
    gameOverDueToFinish = false; 
    chain.clear();
    chain.generatePath(650, 5.0f);
  
    firedBalls.clear(); 
    player = Player(650, 500); 
    ballGenerationTimer = 0.f; 
    finishPosition = chain.getLastPathPoint(); 
    if (!finishTexture.loadFromFile("assets/halfring.png")) {
        std::cerr << "couldn't load png!\n";
    }
    finishSprite.setTexture(finishTexture);
    finishSprite.setOrigin(finishTexture.getSize().x / 2, finishTexture.getSize().y / 2);
    finishSprite.setScale(0.1f, 0.1f);

    if (!chain.getPath().empty()) {
        // last point from path
        sf::Vector2f finishPosition = chain.getPath().back();
        finishSprite.setPosition(finishPosition);
    }

    const int numBalls = 10;
    for (int i = 0; i < numBalls; ++i) {
        chain.addBall(Ball(generateRandomColor()));
    }
    // chain.updatePositions();
}

GameManager::~GameManager() {}

// actual gameplay
void GameManager::update(float mouseX, float mouseY, const sf::RenderWindow& window) {
    player.updateDirection(mouseX, mouseY);
    chain.moveForward(0.005f);
    if (!chain.getBalls().empty()) {
        const auto& balls = chain.getBalls();

        // find max pathindex ball (closest to finish)
        const Ball* lastBall = &balls[0];
        for (const auto& b : balls) {
            if (b.getPathIndex() > lastBall->getPathIndex()) {
                lastBall = &b;
            }
        }

        float dx = lastBall->getX() - finishPosition.x;
        float dy = lastBall->getY() - finishPosition.y;
        float distSquared = dx * dx + dy * dy;

        if (distSquared < Ball::RADIUS * Ball::RADIUS || lastBall->getPathIndex() >= chain.getPathLength() - 1) {
            gameOverDueToFinish = true;
        }
    }


    ballGenerationTimer += ballSpawnClock.restart().asSeconds();
    if (ballGenerationTimer >= generationInterval) {
        Color randomColor = static_cast<Color>(rand() % 6);
        chain.addBallAtStart(Ball(randomColor));
        ballGenerationTimer = 0.f;


        // chain.updatePositions();
}

    chain.updatePositions();

    float speed = 0.5f;


    // fired ball handler
    for (auto it = firedBalls.begin(); it != firedBalls.end(); ) {
        Ball& b = it->first;
        sf::Vector2f dir = it->second;

        b.setPosition(b.getX() + dir.x * speed, b.getY() + dir.y * speed);

        // verificare coliziune
        int index = chain.findCollisionIndexAdvanced(sf::Vector2f(b.getX(), b.getY()), dir);
        if (index != -1) {
            chain.insertBall(index, b);
            // updatePositions: must have for correct output
            chain.updatePositions();
            int gained = chain.checkAndEliminate();
            // debug
            std::cout << "erase score: " << gained << "\n";
            score += gained;
            // remove launched ball from 'array'
            it = firedBalls.erase(it);
        } else if (b.getX() < 0 || b.getX() > 1300 || b.getY() < 0 || b.getY() > 1100) {
            it = firedBalls.erase(it); 
        } else {
            ++it;
        }
    }
}

// shooting logic
void GameManager::handleShoot(const sf::RenderWindow& window) {
    Ball b = player.shoot(window);
    sf::Vector2f velocity = player.getLastShotVelocity();

    float angle = player.getAngle();
    sf::Vector2f dir(std::cos(angle), std::sin(angle));
    firedBalls.push_back({ b, dir });
}

void GameManager::swapBalls() {
    player.swapBalls();
}

int GameManager::getScore() const {
    return score;
}

float GameManager::getElapsedTime() const {
    return gameClock.getElapsedTime().asSeconds();
}

// stop game if time is up or is balls have been swallowed
bool GameManager::isGameOver() const {
    return gameClock.getElapsedTime().asSeconds() >= gameDuration || gameOverDueToFinish;
}

// alternative reset
void GameManager::reset() {
    score = 0;
    ballSpawnClock.restart();
    gameClock.restart();
    gameOverDueToFinish = false; 
    chain.clear();
    chain.generatePath(650, 5.0f);
    finishPosition = chain.getLastPathPoint(); 
    finishSprite.setPosition(finishPosition);
    const int numBalls = 10;
    for (int i = 0; i < numBalls; ++i) {
        chain.addBall(Ball(generateRandomColor()));
    }
    // chain.updatePositions();
    firedBalls.clear(); 
    player = Player(650, 500); 
    ballGenerationTimer = 0.f; 
    firedBalls.clear(); 

}

// draw entities
void GameManager::render(sf::RenderWindow& window) {
    for (const Ball& b : chain.getBalls()) {
        sf::CircleShape circle(Ball::RADIUS);
        circle.setFillColor(convertColor(b.getColor()));
        circle.setOrigin(Ball::RADIUS, Ball::RADIUS);
        circle.setPosition(b.getX(), b.getY());
        window.draw(circle);
        window.draw(finishSprite);
    }

    for (const auto& [b, dir] : firedBalls) {
        sf::CircleShape circle(15.f);
        circle.setFillColor(convertColor(b.getColor()));
        circle.setOrigin(15, 15);
        circle.setPosition(b.getX(), b.getY());
        window.draw(circle);
    }

    sf::CircleShape base(25.f);
    base.setFillColor(sf::Color(80, 80, 80));
    base.setOrigin(25, 25);
    base.setPosition(player.getX(), player.getY());
    window.draw(base);

    sf::CircleShape current(15.f);
    current.setFillColor(convertColor(player.getCurrentBall().getColor()));
    current.setOrigin(15, 15);
    current.setPosition(player.getX(), player.getY());
    window.draw(current);

    sf::CircleShape next(10.f);
    next.setFillColor(convertColor(player.getNextBall().getColor()));
    next.setOrigin(10, 10);
    next.setPosition(player.getX() + 35.f, player.getY() + 35.f);
    window.draw(next);

    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(player.getX(), player.getY()), sf::Color::White),
        sf::Vertex(sf::Vector2f(player.getX() + 50 * std::cos(player.getAngle()),
                               player.getY() + 50 * std::sin(player.getAngle())), sf::Color::White)
    };
    window.draw(line, 2, sf::Lines);
}

