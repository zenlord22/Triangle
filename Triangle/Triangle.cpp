#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rotating Triangle");

    // Set the frame rate limit
    window.setFramerateLimit(60);

    // Define the triangle shape
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0, 0));
    triangle.setPoint(1, sf::Vector2f(100, 0));
    triangle.setPoint(2, sf::Vector2f(50, 100));
    triangle.setFillColor(sf::Color::Green);
    triangle.setOrigin(50, 50); // Set origin to the center of the triangle
    triangle.setPosition(400, 300); // Position the triangle at the center of the window

    float angle = 0.0f; // Initial rotation angle

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the screen
        window.clear();

        // Rotate the triangle
        angle += 1.0f; // Increase the angle
        triangle.setRotation(angle);

        // Draw the triangle
        window.draw(triangle);

        // Update the window
        window.display();
    }

    return 0;
}
