int button_count = 8;
unsigned long timeout = 25000;


int lastpin = 12; //which pin is the last to light up
bool testrunning = false;
unsigned long starttime;
int answered = 0;
bool debug = false;


class answer {
  public:
    bool state;
    int pin;
    answer(int _pin) {
      pin = _pin;
      state = false;
    }
};

class button {
  public:
    int pin;
    answer *target;
    button(int _pin, answer *_target) {
      pinMode(_pin,INPUT_PULLUP);
      pin = _pin;
      target = _target;
    }
    
    bool activate() {
      if(!target->state) {
        testrunning = true;
        digitalWrite(target->pin,HIGH);
        target->state = true;
        answered++;
        starttime = millis();
        return true;
      } else {
        return false;
      }
    }
};

answer answer1 = answer(9);
answer answer2 = answer(10);
answer answer3 = answer(11);
button buttons[] = {
      button(1,&answer1),
      button(2,&answer1),
      button(3,&answer1),
      button(4,&answer2),
      button(5,&answer2),
      button(6,&answer3),
      button(7,&answer3),
      button(8,&answer3)
    };

void reset();

void setup() {
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  if(debug) {
  Serial.begin(9600);
  Serial.println("Configured");
  }
  for(int i = 9;i<=12;i++) {
    digitalWrite(i,HIGH);
  }
  delay(1000);
  for(int i = 9;i<=12;i++) {
    digitalWrite(i,LOW);
  }


//  digitalWrite(12,HIGH);
}

void loop() {

  for(int i =0;i<button_count;i++) {
    if(buttons[i].target->state == false && digitalRead(buttons[i].pin) == LOW) {
      buttons[i].activate();
      if(answered >= 3) {
        digitalWrite(lastpin,HIGH);
        delay(20000);
        reset();
      }
      delay(50);
      while(digitalRead(buttons[i].pin) == LOW) {delay(10);}
      if(debug) {      
        Serial.print("Button ");
        Serial.print(i);
        Serial.println(" pressed!");
      }
    }
  }
  if(testrunning & millis() - starttime > timeout) {
    reset();
  }
}


void reset() {
  digitalWrite(answer1.pin,LOW);
  digitalWrite(answer2.pin,LOW);
  digitalWrite(answer3.pin,LOW);
  digitalWrite(lastpin,LOW);
  testrunning = false;
  answered = 0;
  answer1.state = false;
  answer2.state = false;
  answer3.state = false;
}
