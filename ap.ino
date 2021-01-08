#include <Joystick.h>

#define INTERNAL_LED   13   // on
#define LED_RED        33   // on, when rotary is rotated 
#define LED_GREEN      34   // on, when red and blue is off
#define LED_BLUE       35   // on, when button is pressed


#define LED_WAIT       300 // time in millis, so long is red andf blue on

// pins for the rotary encoders
#define R1_1  24 // Interrupt
volatile unsigned int lr11 = 0;
#define R1_2  25
#define R2_1  16 // Interrupt
#define R2_2  17

#define R3_1  18 // Interrupt
#define R3_2  19

#define R4_1  20 // Interrupt
#define R4_2  21

#define BTNSIZE  10
const int BTN[10] = {0,1,2,3,4,5,6,7,8,9};  // pins for the buttons
const int BTNOUT[10] = {21,22,23,24,25,26}; // id for the joystick buttons
int btnlast[10] = {1,1,1,1,1,1,1,1,1,1}; // last status of the buttons

#define RSIZE  4
volatile int rval[4] = {0,0,0,0}; // open envents from rotary encoders 
const int BTNINC[4] = {11,12,13,14}; // joystick buttons for increment
const int BTNDEC[4] = {1,2,3,4}; // joystick buttons for decrement

unsigned int mred = 0; // current millis when red led isturned on
unsigned int mblue= 0; // current millis when blue led isturned on
long v1 =0 ;

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
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_BLUE,HIGH);  


  for (i=0;i<BTNSIZE;i++) {
    pinMode(BTN[i],INPUT); 
    Joystick.button(BTNOUT[i],0);
  }

  attachInterrupt(digitalPinToInterrupt(R1_1), do11, FALLING);
  attachInterrupt(digitalPinToInterrupt(R1_2), do12, FALLING);
  //attachInterrupt(digitalPinToInterrupt(R1_2), do12, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(R2_1), do2, FALLING);
  //attachInterrupt(digitalPinToInterrupt(R3_1), do3, FALLING);
  //attachInterrupt(digitalPinToInterrupt(R4_1), do4, FALLING);

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

        Joystick.button(BTNOUT[i],b==1?0:1);
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
    if (rval[i] != 0) {   
      int x = rval[i];   
      rval[i]=0;
      Serial.print("ROE");
      Serial.print(i);      
      if (x < 0) {        
        Joystick.button(BTNINC[i],1);
        delay(90);
        Joystick.button(BTNINC[i],0);
        Serial.println("++");
        blue();
      } else if (x > 0) {
        Joystick.button(BTNDEC[i],1);
        delay(90);
        Joystick.button(BTNDEC[i],0);
        Serial.println("--");
        red();
      }      
      
      
    }      
}

void do11() {
  if (millis() < lr11 + 100)  return;
  lr11 = millis();

  if (digitalRead(R1_2) == HIGH) {    
      rval[0]++;    
  } 
}


void do12() {
  if (millis() < lr11 + 100)  return;
  lr11 = millis();

  if (digitalRead(R1_1) == HIGH) {    
      rval[0]--;    
  } 
}

void do2() {
  
}

void do3() {

}

void do4() {

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
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,HIGH);
  mblue = millis();
}
