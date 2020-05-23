#include <LiquidCrystal.h>

//Pin som blir brukt for lys og start knappen
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
unsigned long enEllerTo;

//variabler for poengsum utregninga og lcd skjerm
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int Contrast = 75; //Contrast er brukt for å slippe å bruke potensiometer på LCD skjerm.
unsigned long totalsum = 0;
unsigned long endeligSum = 0;
unsigned long highScore = 0;
unsigned long interv = 0;
unsigned long forrigeInterv = 0;
int stor = 300;
int tall = 0;
int t = 0;
int forst = 0;

//variabler for push-ups/ultrasonic distance sensor
int trigPin = A5; //trigger pin på sensoren for input
int echoPin = 7; //echo pin på sensoren for output
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
  analogWrite(1, Contrast);//setter kontrast på LCD skjerm
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
  digitalWrite(startButton, HIGH);

  //skjermsetup
  lcd.begin(16, 2);

  //RandomSeed er for aa kunne gjoore at det blir random tall hele tiden ved aa bruke en analog port som ikke blir brukt.
  randomSeed(analogRead(A0));
}

void loop() {
  debounce = millis();//Debounce for gjoor at knappen ikke start ikke stopper med engang hvis man holder det litt for lenge
  if (debounce - forrigeDebounce > 500) {
    if (digitalRead(startButton) == LOW) { //trykker paa start knappen gjoor at den kan kjoore
      forrigeDebounce = debounce;
      if (start == 0) { //gjoor at den kjoorer
        start = 255; //gjør at start = 255 sånn at den starter selve hovedprogrammet
        //setter forrige lik som timer, fireMin og enMin sånn at den starter på begynnelsen når man trykker på start knappen
        forrigeTimer = timer; 
        forrigeFire = fireMin;
        forrigeEn = enMin;
        tall = 0; //tall er brukt for å fjerne det som står på lcd skjermen første gang istedenfor gjøre det hele tiden
        forst = 0; //forst gjør at den endeligePoengsummen er totalpoengsum uten at det endrer seg hele tiden 
        lcd.clear(); //fjerner alt som står på lcd
      } else { //trykker en gang til for aa gjoore at den ikke kjoorer
        ferdig();
        lcd.clear();
        start = 0; //setter start som 0, sånn at den kan starte opp igjen hvis man trykker på start
      }
    }
  }
  if (start == 0) { //skal vise til brukeren at de kan starte spillet
    lcd.setCursor(0, 0);
    lcd.print("Start spillet");
  }
  if (start > 0) { //Dette er det som kjører når man trykker på start knappen
    timer = millis();
    if (timer - forrigeTimer < 600000) { //programmet skal vaere 10 min lang foor den slaar seg av
      fireMin = millis();
      enMin = millis();
      if (fireMin - forrigeFire < 240000) { //programmet for selve danse delen
        lcd.setCursor(0, 0);
        lcd.print("SCORE");
        lcd.setCursor(0, 1);
        lcd.print(totalsum);
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
      if (fireMin - forrigeFire > 240000) {//fjerner det som er på skjermen, og gjør det klart til å gå over til Push-Ups fasen
        if (tall == 0) {
          lcd.clear();
          tall++;
        }
      }
      if (enMin - forrigeEn > 240000) { // programmet for push-ups delen
        PushUps();
      }
      if (enMin - forrigeEn > 300000) {//reset da forrigeFire og forrigeEn, saann at det er mulig ta danse og push-ups delen paa nytt
        forrigeFire = fireMin;
        forrigeEn = enMin;
        antallPushUps = 0;
        tall = 0;
        lcd.clear();
      }
    } else { //dette slutt delen, som roterer mellom poengsummet du fikk for den runden og høyeste poengsummen man har fått for den spill økten
      interv = millis();
      if (totalsum > highScore) {//setter ny highScore hvis totalsum er større
        highScore = totalsum;
      }
      if (forst == 0){ //gjør at endeligSum har poensummet som brukeren fikk i løpet av runden
        endeligSum = totalsum;
        forst++;
      }
      if (interv - forrigeInterv < 5000) { //viser den endelige poengsummet for runden
        if (tall == 0) { //brukt for gjør at skjermen ikke blinker
          lcd.clear();
          tall++;
        }
        lcd.setCursor(0, 0);
        lcd.print("FINAL SCORE");
        lcd.setCursor(0, 1);
        lcd.print(endeligSum);
      }
      if (interv - forrigeInterv > 5000) {//viser den høyeste poengsummen for en spille økt
        if (t == 0) {//brukt for gjør at skjermen ikke blinker
          lcd.clear();
          t++;
        }
        lcd.setCursor(0, 0);
        lcd.print("HIGHSCORE");
        lcd.setCursor(0, 1);
        lcd.print(highScore);
      }
      if (interv - forrigeInterv > 10000) { //gjør at den starter på nytt med å vise poengsummen
        forrigeInterv = interv;
        tall = 0;
        t = 0;
      }
      ferdig(); //gjør at lysene er av
    }
  }
}

void enLys(int rnd) {
  //gronn LED
  if (tid - forrige < intervall) {
    if (rnd == 0) { 
      if (clicked == 0) {
        if (digitalRead(gronnKnapp) == HIGH) { //hvis knappen ikke er trykket på så vil denne kjøre
          digitalWrite(gronn, HIGH);
          digitalWrite(gul, LOW);
          digitalWrite(rod, LOW);
          digitalWrite(bla, LOW);
        } else { //når den blir trykket på så skifter den til neste farge med engang
          digitalWrite(gronn, LOW);
          //gir poeng paa riktig klikk
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
        if (digitalRead(gulKnapp) == HIGH) {//hvis knappen ikke er trykket på så vil denne kjøre
          digitalWrite(gronn, LOW);
          digitalWrite(gul, HIGH);
          digitalWrite(rod, LOW);
          digitalWrite(bla, LOW);
        } else { //når den blir trykket på så skifter den til neste farge med engang
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
        if (digitalRead(rodKnapp) == HIGH) {//hvis knappen ikke er trykket på så vil denne kjøre
          digitalWrite(gronn, LOW);
          digitalWrite(gul, LOW);
          digitalWrite(rod, HIGH);
          digitalWrite(bla, LOW);
        } else { //når den blir trykket på så skifter den til neste farge med engang
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
        if (digitalRead(blaKnapp) == HIGH) {//hvis knappen ikke er trykket på så vil denne kjøre
          digitalWrite(gronn, LOW);
          digitalWrite(gul, LOW);
          digitalWrite(rod, LOW);
          digitalWrite(bla, HIGH);
        } else {//når den blir trykket på så skifter den til neste farge med engang
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
        if (digitalRead(gronnKnapp) == LOW && digitalRead(gulKnapp) == LOW) { //hvis knappene blir trykket så får man poeng og endrer fargen
          digitalWrite(gronn, LOW);
          digitalWrite(gul, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {//denne kjører så lenge knappen ikke er trykket på
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
  } else { //denne kjører når tiden har gått over
    digitalWrite(gronn, LOW);
    digitalWrite(gul, LOW);
    clicked = 0;
  }

  //gronn og rod LED
  if (tid - forrige < intervall) { 
    if ((rnd == 0 && r == 2) || (rnd == 2 && r == 0)) {
      if (clicked == 0) {
        if (digitalRead(gronnKnapp) == LOW && digitalRead(rodKnapp) == LOW) {//hvis knappene blir trykket så får man poeng og endrer fargen
          digitalWrite(gronn, LOW);
          digitalWrite(rod, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {//denne kjører så lenge knappen ikke er trykket på
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
  } else {//denne kjører når tiden har gått over
    digitalWrite(gronn, LOW);
    digitalWrite(rod, LOW);
    clicked = 0;
  }

  //gronn og bla LED
  if (tid - forrige < intervall) {
    if ((rnd == 0 && r == 3) || (rnd == 3 && r == 0)) {
      if (clicked == 0) {
        if (digitalRead(gronnKnapp) == LOW && digitalRead(blaKnapp) == LOW) {//hvis knappene blir trykket så får man poeng og endrer fargen
          digitalWrite(gronn, LOW);
          digitalWrite(bla, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {//denne kjører så lenge knappen ikke er trykket på
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
  } else {//denne kjører når tiden har gått over
    digitalWrite(gronn, LOW);
    digitalWrite(bla, LOW);
    clicked = 0;
  }

  //gul og rod LED
  if (tid - forrige < intervall) {
    if ((rnd == 1 && r == 2) || (rnd == 2 && r == 1)) {
      if (clicked == 0) {
        if (digitalRead(gulKnapp) == LOW && digitalRead(rodKnapp) == LOW) {//hvis knappene blir trykket så får man poeng og endrer fargen
          digitalWrite(gul, LOW);
          digitalWrite(rod, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {//denne kjører så lenge knappen ikke er trykket på
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
  } else {//denne kjører når tiden har gått over
    digitalWrite(gul, LOW);
    digitalWrite(rod, LOW);
    clicked = 0;
  }

  //gul og bla LED
  if (tid - forrige < intervall) {
    if ((rnd == 1 && r == 3) || (rnd == 3 && r == 1)) {
      if (clicked == 0) {
        if (digitalRead(gulKnapp) == LOW && digitalRead(blaKnapp) == LOW) {//hvis knappene blir trykket så får man poeng og endrer fargen
          digitalWrite(gul, LOW);
          digitalWrite(bla, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {//denne kjører så lenge knappen ikke er trykket på
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
  } else {//denne kjører når tiden har gått over
    digitalWrite(gul, LOW);
    digitalWrite(bla, LOW);
    clicked = 0;
  }

  //bla og rod LED
  if (tid - forrige < intervall) {
    if ((rnd == 2 && r == 3) || (rnd == 3 && r == 2)) {
      if (clicked == 0) {
        if (digitalRead(rodKnapp) == LOW && digitalRead(blaKnapp) == LOW) {//hvis knappene blir trykket så får man poeng og endrer fargen
          digitalWrite(rod, LOW);
          digitalWrite(bla, LOW);
          //gir poeng paa riktig klikk
          totalsum += stor;
          clicked++;
        } else {//denne kjører så lenge knappen ikke er trykket på
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
  } else {//denne kjører når tiden har gått over
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

void PushUps() { //metoden for Push-Ups fasen
  //endrer lcd skjermen til å vise antall pushups, istedenfor poengsum
  lcd.setCursor(0, 0);
  lcd.print("Push ups");
  lcd.setCursor(0, 1);
  lcd.print("antall");
  lcd.setCursor(8, 1);
  lcd.print(antallPushUps);

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
      //poengene gradevis øker for hver antall Push-Ups
      if (antallPushUps == 1) {
        totalsum += 500;
      }
      if (antallPushUps == 2) {
        totalsum += 600;
      }
      if (antallPushUps == 3) {
        totalsum += 700;
      }
      if (antallPushUps == 4) {
        totalsum += 800;
      }
      if (antallPushUps == 5) {
        totalsum += 900;
      }
      //når man har tatt mer enn 5 push-ups så får du samme poengsum for hver push-ups etter det
      if (antallPushUps > 5) {
        totalsum += 1000;
      }
      antallPushUps++;// legger det til tellingen
    }
  }
}
