uint8_t taster = 2;
uint8_t out = 3;
uint8_t statLed = 7;
uint8_t led1 = 8;
uint8_t led2 = 9;
uint8_t led3 = 11;
uint8_t led4 = 12;

uint8_t pwm = 255;

uint32_t timer = 0;

enum State {
  OFF,
  ON,
  T1,
  T2
};

enum State state = ON;

unsigned long t1 = 0;

void setup() {
  pinMode(taster, INPUT_PULLUP);
  pinMode(out, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(taster), releasedISR, RISING);
}

void releasedISR() {
  if ((timer < 512) && (timer > 64)) {
    switch(state) {
      case OFF: state = ON; break;
      case ON: state = T1; break;
      case T1: state = T2; break;
      case T2: state = OFF; break;
    }
  }
  timer = 0;
}

void loop() {
  analogWrite(out, pwm);

  if (digitalRead(taster) == 0) {
    delay(1);
    timer++;
  }

  while (digitalRead(taster) == 0 && timer >= 512) {
    delay(10);
    pwm--;
    analogWrite(out, pwm);
  }

  switch(state) {
    case OFF:
      pwm = 0;
      t1 = 0;
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      break;
    case ON:
      if (pwm == 0) {
        pwm = 255;
      }
      t1 = 0;
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      break;
    case T1:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 1800000) {
        state = OFF;
      }
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      break;
    case T2:
      if (t1 == 0) {
        t1 = millis();
      }
      if (millis()-t1 > 3600000) {
        state = OFF;
      }
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      break;
  }
  
}
