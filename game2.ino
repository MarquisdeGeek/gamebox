/*
**  GAMEBOX - A handheld games console in a box
**  by Marquis de Geek - @marquisdegeek - 2014
**
**  Released under the GPL, version 2.
**
** Version 1.0
*/

#include "settings.h"
#include "game2.h"




GameUnknown::GameUnknown() {

}

const char *   GameUnknown::getName() {
	return "TWO";  
}

void  GameUnknown::newGame() {
  
}

//
void  	GameUnknown::draw() const {
/*
  if (refreshLCD) {
    lcd.clear();
    // Game name
    lcd.print(gameList[currentSelection]->getName());
    // <-- start    next -->
    lcd.setCursor(0,1);
    lcd.print("<-Play");
  
    lcd.setCursor(10,1);
    lcd.print("Next->");
  }
  */
}

void  	GameUnknown::update() {
   
  if (gamebox.isLeftButtonDown()) {
   
  }
  if (gamebox.isRightButtonDown()) {
    gamebox.startMenu();
  }
}



