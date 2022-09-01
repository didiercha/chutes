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
int phase = 0; // si 0, préparation de la chute , si 1 calcul de la durée, si 3 calculs et fin
const byte NbExp = 10;
float mesDurees[NbExp];
byte i;
byte j = 0;

float moyenne = 0;
float variance2 = 0;
unsigned long timedebut = 0; // 3 lignes pour le calcul durée de la chute statique
unsigned long timefin = 0;
unsigned long time = 0;
unsigned long somme = 0;
// CalculEcart
float Ecart = 0;
float Carre[NbExp];
void setup()
{
  Serial.begin(9600);
  pinMode(PIN_CHRONO, INPUT);
  pinMode(PIN_LEDJAUNE, OUTPUT);
  pinMode(PIN_LASER, OUTPUT);
  pinMode(PIN_AIMANT, OUTPUT);
  pinMode(bouton, INPUT);
  phase = 0;
  digitalWrite(PIN_LEDJAUNE, LOW);

  LCD.init(); // initialisation de l'afficheur
  LCD.backlight();
  // LCD.setCursor(0, 0);
  // LCD.print(phase);
}

void loop()
{
  for (int i = 1; i <= NbExp; i++)

  {

    if (phase == 0)
    {
      etatChrono = 0;
      digitalWrite(PIN_LASER, HIGH);
      digitalWrite(PIN_AIMANT, HIGH);
      LCD.setCursor(0, 0); // affiche dès le 1er caractère
      LCD.print("Placer l'objet");
      LCD.print(phase);
      LCD.print(i);
      delay(3000);

      while (digitalRead(PIN_CHRONO) == LOW) // etatPhotodiode = digitalRead(PIN_CHRONO);
        // si la photodiode est éclairée, il faut la masquer
      {
        LCD.setCursor(0, 1); // affichage 2eme ligne
        LCD.print("Masquer PhotoDiode");
        delay(500);
      }
    }
    while (phase == 0 && etatChrono == 0)
    {
      LCD.setCursor(0, 1); // affichage 2eme ligne
      digitalWrite(PIN_LEDJAUNE, HIGH);

      LCD.print("-Appuyer Bouton ");

      etat = analogRead(bouton); // teste l'etat du bouton

      if (etat <= 300) // teste si le bouton est appuyé
      {
        phase = 1;
        Serial.print("phase");
        Serial.println(phase); // pour passer à la suite
        // digitalWrite(PIN_LEDJAUNE, LOW); //éteindre led jaune pour savoir si le bouton a été pris en compte
      }
    }
    if (phase == 1 && etatChrono == 0)
    {
      // chute statique
      digitalWrite(PIN_LEDJAUNE, LOW); //éteindre led jaune pour savoir si le bouton a été pris en compte
      digitalWrite(PIN_AIMANT, LOW);   // debut chute
      etatChrono = 1;
      LCD.print(etatChrono); // pour éviter que timedebut soit réécrit dans la boucle
      timedebut = micros();  // enregistre l'heure de la chute

    }

    while (phase == 1 && etatChrono == 1) //
    {
      etatPhotodiode = digitalRead(PIN_CHRONO); // on attend l'arrivée de l'objet
      if (etatPhotodiode == LOW)                //-------------------fin de la chute
      {
        timefin = micros(); // enregistre l'heure          de l'arrivée de l'objet
        // timefin = timefin - 13000;  correction inutile ici !!!

        digitalWrite(PIN_LEDJAUNE, LOW);
        digitalWrite(PIN_LASER, LOW); // laser inutile donc OFF

        time = timefin - timedebut; // durée de la chute en µs
        etatChrono = 2;             // pour que le programme passe à la suite
        LCD.clear();                // efface tout
        LCD.setCursor(0, 0);
        LCD.print("Duree de chute :");

        // on affiche le temps de chute

        time = time / 1000; // pour avoir le résultat en ms
        Serial.println("time : ");
        Serial.println(time);
        LCD.setCursor(1, 1);
        LCD.print(time);
        LCD.print(" ms ** N =");
        LCD.print(i); // affiche le numéro de la mesure
        j = i - 1;
        mesDurees[j] = time; // stocke la mesure ds le tableau
        if (i < NbExp)
        {
          phase = 0;
        }
        else
        {
          phase = 3;
        }

        delay(3000); // affichage pendant 4 s.
      }
    }
  }
  while (phase == 3)

  {
    float somme = mesDurees[0]; // calcule la moyenne des durées
    for (int j = 1; j < NbExp ; j++)
    {
      somme = somme + mesDurees[j];
      Serial.println("somme");
      Serial.println(somme);
    }
    moyenne = somme / NbExp;
    LCD.setCursor(1, 1);
    LCD.print(moyenne);
    LCD.print(" ms *");
    LCD.print(i); // vérifie que i =NbExp ????
 Serial.print("moyenne :");
      Serial.println(moyenne); Serial.println(NbExp);


    // calcul de l'écart type
    for (int j = 0; j < NbExp; j++)
    {
      Carre[j] = sq(mesDurees[j]-moyenne);
      Serial.print(j);
      Serial.print("carre  ");
      Serial.println(Carre[j]);
    }
    for (int j = 0; j < NbExp; j++)
    {
      variance2 = variance2 + Carre[j] ;
    }
    Serial.print("variance2  "); Serial.print(variance2);//carré de la variance
    Ecart = sqrt(variance2 / NbExp);
    Serial.print("Ecart  "); Serial.println(Ecart);
     LCD.setCursor(0, 0);
    LCD.print("Duree ");LCD.print(moyenne);
    LCD.print(" ms *");
     LCD.setCursor(0, 1);LCD.print("ecart type ");
    LCD.print(Ecart);
    LCD.print(" ms *");
    phase = 8; //fin des calculs
  }
}
