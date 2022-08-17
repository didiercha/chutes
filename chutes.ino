// mesure du temps de chute libre
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C LCD(0x27, 16, 2); // définit le type d'écran lcd 16 x 2
#define PIN_LEDJAUNE 3
#define PIN_LASER 2
int etatPhotodiode;
#define PIN_CHRONO 5  //(fil ...  + )
#define PIN_AIMANT 11 //(fil
const int bouton = A1;
int etatChrono = 0;
int etat = 0;
int phase = 0;
unsigned long timedebut = 0; // 3 lignes pour le calcul durée de la chute statique
unsigned long timefin = 0;
unsigned long time;

void setup()
{
  pinMode(PIN_CHRONO, INPUT);
  pinMode(PIN_LEDJAUNE, OUTPUT);
  pinMode(PIN_LASER, OUTPUT);
  pinMode(PIN_AIMANT, OUTPUT);
  pinMode(bouton, INPUT);
  phase = 0;
  digitalWrite(PIN_LEDJAUNE, LOW);
  digitalWrite(PIN_LASER, HIGH);
  digitalWrite(PIN_AIMANT, HIGH);
  LCD.init(); // initialisation de l'afficheur
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print(phase);
}

void loop()
{
  if (phase == 0)
  {
    LCD.setCursor(0, 0); // affiche dès le 1er caractère
    LCD.print(" Placer l'objet ");
    delay(3000);

    if (digitalRead(PIN_CHRONO) == LOW) // etatPhotodiode = digitalRead(PIN_CHRONO);
                                        // si la photodiode est éclairée, il faut la masquer
    {
      LCD.setCursor(0, 1); // affichage 2eme ligne décallée au 2éme caractère
      LCD.print("Masquer PhotoDiode");
      delay(500);
    }
    else
    {

      LCD.setCursor(1, 1); // affichage 2eme ligne décallée au 2éme caractère
      digitalWrite(PIN_LEDJAUNE, HIGH);

      LCD.print("-Appuyer Bouton _-_");

      etat = analogRead(bouton); // teste l'etat du bouton
      

      if (etat <= 300) // teste si le bouton est appuyé
      {
        phase = 1;                       // pour passer à la suite
        digitalWrite(PIN_LEDJAUNE, LOW); //éteindre led jaune pour savoir si le bouton a été pris en compte
        digitalWrite(PIN_AIMANT, LOW);   // debut chute
        etatChrono = 1;                  // pour éviter que timedebut soit réécrit dans la boucle
        timedebut = micros();            // enregistre l'heure de la chute
      }
    }
  }
  if (phase == 1 && etatChrono == 1)
  {
    etatPhotodiode = digitalRead(PIN_CHRONO); // on attend l'arrivée de l'objet
    if (etatPhotodiode == LOW)                //-------------------fin de la chute
    {
      timefin = micros();        // enregistre l'heure de l'arrivée de l'objet
      timefin = timefin - 13000; // correction due à la lattence du capteur

      digitalWrite(PIN_LEDJAUNE, LOW);
      digitalWrite(PIN_LASER, LOW); // laser inutile donc OFF

      time = timefin - timedebut; // durée de la chute
      etatChrono = 2;             // pour que le programme passe à la suite
      LCD.clear();                // efface tout
      LCD.setCursor(0, 0);
      LCD.print("Duree de la chute :");

      // on affiche le temps de chute

      time = time / 1000;
      LCD.setCursor(1, 2);
      LCD.print(time);
      LCD.print(" ms **");
      delay(4000); // affichage pendant 4 s.

      phase = 3;
    }
  }
}
