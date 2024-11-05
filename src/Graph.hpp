#pragma once
#include "Node.hpp"
#include "Edge.hpp"
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <set>
#include <algorithm>>

class Graph {
public:
    Graph(sf::RenderWindow& window, const sf::Font& font, const std::string& matrixFilePath)
        : m_window(&window), m_font(font), m_nextNodeId(0),
          m_matrixFilePath(matrixFilePath), m_isOriented(false) {
        initializeUI();
        updateAdjacencyMatrix();
    }

    void initializeUI() {

        m_orientationToggle.setRadius(15.f);
        m_orientationToggle.setPosition(10.f, 10.f);
        m_orientationToggle.setFillColor(sf::Color::White);
        m_orientationToggle.setOutlineColor(sf::Color::Black);
        m_orientationToggle.setOutlineThickness(2.f);


        m_toggleText.setFont(m_font);
        m_toggleText.setString("U");
        m_toggleText.setCharacterSize(20);
        m_toggleText.setFillColor(sf::Color::Black);
        centerText(m_toggleText, m_orientationToggle.getPosition() +
            sf::Vector2f(m_orientationToggle.getRadius(), m_orientationToggle.getRadius()));


        m_helpText.setFont(m_font);
        m_helpText.setString("Click to toggle orientation (U/D)");
        m_helpText.setCharacterSize(17);
        m_helpText.setFillColor(sf::Color::Black);
        m_helpText.setPosition( 45.f, 14.f);


        m_modeText.setFont(m_font);
        m_modeText.setCharacterSize(17);
        m_modeText.setFillColor(sf::Color::Black);
        m_modeText.setPosition(10.f, 570.f);
        updateModeText(false);
    }

    bool handleToggleClick(const sf::Vector2f& mousePos) {
        sf::Vector2f togglePos = m_orientationToggle.getPosition();
        sf::Vector2f toggleCenter = togglePos +
            sf::Vector2f(m_orientationToggle.getRadius(), m_orientationToggle.getRadius());

        float distance = std::sqrt(
            std::pow(mousePos.x - toggleCenter.x, 2) +
            std::pow(mousePos.y - toggleCenter.y, 2)
        );

        if (distance <= m_orientationToggle.getRadius()) {
            toggleOrientation();
            return true;
        }
        return false;
    }

    void updateModeText(bool isDrawingMode) {
        m_modeText.setString(isDrawingMode ? "Drawing Mode (press D to change)" : "Movement Mode (press D to change)");
    }

    void addNode(const sf::Vector2f& position) {
        const float minDistance = 44.0f;

        if (isPositionValid(position, minDistance)) {
            auto newNode = std::make_unique<Node>(++m_nextNodeId, position, m_font);
            m_nodes.push_back(std::move(newNode));
            updateAdjacencyMatrix();
        }
    }

    void addEdge(Node* start, Node* end) {
        if (!nodeExists(start) || !nodeExists(end)) return;


        auto existingEdge = findEdge(start, end);
        auto reverseEdge = findEdge(end, start);

        if (m_isOriented) {
            if (existingEdge) {

                return;
            } else if (reverseEdge) {

                auto newEdge = std::make_unique<Edge>(start, end, true);

                reverseEdge->setBidirectional(true);
                newEdge->setBidirectional(true);
                m_edges.push_back(std::move(newEdge));
            } else {

                auto newEdge = std::make_unique<Edge>(start, end, true);
                m_edges.push_back(std::move(newEdge));
            }
        } else {

            if (existingEdge || reverseEdge) {

                return;
            } else {

                auto newEdge = std::make_unique<Edge>(start, end, false);
                m_edges.push_back(std::move(newEdge));
            }
        }

        updateAdjacencyMatrix();
    }

    void toggleOrientation() {
        m_isOriented = !m_isOriented;
        updateToggleAppearance();

        if (m_isOriented) {

            for (const auto& edge : m_edges) {
                edge->setOriented(true);


                auto reverseEdge = findEdge(edge->getSecond(), edge->getFirst());
                if (reverseEdge) {
                    edge->setBidirectional(true);
                    reverseEdge->setBidirectional(true);
                } else {
                    edge->setBidirectional(false);
                }
            }
        } else {

            std::vector<std::unique_ptr<Edge>> newEdges;
            std::set<std::pair<Node*, Node*>> processedPairs;

            for (auto& edge : m_edges) {
                Node* first = edge->getFirst();
                Node* second = edge->getSecond();


                auto orderedPair = std::make_pair(
                    std::min(first, second),
                    std::max(first, second)
                );


                if (processedPairs.find(orderedPair) == processedPairs.end()) {
                    processedPairs.insert(orderedPair);


                    edge->setOriented(false);
                    edge->setBidirectional(false);
                    newEdges.push_back(std::move(edge));
                }
            }


            m_edges = std::move(newEdges);
        }


        updateEdges();
        updateAdjacencyMatrix();
    }


