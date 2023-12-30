# Chutes

## Objectif

Pour élèves de classes scientifiques. Mesure du temps de chute d'un objet.
l'expérience peut être menée 10 fois (ou plus suivant la programmation ) et le programme détermine la valeur la plus probable avec l'écart type.

 Il utilise un Arduino Uno, un écran LCD I2C, un laser, une photodiode et un bouton.

## Structure

Le programme est divisé en trois phases :

* **Préparation de la chute** (phase 0) : l'objet est placé sous l'électro-aimant et la photodiode est masquée. Le bouton est utilisé pour lancer la mesure.
* **Mesure de la durée de la chute** (phase 1) : l'électro-aimant est désactivé, ce qui libère l'objet. La photodiode détecte le passage de l'objet et enregistre le temps de chute.
* **Calcul de la moyenne et de l'écart type** (phase 3) : le programme calcule la moyenne des 10 mesures effectuées. Il calcule également l'écart type, qui est une mesure de la dispersion des données autour de la moyenne.

## Fonctionnement

Le programme fonctionne comme suit :

* **Dans la phase 0**, le programme affiche des instructions sur l'écran LCD. Il attend que l'objet soit placé et que la photodiode soit masquée. Lorsque le bouton est appuyé, la phase 1 est lancée.
* **Dans la phase 1**, le programme enregistre le temps de chute en microsecondes. Il affiche le temps de chute sur l'écran LCD. Si le nombre de mesures n'est pas encore atteint, la phase 0 est relancée. Sinon, la phase 3 est lancée.
* **Dans la phase 3**, le programme calcule la moyenne et l'écart type des 10 mesures. Il affiche les résultats sur l'écran LCD.

## Exemple

Voici un exemple d'affichage du programme :

```
Placer l'objet
0
1
Appuyer Bouton
-
Duree de chute : 1234 ms ** N =1
Duree de chute : 1235 ms ** N =2
...
Duree 1234 ms *
ecart type 12 ms *
```
