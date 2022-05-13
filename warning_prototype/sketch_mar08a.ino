int interval = 50;
bool flag = false;
bool pressed = false;
const byte inPin = 8;

float testVals[] = {5,4,3,2,1,0.5,0.25};
int currIndex = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(inPin, INPUT_PULLUP);
}

void loop () {
  
  if (currIndex == 7) {
    currIndex = 0;
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);
  digitalWrite(LED_BUILTIN, LOW);
  delay(int(testVals[currIndex]*1000));
  
  Serial.print("Current Delay of: ");
  Serial.println(testVals[currIndex]);

  pressed = false;

  if (digitalRead(inPin) == LOW) {
    flagSwitch();
  }
}

void flagSwitch() {

  if (!pressed) {
    flag = !flag;
    pressed = true;
    currIndex++;
  }
}
