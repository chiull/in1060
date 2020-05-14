//Pin som blir brukt
int gronn = 2;
int gul = 3;
int rod = 4;
int bla = 5;
int gronnKnapp = A4;
int gulKnapp = A3;
int rodKnapp = A2;
int blaKnapp = A1;
int startButton = 6;

//variabler som skal holde de random tallene
unsigned long randomNumber;
unsigned long r2;
long enEllerTo;

//variabler som blir brukt
unsigned long tid;
unsigned long forrige = 0;
unsigned long timer = 0;
unsigned long forrigeTimer = 0;
unsigned long fireMin = 0;
unsigned long forrigeFire = 0;
unsigned long enMin = 0;
unsigned long forrigeEn = 0;
int start;
int clicked = 0;
int intervall = 1000;
int debounce = 0;
int forrigeDebounce = 0;

void setup() {
  pinMode(gronn, OUTPUT);
  pinMode(gul, OUTPUT);
  pinMode(rod, OUTPUT);
  pinMode(bla, OUTPUT);
  pinMode(gronnKnapp, INPUT);
  pinMode(gulKnapp, INPUT);
  pinMode(rodKnapp, INPUT);
  pinMode(blaKnapp, INPUT);
  pinMode(startButton, INPUT);
  digitalWrite(gronnKnapp, HIGH);
  digitalWrite(gulKnapp, HIGH);
  digitalWrite(rodKnapp, HIGH);
  digitalWrite(blaKnapp, HIGH);
  digitalWrite(startButton, LOW);
  start = 0;

  //RandomSeed er for aa kunne gjoore at det blir random tall hele tiden ved aa bruke en analog port som ikke blir brukt.
  randomSeed(analogRead(A0));

}



void loop() {
  debounce = millis();//Debounce for gjoor at knappen ikke start ikke stopper med engang hvis man holder det litt for lenge
  if (debounce - forrigeDebounce > 500) {
    if (digitalRead(startButton) == HIGH) { //trykker paa start knappen gjoor at den kan kjoore
      forrigeDebounce = debounce;
      if (start == 0) { //gjoor at den kjoorer
        start = 255;
        forrigeTimer = timer;
        forrigeFire = fireMin;
        forrigeEn = enMin;
      } else { //trykker en gang til for aa gjoore at den ikke kjoorer
        ferdig();
      }
    }
  }

  if (start > 0) {
    timer = millis();
    if (timer - forrigeTimer < 600000) { //programmet skal vaere 10 min lang foor den slaar seg av
      fireMin = millis();
      enMin = millis();
      if (fireMin - forrigeFire < 240000) { //programmet for selve danse delen
        tid = millis();
        if (tid - forrige > intervall) { //intervaller for naar det skal komme random nummer
          forrige = tid;
          nyRandom(); //generere nye tall
        }
        if (enEllerTo == 0) { //bestemmer om det skal vaere en eller to lys som slaar paa
          enLys(randomNumber);
        } else {
          toLys(randomNumber, r2);
        }
      }
      if (enMin - forrigeEn > 240000) { // programmet for push-ups delen
        //endre dette til push-ups
        digitalWrite(gronn, HIGH);
        digitalWrite(gul, HIGH);
        digitalWrite(rod, HIGH);
        digitalWrite(bla, HIGH);
      }
      if (enMin - forrigeEn > 300000) {//reset da forrigeFire og forrigeEn, saann at det er mulig ta danse og push-ups delen paa nytt
        forrigeFire = fireMin;
        forrigeEn = enMin;
      }
    } else {
      ferdig();
    }
  }
}

void enLys(int rnd) {
  //gronn LED
  if (tid - forrige < intervall) {
    if (rnd == 0) {
      if (clicked == 0) {
        if (digitalRead(gronnKnapp) == HIGH) {
          digitalWrite(gronn, HIGH);
        } else {
          digitalWrite(gronn, LOW);
          clicked++;
        }
      }
    }
  } else {
    digitalWrite(gronn, LOW);
    clicked = 0;
  }

  //gul LED
  if (tid - forrige < intervall) {
    if (rnd == 1) {
      if (clicked == 0) {
        if (digitalRead(gulKnapp) == HIGH) {
          digitalWrite(gul, HIGH);
        } else {
          digitalWrite(gul, LOW);
          clicked++;
        }
      }
    }
  } else {
    digitalWrite(gul, LOW);
    clicked = 0;
  }

  //rod LED
  if (tid - forrige < intervall) {
    if (rnd == 2) {
      if (clicked == 0) {
        if (digitalRead(rodKnapp) == HIGH) {
          digitalWrite(rod, HIGH);
        } else {
          digitalWrite(rod, LOW);
          clicked++;
        }
      }
    }
  } else {
    digitalWrite(rod, LOW);
    clicked = 0;
  }

  //bla LED
  if (tid - forrige < intervall) {
    if (rnd == 3) {
      if (clicked == 0) {
        if (digitalRead(blaKnapp) == HIGH) {
          digitalWrite(bla, HIGH);
        } else {
          digitalWrite(bla, LOW);
          clicked++;
        }
      }
    }
  } else {
    digitalWrite(bla, LOW);
    clicked = 0;
  }
}

