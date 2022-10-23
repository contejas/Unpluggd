/*
HackGT 9 Code
*/

// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, lft = 8, rt = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myservo1;
Servo myservo2;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(lft, INPUT_PULLUP);                 //Setup button pins
  pinMode(rt, INPUT_PULLUP);
  myservo1.attach(9);                        //Setup Servo pins
  myservo1.write(180);
  myservo2.attach(13);
  myservo2.write(0);
  lcd.setCursor(0,0);                         //Prints opening message when arduino is powered
  lcd.print("Click to open");
}

void loop() {
  boolean rightClicked;                       //Variables right and left Clicked
  boolean leftClicked;                        //tell when to read buttons based on if button already clicked
  startPage(rightClicked, leftClicked);       //calls start page method which prints to lcd
}


void startPage(boolean rightClicked, boolean leftClicked) {
                                              //If either button is clicked, executes if block
  if(digitalRead(rt) == 0 || digitalRead(lft) == 0) {
    rightClicked = (digitalRead(rt) == 0);    //determines whether the right button was clicked
    leftClicked = !rightClicked;              //if right was clicked, right was not clicked (practically for speed of transmission)
    lcd.clear();                              //clears display
    lcd.print("LEFT:");                       //following lines set display
    lcd.setCursor(10,0);
    lcd.print("RIGHT:");
    if(rightClicked) {                        //if right was clicked, prints 'ready' under right
      myservo1.write(0);
      lcd.setCursor(10,1);
      lcd.print("READY");
    } else {                                  //if left was clicked, 'ready under left
      lcd.setCursor(0,1);
      lcd.print("READY");
      myservo2.write(180);
    }
    readyPage(rightClicked, leftClicked);     //calls to ready page with values of left and right clicked
  }
}
void readyPage(boolean rightClicked, boolean leftClicked) {
 while(true) {                                //infinite loop checks for left to be clicked
  if(rightClicked) {
     if (digitalRead(lft) == 0) {
          lcd.setCursor(0,1);                 //prints 'ready'
          lcd.print("READY");
          myservo2.write(180);
          break;                              //moves motor to open position
     }
   } else {
     if (digitalRead(rt) == 0) {              //checks for right to be clicked
          lcd.setCursor(10,1);
          lcd.print("READY");                 //prints 'ready'
          myservo1.write(0);
          break;                              //moves motor to open position
          
     }
   }
 }
 delay(1000);                                 //delays 1 second before five second countdown
 lcd.setCursor(7,0);
 int n;
 for(n = 5; n >0; n--) {
  lcd.print(n);
  lcd.setCursor(7,0);
  delay(1000);
 }                                            //five second count down complete
lcd.clear();
lcd.setCursor(0,0);                           //clears and prints 'click when ready'
lcd.print("CLICK WHEN READY");

int num = 0;                                  //variable checks to see how many of right and left have been clicked
int inicior = 0;                              //initial timestamp when right clicked
int iniciol = 0;                              //initial timestamp when left clicked
int tiempor;                                  //incremented time for right clock
int tiempol;                                  //incremented time for left clock
boolean rc = false;                           //tells whether right has been clicked
boolean lc = false;                           //tells whether left has been clicked
boolean rightDone = false;
boolean leftDone = false;
while(true) {
  tiempor = (millis()/1000 - inicior);        //sets the right clock time to current computer time - initial right time
  tiempol = (millis()/1000 - iniciol);        //sets the left clock time to current computer time - initial left time
  if (num == 2) {
    //delay(500);
    if (digitalRead(rt) == 0) {
      rightDone = true;
      lcd.setCursor(10,0);
      lcd.print("DONE ");
      myservo1.write(0);
    }
    if (digitalRead(lft) == 0) {
      leftDone = true;
      lcd.setCursor(0,0);
      lcd.print("DONE ");
      myservo2.write(180);
    }
  }
  if (!rc) {                                  //if right hasn't been clicked
    rc = (digitalRead(rt) == 0);              //set rc to value of (right is clicked)
    if (rc) {                                 //if right was clicked
      num++;                                  //increment number of clicked buttons
      inicior = millis()/1000;                //take the initial timestamp
      myservo1.write(180);                      //close the box
      if (num == 1) {                         //if this was the first button clicked, print header
      printHeader();
      } else {
        delay(200);
      }
    }
  } else {                                    //if right has been clicked already, call rightClock, passing in the right time
    if (!rightDone) {
      rightClock(tiempor);
    }  
  }
  if (!lc) {                                  //if left button has not been clicked
    lc = (digitalRead(lft) == 0);             //check if left button is clicked, save this to lc
    if(lc) {
      iniciol = millis()/1000;                //take timestamp at time of left click
      myservo2.write(0);
      num++;                                  //increment number of buttons clicked
      if (num == 1) {                         //if this is the first button clicked print header
      printHeader();
     } else {
      delay(200);
     }
    }
  } else {                                    //if left already clicked, call left clock with left time
    if (!leftDone) {
      leftClock(tiempol);
    }
  }
}    
}

void rightClock(int r) {                      //takes current time in
  lcd.setCursor(14,1);                        //bring cursor to right time position, prints value digit by digit
  lcd.print(r % 10);
  lcd.setCursor(13,1);
  lcd.print((r % 60)/10);

  lcd.setCursor(11,1);
  lcd.print((r % 3600)/60);
  lcd.setCursor(10,1);
  lcd.print((r%216000)/3600);
  delay(10);
}
void leftClock(int l) {                       //takes current time in
  lcd.setCursor(4,1);                         //bring cursor to left time position, prints value digit by digit
  lcd.print(l % 10);  
  lcd.setCursor(3,1);
  lcd.print((l % 60)/10);

  lcd.setCursor(1,1);
  lcd.print((l % 3600)/60);
  lcd.setCursor(0,1);
  lcd.print((l%216000)/3600);
  delay(10);
}

void printHeader() {                          //Prints the header for the timing page, including the zeros in the clock area to the lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LEFT");
  lcd.setCursor(0,1);
  lcd.print("00:00");
  lcd.setCursor(10,0);
  lcd.print("RIGHT");
  lcd.setCursor(10,1);
  lcd.print("00:00");
  delay(10);
}
