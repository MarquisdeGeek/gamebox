/*
**  GAMEBOX - A handheld games console in a box
**  by Marquis de Geek - @marquisdegeek - 2014
**
**  Released under the GPL, version 2.
**
** Version 1.0
*/

#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

#define GAMEBOX_ETHERNET    1
#define GAMEBOX_AUDIO       1
#define GAMEBOX_TRACE       0

//
// Hardware pins
//
int pinButtonLeft = A5;
int pinButtonRight = 9;
int pinSpeaker = A2;
//
LiquidCrystal lcd(2, 3, 5, 6, 7, 8);
//
// Ethernet component
//
#if GAMEBOX_ETHERNET
byte mac[] = { 0x90, 0xA2, 0xda, 0x0d, 0x62, 0x9c };
IPAddress ip(192, 168, 0, 82);
EthernetServer server(80);
#endif

//
// Runner game specific data.
//
char *g_RunnerPlayerAnimation[] = { "[", "|", "]" };
int   g_RunnerMapData[] = {0,0,0,0,0,0,0,1,0,0,0,0, 1,0,0, 1,0,0,1,1,0,0,0,1,0,0,1,0,0,0,0, 1,0,0,1,0,0,1,0,0,1,1};


void log(const char *p) {
#if GAMEBOX_TRACE
  Serial.println(p);
#endif
}

const int GB_NOTE_C3 = 131;
const int GB_NOTE_C4 = 262;
const int GB_NOTE_G3 = 196;
const int GB_NOTE_A3 = 220;
const int GB_NOTE_B3 = 247;
const int GB_NOTE_C6 = 1047;


class GameBox {
public:
  
  void init() {
    Serial.begin(9600);
    log("GameBox.init");
    
    lcd.begin(16, 2);
  
    pinMode(pinButtonLeft, INPUT); 
    pinMode(pinButtonRight, INPUT); 
    pinMode(pinSpeaker, OUTPUT);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gamebox - by");
    lcd.setCursor(1,1);
    lcd.print("Marquis de Geek");
    
#if GAMEBOX_AUDIO
    for(int i=GB_NOTE_C4;i<GB_NOTE_C6;++i) {
      playSound(i, 150);
    }
#endif

    delay(2000);
  }    
  
  void playSound(const int pitch, const int duration) {
#if GAMEBOX_AUDIO
    tone(pinSpeaker, pitch, duration);
#endif
  }
  
  void playTune(const int *pMelody) {
#if GAMEBOX_AUDIO
    while(*pMelody) {
       int notePitch = pMelody[0];
       int noteDuration = 1000/pMelody[1];
       tone(pinSpeaker, notePitch,noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(pinSpeaker);
      
      pMelody += 2;
    }
#endif
   }
    
  boolean isLeftButtonDown() {
    return analogRead(pinButtonLeft) > 10;
  }

  boolean isRightButtonDown() {
    return digitalRead(pinButtonRight);
  }
  
};
GameBox     gamebox;


class GameRunnerPlayer {
public:

  void newGame() {
    y = 1;
    jump = 0;
    animIdx = 0;
    dead = 0;
    score = 0;
  }
  
  void draw() {
    lcd.setCursor(0,y);
    lcd.print(dead ? "X" : g_RunnerPlayerAnimation[animIdx]);
    lcd.setCursor(0,1-y);
    lcd.print(" ");
    
    drawScore();
  }

  void drawScore() {
    lcd.setCursor(7,0);
    lcd.print("Score ");
  
    lcd.setCursor(13,0);
    lcd.print(score);
  }
  
  void update() {
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
  
  boolean isJumping() {
    return jump ? true : false;
  }
  
  boolean isDead() {
    return dead ? true : false;
  }
 
  void jumpUp() {
      y = 0;
      jump = 3;
       
      int jumpSound[] = {GB_NOTE_C3,300,  0};
      gamebox.playTune(jumpSound);
  }
  
  void incScore() {
     ++score;
  }
  
  void kill() {
    int tuneData[] = {GB_NOTE_C4,4 ,  GB_NOTE_G3, 8,  GB_NOTE_C3,8, 0};

    log("player.kill");
    if (!dead) {
      dead = 1;
      draw();
      gamebox.playTune(tuneData);
    }
  }
  
  int score;
private:
  int y;
  int jump;
  int animIdx;
  int dead;
 
};

class GameRunnerMap {
public:
  
  void newGame() {
    // TODO: Randomize g_RunnerMapData, or vary according to level ?
  }
  
  void  update() {
  }
  
  boolean isCollision(const int pos) {
     return g_RunnerMapData[pos&0xf] ? true : false;
  }
 
  void draw(const int pos) {
    lcd.setCursor(0,1);
    for(int i=0;i<16;++i) {
        lcd.print(g_RunnerMapData[(pos+i)&0xf] ? "o" : " ");
    }
  }
};

class GameRunnerState {
public:
  //
  void newGame() {
    gameSpeed = 200;
    pos = 0;
  }
  
  void  draw() {
  }

  void update() {        
    if ((pos % 2) == 0) {
      --gameSpeed;
    }
    ++pos;
  }
  
 public:
  int gameSpeed;
  int pos;

};

class GameRunner {
public:

  GameRunner() {
      gamesPlayed = 0;
      highScore = 0;
  }
  
  void  newGame() {
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
  
  void  draw() {
    if (!isGameOver()) {
      gamemap.draw(state.pos);
      player.draw();
      state.draw();
    }
  }
  
  void  update() {
    
    if (isGameOver()) {
       if (gamebox.isLeftButtonDown()) {
         newGame();
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
   
   boolean isGameOver() {
     return player.isDead();
   }
    
#if  GAMEBOX_ETHERNET   
   void renderWebPage(EthernetClient &client) {
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


protected:
  GameRunnerPlayer   player;
  GameRunnerState    state;
  GameRunnerMap      gamemap;
  //
  int gamesPlayed;
  int highScore;
};

GameRunner  game;



void handleServer() {
#if GAMEBOX_ETHERNET
  EthernetClient client = server.available();
  if (client) {
    log("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
        // TODO: Check that this still works
        if (c == '\n' && currentLineIsBlank) {
          game.renderWebPage(client);
          break;
        }
   
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    log("client disconnected");
  }
#endif
}


void setup() {

#if GAMEBOX_ETHERNET
  Ethernet.begin(mac, ip);
  server.begin();
#endif

  gamebox.init();
   
  game.newGame();
}

void loop() {
  handleServer();
    
  game.draw();
  game.update();
}




