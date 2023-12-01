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

// === Config ===
#define BUZZER_PIN        2     // Digital output pin
//#define INVERT_LOGIC
#define STEPPER_ENABLE    8



// === Pre-defined note frequencies (Hz) ===
#define SILENT   0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

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

// Super Mario
int musicFreqsJingle[] =       {NOTE_D6, NOTE_B6, NOTE_A6, NOTE_G6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_B6, NOTE_A6, NOTE_G6, NOTE_E6, 0,  NOTE_E6, NOTE_C7, NOTE_B6, NOTE_A6, NOTE_FS6, 0,   NOTE_D7, NOTE_D7, NOTE_C7, NOTE_A6, NOTE_B6,  NOTE_D6, NOTE_B6, NOTE_A6, NOTE_G6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_B6, NOTE_A6, NOTE_G6, NOTE_E6, 0,  NOTE_E6, NOTE_C7, NOTE_B6, NOTE_A6, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_E7, NOTE_D7, NOTE_C7, NOTE_A6, NOTE_G6, 0,   NOTE_B6, NOTE_B6, NOTE_B6, NOTE_B6, NOTE_B6, NOTE_B6, NOTE_B6, NOTE_D7, NOTE_G6, NOTE_A6, NOTE_B6, NOTE_C7, NOTE_C7, NOTE_C7, NOTE_C7, NOTE_C7, NOTE_B6, NOTE_B6, NOTE_B6, NOTE_B6, NOTE_B6, NOTE_A6, NOTE_A6, NOTE_B6, NOTE_A6, NOTE_D7, 0};
float musicSymbolsJingle[] =   {1.0,     1.0,     1.0,     1.0,     3.0,     0.5,     0.5,     1.0,     1.0,     1.0,     1.0,     3.0,     1.0,1.0,     1.0,     1.0,     1.0,     3.0,      1.0, 1.0,     1.0,     1.0,     1.0,     4.0,      1.0,     1.0,     1.0,     1.0,     3.0,     0.5,     0.5,     1.0,     1.0,     1.0,     1.0,     3.0,     1.0,1.0,     1.0,     1.0,     1.0,     1.0,     1.0,     1.0,     1.0,     1.0,     1.0,     1.0,     1.0,     3.0,     1.0, 1.0,     1.0,     2.0,     1.0,     1.0,     2.0,     1.0,     1.0,     1.5,     0.5,     3.0,     1.0,     1.0,     1.5,     0.5,     1.0,     1.0,     1.0,     0.5,     0.5,     1.0,     1.0,     1.0,     1.0,     2.0,     1.0,     1.0};

