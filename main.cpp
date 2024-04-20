
#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <iostream>
#include <utility>
#include <cmath>
#include "ray.hpp"

# define M_PI  3.14159

std::pair<bool, sf::Vector2f> raycastLine(sf::VertexArray& lineSeg, sf::Vector2f dirVector, sf::Vector2f startPos, float lengthOfRay);
void rotateVector(sf::Vector2f& vectorToRotate, float radians, sf::Vector2f pointToRotateAround);
std::vector<sf::Vector2f> generateDirectionVectors();
void createRayObjects(std::vector<Ray>& rayVect, std::vector<sf::Vector2f>& directions, float initialLength, sf::Vector2f startPoint);
void drawAllRays(std::vector<Ray>& rayObjects, sf::RenderWindow& window);
void updateAllRayPositions(std::vector<Ray>& rayObjects, sf::Vector2f lightPos);
void checkCollisionRays(std::vector<sf::VertexArray>& lineSegArr, std::vector<Ray>& rays, float lengthOfRayOriginal, sf::Vector2f lightPos);
void drawAllLineSegments(std::vector<sf::VertexArray>& lineSegments, sf::RenderWindow& window);


int main() 
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "2D-Raycasting");

    sf::Event e;

    float LIGHT_RADIUS = 25.f;
    float LIGHT_RANGE = 400.f;

    sf::CircleShape light;
    light.setRadius(LIGHT_RADIUS);
    light.setOrigin(LIGHT_RADIUS, LIGHT_RADIUS);

    /*
        Instantiate all directions
        Currently, it is set to have a direction every 5 degrees over 2PI
    */
    
    std::vector<sf::Vector2f> directions = generateDirectionVectors();
    std::vector<Ray> rays;

    // Create all of the ray objects that will be shooting out of the light source
    createRayObjects(rays, directions, LIGHT_RANGE, light.getPosition());
    
    std::vector<sf::VertexArray> lineSegments;
    
    sf::VertexArray lineSeg(sf::PrimitiveType::Lines, 2);
    lineSeg[0].position = sf::Vector2f(400.f, 400.f);
    lineSeg[1].position = sf::Vector2f(800.f, 600.f);
    
    // add an initial line segment
    lineSegments.push_back(lineSeg);

    // Main loop
    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            if (e.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
                light.setPosition(mousePos);
                updateAllRayPositions(rays, light.getPosition());
            }
            
            // Clear the window
            window.clear(sf::Color::Black);

            // draw
            window.draw(light);
            drawAllLineSegments(lineSegments, window);
            drawAllRays(rays, window);

            // display
            window.display();
        }

        checkCollisionRays(lineSegments, rays, LIGHT_RANGE, light.getPosition());
    }
}

void createRayObjects(std::vector<Ray>& rayVect, std::vector<sf::Vector2f>& directions, float initialLength, sf::Vector2f startPoint)
{
    for (int i = 0; i < directions.size(); i++)
    {
        Ray ray(directions[i], initialLength, startPoint);
        rayVect.push_back(ray);
    }
}

void drawAllRays(std::vector<Ray>& rayObjects, sf::RenderWindow& window)
{
    for (int i = 0; i < rayObjects.size(); i++)
    {
        rayObjects[i].draw(window);
    }
} 

void updateAllRayPositions(std::vector<Ray>& rayObjects, sf::Vector2f lightPos)
{
    for (int i = 0; i < rayObjects.size(); i++)
    {
        rayObjects[i].setStartPoint(lightPos);
        rayObjects[i].updateEndPoint();
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

void checkCollisionRays(std::vector<sf::VertexArray>& lineSegArr, std::vector<Ray>& rays, float lengthOfRayOriginal, sf::Vector2f lightPos)
{
    for (int k = 0; k < lineSegArr.size(); k++)
    {
        sf::Vector2f AB = lineSegArr[k][1].position - lineSegArr[k][0].position;
        sf::Vector2f AS = lightPos - lineSegArr[k][0].position;

        for (int i = 0; i < rays.size(); i++)
        {
            float determinant = AB.x * rays[i].getDirection().y - AB.y * rays[i].getDirection().x;

            // if determinant is 0, then the lines are parallel and can't have an intersection point
            if (determinant == 0)
            {
                // reset ray to its full length
                rays[i].setLength(lengthOfRayOriginal);
                rays[i].updateEndPoint();
                continue;
            }

            float t1 = (rays[i].getDirection().y * AS.x - rays[i].getDirection().x * AS.y) / determinant;
            float t2 = -(-AB.y * AS.x + AB.x * AS.y) / determinant;

            if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= lengthOfRayOriginal)
            {
                std::cout << "?" << std::endl;
                rays[i].setLength(t2);
                rays[i].updateEndPoint();
            }
            else
            {
                rays[i].setLength(lengthOfRayOriginal);
                rays[i].updateEndPoint();
            }
        }
    }
}

std::vector<sf::Vector2f> generateDirectionVectors()
{
    std::vector<sf::Vector2f> directions;
    sf::Vector2f baseVector(1, 0); // basically x axis

    for (int i = 0; i < 360; ++i) // Loop through 360 degrees
    {   
        if (i % 5 == 0)
        {
            float angle = i * (M_PI / 180);
            float x = std::cos(angle);
            float y = std::sin(angle);
            sf::Vector2f direction(x, y);

            directions.push_back(direction);
        }
    }

    return directions;
}

void rotateVector(sf::Vector2f& vectorToRotate, float radians, sf::Vector2f pointToRotateAround)
{
    //float degrees = radians * (180 / M_PI);

    sf::Vector2f offsetPoint(vectorToRotate - pointToRotateAround);

    // simple rotation matrix 
    //[cos  sin]
   // [-sin cos]

    sf::Vector2f rotatedVector(
        offsetPoint.x * std::cos(radians) - offsetPoint.y * std::sin(radians),
        offsetPoint.x * std::sin(radians) + offsetPoint.y * std::cos(radians)
    );

    vectorToRotate = rotatedVector + pointToRotateAround;
}

void drawAllLineSegments(std::vector<sf::VertexArray>& lineSegments, sf::RenderWindow& window)
{
    for (int i = 0; i < lineSegments.size(); i++)
    {
        window.draw(lineSegments[i]);
    }
}