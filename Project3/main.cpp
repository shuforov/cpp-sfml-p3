#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <variant>

class Configs
{
    int windowWidthSize;
    int windowHeightSize;
    std::string fontPath;
    int fontSize;
    sf::Color fontColor;

public:
    sf::Font fontNode;
    Configs()
        : windowWidthSize(800)
        , windowHeightSize(600)
        , fontPath("fonts/arial.ttf")
        , fontSize(16)
        , fontColor(sf::Color::White)
    {
        if (!fontNode.loadFromFile(fontPath))
        {
            throw std::invalid_argument("Font not loaded");
        }
    }
    void setWindowWidthSize(int widthSize)
    {
        windowWidthSize = widthSize;
    }

    void setWindowHeightSize(int heightSize)
    {
        windowHeightSize = heightSize;
    }

    void setFontPath(std::string pathName)
    {
        fontPath = pathName;
        fontNode.loadFromFile(pathName);
    }

    void setFontSize(int sizeNumber)
    {
        fontSize = sizeNumber;
    }

    void setFontColor(sf::Color colorPalet)
    {
        fontColor = colorPalet;
    }

    const int getWidth()
    {
        return windowWidthSize;
    }

    const int getHeight()
    {
        return windowHeightSize;
    }

    const std::string getFontPath()
    {
        return fontPath;
    }

    const sf::Color getFontColor()
    {
        return fontColor;
    }

    const int getFontSize()
    {
        return fontSize;
    }
};

using ShapeVariant = std::variant<sf::CircleShape, sf::RectangleShape>;

class ShapeNode
{
public:
    virtual ShapeVariant getNode() const = 0;
    virtual sf::Vector2f getSpeed() const = 0;
    virtual sf::Text getTextNode() const = 0;
    virtual sf::Vector2f getShapeCenterPosition() const = 0;
    virtual void moveNode(const sf::Vector2f& delta) = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual void setSpeed(const sf::Vector2f& speed) = 0;
    virtual ~ShapeNode() = default;
};

class CircleShapeNode : public ShapeNode
{
    std::string m_name;
    sf::Vector2f m_speed;
    sf::Vector2f m_initPosition;
    float m_shapeSize;

public:
    sf::CircleShape node;
    sf::Text textNode;
    CircleShapeNode(
        std::string name,
        sf::Vector2f initPosition,
        sf::Vector2f speed,
        sf::Color rgbColor,
        float shapeSize,
        Configs& configObject
    )
        : m_name(name)
        , m_speed(speed)
        , m_initPosition(initPosition)
        , m_shapeSize(shapeSize)
        , node(shapeSize)
    {
        // Create shape node
        node.setPosition(initPosition);
        node.setFillColor(rgbColor);
        // Create text node
        textNode.setFont(configObject.fontNode);
        textNode.setString(name);
        textNode.setCharacterSize(configObject.getFontSize());
        textNode.setFillColor(configObject.getFontColor());
    }

    const std::string getName()
    {
        return m_name;
    }

    const sf::Vector2f getPosition()
    {
        return m_initPosition;
    }

    const float getSize()
    {
        return m_shapeSize;
    }

    const sf::Vector2f getSpeed()
    {
        return m_speed;
    }

    const sf::Vector2f getShapeCenterPosition()
    {
        return sf::Vector2f(
            node.getPosition().x + getSize(),
            node.getPosition().y + getSize()
        );
    }

    sf::Vector2f getShapeCenterPosition() const override
    {
        return sf::Vector2f(
            node.getPosition().x + m_shapeSize,
            node.getPosition().y + m_shapeSize
        );
    }

    ShapeVariant getNode() const override
    {
        return node;
    }

    sf::Vector2f getSpeed() const override
    {
        return m_speed;
    }

    sf::Text getTextNode() const override
    {
        return textNode;
    }

    void moveNode(const sf::Vector2f& delta) override
    {
        node.move(delta);
    }

    sf::FloatRect getBounds() const override
    {
        return node.getGlobalBounds();
    }

    void setSpeed(const sf::Vector2f& speed) override
    {
        m_speed = speed;
    }
};

class RectangleShapeNode : public ShapeNode
{
    std::string m_name;
    sf::Vector2f m_speed;
    sf::Vector2f m_initPosition;

public:
    sf::RectangleShape node;
    sf::Text textNode;
    RectangleShapeNode(
        std::string name,
        sf::Vector2f initPosition,
        sf::Vector2f speed,
        sf::Color rgbColor,
        sf::Vector2f rectSize,
        Configs& configObject
    )
        : m_name(name)
        , m_speed(speed)
        , m_initPosition(initPosition)
        , node(rectSize)
    {
        // Create shape node
        node.setPosition(initPosition);
        node.setFillColor(rgbColor);
        // Create text node
        textNode.setFont(configObject.fontNode);
        textNode.setString(name);
        textNode.setCharacterSize(configObject.getFontSize());
        textNode.setFillColor(configObject.getFontColor());
    }

    const std::string getName()
    {
        return m_name;
    }

    const sf::Vector2f getPosition()
    {
        return m_initPosition;
    }

    const sf::Vector2f getSpeed()
    {
        return m_speed;
    }

    sf::Vector2f getShapeCenterPosition() const override
    {
        return sf::Vector2f(
            node.getPosition().x + node.getLocalBounds().width / 2,
            node.getPosition().y + node.getLocalBounds().height / 2
        );
    }

