
#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <iostream>
#include <utility>
#include <cmath>

std::pair<bool, sf::Vector2f> raycastLine(sf::VertexArray& lineSeg, sf::Vector2f dirVector, sf::Vector2f startPos, float lengthOfRay);
void rotateVector(sf::Vector2f& vectorToRotate, float radians, sf::Vector2f pointToRotateAround);
std::vector<sf::Vector2f> generateDirectionVectors();


# define M_PI  3.14159

int main() 
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "2D-Raycasting");

    sf::Event e;

    float LIGHT_RADIUS = 25.f;

    sf::CircleShape light;
    light.setRadius(LIGHT_RADIUS);
    light.setOrigin(LIGHT_RADIUS, LIGHT_RADIUS);
    
    sf::CircleShape interesectionPointTest;
    interesectionPointTest.setRadius(5.f);
    interesectionPointTest.setOrigin(5.f, 5.f);

    std::vector<sf::Vector2f> directions = generateDirectionVectors();

    for (int i = 0; i < directions.size(); i++)
    {
        std::cout << "x: " << directions[i].x << ", y: " << directions[i].y << std::endl;
    }

    sf::Vector2f dirVectorTest(1.f, -1.f);

    sf::VertexArray vectorTestVisual(sf::PrimitiveType::Lines, 2);
    vectorTestVisual[0].color = sf::Color::Green;
    vectorTestVisual[1].color = sf::Color::Green;

    sf::VertexArray lineSeg(sf::PrimitiveType::Lines, 2);
    lineSeg[0].position = sf::Vector2f(400.f, 400.f);
    lineSeg[1].position = sf::Vector2f(800.f, 600.f);

    // Main loop
    while (window.isOpen())
    {
        std::pair<bool, sf::Vector2f> raycastRes = raycastLine(lineSeg, dirVectorTest, light.getPosition(), 100.f);
        if (raycastRes.first)
        {
            interesectionPointTest.setPosition(raycastRes.second);
        }

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

            light.setPosition(mousePos);

            vectorTestVisual[0].position = light.getPosition() + dirVectorTest * LIGHT_RADIUS / 2.f;
            vectorTestVisual[1].position = light.getPosition() + dirVectorTest * 10.f * LIGHT_RADIUS / 2.f;

            // Clear the window
            window.clear(sf::Color::Black);

            // draw
            window.draw(light);
            window.draw(lineSeg);
            window.draw(vectorTestVisual);

            if (raycastRes.first)
            {
                window.draw(interesectionPointTest);
            }
            

            // display
            window.display();
        }
    }
}

std::pair<bool, sf::Vector2f> raycastLine(sf::VertexArray& lineSeg, sf::Vector2f dirVector, sf::Vector2f startPos, float lengthOfRay)
{
    // the parametric equations for the intersection point that we have are:
    // P = startPos + (dirVector) * t2
    // P = lineSeg[0].position + AB * t1

    // AB is the vector representing the line segment
    // AS is the vector representing the connection from our viewpoint to the line segment
    sf::Vector2f AB = lineSeg[1].position - lineSeg[0].position;
    sf::Vector2f AS = startPos - lineSeg[0].position;

    float determinant = AB.x * dirVector.y - AB.y * dirVector.x;

    // if determinant is 0, then the lines are parallel and can't have an intersection point
    if (determinant == 0)
    {
        return std::make_pair(false, sf::Vector2f(0.f,0.f));
    }

    // if t1 and t2 are > 1, then the segment is scaled and we don't need that
    // since we simply solve for t2 and t1, it will always return a value, but we only need
    // the specific t1 and t2 when they are 0 <= t1,t2 <= 1

    float t1 = (dirVector.y * AS.x - dirVector.x * AS.y) / determinant;
    float t2 = - (-AB.y * AS.x + AB.x * AS.y) / determinant;

    if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= lengthOfRay)
    {
        return std::make_pair(true, dirVector * t2 + startPos);
    }
    
    return std::make_pair(false, sf::Vector2f(0.f, 0.f));
}

std::vector<sf::Vector2f> generateDirectionVectors()
{
    std::vector<sf::Vector2f> directions;
    sf::Vector2f baseVector(1, 0); // basically x axis
    
    for (int i = 0; i < 361.f; i++)
    {
        rotateVector(baseVector, (i * (M_PI / 180)), sf::Vector2f(0, 0));
        directions.push_back(baseVector);
    }

    return directions;

}

void rotateVector(sf::Vector2f& vectorToRotate, float radians, sf::Vector2f pointToRotateAround)
{
    //float degrees = radians * (180 / M_PI);

    sf::Vector2f offsetPoint(vectorToRotate - pointToRotateAround);

    // simple rotation matrix 
    //[cos  sin ]
   // [-sin cos ]

    sf::Vector2f rotatedVector(
        offsetPoint.x * std::cos(radians) - offsetPoint.y * std::sin(radians),
        offsetPoint.x * std::sin(radians) + offsetPoint.y * std::cos(radians)
    );

    vectorToRotate = rotatedVector + pointToRotateAround;
}