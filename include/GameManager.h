#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "BallChain.h"

class GameManager {
private:
    Player player;
    BallChain chain;
    std::vector<std::pair<Ball, sf::Vector2f>> firedBalls;
    int score = 0;
    const float gameDuration = 60.0f;
    float ballGenerationTimer = 0.f;
    const float generationInterval = 0.5f;
    sf::Texture finishTexture;
    sf::Sprite finishSprite;
    bool gameOverDueToFinish = false;
    sf::Vector2f finishPosition;
    sf::Clock gameClock;
    sf::Clock ballSpawnClock;


public:
    GameManager();
    ~GameManager();
    void handleShoot(const sf::RenderWindow& window);
    void swapBalls();
    void update(float mouseX, float mouseY, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    int getScore() const;
    float getElapsedTime() const;
    bool isGameOver() const;
    void reset();
};
#endif
