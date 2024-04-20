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

	public:
		Ray(sf::Vector2f direction, float length, sf::Vector2f startPoint);
		void setLength(float length);
		void updateEndPoint();
		void setStartPoint(sf::Vector2f startPoint);
		void draw(sf::RenderWindow& window);
};