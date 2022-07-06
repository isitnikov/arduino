/*
  Showing number 0-9 on a Common Anode 7-segment LED display
  Displays the numbers 0-9 on the display, with one second inbetween.
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
  This example code is in the public domain.
 */
 
// Pin 2-8 is connected to the 7 segments of the display.

const int SYMBOLS = 26;
const int PINS = 7;
const int DIGITS = 4;

int pins[PINS] = {
  2, 3, 4, 5, 6, 7, 8
};

int digitPointer = 0;

int digits[] = {
  9, 10, 11, 12
};

int symbols[SYMBOLS][PINS] = {
// A     B     C     D     E     F     G
// 0     1     2     3     4     5     6
// 
  {LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  HIGH}, // 0
  {HIGH, LOW,  LOW,  HIGH, HIGH, HIGH, HIGH}, // 1
  {LOW,  LOW,  HIGH, LOW,  LOW,  HIGH, LOW},  // 2
  {LOW,  LOW,  LOW,  LOW,  HIGH, HIGH, LOW},  // 3
  {HIGH, LOW,  LOW,  HIGH, HIGH, LOW,  LOW},  // 4
  {LOW,  HIGH, LOW,  LOW,  HIGH, LOW,  LOW},  // 5
  {LOW,  HIGH, LOW,  LOW,  LOW,  LOW,  LOW},  // 6
  {LOW,  LOW,  LOW,  HIGH, HIGH, HIGH, HIGH}, // 7
  {LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW},  // 8
  {LOW,  LOW,  LOW,  LOW,  HIGH, LOW,  LOW},  // 9
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}, // Space
  {LOW,  LOW,  LOW,  HIGH, LOW,  LOW,  LOW},  // A
  {LOW,  HIGH, HIGH, LOW,  LOW,  LOW,  HIGH}, // C
  {LOW,  HIGH, HIGH, LOW,  LOW,  LOW,  LOW},  // E
  {LOW,  HIGH, HIGH, HIGH, LOW,  LOW,  LOW},  // F
  {LOW,  HIGH, LOW,  LOW,  LOW,  LOW,  LOW},  // G
  {HIGH, LOW,  LOW,  HIGH, LOW,  LOW,  LOW},  // H
  {HIGH, LOW,  LOW,  LOW,  LOW,  HIGH, HIGH}, // J
  {HIGH, HIGH, HIGH, LOW,  LOW,  LOW,  HIGH}, // L
  {LOW,  LOW,  HIGH, HIGH, LOW,  LOW,  LOW},  // P
  {HIGH, LOW,  LOW,  LOW,  LOW,  LOW,  HIGH}, // U
  {HIGH, LOW,  LOW,  LOW,  HIGH, LOW,  LOW},  // Y
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},  // -
  {HIGH, HIGH, HIGH, LOW,  HIGH, HIGH, HIGH}, // _
  {HIGH, HIGH, HIGH, LOW,  HIGH, HIGH, LOW},  // =
  {HIGH, HIGH, HIGH, HIGH, LOW,  LOW, HIGH}   // |
};

char symbolMap[SYMBOLS] = {
  '0', '1', '2', '3', '4', '5',
  '6', '7', '8', '9', ' ', 'A',
  'C', 'E', 'F', 'G', 'H', 'J',
  'L', 'P', 'U', 'Y', '-', '_',
  '=', '|'
};

void printSymbol(char symbol, int digit)
{
  int index = findSymbolIndex(symbol);
  if (index != -1){
    if (symbols[index] && digits[digit])
    {
      int iterate = 0;
      
      do {        
        if (digit == iterate) {     
          digitalWrite(digits[iterate], HIGH);
        }
        iterate++;
      } while (iterate < DIGITS);

      iterate = 0;

      do {        
        digitalWrite(pins[iterate], symbols[index][iterate]);
        iterate++;
      } while (iterate < PINS);
      
      delay(1);
      
      digitPointer++;
      if (digitPointer == DIGITS) {
        digitPointer = 0;
      }
    }
  }
}

int findSymbolIndex(const char * key)
{
  for ( int i = 0; i < SYMBOLS; i++ )
    if ( symbolMap[i] == key) {
      return i;
    }
  return -1;     
}

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);

  int iterate = 0;

  do {
    pinMode(pins[iterate], OUTPUT);  
    iterate++;       
  } while (iterate < PINS);

  iterate = 0;
  do {
    pinMode(digits[iterate], OUTPUT);  
    iterate++;       
  } while (iterate < DIGITS); 
}

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    Serial.print("I received: ");
    Serial.print(incomingByte);
    Serial.print(" means ");
    printSymbol(incomingByte, digitPointer);
    Serial.println();
  }
//  digitalWrite(D1, HIGH);
//  digitalWrite(D2, LOW);
//  digitalWrite(D3, LOW);
//  digitalWrite(D4, LOW); 
//  //0
//  digitalWrite(pinA, LOW);   
//  digitalWrite(pinB, LOW);   
//  digitalWrite(pinC, LOW);   
//  digitalWrite(pinD, LOW);   
//  digitalWrite(pinE, LOW);   
//  digitalWrite(pinF, LOW);   
//  digitalWrite(pinG, HIGH);   
//  delay(1);               // wait for a second
//  
//  digitalWrite(D1, LOW);
//  digitalWrite(D2, HIGH);
//  digitalWrite(D3, LOW);
//  digitalWrite(D4, LOW); 
//  //1
//  digitalWrite(pinA, HIGH);   
//  digitalWrite(pinB, LOW);   
//  digitalWrite(pinC, LOW);   
//  digitalWrite(pinD, HIGH);   
//  digitalWrite(pinE, HIGH);   
//  digitalWrite(pinF, HIGH);   
//  digitalWrite(pinG, HIGH);   
//  delay(1);               // wait for a second
//  
//  digitalWrite(D1, LOW);
//  digitalWrite(D2, LOW);
//  digitalWrite(D3, HIGH);
//  digitalWrite(D4, LOW); 
//  //2
//  digitalWrite(pinA, LOW);   
//  digitalWrite(pinB, LOW);   
//  digitalWrite(pinC, HIGH);   
//  digitalWrite(pinD, LOW);   
//  digitalWrite(pinE, LOW);   
//  digitalWrite(pinF, HIGH);   
//  digitalWrite(pinG, LOW);     
//  delay(1);               // wait for a second
//
//  digitalWrite(D1, LOW);
//  digitalWrite(D2, LOW);
//  digitalWrite(D3, LOW);
//  digitalWrite(D4, HIGH); 
//  //3
//  digitalWrite(pinA, LOW);   
//  digitalWrite(pinB, LOW);   
//  digitalWrite(pinC, LOW);   
//  digitalWrite(pinD, LOW);   
//  digitalWrite(pinE, HIGH);   
//  digitalWrite(pinF, HIGH);   
//  digitalWrite(pinG, LOW);     
//  delay(1);               // wait for a second
/*  
  //4
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);     
  delay(1000);               // wait for a second
  
  //5
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);     
  delay(1000);               // wait for a second
  
  //6
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);     
  delay(1000);               // wait for a second
  
  //7
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);     
  delay(1000);               // wait for a second
  
  //8
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);     
  delay(1000);               // wait for a second

  //9
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);     
  delay(1000);               // wait for a second
  */
}
