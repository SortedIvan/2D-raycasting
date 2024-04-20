#include "ray.hpp"

Ray::Ray(sf::Vector2f direction, float length, sf::Vector2f startPoint)
{
	rayLine = sf::VertexArray(sf::Lines, 2);

	rayLine[0].position = startPoint;
	rayLine[1].position = startPoint + direction * length;

	this->startPoint = rayLine[0].position;
	endPoint = rayLine[1].position;

	this->length = length;

	this->direction = direction;

	rayLine[0].color = sf::Color(255, 165, 0);
	rayLine[1].color = sf::Color::Yellow;
}

void Ray::setLength(float length)
{
	this->length = length;
}

void Ray::setStartPoint(sf::Vector2f startPoint)
{
	this->startPoint = startPoint;
	rayLine[0].position = startPoint;
}

// must be called after start point/length updates
void Ray::updateEndPoint()
{
	rayLine[1].position = startPoint + direction * length;
}

void Ray::setEndPoint(sf::Vector2f newPoint)
{
	rayLine[1].position = newPoint;
	endPoint = newPoint;
}

void Ray::draw(sf::RenderWindow& window) 
{
	window.draw(rayLine);
}


sf::Vector2f Ray::getDirection()
{
	return direction;
}

sf::Vector2f Ray::getStartPoint()
{
	return startPoint;
}

sf::Vector2f Ray::getEndPoint()
{
	return endPoint;
}

float Ray::getLength()
{
	return length;
}