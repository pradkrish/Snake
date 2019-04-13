#include "window.hpp"

Window::Window()
{
 setup("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string& title, const sf::Vector2u& size)
{
  setup(title, size);
}

Window::~Window()
{
  destroy();
}

void Window::Setup(const std::string title, const sf::Vector2u& size)
{
  m_windowTitle = title;
  m_windowSize = size;
  m_isFullScreen = false;
  m_isDone = false;
  create();
}

void Window::create()
{
  auto style = (m_isFullScreen ? sf::Style::FullScreen
		: sf::Style::Default);
  m_window.create({ m_windowSize.x, m_windowSize.y, 32 },
		  m_windowTitle, style);
}

void Window::destroy()
{
  m_window.close();
}

void Window::beginDraw()
{
  m_window.clear(sf::Color::Black);
}

void Window::endDraw()
{
  m_window.display();
}

bool Window::isDone()
{
  return m_isDone;
}

bool Window::isFullScreen()
{
  return m_isFullScreen;
}

sf::RenderWindow* Window::getRenderWindow()
{
  return &m_window;
}

sf::Vector2u Window::getWindowSize()
{
  return m_windowSize;
}

void Window::toggleFullScreen()
{
  m_isFullscreen = !m_isFullscreen;
  m_window.close();
  create();
}

void Window::update()
{
  sf::Event event;
  while(m_window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
	m_isDone = true; 
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
      m_isDone = true;
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
      toggleFullScreen(); 
    }
}
