#ifndef GAMEBOX_MENU_HPP
#define	GAMEBOX_MENU_HPP	1


#define MAX_GAMES  16

class GameBoxMenu : public GBGame {
public:

  GameBoxMenu();
  
  virtual void  	newGame();  
  virtual const char *  getName() { return ""; }
  virtual void  	draw() const;  
  virtual void  	update();

          void  	addGame(GBGame *pGame);

protected:
  GBGame *      gameList[MAX_GAMES];
  //
  int           gameCount;
  int           currentSelection;
  //
  boolean       refreshLCD;
};

#endif	// GAMEBOX_MENU_HPP
