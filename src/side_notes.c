#include "variables.h"
#include "side_notes.h"
#include <stdio.h>
#include <stdlib.h>

void sidenote()
{
  int qoute = rand() % 26;
  switch (qoute)
  {
  case 0:
    printf("2 is forever lonely in the prime world!");
    break;
  case 1:
    printf("Error 404: Motivation Not Found");
    break;
  case 2:
    printf("One man's constant is another man's variable");
    break;
  case 3:
    printf("while(alive){ code; }");
    break;
  case 4:
    printf("1 + 1 = 10 (but in binary!)");
    break;
  case 5:
    printf("Just keep taking chances and having fun");
    break;
  case 6:
    printf("!false (it's true!)");
    break;
  case 7:
    printf("Recursion: see 'Recursion'");
    break;
  case 8:
    printf("It always seems impossible until it's done");
    break;
  case 9:
    printf("Real Programmers Count From 0");
    break;
  case 10:
    printf("Decimals have a point.");
    break;
  case 11:
    printf("π: Be Rational,i: Get Real");
    break;
  case 12:
    printf("It's not a bug, it's a feature");
    break;
  case 13:
    printf("Home is where the ~/ is");
    break;
  case 14:
    printf("Life is short. Use fast algorithms");
    break;
  case 15:
    printf("Trust the compiler. Fear the warnings");
    break;
  case 16:
    printf("Binary is just shy decimal");
    break;
  case 17:
    printf("Math is the language. Code is the accent");
    break;
  case 18:
    printf("404: Social Life Not Found");
    break;
  case 19:
    printf("Grass not found. Staying in terminal");
    break;
  case 20:
    printf("To C or not to C: that is the question");
    break;
  case 21:
    printf("Networking: It's all about connections");
    break;
  case 22:
    printf("Schrodinger's Cat: Wanted Dead and Alive");
    break;
  case 23:
    printf("Code never lies, comments sometimes do");
    break;
  case 24:
    printf("Git: In case of fire, commit and push");
    break;
  case 25:
    printf("I need a nap, not a solution");
    break;
  default:
    break;
  }
}