void toLys(int rnd, int r) {
  //gronn og gul LED
  if (tid - forrige < intervall) {
    if ((rnd == 0 && r == 1) || (rnd == 1 && r == 0)) {
      if (clicked == 0) {
        if (digitalRead(gronnKnapp) == LOW && digitalRead(gulKnapp) == LOW) {
          digitalWrite(gronn, LOW);
          digitalWrite(gul, LOW);
          clicked++;
        } else {
          digitalWrite(gronn, HIGH);
          digitalWrite(gul, HIGH);
        }
      }
    }
  } else {
    digitalWrite(gronn, LOW);
    digitalWrite(gul, LOW);
    clicked = 0;
  }

  //gronn og rod LED
  if (tid - forrige < intervall) {
    if ((rnd == 0 && r == 2) || (rnd == 2 && r == 0)) {
      if (clicked == 0) {
        if (digitalRead(gronnKnapp) == LOW && digitalRead(rodKnapp) == LOW) {
          digitalWrite(gronn, LOW);
          digitalWrite(rod, LOW);
          clicked++;
        } else {
          digitalWrite(gronn, HIGH);
          digitalWrite(rod, HIGH);
        }
      }
    }
  } else {
    digitalWrite(gronn, LOW);
    digitalWrite(rod, LOW);
    clicked = 0;
  }

  //gronn og bla LED
  if (tid - forrige < intervall) {
    if ((rnd == 0 && r == 3) || (rnd == 3 && r == 0)) {
      if (clicked == 0) {
        if (digitalRead(gronnKnapp) == LOW && digitalRead(blaKnapp) == LOW) {
          digitalWrite(gronn, LOW);
          digitalWrite(bla, LOW);
          clicked++;
        } else {
          digitalWrite(gronn, HIGH);
          digitalWrite(bla, HIGH);
        }
      }
    }
  } else {
    digitalWrite(gronn, LOW);
    digitalWrite(bla, LOW);
    clicked = 0;
  }

  //gul og rod LED
  if (tid - forrige < intervall) {
    if ((rnd == 1 && r == 2) || (rnd == 2 && r == 1)) {
      if (clicked == 0) {
        if (digitalRead(gulKnapp) == LOW && digitalRead(rodKnapp) == LOW) {
          digitalWrite(gul, LOW);
          digitalWrite(rod, LOW);
          clicked++;
        } else {
          digitalWrite(gul, HIGH);
          digitalWrite(rod, HIGH);
        }
      }
    }
  } else {
    digitalWrite(gul, LOW);
    digitalWrite(rod, LOW);
    clicked = 0;
  }

  //gul og bla LED
  if (tid - forrige < intervall) {
    if ((rnd == 1 && r == 3) || (rnd == 3 && r == 1)) {
      if (clicked == 0) {
        if (digitalRead(gulKnapp) == LOW && digitalRead(blaKnapp) == LOW) {
          digitalWrite(gul, LOW);
          digitalWrite(bla, LOW);
          clicked++;
        } else {
          digitalWrite(gul, HIGH);
          digitalWrite(bla, HIGH);
        }
      }
    }
  } else {
    digitalWrite(gul, LOW);
    digitalWrite(bla, LOW);
    clicked = 0;
  }

  //bla og rod LED
  if (tid - forrige < intervall) {
    if ((rnd == 2 && r == 3) || (rnd == 3 && r == 2)) {
      if (clicked == 0) {
        if (digitalRead(rodKnapp) == LOW && digitalRead(blaKnapp) == LOW) {
          digitalWrite(rod, LOW);
          digitalWrite(bla, LOW);
          clicked++;
        } else {
          digitalWrite(rod, HIGH);
          digitalWrite(bla, HIGH);
        }
      }
    }
  } else {
    digitalWrite(rod, LOW);
    digitalWrite(bla, LOW);
    clicked = 0;
  }
}

void ferdig() { //dette kjøører naar man har stoppet programmet eller naar tiden har stoppet
  digitalWrite(gronn, LOW);
  digitalWrite(gul, LOW);
  digitalWrite(rod, LOW);
  digitalWrite(bla, LOW);
  start = 0;
  forrigeFire = fireMin;
  forrigeEn = enMin;
}

void nyRandom() { //metode for aa generere nye tall for random
  enEllerTo = random(0, 2);
  randomNumber = random(0, 4);
  r2 = random(0, 4);
}
