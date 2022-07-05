//sketch created by Akshay Joseph
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

int charCnt = 0;
String inputPin = "";
String pinCode = "1234";
int auth = 0;
int cursorBlink = 1;

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

LiquidCrystal_I2C lcd(0x27, 16, 2);

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()                                                                                      
{
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PIN: ");
}

void loop()
{
  if (cursorBlink == 1)
    lcd.blink();
  
  char key = keypad.getKey();
  if (key){


    if (auth == 0 && key == '#') {
      charCnt--;
      
      if (charCnt >= 0) {
        inputPin.remove(charCnt);
        lcd.setCursor(5 + charCnt, 0);
        lcd.print(" ");
        lcd.setCursor(5 + charCnt, 0);
      } else {
        inputPin = "";
        charCnt = 0;
      }
    } else if (auth == 1 && key == 'A') {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Change PIN");
    } else if (auth == 1 && key == 'C') {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Logout");
    } else if (auth == 1 && key == 'D') {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reboot");
      for (int i = 5; i >= 1; i--) {
        lcd.setCursor(0,1);
        lcd.print(i);
        delay(1000);
      }
      resetFunc();
    } else if(auth == 0 && key == '*') {
      lcd.setCursor(0,1);

      if (inputPin == pinCode) {
        lcd.print("SUCCESS!        ");
        auth = 1;
        lcd.noBlink();
        cursorBlink = 0;
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Welcome!");
      } else {
        lcd.print("Incorrect PIN   ");
        lcd.setCursor(0,0);
        lcd.print("PIN:            ");
        lcd.setCursor(5,0); 
      }    
      charCnt = 0;
      inputPin = ""; 
    } else {
      if (auth == 0){
        charCnt++;
        lcd.print(key);
        inputPin += key;
      }
    }
  }
}
