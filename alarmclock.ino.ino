#include <TimeLib.h>
#include <Adafruit_RGBLCDShield.h>
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7 

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

time_t snoozeAlarm = now();
int MenuToggle = 0;
String alarmMode = "OFF";
time_t alarmTime = now();
int snoozeHours = 0;
int snoozeMins = 0;
int snoozeSecs = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  setTime(0,0,0,26,04,2016);//Setting time so that when setting alarm's second count will start at 0. 
  
  alarmTime = now();//Sets the alarm time to the time above.
  alarmTime +=23400;//Stops alarm going off immediately. Also automatically sets alarm time to 6:30 a.m. upon first boot.
  snoozeAlarm = alarmTime;
  
}

void loop() {
  uint8_t btns = lcd.readButtons();

  // put your main code here, to run repeatedly:
 switch(MenuToggle){
  //main display
  case 0:
    //lcd.setBacklight(WHITE);
    clockDisplay(); 
    lcd.setCursor(0,1);

    //Alarm mode dictator:
    if(alarmMode == "ON"){
     // if statement to check if the time is the same
       lcd.print("ALARM: ");
       print0(hourFormat12(alarmTime)); //Prints out alarm in 12 hour format
       printDigs(minute(alarmTime));
       switchampm();
      if(hour()==hour(alarmTime) && minute()== minute(alarmTime) && second()==second(alarmTime)){
             lcd.clear();
            MenuToggle = 9; 
        } else if (hour()==hour(snoozeAlarm) && minute() == minute(snoozeAlarm) && second()==second(snoozeAlarm)){
            lcd.clear();
            MenuToggle = 9;
        }
      
    } else {
          lcd.print("ALARM: " + alarmMode);
    } 
    
   if(btns){
       if(btns & BUTTON_SELECT){
          lcd.clear();
          MenuToggle = 1;
        }
       if(btns & BUTTON_DOWN){
        lcd.clear();
        alarmToggle();
       }
   }
    break;
    
  //Open menu  
  case 1: 
    lcd.setCursor(1,0);
    lcd.print("MENU SELECTION");  
    lcd.setCursor(4,1);
    lcd.print("< BACK >");

    if(btns){
        //Goes back to main menu.
       if(btns & BUTTON_SELECT){
          lcd.clear();
          MenuToggle = 0;
        }
      //Change to set time  
      if(btns & BUTTON_RIGHT){
        lcd.clear();
        MenuToggle = 2;
      }

      //Change to set alarm
      if(btns & BUTTON_LEFT){
       lcd.clear();
       MenuToggle = 3;
       }
      
    }
    break;

  //Set time selection
  case 2: 
    lcd.setCursor(1,0);
    lcd.print("MENU SELECTION");  
    lcd.setCursor(2,1);
    lcd.print("< Set Time >");
    
    //Select the set time option.
    if(btns){
     if(btns & BUTTON_SELECT){
          lcd.clear();
          MenuToggle = 4;
        }
  
    //Set Alarm option
    if(btns & BUTTON_RIGHT){
        lcd.clear();
        MenuToggle = 3;
      }

    //Go back option
    if(btns & BUTTON_LEFT){
       lcd.clear();
       MenuToggle = 1;
       }
   }   
     break;
  //Set alarm selection
 case 3:
    lcd.setCursor(1,0);
    lcd.print("MENU SELECTION");  
    lcd.setCursor(1,1);
    lcd.print("< Set Alarm >");

    if(btns){
    //Select the set alarm option.
     if(btns & BUTTON_SELECT){
          lcd.clear();
          MenuToggle = 7;
        }

    //Go back option
    if(btns & BUTTON_RIGHT){
        lcd.clear();
        MenuToggle = 1;
      }

    if(btns & BUTTON_LEFT){
       lcd.clear();
       MenuToggle = 2;
       }
    } 
     break;
     
//set time adjust hour
case 4: 
  lcd.setCursor(2,0);
  print0(hour());
  printDigs(minute());
  printDigs(second());
  amorpm();
  lcd.setCursor(2,1);
  lcd.print("--");
  
  if(btns){
    //INCREMENT hour
    if(btns & BUTTON_UP){
      adjustTime(3600);
    }
    
    //DECREMENT hour
    if(btns & BUTTON_DOWN){
      adjustTime(-3600);  
    }
    
    //SWAP to seconds
    if(btns & BUTTON_LEFT){
      MenuToggle = 6;
    }
    
    //SWAP to minutes
    if(btns & BUTTON_RIGHT){
      MenuToggle = 5;
    }
    //GO back to Menu Selection, Select Time 
    if(btns & BUTTON_SELECT){
      lcd.clear();
      MenuToggle = 2;
    }
  }
 break;

//Set Minutes
case 5: 
  lcd.setCursor(2,0);
  print0(hour());
  printDigs(minute());
  printDigs(second());
  amorpm();
  lcd.setCursor(5,1);
  lcd.print("--");
  
  
  if(btns){
    //INCREMENT minutes
    if(btns & BUTTON_UP){
      adjustTime(60);
    }
    
    //DECREMENT minutes
    if(btns & BUTTON_DOWN){
      adjustTime(-60);  
    }
    
    //SWAP to hours
    if(btns & BUTTON_LEFT){
      lcd.clear();
      MenuToggle = 4;
    }
    
    //SWAP to seconds
    if(btns & BUTTON_RIGHT){
      lcd.clear();
      MenuToggle = 6;
    }
    //GO back to Menu Selection, Select Time 
    if(btns & BUTTON_SELECT){
      lcd.clear();
      MenuToggle = 2;
    }
  }
 break; 

//Adjust seconds
 case 6: 
  lcd.setCursor(2,0);
  print0(hour());
  printDigs(minute());
  printDigs(second());
  amorpm();
  lcd.setCursor(8,1);
  lcd.print("--");
  
  if(btns){
    //INCREMENT seconds
    if(btns & BUTTON_UP){
      adjustTime(1);
    }
    
    //DECREMENT seconds
    if(btns & BUTTON_DOWN){
      adjustTime(-1);  
    }
    
    //SWAP to minutes
    if(btns & BUTTON_LEFT){
      lcd.clear();
      MenuToggle = 5;
    }
    
    //SWAP to hours
    if(btns & BUTTON_RIGHT){
      lcd.clear();
      MenuToggle = 4;
    }
    //GO back to Menu Selection, Select Time 
    if(btns & BUTTON_SELECT){
      lcd.clear();
      MenuToggle = 2;
    }
  }
 break;

//Alarm setting change hours
case 7: 
  lcd.setCursor(3,0);
  print0(hourFormat12(alarmTime));
  printDigs(minute(alarmTime));
  second(alarmTime); 
  //Need to put seconds in so that the alarm check can tell 
  switchampm();//swithces alarm between am and p.m. when setting alarm
  lcd.setCursor(3,1);
  
  lcd.print("--");

  if(btns){
    //INCREMENT hour
    if(btns & BUTTON_UP){
      alarmTime +=3600 ;
    }
    if(btns & BUTTON_DOWN){
      alarmTime -=3600 ;
    }
    
    //SWAP to minutes
    if(btns & BUTTON_LEFT){
      lcd.clear();
      MenuToggle = 8;
    }
   
    //SWAP to minutes
    if(btns & BUTTON_RIGHT){
      lcd.clear();
      MenuToggle = 8;
    }
    
    //GO back to Menu Selection, Select Alarm
    if(btns & BUTTON_SELECT){
      lcd.clear();
      alarmMode = "ON";
      MenuToggle = 3;
    }
  }
 break;


//Change minutes in alarm
 case 8:
   lcd.setCursor(3,0);
  print0(hourFormat12(alarmTime));
  printDigs(minute(alarmTime));
  second(alarmTime);
  switchampm();
  lcd.setCursor(6,1);
  lcd.print("--");

  if(btns){
    //INCREMENT minutes
    if(btns & BUTTON_UP){
      alarmTime += 60;
    }
    //DECREMENT minutes
    if(btns & BUTTON_DOWN){
      alarmTime -= 60;
    }
    
    //SWAP to seconds
    if(btns & BUTTON_LEFT){
      lcd.clear();
      MenuToggle = 7;
    }
   
    //SWAP to seconds
    if(btns & BUTTON_RIGHT){
      lcd.clear();
      MenuToggle = 7;
    }
    
    //GO back to Menu Selection, Select Alarm
    if(btns & BUTTON_SELECT){
      lcd.clear();
      alarmMode = "ON";
      MenuToggle = 3;
    }
  }
 break;

 //Turn alarm ON 
 case 9:  
    clockDisplay();
    lcd.setCursor(4,1);
    lcd.print("WAKE UP!");
    
    //Flashing background
    lcd.setBacklight(GREEN);
    delay(500);
    lcd.setBacklight(VIOLET);
    delay(500);
    lcd.setBacklight(BLUE);
    delay(500);
    lcd.setBacklight(RED);
    delay(500);
    
    //Turning alarm off 
    if(btns){
      if(btns & BUTTON_SELECT){
        lcd.clear();
        snoozeAlarm = alarmTime;
        alarmToggle();
        lcd.setCursor(1,0);
        lcd.print("ALARM SWITCHED");
        lcd.setCursor(6,1);
        lcd.print("OFF!");
        delay(2000);
        lcd.clear();
        MenuToggle = 0;
      }
      
    //Snooze the alarm
      if(btns & BUTTON_DOWN){
        lcd.clear();
        snooze();
        /*snooze();
        lcd.setCursor(6,0);
        lcd.print("Snooze for 1 minute");
         for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
             lcd.scrollDisplayLeft();
            delay(150);
         }
         lcd.clear();*/
        MenuToggle = 10;
      }
    }
  break;

//Change the hours user wants to snooze for. 
case 10: 
  lcd.setCursor(0,0);
  snoozeSettings();
  lcd.print("HOURS: ");
  print0(snoozeHours);
  printDigs(snoozeMins);
  printDigs(snoozeSecs);

  lcd.setCursor(0,1);
  lcd.print("<BACK SEL:SNOOZE");

  if(btns){
    //INCREMENT minutes
    if(btns & BUTTON_UP){
      snoozeHours += 1;
    }
    //DECREMENT minutes
    if(btns & BUTTON_DOWN){
      snoozeHours -= 1;
    }
    
    //SWAP to seconds
    if(btns & BUTTON_LEFT){
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("ALARM SWITCHED");
      lcd.setCursor(6,1);
      lcd.print("OFF!");
      delay(2000);
      lcd.clear();
      alarmMode = "OFF";
      MenuToggle = 0;
    }

    if(btns & BUTTON_RIGHT){
      lcd.clear();
      MenuToggle = 11;
    }
   
    //GO back to main display
    if(btns & BUTTON_SELECT){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Snoozed for");

      //Display snooze time amount
      lcd.setCursor(0,1);
      lcd.print(snoozeHours);
      lcd.print("h ");
      print0(snoozeMins); 
      lcd.print("m ");
      print0(snoozeSecs);
      lcd.print("s.");
      delay(1000);
      snooze();
      lcd.clear();
      alarmMode = "ON";
      MenuToggle = 0;
    }
  }
 break;

//Change mins on snooze time
case 11:
  lcd.setCursor(0,0);
  snoozeSettings();
  lcd.print("MINS: ");
  print0(snoozeHours);
  printDigs(snoozeMins);
  printDigs(snoozeSecs);

  lcd.setCursor(0,1);
  lcd.print("<BACK SEL:SNOOZE");

  if(btns){
    //INCREMENT minutes
    if(btns & BUTTON_UP){
      snoozeMins += 1;
    }
    //DECREMENT minutes
    if(btns & BUTTON_DOWN){
      snoozeMins -= 1;
    }
    
    //SWAP to seconds
    if(btns & BUTTON_LEFT){
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("ALARM SWITCHED");
      lcd.setCursor(6,1);
      lcd.print("OFF!");
      delay(2000);
      lcd.clear();
      alarmMode = "OFF";
      MenuToggle = 0;
    }

    if(btns & BUTTON_RIGHT){
      lcd.clear();
      MenuToggle = 12;
    }
   
    //GO back to main display
    if(btns & BUTTON_SELECT){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Snoozed for");
      
      //Display snooze time amount
      lcd.setCursor(0,1);
      lcd.print(snoozeHours);
      lcd.print("h ");
      print0(snoozeMins); 
      lcd.print("m ");
      print0(snoozeSecs);
      lcd.print("s.");
      delay(1000);
      snooze();
      lcd.clear();
      alarmMode = "ON";
      MenuToggle = 0;
    }
  }
 break;

//Change seconds in snooze time
 case 12:
 lcd.setCursor(0,0);
  snoozeSettings();
  lcd.print("SECS: ");
  print0(snoozeHours);
  printDigs(snoozeMins);
  printDigs(snoozeSecs);

  lcd.setCursor(0,1);
  lcd.print("<BACK SEL:SNOOZE");

  if(btns){
    //INCREMENT seconds
    if(btns & BUTTON_UP){
      snoozeSecs += 1;
    }
    //DECREMENT seconds
    if(btns & BUTTON_DOWN){
      snoozeSecs -= 1;
    }
    
    //Turns off alarm
    if(btns & BUTTON_LEFT){
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("ALARM SWITCHED");
      lcd.setCursor(6,1);
      lcd.print("OFF!");
      delay(2000);
      lcd.clear();
      alarmMode = "OFF";
      MenuToggle = 0;
    }

    if(btns & BUTTON_RIGHT){
      lcd.clear();
      MenuToggle = 10;
    }
   
    //GO back to main display
    if(btns & BUTTON_SELECT){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Snoozed for");

      //Display snooze time amount
      lcd.setCursor(0,1);
      lcd.print(snoozeHours);
      lcd.print("h ");
      print0(snoozeMins); 
      lcd.print("m ");
      print0(snoozeSecs);
      lcd.print("s.");
      delay(2000);
      snooze();
      lcd.clear();
      alarmMode = "ON";
      MenuToggle = 0;
    }
  }
 break;

  
  }//end of switch block
  
}//end of void loop


