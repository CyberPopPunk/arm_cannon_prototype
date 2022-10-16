int NUM_PINS = 3;
int LED_PINS[3] = { 2, 3, 4 };
int motorPin = 5;

int buttonPin = 12;
int forceResistor;
bool loaded = 0;
int charge = 0;
int threshold = 800;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
  pinMode(motorPin, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (loaded) {
    bool trigger = !digitalRead(buttonPin);
    Serial.print("waiting for trigger");
    if (trigger) {
      firingAnimation(charge);
      loaded = false;
      charge = 0;
    }
  }
  if (analogRead(forceResistor) < threshold) {
    charge = chargingFunction();
    loaded = 1;
  }
  Serial.print("DEBUG INFO:");
  Serial.print("      Analog Sensor: ");
  Serial.print(analogRead(A0));
  Serial.print("   ");
  Serial.print("Charge: ");
  Serial.print(charge);
  Serial.print("   Loaded: ");
  Serial.println(loaded);
  Serial.print("   ");
}

void firingAnimation(int chargeLevel) {
  Serial.print("fire!");
  
  digitalWrite(motorPin, HIGH);
  for (int i = chargeLevel; i >= 0; i--) {
    digitalWrite(LED_PINS[i], LOW);
    delay(50);
  }
  delay(chargeLevel * 300);
  digitalWrite(motorPin, LOW);
}

int chargingFunction() {
  int startTime = millis();
  int pressure = analogRead(forceResistor);
  Serial.print("starting to charge!");
  int currCharge = 0;
  while (pressure < threshold) {
    if ((millis() - startTime) > 3000) {
      digitalWrite(LED_PINS[0], HIGH);
      currCharge = 3;
      Serial.print("Charge level 3");
    } else if ((millis() - startTime) > 2000) {
      digitalWrite(LED_PINS[1], HIGH);
      currCharge  = 2;
      Serial.print("Charge level 2");
    } else if ((millis() - startTime) > 1000) {
      digitalWrite(LED_PINS[2], HIGH);
      currCharge  = 1;
      Serial.print("Charge level 1");
    }
    Serial.println(analogRead(forceResistor));
    pressure = analogRead(forceResistor);
    delay(50);
  }
  delay(100);
  return currCharge;
}