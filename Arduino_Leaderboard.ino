  /*
  Program for an LCD leaderboard
*/

//includes proper librarys to print lCD and store memory
#include <Time.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <Player.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//Sets up I2C address and pins for lcd panel
#define I2C_ADDR    0x3F
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

//inititates LCD display
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

//set up button pins
int button1 = 10;
int button2 = 9;
int button3 = 8;
int button4 = 7;

/*
 * int to keep track of what screen is displayed
 * 0 = Main display wins
 * 1 = Menu
 * 2 = New game winner
 * 3 = New game p p1
 * 4 = New game p p2
 * 5 = New game p p3
 * 6 = New game p p4
 * 7 = clear mem
 */

 //State counter keeps track of what menu is displayed on the lcd
float state = 0;

//int to store points when entered
int p = 0;

//last button int keeps track of which button was clicked last
int lastButton = -1;

//Player Classes are instantiated
Player player1("Carter",0,0,0,0);
Player player2("Emma",0,0,0,3);
Player player3("Harrison",0,0,0,6);
Player player4("Hunter",0,0,0,9);

void setup() {
  Serial.begin(9600);
  //Starts LCD and displays project catan
  lcd.begin(20, 4);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  lcd.print("Project Catan");
  delay(1000);

  //Sets time to 0
  setTime(0,0,0,1,1,1);
  
  //calls read from mem function to update the players information in the ram
  player1.readFromMem();
  player2.readFromMem();
  player3.readFromMem();
  player4.readFromMem();

  //displays players sorted by wins
  updateAll();
}

void loop() {
  //on the main screen the numbers alternate between wins and Win loss ratio
  if(state == 0 && (second() < 15)){
    updateAll();
    state = 0.5;
  }else if(state == 0.5 && second() >= 15){
    updateWL();
    state = 0;
  }else if(second() >= 30){
    setTime(0,0,0,1,1,1);
  }
  
  //if button pressed on main screen go to menu
  if(checkButtons() && state >= 0 && state < 1){
    state = 1;
    menu();
    //changes button position to null
    lastButton = -1;
    delay(300);
  }

  //if button pressed on menu process input
  if(checkButtons() && state == 1){
    //new game
    if(lastButton == 1){
      state = 2;
      newGame();
    }
    //clear menu
    else if(lastButton == 2){
      state = 7;
      clearMenu();
    }
    //return to main screen
    else if(lastButton == 4){
      state = 0;
      updateAll();
    }
    //button position null
    lastButton = -1;
    delay(300);
  }

  //if on new game menu and button pressed process input
  if(checkButtons() && state == 2){
    //update score based on which button was pressed
    state = 3;
    if(lastButton == 1){
      player1.incrementWin(1);
      player2.incrementLoss(1);
      player3.incrementLoss(1);
      player4.incrementLoss(1);
    }
    else if(lastButton == 2){
      player2.incrementWin(1);
      player1.incrementLoss(1);
      player3.incrementLoss(1);
      player4.incrementLoss(1);
    }
    else if(lastButton == 3){
      player3.incrementWin(1);
      player1.incrementLoss(1);
      player2.incrementLoss(1);
      player4.incrementLoss(1);
    }
    else if(lastButton == 4){
      player4.incrementWin(1);
      player1.incrementLoss(1);
      player2.incrementLoss(1);
      player3.incrementLoss(1);
    }
    player1.writeToMem();
    player2.writeToMem();
    player3.writeToMem();
    player4.writeToMem();
    //button position null
    lastButton = -1;
    //changes to the point screen with argument player1
    points(player1);
    delay(300);
  }

  //calls PointIncrement for player1
  if(checkButtons() && state == 3){
    pointIncrement(player1, player2, 4);
  }

  //calls PointIncrement for player2
  if(checkButtons() && state == 4){
    pointIncrement(player2, player3, 5);
  }

  //calls PointIncrement for player3
  if(checkButtons() && state == 5){
    pointIncrement(player3, player4, 6);
  }

  //Point increment for player4
  if(checkButtons() && state == 6){
    pointIncrement(player4, 0);
  }

  if(checkButtons() && state == 7){
    //if clear mem = yes clear the mem for each player
    if(lastButton == 3){
      state = 0;
      player1.clearMem();
      player2.clearMem();
      player3.clearMem();
      player4.clearMem();
      updateAll();
    }
    //if no go back to main page
    else if(lastButton == 4){
      state = 0;
      updateAll();
    }
    //button null
    lastButton = -1;
    delay(500);
  }
}

/*
 * prints out the player name and score on the line number entered
 */
void updateDisplay(Player player, int number){
  lcd.setCursor(0,number);
  lcd.print(player.name);
  if(player.getWins() > 10){
    lcd.setCursor(18,number);
  }else{
    lcd.setCursor(19,number);
  }
  lcd.print(player.getWins());
}

/*
 * Updates the entire screen of players
 */
