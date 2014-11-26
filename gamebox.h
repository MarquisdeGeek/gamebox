#ifndef GAMEBOX_HPP
#define	GAMEBOX_HPP	1

class GBGame {
public:
  virtual void newGame() {} 
  virtual const char *  getName() = 0;
#if GAMEBOX_ETHERNET
  virtual void renderWebPage(EthernetClient &client){}
#endif
  virtual void draw() const   {}
  virtual void update()       {}
};


class GameBox {
public:
  
  void init();
  void playSound(const int pitch, const int duration) const;  
  void playTune(const int *pMelody) const;    
  boolean isLeftButtonDown() const;
  boolean isRightButtonDown() const;
  boolean wasLeftButtonPressed() const;
  boolean wasRightButtonPressed() const;
  //
#if GAMEBOX_ETHERNET  
  void renderWebPage(EthernetClient &client);
#endif  
  void draw() const;
  void update();
  
  //
  void startMenu();
  void startGame(GBGame *pGame);

protected:
  GBGame *     pCurrentGame; 
  boolean      wasLeft;
  boolean      wasRight;
};

#endif	// GAMEBOX_HPP
