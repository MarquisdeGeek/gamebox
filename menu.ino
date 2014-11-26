/*
**  GAMEBOX - A handheld games console in a box
**  by Marquis de Geek - @marquisdegeek - 2014
**
**  Released under the GPL, version 2.
**
** Version 1.0
*/

#include "settings.h"
#include "menu.h"




GameBoxMenu::GameBoxMenu() {
  gameCount = 0;
}

void  GameBoxMenu::newGame() {
  refreshLCD = true;
}

void  	GameBoxMenu::addGame(GBGame *pGame) {
  if (gameCount < MAX_GAMES) {
    gameList[gameCount] = pGame;
    ++gameCount;
  } else {
    // TODO: report an error, possibly
  }
}

//
void  	GameBoxMenu::draw() const {
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
}

void  	GameBoxMenu::update() {
  refreshLCD = false;
  
  if (gamebox.isLeftButtonDown()) {
    gamebox.startGame(gameList[currentSelection]);
  }
  if (gamebox.isRightButtonDown()) {
    gamebox.playSound(440, 50);
    gamebox.playSound(880, 30);
    if (++currentSelection == gameCount) {
      currentSelection = 0;
    }
    refreshLCD = true;
  }
}



