#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <iostream>

class Notification {
public:
    virtual void send(int value = 0) const = 0;  // send score updates
    virtual ~Notification() = default;
};

class ScoreNotification : public Notification {
public:
    void send(int value = 0) const override {
        std::cout << "Score increased by: " << value << " points\n";
    }
};

// game over notification not in use yet. only for demo purposes
class GameOverNotification : public Notification {
public:
    void send(int value = 0) const override {
        std::cout << "Game over! Try again..\n";
    }
};

#endif
