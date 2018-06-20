/*
 * Player.h - Library for keeping track of catan player information such as wins, losses, etc
 * Created by Cater A. Richmond, April 16, 2018.
 */ 

#ifndef Player_h
#define Player_h

#include "Arduino.h"

class Player{
  public:
    Player(String name, int wins, int losses, int points, int start);
	String name;
    void readFromMem();
    void writeToMem();
    void incrementWin(int amount);
    void incrementLoss(int amount);
    void incrementPoints(int amount);
    void updateScore(int wins, int losses, int points);
    void clearMem();
	int getWins();
	int getLosses();
	int getGames();
	int getPoints();
  private:
    int playerWins;
    int playerLosses;
    int playerPoints;
    int startMemLocation;
};

#endif