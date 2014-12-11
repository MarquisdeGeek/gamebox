#ifndef GAME_UNKNOWN_HPP
#define	GAME_UNKNOWN_HPP	1

#define MAX_WORD_LENGTH    12

class GameHangman : public GBGame {
public:

  GameHangman();
  virtual const char *  getName();

  virtual void  	newGame();  
  virtual void  	draw() const;  
  virtual void  	update();



protected:
          void        drawSelectionWheel() const;
          void        drawSelectionLetter(int pos) const;
          void        moveToNextLetter();
  
  char targetWord[MAX_WORD_LENGTH];
  char guessedWordState[MAX_WORD_LENGTH];
  char alphabetOrder[26];
  boolean  lettersPicked[26];
  int  cursorPosition;
  int  livesLeft;
  
  boolean   gameOver;
  boolean   gameWon;
};

#endif	// GAME_UNKNOWN_HPP
