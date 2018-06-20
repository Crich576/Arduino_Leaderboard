/*
 * Player.cpp - Library for keeping track of catan player information such as wins, losses, etc
 * Created by Cater A. Richmond, April 16, 2018.
 */

#include "Arduino.h"
#include "EEPROM.h"
#include "Player.h"

Player::Player(String player, int wins, int losses, int points, int start){
	name = player;
	playerWins = wins;
	playerLosses = losses;
	playerPoints = points;
	startMemLocation = start;
}

void Player::readFromMem(){
  playerWins = EEPROM.read(startMemLocation);
  playerLosses = EEPROM.read(startMemLocation + 1);
  playerPoints = EEPROM.read(startMemLocation + 2);
}

void Player::writeToMem(){
  EEPROM.write(startMemLocation, playerWins);
  EEPROM.write(startMemLocation + 1, playerLosses);
  EEPROM.write(startMemLocation + 2, playerPoints);
}

void Player::incrementWin(int amount){
	playerWins+= amount;
}

void Player::incrementLoss(int amount){
	playerLosses = (playerLosses + amount);
}

void Player::incrementPoints(int amount){
	playerPoints+= amount;
}

int Player::getWins(){
	return playerWins;
}

int Player::getLosses(){
	return playerLosses;
}

int Player::getGames(){
	return playerWins + playerLosses;
}

int Player::getPoints(){
	return playerPoints;
}

void Player::clearMem(){
	playerWins = 0;
	playerLosses = 0;
	playerPoints = 0;
	writeToMem();
}

void Player::updateScore(int wins, int losses, int points){
	playerWins = wins;
	playerLosses = losses;
	playerPoints = points;
}