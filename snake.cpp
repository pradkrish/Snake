#include "snake.hpp"

Snake::Snake(int l_blockSize, Textbox* l_log)
{
  m_log = l_log;
  m_size = l_blockSize;

  m_bodyRect.setSize(sf::Vector2f(m_size - 1, m_size - 1));
  reset();
}

Snake::~Snake()
{
}

void Snake::setDirection(Direction l_dir)
{
  m_dir = l_dir;
}

Direction Snake::getDirection()
{
  return m_dir;
}

int Snake::getSpeed()
{
  return m_speed;
}

sf::Vector2i Snake::getPosition()
{
  return (!m_snakeBody.empty() ? m_snakeBody.front().position : sf::Vector2i(1,1));
}

int Snake::getLives()
{
  return m_lives;
}

int Snake::getScore()
{
  return m_score;
}

void Snake::increaseScore()
{
  m_score += 10;
  m_log->Add("You ate an apple. Score: "
	     +std::to_string((long long)m_score));
}

bool Snake::hasLost()
{
  return m_lost;
}

void Snake::lose()
{
  m_lost = true;
}

void Snake::toggleLost()
{
  m_lost = !m_lost;
}

Direction Snake::getPhysicalDirection()
{
  if(m_snakeBody.size() <= 1)
    {
      return Direction::None;
    }

  SnakeSegment& head = m_snakeBody[0];
  SnakeSegment& neck = m_snakeBody[1];

  if(head.position.x == neck.position.x)
    {
      return (head.position.y > neck.position.y 
	      ? Direction::Down : Direction::Up);
    }
  else if(head.position.y == neck.position.y)
    {
      return (head.position.x > neck.position.x 
	      ? Direction::Right : Direction::Left);
    }

  return Direction::None;
}

void Snake::extend()
{
  if (m_snakeBody.empty()) { return; }
  SnakeSegment& tail_head = 
    m_snakeBody[m_snakeBody.size() - 1];

  if(m_snakeBody.size() > 1)
    {
      SnakeSegment& tail_bone = 
	m_snakeBody[m_snakeBody.size() - 2];

      if(tail_head.position.x == tail_bone.position.x)
	{
	  if(tail_head.position.y > tail_bone.position.y)
	    {
	      m_snakeBody.push_back(SnakeSegment(
						 tail_head.position.x, tail_head.position.y + 1));
	    }
	  else
	    {
	      m_snakeBody.push_back(SnakeSegment(
						 tail_head.position.x, tail_head.position.y - 1));
	    }
	}
      else if(tail_head.position.y == tail_bone.position.y)
	{
	  if(tail_head.position.x > tail_bone.position.x)
	    {
	      m_snakeBody.push_back(SnakeSegment(
						 tail_head.position.x + 1, tail_head.position.y));
	    }
	  else
	    {
	      m_snakeBody.push_back(SnakeSegment(
						 tail_head.position.x - 1, tail_head.position.y));
	    }
	}
    }
  else
    {
      if(m_dir == Direction::Up)
	{
	  m_snakeBody.push_back(SnakeSegment(
					     tail_head.position.x, tail_head.position.y + 1));
	}
      else if (m_dir == Direction::Down)
	{
	  m_snakeBody.push_back(SnakeSegment(
					     tail_head.position.x, tail_head.position.y - 1));
	}
      else if (m_dir == Direction::Left)
	{
	  m_snakeBody.push_back(SnakeSegment(
					     tail_head.position.x + 1, tail_head.position.y));
	}
      else if (m_dir == Direction::Right)
	{
	  m_snakeBody.push_back(SnakeSegment(
					     tail_head.position.x - 1, tail_head.position.y));
	}
    }
}

void Snake::reset()
{
  m_snakeBody.clear();
  m_snakeBody.push_back(SnakeSegment(5,7));
  m_snakeBody.push_back(SnakeSegment(5,6));
  m_snakeBody.push_back(SnakeSegment(5,5));

  setDirection(Direction::None); // Start off still.
  m_speed = 15;
  m_lives = 3;
  m_score = 0;
  m_lost = false;
}

void Snake::checkCollision()
{
  if (m_snakeBody.size() < 5){ return; }
  SnakeSegment& head = m_snakeBody.front();
  for(auto itr = m_snakeBody.begin() + 1; 
      itr != m_snakeBody.end(); ++itr)
    {
      if(itr->position == head.position)
	{
	  int segments = m_snakeBody.end() - itr;
	  cut(segments);
	  break;
	}
    }
}

void Snake::move()
{
  for (int i = m_snakeBody.size() - 1; i > 0; --i)
    {
      m_snakeBody[i].position = m_snakeBody[i - 1].position;
    }
  if (m_dir == Direction::Left)
    {
      --m_snakeBody[0].position.x;
    }
  else if (m_dir == Direction::Right)
    {
      ++m_snakeBody[0].position.x;
    }
  else if (m_dir == Direction::Up)
    {
      --m_snakeBody[0].position.y;
    }
  else if (m_dir == Direction::Down)
    {
      ++m_snakeBody[0].position.y;
    }
}

void Snake::tick()
{
  if (m_snakeBody.empty()){ return; }
  if (m_dir == Direction::None){ return; }
  move();
  checkCollision();
}

void Snake::cut(int l_segments)
{
  for (int i = 0; i < l_segments; ++i)
    {
      m_snakeBody.pop_back();
    }
  --m_lives;
  if (!m_lives)
    {
      lose();
      return;
    }

  m_log->Add("You have lost a life! Lives left: "
	     + std::to_string((long long)m_lives));
}

void Snake::render(sf::RenderWindow& l_window)
{
  if (m_snakeBody.empty()){ return; }

  auto head = m_snakeBody.begin();
  m_bodyRect.setFillColor(sf::Color::Yellow);
  m_bodyRect.setPosition(head->position.x * m_size,
			 head->position.y * m_size);
  l_window.draw(m_bodyRect);

  m_bodyRect.setFillColor(sf::Color::Green);
  for(auto itr = m_snakeBody.begin() + 1; 
      itr != m_snakeBody.end(); ++itr)
    {
      m_bodyRect.setPosition(itr->position.x * m_size, 
			     itr->position.y * m_size);
      l_window.draw(m_bodyRect);
    }
}
