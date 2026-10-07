#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

int main()
{
    // Create our window and world with gravity 0,1
    RenderWindow window(VideoMode(800, 600), "Bounce");
    World world(Vector2f(0, 1));

    // Create the ball
    PhysicsCircle ball;
    ball.setCenter(Vector2f(100, 300));
    ball.setRadius(20);
    world.AddPhysicsBody(ball);

    // Give the ball an initial velocity toward the center box
    ball.applyImpulse(Vector2f(0.5, -0.1));

    // Create the floor
    PhysicsRectangle floor;
    floor.setSize(Vector2f(800, 20));
    floor.setCenter(Vector2f(400, 590));
    floor.setStatic(true);
    world.AddPhysicsBody(floor);

    // Create the ceiling
    PhysicsRectangle ceiling;
    ceiling.setSize(Vector2f(800, 20));
    ceiling.setCenter(Vector2f(400, 10));
    ceiling.setStatic(true);
    world.AddPhysicsBody(ceiling);

    // Create the left wall
    PhysicsRectangle leftWall;
    leftWall.setSize(Vector2f(20, 560));
    leftWall.setCenter(Vector2f(10, 300));
    leftWall.setStatic(true);
    world.AddPhysicsBody(leftWall);

    // Create the right wall
    PhysicsRectangle rightWall;
    rightWall.setSize(Vector2f(20, 560));
    rightWall.setCenter(Vector2f(790, 300));
    rightWall.setStatic(true);
    world.AddPhysicsBody(rightWall);

    // Create the center obstacle
    PhysicsRectangle center;
    center.setSize(Vector2f(100, 100));
    center.setCenter(Vector2f(400, 300));
    center.setStatic(true);
    world.AddPhysicsBody(center);

    // Collision counters
    int thudCount(0);
    int bang(0);

    // Lambda callback for the four outer walls ("thud")
    auto thudCallback = [&thudCount](PhysicsBodyCollisionResult result) {
        cout << "thud " << thudCount << endl;
        thudCount++;
        };
    floor.onCollision = thudCallback;
    ceiling.onCollision = thudCallback;
    leftWall.onCollision = thudCallback;
    rightWall.onCollision = thudCallback;

    // Separate lambda callback for the center box ("bang")
    center.onCollision = [&bang](PhysicsBodyCollisionResult result) {
        cout << "bang " << bang << endl;
        bang++;
        if (bang >= 3) {
            exit(0);
        }
        };

    Clock clock;
    Time lastTime(clock.getElapsedTime());
    while (true) {
        // calculate MS since last frame
        Time currentTime(clock.getElapsedTime());
        Time deltaTime(currentTime - lastTime);
        int deltaTimeMS(deltaTime.asMilliseconds());
        if (deltaTimeMS > 0) {
            world.UpdatePhysics(deltaTimeMS);
            lastTime = currentTime;
        }

        window.clear(Color(0, 0, 0));
        window.draw(ball);
        window.draw(floor);
        window.draw(ceiling);
        window.draw(leftWall);
        window.draw(rightWall);
        window.draw(center);
        window.display();
    }
}