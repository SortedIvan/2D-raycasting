#pragma once

#include "SFML/Graphics.hpp"

class Ray
{
	private:
		sf::Vector2f direction;
		float length = 0.f;
		sf::VertexArray rayLine;

		sf::Vector2f startPoint;
		sf::Vector2f endPoint;

		bool isIntersectingSegment = false;

	public:
		Ray(sf::Vector2f direction, float length, sf::Vector2f startPoint);
		void setLength(float length);
		void updateEndPoint();
		void setStartPoint(sf::Vector2f startPoint);
		void draw(sf::RenderWindow& window);
		void setEndPoint(sf::Vector2f newPoint);

		sf::Vector2f getDirection();
		sf::Vector2f getStartPoint();
		sf::Vector2f getEndPoint();
		float getLength();
};