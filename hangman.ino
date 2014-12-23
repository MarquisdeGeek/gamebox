/*
**  GAMEBOX - A handheld games console in a box
**  by Marquis de Geek - @marquisdegeek - 2014
**
**  Released under the GPL, version 2.
**
** Version 1.0
*/

#include "settings.h"
#include "hangman.h"

// Hangman
const char *pWordList[] = {
 "curlier",
"mousy",
"biased",
"fakery",
"gore",
"journal",
"citizen",
"prelude",
"catwalk",
"costant",
"eggs",
"spool",
"malarky",
"unless",
"agra",
"runway",
"sunbird",
"finch",
"diluted",
"inch",
"geisha",
"toccata",
"employer",
"longer",
"cure",
"bacon",
"find",
"cloudy",
"sunday",
"wrangle",
};


GameHangman::GameHangman() {
  for(int i=0;i<26;++i) {
    alphabetOrder[i] = i;
  }
}

const char *   GameHangman::getName() {
	return "Hangman";  
}

void  GameHangman::newGame() {
   lcd.clear();
   
   int wordCount = sizeof(pWordList)/sizeof(pWordList[0]);
   strcpy(targetWord, pWordList[random(wordCount)]);
   strcpy(guessedWordState, targetWord);
   
   for(int i=0;i<strlen(guessedWordState);++i) {
     guessedWordState[i] = '_';
   }
   
  for(int i=0;i<26;++i) {
    lettersPicked[i] = false;
  }
   
   livesLeft = 9;
   cursorPosition = 0;
   gameOver = false;
}

//
void  	GameHangman::draw() const {
  
  lcd.setCursor(0,0);
  lcd.print(guessedWordState);
  
  if (gameOver) {
    if (gameWon) {
      lcd.setCursor(1,1);
      lcd.print("You're saved!");
    } else {
      lcd.setCursor(0,1);
      lcd.print("It was:");
      lcd.print(targetWord);
    }
  } else {
    lcd.setCursor(15,0);
    lcd.print(livesLeft);

    drawSelectionWheel();
  }
}

void  	GameHangman::drawSelectionLetter(int pos) const {
  char offset;
  
  while (pos < 0) {
    pos += 26;
  }
  pos %= 26;  // two stages, because negative modulus aren't correct

  offset = (lettersPicked[pos]) ? 'a' : 'A';  // upper case means 'available'
  lcd.print((char)(alphabetOrder[pos] + offset));
  }
  
void  	GameHangman::drawSelectionWheel() const {
  lcd.setCursor(0,1);
  lcd.print('<');  // indicate to the player that pressing the left button changes the wheel
 
  lcd.setCursor(7,1);
  lcd.print('_');
  lcd.print((char)(alphabetOrder[cursorPosition] + 'A'));
  lcd.print('_');
 
  char offset;
  for(int i=1;i<6;++i) {
    lcd.setCursor(7-i,1);
    drawSelectionLetter(cursorPosition - i);

    lcd.setCursor(9+i,1);
    drawSelectionLetter(cursorPosition + i);
  }

}

void  	GameHangman::moveToNextLetter() {

    do {
      if (++cursorPosition == 26) {
        cursorPosition = 0;
      }
      
    } while(lettersPicked[cursorPosition]);
    
}

void  	GameHangman::update() {
  if (gameOver) {
    if (gamebox.wasLeftButtonPressed()) {
      newGame();       
    } else if (gamebox.wasRightButtonPressed()) {
      gamebox.startMenu();
    }
  } else {  // still playing
      if (gamebox.wasLeftButtonPressed()) {
 
        moveToNextLetter();        
        drawSelectionWheel();
        
    } else if (gamebox.wasRightButtonPressed()) {
      boolean foundIt = false;
       
       for(int i=0;i<strlen(targetWord);++i) {
         if (alphabetOrder[cursorPosition]+'a' == targetWord[i]) {
           guessedWordState[i] = targetWord[i];
           foundIt = true;
         }
       }
       
       lettersPicked[cursorPosition] = true;
       moveToNextLetter();
       
       if (strcmp(targetWord, guessedWordState) == 0) {
         lcd.clear();
         gameOver = true;
         gameWon = true;
       }
       
       if (!foundIt) {
         if (--livesLeft == 0) {
           lcd.clear();
           gameOver = true;
           gameWon = false;
         }
       }
        
    }
  }
  

}



