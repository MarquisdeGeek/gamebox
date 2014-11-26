/*
**  GAMEBOX - A handheld games console in a box
**  by Marquis de Geek - @marquisdegeek - 2014
**
**  Released under the GPL, version 2.
**
** Version 1.0
*/

#include <LiquidCrystal.h>
#include "settings.h"
#include "runner.h"


//
// Runner game specific data.
//
namespace {
  char *g_RunnerPlayerAnimation[] = { "[", "|", "]" };
  int   g_RunnerMapData[] = {0,0,0,0,0,0,0,1,0,0,0,0, 1,0,0, 1,0,0,1,1,0,0,0,1,0,0,1,0,0,0,0, 1,0,0,1,0,0,1,0,0,1,1};
}
//
// GameRunnerPlayer
//
  void GameRunnerPlayer::newGame() {
    y = 1;
    jump = 0;
    animIdx = 0;
    dead = 0;
    score = 0;
  }
  
  void GameRunnerPlayer::draw() const {
    lcd.setCursor(0,y);
    lcd.print(dead ? "X" : g_RunnerPlayerAnimation[animIdx]);
    lcd.setCursor(0,1-y);
    lcd.print(" ");
    
    drawScore();
  }

  void GameRunnerPlayer::drawScore() const {
    lcd.setCursor(7,0);
    lcd.print("Score ");
  
    lcd.setCursor(13,0);
    lcd.print(score);
  }
  
  void GameRunnerPlayer::update() {
    if (dead) {
        return;
    }
    
    // The animation isn't much, but we only do it once per tick
    if (++animIdx == sizeof(g_RunnerPlayerAnimation)/sizeof(g_RunnerPlayerAnimation[0])) {
      animIdx = 0;
    }
   
    // While in the air, count down to 0, and replace ply on ground
    if  (jump && --jump == 0) {
      y = 1;
    }
  }
  
  boolean GameRunnerPlayer::isJumping() const {
    return jump ? true : false;
  }
  
  boolean GameRunnerPlayer::isDead() const {
    return dead ? true : false;
  }
 
  void GameRunnerPlayer::jumpUp() {
      y = 0;
      jump = 3;
       
      int jumpSound[] = {GB_NOTE_C3,300,  0};
      gamebox.playTune(jumpSound);
  }
  
  void GameRunnerPlayer::incScore() {
     ++score;
  }
  
  void GameRunnerPlayer::kill() {
    int tuneData[] = {GB_NOTE_C4,4 ,  GB_NOTE_G3, 8,  GB_NOTE_C3,8, 0};

    log("player.kill");
    if (!dead) {
      dead = 1;
      draw();
      gamebox.playTune(tuneData);
    }
  }
  
//
// GameRunnerMap
//
  void GameRunnerMap::newGame() {
    // TODO: Randomize g_RunnerMapData, or vary according to level ?
  }
  
  void  GameRunnerMap::update() {
  }
  
  boolean GameRunnerMap::isCollision(const int pos) const {
     return g_RunnerMapData[pos&0xf] ? true : false;
  }
 
  void GameRunnerMap::draw(const int pos) const {
    lcd.setCursor(0,1);
    for(int i=0;i<16;++i) {
        lcd.print(g_RunnerMapData[(pos+i)&0xf] ? "o" : " ");
    }
  }


//
// GameRunnerState::
//
  void GameRunnerState::newGame() {
    gameSpeed = 200;
    pos = 0;
  }
  
  void  GameRunnerState::draw() const {
  }

  void GameRunnerState::update() {        
    if ((pos % 2) == 0) {
      --gameSpeed;
    }
    ++pos;
  }
  

//
// GameRunner
//

  GameRunner::GameRunner() {
      gamesPlayed = 0;
      highScore = 0;
  }
  
  const char *  GameRunner::getName() {
    return "Arduino Runner";
  }
  
  void  GameRunner::newGame() {
    int tuneData[] = {GB_NOTE_C4,4 ,  GB_NOTE_G3, 8,  GB_NOTE_G3,8, GB_NOTE_A3,4, GB_NOTE_G3,2, GB_NOTE_B3,4, GB_NOTE_C4,4, 0};

      log("GameRunner.newGame");

      lcd.clear();
      lcd.print("Arduino Runner");

      gamebox.playTune(tuneData);
    
      player.newGame();
      state.newGame();
      
      lcd.clear();
      //
      ++gamesPlayed;
  }
  
  void  GameRunner::draw() const {
    if (!isGameOver()) {
      gamemap.draw(state.pos);
      player.draw();
      state.draw();
    }
  }
  
  void  GameRunner::update() {
    
    if (isGameOver()) {
       if (gamebox.isLeftButtonDown()) {
         newGame();
       }
       if (gamebox.isRightButtonDown()) {
         gamebox.startMenu();
       }
       return;
    }

    gamemap.update();
    player.update();
  
    // The player is in the same column as a boulder
    if (gamemap.isCollision(state.pos)) {

      if (player.isJumping()) {
        player.incScore();
      } else {
        player.kill();
        return;
      }
    }
   
    for(int i=0;i<state.gameSpeed;++i) {
       // don't allow the jump when ply is already in the air
       // TODO: Improve this logic so stop the player hammering the
       // button
       if (!player.isJumping() && gamebox.isRightButtonDown()) {
         player.jumpUp();
       }
       //
       delay(1);
    }
    
     state.update();
    
     if (player.score > highScore) {
        highScore = player.score;
     }
   }
   
   boolean GameRunner::isGameOver() const {
     return player.isDead();
   }
    
#if  GAMEBOX_ETHERNET   
   void GameRunner::renderWebPage(EthernetClient &client) const {
    // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connnection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    // add a meta refresh tag, so the browser pulls again every 5 seconds:
    //client.println("<meta http-equiv=\"refresh\" content=\"5\">");
    
    client.println("<h1>Arduino Runner</h1>");
    
    client.print("Games played : ");
    client.print(gamesPlayed);
    client.println("<br />");       
     
    client.print("Highest score : ");
    client.print(highScore);
    client.println("<br />");       
     
    client.print("Current score : ");
    client.print(player.score);
    client.println("<br />");
     
    client.println("</html>");
   }
#endif





