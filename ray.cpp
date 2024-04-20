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
}

void Ray::setLength(float length)
{
	length = length;
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

void Ray::draw(sf::RenderWindow& window) 
{
	window.draw(rayLine);
}