/*
                                          ARDUINO BASIC MUSIC GENERATOR
                                 ===============================================


   This code lets you play basic music sheets (one track) from ANY 'digitalWrite()' Arduino compatible board. Using a
   passive buzzer, a transistorized speaker or even a stepper motor with its corresponding driver you can play music :D

   BUZZER_PIN    → (pinNumber) Digital output pin

   musicTempo   → (float) Number of crotchet per minute

   musicPitch   → (float) Change pitch in semitones. 0 is no change at all. Positive values are high-pitched and negative ones are low-pitched

   INVERT_LOGIC  → By enabling this, output pin will be defined as inverted logic (to use with an inverted buzzer)

   int musicFreqs[]       → In this int array, you can put your pre-defined music's notes or any frequency you want

   float musicSymbols[]   → In this float array, you can define the music symbols per each note:
                             0.25 for a Semiquaver
                             0.5  for a Quaver
                             1.0  for a Crotchet
                             2.0  for a Minim
                             4.0  for a Semibreve
                             0.33 for a Triplet of quavers
                             and so on...



   "THE BEERWARE LICENSE" (Revision 42):

   Aaron G. wrote this code in January 2019. As long as you retain this
   notice, you can do whatever you want with this stuff. If we meet someday,
   and you think this stuff is worth it, you can buy me a beer in return.

*/

#include "frequencies.h"

// === Config ===
#define BUZZER_PIN        2     // Digital output pin
//#define INVERT_LOGIC
#define STEPPER_ENABLE    8


#ifdef INVERT_LOGIC
#define BZ_ON   LOW
#define BZ_OFF  HIGH
#else
#define BZ_ON   HIGH
#define BZ_OFF  LOW
#endif


float musicTempo = 120;     // Number of crotchets per minute
float musicPitch = 0;       // Note change in semitones
float freqChange;
int counter;



// === Sample music ===

// Jingle Bells
int musicFreqsJingle[] =          {D6,  B6,  A6,  G6,  D6,  D6,  D6,  D6,  B6,  A6,  G6,  E6,  0,   E6,  C7,  B6,  A6,  FS6,  0,  D7,  D7,  C7,  A6,  B6,  D6,  B6,  A6,  G6,  D6,  D6,  D6,  D6,  B6,  A6,  G6,  E6,  0,   E6,  C7,  B6,  A6,  D7,  D7,  D7,  D7,  E7,  D7,  C7,  A6,  G6,  0,   B6,  B6,  B6,  B6,  B6,  B6,  B6,  D7,  G6,  A6,  B6,  C7,  C7,  C7,  C7,  C7,  B6,  B6,  B6,  B6,  B6,  A6,  A6,  B6,  A6,  D7,  0,   B6,  B6,  B6,  B6,  B6,  B6,  B6,  D7,  G6,  A6,  B6,  0,   C7,  C7,  C7,  C7,  C7,  B6,  B6,  B6,  B6,  D7,  D7,  C7,  A6,  G6,  0};
float musicSymbolsJingle[] =      {1.0, 1.0, 1.0, 1.0, 3.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0, 1.0, 1.0, 1.0, 1.0, 4.0, 1.0, 1.0, 1.0, 1.0, 3.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.5, 0.5, 3.0, 1.0, 1.0, 1.5, 0.5, 1.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.5, 0.5, 3.0, 1.0, 1.0, 1.0, 1.5, 0.5, 1.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0};
/*
// Happy Birthday
int musicFreqsBirthday[] =        {C6,  C6,  D6,  C6,  F6,  E6,  C6,  C6,  D6,  C6,  G6,  F6,  C6,  C6,  C7,  A6,  F6,  E6,  D6,  AS6, AS6, A6,  F6,  G6,  F6};
float musicSymbolsBirthday[] =    {0.5, 0.5, 1.0, 1.0, 1.0, 2.0, 0.5, 0.5, 1.0, 1.0, 1.0, 2.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 0.5, 1.0, 1.0, 1.0, 4.0};

// Super Mario
int musicFreqsMario[] =           {E7,  E7,  0,   E7,  0,   C7,  E7,  0,   G7,  0,   0,   0,   G6,  0,   0,   0,   C7,  0,   0,   G6,  0,   0,   E6,  0,   0,   A6,  0,   B6,  0,   AS6, A6,  0,   G6,   E7,   G7,   A7,  0,   F7,  G7,  0,   E7,  0,   C7,  D7,  B6,  0,   0,   C7,  0,   0,   G6,  0,   0,   E6,  0,   0,   A6,  0,   B6,  0,   AS6, A6,  0,   G6,   E7,   G7,   A7,  0,   F7,  G7,  0,   E7,  0,   C7,  D7,  B6,  0,   0};
float musicSymbolsMario[] =       {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.75, 0.75, 0.75, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.75, 0.75, 0.75, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

// La Cucaracha
int musicFreqsCucaracha[] =       {0,   C5,  C5,  C5,  F5,  0,   A5,  0,   C5,  C5,  C5,  0,   F5,  0,   A5,  0,   F5,  F5,  E5,  E5,  D5,  D5,  C5,  0,   C5,  C5,  C5,  E5,  0,   G5,  0,   C5,  C5,  C5,  E5,  0,   G5,  0,   C6,  D6,  C6,  AS5, A5,  G5,  F5};
float musicSymbolsCucaracha[] =   {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 0.5, 1.5, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2.0, 0.5, 0.5, 0.5, 0.5, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 0.5, 1.5, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0 };

// Despacito
int musicFreqsDespacito[] =       {B5,  0,    FS5,  B5,   CS6,  D6,   E6,   D6,   CS6,  B5,   A5,   G5,   D6,   D6,  0,   D6,  A5,  D6,  A5,  D6,  A5,  D6,  E6,   CS6,  0,   B5,  0,    FS5,  B5,   CS6,  D6,   E6,   D6,   CS6,  B5,   A5,   G5,   D6,   D6,   E6,   D6,  0,   D6,  A5,  D6,  A5,  D6,  A5,  D6,  E6,   CS6,  0,   B5,  B5,   B5,   B5,   CS6,  D6,   CS6,  D6,   CS6, D6,   CS6, B5,  B5,   B5,   B5,   CS6,  D6,   CS6,  D6,   CS6, D6,   E6,   A5,   A5,   A5,   A5,   A5,   D6,   CS6,  D6,   CS6,  D6,  E6,   E6,   CS6, 0};
float musicSymbolsDespacito[] =   {1.0, 0.25, 0.25, 0.25, 0.25, 0.33, 0.33, 0.33, 0.33, 0.33, 0.33, 0.75, 0.75, 1.5, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.25, 1.25, 3.0, 1.0, 0.25, 0.25, 0.25, 0.25, 0.33, 0.33, 0.33, 0.33, 0.33, 0.33, 0.75, 0.75, 0.25, 0.25, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.25, 1.25, 3.0, 1.0, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.5, 1.0, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 1.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.75, 1.0, 2.5};
*/


