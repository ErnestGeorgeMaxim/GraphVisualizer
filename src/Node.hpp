#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

class Node {
public:
    Node(uint8_t id, sf::Vector2f pos, const sf::Font& font)
        : m_id(id), m_position(pos) {

        m_circle.setRadius(20.f);
        m_circle.setFillColor(sf::Color(52, 235, 168));
        m_circle.setOutlineColor(sf::Color::Black);
        m_circle.setOutlineThickness(1.f);
        m_circle.setOrigin(m_circle.getRadius(), m_circle.getRadius());
        m_circle.setPosition(m_position);


        m_text.setFont(font);
        m_text.setString(std::to_string(m_id));
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color::Black);


        sf::FloatRect textBounds = m_text.getLocalBounds();
        m_text.setOrigin(textBounds.left + textBounds.width / 2,
                        textBounds.top + textBounds.height / 2);
        m_text.setPosition(m_circle.getPosition());
    }

    bool containsPoint(const sf::Vector2f& point) const {
        float dx = point.x - m_position.x;
        float dy = point.y - m_position.y;
        return (dx * dx + dy * dy) <= (m_circle.getRadius() * m_circle.getRadius());
    }

    sf::Vector2f getOrigin() const { return m_circle.getOrigin(); }
    uint8_t getId() const { return m_id; }
    sf::Vector2f getPosition() const { return m_circle.getPosition(); }
    sf::CircleShape& getCircle() { return m_circle; }

    void setPosition(const sf::Vector2f& newPos) {
        m_position = newPos;
        m_circle.setPosition(m_position);
        m_text.setPosition(m_position);
    }

    void drawNode(sf::RenderWindow& window) {
        window.draw(m_circle);
        window.draw(m_text);
    }

    void setHighlighted(bool highlighted) {
        m_circle.setFillColor(highlighted ?
            sf::Color(52, 235, 170, 200) : sf::Color(52, 235, 168));
        m_circle.setOutlineThickness(highlighted ? 2.f : 1.f);
    }

private:
    uint8_t m_id;
    sf::Text m_text;
    sf::CircleShape m_circle;
    sf::Vector2f m_position;
};