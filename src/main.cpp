#include <SFML/Graphics.hpp>
#include <iostream>
#include "Graph.hpp"

const float MIN_DISTANCE = 50.f;
const float MOVE_SPEED = 0.3f;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualization");
    window.setFramerateLimit(60);


    sf::Font font;
    if (!font.loadFromFile("../Fonts/Arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    Graph graph(window, font, "D:/clion_projects/GraphVisualizerr/adjacency_matrix.txt");

    Node* draggedNode = nullptr;
    Node* selectedNode = nullptr;
    bool isDragging = false;
    bool isDrawingMode = false;


    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(14);
    statusText.setFillColor(sf::Color::Black);
    statusText.setPosition(10.f, window.getSize().y - 30.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {

                        if (selectedNode) {
                            selectedNode->setHighlighted(false);
                            selectedNode = nullptr;
                        }
                        window.close();
                    }

                    else if (event.key.code == sf::Keyboard::D) {
                        isDrawingMode = !isDrawingMode;

                        if (selectedNode) {
                            selectedNode->setHighlighted(false);
                            selectedNode = nullptr;
                        }
                        graph.updateModeText(isDrawingMode);
                        std::cout << (isDrawingMode ? "Edge Drawing Mode" : "Node Moving Mode") << std::endl;
                    }
                    break;

                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));


                        if (graph.handleToggleClick(mousePos)) {
                            break;
                        }

                        bool nodeClicked = false;

                        for (const auto& node : graph.getNodes()) {
                            if (node->containsPoint(mousePos)) {
                                nodeClicked = true;

                                if (isDrawingMode) {
                                    if (!selectedNode) {

                                        selectedNode = node.get();
                                        selectedNode->setHighlighted(true);
                                    } else if (node.get() != selectedNode) {

                                        graph.addEdge(selectedNode, node.get());

                                        selectedNode->setHighlighted(false);
                                        selectedNode = nullptr;
                                    }
                                } else {

                                    draggedNode = node.get();
                                    isDragging = true;
                                    draggedNode->setHighlighted(true);
                                }
                                break;
                            }
                        }


                        if (!nodeClicked && !isDrawingMode) {
                            graph.addNode(mousePos);
                        }
                    }

                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        if (selectedNode) {
                            selectedNode->setHighlighted(false);
                            selectedNode = nullptr;
                        }
                    }
                    break;
                }

                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (isDragging && draggedNode) {
                            draggedNode->setHighlighted(false);
                            draggedNode = nullptr;
                            isDragging = false;
                        }
                    }
                    break;

                case sf::Event::MouseMoved:
                    if (isDragging && draggedNode) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        bool canMove = true;


                        for (const auto& node : graph.getNodes()) {
                            if (node.get() != draggedNode) {
                                sf::Vector2f nodePos = node->getPosition();
                                float dx = mousePos.x - nodePos.x;
                                float dy = mousePos.y - nodePos.y;
                                float distance = std::sqrt(dx * dx + dy * dy);

                                if (distance < MIN_DISTANCE) {
                                    canMove = false;
                                    break;
                                }
                            }
                        }


                        if (canMove) {
                            sf::Vector2f currentPos = draggedNode->getPosition();
                            sf::Vector2f direction = mousePos - currentPos;
                            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                            if (length > 0) {
                                direction /= length;
                                sf::Vector2f newPos = currentPos + direction * MOVE_SPEED * length;


                                newPos.x = std::max(20.f, std::min(newPos.x, window.getSize().x - 20.f));
                                newPos.y = std::max(20.f, std::min(newPos.y, window.getSize().y - 20.f));

                                draggedNode->setPosition(newPos);
                                graph.updateEdges();
                            }
                        }
                    }
                    break;
            }
        }


        window.clear(sf::Color::White);


        graph.draw();

        window.display();
    }

    return 0;
}