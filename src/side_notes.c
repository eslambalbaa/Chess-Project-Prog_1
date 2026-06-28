#include "variables.h"
#include "side_notes.h"
#include <stdlib.h>

const char* get_random_sidenote() {
    const char *notes[] = {
      "2 is forever lonely in the prime world!",
      "Error 404: Motivation Not Found",
      "One man's constant is another man's variable",
      "while(alive){ code; }",
      "1 + 1 = 10 (but in binary!)",
      "Just keep taking chances and having fun",
      "!false (it's true!)",
      "Recursion: see 'Recursion'",
      "It always seems impossible until it's done",
      "Real Programmers Count From 0",
      "Decimals have a point.",
      "π: Be Rational,i: Get Real",
      "It's not a bug, it's a feature",
      "Home is where the ~/ is",
      "Life is short. Use fast algorithms",
      "Trust the compiler. Fear the warnings",
      "Binary is just shy decimal",
      "Math is the language. Code is the accent",
      "404: Social Life Not Found",
      "Grass not found. Staying in terminal",
      "To C or not to C: that is the question",
      "Networking: It's all about connections",
      "Schrodinger's Cat: Wanted Dead and Alive",
      "Code never lies, comments sometimes do",
      "Git: In case of fire, commit and push",
      "I need a nap, not a solution"};
    int num_quotes = sizeof(notes) / sizeof(notes[0]);
    return notes[rand() % num_quotes];
}
