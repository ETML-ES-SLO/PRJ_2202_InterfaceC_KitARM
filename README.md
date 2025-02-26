Le projet permet de communiquer avec le Starter kit-ARM de l’ES via son port série avec un programme en C sur Visual studio. Le but et de pouvoir concevoir un programme sur Visual studio qui permettra de piloter le starter Kit-ARM.  

voici la tramme utiliser pour la comunication:![image](https://github.com/Toxik24/2202_Interface_-StarterKit-ARM-1701-/assets/97610782/607f9cbb-4e74-4b8d-8a54-46ca19fae724)

Header:

Le Header byte sera tout le temps 0xFF pour la communication entre le kit et le PC. Cette byte représente comme une signature qui représente que on communique bien entre le kit et le PC et pas quelque chose d’autre.

Cmdld1 & Cmdld0:

C’est deux bytes sont destinés pour le choix du périphérique, que nous voulons atteindre sur le Kit ARM.

DataLen :

De l’octet 4 à n, ce sont les données.

CRC:

La dernier bytes (n+1) représente le CRC. C’est l’addition de tous les bytes sans compter le Header.

Concept Architecture de reception du Kit-ARM:

![image](https://github.com/Toxik24/2202_Interface_-StarterKit-ARM-1701-/assets/97610782/171a5eda-94e8-4808-b49a-1fb682419ac4)





