#include "Game.cpp"
#include "Window.cpp"
#include "Snake.cpp"
#include "World.cpp"
#include "Textbox.cpp"

int main(int argc, char* argv[])
{
  Game game;
  while(!game.GetWindow()->IsDone())
    {
      game.HandleInput();
      game.Update();
      game.Render();
      game.RestartClock();
    }
  return 0;
}
