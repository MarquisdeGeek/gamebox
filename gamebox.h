#ifndef GAMEBOX_HPP
#define	GAMEBOX_HPP	1

class GBGame {
public:
};


class GameBox {
public:
  
  void init();
  void playSound(const int pitch, const int duration) const;  
  void playTune(const int *pMelody) const;    
  boolean isLeftButtonDown() const;
  boolean isRightButtonDown() const;
  
};

#endif	// GAMEBOX_HPP