// Super Mario
int musicFreqsMario[] =       { NOTE_E7, NOTE_E7, 0, NOTE_E7, 0, NOTE_C7, NOTE_E7, 0, NOTE_G7, 0, 0,  0, NOTE_G6, 0, 0, 0, NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0, NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0, NOTE_C7, NOTE_D7, NOTE_B6, 0, 0, NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0, NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0, NOTE_C7, NOTE_D7, NOTE_B6, 0, 0};
float musicSymbolsMario[] =   {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.75, 0.75, 0.75, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.75, 0.75, 0.75, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

// La Cucaracha
int musicFreqsCucaracha[] =       { SILENT, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_F5, SILENT, NOTE_A5, SILENT, NOTE_C5, NOTE_C5, NOTE_C5, SILENT, NOTE_F5, SILENT, NOTE_A5, SILENT, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, SILENT, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_E5, SILENT, NOTE_G5, SILENT, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_E5, SILENT, NOTE_G5, SILENT, NOTE_C6, NOTE_D6, NOTE_C6, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5    };
float musicSymbolsCucaracha[] =   {   0.5,   0.5,    0.5,    0.5,    0.5,    0.5,   0.5,    0.5,    0.5,   0.5,    0.5,    0.5,   1.0,    0.5,   1.5,    1.0,   0.5,    0.5,    0.5,    0.5,   0.5,     0.5,    2.0,    0.5,    0.5,   0.5,    0.5,    1.0,    0.5,   0.5,    0.5,   0.5,    0.5,    0.5,    1.0,    0.5,   1.5,    1.0,   0.5,    0.5,    0.5,    0.5,    0.5,     0.5,    1.0      };

// Despacito
int musicFreqsDespacito[] =       { NOTE_B5, SILENT, NOTE_FS5, NOTE_B5, NOTE_CS6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_CS6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_D6, NOTE_D6, SILENT, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_E6, NOTE_CS6, SILENT, NOTE_B5, SILENT, NOTE_FS5, NOTE_B5, NOTE_CS6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_CS6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_D6, SILENT, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_E6, NOTE_CS6, SILENT, NOTE_B5, NOTE_B5, NOTE_B5, NOTE_B5, NOTE_CS6, NOTE_D6, NOTE_CS6, NOTE_D6, NOTE_CS6, NOTE_D6, NOTE_CS6, NOTE_B5, NOTE_B5, NOTE_B5, NOTE_B5, NOTE_CS6, NOTE_D6, NOTE_CS6, NOTE_D6, NOTE_CS6, NOTE_D6, NOTE_E6, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_D6, NOTE_CS6, NOTE_D6, NOTE_CS6, NOTE_D6, NOTE_E6, NOTE_E6, NOTE_CS6, SILENT    };
float musicSymbolsDespacito[] =   {    1.0,  0.25,   0.25,    0.25,    0.25,   0.33,   0.33,   0.33,  0.33,    0.33,    0.33,   0.75,   0.75,   1.5,    1.0,   0.5,   0.5,    0.5,     0.5,    0.5,    0.5,    0.5,    0.25,   1.25,   3.0,    1.0,  0.25,   0.25,    0.25,    0.25,   0.33,   0.33,   0.33,  0.33,    0.33,    0.33,    0.75,   0.75,   0.25,   0.25,   1.0,   1.0,    0.5,   0.5,    0.5,     0.5,    0.5,    0.5,    0.5,    0.25,   1.25,   3.0,    1.0,   0.25,    0.25,   0.25,    0.25,  0.25,    0.25,    0.25,    0.5,   0.25,    0.5,    1.0,    0.25,    0.25,   0.25,   0.25,    0.25,   0.25,   0.25,     0.5,    0.25,   0.25,   1.25,   0.25,   0.25,   0.25,   0.25,   0.25,  0.25,    0.25,    0.25,    0.5,    0.25,   0.75,    1.0,    2.5     };

// Happy Birthday
int musicFreqsBirthday[] =       { NOTE_C6, NOTE_C6, NOTE_D6, NOTE_C6, NOTE_F6, NOTE_E6, NOTE_C6, NOTE_C6, NOTE_D6, NOTE_C6, NOTE_G6, NOTE_F6, NOTE_C6, NOTE_C6, NOTE_C7, NOTE_A6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_AS6, NOTE_AS6, NOTE_A6, NOTE_F6, NOTE_G6, NOTE_F6       };
float musicSymbolsBirthday[] =   {  0.5,    0.5,    1.0,    1.0,     1.0,    2.0,   0.5,     0.5,    1.0,    1.0,    1.0,   2.0,      0.5,   0.5,    1.0,    1.0,    1.0,    1.0,   2.0,     0.5,     0.5,     1.0,    1.0,    1.0,    4.0         };




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
  for (counter = 0; counter < (sizeof(musicFreqsMario) / sizeof(int)); counter++) {
    generateTone((int)(musicFreqsJingle[counter] * freqChange), (int)((60000 / musicTempo) * musicSymbolsJingle[counter]));
  }
  delay(2000);

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
  

  // Disable Stepper motor in case of using one
  digitalWrite(BUZZER_PIN, BZ_OFF);
  digitalWrite(8, LOW);
}

void loop() {
}
