//#include <Keyboard.h>
#include <Joystick.h>


#define INTERNAL_LED   13   // on
#define LED_RED        24   // on, when rotary is rotated 
#define LED_BLUE       25   // on, when button is pressed
#define LED_GREEN      26   // on, when red and blue is off

#define LED_WAIT       1000 // time in millis, so long is red andf blue on

// pins for the rotary encoders
#define R1_1  14 // Interrupt
#define R1_2  15

#define R2_1  16 // Interrupt
#define R2_2  17

#define R3_1  18 // Interrupt
#define R3_2  19

#define R4_1  20 // Interrupt
#define R4_2  21

#define BTNSIZE  10
const int BTN[10] = {0,1,2,3,4,5,6,7,8,9};  // pins for the buttons
const int BTNOUT[10] = {21,22,23,24,25,26}; // id for the joystick buttons
int btnlast[10] = {0,0,0,0,0,0,0,0,0,0}; // last status of the buttons

#define RSIZE  4
volatile int rval[4] = {0,0,0,0}; // open envents from rotary encoders 
//const char charinc[4] = {'A','B','C','D'};
//const char chardec[4] = {'a','b','c','d'};
const int BTNINC[4] = {11,12,13,14}; // joystick buttons for increment
const int BTNDEC[4] = {1,2,3,4}; // joystick buttons for decrement

unsigned int mred = 0; // current millis when red led isturned on
unsigned int mblue= 0; // current millis when blue led isturned on

void setup() {
  int i;

  Serial.begin(115200);
  Serial.println("Starte....");
  
  pinMode(R1_1, INPUT);
  pinMode(R1_2, INPUT);
  pinMode(R2_1, INPUT);
  pinMode(R2_2, INPUT);
  pinMode(R3_1, INPUT);
  pinMode(R3_2, INPUT);
  pinMode(R4_1, INPUT);
  pinMode(R4_2, INPUT);

  pinMode(INTERNAL_LED, OUTPUT);
  digitalWrite(INTERNAL_LED,HIGH);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT); 
  pinMode(LED_GREEN, OUTPUT);
  green();

  for (i=0;i<BTNSIZE;i++) {
    pinMode(BTN[i],INPUT); 
  }

  attachInterrupt(digitalPinToInterrupt(R1_1), do1, FALLING);
  attachInterrupt(digitalPinToInterrupt(R2_1), do2, FALLING);
  attachInterrupt(digitalPinToInterrupt(R3_1), do3, FALLING);
  attachInterrupt(digitalPinToInterrupt(R4_1), do4, FALLING);

  //Keyboard.begin();
  Joystick.begin();
}

void loop() {
  int i;
  int b;
  
  for (i=0;i<RSIZE;i++) {
    sendEvent(i);
  }

  for (i=0;i<BTNSIZE;i++) {
     b = digitalRead(BTN[i]);
     if (b != btnlast[i]) {
        Serial.print("BTN");
        Serial.print(i);
        Serial.print(":");
        Serial.println(b);

        Joystick.button(BTNOUT[i],b);
        blue();
        btnlast[i] = b;
     }
  }

  ledstatus();

}

void ledstatus() {
  if (mred > 0) {
    if (millis()>mred+LED_WAIT) {      
      green();
      mred=0;
    }    
  }

  if (mblue > 0) {
    if (millis()>mblue+LED_WAIT) {      
      green();
      mblue=0;
    }    
  }
  
}

void sendEvent(int i) {
    while (rval[i] != 0) {
      Serial.print("ROE");
      Serial.print(i);
      if (rval[i] < 0) {
        Joystick.button(i,BTNINC[i]);
        //Keyboard.write(charinc[i]);        
        rval[i]++;
        Serial.println("++");
      } else {
        Joystick.button(i,BTNDEC[i]);
        //Keyboard.write(chardec[i]);        
        rval[i]--;
        Serial.println("--");
      }      
      red();
    }      
}

void do1() {
  if (digitalRead(R1_2) == HIGH) {
    rval[0] ++;    
  } else {
    rval[0] --;    
  }  
}

void do2() {
  if (digitalRead(R2_2) == HIGH) {
    rval[1] ++;    
  } else {
    rval[1] --;    
  }  
}

void do3() {
  if (digitalRead(R3_2) == HIGH) {
    rval[2] ++;
  } else {
    rval[2] --;    
  }  
}

void do4() {
  if (digitalRead(R4_2) == HIGH) {
    rval[3] ++;    
  } else {
    rval[3] --;    
  }  
}


void red() {
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_GREEN,LOW);
  mred = millis();
}

void green() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_BLUE,LOW);  
}

void blue() {
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_BLUE,HIGH);
  mblue = millis();
}
