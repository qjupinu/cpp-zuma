#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include <fstream>
#include <string>

enum class GameState {
    MainMenu,
    Playing,
    GameOver
};


int main() {
    sf::RenderWindow window(sf::VideoMode(1300, 1100), "cpp-zuma");
    GameState state = GameState::MainMenu;
    GameManager game;

    sf::Font font;
    font.loadFromFile("assets/native.ttf");

    int highscoreValue = 0;
    std::ifstream infile("data/highscore.txt");
    if (infile.is_open()) {
        infile >> highscoreValue;
    }

    sf::Text title("CPP-Zuma", font, 64);
    title.setFillColor(sf::Color::Yellow);
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(1300 / 2, 200);

    sf::Text highscore("Highscore: " + std::to_string(highscoreValue), font, 24);
    highscore.setFillColor(sf::Color::White);
    highscore.setOrigin(highscore.getLocalBounds().width / 2, highscore.getLocalBounds().height / 2);
    highscore.setPosition(1300 / 2, 270);

    sf::Text playButton("Play", font, 48);
    playButton.setFillColor(sf::Color::Green);
    playButton.setOrigin(playButton.getLocalBounds().width / 2, playButton.getLocalBounds().height / 2);
    playButton.setPosition(1300 / 2, 400);

    sf::Text exitButton("Exit", font, 48);
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setOrigin(exitButton.getLocalBounds().width / 2, exitButton.getLocalBounds().height / 2);
    exitButton.setPosition(1300 / 2, 500);

    sf::Text gameOverText("Game Over", font, 64);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
    gameOverText.setPosition(1300 / 2, 350);

    sf::Text returnMenuButton("Return to main menu", font, 32);
    returnMenuButton.setFillColor(sf::Color::Yellow);
    returnMenuButton.setOrigin(returnMenuButton.getLocalBounds().width / 2, returnMenuButton.getLocalBounds().height / 2);
    returnMenuButton.setPosition(1300 / 2, 750);

    sf::Text timeText("", font, 28);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(20, 20);

    sf::Text scoreText("", font, 28);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 60);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameState::MainMenu && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (playButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    game.reset();
                    state = GameState::Playing;
                } else if (exitButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    window.close();
                }
            } else if (state == GameState::Playing && event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left)
                    game.handleShoot(window);
                else if (event.mouseButton.button == sf::Mouse::Right)
                    game.swapBalls();
            } else if (state == GameState::GameOver && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (returnMenuButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    state = GameState::MainMenu;
                }
            }
        }

        window.clear();

        if (state == GameState::MainMenu) {
            window.draw(title);
            window.draw(highscore);
            window.draw(playButton);
            window.draw(exitButton);
        } else if (state == GameState::Playing) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            game.update(mousePos.x, mousePos.y, window);
            game.render(window);

            float timeLeft = 60.0f - game.getElapsedTime(); 
            if (timeLeft < 0) timeLeft = 0;

            int score = game.getScore();

            timeText.setString("Time left: " + std::to_string(static_cast<int>(timeLeft)) + " seconds");
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(timeText);
            window.draw(scoreText);



            if (game.isGameOver()) {
                int finalScore = game.getScore();
                if (finalScore > highscoreValue) {
                    std::ofstream outfile("data/highscore.txt");
                    outfile << finalScore;
                    highscoreValue = finalScore;
                    highscore.setString("Highscore: " + std::to_string(highscoreValue));
                    highscore.setOrigin(highscore.getLocalBounds().width / 2, highscore.getLocalBounds().height / 2);
                }
                state = GameState::GameOver;
                
            }
        } else if (state == GameState::GameOver) {
            window.draw(gameOverText);
            window.draw(returnMenuButton);
            
        }

        window.display();
    }

    return 0;
}
