#include <Stepper.h>
#include "IRremote.h"
#include <stdlib.h>

int LED_PIN = 2;
int REMOTE_PIN = 3;
 
Stepper steppermotor(32, 8, 10, 9, 11);
IRrecv irrecv(REMOTE_PIN);
decode_results results;

int q_open;
int q_close;
int cur_state;

bool movingUp;
bool movingDown;

int goToState(double q) {
  int steps = q - cur_state;
  steppermotor.step(steps);
  cur_state = q;
  return steps; 
}

void recordCloseState(){
   q_close = cur_state;
}

void recordOpenState(){
  q_open = cur_state;  
}

void moveUp(){
  steppermotor.step(1);
  cur_state += 1;
}

void moveDown(){
  steppermotor.step(-1);
  cur_state -= 1;
}

void triggerMovingUp(){
  movingDown = false;
  movingUp = !movingUp;
}

void triggerMovingDown(){
  movingUp = false;
  movingDown = !movingDown;
}

void translateIR(){
  
  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK"); triggerMovingDown(); break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");triggerMovingUp(); break;
  case 0xFFE01F: Serial.println("DOWN"); recordCloseState(); break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP"); recordOpenState(); break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897: Serial.println("0"); goToState(q_close); break;
  case 0xFF30CF: Serial.println("1"); goToState(q_open); break;
  case 0xFF18E7: Serial.println("2");    break;
  case 0xFF7A85: Serial.println("3");    break;
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");    break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: break;  

  default: 
    Serial.println("weak signal");

  }
}

void setup()
{
movingUp = false;
movingDown = false;
cur_state= 0;
q_close = 0;
q_open = 0;

Serial.begin(9600);
irrecv.enableIRIn();
steppermotor.setSpeed(1000);

Serial.println("Setup Completed.");
}

void loop()
{
    if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }
  if (movingUp) moveUp();
  if (movingDown) moveDown();
  
//  if(Serial.available()){
//  double input = atof(Serial.readStringUntil('\n').c_str());
//  q_close = (int(input * 2048));
//   Serial.println(goToState(q_close));
//  }
}
