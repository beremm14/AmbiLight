uint8_t taster_timer = 3;
uint8_t taster_mode = 2;
uint8_t R = 10;
uint8_t G = 9;
uint8_t B = 6;
uint8_t Poti_R = A0;
uint8_t Poti_G = A1;
uint8_t Poti_B = A2;
uint8_t led1 = 4;
uint8_t led2 = 5;
uint8_t led3 = 7;
uint8_t led4 = 8;

uint8_t pwm_R = 0;
uint8_t pwm_G = 0;
uint8_t pwm_B = 0;
uint32_t timer1 = 0;
uint32_t timer2 = 0;
uint32_t timer3 = 0;



//TODO Effekte (Farbverlauf, blinken)

enum Timer_State 
{
  OFF,
  ON,
  T1,
  T2,
  T3,
  T4
};

enum Mode_State
{
  Manual,
  Warm_white,
  Cool_red,
  Cool_green,
  Cool_blue,
  Cool_turqoise
};

enum Timer_State timer_state = ON;
enum Mode_State mode_state = Manual;

unsigned long t1 = 0;

void setup() {
  pinMode(taster_timer, INPUT_PULLUP);
  pinMode(taster_mode, INPUT_PULLUP);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(led1 , OUTPUT);
  pinMode(led2 , OUTPUT);
  pinMode(led3 , OUTPUT);
  pinMode(led4 , OUTPUT);
  pinMode(Poti_R, INPUT);
  pinMode(Poti_G, INPUT);
  pinMode(Poti_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(taster_timer), timerButtonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(taster_mode), modeButtonPress, RISING);
  analogReference(EXTERNAL);
}
//Switch Timer state
void timerButtonPress() {
  if (timer1 > 100) {
    switch(timer_state) {
      case OFF: timer_state = ON; break;
      case ON: timer_state = T1; break;
      case T1: timer_state = T2; break;
      case T2: timer_state = T3; break;
      case T3: timer_state = T4; break;
      case T4: timer_state = OFF; break;
    }
  }
  timer1 = 0;
}

//Switch Mode
void modeButtonPress()
{
   if (timer2 > 100) {
    switch(mode_state) {
      case Manual: mode_state = Warm_white; break;
      case Warm_white: mode_state = Cool_red; break;
      case Cool_red: mode_state = Cool_green; break;
      case Cool_green: mode_state = Cool_blue; break;
      case Cool_blue: mode_state = Cool_turqoise; break;
      case Cool_turqoise: mode_state = Manual; break;
    }
  }
  timer2 = 0;
  
}


void checkReset()
{
  if (!digitalRead(taster_timer) && !digitalRead(taster_mode))
  {
    if(timer3 > 100)
    {
      mode_state = Manual;
      timer_state = ON;
    }
  }
  timer3 = 0;
}

void writeRGB(int val_R, int val_G, int val_B)
{
  //Set PWM Signal
    pwm_R = map(val_R, 0, 1023, 0 , 255);
    pwm_G = map(val_G, 0, 1023, 0 , 255);
    pwm_B = map(val_B, 0, 1023, 0 , 255);
  
    
    analogWrite(R, pwm_R);
    analogWrite(G, pwm_G);
    analogWrite(B, pwm_B);
}

void setTimer ()
{
  if (digitalRead(taster_timer))
  {
    delay(1);
    timer1 ++;
  }

  if (digitalRead(taster_mode))
  {
    delay(1);
    timer2 ++;
  }

  if (digitalRead(taster_mode) && digitalRead(taster_timer))
  {
    delay(1);
    timer3++;
  }
}

void loop() {

setTimer();
checkReset();


//Sleep Timer
  switch(timer_state) {
  case OFF:
            writeRGB(0,0,0);   
      t1 = 0;
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      break;
  case ON:
      switch(mode_state)
      {
        case Manual:
          writeRGB(analogRead(Poti_R), analogRead(Poti_G), analogRead(Poti_B)); break;
        case Warm_white:
          writeRGB(623, 523, 50);  break;
        case Cool_red:
          writeRGB(255, 0, 0);  break;
        case Cool_green: 
          writeRGB(0,255,0); break;
        case Cool_blue:
          writeRGB(0,0,255); break;
        case Cool_turqoise:
          writeRGB(0,255, 210); break;
      }
          
      t1 = 0;
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH);    
      break;
  case T1:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 10000) {
        timer_state = OFF;
        t1=0;
      }
      switch(mode_state)
      {
        case Manual:
          writeRGB(analogRead(Poti_R), analogRead(Poti_G), analogRead(Poti_B)); break;
        case Warm_white:
          writeRGB(623, 523, 50);  break;
        case Cool_red:
          writeRGB(255, 0, 0);  break;
        case Cool_green: 
          writeRGB(0,255,0); break;
        case Cool_blue:
          writeRGB(0,0,255); break;
        case Cool_turqoise:
          writeRGB(0,255, 210); break;
      }
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW); 
      break;
  case T2:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 10000) {
        timer_state = T1;
        t1=0;
      }
      switch(mode_state)
      {
        case Manual:
          writeRGB(analogRead(Poti_R), analogRead(Poti_G), analogRead(Poti_B)); break;
        case Warm_white:
          writeRGB(623, 523, 50);  break;
        case Cool_red:
          writeRGB(255, 0, 0);  break;
        case Cool_green: 
          writeRGB(0,255,0); break;
        case Cool_blue:
          writeRGB(0,0,255); break;
        case Cool_turqoise:
          writeRGB(0,255, 210); break;
      }
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW); 
      break;
  case T3:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 10000) {
        timer_state = T2;
        t1=0;
      }
      switch(mode_state)
      {
        case Manual:
          writeRGB(analogRead(Poti_R), analogRead(Poti_G), analogRead(Poti_B)); break;
        case Warm_white:
          writeRGB(623, 523, 50);  break;
        case Cool_red:
          writeRGB(255, 0, 0);  break;
        case Cool_green: 
          writeRGB(0,255,0); break;
        case Cool_blue:
          writeRGB(0,0,255); break;
        case Cool_turqoise:
          writeRGB(0,255, 210); break;
      }  
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, LOW); 
      break;
  case T4:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 10000) {
        timer_state = T3;
        t1=0;
      }
      switch(mode_state)
      {
        case Manual:
          writeRGB(analogRead(Poti_R), analogRead(Poti_G), analogRead(Poti_B)); break;
        case Warm_white:
          writeRGB(623, 523, 50);  break;
        case Cool_red:
          writeRGB(255, 0, 0);  break;
        case Cool_green: 
          writeRGB(0,255,0); break;
        case Cool_blue:
          writeRGB(0,0,255); break;
        case Cool_turqoise:
          writeRGB(0,255, 210); break;
      }  
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH); 
      break;
  }

}
