#pragma once
#include <SFML/Graphics.hpp>
//#include "Textbox.h"
#include <vector>

struct SnakeSegment
{
  SnakeSegment(int x, int y) : position(x,y){}
  sf::Vector2i position;
};

using SnakeContainer = std::vector<SnakeSegment>;

enum class Direction { None, Up, Down, Left, Right };

class Snake
{
public:
  Snake(int l_blockSize, Textbox* l_log);
  ~Snake();

  // Helper methods.
  void setDirection(Direction l_dir);
  Direction getDirection();
  int getSpeed();
  sf::Vector2i getPosition();
  int getLives();
  int getScore();
  void increaseScore();
  bool hasLost();

  void lose(); // Handle losing here.
  void toggleLost();

  Direction getPhysicalDirection();

  void extend(); // Grow the snake.
  void reset(); // Reset to starting position.

  void move(); // Movement method.
  void tick(); // Update method.
  void cut(int l_segments); // Method for cutting snake.
  void render(sf::RenderWindow& l_window);
private:
  void checkCollision(); // Checking collisions.

  SnakeContainer m_snakeBody; // Segment vector.
  int m_size; // Size of the graphics.
  Direction m_dir; // Current direction.
  int m_speed; // Speed of the snake.
  int m_lives; // Lives.
  int m_score; // Score.
  bool m_lost; // Losing state.
  sf::RectangleShape m_bodyRect; // Shape used in rendering.
  Textbox* m_log;
};
