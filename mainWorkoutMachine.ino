/*

*/

#include <TimeLib.h>
#include <LiquidCrystal.h>


int 
    Day = 22, mnth = 6,  yr = 2020,    hr = 19, min = 03, sec = 0;
int
    rs = 13,
    e = 12,
    d0 = 11,
    d1 = 10,
    d2 = 9,
    d3 = 8,
    d4 = 7,
    d5 = 6,
    d6 = 5,
    d7 = 4,
    beeper = 3,
    leftBut = A0,
    pauseBut = A1,
    rightBut = A2,
    beepDelay = 500,
    beeps = 5,
    alarmMin = 0,
    alarmState = 1
    ;
int rndmNum[8];
unsigned long 
  l,p,r,t;
int timerSec = 00;
int timerMin;

int item = 0;
int start, stop;

typedef struct Exercise{
  char* exercise;
  int duration;
};

int routine[4][2] = {{0,10},{10,19},{19,28},{28,34}};
Exercise workout[] = 
{{"7-minAb'v'Twists",7},{
  "SeatedAbCirclesCW",60},{"SeatedAbCirclesCCW",60},{"Drunken Mountain Climbers",60},{"Rest Period",30},{"Marching Planks",60},{"Scissors",60},{"Starfish Crunch",30},{"Rest Period",30},{"Russian 'v' Tuck Twists",30},
{"5minAbScissorVup",5},{
  "Figure 8's",30},{"HandsBackRasises",60},{"Twisting Pistons",60},{"Rest Period",30},{"SeatedAbCirclesCW",30},{"SeatedAbCirclesCCW",30},{"Scissor 'v' Ups",60},{"21 Crunch",60},
{"7-minAb WindWipe",7},{
  "Figure 8's",60},{"Windshield Wipers",60},{"Twisting Pistons",60},{"Rest Period",30},{"Starfish Crunch",30},{"Tuck Planks",60},{"UpperCircleCrunches CW",60},{"UpperCircleCrunches CCW",60},
{"5-min NO REST   ",5}, {
  "Halos CW",30},{"Halos CCW",30},{"Butt Twists",60},{"Plank Reachouts",60},{"Chainsaws",60},{"Situp Rollups",60}
};

