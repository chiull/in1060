#include <LiquidCrystal.h>

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

//variabler for poengsum utregninga
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int Contrast = 75;
unsigned long totalsum = 0;
unsigned long endeligSum = 0;
unsigned long highScore = 0;
unsigned long interv = 0;
unsigned long forrigeInterv = 0;
int stor = 300;
int medium = 100;
int liten = 50;

//variabler for push-ups/ultrasonic distance sensor
int trigPin = A5;
int echoPin = 7;
unsigned long varighet = 0;
unsigned long cm = 0;
int antallPushUps = 0;
unsigned long mellomrom = 0;
unsigned long forrigeMellom = 0;
unsigned long micro = 0;
unsigned long forrigeMicro = 0;

//variabler som blir brukt
unsigned long tid;
unsigned long forrige = 0;
unsigned long timer = 0;
unsigned long forrigeTimer = 0;
unsigned long fireMin = 0;
unsigned long forrigeFire = 0;
unsigned long enMin = 0;
unsigned long forrigeEn = 0;
int start = 0;
int clicked = 0;
int intervall = 1500;
int debounce = 0;
int forrigeDebounce = 0;

void setup() {
  analogWrite(1, Contrast);
  pinMode(gronn, OUTPUT);
  pinMode(gul, OUTPUT);
  pinMode(rod, OUTPUT);
  pinMode(bla, OUTPUT);
  pinMode(gronnKnapp, INPUT);
  pinMode(gulKnapp, INPUT);
  pinMode(rodKnapp, INPUT);
  pinMode(blaKnapp, INPUT);
  pinMode(startButton, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(gronnKnapp, HIGH);
  digitalWrite(gulKnapp, HIGH);
  digitalWrite(rodKnapp, HIGH);
  digitalWrite(blaKnapp, HIGH);
  digitalWrite(startButton, LOW);

  //skjermsetup
  lcd.begin(16, 2);

  //RandomSeed er for aa kunne gjoore at det blir random tall hele tiden ved aa bruke en analog port som ikke blir brukt.
  randomSeed(analogRead(A0));

}

void loop() {
  if (start == 0) { //skal vise til brukeren at de kan starte spillet
    lcd.setCursor(0, 0);
    lcd.print("Start spillet");
  }

  debounce = millis();//Debounce for gjoor at knappen ikke start ikke stopper med engang hvis man holder det litt for lenge
  if (debounce - forrigeDebounce > 500) {
    if (digitalRead(startButton) == HIGH) { //trykker paa start knappen gjoor at den kan kjoore
      forrigeDebounce = debounce;
      if (start == 0) { //gjoor at den kjoorer
        start = 255;
        forrigeTimer = timer;
        forrigeFire = fireMin;
        forrigeEn = enMin;
        lcd.clear();
      } else { //trykker en gang til for aa gjoore at den ikke kjoorer
        ferdig();
        lcd.clear();
      }
    }
  }

  if (start > 0) {
    timer = millis();

    if (timer - forrigeTimer < 600000) { //programmet skal vaere 10 min lang foor den slaar seg av
      lcd.setCursor(0, 0);
      lcd.print("SCORE");
      lcd.setCursor(0, 1);
      lcd.print(totalsum);
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
        PushUps();
      }
      if (enMin - forrigeEn > 300000) {//reset da forrigeFire og forrigeEn, saann at det er mulig ta danse og push-ups delen paa nytt
        forrigeFire = fireMin;
        forrigeEn = enMin;
        antallPushUps = 0;
        lcd.clear();
      }
    } else {
      interv = millis();
      if (totalsum > highScore) {
        highScore = totalsum;
      }
      endeligSum = totalsum;
      if (interv - forrigeInterv > 0) {
        lcd.setCursor(0, 0);
        lcd.print("FINAL SCORE");
        lcd.setCursor(0, 1);
        lcd.print(endeligSum);
      }
      if (interv - forrigeInterv > 5000) {
        lcd.setCursor(0, 0);
        lcd.print("HIGHSCORE");
        lcd.setCursor(0, 1);
        lcd.print(highScore);
      }
      if (interv - forrigeInterv > 5000) {
        forrigeInterv = interv;
      }
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
          digitalWrite(gul, LOW);
          digitalWrite(rod, LOW);
          digitalWrite(bla, LOW);
        } else {
          digitalWrite(gronn, LOW);
          totalsum += stor;
          clicked++;
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
        digitalWrite(gronn, LOW);
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
          digitalWrite(gronn, LOW);
          digitalWrite(gul, HIGH);
          digitalWrite(rod, LOW);
          digitalWrite(bla, LOW);
        } else {
          digitalWrite(gul, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          digitalWrite(gronn, LOW);
          digitalWrite(gul, LOW);
          digitalWrite(rod, HIGH);
          digitalWrite(bla, LOW);
        } else {
          digitalWrite(rod, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          digitalWrite(gronn, LOW);
          digitalWrite(gul, LOW);
          digitalWrite(rod, LOW);
          digitalWrite(bla, HIGH);
        } else {
          digitalWrite(bla, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {
          digitalWrite(gronn, HIGH);
          digitalWrite(gul, HIGH);
          digitalWrite(rod, LOW);
          digitalWrite(bla, LOW);
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {
          digitalWrite(gronn, HIGH);
          digitalWrite(rod, HIGH);
          digitalWrite(gul, LOW);
          digitalWrite(bla, LOW);
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {
          digitalWrite(gronn, HIGH);
          digitalWrite(bla, HIGH);
          digitalWrite(gul, LOW);
          digitalWrite(rod, LOW);
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {
          digitalWrite(gul, HIGH);
          digitalWrite(rod, HIGH);
          digitalWrite(gronn, LOW);
          digitalWrite(bla, LOW);
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {
          digitalWrite(gul, HIGH);
          digitalWrite(bla, HIGH);
          digitalWrite(gronn, LOW);
          digitalWrite(rod, LOW);
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {
          digitalWrite(rod, HIGH);
          digitalWrite(bla, HIGH);
          digitalWrite(gronn, LOW);
          digitalWrite(gul, LOW);
        }
      } else { //gjør at det skifter farge med engang istedenfor å vente til tiden har gått
        forrige = tid;
        nyRandom();
        clicked = 0;
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
  forrigeFire = fireMin;
  forrigeEn = enMin;
  totalsum = 0;
}

void nyRandom() { //metode for aa generere nye tall for random
  enEllerTo = random(0, 2);
  randomNumber = random(0, 4);
  r2 = random(0, 4);
}

void PushUps() {
  int tall = 0;
  if (tall == 0){
    lcd.clear();
    tall++;
  }
  //slår av alle lysene til begynne med
  digitalWrite(gronn, LOW);
  digitalWrite(gul, LOW);
  digitalWrite(rod, LOW);
  digitalWrite(bla, LOW);

  mellomrom = millis();
  micro = micros();

  digitalWrite(trigPin, HIGH); //slår av Ultrasonic distance sensor
  if (micro - forrigeMicro > 5) { //venter at den 5 microsekunder
    forrigeMicro = micro;
    digitalWrite(trigPin, LOW); //så slår den på distance sensor sånn at den sender lyd ut sånn at den kan detektere avstand
  }
  if (micro - forrigeMicro > 10) { //venter 5 microsekunder så slår den av
    forrigeMicro = micro;
    digitalWrite(trigPin, HIGH);
  }
  varighet = pulseIn(echoPin, HIGH, 200000); //lagerer dataen til varighet

  cm = (varighet / 2) / 29.1; //gjør om tallene fra distance sensoren til cm
  if (cm <= 10) { // sjekker om man er innen 10cm til sensoren
    if (mellomrom - forrigeMellom > 350) {
      forrigeMellom = mellomrom;
      if (antallPushUps == 1) {
        lcd.setCursor(10, 1);
        lcd.print("+500");
        totalsum += 500;
      }
      if (antallPushUps == 2) {
        lcd.setCursor(10, 1);
        lcd.print("+600");
        totalsum += 600;
      }
      if (antallPushUps == 3) {
        lcd.setCursor(10, 1);
        lcd.print("+700");
        totalsum += 700;
      }
      if (antallPushUps == 4) {
        lcd.setCursor(10, 1);
        lcd.print("+800");
        totalsum += 800;
      }
      if (antallPushUps == 5) {
        lcd.setCursor(10, 1);
        lcd.print("+900");
        totalsum += 900;
      }
      if (antallPushUps > 5) {
        lcd.setCursor(10, 1);
        lcd.print("+1000");
        totalsum += 1000;
      }
      antallPushUps++;// legger det til tellingen
      lcd.setCursor(0, 0);
      lcd.print("ANT: " + antallPushUps);
      lcd.setCursor(0, 1);
      lcd.print(totalsum);
    }
  }
}