/***********
 * 
 *METHODS
 *
  *********/
//Clock display
int clockDisplay(){
    lcd.setCursor(5,0);
    lcd.print(hour());
    printDigs(minute());
    amorpm();

    //Daylight Savings functionality :3
    if(hour()>= 7 && hour() <=18){
      lcd.setBacklight(OFF);
    } else{
      lcd.setBacklight(WHITE);
    }
}

//Checks if the time being set is a.m. or p.m. 
void amorpm(){
  lcd.print(" ");
  if(isAM()){      
      lcd.print("A.M.");
    } else{
      lcd.print("P.M.");
    }
}

//Will switch to tell if the time being shown is am or pm. Useful for setting alarm.
void switchampm(){
  lcd.print(" ");
  if(hour(alarmTime)>=12){
    lcd.print("P.M.");
  } else {
    lcd.print("A.M.");
  }
}

//Prints out the ":" between the times
void printDigs(int digits){
  lcd.print(":");
  if(digits<10)
    lcd.print('0');
  lcd.print(digits);
}

//Will print out 0 infront of hour when changing time
void print0(int digits){
  if(digits<10)
    lcd.print('0');
  lcd.print(digits);
}

//Toggle the alarm ON or OFF
void alarmToggle(){
  if(alarmMode == "ON"){
    alarmMode = "OFF";
  } else if (alarmMode == "OFF"){
    alarmMode = "ON";
  } 
}

//Snooze function to set the alarm to snooze for a minute - for testing purposes
void snooze(){
  snoozeAlarm = now();
  snoozeAlarm += (snoozeHours*3600) + (snoozeMins *60) + snoozeSecs; 
  //This will add the total of time provided by the three snooze integers into seconds which can be added to snoozeAlarm's value
  
}

//Controls the the time through changing the values of integers. This is used in the snooze cases
void snoozeSettings(){
  
  if(snoozeHours>23){
    snoozeHours = 0;
    snoozeMins = 0;
    snoozeSecs = 0;
  }

 
  if(snoozeHours<0){
    snoozeHours = 23;
    snoozeMins = 59;
    snoozeSecs = 59;
  }

  if(snoozeMins > 59){
    snoozeMins = 0;
    snoozeSecs = 0;
  }

  if(snoozeMins < 0){
    snoozeMins = 59;
  }
  
  if(snoozeSecs > 59){
    snoozeMins +=1;
    snoozeSecs = 0;
  }

  if(snoozeSecs < 0){
    snoozeMins -=1;
    snoozeSecs = 59;
  }
    
}



