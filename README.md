# Arduino_Leaderboard (Current V1.0)

This is an exercise in creating an arduino based digital Leaderboard. This was originally intended to keep track of statistics of the 
board game Settlers of Catan. This is an inpractical device that was made for the sole purpose to have a novelity lcd based leaderboard to
keep track of the winners of our catan games. I realize this code is not as efficent as it can be and I am open to suggestions on how to improve it

## Getting Started

This project was originally designed around a standard arduino uno r3 with a 20x4 lcd panel using the I2C protocol on pins A5 and A4. 
There are also 4 buttons on pins 7, 8, 9, and 10. The specific board or LCD panel is not important to the code

### Prerequisites
```
Install the included player library by placeing it in the arduino libraries folder
```
```
Install the Arduino LCD I2C library found [here](https://github.com/boutboutnico/LCD_I2C_lib)
```

## Planned Features
```
Alterable player names
Revised Screen State System
Updated Event Based programming to not rely on arduino main loop
Battery saving messures
Bluetooth companion app to read more data
```

## Author
* **Carter Richmond** - *Initial work* - [Crich576](https://github.com/Crich576)

## Acknowledgments
Thank you to [nboutin](https://github.com/boutboutnico) for his LCD I2C library
