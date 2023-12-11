#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <stdlib.h>

class AShape
{
public:

    std::string name;
    std::shared_ptr<sf::Shape> shape;
    sf::Vector2f speed;
    sf::Text text;
    bool isCollidedInThisFrame = false;

    AShape(std::string name, std::shared_ptr<sf::Shape> shape, sf::Vector2f speed)
        : name(name)
        , shape(shape)
        , speed(speed)
    {
    }

    ~AShape()
    {
        // RAII need to be done
    }

    void SetText(sf::Font& font, float size, sf::Color color)
    {
        sf::Vector2f initialShapePos = shape->getPosition();
        text.setFont(font);
        text.setCharacterSize(size);
        text.setString(name);

        float shiftCenterX = (shape->getLocalBounds().width / 2) - (text.getLocalBounds().width / 2);
        float shiftCenterY = (shape->getLocalBounds().height / 2) - (text.getLocalBounds().height / 2);
        text.setPosition(sf::Vector2f(initialShapePos.x + shiftCenterX, initialShapePos.y + shiftCenterY));
    }

    void Move()
    {
        shape->move(speed);
        text.move(speed);
    }

    void CheckCollideWithWindowX(float wWidth, float wHeight)
    {
        bool isCollidedX = (shape->getPosition().x <= 0)
            || (shape->getPosition().x + shape->getLocalBounds().width >= wWidth);

        if (isCollidedX) CollideX();
    }

    void CheckCollideWithWindowY(float wWidth, float wHeight)
    {
        bool isCollidedY = (shape->getPosition().y <= 0)
            || (shape->getPosition().y + shape->getLocalBounds().height >= wHeight);

        if (isCollidedY) CollideY();
    }

    void CheckCollideWithObject(std::vector<AShape>& shapes)
    {
        for (auto& otherShape : shapes)
        {
            if (otherShape.name.compare(name) == 0)
            {
                continue;
            }

            bool isCollided = shape->getPosition().x + shape->getLocalBounds().width > otherShape.shape->getPosition().x &&
                               shape->getPosition().x < otherShape.shape->getPosition().x + otherShape.shape->getLocalBounds().width &&
                               shape->getPosition().y + shape->getLocalBounds().height > otherShape.shape->getPosition().y &&
                               shape->getPosition().y < otherShape.shape->getPosition().y + otherShape.shape->getLocalBounds().height;

            if (isCollided)
            {
                // Check side collision
                float diffX = (shape->getPosition().x + shape->getLocalBounds().width / 2) - (otherShape.shape->getPosition().x + otherShape.shape->getLocalBounds().width / 2);
                float diffY = (shape->getPosition().y + shape->getLocalBounds().height / 2) - (otherShape.shape->getPosition().y + otherShape.shape->getLocalBounds().height / 2);

                float minDistX = shape->getLocalBounds().width / 2 + otherShape.shape->getLocalBounds().width / 2;
                float minDistY = shape->getLocalBounds().height / 2 + otherShape.shape->getLocalBounds().height / 2;

                float depthX = diffX > 0 ? minDistX - diffX : minDistX + diffX;
                float depthY = diffY > 0 ? minDistY - diffY : minDistY + diffY;

                if (abs(depthX) < abs(depthY))
                {
                    CollideX();
                }
                else
                {
                    CollideY();
                }

                return;
            }
        }
    }


    void CollideX()
    {
        speed.x *= -1.0f;
    }

    void CollideY()
    {
        speed.y *= -1.0f;
    }

    std::shared_ptr<sf::Shape> GetShape()
    {
        return shape;
    }

    sf::Text& GetText()
    {
        return text;
    }
};

