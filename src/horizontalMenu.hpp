#include "directives.hpp"

#ifndef HORIZONTAL_MENU_HPP
#define HORIZONTAL_MENU_HPP

template <typename T>
class HorizontalMenu {
public:
    HorizontalMenu(float width, float height, float border, sf::Color bgColor, sf::Color textColor);
    void addTextItem(const T& item);
    void staticDisplay(sf::RenderWindow& window);
    void setPosition(float x, float y);
    int getSelectedIndex() const;
    void hide(sf::RenderWindow& window);
    void handleHover(const sf::RenderWindow& window);
    bool itemsClicked(const sf::RenderWindow& window);
    bool itemClicked(const sf::RenderWindow& window, int index);
    sf::RectangleShape getBackground() const;

private:
    float m_width;
    float m_height;
    float m_border;
    sf::Color m_bgColor;
    sf::Color m_textColor;
    sf::Font m_font;
    sf::Text m_text;
    std::vector<sf::Text> m_items;
    sf::RectangleShape m_background;
    sf::RectangleShape m_hoverRect;
    sf::Vector2f m_position;
    int m_selectedIndex;
};

//Initiate vertical menu
template <typename T>
HorizontalMenu<T>::HorizontalMenu(float width, float height, float border, sf::Color bgColor, sf::Color textColor)
    : m_width(width), m_height(height), m_border(border), m_bgColor(bgColor), m_textColor(textColor)
{
    m_font.loadFromFile("arial.ttf");
    m_text.setFont(m_font);
    m_text.setCharacterSize(16);
    m_text.setFillColor(m_textColor);

    m_background.setSize(sf::Vector2f(m_width, m_height));
    m_background.setFillColor(m_bgColor);
    m_background.setPosition(sf::Vector2f(0.f, 0.f));
}

//Add item to the menu
template <typename T>
void HorizontalMenu<T>::addTextItem(const T& item)
{
    sf::Text text;
    text.setFont(m_font);
    text.setCharacterSize(16);
    text.setFillColor(m_textColor);
    text.setString(item);
    m_items.push_back(text);
}

//Display all of the menu's content on at a origin position
template <typename T>
void HorizontalMenu<T>::staticDisplay(sf::RenderWindow& window)
{
    float totalWidth = 2 * m_border;
    for (const auto& item : m_items) {
        totalWidth += item.getLocalBounds().width + m_border;
    }

    m_background.setFillColor(m_bgColor);
    m_background.setPosition(m_position);
    m_background.setSize(sf::Vector2f(totalWidth, m_height));
    window.draw(m_background);

    float startX = m_border + m_position.x;
    float startY = m_border + m_position.y;

    for (int i = 0; i < m_items.size(); i++) {
        auto transformable = const_cast<sf::Transformable*>(static_cast<const sf::Transformable*>(&m_items[i]));
        transformable->setPosition(sf::Vector2f(startX, startY));

        if (i == m_selectedIndex) {
            float hoverXOffset = 0.f;
            float hoverRectStartX = startX - m_border / 2 - hoverXOffset;
            float hoverWidth = std::min(m_width - 2 * m_border, m_items[i].getLocalBounds().width + m_border);
            float hoverHeight = m_items[i].getLocalBounds().height + m_border;
            float hoverYOffset = (m_height - hoverHeight) / 2; 
            m_hoverRect.setSize(sf::Vector2f(hoverWidth, hoverHeight));
            m_hoverRect.setPosition(sf::Vector2f(hoverRectStartX, m_position.y + m_border + hoverYOffset - 15.f));
            m_hoverRect.setFillColor(sf::Color(211, 211, 211));
            window.draw(m_hoverRect);
        }

        window.draw(m_items[i]);
        startX += m_items[i].getLocalBounds().width + m_border;
    }
}

//Sets position of given menu
template <typename T>
void HorizontalMenu<T>::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

//Hide the menu from visible view
template <typename T>
void HorizontalMenu<T>::hide(sf::RenderWindow& window)
{
    float startX = -10000.f;
    float startY = -10000.f;
    for (auto& item : m_items) {
        item.setPosition(startX, startY);
    }

    m_background.setFillColor(m_bgColor);
    m_background.setPosition(m_position);
    m_background.setSize(sf::Vector2f(0.f, m_height));
    window.draw(m_background);
}

//Get selected index
template <typename T>
int HorizontalMenu<T>::getSelectedIndex() const
{
    return m_selectedIndex;
}

//Get background
template <typename T>
sf::RectangleShape HorizontalMenu<T>::getBackground() const
{
    return m_background;
}

//Handle hovering
template <typename T>
void HorizontalMenu<T>::handleHover(const sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    sf::Vector2f mousePosView = window.mapPixelToCoords(mousePos);

    int oldIndex = m_selectedIndex;
    m_selectedIndex = -1;
    for (int i = 0; i < m_items.size(); i++) {
        if (m_items[i].getGlobalBounds().contains(mousePosView)) {
            m_selectedIndex = i;
            break;
        }
    }
}

//Affects all items on click.
template <typename T>
bool HorizontalMenu<T>::itemsClicked(const sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosView = window.mapPixelToCoords(mousePos);

    for (const auto& item : m_items) {
        if (item.getGlobalBounds().contains(mousePosView)) {
            return true;
        }
    }
    return false;
}

//Affects an item on a select index
template <typename T>
bool HorizontalMenu<T>::itemClicked(const sf::RenderWindow& window, int index)
{
    static bool wasMousePressed = false;

    if (index < 0 || index >= m_items.size()) {
        return false;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!wasMousePressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosView = window.mapPixelToCoords(mousePos);

            if (m_items[index].getGlobalBounds().contains(mousePosView)) {
                wasMousePressed = true; // Mouse flag to prevent infinite and constant input.
                return true;
            }
        }
    }
    else {
        wasMousePressed = false;
    }

    return false;
}

#endif
