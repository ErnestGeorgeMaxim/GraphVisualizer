#include "Node.hpp"
#define M_PI 3.14159f

class Edge {
public:
    Edge(Node* start, Node* end, bool oriented = false, bool bidirectional = false)
        : m_start(start), m_end(end), m_oriented(oriented), m_bidirectional(bidirectional) {
        m_line.setPrimitiveType(sf::Lines);
        m_line.resize(2);
        m_line[0].color = sf::Color(235, 64, 52);
        m_line[1].color = sf::Color(235, 64, 52);

        m_arrow.setPointCount(3);
        const float arrowSize = 10.0f;
        m_arrow.setPoint(0, sf::Vector2f(0, -arrowSize));
        m_arrow.setPoint(1, sf::Vector2f(-arrowSize, arrowSize));
        m_arrow.setPoint(2, sf::Vector2f(arrowSize, arrowSize));
        m_arrow.setFillColor(sf::Color(235, 64, 52));
        m_arrow.setOrigin(0, 0);

        updateLinePositions();
    }

    void setOriented(bool oriented) {
        m_oriented = oriented;
        updateLinePositions();
    }

    void setBidirectional(bool bidirectional) {
        m_bidirectional = bidirectional;
        updateLinePositions();
    }

    bool isBidirectional() const { return m_bidirectional; }

    void updateLinePositions() {
        if (!m_start || !m_end) return;

        sf::Vector2f startPos = m_start->getPosition();
        sf::Vector2f endPos = m_end->getPosition();

        sf::Vector2f direction = endPos - startPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0) {
            direction.x /= length;
            direction.y /= length;

            float radius = m_start->getCircle().getRadius();
            sf::Vector2f adjustedStart = startPos + (direction * radius);
            sf::Vector2f adjustedEnd = endPos - (direction * radius);


            if (m_oriented) {
                adjustedEnd -= direction * 15.0f;
            }

            m_line[0].position = adjustedStart;
            m_line[1].position = adjustedEnd;

            if (m_oriented) {

                float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
                m_arrow.setPosition(adjustedEnd);
                m_arrow.setRotation(angle + 90);
            }
        }
    }

    void drawEdge(sf::RenderWindow& window) {
        window.draw(m_line);
        if (m_oriented) {
            window.draw(m_arrow);
        }
    }

    void drawReverseArrow(sf::RenderWindow& window) {
        if (m_bidirectional) {
            sf::Vector2f startPos = m_end->getPosition();
            sf::Vector2f endPos = m_start->getPosition();

            sf::Vector2f direction = endPos - startPos;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length > 0) {
                direction /= length;

                float radius = m_end->getCircle().getRadius();
                sf::Vector2f adjustedEnd = endPos - (direction * (radius + 15.0f));

                float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
                m_arrow.setPosition(adjustedEnd);
                m_arrow.setRotation(angle + 90);
                window.draw(m_arrow);
            }
        }
    }

    Node* getFirst() const { return m_start; }
    Node* getSecond() const { return m_end; }

private:
    Node* m_start;
    Node* m_end;
    sf::VertexArray m_line;
    sf::ConvexShape m_arrow;
    bool m_oriented;
    bool m_bidirectional;
};