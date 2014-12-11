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
#include "settings.h"
#include "gamebox.h"
#include "menu.h"
#include "runner.h"
#include "hangman.h"


//
// Globals
//
GameBox     gamebox;
GameBoxMenu menu;


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


  void GameBox::init() {
#if GAMEBOX_TRACE
    Serial.begin(9600);
#endif
    log("GameBox.init");

    wasLeft = false;
    wasRight = false;
   
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
  
  void GameBox::playSound(const int pitch, const int duration) const {
#if GAMEBOX_AUDIO
    tone(pinSpeaker, pitch, duration);
#endif
  }
  
  void GameBox::playTune(const int *pMelody) const {
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
    
  boolean GameBox::isLeftButtonDown() const {
    return analogRead(pinButtonLeft) > 10;
  }

  boolean GameBox::isRightButtonDown() const {
    return digitalRead(pinButtonRight);
  }
    
  boolean GameBox::wasLeftButtonPressed() const {
    return !wasLeft && isLeftButtonDown();
  }

  boolean GameBox::wasRightButtonPressed() const {
    return !wasRight && isRightButtonDown();
  }
 
  void GameBox::startMenu() {
    startGame(&menu);
  }
  
  void GameBox::startGame(GBGame *pGame) {
      pCurrentGame = pGame;
      pGame->newGame();
  }
#if GAMEBOX_ETHERNET  
  void GameBox::renderWebPage(EthernetClient &client) {
    pCurrentGame->renderWebPage(client);
  }
#endif
  void GameBox::draw() const {
    pCurrentGame->draw();
  }
  
  void GameBox::update() {
    
    pCurrentGame->update();

    wasLeft = gamebox.isLeftButtonDown();
    wasRight = gamebox.isRightButtonDown();
  }


//
// Ethernet handler
//

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
          gamebox.renderWebPage(client);
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
  menu.addGame(new GameRunner());
  menu.addGame(new GameHangman());

  gamebox.startMenu();
}

void loop() {
  handleServer();
    
  gamebox.draw();
  gamebox.update();
}