int main(int argc, char * argv[])
{
    std::string configFileName = "config.txt";
    std::ifstream config(configFileName);

    // Initialize Variables
    std::vector<AShape> shapes = {};
    int wWidth = 1280;
    int wHeight = 720;
    int frameRate = 60;
    std::string fontFile;
    sf::Font font;
    int fontSize = 0;
    int fontR = 0;
    int fontG = 0;
    int fontB = 0;

    // Read config file
    std::string identifier;
    while (config >> identifier)
    {
        if (identifier == "Window")
        {
            config >> wWidth;
            config >> wHeight;
            config >> frameRate;

            std::cout << wWidth << " " << wHeight << "\n";
        }

        if (identifier == "Font")
        {
            config >> fontFile;
            config >> fontSize;
            config >> fontR;
            config >> fontG;
            config >> fontB;

            if (!font.loadFromFile(fontFile))
            {
                std::cout << "Cannot download font from file!\n";
                exit(-1);
            }

            std::cout << fontFile << " " << fontSize << " " << fontR << " " << fontG << " " << fontB << "\n";
        }

        if (identifier == "Circle")
        {
            std::string shapeName;
            config >> shapeName;

            float posX, posY, speedX, speedY = 0.0f;
            config >> posX;
            config >> posY;
            config >> speedX;
            config >> speedY;

            int r, g, b = 0;
            config >> r;
            config >> g;
            config >> b;

            float radius = 0.0f;
            config >> radius;

            std::shared_ptr<sf::Shape> circle = std::make_shared<sf::CircleShape>(radius);
            circle.get()->setFillColor(sf::Color(r, g, b));
            circle.get()->setPosition(sf::Vector2f(posX, posY));
            AShape aShape(shapeName, circle, sf::Vector2f(speedX, speedY));
            aShape.SetText(font, fontSize, sf::Color(fontR, fontG, fontB));
            shapes.push_back(aShape);

            std::cout << "created Circle " << shapeName << " " << posX << " " << posY << " " << speedX << " " << speedY << " " << r << " " << g << " " << b << " " << radius << "\n";
        }

        if (identifier == "Rectangle")
        {
            std::string shapeName;
            config >> shapeName;

            float posX, posY, speedX, speedY = 0.0f;
            config >> posX;
            config >> posY;
            config >> speedX;
            config >> speedY;

            int r, g, b = 0;
            config >> r;
            config >> g;
            config >> b;

            float width, height = 0.0f;
            config >> width;
            config >> height;

            std::shared_ptr<sf::Shape> circle = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
            circle.get()->setFillColor(sf::Color(r, g, b));
            circle.get()->setPosition(sf::Vector2f(posX, posY));
            AShape aShape(shapeName, circle, sf::Vector2f(speedX, speedY));
            aShape.SetText(font, fontSize, sf::Color(fontR, fontG, fontB));
            shapes.push_back(aShape);
            std::cout << "created Rectangle " << shapeName << " " << posX << " " << posY << " " << speedX << " " << speedY << " " << r << " " << g << " " << b << " " << width << " " << height << "\n";
        }
    }


    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Game Window");
    window.setFramerateLimit(frameRate);
    
    // Sample Drawing
    //sf::CircleShape circle(50);
    //circle.setFillColor(sf::Color(r, g, b));
    //circle.setPosition(100.0f, 100.0f);
    //float circleMoveSpeed = 0.5f; // pixel circle will move per frame
    // (in this case we set it to 60 so it will travel 0.5 pixel per frame so in 1 second, it will move 30 pixels)

    sf::Text text("Collision Test", font, 40);
    text.setPosition(wWidth - (float)text.getCharacterSize() * 14, 0);
    text.setFillColor(sf::Color(fontR, fontG, fontB));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            /*if (event.type == sf::Keyboard::G)
            {
                circleMoveSpeed *= -1.0f;
            }*/
        }

        // Logic
        for (auto& shape : shapes)
        {
            shape.Move();

            shape.CheckCollideWithWindowX(wWidth, wHeight);
            shape.CheckCollideWithWindowY(wWidth, wHeight);
            shape.CheckCollideWithObject(shapes);
        }

        // window rendering
        window.clear(sf::Color::Black);

        for (auto& shape : shapes)
        {
            window.draw(*shape.GetShape());
            window.draw(shape.GetText());
        }

        window.draw(text);
        window.display();
    }

    return 0;
}
