#ifndef GAMEBOX_RUNNER_HPP
#define	GAMEBOX_RUNNER_HPP	1


class GameRunnerPlayer {
public:

  void newGame();
  void draw() const;
  void drawScore() const;  
  void update();
  boolean isJumping() const;
  boolean isDead() const;
  void jumpUp();  
  void incScore();  
  void kill();
  
public:  
  int score;
private:
  int y;
  int jump;
  int animIdx;
  int dead;
 
};

class GameRunnerMap {
public:
  
  void newGame();
  void update();
  boolean isCollision(const int pos) const;
  void draw(const int pos) const;
};

class GameRunnerState {
public:

  void newGame();  
  void draw() const;
  void update();
  
 public:
  int gameSpeed;
  int pos;
};

class GameRunner : public GBGame {
public:

  GameRunner();
  
  void  	newGame();  
  void  	draw() const;  
  void  	update();
  boolean 	isGameOver() const;
    
#if  GAMEBOX_ETHERNET   
   void renderWebPage(EthernetClient &client) const;
#endif


protected:
  GameRunnerPlayer   player;
  GameRunnerState    state;
  GameRunnerMap      gamemap;
  //
  int gamesPlayed;
  int highScore;
};

#endif	// GAMEBOX_RUNNER_HPP
