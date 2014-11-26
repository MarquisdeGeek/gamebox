#ifndef GAMEBOX_HPP
#define	GAMEBOX_HPP	1

class GBGame {
public:
};


class GameBox {
public:
  
  void init();
  void playSound(const int pitch, const int duration);  
  void playTune(const int *pMelody);    
  boolean isLeftButtonDown();
  boolean isRightButtonDown();
  
};

#endif	// GAMEBOX_HPP