    void draw() {

        for (const auto& edge : m_edges) {
            edge->drawEdge(*m_window);


            if (edge->isBidirectional()) {
                edge->drawReverseArrow(*m_window);
            }
        }


        for (const auto& node : m_nodes) {
            node->drawNode(*m_window);
        }


        m_window->draw(m_orientationToggle);
        m_window->draw(m_toggleText);
        m_window->draw(m_helpText);
        m_window->draw(m_modeText);
    }

    void updateEdges() {
        for (const auto& edge : m_edges) {
            edge->updateLinePositions();
        }
    }

    const std::vector<std::unique_ptr<Node>>& getNodes() const {
        return m_nodes;
    }

private:
    bool m_isOriented;
    sf::RenderWindow* m_window;
    sf::Font m_font;
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Edge>> m_edges;
    std::vector<std::vector<int>> m_adjacencyMatrix;
    uint8_t m_nextNodeId;
    std::string m_matrixFilePath;


    sf::CircleShape m_orientationToggle;
    sf::Text m_toggleText;
    sf::Text m_helpText;
    sf::Text m_modeText;

    void centerText(sf::Text& text, const sf::Vector2f& position) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f,
                      bounds.top + bounds.height / 2.0f);
        text.setPosition(position);
    }

    void updateToggleAppearance() {
        m_orientationToggle.setFillColor(
            m_isOriented ? sf::Color(200, 200, 200) : sf::Color::White);
        m_toggleText.setString(m_isOriented ? "D" : "U");
        centerText(m_toggleText, m_orientationToggle.getPosition() +
            sf::Vector2f(m_orientationToggle.getRadius(), m_orientationToggle.getRadius()));
    }

    bool nodeExists(Node* node) const {
        return std::any_of(m_nodes.begin(), m_nodes.end(),
            [node](const auto& n) { return n.get() == node; });
    }

    bool edgeExists(Node* start, Node* end) const {
        return std::any_of(m_edges.begin(), m_edges.end(),
            [start, end, this](const auto& edge) {
                if (m_isOriented) {

                    return edge->getFirst() == start && edge->getSecond() == end;
                } else {

                    return (edge->getFirst() == start && edge->getSecond() == end) ||
                           (edge->getFirst() == end && edge->getSecond() == start);
                }
            });
    }


    bool isPositionValid(const sf::Vector2f& newPos, float minDistance) const {
        return std::all_of(m_nodes.begin(), m_nodes.end(),
            [&newPos, minDistance](const auto& node) {
                sf::Vector2f pos = node->getPosition();
                float dx = newPos.x - pos.x;
                float dy = newPos.y - pos.y;
                return std::sqrt(dx * dx + dy * dy) >= minDistance;
            });
    }

    Edge* findEdge(Node* start, Node* end) {
        auto it = std::find_if(m_edges.begin(), m_edges.end(),
            [start, end](const auto& edge) {
                return edge->getFirst() == start && edge->getSecond() == end;
            });
        return it != m_edges.end() ? it->get() : nullptr;
    }

    void cleanupDuplicateEdges() {
        std::vector<std::unique_ptr<Edge>> newEdges;
        std::set<std::pair<Node*, Node*>> processedPairs;

        for (auto& edge : m_edges) {
            Node* first = edge->getFirst();
            Node* second = edge->getSecond();


            auto pair1 = std::make_pair(std::min(first, second), std::max(first, second));

            if (processedPairs.find(pair1) == processedPairs.end()) {
                processedPairs.insert(pair1);
                edge->setOriented(false);
                newEdges.push_back(std::move(edge));
            }
        }

        m_edges = std::move(newEdges);
    }

    int getNodeIndex(Node* node) const {
        for (size_t i = 0; i < m_nodes.size(); ++i) {
            if (m_nodes[i].get() == node) return i;
        }
        return -1;
    }

    void updateAdjacencyMatrix() {
        size_t n = m_nodes.size();
        m_adjacencyMatrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));

        for (const auto& edge : m_edges) {
            Node* start = edge->getFirst();
            Node* end = edge->getSecond();

            int startIdx = getNodeIndex(start);
            int endIdx = getNodeIndex(end);

            if (startIdx != -1 && endIdx != -1) {
                if (m_isOriented) {

                    m_adjacencyMatrix[startIdx][endIdx] = 1;

                    if (edge->isBidirectional() || findEdge(end, start)) {
                        m_adjacencyMatrix[endIdx][startIdx] = 1;
                    }
                } else {

                    m_adjacencyMatrix[startIdx][endIdx] = 1;
                    m_adjacencyMatrix[endIdx][startIdx] = 1;
                }
            }
        }
        writeMatrixToFile();
    }


    void writeMatrixToFile() {
        std::ofstream file(m_matrixFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << m_matrixFilePath << std::endl;
            return;
        }


        file << m_nodes.size() << "\n";


        for (const auto& node : m_nodes) {
            file << int(node->getId()) << " ";
        }
        file << "\n";


        for (const auto& row : m_adjacencyMatrix) {
            for (int val : row) {
                file << val << " ";
            }
            file << "\n";
        }
    }
};