void generateTone(uint16_t noteFreq, uint16_t noteTime) {
  if (noteFreq == 0) {
    digitalWrite(BUZZER_PIN, BZ_OFF);
    delay(noteTime);
  } else {
    uint16_t semiPeriod = (uint16_t)(1000000 / (noteFreq * 2.0));
    uint32_t startTime = millis();
    do {
      digitalWrite(BUZZER_PIN, BZ_ON);
      delayMicroseconds(semiPeriod);
      digitalWrite(BUZZER_PIN, BZ_OFF);
      delayMicroseconds(semiPeriod);
    } while ((millis() - startTime) < noteTime);
  }
  delay(10);
}



void setup() {
  // Make digital pin an OUTPUT
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(STEPPER_ENABLE, OUTPUT);

  // Enable Stepper motor in case of using one
  digitalWrite(STEPPER_ENABLE, HIGH);


  // A for() loop is used to play the whole song for one time, no matter the lenght of it

  // Play Super Jingle Bells
  musicTempo = 250;
  musicPitch = -12;
  freqChange = pow(2, ((float)musicPitch / 12.0));
  for (counter = 0; counter < (sizeof(musicFreqsJingle) / sizeof(int)); counter++) {
    generateTone((int)(musicFreqsJingle[counter] * freqChange), (int)((60000 / musicTempo) * musicSymbolsJingle[counter]));
  }
  delay(2000);
/*
  // Play Super Mario Theme
  musicTempo = 400;
  musicPitch = -12;
  freqChange = pow(2, ((float)musicPitch / 12.0));
  for (counter = 0; counter < (sizeof(musicFreqsMario) / sizeof(int)); counter++) {
    generateTone((int)(musicFreqsMario[counter] * freqChange), (int)((60000 / musicTempo) * musicSymbolsMario[counter]));
  }
  delay(2000);


  // Play La Cucaracha
  musicTempo = 150;
  musicPitch = 0;
  freqChange = pow(2, ((float)musicPitch / 12.0));
  for (counter = 0; counter < (sizeof(musicFreqsCucaracha) / sizeof(int)); counter++) {
    generateTone((int)(musicFreqsCucaracha[counter] * freqChange), (int)((60000 / musicTempo) * musicSymbolsCucaracha[counter]));
  }
  delay(2000);


  // Play Despacito
  musicTempo = 110;
  musicPitch = 0;
  freqChange = pow(2, ((float)musicPitch / 12.0));
  for (counter = 0; counter < (sizeof(musicFreqsDespacito) / sizeof(int)); counter++) {
    generateTone((int)(musicFreqsDespacito[counter] * freqChange), (int)((60000 / musicTempo) * musicSymbolsDespacito[counter]));
  }
  delay(2000);


  // Play Happy Birthday
  musicTempo = 105;
  musicPitch = 0;
  freqChange = pow(2, ((float)musicPitch / 12.0));
  for (counter = 0; counter < (sizeof(musicFreqsBirthday) / sizeof(int)); counter++) {
    generateTone((int)(musicFreqsBirthday[counter] * freqChange), (int)((60000 / musicTempo) * musicSymbolsBirthday[counter]));
  }
*/

  // Disable Stepper motor in case of using one
  digitalWrite(8, LOW);
}

void loop() {
}
