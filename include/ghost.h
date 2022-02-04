#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "map.h"
#include <queue>

namespace PacmanGame {

typedef std::pair<size_t, size_t> Coordinates;

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

class Ghost {
 public:
  
  /**
   * Constructs an instance of the ghost given its position
   * @param position the position of the ghost on the map
   * @param color the color of the ghost
   * @param name the first letter of the name of the ghost 
   */
  Ghost(const glm::vec2 &position, const std::string& color, char name);

  /**
   * Draws the ghost 
   */
  void Draw();
  
  /**
   * Updates the position of the ghost
   * @param map_tiles the 2d array of the map tiles
   * @param pacman_location the location of Pacman
   * @param map the map of the game
   */
  void Update(const std::vector<std::vector<char>>& map_tiles, const 
  Coordinates& pacman_coordinates, const Map& map);
  
  /**
   * Gets the initial coordinates of the ghost
   * @param map_tiles the 2d array of the map tiles
   */
  void GetInitialGhostCoordinates(const std::vector<std::vector<char>>
                                 &map_tiles);

  /**
  * Respawns the ghost back to its initial coordinates
  */
  void Respawn();
  
  const glm::vec2 &GetPosition() const;
  const std::pair<size_t, size_t> &GetGhostCoordinates() const;

  enum State {
    SCATTER, CHASE, FRIGHTEN
  };

  void SetState(const State& state);

  enum Move {
    UP, DOWN, RIGHT, LEFT
  };

  void SetSize(double size);
  
  State GetState() const;

 private:
  char name_;
  glm::vec2 initial_position_;
  glm::vec2 position_;
  Coordinates ghost_coordinates_;
  Coordinates initial_ghost_coordinates_;
  ci::Color color_;
  ci::Color normal_color_;
  ci::Color frighten_color_;
  TimePoint begin_time_;
  State state_;
  Move last_move_;
  size_t update_count_;
  double size_;
  
  // Modify these constants to change the speed of the ghosts
  constexpr static double kSpeed = 40;
  
  /**
   * Draws the eyes of the ghost
   */
  void DrawEyes();

  /**
   * Gets the neighboring position vectors surrounding the ghost 
   * @return a map of the move and the neighboring position vector
   */
  std::vector<Coordinates> GetNeighbors(const Coordinates& coordinates);

  /**
   * Moves the ghost up
   */
  void MoveUp();
  
  /**
   * Moves the ghost down
   */
  void MoveDown();
  
  /**
   * Moves the ghost left
   */
  void MoveLeft();
  
  /**
   * Moves the ghost right
   */
  void MoveRight();
  
  /**
   * Scatters the movement of the ghost
   */
  void Scatter(const std::vector<std::vector<char>>& map_tiles);
  
  /**
   * Chases Pacman
   * @param pacman_location the location of Pacman
   */
  void Chase(const std::vector<std::vector<char>>& map_tiles, const 
  Coordinates& pacman_coordinates);
  
  void Frighten(const std::vector<std::vector<char>>& map_tiles);
  

  /**
   * Finds a path to a certain coordinate using breadth first search
   * @param map_tiles the 2d array of map tiles
   * @param destination the coordinates of the destination 
   * @return a vector of coordinates that represents the path to the destination
   */
  std::vector<Coordinates> FindPathToTile(const std::vector<std::vector<char>>&
  map_tiles, const Coordinates& destination);
  
  /**
   * Reconstructs the path to the destination from the destination to the 
   * ghost's location and reverses it
   * @param prev the 2d vector of the previous coordinates before the step is
   * made
   * @param destination the destination of the ghost
   * @return a vector of coordinates that represents the path to the destination
   */
  std::vector<Coordinates> ReconstructPath(const 
                                           std::vector<std::vector<Coordinates>>& prev, const Coordinates& destination);
  void DrawPupils();
};

} // namespace PacmanGame
