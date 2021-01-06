//#include <Keyboard.h>
#include <Joystick.h>


const int R1_1 = 13; // Interrupt
const int R1_2 = 14;

const int R2_1 = 15; // Interrupt
const int R2_2 = 16;

const int R3_1 = 17; // Interrupt
const int R3_2 = 18;

const int R4_1 = 19; // Interrupt
const int R4_2 = 20;

const int BTNSIZE = 10;
const int BTN[10] = {0,1,2,3,4,5,6,7,8,9};
const int BTNOUT[10] = {21,22,23,24,25,26};
int btnlast[10] = {0,0,0,0,0,0,0,0,0,0};

const int RSIZE = 4;
volatile int rval[4] = {0,0,0,0};
//const char charinc[4] = {'A','B','C','D'};
//const char chardec[4] = {'a','b','c','d'};
const int BTNINC[4] = {11,12,13,14};
const int BTNDEC[4] = {1,2,3,4};

void setup() {
  int i;
  
  pinMode(R1_1, INPUT);
  pinMode(R1_2, INPUT);
  pinMode(R2_1, INPUT);
  pinMode(R2_2, INPUT);
  pinMode(R3_1, INPUT);
  pinMode(R3_2, INPUT);
  pinMode(R4_1, INPUT);
  pinMode(R4_2, INPUT);

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
        Joystick.button(BTNOUT[i],b);
        btnlast[i] = b;
     }
  }
  
}

void sendEvent(int i) {
    while (rval[i] != 0) {
      if (rval[i] < 0) {
        Joystick.button(i,BTNINC[i]);
        //Keyboard.write(charinc[i]);        
        rval[i]++;
      } else {
        Joystick.button(i,BTNDEC[i]);
        //Keyboard.write(chardec[i]);        
        rval[i]--;
      }
      debug(rval[i]);
    }      
}

void debug(int s) {
  Serial.println(s);
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