LiquidCrystal lcd(rs, e, d0, d1, d2, d3, d4, d5, d6, d7);
void setup() {
  pinMode(rs, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(d0, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);
  pinMode(beeper, OUTPUT);
  pinMode(pauseBut, INPUT);
  pinMode(leftBut, INPUT);
  pinMode(rightBut, INPUT);
  lcd.begin(16,2);
  setTime(hr,min,sec,Day,mnth,yr);
  randomSeed(analogRead(5));
  
}

void loop() {

  //Add a clock: Date on cursor(0,0), Time on (0,8).
  displayTime();
  //Add alarm at 7:00.
  while (hourFormat12() == 7 && minute() == alarmMin && alarmState && isAM())
  {
    tone(beeper,300);
    if (digitalRead(pauseBut)){noTone(beeper); alarmState = 0;}
    else if (digitalRead(leftBut) or digitalRead(rightBut)) {noTone(beeper); alarmMin += 30;}
    else if (second() > 25) {noTone(beeper); alarmMin += 30;}
    delay(20);
  }
  //Stop alarm at press of pause button ^
  //Snooze for thirty minutes at press of left or right button ^
  //Display random assortment of characters to memorize for 1 minute:
  while (!alarmState)
  {
    lcd.clear();
    createRandomChar();//Creates and Displays the characters for a minute.
    delay(60000); // Change this to 60,000 when finalized.
    alarmState = 1;
    alarmMin = 0;
    lcd.clear();
    displayTime();
  }
  //Start workout routine when pause button pressed twice within 1000 ms:
  
  if (digitalRead(pauseBut) && millis() - p < 1000 && millis() - p > 100){
    exercise();
  }
  if (digitalRead(pauseBut)){p = millis();}
  //When Left button pressed twice to reveal the previous random characters:
  revealChar();
  // Reset variables as needed at the end of the day: NO NEED.
  
}
void displayTime() {
    lcd.setCursor(0,0);
  switch(weekday()) {
    case 1:
        lcd.print("Sun");
        break;
    case 2:
        lcd.print("Mon");
        break;
    case 3:
        lcd.print("Tue");
        break;
    case 4:
        lcd.print("Wed");
        break;
    case 5:
        lcd.print("Thu");
        break;
    case 6:
        lcd.print("Fri");
        break;
    case 7:
        lcd.print("Sat");
        break;
  }
  lcd.print(",");
    switch(month()) {
    case 1:
        lcd.print("Jan");
        break;
    case 2:
        lcd.print("Feb");
        break;
    case 3:
        lcd.print("Mar");
        break;
    case 4:
        lcd.print("Apr");
        break;
    case 5:
        lcd.print("May");
        break;
    case 6:
        lcd.print("Jun");
        break;
    case 7:
        lcd.print("Jul");
        break;
    case 8:
        lcd.print("Aug");
        break;
    case 9:
        lcd.print("Sep");
        break;
    case 10:
        lcd.print("Oct");
        break;
    case 11:
        lcd.print("Nov");
        break;
    case 12:
        lcd.print("Dec");
        break;
  }
  lcd.print(" "); lcd.print(day()); lcd.print("  ");
  lcd.setCursor(4,1); 
  lcd.print(hourFormat12()); lcd.print(":"); 
  lcd.print(minute()); lcd.print(":"); 
  lcd.print(second()); lcd.print("   ");
}

void createRandomChar() {
  rndmNum[0] = random(0,10);
  lcd.print(rndmNum[0]);
  for (int i = 1; i<9; i++)
  {
    rndmNum[i] = random(65,91);
    delay(20);
    lcd.print((char) rndmNum[i]);
  }
  rndmNum[9] = random(0,10);
  lcd.print(rndmNum[9]);
}

void revealChar() {
 
    if (digitalRead(leftBut) && millis() - l < 1000 && millis() - l > 100) //Check this before starting timer.
    {
      lcd.clear();
      lcd.print(rndmNum[0]);
      for (int i = 1; i<9; i++)
      {
        lcd.print((char) rndmNum[i]);
        delay(20);
      }
      lcd.print(rndmNum[9]);
      delay(15000);
    } 
    if (digitalRead(leftBut)) { // if leftBut is pressed start timer first.
    l = millis();
  }
}

void startEx(int i, char* tempEx, int tempDur){ // temp variables so the original don't get changed.
    while (true) {
      lcd.clear();lcd.home();
      lcd.print(tempEx);
      lcd.setCursor(0,1);
      lcd.print(tempDur--);
      lcd.setCursor(10,1);
      lcd.print(timerMin); lcd.print(":");
      lcd.print(timerSec++);
      if (timerSec == 60) {timerSec = 0; timerMin++;}
      delay(1000); //Wait one sec before incrementing timers.
      if (tempDur < 0) {lcd.clear();tone(beeper,100,500);break;} // exit once workout finishes.
      if (digitalRead(rightBut)) {lcd.clear();break;} // hold for 1 sec to exit.
  }// end while
}

void exercise() {
  lcd.clear();delay(500); // clear to print new stuff; and delay to give time to ease off pauseBut.
  while (true) {
    
    if(item > 3) {item = 0;} if(item < 0) {item = 3;}// Swipe in a circle.
      lcd.home();
      lcd.print(workout[routine[item][0]].exercise);
    if (digitalRead(rightBut)) {item++; delay(300);}//Less than 300 ms constitutes a press. Move to next workout.
    if (digitalRead(leftBut)) {item--; delay(300);}// move to previous workout.
    
    if (digitalRead(pauseBut)) {delay(900);
      if (longPress()) {//if 'home button' held for 1 second, exit. 
        lcd.clear();
        break;
      } 
      else { // else do the selected exersize.
        
        start = routine[item][0];
        stop = routine[item][1];
        timerMin = 0;
        for (int i = start+1; i<stop;i++)
          {   
            startEx(i,workout[i].exercise,workout[i].duration+5); //create temp variables for workouts; +5 seconds for time to get ready.
              delay(500);// give 1/2 second to unpress skip button.
          } //end for.
          tone(beeper,500,1000);
        } // end else.
      }// end main if.
  }//end while.
}// end function.
bool longPress(){
  t = millis();
  while (millis()-t<100){ //if pauseBut is pressed within 100 ms. cuz its unreliable; cant be pressed exactly after 1000 ms.
    if (digitalRead(pauseBut))
    {t=0; return true;}
  }
  t = 0;
  return false;
}


