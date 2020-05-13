int gronn = 2;
int gul = 3;
int rod = 4;
int bla = 5;
int gronnKnapp = A4;
int gulKnapp = A3;
int rodKnapp = A2;
int blaKnapp = A1;
int start = 6;
//legg til flere knapper

//variabler som skal holde de random tallene
long randomNumber;
long r2;
long enEllerTo;

unsigned long tid;
unsigned long forrigeTimer = 0;
unsigned long forrige = 0;
int clicked = 0;
int intervall = 1000;
unsigned long timer = 0;
int run;

void setup() {
  pinMode(gronn, OUTPUT);
  pinMode(gul, OUTPUT);
  pinMode(rod, OUTPUT);
  pinMode(bla, OUTPUT);
  pinMode(gronnKnapp, INPUT);
  pinMode(gulKnapp, INPUT);
  pinMode(rodKnapp, INPUT);
  pinMode(blaKnapp, INPUT);
  digitalWrite(gronnKnapp, HIGH);
  digitalWrite(gulKnapp, HIGH);
  digitalWrite(rodKnapp, HIGH);
  digitalWrite(blaKnapp, HIGH);
  run = 0;


  //RandomSeed er for å kunne gjøre at det blir random tall hele tiden ved å bruke en analog port som ikke blir brukt.
  randomSeed(analogRead(A0));

}

void loop() {
  if (digitalRead(start) == HIGH) { //trykker på start knappen gjør at den kan kjøre
    if (run == 0) { //gjør at den kjører
      run = 255;
      forrigeTimer = timer;
    } else { //trykker en gang til for å gjøre at den ikke kjører
      run = 0;
      ferdig();
    }
  }

  if (run > 0) {
    timer = millis();
    if (timer - forrigeTimer < 600000) { //programmet skal være 10 min lang før den slår seg av
      tid = millis();
      if (tid - forrige > intervall) { //intervaller for når det skal komme random nummer
        forrige = tid;
        enEllerTo = random(0, 2);
        randomNumber = random(0, 4);
        r2 = random(0, 4);
      }
      if (enEllerTo == 0) { //bestemmer om det skal være en eller to lys som slår på
        enLys(randomNumber);
      } else {
        toLys(randomNumber, r2);
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

void ferdig() {
  digitalWrite(gronn, LOW);
  digitalWrite(gul, LOW);
  digitalWrite(rod, LOW);
  digitalWrite(bla, LOW);
}