void updateAll(){
  player1.writeToMem();
  player2.writeToMem();
  player3.writeToMem();
  player4.writeToMem();
  lcd.clear();
  String playersNames[4] = {player1.name, player2.name, player3.name, player4.name};
  int wins[4] = {player1.getWins(), player2.getWins(), player3.getWins(), player4.getWins()};

  sort(wins, playersNames, 4);
  
  lcd.clear();
  for(int i = 0; i < 4; i ++){
    lcd.setCursor(0,i);
    lcd.print(playersNames[i]);
    if(wins[i] > 10){
      lcd.setCursor(18,i);
    }
    else{
      lcd.setCursor(19,i);
    }
    lcd.print(wins[i]);
  }
}

void updateWL(){
  player1.writeToMem();
  player2.writeToMem();
  player3.writeToMem();
  player4.writeToMem();
  lcd.clear();
  String playersNames[4] = {player1.name, player2.name, player3.name, player4.name};
  float wl[4] = {((float)player1.getWins()/player1.getGames()), ((float)player2.getWins()/player2.getGames()), ((float)player3.getWins()/player3.getGames()), ((float)player4.getWins()/player4.getGames())};
  sort(wl, playersNames, 4);
  
  lcd.clear();
  for(int i = 0; i < 4; i ++){
    lcd.setCursor(0,i);
    lcd.print(playersNames[i]);
    lcd.setCursor(15,i);
    //check for divide by zero error
    if (wl[i] == wl[i]){
      lcd.print(wl[i], 3);
    }else{
      lcd.print(0.000);
    }
  }
}

/*
 * Checks if button is pressed. Returns true if button pressed and updates last button int to correct button
 */
bool checkButtons(){
  if(digitalRead(button1) == HIGH){
    lastButton = 1;
    return true;
  }
  else if(digitalRead(button2) == HIGH){
    lastButton = 2;
    return true;
  }
  else if(digitalRead(button3) == HIGH){
    lastButton = 3;
    return true;
  }
  else if(digitalRead(button4) == HIGH){
    lastButton = 4;
    return true;
  }
  else{
    lastButton = -1;
    return false;
  }
}

/*
 * updates lcd to display the menu
 */
void menu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("New Game");
  lcd.setCursor(0,1);
  lcd.print("Clear Memory");
  lcd.setCursor(0,3);
  lcd.print("Cancel");
}

/*
 * updates lcd to display the new game screen
 */
void newGame(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Winner?");
  lcd.setCursor(20-player1.name.length(),0);
  lcd.print(player1.name);
  lcd.setCursor(20-player2.name.length(),1);
  lcd.print(player2.name);
  lcd.setCursor(20-player3.name.length(),2);
  lcd.print(player3.name);
  lcd.setCursor(20-player4.name.length(),3);
  lcd.print(player4.name);
}

/*
 * updates lcd to display clear menu
 */
void clearMenu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Are you sure");
  lcd.setCursor(17,2);
  lcd.print("Yes");
  lcd.setCursor(18,3);
  lcd.print("No");
}

void points(Player player){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(player.name);
  lcd.print(" VP?");
  if(p > 9){
    lcd.setCursor(18,0);
  }
  else{
    lcd.setCursor(19,0);
  }
  lcd.print(p);
  lcd.setCursor(19,1);
  lcd.print("^");
  lcd.setCursor(19,2);
  lcd.print("v");
  lcd.setCursor(15,3);
  lcd.print("Enter");
}

void pointIncrement(Player& player, Player& next, int newState){
  if(lastButton == 2){
      p ++;
      points(player);
    }
    if(lastButton == 3){
      p --;
      points(player);
    }
    if(lastButton == 4){
      state = newState;
      Serial.println(p);
      if(p > 0){
        player.incrementPoints(p);
      }else if(p <= 0){
        player.incrementLoss(-1);
      }
      p = 0;
      points(next);
    }
    lastButton = -1;
    delay(300);
}

void pointIncrement(Player& player, int newState){
  if(lastButton == 2){
      p ++;
      points(player);
    }
    if(lastButton == 3){
      p --;
      points(player);
    }
    if(lastButton == 4){
      state = newState;
      if(p > 0){
        player.incrementPoints(p);
      }else if(p <= 0){
        player.incrementLoss(-1);
      }
      p = 0;
      updateAll();
    }
    lastButton = -1;
    delay(300);
}

void sort(int a[], String b[], int size) {
  for(int i=0; i<(size-1); i++) {
    for(int o=0; o<(size-(i+1)); o++) {
      if(a[o] < a[o+1]) {
        int t = a[o];
        String u = b[o];
        a[o] = a[o+1];
        b[o] = b[o+1];
        a[o+1] = t;
        b[o+1] = u;
        }
      }
   }
}

void sort(float a[], String b[], int size) {
  for(int i=0; i<(size-1); i++) {
    for(int o=0; o<(size-(i+1)); o++) {
      if(a[o] < a[o+1]) {
        int t = a[o];
        String u = b[o];
        a[o] = a[o+1];
        b[o] = b[o+1];
        a[o+1] = t;
        b[o+1] = u;
        }
      }
   }
}
