#define BIN1 9
#define BIN2 10
#define ENABLE 2

#define MAX 255

#define DELAY 50

void setup() {
  digitalWrite(ENABLE,LOW);
  // put your setup code here, to run once:
  analogWrite(BIN1,LOW);
  delay(10);
  analogWrite(BIN2,LOW);
  Serial.begin(9600);
  Serial.println("Waiting...");
  delay(1000);
  Serial.println("START!");
}

void loop() {
  digitalWrite(ENABLE,LOW);
  // put your main code here, to run repeatedly:
  for(int i=0;i<MAX;i++) {
    analogWrite(BIN1,i);  
    delay(10);
    analogWrite(BIN2,i);
    Serial.println(i);
    delay(DELAY);
  }

  digitalWrite(ENABLE,HIGH);
  for(int i=MAX;i>=0;i--) {
    analogWrite(BIN1,i);
    delay(10);
    analogWrite(BIN2,i);
    Serial.println(i);
    delay(DELAY);
  }

  analogWrite(BIN1,LOW);
  delay(10);
  analogWrite(BIN2,LOW);
  delay(500);
}
