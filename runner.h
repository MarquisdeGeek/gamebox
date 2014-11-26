#ifndef GAMEBOX_RUNNER_HPP
#define	GAMEBOX_RUNNER_HPP	1


class GameRunnerPlayer {
public:

  void newGame();
  void draw();
  void drawScore();  
  void update();
  boolean isJumping();
  boolean isDead();
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
  boolean isCollision(const int pos);
  void draw(const int pos);
};

class GameRunnerState {
public:

  void newGame();  
  void draw();
  void update();
  
 public:
  int gameSpeed;
  int pos;
};

class GameRunner : public GBGame {
public:

  GameRunner();
  
  void  	newGame();  
  void  	draw();  
  void  	update();
  boolean 	isGameOver();
    
#if  GAMEBOX_ETHERNET   
   void renderWebPage(EthernetClient &client);
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
