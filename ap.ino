#include <Joystick.h>

#define DEBUG 1

#define INTERNAL_LED   13   // on
#define LED_RED        33   // on, when rotary is rotated 
#define LED_GREEN      34   // on, when red and blue is off
#define LED_BLUE       35   // on, when button is pressed


#define LED_WAIT       300 // time in millis, so long is red andf blue on
#define INPUT_WAIT     20
#define LOOP_DELAY     50

// pins for the rotary encoders
#define R1_1  24 // BARO
#define R1_2  25 
#define R2_1  26 // CRS
#define R2_2  27  
#define R3_1  28 // HDG 
#define R3_2  29
#define R4_1  30 // SPD
#define R4_2  31
#define R5_1  14 // ALT
#define R5_2  15
#define R6_1  16 // VS
#define R6_2  17



volatile unsigned int lr11 = 0;
volatile unsigned int lr21 = 0;
volatile unsigned int lr31 = 0;
volatile unsigned int lr41 = 0;
volatile unsigned int lr51 = 0;
volatile unsigned int lr61 = 0;


#define BTNSIZE  12
// pins for the buttons
const int BTN[12] = {
  0,   // BARO RE
  1,   // CRS  RE
  2,   // HDG  RE
  3,   // ALT  RE
  4,   // VS
  5,   // VNAV
  6,   // FLC
  7,   // AP
  8,   // NAV
  9,   // HDG
 10,   // APPR
 11,   // ALT
  };  
const int BTNOUT[12] = {21,22,23,24,25,26,27,28}; // id for the joystick buttons
int btnlast[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; // last status of the buttons

#define RSIZE  6
volatile int rval[6] = {0,0,0,0,0,0}; // open envents from rotary encoders 
volatile int rvalr[6] = {0,0,0,0,0,0}; 
const int BTNINC[6] = {11,12,13,14,15,16}; // joystick buttons for increment
const int BTNDEC[6] = {1,2,3,4,5,6}; // joystick buttons for decrement

unsigned int mled = 0; // current millis when led is turned on

long v1 =0 ;

void setup() {
  int i;

  if (DEBUG>0) {
    Serial.begin(115200);
    Serial.println("Starte....");
  }
  
  pinMode(R1_1, INPUT);
  pinMode(R1_2, INPUT);
  pinMode(R2_1, INPUT);
  pinMode(R2_2, INPUT);
  pinMode(R3_1, INPUT);
  pinMode(R3_2, INPUT);
  pinMode(R4_1, INPUT);
  pinMode(R4_2, INPUT);  
  pinMode(R5_1, INPUT);
  pinMode(R5_2, INPUT);
  pinMode(R5_1, INPUT);
  pinMode(R5_2, INPUT);



  pinMode(INTERNAL_LED, OUTPUT);
  digitalWrite(INTERNAL_LED,HIGH);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT); 
  pinMode(LED_GREEN, OUTPUT);
  
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_BLUE,HIGH);  


  for (i=0;i<BTNSIZE;i++) {
    pinMode(BTN[i],INPUT); 
    Joystick.button(BTNOUT[i],0);
  }

  attachInterrupt(digitalPinToInterrupt(R1_1), do11, FALLING);
  attachInterrupt(digitalPinToInterrupt(R1_2), do12, FALLING);
  attachInterrupt(digitalPinToInterrupt(R2_1), do21, FALLING);
  attachInterrupt(digitalPinToInterrupt(R2_2), do22, FALLING);
  attachInterrupt(digitalPinToInterrupt(R3_1), do31, FALLING);
  attachInterrupt(digitalPinToInterrupt(R3_2), do32, FALLING);
  attachInterrupt(digitalPinToInterrupt(R4_1), do41, FALLING);
  attachInterrupt(digitalPinToInterrupt(R4_2), do42, FALLING);
  attachInterrupt(digitalPinToInterrupt(R5_1), do51, FALLING);
  attachInterrupt(digitalPinToInterrupt(R5_2), do52, FALLING);
  attachInterrupt(digitalPinToInterrupt(R6_1), do61, FALLING);
  attachInterrupt(digitalPinToInterrupt(R6_2), do62, FALLING);

  Joystick.begin();
}

void loop() {

  for (int i=0;i<RSIZE;i++) {
    sendEvent(i);
  }

  for (int i=0;i<BTNSIZE;i++) {
     int b = digitalRead(BTN[i]);
     if (b != btnlast[i]) {
        if (DEBUG>0) {
          Serial.print("BTN");
          Serial.print(i);
          Serial.print(":");
          Serial.println(b);
        }

        Joystick.button(BTNOUT[i],b==1?0:1);
        purple();
        btnlast[i] = b;
     }
  }

  ledstatus();

  delay(LOOP_DELAY);
  
  for (int i=0;i<RSIZE;i++) {
    resetEvent(i);
  }
}


