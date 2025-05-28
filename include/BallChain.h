#ifndef BALLCHAIN_H
#define BALLCHAIN_H
#include "Ball.h"
#include <vector>
#include <stdexcept>
#include <SFML/System/Vector2.hpp>
#include <iostream>

class BallChain {
private:
    std::vector<Ball> balls;
    std::vector<sf::Vector2f> path;
    
    // start index
    int movementIndex;

public:
    BallChain();
    BallChain(const BallChain& other);
    BallChain& operator=(const BallChain& other);
    ~BallChain();

    const std::vector<Ball>& getBalls() const;

    void addBall(const Ball& ball);                    
    void insertBall(size_t index, const Ball& ball);   
    void updatePositions();                           
    void moveForward(float speed);                    
    int checkAndEliminate();
    void clear();      
    void generatePath(int maxPoints, float spacing = 24.0f);
    void addBallAtStart(const Ball& ball);


    // get index after collision (where to insert new ball)
    int findCollisionIndexAdvanced(sf::Vector2f position, sf::Vector2f velocity);

    friend std::ostream& operator<<(std::ostream& os, const BallChain& chain);
    const std::vector<sf::Vector2f>& getPath() const { return path; }
    sf::Vector2f getLastPathPoint() const;
    int getPathLength() const;
    void compactChain();
};

#endif