    ShapeVariant getNode() const override
    {
        return node;
    }

    sf::Vector2f getSpeed() const override
    {
        return m_speed;
    }

    sf::Text getTextNode() const override
    {
        return textNode;
    }

    void moveNode(const sf::Vector2f& delta) override
    {
        node.move(delta);
    }

    sf::FloatRect getBounds() const override
    {
        return node.getGlobalBounds();
    }

    void setSpeed(const sf::Vector2f& speed) override
    {
        m_speed = speed;
    }
};

void handleBounce(
    ShapeNode& shape,
    int windowWidth,
    int windowHeight
)
{
    sf::FloatRect bounds = shape.getBounds();
    sf::Vector2f speed = shape.getSpeed();

    if (bounds.left < 0)
    {
        speed.x = std::abs(speed.x);
    }
    else if (bounds.left + bounds.width > windowWidth)
    {
        speed.x = -std::abs(speed.x);
    }

    if (bounds.top < 0)
    {
        speed.y = std::abs(speed.y);
    }
    else if (bounds.top + bounds.height > windowHeight)
    {
        speed.y = -std::abs(speed.y);
    }

    shape.setSpeed(speed);
}

void loadFromFile(
    const std::string& filename,
    Configs& configObject,
    std::vector<std::unique_ptr<ShapeNode>>& shapes
) 
{
    std::ifstream fin(filename);
    std::string configName;
    int windowWidthSize;
    int windowHeightSize;
    std::string fontPath;
    int fontSize;
    int fontColorR, fontColorG, fontColorB;
    std::string shapeName;
    sf::Vector2f initPosition;
    sf::Vector2f speed;
    int shapeColorR, shapeColorG, shapeColorB;
    float shapeSize;
    sf::Vector2f rectSize;

    while (fin >> configName)
    {
        if (configName == "Window")
        {
            fin >> windowWidthSize 
                >> windowHeightSize;
            configObject.setWindowWidthSize(windowWidthSize);
            configObject.setWindowHeightSize(windowHeightSize);
        }
        else if (configName == "Font")
        {
            fin >> fontPath 
                >> fontSize 
                >> fontColorR
                >> fontColorG
                >> fontColorB;
            configObject.setFontPath(fontPath);
            configObject.setFontSize(fontSize);
            configObject.setFontColor(sf::Color(fontColorR, fontColorG, fontColorB));
        }
        else if (configName == "Circle")
        {
            fin >> shapeName 
                >> initPosition.x 
                >> initPosition.y 
                >> speed.x 
                >> speed.y 
                >> shapeColorR
                >> shapeColorG
                >> shapeColorB
                >> shapeSize;
            CircleShapeNode circleNode(
                shapeName,
                initPosition,
                speed,
                sf::Color(shapeColorR, shapeColorG, shapeColorB),
                shapeSize,
                configObject
            );
            shapes.push_back(std::make_unique<CircleShapeNode>(circleNode));
        }
        else if (configName == "Rectangle")
        {
            fin >> shapeName
                >> initPosition.x
                >> initPosition.y
                >> speed.x
                >> speed.y
                >> shapeColorR
                >> shapeColorG
                >> shapeColorB
                >> rectSize.x
                >> rectSize.y;
            RectangleShapeNode rectangleNode(
                shapeName,
                initPosition,
                speed,
                sf::Color(shapeColorR, shapeColorG, shapeColorB),
                rectSize,
                configObject
            );
            shapes.push_back(std::make_unique<RectangleShapeNode>(rectangleNode));
        }
    }
}

void changeTextPosition(
    sf::Text& textNode,
    sf::Vector2f& shapeCenterPosition
)
{
    sf::FloatRect bounds = textNode.getLocalBounds();
    textNode.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );
    textNode.setPosition(shapeCenterPosition);  // Position text in center of shape
}

void drawTextNode(
    sf::Text& text,
    sf::Vector2f shapeCenterPosition,
    sf::RenderWindow& window
)
{
    // Center the text origin
    changeTextPosition(text, shapeCenterPosition);
    window.draw(text);
}

void renderShapes(
    sf::RenderWindow& window,
    std::vector<std::unique_ptr<ShapeNode>>& shapes,
    Configs& configObject
)
{
    for (const auto& shape : shapes)
    {
        sf::Vector2f nodeSpeed = shape->getSpeed();
        shape->moveNode(nodeSpeed);

        handleBounce(*shape, configObject.getWidth(), configObject.getHeight());

        ShapeVariant node = shape->getNode();

        std::visit([&window](auto&& shape) {
            window.draw(shape);
        }, node);

        sf::Text textNode = shape->getTextNode();
        sf::Vector2f shapeCenter = shape->getShapeCenterPosition();
        drawTextNode(textNode, shapeCenter, window);
    }
}

int main()
{
    Configs configObject; // init configObject
    std::vector<CircleShapeNode> circles; // init circles array
    std::vector<RectangleShapeNode> rectangles; // init rectangles array
    std::vector<std::unique_ptr<ShapeNode>> shapes; // init shapes array
    loadFromFile("config.txt", configObject, shapes); // preload configs and shapes from config.txt file
    sf::RenderWindow window(sf::VideoMode(configObject.getWidth(), configObject.getHeight()), "SFML works!"); // render window
    window.setFramerateLimit(60); // set frame limit
    // while loop for game runing untill window is close
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        renderShapes(window, shapes, configObject); // render all shapes
        window.display();
    }

    return 0;
}