void ledstatus() {
  
  if (mled > 0) {
    if (millis()>mled+LED_WAIT) {      
      green();
      mled=0;
    }    
  }
  
}

void sendEvent(int i) {
    if (rval[i] != 0) {   
      int x = rval[i];   
      rval[i] = 0;
      rvalr[i] = 1;
      if (DEBUG>0) {
        Serial.print("ROE");
        Serial.print(i);      
      }
      if (x < 0) {        
        Joystick.button(BTNINC[i],1);
        if (DEBUG>0) Serial.println("++");
        blue();
      } else if (x > 0) {
        Joystick.button(BTNDEC[i],1);
        if (DEBUG>0) Serial.println("--");
        red();
      }      
    }      
}

void  resetEvent(int i) {
  if (rvalr[i] != 0)  {    
    rvalr[i] = 0;
    Joystick.button(BTNINC[i],0);
    Joystick.button(BTNDEC[i],0);
    if (DEBUG>0) {
      Serial.print("RESET");
      Serial.println(i);      
    }

  }
}

// 1
void do11() {
  int v = digitalRead(R1_2);
  if (DEBUG>1) Serial.println("++1");
  if (millis() < lr11)  return;
  lr11 = millis();

  if (v == HIGH) {    
      rval[0]++;    
  } 
}


void do12() {
  int v = digitalRead(R1_1);
  if (DEBUG>1) Serial.println("--1");
  if (millis() < lr11 + INPUT_WAIT)  return;
  lr11 = millis();

  if (v == HIGH) {    
      rval[0]--;    
  } 
}
// 2
void do21() {  
  int v = digitalRead(R2_2);
  if (DEBUG>1) Serial.println("++2");
  if (millis() < lr21 + INPUT_WAIT)  return;  
  lr21 = millis();

  if (v == HIGH) {    
      rval[1]++;    
  } 
}
void do22() {
  int v = digitalRead(R2_1);
  if (DEBUG>1) Serial.println("--2");
  if (millis() < lr21 + INPUT_WAIT)  return;
  lr11 = millis();

  if (v == HIGH) {    
      rval[1]--;    
  } 
}

// 3
void do31() {
  int v = digitalRead(R3_2);
  if (DEBUG>1) Serial.println("++3");
  if (millis() < lr31 + INPUT_WAIT)  return;
  lr31 = millis();

  if (v == HIGH) {    
      rval[2]++;    
  } 
}


void do32() {
  int v = digitalRead(R3_1);
  if (DEBUG>1) Serial.println("--3");
  if (millis() < lr31 + INPUT_WAIT)  return;
  lr31 = millis();

  if (v == HIGH) {    
      rval[2]--;    
  } 
}

// 4
void do41() {
  int v = digitalRead(R4_2);
  if (millis() < lr41 + INPUT_WAIT)  return;
  lr41 = millis();

  if (v == HIGH) {    
      rval[3]++;    
  } 
}

void do42() {
  int v = digitalRead(R4_1);
  if (millis() < lr41 + INPUT_WAIT)  return;
  lr41 = millis();

  if (v == HIGH) {    
      rval[3]--;    
  } 
}

// 5
void do51() {
  int v = digitalRead(R5_2);
  if (millis() < lr51 + INPUT_WAIT)  return;
  lr51 = millis();

  if (v == HIGH) {    
      rval[4]++;    
  } 
}


void do52() {
  int v = digitalRead(R5_1);
  if (millis() < lr51 + INPUT_WAIT)  return;
  lr51 = millis();

  if (v == HIGH) {    
      rval[4]--;    
  } 
}

// 6
void do61() {
  int v = digitalRead(R6_2);
  if (millis() < lr61 + INPUT_WAIT)  return;
  lr61 = millis();

  if (v == HIGH) {    
      rval[5]++;    
  } 
}


void do62() {
  int v = digitalRead(R6_1);
  if (millis() < lr61 + INPUT_WAIT)  return;
  lr61 = millis();

  if (v == HIGH) {    
      rval[0]--;    
  } 
}



void red() {
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_GREEN,LOW);
  mled = millis();
}

void green() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_BLUE,LOW);  
}

void blue() {
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,HIGH);
  mled = millis();
}

void purple() {
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_RED,HIGH);
  mled = millis();
}
