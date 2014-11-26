#ifndef GAME_UNKNOWN_HPP
#define	GAME_UNKNOWN_HPP	1


class GameUnknown : public GBGame {
public:

  GameUnknown();
  virtual const char *  getName();

  virtual void  	newGame();  
  virtual void  	draw() const;  
  virtual void  	update();



protected:

};

#endif	// GAME_UNKNOWN_HPP
