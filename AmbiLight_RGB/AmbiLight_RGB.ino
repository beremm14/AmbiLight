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
uint32_t timer = 0;

//TODO Make Preprogrammed warm white button
//TODO Presets
//TODO Effekte (Farbverlauf

enum State 
{
  OFF,
  ON,
  T1,
  T2,
  T3,
  T4
};

enum State state = ON;

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

  attachInterrupt(digitalPinToInterrupt(taster_timer), releasedISR, RISING);
  attachInterrupt(digitalPinToInterrupt(taster_mode), releasedISR, RISING);
  analogReference(EXTERNAL);
}
//Switch Timer state
void releasedISR() {
  if ((timer < 500) && (timer > 50)) {
    switch(state) {
      case OFF: state = ON; break;
      case ON: state = T1; break;
      case T1: state = T2; break;
      case T2: state = T3; break;
      case T3: state = T4; break;
      case T4: state = OFF; break;
    }
  }
  timer = 0;
}

void writeRGB()
{
  //Read Poti values and set analog out
    int val_R = analogRead(Poti_R);
    int val_G = analogRead(Poti_G);
    int val_B = analogRead(Poti_B);
    pwm_R = map(val_R, 0, 1023, 0 , 255);
    pwm_G = map(val_G, 0, 1023, 0 , 255);
    pwm_B = map(val_B, 0, 1023, 0 , 255);
  
    
    analogWrite(R, pwm_R);
    analogWrite(G, pwm_G);
    analogWrite(B, pwm_B);
}

void loop() {

    if (digitalRead(taster_timer) == 0) {
    delay(1);
    timer++;
  }


//Sleep Timer
  switch(state) {
    case OFF:
      t1 = 0;
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      break;
    case ON:
      writeRGB();   
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
        state = OFF;
      }
      writeRGB();   
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW); 
      break;
    case T2:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 20000) {
        state = OFF;
      }
      writeRGB();   
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW); 
      break;
    case T3:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 30000) {
        state = OFF;
      }
      writeRGB();   
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, LOW); 
      break;
    case T4:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 40000) {
        state = OFF;
      }
      writeRGB();   
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH); 
      break;
  }

}
