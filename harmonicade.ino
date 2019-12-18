//-----------------------------------------------------------------------------------------------//
//--                                -----  Harmonicade  -----                                  --//
//--                  A modular 5.5 (x2) octave, multi-channel MIDI keyboard                   --//
//--                using arcade push-buttons in the Wicki-Hayden button layout.               --//
//--                    Written for Teensy 3.6 with MIDIUSB library v1.0.4                     --//
//--                          Copyright (C) 2019 - Michael Koopman                             --//
//--                       KOOP Instruments (koopinstruments@gmail.com)                        --//
//-----------------------------------------------------------------------------------------------//
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//
// Hardware Information:
// Arduino compatible micro-controller with sufficient I/O:
// Pin requirments:
//     37 I/O pins in total --
//         4 analog
//         32 digital
//         1 serial TX
//     1 +5V for DIN MIDI
//     1 +3.3V/+5V for potentiometers
//     1 GND for DIN MIDI and potentiometers
//
// 231 x digital buttons in total (21 columns * 11 rows)
//   4 x potentiometers
//   2 x 1/4" TS jacks
//   1 x 5 pin DIN MIDI serial jack
//
// Device pinout:
// (Both decks are wired identically; rotate right deck 180 deg to position)
// Name                  DB25 Male (Decks)     DB25 Female Left (Control)  DB25 Female Right (Control)     Teensy 3.6 Pin
// ----------------------------------------------------------------------------------------------------------------------
// Matrix Col 0          24                    24                          N/A                             2
// Matrix Col 1          23                    23                          N/A                             3
// Matrix Col 2          22                    22                          N/A                             4
// Matrix Col 3          21                    21                          N/A                             5
// Matrix Col 4          20                    20                          N/A                             6
// Matrix Col 5          19                    19                          N/A                             7
// Matrix Col 6          18                    18                          N/A                             8
// Matrix Col 7          17                    17                          N/A                             9
// Matrix Col 8          16                    16                          N/A                             10
// Matrix Col 9          15                    15                          N/A                             11
// --
// Matrix Col 10         N/A                   N/A                         N/A                             38
// --
// Matrix Col 11 (9)     15                    N/A                         15                              12
// Matrix Col 12 (8)     16                    N/A                         16                              24
// Matrix Col 13 (7)     17                    N/A                         17                              25
// Matrix Col 14 (6)     18                    N/A                         18                              26
// Matrix Col 15 (5)     19                    N/A                         19                              27
// Matrix Col 16 (4)     20                    N/A                         20                              28
// Matrix Col 17 (3)     21                    N/A                         21                              29
// Matrix Col 18 (2)     22                    N/A                         22                              30
// Matrix Col 19 (1)     23                    N/A                         23                              31
// Matrix Col 20 (0)     24                    N/A                         24                              32
// --
// Matrix Row 0          12                    12                          2                               39
// Matrix Row 1          11                    11                          3                               14
// Matrix Row 2          10                    10                          4                               15
// Matrix Row 3          9                     9                           5                               16
// Matrix Row 4          8                     8                           6                               17
// Matrix Row 5          7                     7                           7                               18
// Matrix Row 6          6                     6                           8                               19
// Matrix Row 7          5                     5                           9                               20
// Matrix Row 8          4                     4                           10                              21
// Matrix Row 9          3                     3                           11                              22
// Matrix Row 10         2                     2                           12                              23
// --
// Pot UL                                                                                                  A14
// Pot UR                                                                                                  A15
// Pot LL                                                                                                  A16
// Pot LR                                                                                                  A17
// --
// DIN MIDI TX           N/A                   N/A                         N/A                             1
// 
// Button number locations in the decks and control panel (square brackets indicate unused locations):
//                                              Controls
//                                               ------
//                   Left Deck                  | 0  1 |                    Right Deck
//  ------------------------------------------------------------------------------------------------
// |   0   1   2   3   4   5   6   7   8 [  9]  |      |  [  0]   1   2   3   4   5   6   7   8   9 |
// |  10  11  12  13  14  15  16  17  18   19   |      |    10   11  12  13  14  15  16  17  18  19 |
// |  20  21  22  23  24  25  26  27  28 [ 29]  |   2  |  [ 20]  21  22  23  24  25  26  27  28  29 |
// |  30  31  32  33  34  35  36  37  38   39   |   3  |    30   31  32  33  34  35  36  37  38  39 |
// |  40  41  42  43  44  45  46  47  48 [ 49]  |   4  |  [ 40]  41  42  43  44  45  46  47  48  49 |
// |  50  51  52  53  54  55  56  57  58   59   |   5  |    50   51  52  53  54  55  56  57  58  59 |
// |  60  61  62  63  64  65  66  67  68 [ 69]  |   6  |  [ 60]  61  62  63  64  65  66  67  68  69 |
// |  70  71  72  73  74  75  76  77  78   79   |   7  |    70   71  72  73  74  75  76  77  78  79 |
// |  80  81  82  83  84  85  86  87  88 [ 89]  | [ 8] |  [ 80]  81  82  83  84  85  86  87  88  89 |
// |  90  91  92  93  94  95  96  97  98   99   | [ 9] |    90   91  92  93  94  95  96  97  98  99 |
// | 100 101 102 103 104 105 106 107 108 [109]  | [10] |  [100] 101 102 103 104 105 106 107 108 109 |
//  ------------------------------------------------------------------------------------------------
// Control Panel Knobs:
// Input                                                   Input
// ------------------------------------------------------------------------------------------------
// Upper Left:     Left Deck Main Velocity                 Upper Right:    Right Deck Main Velocity
// Lower Left:     Metronome/Layer Velocity                Lower Right:    Metronome Speed
//
// Control Panel Buttons:
// Input            Normal Function     Meta A              Meta B                      Meta A+B
// ---------------------------------------------------------------------------------------------------------------------------
// Control Up:      Pitch Bend Up       Main Oct Up         Drums Mode Toggle           Pitch Bend Whole Step/Half Step Toggle
// Control Middle:  Apply Modulation    Chorus Toggle       Reverb Toggle               Controller Reset
// Control Down:    Pitch Bend Down     Main Oct Down       Legato/Drone Mode Toggle    Pitch Bend Fast/Slow Toggle
// Loop Button:     Looper              N/A                 N/A                         N/A
//
// Foot Switches:
// Input                                                    Input
// --------------------------------------------------------------------------------------------
// Expression Pedal (left side/0): Expression (MIDI CC #11) Looper Pedal (right side/1): Looper
//
// Note Buttons:
// Input           Left Deck                                                           Right Deck
// ---------------------------------------------------------------------------------------------------------------------------------------------------
// Meta A          Left Deck Program Select 0-103                                      Right Deck Program Select 0-103
// Meta B          Left Deck Layer Program Select 0-103                                Right Deck Layer Program Select 0-103
// Meta A+B        Main Prg Select 104-127 | Layer Mute | Layer Prg Select 104-127     Main Prg Select 104-127 | Layer Mute | Layer Prg Select 104-127
// 
// MIDI Channel Assignments:
// Left Deck:                  MIDI CH 0
// Right Deck:                 MIDI CH 1
// Left Deck Layer:            MIDI CH 2
// Right Deck Layer:           MIDI CH 3
// Looper:                     MIDI CH 4
// Looper:                     MIDI CH 5
// Looper:                     MIDI CH 6
// Looper:                     MIDI CH 7
// Looper:                     MIDI CH 8
// Drums/Looper:               MIDI CH 9
// Looper:                     MIDI CH 10
// Looper:                     MIDI CH 11
// Looper:                     MIDI CH 12
// Looper:                     MIDI CH 13
// Looper:                     MIDI CH 14
// Looper:                     MIDI CH 15

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START OF PROGRAM

#include "MIDIUSB.h"

// Define digital button matrix pins
const byte leftDeckColumns[]        = { 2,  3,  4,  5,  6,  7,  8,  9, 10, 11};         // Left deck column pins in order from left to right
const byte rightDeckColumns[]       = {12, 24, 25, 26, 27, 28, 29, 30, 31, 32};         // Right deck column pins in order from left to right
const byte controlDeckColumns[]     = {38};                                             // Control deck column
const byte buttonDeckRows[]         = {39, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};     // Shared button deck rows in order from top to bottom
const byte controlDeckRows[]        = {39, 14, 15, 16, 17, 18, 19, 20};                 // Control deck rows in order from top to bottom
const byte leftDeckColumnCount      = sizeof(leftDeckColumns);                          // The number of left deck columns in the matrix
const byte rightDeckColumnCount     = sizeof(rightDeckColumns);                         // The number of right deck columns in the matrix
const byte controlDeckColumnCount   = sizeof(controlDeckColumns);                       // The number of control deck columns in the matrix
const byte buttonDeckRowCount       = sizeof(buttonDeckRows);                           // The number of shared button deck rows in the matrix
const byte controlDeckRowCount      = sizeof(controlDeckRows);                          // The number of control deck rows in the matrix
const byte leftDeckElementCount     = leftDeckColumnCount * buttonDeckRowCount;         // The number of elements in the left deck matrix
const byte rightDeckElementCount    = rightDeckColumnCount * buttonDeckRowCount;        // The number of elements in the right deck matrix
const byte controlDeckElementCount  = controlDeckColumnCount * controlDeckRowCount;     // The number of elements in the control deck matrix

// Define analog input pins
const byte ULPotPin = A14;
const byte URPotPin = A15;
const byte LLPotPin = A16;
const byte LRPotPin = A17;

// MIDI note value tables for the Wicki-Hayden (default) deck layouts, and the drums layout
const byte leftDeckWickiHayden[leftDeckElementCount] = {
   78,  80,  82,  84,  86,  88,  90,  92,  94,   0,
71,  73,  75,  77,  79,  81,  83,  85,  87,  89,
   66,  68,  70,  72,  74,  76,  78,  80,  82,   0,
59,  61,  63,  65,  67,  69,  71,  73,  75,  77,
   54,  56,  58,  60,  62,  64,  66,  68,  70,   0,
47,  49,  51,  53,  55,  57,  59,  61,  63,  65,
   42,  44,  46,  48,  50,  52,  54,  56,  58,   0,
35,  37,  39,  41,  43,  45,  47,  49,  51,  53,
   30,  32,  34,  36,  38,  40,  42,  44,  46,   0,
23,  25,  27,  29,  31,  33,  35,  37,  39,  41,
   18,  20,  22,  24,  26,  28,  30,  32,  34,   0
};
const byte rightDeckWickiHayden[rightDeckElementCount] = {
 0,  90,  92,  94,  96,  98, 100, 102, 104, 106,
   83,  85,  87,  89,  91,  93,  95,  97,  99, 101,
 0,  78,  80,  82,  84,  86,  88,  90,  92,  94,
   71,  73,  75,  77,  79,  81,  83,  85,  87,  89,
 0,  66,  68,  70,  72,  74,  76,  78,  80,  82,
   59,  61,  63,  65,  67,  69,  71,  73,  75,  77,
 0,  54,  56,  58,  60,  62,  64,  66,  68,  70,
   47,  49,  51,  53,  55,  57,  59,  61,  63,  65,
 0,  42,  44,  46,  48,  50,  52,  54,  56,  58,
   35,  37,  39,  41,  43,  45,  47,  49,  51,  53,
 0,  30,  32,  34,  36,  38,  40,  42,  44,  46
};
const byte leftDeckDrums[leftDeckElementCount] = {
   00,  00,  00,  00,  00,  00,  00,  00,  00,   0,
00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
   00,  00,  00,  00,  28,  00,  00,  00,  00,   0,
00,  25,  26,  29,  30,  86,  87,  78,  79,  00,
   73,  74,  75,  82,  27,  83,  84,  71,  72,   0,
00,  77,  76,  62,  64,  63,  85,  68,  67,  00,
   66,  65,  41,  47,  48,  50,  57,  80,  81,   0,
00,  61,  60,  43,  45,  48,  49,  52,  69,  00,
   70,  58,  42,  46,  46,  42,  51,  59,  54,   0,
25,  39,  40,  37,  38,  38,  37,  40,  31,  25,
   44,  35,  55,  36,  35,  36,  53,  35,  56,   0
};

// MIDI program selection value tables
const byte leftDeckMidiProgramsA[leftDeckElementCount] = {
    0,   1,   2,   3,   4,   5,   6,   7,   8, 000,
  9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
   19,  20,  21,  22,  23,  24,  25,  26,  27, 000,
 28,  29,  30,  31,  32,  33,  34,  35,  36,  37,
   38,  39,  40,  41,  42,  43,  44,  45,  46, 000,
 47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
   57,  58,  59,  60,  61,  62,  63,  64,  65, 000,
 66,  67,  68,  69,  70,  71,  72,  73,  74,  75,
   76,  77,  78,  79,  80,  81,  82,  83,  84, 000,
 85,  86,  87,  88,  89,  90,  91,  92,  93,  94,
   95,  96,  97,  98,  99, 100, 101, 102, 103, 000
};
const byte leftDeckMidiProgramsB[leftDeckElementCount] = {
  104, 105, 106, 107, 108, 109, 110, 111, 112, 000,
113, 114, 115, 116, 117, 118, 119, 120, 121, 121,
  123, 124, 125, 126, 127, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
  000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
  000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
  104, 105, 106, 107, 108, 109, 110, 111, 112, 000,
113, 114, 115, 116, 117, 118, 119, 120, 121, 121,
  123, 124, 125, 126, 127, 000, 000, 000, 000, 000
};
const byte rightDeckMidiProgramsA[rightDeckElementCount] = {
000,   0,   1,   2,   3,   4,   5,   6,   7,   8,
    9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
000,  19,  20,  21,  22,  23,  24,  25,  26,  27,
   28,  29,  30,  31,  32,  33,  34,  35,  36,  37,
000,  38,  39,  40,  41,  42,  43,  44,  45,  46,
   47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
000,  57,  58,  59,  60,  61,  62,  63,  64,  65,
   66,  67,  68,  69,  70,  71,  72,  73,  74,  75,
000,  76,  77,  78,  79,  80,  81,  82,  83,  84,
   85,  86,  87,  88,  89,  90,  91,  92,  93,  94,
000,  95,  96,  97,  98,  99, 100, 101, 102, 103
};
const byte rightDeckMidiProgramsB[rightDeckElementCount] = {
000, 104, 105, 106, 107, 108, 109, 110, 111, 112,
  113, 114, 115, 116, 117, 118, 119, 120, 121, 121,
000, 123, 124, 125, 126, 127, 000, 000, 000, 000,
  000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
  000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
  000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 104, 105, 106, 107, 108, 109, 110, 111, 112,
  113, 114, 115, 116, 117, 118, 119, 120, 121, 121,
000, 123, 124, 125, 126, 127, 000, 000, 000, 000
};

// Global time variables
unsigned long   currentTime;                                            // Time in milliseconds since power on
const byte      debounceTime = 50;                                      // Global debounce time in milliseconds

// Variables for holding digital button states and debounce times
byte            activeButtonsLeft[leftDeckElementCount];                // Array to hold current button states (1/0) in the left deck
byte            previousActiveButtonsLeft[leftDeckElementCount];        // Array to hold previous button states (1/0) in the left deck for comparison
unsigned long   activeButtonsLeftTime[leftDeckElementCount];            // Array to track last activation time for debounce
byte            activeButtonsRight[rightDeckElementCount];              // Array to hold current button states (1/0) in the right deck
byte            previousActiveButtonsRight[rightDeckElementCount];      // Array to hold previous button states (1/0) in the right deck for comparison
unsigned long   activeButtonsRightTime[rightDeckElementCount];          // Array to track last activation time for debounce
byte            activeButtonsControl[controlDeckElementCount];          // Array to hold current button states (1/0) in the control deck
byte            previousActiveButtonsControl[controlDeckElementCount];  // Array to hold previous button states (1/0) in the control deck for comparison
unsigned long   activeButtonsControlTime[controlDeckElementCount];      // Array to track last activation time for debounce

// Control button states
byte expressionPedalState;                                              // Expression pedal - 1/4" TS jack left side
byte loopPedalState;                                                    // Loop pedal - 1/4" TS jack right side - mirrors loop button function
byte controlUpState;                                                    // Top control button (white)
byte controlMiddleState;                                                // Middle control button (black)
byte controlDownState;                                                  // Bottom control button (white)
byte metaAState;                                                        // Top meta key (red)
byte metaBState;                                                        // Bottom meta key (red)
byte loopButtonState;                                                   // Loop button (white) - mirrors loop pedal function
byte previousExpressionPedalState;                                      // Input locking variable
byte previousControlUpState;                                            // Input locking variable
byte previousControlMiddleState;                                        // Input locking variable
byte previousControlDownState;                                          // Input locking variable
byte previousMetaAState;                                                // Input locking variable
byte previousMetaBState;                                                // Input locking variable

// Analog knob values
const byte analogDeadzone = 5;                                          // The amount an analog reading needs to change before the variable is updated
const byte analogReadings = 24;                                         // The number of iterations to keep a running average for smoothing analog inputs
byte analogIndex;                                                       // Array index position for tracking averages
int  ULPotAverage;                                                      // The average value of the Upper Left pot
int  previousULPotAverage;                                              // The previous average value of the pot for deadzone comparison
int  ULPotReadings[analogReadings];                                     // Array of previous readings that will be FIFO cycled each loop
word ULPotTotal;                                                        // Total value of all readings that will be divided by number of readings to find the average
int  URPotAverage;                                                      // The average value of the Upper Right pot
int  previousURPotAverage;                                              // The previous average value of the pot for deadzone comparison
int  URPotReadings[analogReadings];                                     // Array of previous readings that will be FIFO cycled each loop
word URPotTotal;                                                        // Total value of all readings that will be divided by number of readings to find the average
int  LLPotAverage;                                                      // The average value of the Lower Left pot
int  previousLLPotAverage;                                              // The previous average value of the pot for deadzone comparison
int  LLPotReadings[analogReadings];                                     // Array of previous readings that will be FIFO cycled each loop
word LLPotTotal;                                                        // Total value of all readings that will be divided by number of readings to find the average
int  LRPotAverage;                                                      // The average value of the Lower Right pot
int  previousLRPotAverage;                                              // The previous average value of the pot for deadzone comparison
int  LRPotReadings[analogReadings];                                     // Array of previous readings that will be FIFO cycled each loop
word LRPotTotal;                                                        // Total value of all readings that will be divided by number of readings to find the average

// MIDI channel assignments
// Channels 4-15 (including drums channel 9) are used by the looper function
      byte leftDeckMidiChannel          = 0;                            // Toggle between 0 (default) and 9 (MIDI percussion channel)
const byte leftDeckDefaultMidiChannel   = 0;
const byte rightDeckMidiChannel         = 1;
const byte leftDeckLayerMidiChannel     = 2;
const byte rightDeckLayerMidiChannel    = 3;
const byte drumsChannel                 = 9;                            // MIDI percussion channel

// MIDI program variables
byte midiProgram[16];                                                   // Save program selection per channel (0-15)
byte leftDeckActive;                                                    // Track the whole button deck activation state on a program loop for quick comparison purposes
byte rightDeckActive;                                                   // Track the whole button deck activation state on a program loop for quick comparison purposes

// Octave modifier
int octave;                                                             // MIDI note number offset; adjustable up or down by user in steps of 12

// MIDI CC related variables
byte chorusState;                                                       // Toggle for enabling/disabling chorus effect (MIDI CC #93)
byte reverbState;                                                       // Toggle for enabling/disabling reverb effect (MIDI CC #91)

// Velocity levels
byte leftDeckVelocity   = 95;                                           // Default velocity for testing purposes; this will update immediately via analog pot
byte rightDeckVelocity  = 95;                                           // Default velocity for testing purposes; this will update immediately via analog pot
byte multiVelocity      = 95;                                           // Default velocity for the metronome and the layer channels; this will update immediately via analog pot

// Metronome
word metronomeSpeed;                                                    // The length of time in milliseconds between metronome clicks; updates immediately via analog pot
unsigned long previousMetronomeClickTime;                               // Previous click time for comparison

// Pitch Bend
int pitchOffset     = 0;                                                // Offset amount for pitch bending.  pitchSpeed is added to this each loop up to a max value of 8191
int pitchSpeed      = 500;                                              // The amount to increment pitchOffset each program loop (user can toggle between 500 "slow" and 1500 "fast")
int pitchStep       = 8191;                                             // The maximum amount that pitchOffset is allowed reach (toggles between 8191 for whole-step and 4095 for half-step)
int referencePitch  = 8192;                                             // Default neutral pitch position; could exploit this for global instrument tuning (i.e. reference pitch other than A=440Hz)

// Toggles for layered instrument, drums and drone/legato modes
byte leftDeckLayerNotesEnabled;                                         // Toggle optional layered instrument channel on/off for the left deck
byte rightDeckLayerNotesEnabled;                                        // Toggle optional layered instrument channel on/off for the right deck
byte leftDeckDrumsModeEnabled;                                          // Toggle optional drums mode on/off for the left deck, and switch to a button layout to one more comfortable for finger drumming
byte leftDeckLegatoModeEnabled;                                         // Toggle optional drone/legato mode on/off for the left deck (sustain enabled on every button press, and then blipped off and on for each subsequent press)

// Looper related variables
byte loopActivationState;                                               // Loop activation state variable shared by both loop pedal and loop button
byte previousLoopActivationState;                                       // Previous state variable for comparison
const byte maxTracks = 12;                                              // The maximum number of tracks available to the looper
const int maxIndexes = 2040;                                            // The maximum number of indexes allowed for each channel (limited by available RAM)
const byte loopChannelAssignment[maxTracks] = {4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 9};   // The MIDI channel assignment dedicated to each looper track (0-12)
unsigned long midiPacketIndex;                                          // A running counter of all MIDI packets sent to the bus
unsigned long previousMidiPacketIndex;                                  // The previous packet counter index value for comparison
byte loopRecordingEnabled;                                              // The recording state of the looper on/off
byte loopPlaybackEnabled;                                               // The playback state of the looper on/off
byte loopInMemory;                                                      // Flag indicating if a loop is recorded to memory or not
byte loopDrumsInMemory;                                                 // Flag indicating if a drums loop is currently saved in memory or not (dedicated track 12)
unsigned long loopStartTime;                                            // The global loop start time.  This number is subtracted from subsequent saved event times to record relative event times in the loop.  Resets to 0 on every playback loop.
unsigned long loopDuration;                                             // The global loop duration.  When currentTime is equal to loopStartTime plus this number, the loopStartTime and indexes are reset to 0 for another playthrough.
unsigned long loopResetClock = 4294967295;                              // When the loop button is held, this value is set to currentTime + 1500.  When currentTime equals this number, the looper is reset. Default is max value of an unsigned long (49.7 days of milliseconds).
byte previousLoopResetState;                                            // Input locking variable for the loop reset button
byte currentLoopIteration;                                              // Variable to store the currently recording loop iteration.  When this is incremented, a new loop is initialized (can be expanded in the future for loop overdubbing).
byte previousLoopIteration;                                             // Previous loop iteration for comparison
byte loopTrackActive[maxTracks];                                        // Flag indicating if this loop channel is in use
int loopTrackIndex[maxTracks];                                          // The current index position of the arrays assigned to this loop track
int loopTrackHighestIndex[maxTracks];                                   // The highest track index reached thus far in the current loop track.  Needed for overflow detection, and for limiting unnecessary array searches when scanning a full array.
byte loopTrackEventType[maxTracks][maxIndexes];                         // Variable containing the event type of the packet saved to this looper track (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
byte loopTrackChannel[maxTracks][maxIndexes];                           // Variable containing the channel number of the packet saved to this looper track (0-15)
byte loopTrackByte3[maxTracks][maxIndexes];                             // Variable containing the note number, control change number, or the pitch bend low byte of the packet saved to this looper track (0-127)
byte loopTrackByte4[maxTracks][maxIndexes];                             // Variable containing the velocity value, control change value, or the pitch bend high byte of the packet saved to this looper track (0-127)
unsigned long loopTrackEventTime[maxTracks][maxIndexes];                // Variable containing the event time of this index position in the looper track relative to the loopStartTime
byte hangingNotes[maxTracks][maxIndexes];                               // Track noteOn vs. noteOff, to make sure we don't have any stragglers (the bane of my existence...)
byte hangingNotesIndex[maxTracks];                                      // Hanging note index counter
byte loopTrackInputChannel[maxTracks];                                  // The current input MIDI channel assigned to this looper track
byte loopTrackOutputChannel[maxTracks];                                 // The output MIDI channel assigned to this looper track (4-15)

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START SETUP SECTION
void setup()
{
    // Enable serial for the MIDI DIN connector
    Serial1.begin(31250);                                                           // MIDI is 31250 baud

    // Set pinModes for the digital button matrix.
    for (int pinNumber = 0; pinNumber < leftDeckColumnCount; pinNumber++)           // For each left deck column pin...
    {
        pinMode(leftDeckColumns[pinNumber], INPUT_PULLUP);                          // set the pinMode to INPUT_PULLUP (+3.3V / HIGH).
    }
    for (int pinNumber = 0; pinNumber < rightDeckColumnCount; pinNumber++)          // For each right deck column pin...
    {
        pinMode(rightDeckColumns[pinNumber], INPUT_PULLUP);                         // set the pinMode to INPUT_PULLUP (+3.3V / HIGH).
    }
        for (int pinNumber = 0; pinNumber < controlDeckColumnCount; pinNumber++)    // For each control deck column pin...
    {
        pinMode(controlDeckColumns[pinNumber], INPUT_PULLUP);                       // set the pinMode to INPUT_PULLUP (+3.3V / HIGH).
    }
    for (int pinNumber = 0; pinNumber < buttonDeckRowCount; pinNumber++)            // For each row pin across all decks...
    {
        pinMode(buttonDeckRows[pinNumber], INPUT);                                  // Set the pinMode to INPUT (0V / LOW).
    }

    // Set pinModes for analog inputs.
    pinMode(ULPotPin, INPUT);                                                       // Upper Left pot
    pinMode(URPotPin, INPUT);                                                       // Upper Right pot
    pinMode(LLPotPin, INPUT);                                                       // Lower Left pot
    pinMode(LRPotPin, INPUT);                                                       // Lower Right pot

    // Set expression level on all channels to 95 (default 127) to leave headroom for the expression pedal 
    for (int midiChannel = 0; midiChannel < 16; midiChannel++)                      // For all MIDI channels (0-15)
    {
        controlChange(midiChannel, 11, 95);                                         // Set expression level (MIDI CC #11)
    }

}
// END SETUP SECTION
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START LOOP SECTION
void loop()
{
    // Print diagnostic troubleshooting information to serial monitor
    // diagnosticTest();

    // In the extremely unlikely event that the MIDI packet index counter ever reaches its maximum, reset it to 0.
    if (midiPacketIndex >= 4294967295)
    {
        midiPacketIndex = 0;        // Index of MIDI packets intercepted by the looper before being sent to the bus
    }

    // Set the current time to a variable for this program loop
    currentTime = millis();

    // Read and store the digital button states of the scanning matrix
    readDigitalButtons();

    // Read, compute averages, and store the analog potentiometer values
    readAnalogKnobs();

    // Set all states and values relating to the control deck's buttons and pots
    runControlModule();

    // Run the main and layer program select function
    runProgramSelect();

    // Run MIDI control change related tasks (expression/modulation/chorus/reverb/sustain)
    runMidiCC();

    // Run the pitch bend function
    runPitchBend();

    // Run the octave select function
    runOctave();

    // Run the drums mode function
    runDrumsMode();

    // Run the legato/drone mode function
    runLegatoMode();

    // Run the metronome function
    runMetronome();

    // Controller Reset (needs to come before any note sending functions)
    runControllerReset();

    // Run the loop pedal function
    runLoopPedal();

    // Send notes to the MIDI bus
    playNotes();

    // Reset input locking variables for next loop where appropriate
    if (loopActivationState == LOW) { previousLoopActivationState   = LOW;  previousLoopResetState = LOW; }

    if (controlUpState      == LOW) { previousControlUpState        = LOW; }
    if (controlMiddleState  == LOW) { previousControlMiddleState    = LOW; }
    if (controlDownState    == LOW) { previousControlDownState      = LOW; }

    if (metaAState          == LOW) { previousMetaAState            = LOW; }
    if (metaBState          == LOW) { previousMetaBState            = LOW; }

    leftDeckActive = LOW;   // Reset the whole deck activation status variables for the next loop (set in readDigitalButtons function)
    rightDeckActive = LOW;  // Reset the whole deck activation status variables for the next loop (set in readDigitalButtons function)

}
// END LOOP SECTION
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START FUNCTIONS SECTION

void diagnosticTest()
{
    for (int buttonNumber = 0; buttonNumber < leftDeckElementCount; buttonNumber++)
    {
        Serial.print(activeButtonsLeft[buttonNumber]);
    }
    Serial.println();
        for (int buttonNumber = 0; buttonNumber < rightDeckElementCount; buttonNumber++)
    {
        Serial.print(activeButtonsRight[buttonNumber]);
    }
    Serial.println();
    for (int buttonNumber = 0; buttonNumber < controlDeckElementCount; buttonNumber++)
    {
        Serial.print(activeButtonsControl[buttonNumber]);
    }
    Serial.println();
    Serial.print(ULPotAverage); Serial.print("\t");
    Serial.print(URPotAverage); Serial.print("\t");
    Serial.print(LLPotAverage); Serial.print("\t");
    Serial.print(LRPotAverage); Serial.println();
}

void readDigitalButtons()
{
    // Left Deck
    for (byte columnIndex = 0; columnIndex < leftDeckColumnCount; columnIndex++)                                // Iterate through each of the column pins.
    {
        byte currentColumn = leftDeckColumns[columnIndex];                                                      // Hold the currently selected column pin in a variable.
        pinMode(currentColumn, OUTPUT);                                                                         // Set that column pin to OUTPUT mode and...
        digitalWrite(currentColumn, LOW);                                                                       // set the pin state to LOW turning it into a temporary ground.
        for (byte rowIndex = 0; rowIndex < buttonDeckRowCount; rowIndex++)                                      // Now iterate through each of the row pins that are connected to the current column pin.
        {                       
            byte currentRow = buttonDeckRows[rowIndex];                                                         // Hold the currently selected row pin in a variable.
            pinMode(currentRow, INPUT_PULLUP);                                                                  // Set that row pin to INPUT_PULLUP mode (+3.3V / HIGH).
            byte buttonNumber = columnIndex + (rowIndex * leftDeckColumnCount);                                 // Assign this location in the matrix a unique number.
            delayMicroseconds(25);                                                                              // Delay to give the pin modes time to change state (false readings are caused otherwise).
            byte buttonState = digitalRead(currentRow);                                                         // Save the currently selected pin state in a variable.
            if (buttonState == LOW && (currentTime - activeButtonsLeftTime[buttonNumber]) > debounceTime)       // If button is active (LOW due to INPUT_PULLUP) and passes debounce
            {
                activeButtonsLeft[buttonNumber] = 1;                                                            // write a 1 to the storage variable
                activeButtonsLeftTime[buttonNumber] = currentTime;                                              // and save the last button press time for debounce comparison.
            }
            if (buttonState == HIGH)
            {
                activeButtonsLeft[buttonNumber] = 0;                                                            // Or if the button is inactive, write a 0.
            }
            pinMode(currentRow, INPUT);                                                                         // Set the selected row pin back to INPUT mode (0V / LOW).
        }
        pinMode(currentColumn, INPUT);                                                                          // Set the selected column pin back to INPUT mode (0V / LOW) and move onto the next column pin.
    }
    // Right Deck
    for (byte columnIndex = 0; columnIndex < rightDeckColumnCount; columnIndex++)                               // Iterate through each of the column pins.
    {
        byte currentColumn = rightDeckColumns[columnIndex];                                                     // Hold the currently selected column pin in a variable.
        pinMode(currentColumn, OUTPUT);                                                                         // Set that column pin to OUTPUT mode and...
        digitalWrite(currentColumn, LOW);                                                                       // set the pin state to LOW turning it into a temporary ground.
        for (byte rowIndex = 0; rowIndex < buttonDeckRowCount; rowIndex++)                                      // Now iterate through each of the row pins that are connected to the current column pin.
        {
            byte currentRow = buttonDeckRows[rowIndex];                                                         // Hold the currently selected row pin in a variable.
            pinMode(currentRow, INPUT_PULLUP);                                                                  // Set that row pin to INPUT_PULLUP mode (+3.3V / HIGH).
            byte buttonNumber = columnIndex + (rowIndex * rightDeckColumnCount);                                // Assign this location in the matrix a unique number.
            delayMicroseconds(25);                                                                              // Delay to give the pin modes time to change state (false readings are caused otherwise).
            byte buttonState = digitalRead(currentRow);                                                         // Save the currently selected pin state in a variable.
            if (buttonState == LOW && (currentTime - activeButtonsRightTime[buttonNumber]) > debounceTime)      // If button is active (LOW due to INPUT_PULLUP) and passes debounce
            {
                activeButtonsRight[buttonNumber] = 1;                                                           // write a 1 to the storage variable
                activeButtonsRightTime[buttonNumber] = currentTime;                                             // and save the last button press time for debounce comparison.
            }
            if (buttonState == HIGH)
            {
                activeButtonsRight[buttonNumber] = 0;                                                           // Or if the button is inactive, write a 0.
            }
            pinMode(currentRow, INPUT);                                                                         // Set the selected row pin back to INPUT mode (0V / LOW).
        }
        pinMode(currentColumn, INPUT);                                                                          // Set the selected column pin back to INPUT mode (0V / LOW) and move onto the next column pin.
    }
    // Control Deck
    for (byte columnIndex = 0; columnIndex < controlDeckColumnCount; columnIndex++)                             // Iterate through each of the column pins.
    {
        byte currentColumn = controlDeckColumns[columnIndex];                                                   // Hold the currently selected column pin in a variable.
        pinMode(currentColumn, OUTPUT);                                                                         // Set that column pin to OUTPUT mode and...
        digitalWrite(currentColumn, LOW);                                                                       // set the pin state to LOW turning it into a temporary ground.
        for (byte rowIndex = 0; rowIndex < controlDeckRowCount; rowIndex++)                                     // Now iterate through each of the row pins that are connected to the current column pin.
        {
            byte currentRow = controlDeckRows[rowIndex];                                                        // Hold the currently selected row pin in a variable.
            pinMode(currentRow, INPUT_PULLUP);                                                                  // Set that row pin to INPUT_PULLUP mode (+3.3V / HIGH).
            byte buttonNumber = columnIndex + (rowIndex * controlDeckColumnCount);                              // Assign this location in the matrix a unique number.
            delayMicroseconds(25);                                                                              // Delay to give the pin modes time to change state (false readings are caused otherwise).
            byte buttonState = digitalRead(currentRow);                                                         // Save the currently selected pin state in a variable.
            if (buttonState == LOW && (currentTime - activeButtonsControlTime[buttonNumber]) > debounceTime)    // If button is active (LOW due to INPUT_PULLUP) and passes debounce
            {
                activeButtonsControl[buttonNumber] = 1;                                                         // write a 1 to the storage variable
                activeButtonsControlTime[buttonNumber] = currentTime;                                           // and save the last button press time for debounce comparison.
            }
            if (buttonState == HIGH)
            {
                activeButtonsControl[buttonNumber] = 0;                                                         // Or if the button is inactive, write a 0.
            }
            pinMode(currentRow, INPUT);                                                                         // Set the selected row pin back to INPUT mode (0V / LOW).
        }
        pinMode(currentColumn, INPUT);                                                                          // Set the selected column pin back to INPUT mode (0V / LOW) and move onto the next column pin.
    }
    // Save the entire deck's activation status for comparison purposes
    for (int buttonNumber = 0; buttonNumber < leftDeckElementCount; buttonNumber++)                             // For all buttons in the left deck
    {
        if (activeButtonsLeft[buttonNumber] == 1)                                                               // Check to see if any key was held
        {
            leftDeckActive = HIGH;                                                                              // If yes, set activation status to HIGH
        }
    }
    for (int buttonNumber = 0; buttonNumber < rightDeckElementCount; buttonNumber++)                            // For all buttons in the left deck
    {
        if (activeButtonsRight[buttonNumber] == 1)                                                              // Check to see if a key was held in the right deck
        {
            rightDeckActive = HIGH;                                                                             // If yes, set activation status to HIGH
        }
    }
}

void readAnalogKnobs()
{
    // Upper Left Potentiometer
    ULPotTotal = ULPotTotal - ULPotReadings[analogIndex];       // Remove the oldest reading from the array
    ULPotReadings[analogIndex] = analogRead(ULPotPin);          // Update this position array with the fresh reading from the sensor
    ULPotTotal = ULPotTotal + ULPotReadings[analogIndex];       // Add the fresh reading to the running total
    ULPotAverage = ULPotTotal / analogReadings;                 // Calculate and store the average
    // Upper Right Potentiometer
    URPotTotal = URPotTotal - URPotReadings[analogIndex];       // Remove the oldest reading from the array
    URPotReadings[analogIndex] = analogRead(URPotPin);          // Update this position array with the fresh reading from the sensor
    URPotTotal = URPotTotal + URPotReadings[analogIndex];       // Add the fresh reading to the running total
    URPotAverage = URPotTotal / analogReadings;                 // Calculate and store the average
    // Lower Left Potentiometer
    LLPotTotal = LLPotTotal - LLPotReadings[analogIndex];       // Remove the oldest reading from the array
    LLPotReadings[analogIndex] = analogRead(LLPotPin);          // Update this position array with the fresh reading from the sensor
    LLPotTotal = LLPotTotal + LLPotReadings[analogIndex];       // Add the fresh reading to the running total
    LLPotAverage = LLPotTotal / analogReadings;                 // Calculate and store the average
    // Lower Right Potentiometer
    LRPotTotal = LRPotTotal - LRPotReadings[analogIndex];       // Remove the oldest reading from the array
    LRPotReadings[analogIndex] = analogRead(LRPotPin);          // Update this position array with the fresh reading from the sensor
    LRPotTotal = LRPotTotal + LRPotReadings[analogIndex];       // Add the fresh reading to the running total
    LRPotAverage = LRPotTotal / analogReadings;                 // Calculate and store the average
    // Increment or reset the index counter
    analogIndex = analogIndex + 1;                              // Increment the index counter
    if (analogIndex >= analogReadings)                          // If the current index number is greater than the desired number of averages
    {
        analogIndex = 0;                                        // Reset the index counter to 0
    }
}

void runControlModule()
{
    // Analog Knobs
    if (ULPotAverage > (previousULPotAverage + analogDeadzone) || ULPotAverage < (previousULPotAverage - analogDeadzone)) // Deadzone to prevent noise from updating constantly
    {
            leftDeckVelocity = map(ULPotAverage, 0, 1023, 0, 127);                      // Map to 7-bit value
            previousULPotAverage = ULPotAverage;                                        // Update "previous" variable for comparison on next loop
    }
    if (URPotAverage > (previousURPotAverage + analogDeadzone) || URPotAverage < (previousURPotAverage - analogDeadzone)) // Deadzone to prevent noise from updating constantly
    {
            rightDeckVelocity = map(URPotAverage, 0, 1023, 0, 127);                     // Map to 7-bit value
            previousURPotAverage = URPotAverage;                                        // Update "previous" variable for comparison on next loop
    }
    if (LLPotAverage > (previousLLPotAverage + analogDeadzone) || LLPotAverage < (previousLLPotAverage - analogDeadzone)) // Deadzone to prevent noise from updating constantly
    {
        multiVelocity = map(LLPotAverage, 0, 1023, 0, 127);                             // Map to 7-bit value
        previousLLPotAverage = LLPotAverage;                                            // Update "previous" variable for comparison on next loop
    }
    if (LRPotAverage > (previousLRPotAverage + analogDeadzone) || LRPotAverage < (previousLRPotAverage - analogDeadzone)) // Deadzone to prevent noise from updating constantly
    {
        metronomeSpeed = map(LRPotAverage, 0, 1023, 195, 1000);                         // Map to metronome click delay in milliseconds (with slight deadzone margin) 200ms = 300bpm, 1000ms = 60bpm
        previousLRPotAverage = LRPotAverage;                                            // Update "previous" variable for comparison on next loop
    }
    // Digital Buttons
    for (int buttonNumber = 0; buttonNumber < controlDeckElementCount; buttonNumber++)
    {
        if (activeButtonsControl[buttonNumber] != previousActiveButtonsControl[buttonNumber])   // Compare current button state to the previous state, and if a difference is found...
        {
            if (activeButtonsControl[buttonNumber] == 1)                                        // If the buttons is active
            {
                if (buttonNumber == 0) { expressionPedalState  = HIGH; }                        // Set the state to HIGH (active)
                if (buttonNumber == 1) { loopPedalState        = HIGH; }
                if (buttonNumber == 2) { controlUpState        = HIGH; }
                if (buttonNumber == 3) { controlMiddleState    = HIGH; }
                if (buttonNumber == 4) { controlDownState      = HIGH; }
                if (buttonNumber == 5 && loopRecordingEnabled == LOW) { metaAState = HIGH; }    // Disallow meta keys when recording is in progress to prevent mid loop program changes
                if (buttonNumber == 6 && loopRecordingEnabled == LOW) { metaBState = HIGH; }
                if (buttonNumber == 7) { loopButtonState       = HIGH; }
                previousActiveButtonsControl[buttonNumber] = 1;                                 // Update the "previous" variable for the next loop
            }
            else                                                                                // If the button is inactive
            {
                if (buttonNumber == 0) { expressionPedalState  = LOW; }                         // Set the state to LOW (inactive)
                if (buttonNumber == 1) { loopPedalState        = LOW; }
                if (buttonNumber == 2) { controlUpState        = LOW; }
                if (buttonNumber == 3) { controlMiddleState    = LOW; }
                if (buttonNumber == 4) { controlDownState      = LOW; }
                if (buttonNumber == 5) { metaAState            = LOW; }
                if (buttonNumber == 6) { metaBState            = LOW; }
                if (buttonNumber == 7) { loopButtonState       = LOW; }
                previousActiveButtonsControl[buttonNumber] = 0;                                 // Update the "previous" variable for the next loop
            }
        }
    }
    // Loop button and pedal both activate the same toggle
    if (loopButtonState == LOW && loopPedalState == LOW)
    {
        loopActivationState = LOW;
    }
    if (loopButtonState || loopPedalState)
    {
        loopActivationState = HIGH;
    }
}

void runProgramSelect()
{
    // Left Deck
    if (metaAState == HIGH && metaBState == LOW)                                        // If the Meta A key is held (main program selection)
    {
        for (int i = 0; i < leftDeckElementCount; i++)                                  // For all buttons in the left deck...
        {
            if (activeButtonsLeft[i] != previousActiveButtonsLeft[i])                   // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsLeft[i] == 1)                                          // If the button is active
                {
                    previousMetaAState = HIGH;                                          // Required for detecting hanging notes when running program selection
                    if (leftDeckDrumsModeEnabled == HIGH)                               // If drums mode is enabled
                    {
                        midiProgram[drumsChannel] = leftDeckMidiProgramsA[i];           // Update the program saved for this channel
                        programChange(drumsChannel, leftDeckMidiProgramsA[i]);          // Change the program for this channel
                        noteOn(drumsChannel, 38, leftDeckVelocity);                     // Always play snare (note #38) when sampling
                    }
                    if (leftDeckDrumsModeEnabled == LOW)                                // If drums mode is disabled
                    {
                        midiProgram[leftDeckMidiChannel] = leftDeckMidiProgramsA[i];    // Update the program saved for this channel
                        programChange(leftDeckMidiChannel, leftDeckMidiProgramsA[i]);   // Change the program for this channel
                        noteOn(leftDeckMidiChannel, 57, leftDeckVelocity);              // Always play A3 (note #57) when sampling
                    }
                    previousActiveButtonsLeft[i] = 1;                                   // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsLeft[i] == 0 && leftDeckActive == LOW)                 // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    if (leftDeckDrumsModeEnabled == HIGH)                               // If drums mode is enabled
                    {
                        noteOff(leftDeckMidiChannel, 38, 0);                            // Snare (note #38)
                        previousActiveButtonsLeft[i] = 0;                               // Update the "previous" variable for comparison on next loop.
                    }
                    if (leftDeckDrumsModeEnabled == LOW)                                // If drums mode is disabled
                    {
                        noteOff(leftDeckMidiChannel, 57, 0);                            // A3 (note #57)
                        previousActiveButtonsLeft[i] = 0;                               // Update the "previous" variable for comparison on next loop.
                    }
                }
            }
        }
    }
    if (metaAState == HIGH && metaBState == HIGH)                                       // If both meta keys are held
    {
        for (int i = 0; i <= 24; i++)                                                   // For all remaining program change values in the left deck (top section is main, bottom selection is layer)
        {
            if (activeButtonsLeft[i] != previousActiveButtonsLeft[i])                   // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsLeft[i] == 1)                                          // If the button is active
                {
                    previousMetaAState = HIGH;                                          // Required for catching hanging notes when running program selection
                    midiProgram[leftDeckMidiChannel] = leftDeckMidiProgramsB[i];        // Update the program saved for this channel
                    programChange(leftDeckMidiChannel, leftDeckMidiProgramsB[i]);       // Change the program for this channel
                    noteOn(leftDeckMidiChannel, 57, leftDeckVelocity);                  // Always play A3 (note #57) when sampling
                    previousActiveButtonsLeft[i] = 1;                                   // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsLeft[i] == 0 && leftDeckActive == LOW)                 // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    noteOff(leftDeckMidiChannel, 57, 0);                                // A3 (note #57)
                    previousActiveButtonsLeft[i] = 0;                                   // Update the "previous" variable for comparison on next loop.
                }
            }
        }
    }
    // Right Deck
    if (metaAState == HIGH && metaBState == LOW)                                        // If the Meta A key is held (main program selection)
    {
        for (int i = 0; i < rightDeckElementCount; i++)                                 // For all buttons in the right deck...
        {
            if (activeButtonsRight[i] != previousActiveButtonsRight[i])                 // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsRight[i] == 1)                                         // If the button is active
                {
                    previousMetaAState = HIGH;                                          // Required for detecting hanging notes when running program selection
                    midiProgram[rightDeckMidiChannel] = rightDeckMidiProgramsA[i];      // Update the program saved for this channel
                    programChange(rightDeckMidiChannel, rightDeckMidiProgramsA[i]);     // Change the program for this channel
                    noteOn(rightDeckMidiChannel, 57, rightDeckVelocity);                // Always play A3 (note #57) when sampling
                    previousActiveButtonsRight[i] = 1;                                  // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsRight[i] == 0 && rightDeckActive == LOW)               // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    noteOff(rightDeckMidiChannel, 57, 0);                               // A3 (note #57)
                    previousActiveButtonsRight[i] = 0;                                  // Update the "previous" variable for comparison on next loop.
                }
            }
        }
    }
    if (metaAState == HIGH && metaBState == HIGH)                                       // If both meta keys are held
    {
        for (int i = 0; i <= 25; i++)                                                   // For all remaining program change values in the right deck (top section is main, bottom selection is layer)
        {
            if (activeButtonsRight[i] != previousActiveButtonsRight[i])                 // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsRight[i] == 1)                                         // If the button is active
                {
                    previousMetaAState = HIGH;                                          // Required for catching hanging notes when running program selection
                    midiProgram[rightDeckMidiChannel] = rightDeckMidiProgramsB[i];      // Update the program saved for this channel
                    programChange(rightDeckMidiChannel, rightDeckMidiProgramsB[i]);     // Change the program for this channel
                    noteOn(rightDeckMidiChannel, 57, rightDeckVelocity);                // Always play A3 (note #57) when sampling
                    previousActiveButtonsRight[i] = 1;                                  // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsRight[i] == 0 && rightDeckActive == LOW)               // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    noteOff(rightDeckMidiChannel, 57, 0);                               // A3 (note #57)
                    previousActiveButtonsRight[i] = 0;                                  // Update the "previous" variable for comparison on next loop.
                }
            }
        }
    }
   // Left Deck Layer
    if (metaAState == LOW && metaBState == HIGH)                                        // If Meta B key is held (layer program selection)
    {
        for (int i = 0; i < leftDeckElementCount; i++)                                  // For all buttons in the left deck...
        {
            if (activeButtonsLeft[i] != previousActiveButtonsLeft[i])                   // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsLeft[i] == 1)                                          // If the button is active
                {
                    previousMetaBState = HIGH;                                          // Required for catching hanging notes when running program selection
                    leftDeckLayerNotesEnabled = HIGH;                                   // Enable the layer functionality in the playNotes functions
                    midiProgram[leftDeckLayerMidiChannel] = leftDeckMidiProgramsA[i];   // Update the program saved for this channel
                    programChange(leftDeckLayerMidiChannel, leftDeckMidiProgramsA[i]);  // Change the program for this channel
                    noteOn(leftDeckLayerMidiChannel, 57, multiVelocity);                // Always play A3 (note #57) when sampling
                    previousActiveButtonsLeft[i] = 1;                                   // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsLeft[i] == 0 && leftDeckActive == LOW)                 // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    noteOff(leftDeckLayerMidiChannel, 57, 0);                           // A3 (note #57)
                    previousActiveButtonsLeft[i] = 0;                                   // Update the "previous" variable for comparison on next loop.
                }
            }
        }
    }
    if (metaAState == HIGH && metaBState == HIGH)                                       // If both meta keys are held
    {
        for (int i = 25; i <= 79; i++)                                                  // Disable layer if a non program assigned button is pressed among the remaining buttons
        {
            if (activeButtonsLeft[i] != previousActiveButtonsLeft[i])                   // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsLeft[i] == 1)                                          // If the button is active
                {
                    leftDeckLayerNotesEnabled = LOW;                                    // Disable the layer channel (functionality is in the playNotes function)
                }
            }
        }
        for (int i = 80; i <= 104; i++)                                                 // For all remaining program change values in the left deck layer, bottom section...
        {
            if (activeButtonsLeft[i] != previousActiveButtonsLeft[i])                   // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsLeft[i] == 1)                                          // If the button is active
                {
                    previousMetaAState = HIGH;                                          // Required for catching hanging notes when running program selection
                    leftDeckLayerNotesEnabled = HIGH;                                   // Enable the layer functionality in the playNotes function
                    midiProgram[leftDeckLayerMidiChannel] = leftDeckMidiProgramsB[i];   // Update the program saved for this channel
                    programChange(leftDeckLayerMidiChannel, leftDeckMidiProgramsB[i]);  // Change the program for this channel
                    noteOn(leftDeckLayerMidiChannel, 57, multiVelocity);                // Always play A3 (note #57) when sampling
                    previousActiveButtonsLeft[i] = 1;                                   // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsLeft[i] == 0 && leftDeckActive == LOW)                 // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    noteOff(leftDeckLayerMidiChannel, 57, 0);                           // A3 (note #57)
                    previousActiveButtonsLeft[i] = 0;                                   // Update the "previous" variable for comparison on next loop.
                }
            }
        }
    }

   // Right Deck Layer
    if (metaAState == LOW && metaBState == HIGH)                                        // If Meta B key is held (layer program selection)
    {
        for (int i = 0; i < rightDeckElementCount; i++)                                 // For all buttons in the right deck...
        {
            if (activeButtonsRight[i] != previousActiveButtonsRight[i])                 // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsRight[i] == 1)                                         // If the button is active
                {
                    previousMetaBState = HIGH;                                          // Required for catching hanging notes when running program selection
                    rightDeckLayerNotesEnabled = HIGH;                                  // Enable the layer functionality in the playNotes functions
                    midiProgram[rightDeckLayerMidiChannel] = rightDeckMidiProgramsA[i]; // Update the program saved for this channel
                    programChange(rightDeckLayerMidiChannel, rightDeckMidiProgramsA[i]);// Change the program for this channel
                    noteOn(rightDeckLayerMidiChannel, 57, multiVelocity);               // Always play A3 (note #57) when sampling
                    previousActiveButtonsRight[i] = 1;                                  // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsRight[i] == 0 && rightDeckActive == LOW)               // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    noteOff(rightDeckLayerMidiChannel, 57, 0);                          // A3 (note #57)
                    previousActiveButtonsRight[i] = 0;                                  // Update the "previous" variable for comparison on next loop.
                }
            }
        }
    }
    if (metaAState == HIGH && metaBState == HIGH)                                       // If both meta keys are held
    {
        for (int i = 26; i <= 80; i++)                                                  // Disable layer if a non program assigned button is pressed among the remaining buttons
        {
            if (activeButtonsRight[i] != previousActiveButtonsRight[i])                 // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsRight[i] == 1)                                         // If the button is active
                {
                    rightDeckLayerNotesEnabled = LOW;                                   // Disable the layer channel (functionality is in the playNotes function)
                }
            }
        }
        for (int i = 81; i <= 105; i++)                                                 // For all remaining program change values in the right deck layer, bottom section...
        {
            if (activeButtonsRight[i] != previousActiveButtonsRight[i])                 // Compare current button state to the previous state, and if a difference is found...
            {
                if (activeButtonsRight[i] == 1)                                         // If the button is active
                {
                    previousMetaAState = HIGH;                                          // Required for catching hanging notes when running program selection
                    rightDeckLayerNotesEnabled = HIGH;                                  // Enable the layer functionality in the playNotes function
                    midiProgram[rightDeckLayerMidiChannel] = rightDeckMidiProgramsB[i]; // Update the program saved for this channel
                    programChange(rightDeckLayerMidiChannel, rightDeckMidiProgramsB[i]);// Change the program for this channel
                    noteOn(rightDeckLayerMidiChannel, 57, multiVelocity);               // Always play A3 (note #57) when sampling
                    previousActiveButtonsRight[i] = 1;                                  // Update the "previous" variable for comparison on next loop.
                }
                if (activeButtonsRight[i] == 0 && rightDeckActive == LOW)               // Only send a noteOff if no keys are held.  The preview only plays one note (A3 - #57), and it would be quickly squelched otherwise
                {
                    noteOff(rightDeckLayerMidiChannel, 57, 0);                          // A3 (note #57)
                    previousActiveButtonsRight[i] = 0;                                  // Update the "previous" variable for comparison on next loop.
                }
            }
        }
    }
    // Ensure that notes are not left hanging (i.e. no closing noteOff) if a meta key is released before a program selection note button
    if (metaAState == LOW && previousMetaAState == HIGH && leftDeckActive == HIGH)      // If a key was held
    {
        noteOff(leftDeckMidiChannel, 57, 0);                                            // Send a noteOff for the program preview note A3 (note #57)
        if (leftDeckLayerNotesEnabled == HIGH)
        {
            noteOff(leftDeckLayerMidiChannel, 57, 0);                                   // Send a noteOff for the program preview note A3 (note #57)
        }
        for (int i = 0; i < leftDeckElementCount; i++)
        {
            activeButtonsLeft[i] = 0;                                                   // Clear the note arrays to pop any active keys for this loop
            previousActiveButtonsLeft[i] = 0;                                           // Also clear the "previous" variable to prevent any erroneous noteOffs
        }
    }
    if (metaAState == LOW && previousMetaAState == HIGH && rightDeckActive == HIGH)     // If a key was held
    {
        noteOff(rightDeckMidiChannel, 57, 0);                                           // Send a noteOff for the program preview note A3 (note #57)
        if (rightDeckLayerNotesEnabled == HIGH)
        {
            noteOff(rightDeckLayerMidiChannel, 57, 0);                                  // Send a noteOff for the program preview note A3 (note #57)
        }
        for (int i = 0; i < rightDeckElementCount; i++)
        {
            activeButtonsRight[i] = 0;                                                  // Clear the note arrays to pop any active keys for this loop
            previousActiveButtonsRight[i] = 0;                                          // Also clear the "previous" variable to prevent any erroneous noteOffs
        }
    }
    if (metaBState == LOW && previousMetaBState == HIGH && leftDeckActive == HIGH)      // If a key was held
    {
        noteOff(leftDeckMidiChannel, 57, 0);                                            // Send a noteOff for the program preview note A3 (note #57)
        if (leftDeckLayerNotesEnabled == HIGH)
        {
            noteOff(leftDeckLayerMidiChannel, 57, 0);                                   // Send a noteOff for the program preview note A3 (note #57)
        }
        for (int i = 0; i < leftDeckElementCount; i++)
        {
            activeButtonsLeft[i] = 0;                                                   // Clear the note arrays to pop any active keys for this loop
            previousActiveButtonsLeft[i] = 0;                                           // Also clear the "previous" variable to prevent any erroneous noteOffs
        }
    }
    if (metaBState == LOW && previousMetaBState == HIGH && rightDeckActive == HIGH)     // If a key was held
    {
        noteOff(rightDeckMidiChannel, 57, 0);                                           // Send a noteOff for the program preview note A3 (note #57)
        if (rightDeckLayerNotesEnabled == HIGH)
        {
            noteOff(rightDeckLayerMidiChannel, 57, 0);                                  // Send a noteOff for the program preview note A3 (note #57)
        }
        for (int i = 0; i < rightDeckElementCount; i++)
        {
            activeButtonsRight[i] = 0;                                                  // Clear the note arrays to pop any active keys for this loop
            previousActiveButtonsRight[i] = 0;                                          // Also clear the "previous" variable to prevent any erroneous noteOffs
        }
    }
}

void runMidiCC()
{
    // Expression
    if (expressionPedalState == HIGH && previousExpressionPedalState == LOW)                        // If the expression pedal is pressed
    {
        previousExpressionPedalState = HIGH;                                                        // Lock input
        for (int midiChannel = 0; midiChannel < 4; midiChannel++)                                   // For MIDI channels 0-3 (left, right, left layer, right layer)
        {
            controlChange(midiChannel, 11, 127);                                                    // Enable expression full (127) (MIDI CC#11) - 7 bit value
        }
    }
    if (expressionPedalState == LOW && previousExpressionPedalState == HIGH)                        // If the expression pedal is released
    {
        previousExpressionPedalState = LOW;                                                         // Lock input
        for (int midiChannel = 0; midiChannel < 4; midiChannel++)                                   // For MIDI channels 0-3 (left, right, left layer, right layer)
        {
            controlChange(midiChannel, 11, 95);                                                     // Set expression level to default (95) (MIDI CC #11)
        }
    }
    // Modulation
    if (metaAState == LOW && metaBState == LOW)                                                     // If no meta keys are held
    {
        if (controlMiddleState == HIGH && previousControlMiddleState == LOW)                        // and the middle button is pressed
        {
            previousControlMiddleState = HIGH;                                                      // Lock input
            // MIDI channels 2-3 only (right main, right layer)
            controlChange(rightDeckMidiChannel, 1, 127);                                            // Enable modulation full (MIDI CC #1) - 7 bit value
            controlChange(rightDeckLayerMidiChannel, 1, 127);                                       // Enable modulation full (MIDI CC #1) - 7 bit value
        }
        if (controlMiddleState == LOW && previousControlMiddleState == HIGH)                        // if the middle button is released
        {
            previousControlMiddleState = HIGH;                                                      // Lock input
            // MIDI channels 2-3 only (right main, right layer)
            controlChange(rightDeckMidiChannel, 1, 0);                                              // Disable modulation (MIDI CC #1) - 7 bit value
            controlChange(rightDeckLayerMidiChannel, 1, 0);                                         // Disable modulation (MIDI CC #1) - 7 bit value
        }

    }
    // Chorus
    if (metaAState == HIGH && metaBState == LOW)                                                    // If Meta A key is held and the middle button is pressed
    {
        if (controlMiddleState == HIGH && previousControlMiddleState == LOW && chorusState == LOW)  // If chorus is disabled
        {
            previousControlMiddleState = HIGH;                                                      // Lock input
            for (int midiChannel = 0; midiChannel < 4; midiChannel++)                               // For MIDI channels 0-3 (left, right, left layer, right layer)
            {
                controlChange(midiChannel, 93, 127);                                                // Enable chorus full (MIDI CC #93) - 7 bit value
            }
            chorusState = HIGH;                                                                     // Set chorus state to enabled
        }
        if (controlMiddleState == HIGH && previousControlMiddleState == LOW && chorusState == HIGH) // If chorus is enabled
        {
            previousControlMiddleState = HIGH;                                                      // Lock input
            for (int midiChannel = 0; midiChannel < 4; midiChannel++)                               // For MIDI channels 0-3 (left, right, left layer, right layer)
            {
                controlChange(midiChannel, 93, 0);                                                  // Disable chorus (MIDI CC #93) - 7 bit value
            }
            chorusState = LOW;                                                                      // Set chorus state to disabled
        }
    }
    // Reverb
    if (metaAState == LOW && metaBState == HIGH)                                                    // If Meta B key is held and the middle button is pressed
    {
        if (controlMiddleState == HIGH && previousControlMiddleState == LOW && reverbState == LOW)  // If reverb is disabled
        {
            previousControlMiddleState = HIGH;                                                      // Lock input
            for (int midiChannel = 0; midiChannel < 4; midiChannel++)                               // For MIDI channels 0-3 (left, right, left layer, right layer)
            {
                controlChange(midiChannel, 91, 127);                                                // Enable Reverb (MIDI CC #91) - 7 bit value
            }
            reverbState = HIGH;                                                                     // Set reverb to enabled
        }
        if (controlMiddleState == HIGH && previousControlMiddleState == LOW && reverbState == HIGH) // If reverb is enabled
        {
            previousControlMiddleState = HIGH;                                                      // Lock input
            for (int midiChannel = 0; midiChannel < 4; midiChannel++)                               // For MIDI channels 0-3 (left, right, left layer, right layer)
            {
                controlChange(midiChannel, 91, 0);                                                  // Disable Reverb (MIDI CC #91) - 7 bit value
            }
            reverbState = LOW;                                                                      // Set reverb to disabled
        }
    }
}

void runPitchBend()
{
    // Pitch Bend
    if (metaAState == LOW && metaBState == LOW)                                                     // If no meta keys are held, and the up or down buttons are pressed
    {
        if (controlUpState == HIGH && pitchOffset < pitchStep)                                      // Stay below max values of 16363 (whole step) or 12267 (half step)
        {
            previousControlUpState = HIGH;                                                          // Lock input
            pitchOffset = pitchOffset + pitchSpeed;                                                 // Increment the pitchOffset by the amount of pitchSpeed for this program loop
            // MIDI channels 2-3 only (right main, right layer)
            pitchBendChange(rightDeckMidiChannel, ((referencePitch - 1) + constrain(pitchOffset, 0, pitchStep)) & 0x7F, ((referencePitch - 1) + constrain(pitchOffset, 0, pitchStep)) >> 7);        // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
            pitchBendChange(rightDeckLayerMidiChannel, ((referencePitch - 1) + constrain(pitchOffset, 0, pitchStep)) & 0x7F, ((referencePitch - 1) + constrain(pitchOffset, 0, pitchStep)) >> 7);   // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
        }
        if (controlUpState == LOW && previousControlUpState == HIGH)                                // If the button is released
        {
            pitchOffset = 0;                                                                        // Reset pitchOffset to default
            // MIDI channels 2-3 only (right main, right layer)
            pitchBendChange(rightDeckMidiChannel, referencePitch & 0x7F, referencePitch >> 7);      // Reset pitch bend to reference level (8192)
            pitchBendChange(rightDeckLayerMidiChannel, referencePitch & 0x7F, referencePitch >> 7); // Reset pitch bend to reference level (8192)
        }
        if (controlDownState == HIGH && pitchOffset < pitchStep)                                    // Stay below max values of 16363 (whole step) or 12267 (half step)
        {
            previousControlDownState = HIGH;                                                        // Lock input
            pitchOffset = pitchOffset + pitchSpeed;                                                 // Increment the pitchOffset by the amount of pitchSpeed for this program loop
            // MIDI channels 2-3 only (right main, right layer)
            pitchBendChange(rightDeckMidiChannel, (referencePitch - constrain(pitchOffset, 0, pitchStep)) & 0x7F, (referencePitch - constrain(pitchOffset, 0, pitchStep)) >> 7);        // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
            pitchBendChange(rightDeckLayerMidiChannel, (referencePitch - constrain(pitchOffset, 0, pitchStep)) & 0x7F, (referencePitch - constrain(pitchOffset, 0, pitchStep)) >> 7);   // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
        }
        if (controlDownState == LOW && previousControlDownState == HIGH)                            // If the button is released
        {
            pitchOffset = 0;                                                                        // Reset pitchOffset to default
            // MIDI channels 2-3 only (right main, right layer)
            pitchBendChange(rightDeckMidiChannel, referencePitch & 0x7F, referencePitch >> 7);      // Reset pitch bend to reference level (8192)
            pitchBendChange(rightDeckLayerMidiChannel, referencePitch & 0x7F, referencePitch >> 7); // Reset pitch bend to reference level (8192)
        }
    }
    if (metaAState == HIGH && metaBState == HIGH)                                                   // If both meta keys are held
    {
        // Half-step/Whole-Step Pitch Bend Toggle
        if (controlUpState == HIGH && previousControlUpState == LOW && pitchStep == 8191)           // If control up is pressed and the value is 8191 "whole-step" (default)
        {
            previousControlUpState = HIGH;                                                          // Lock input
            pitchStep = 4095;                                                                       // Set the value to half step
        }
        if (controlUpState == HIGH && previousControlUpState == LOW && pitchStep == 4095)           // If control up is pressed and the value is 4095 "half-step"
        {
            previousControlUpState = HIGH;                                                          // Lock input
            pitchStep = 8191;                                                                       // Set the value to whole step
            // referencePitch = 8192;                                                               // Needed for whole instrument tuning adjustment to keep within bounds (currently not implemented to cut down on complexity)
        } 
        // Speed Adjustment
        if (controlDownState == HIGH && previousControlDownState == LOW && pitchSpeed == 500)       // If pitch bend speed is 500 "slow" (default)
        {
            previousControlDownState = HIGH;                                                        // Lock input
            pitchSpeed = 1500;                                                                      // Set pitch bend speed to fast
        }
        if (controlDownState == HIGH && previousControlDownState == LOW && pitchSpeed == 1500)      // If pitch bend speed is 1500 "fast"
        {
            previousControlDownState = HIGH;                                                        // Lock input
            pitchSpeed = 500;                                                                       // Set pitch bend speed to slow
        }
    }
}

void runOctave()
{
    // Normal note buttons
    if (metaAState == HIGH && metaBState == LOW)                                                    // If Meta A key is held (main decks only)
    {
        if (controlUpState == HIGH && previousControlUpState == LOW && octave < 12)                 // Keep octave in bounds (highest and lowest keys overflow 7 bit range (0-127) otherwise)
        {
            octave = octave + 12;                                                                   // Increment octave modifier
            previousControlUpState = HIGH;                                                          // Lock input
        }
        if (controlDownState == HIGH && previousControlDownState == LOW && octave > -11)            // Keep octave in bounds (highest and lowest keys overflow 7 bit range (0-127) otherwise)
        {
            octave = octave - 12;                                                                   // Decrement octave modifier
            previousControlDownState = HIGH;                                                        // Lock input
        }
    }
}

void runDrumsMode()
{
    if (metaAState == LOW && metaBState == HIGH)                                                            // If Meta B key is held and control up is pressed
    {
        if (controlUpState == HIGH && previousControlUpState == LOW && leftDeckDrumsModeEnabled == LOW)     // If drums mode is currently disabled
        {
            previousControlUpState = HIGH;                                                                  // Lock input state
            leftDeckLayerNotesEnabled = LOW;                                                                // Make sure that the layer mode is disabled
            leftDeckDrumsModeEnabled = HIGH;                                                                // Enable drums mode
            leftDeckMidiChannel = drumsChannel;                                                             // Set the left deck channel to drums (MIDI channel 9)
            programChange(drumsChannel, midiProgram[drumsChannel]);                                         // Change the program to what was saved for this channel
        }
        if (controlUpState == HIGH && previousControlUpState == LOW && leftDeckDrumsModeEnabled == HIGH)    // If drums mode is currently enabled
        {
            previousControlUpState = HIGH;                                                                  // Lock input state
            leftDeckDrumsModeEnabled = LOW;                                                                 // Disable drums mode
            leftDeckMidiChannel = leftDeckDefaultMidiChannel;                                               // Set the left deck channel back to default (MIDI channel 0)
            programChange(leftDeckMidiChannel, midiProgram[leftDeckMidiChannel]);                           // Change the program back to what was saved for this channel
        }
    }
}

void runLegatoMode()
{
    if (metaAState == LOW && metaBState == HIGH)                                                                // If the Meta B key is held and control down is pressed
    {
        if (controlDownState == HIGH && previousControlDownState == LOW && leftDeckLegatoModeEnabled == LOW)    // If legato mode is currently disabled
        {
            previousControlDownState = HIGH;                                                                    // Lock input state
            leftDeckLegatoModeEnabled = HIGH;                                                                   // Enable legato mode (functionality is in the playNotes function)
        }
        if (controlDownState == HIGH && previousControlDownState == LOW && leftDeckLegatoModeEnabled == HIGH)   // If legato mode is currently enabled
        {
            previousControlDownState = HIGH;                                                                    // Lock input state
            leftDeckLegatoModeEnabled = LOW;                                                                    // Disable legato mode
            controlChange(leftDeckMidiChannel, 64, 0);                                                          // Send a sustain off command (MIDI CC #64)  - 0-63 OFF / 64-127 ON
            controlChange(leftDeckLayerMidiChannel, 64, 0);                                                     // Send a sustain off command (MIDI CC #64)  - 0-63 OFF / 64-127 ON
        }
    }
}

void runMetronome()
{
    if ((currentTime - previousMetronomeClickTime) > metronomeSpeed && metronomeSpeed >= 200)                   // Allow for small deadzone when parked at 0 (full range 195-1000)
    {                                                                                                           // 200ms = 300bpm - 1000ms = 60bpm
        noteOn(drumsChannel, 37, multiVelocity);                                                                // Note #37 = Rim Shot
        noteOff(drumsChannel, 37, 0);                                                                           // Send closing noteOff
        previousMetronomeClickTime = currentTime;                                                               // Update "previous" variable for comparison on next loop
    }
}

void runControllerReset()
{
    if (metaAState == HIGH && metaBState == HIGH && controlMiddleState == HIGH && previousControlMiddleState == LOW)    // If both meta keys and control middle are pressed
    {
        previousControlMiddleState = HIGH;                                              // Lock input
        memset(activeButtonsLeft, 0, sizeof(activeButtonsLeft));                        // Pop any active note buttons
        memset(previousActiveButtonsLeft, 0, sizeof(previousActiveButtonsLeft));        // Pop any active note buttons
        memset(activeButtonsRight, 0, sizeof(activeButtonsRight));                      // Pop any active note buttons
        memset(previousActiveButtonsRight, 0, sizeof(previousActiveButtonsRight));      // Pop any active note buttons
        for (int midiChannel = 0; midiChannel <= 15; midiChannel++)                     // For MIDI channels 0 through 15
        {
            controlChange(midiChannel, 120, 0);                                         // All Sound Off (MIDI CC #120) - Mute all currently sounding notes
            programChange(midiChannel, 0);                                              // Reset program on all channels to 0
            controlChange(midiChannel, 91, 0);                                          // Disable reverb on all channels (MIDI CC #91)
            controlChange(midiChannel, 93, 0);                                          // Disable chorus on all channels (MIDI CC #93)
            controlChange(midiChannel, 64, 0);                                          // Disable sustain on all channels (MIDI CC #64)
            controlChange(midiChannel, 11, 95);                                         // Set default expression level (MIDI CC #11)
            midiProgram[midiChannel] = 0;                                               // Reset saved program on all channels to 0
        }
        leftDeckMidiChannel = 0;                                                        // Reset left deck to default channel 0
        octave = 0;                                                                     // Reset octave modifier to 0
        chorusState = LOW;                                                              // Disable chorus function
        reverbState = LOW;                                                              // Disable reverb function
        pitchOffset = 0;                                                                // Reset pitch offset to 0
        pitchSpeed = 500;                                                               // Reset pitch speed modifier to default 500
        pitchStep = 8191;                                                               // Reset pitch step modifier to default 8191 (whole-step bend)
        leftDeckLayerNotesEnabled   = LOW;                                              // Disable left deck layer mode
        rightDeckLayerNotesEnabled  = LOW;                                              // Disable right deck layer mode
        leftDeckDrumsModeEnabled    = LOW;                                              // Disable left deck drums mode
        leftDeckLegatoModeEnabled   = LOW;                                              // Disable left deck legato/drone mode
        loopDrumsInMemory           = LOW;                                              // Clear drums in memory flag
        loopResetClock = currentTime;                                                   // Trigger a looper reset (will occur in the looper function)
        delay(250);                                                                     // Delay to give the process some weight
        noteOn(0, 60, 95);                                                              // Send a notification jingle to alert the user that the reset process is complete
        delay(100);
        noteOn(0, 64, 95);
        delay(100);
        noteOn(0, 67, 95);
        delay(100);
        noteOn(0, 72, 95);
        delay(250);
        noteOff(0, 60, 0);
        noteOff(0, 64, 0);
        noteOff(0, 67, 0);
        noteOff(0, 72, 0);
    }
}

void runLoopPedal()
{
    // The maximum number of indexes for a track has been reached.  Force a reset to prevent an overflow.
    for (int trackNumber = 0; trackNumber < maxTracks; trackNumber++)
    {
        if (loopTrackHighestIndex[trackNumber] >= maxIndexes)
        {
            loopResetClock = currentTime;                                               // Set the loopResetClock to currentTime to trigger a reset
            loopInMemory = HIGH;                                                        // Prevent a regular controller reset from triggering loop reset unnecessarily
            noteOn(9, 71, 95);                                                          // Play alert sound (MIDI Note #71 - Short Whistle)
            noteOff(9, 81, 0);
            delay(50);
            noteOn(9, 71, 95);
            noteOff(9, 81, 0);
            delay(50);
            noteOn(9, 71, 95);
            noteOff(9, 81, 0);
            delay(100);
        }
    }

    // A button reset, or overflow reset has been detected
    if (loopResetClock <= currentTime && loopInMemory == HIGH)                                              // loopInMemory check to prevent a controler reset from triggering this unnecessarily
    {

            // If looper was interrupted mid-playback, clean up any potential unclosed noteOn/noteOff events
            for (int myTrack = 0; myTrack < maxTracks; myTrack++)                                           // For all tracks allocated to the looper
            {
                if (loopTrackActive[myTrack] == HIGH)                                                       // If the channel is marked as active (HIGH)
                {
                    for (int myIndex = 0 ; myIndex < hangingNotesIndex[myTrack]; myIndex++)                 // For each noteOn played thus far in the loop
                    {
                        if (hangingNotes[myTrack][myIndex] != 254)                                          // If note value isn't 254 (meaning it's an actual note)
                        {
                            noteOff(loopTrackOutputChannel[myTrack], hangingNotes[myTrack][myIndex], 0);    // Send a matching noteOff for every noteOn played thus far to make sure everything was closed properly
                        }
                    }
                }
                hangingNotesIndex[myTrack] = 0;                                                             // Reset the hanging notes index counter
            }

        midiPacketIndex = 0;                                                            // A running counter of all MIDI packets sent to the bus
        previousMidiPacketIndex = 0;                                                    // The previous packet index value for comparison
        loopRecordingEnabled = LOW;                                                     // The recording state of the looper on/off
        loopPlaybackEnabled = LOW;                                                      // The playback state of the looper on/off
        loopInMemory = LOW;                                                             // Flag indicating if a loop is currently saved in memory or not
        loopDrumsInMemory = LOW;                                                        // Flag indicating if a drums track is currently saved in memory or not
        unsigned long loopStartTime = 0;                                                // The global loop start time.  This number is subtracted from subsequent saved event times to record relative event times in the loop.  Resets to 0 on every playback loop.
        unsigned long loopDuration = 0;                                                 // The global loop duration.  When currentTime is equal to loopStartTime plus this number, the loopStartTime and indexes are reset to 0 for another playthrough.
        unsigned long loopResetClock = 4294967295;                                      // When the loop button is held, this value is set to currentTime + 1500.  When currentTime equals this number, the looper is reset. Default is max value of an unsigned long (49.7 days of milliseconds).
        currentLoopIteration = LOW;                                                     // Variable to store the currently recording loop iteration.  When this is incremented, a new loop is initialized.
        previousLoopIteration = LOW;                                                    // Previous loop iteration for comparison
        memset(loopTrackActive, 0, sizeof(loopTrackActive));                            // Flag indicating if this loop channel is in use
        memset(loopTrackIndex, 0, sizeof(loopTrackIndex));                              // The current index position of the arrays assigned to this loop track
        memset(loopTrackHighestIndex, 0, sizeof(loopTrackHighestIndex));                // The highest track index reached thus far in the current loop track.  Needed for overflow detection, and for limiting unnecessary array searches when scanning a full array.
        memset(loopTrackEventType, 0, sizeof(loopTrackEventType));                      // Variable containing the event type of the packet saved to this looper track (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
        memset(loopTrackChannel, 0, sizeof(loopTrackChannel));                          // Variable containing the channel number of the packet saved to this looper track (0-15)
        memset(loopTrackByte3, 0, sizeof(loopTrackByte3));                              // Variable containing the note number, control change number, or the pitch bend low byte of the packet saved to this looper track (0-127)
        memset(loopTrackByte4, 0, sizeof(loopTrackByte4));                              // Variable containing the velocity value, control change value, or the pitch bend high byte of the packet saved to this looper track (0-127)
        memset(loopTrackEventTime, 0, sizeof(loopTrackEventTime));                      // Variable containing the event time of this index position in the looper track relative to the loopStartTime
        memset(loopTrackInputChannel, 0, sizeof(loopTrackInputChannel));                // The current input MIDI channel assigned to this looper track
        memset(loopTrackOutputChannel, 0, sizeof(loopTrackOutputChannel));              // The output MIDI channel assigned to this looper track (4-15)
        memset(activeButtonsLeft, 0, sizeof(activeButtonsLeft));                        // Pop any active note buttons
        memset(previousActiveButtonsLeft, 0, sizeof(previousActiveButtonsLeft));        // Pop any active note buttons
        memset(activeButtonsRight, 0, sizeof(activeButtonsRight));                      // Pop any active note buttons
        memset(previousActiveButtonsRight, 0, sizeof(previousActiveButtonsRight));      // Pop any active note buttons
        noteOn(9, 65, 95);                                                              // Play an alert sound (GM Percussion Note #65 High Timbale)
        noteOff(9, 65, 0);
        delay(75);
        noteOn(9, 66, 95);                                                              // Play an alert sound (GM Percussion Note #66 Low Timbale)
        noteOff(9, 66, 0);
    }

    // The looper button is pressed and there is no loop in memory.  Enable recording mode and wait for note input.
    if (loopActivationState == HIGH && previousLoopActivationState == LOW && previousLoopResetState == LOW && loopRecordingEnabled == LOW && loopInMemory == LOW) // previousLoopResetState prevents looper from immediately activating again after a reset since the key is still being held
    {
        previousLoopActivationState = HIGH;                                                                 // Lock the input button
        currentLoopIteration = 0;                                                                           // Initialize the loop iteration counter
        previousLoopIteration = 0;                                                                          // Initialize the previous loop iteration counter
        midiPacketIndex = 0;                                                                                // Initialize the MIDI packet index counter
        previousMidiPacketIndex = 0;                                                                        // Initialize the MIDI packet previous index counter
        for (int myTrack = 0; myTrack < maxTracks; myTrack++)                                               // For all tracks in the looper
        {
            loopTrackActive[myTrack] = LOW;                                                                 // Mark the tracks open/unused
            loopTrackInputChannel[myTrack] = 20;                                                            // Set the input channel to a value outside the valid range
        }
        noteOn(9, 66, 95);                                                                                  // Play an alert sound (GM Percussion Note #66 Low Timbale)
        noteOff(9, 66, 0);
        delay(75);
        noteOn(9, 65, 95);                                                                                  // Play an alert sound (GM Percussion Note #65 High Timbale)
        noteOff(9, 65, 0);
        loopRecordingEnabled = HIGH;                                                                        // Set loop recording to enabled
    }

    // The looper button has been pressed again before any note buttons were input.
    if (loopActivationState == HIGH && previousLoopActivationState == LOW && loopRecordingEnabled == HIGH && loopInMemory == LOW)
    {
        previousLoopActivationState = HIGH;                                                                 // Lock the input button
        loopRecordingEnabled = LOW;                                                                         // Set loop recording to disabled
        noteOn(9, 65, 95);                                                                                  // Play an alert sound (GM Percussion Note #65 High Timbale)
        noteOff(9, 65, 0);
        delay(75);
        noteOn(9, 66, 95);                                                                                  // Play an alert sound (GM Percussion Note #66 Low Timbale)
        noteOff(9, 66, 0);
    }

    // The first note button has been input, and recording mode is active.
    if (loopRecordingEnabled == HIGH && loopInMemory == LOW && (leftDeckActive == HIGH || rightDeckActive == HIGH))
    {
        loopStartTime = currentTime;                                                                        // Set loop start time to the current time
        loopInMemory = HIGH;                                                                                // Flag that there is a loop in memory
        currentLoopIteration = currentLoopIteration + 1;                                                    // Increment the the loop iteration counter
    }

    // A new loop iteration is detected.
    if (loopInMemory == HIGH && currentLoopIteration > previousLoopIteration)
    {
        // Loop track overflow detection
        byte loopTrackCounter = 0;                                                                          // Keep track of how many loop tracks are active so we can stop recording before an overflow
        for (int myTrack = 0; myTrack < maxTracks; myTrack++)                                               // For all tracks allocated to the looper
        {
            if (loopTrackActive[myTrack] == HIGH)                                                           // If the loop track is active
            {
                loopTrackCounter = loopTrackCounter + 1;                                                    // Increment the counter
            }
        }
        if (loopTrackCounter > maxTracks - 4)                                                               // If there isn't enough room for 4 potential new tracks (left, right, left layer, right layer)
        {
            loopRecordingEnabled = LOW;                                                                     // Disable recording
            goto looperFull;                                                                                // Skip to the end of this section without initializing more tracks
        }

        // Initialize new loop iteration
        noteOn(9, 75, 95);                                                                                  // Play quick alert sound on new loop start (GM Percussion Note #75 Claves)
        noteOff(9, 75, 0);
        if (leftDeckDrumsModeEnabled == HIGH && loopTrackActive[11] == LOW)                                 // If drums mode is enabled, and the track isn't already in use
        {
            loopTrackActive[11] = HIGH;                                                                     // Mark the current track as in use
            loopTrackInputChannel[11] = 9;                                                                  // Take input from the drums channel 9
            loopTrackOutputChannel[11] = loopChannelAssignment[11];                                         // Assign this loop track to the appropriate output channel
            loopTrackIndex[11] = 0;                                                                         // Set the track index to 0
            loopTrackHighestIndex[11] = 0;                                                                  // Set the highest track index to 0
            programChange(loopTrackOutputChannel[11], midiProgram[leftDeckMidiChannel]);                    // Copy the currently selected MIDI program to the new track channel
            if (chorusState == HIGH)                                                                        // Copy chorus setting to loop track channel if enabled
            {
                controlChange(loopTrackOutputChannel[11], 93, 127);
            }
            else
            {
                controlChange(loopTrackOutputChannel[11], 93, 0);
            }
            if (reverbState == HIGH)                                                                        // Copy reverb setting to loop track channel if enabled
            {
                controlChange(loopTrackOutputChannel[11], 91, 127);
            }
            else
            {
                controlChange(loopTrackOutputChannel[11], 91, 0);
            }
            pitchBendChange(loopTrackOutputChannel[11], referencePitch & 0x7F, referencePitch >> 7);        // Reset pitch bend to reference level
            controlChange(loopTrackOutputChannel[11], 1, 0);                                                // Reset modulation
        }
        if (leftDeckDrumsModeEnabled == LOW)                                                                // If drums mode is disabled
        {
            // Left Deck
            for (int myTrack = 0; myTrack < maxTracks - 1; myTrack++)                                       // For all tracks allocated to the looper (minus the drum track)
            {
                if (loopTrackActive[myTrack] == LOW)                                                        // If this track is LOW meaning unused
                {
                    loopTrackActive[myTrack] = HIGH;                                                        // Mark the current track as in use
                    loopTrackInputChannel[myTrack] = 0;                                                     // Take input from left deck channel 0
                    loopTrackOutputChannel[myTrack] = loopChannelAssignment[myTrack];                       // Assign this looper track to the appropriate output channel
                    loopTrackIndex[myTrack] = 0;                                                            // Set the track index to 0
                    loopTrackHighestIndex[myTrack] = 0;                                                     // Set the highest track index to 0
                    programChange(loopTrackOutputChannel[myTrack], midiProgram[leftDeckMidiChannel]);       // Copy the currently selected MIDI program to the new track channel
                    if (chorusState == HIGH)                                                                // Copy chorus setting to loop track channel if enabled
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 93, 127);
                    }
                    else
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 93, 0);
                    }
                    if (reverbState == HIGH)                                                                // Copy reverb setting to loop track channel if enabled
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 91, 127);
                    }
                    else
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 91, 0);
                    }
                    pitchBendChange(loopTrackOutputChannel[myTrack], referencePitch & 0x7F, referencePitch >> 7);   // Reset pitch bend to reference level (8192)
                    controlChange(loopTrackOutputChannel[myTrack], 1, 0);                                           // Reset modulation
                    break;                                                                                          // Break out of the loop with our new track
                }
            }
        }
        // Right Deck
        for (int myTrack = 0; myTrack < maxTracks - 1; myTrack++)                                       // For all tracks allocated to the looper (minus the drum track)
        {
            if (loopTrackActive[myTrack] == LOW)                                                        // If this track is LOW meaning unused
            {
                loopTrackActive[myTrack] = HIGH;                                                        // Mark the current track as in use
                loopTrackInputChannel[myTrack] = 1;                                                     // Take input from right deck channel 1

                loopTrackOutputChannel[myTrack] = loopChannelAssignment[myTrack];                       // Assign this looper track to the appropriate output channel
                loopTrackIndex[myTrack] = 0;                                                            // Set the track index to 0
                loopTrackHighestIndex[myTrack] = 0;                                                     // Set the highest track index to 0
                programChange(loopTrackOutputChannel[myTrack], midiProgram[rightDeckMidiChannel]);      // Copy the currently selected MIDI program to the new track channel
                if (chorusState == HIGH)                                                                // Copy chorus setting to loop track channel if enabled
                {
                    controlChange(loopTrackOutputChannel[myTrack], 93, 127);
                }
                else
                {
                    controlChange(loopTrackOutputChannel[myTrack], 93, 0);
                }
                if (reverbState == HIGH)                                                                // Copy reverb setting to loop track channel if enabled
                {
                    controlChange(loopTrackOutputChannel[myTrack], 91, 127);
                }
                else
                {
                    controlChange(loopTrackOutputChannel[myTrack], 91, 0);
                }
                pitchBendChange(loopTrackOutputChannel[myTrack], referencePitch & 0x7F, referencePitch >> 7);   // Reset pitch bend to reference level (8192)
                controlChange(loopTrackOutputChannel[myTrack], 1, 0);                                           // Reset modulation
                break;                                                                                          // Break out of the loop with our new track
            }
        }
        // Left Deck Layer
        if (leftDeckLayerNotesEnabled == HIGH)
        {
            for (int myTrack = 0; myTrack < maxTracks - 1; myTrack++)                                       // For all tracks allocated to the looper (minus the drum track)
            {
                if (loopTrackActive[myTrack] == LOW)                                                        // If this track is LOW meaning unused
                {
                    loopTrackActive[myTrack] = HIGH;                                                        // Mark the current track as in use
                    loopTrackInputChannel[myTrack] = 2;                                                     // Take input from left  layer channel 2

                    loopTrackOutputChannel[myTrack] = loopChannelAssignment[myTrack];                       // Assign this looper track to the appropriate output channel
                    loopTrackIndex[myTrack] = 0;                                                            // Set the track index to 0
                    loopTrackHighestIndex[myTrack] = 0;                                                     // Set the highest track index to 0
                    programChange(loopTrackOutputChannel[myTrack], midiProgram[leftDeckLayerMidiChannel]);  // Copy the currently selected MIDI program to the new track channel
                    if (chorusState == HIGH)                                                                // Copy chorus setting to loop track channel if enabled
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 93, 127);
                    }
                    else
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 93, 0);
                    }
                    if (reverbState == HIGH)                                                                // Copy reverb setting to loop track channel if enabled
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 91, 127);
                    }
                    else
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 91, 0);
                    }
                    pitchBendChange(loopTrackOutputChannel[myTrack], referencePitch & 0x7F, referencePitch >> 7);   // Reset pitch bend to reference level (8192)
                    controlChange(loopTrackOutputChannel[myTrack], 1, 0);                                           // Reset modulation
                    break;                                                                                          // Break out of the loop with our new track
                }
            }
        }
        // Right Deck Layer
        if (rightDeckLayerNotesEnabled == HIGH)
        {
            for (int myTrack = 0; myTrack < maxTracks - 1; myTrack++)                                       // For all tracks allocated to the looper (minus the drum track)
            {
                if (loopTrackActive[myTrack] == LOW)                                                        // If this track is LOW meaning unused
                {
                    loopTrackActive[myTrack] = HIGH;                                                        // Mark the current track as in use
                    loopTrackInputChannel[myTrack] = 3;                                                     // Take input from right layer channel 3

                    loopTrackOutputChannel[myTrack] = loopChannelAssignment[myTrack];                       // Assign this looper track to the appropriate output channel
                    loopTrackIndex[myTrack] = 0;                                                            // Set the track index to 0
                    loopTrackHighestIndex[myTrack] = 0;                                                     // Set the highest track index to 0
                    programChange(loopTrackOutputChannel[myTrack], midiProgram[rightDeckLayerMidiChannel]); // Copy the currently selected MIDI program to the new track channel
                    if (chorusState == HIGH)                                                                // Copy chorus setting to loop track channel if enabled
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 93, 127);
                    }
                    else
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 93, 0);
                    }
                    if (reverbState == HIGH)                                                                // Copy reverb setting to loop track channel if enabled
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 91, 127);
                    }
                    else
                    {
                        controlChange(loopTrackOutputChannel[myTrack], 91, 0);
                    }
                    pitchBendChange(loopTrackOutputChannel[myTrack], referencePitch & 0x7F, referencePitch >> 7);   // Reset pitch bend to reference level (8192)
                    controlChange(loopTrackOutputChannel[myTrack], 1, 0);                                           // Reset modulation
                    break;                                                                                          // Break out of the loop with our new track
                }
            }
        }

        // If no available looper tracks were detected above, skip to here.
        looperFull:                                                                                                 // If no loop tracks are available for recording, continue without initializing more loops

        previousLoopIteration = currentLoopIteration;                                                               // Setup is complete.  Change previousLoopIteration to match currentLoopIteration.
    }


    // Recording happens inside of the MIDI packet functions at the bottom of the program code.
    // It must be called every time a note is sent, or the looper can only intercept the most recent note played in a single program loop.


    // The looper button is pressed again and there is data in memory that is not yet in playback.  Finalize for playback.
    if (loopActivationState == HIGH && previousLoopActivationState == LOW && loopRecordingEnabled == HIGH && loopPlaybackEnabled == LOW && loopInMemory == HIGH)
    {
        previousLoopActivationState = HIGH;                                             // Lock the input
        loopDrumsInMemory == HIGH;                                                      // Mark drums in memory
        leftDeckDrumsModeEnabled = LOW;                                                 // Disable drums mode to allow for immediate playing on second loop
        leftDeckMidiChannel = leftDeckDefaultMidiChannel;                               // Set the left deck channel to default (MIDI channel 0)
        programChange(leftDeckMidiChannel, midiProgram[leftDeckMidiChannel]);           // Change the program back to what was saved for this channel
        loopPlaybackEnabled = HIGH;                                                     // Set loop playback to enabled
        loopDuration = (currentTime - loopStartTime);                                   // Subtract current time from loop start time to find the loop duration
        loopStartTime = currentTime;                                                    // Reset loop start time to current time in preparation for another trip through the loop
        for (int myTrack = 0; myTrack < maxTracks; myTrack++)                           // For all tracks in the looper
        {
            loopTrackIndex[myTrack] = 0;                                                // Reset all track indexes to 0 in preparation for another trip through the loop
            loopTrackInputChannel[myTrack] = 20;                                        // Set track input channels to a value outside valid range
        }
        currentLoopIteration = currentLoopIteration + 1;                                // Iterate the loop counter to prime recording for a new loop immediately
    }

    // The looper button is pressed again during a playback/recording loop.  Disable recording.
    if (loopActivationState == HIGH && previousLoopActivationState == LOW && loopRecordingEnabled == HIGH && loopPlaybackEnabled == HIGH && loopInMemory == HIGH)
    {
        previousLoopActivationState = HIGH;                                             // Lock the input
        loopRecordingEnabled = LOW;                                                     // Disable recording
    }

    // Playback runs continuously until cancelled
    if (loopPlaybackEnabled == HIGH && loopInMemory == HIGH)
    {
        // If loop duration is exceeded, set the loop start back to current time, and reset the index positions to 0
        if ((loopStartTime + loopDuration) < currentTime)
        {
            if (loopRecordingEnabled == HIGH)
            {
                currentLoopIteration = currentLoopIteration + 1;
            }

            // Clean up any potential remaining noteOff events
            for (int myTrack = 0; myTrack < maxTracks; myTrack++)                                           // For all tracks allocated to the looper
            {
                if (loopTrackActive[myTrack] == HIGH)                                                       // If the channel is marked as active (HIGH)
                {
                    for (int myIndex = 0 ; myIndex < hangingNotesIndex[myTrack]; myIndex++)                 // For each noteOn played thus far in the loop
                    {
                        if (hangingNotes[myTrack][myIndex] != 254)                                          // If note value isn't 254 (meaning it's an actual note)
                        {
                            noteOff(loopTrackOutputChannel[myTrack], hangingNotes[myTrack][myIndex], 0);    // Send a matching noteOff for every noteOn played thus far to make sure everything was closed properly
                        }
                    }
                }
                hangingNotesIndex[myTrack] = 0;                                                             // Reset the hanging notes index counter
            }
            loopStartTime = currentTime;                                                // Reset loop start to current time for a new trip through
            for (int myTrack = 0; myTrack < maxTracks; myTrack++)                       // For all loop tracks
            {
                loopTrackIndex[myTrack] = 0;                                            // Set the index counter back to 0 for a new trip through
                loopTrackInputChannel[myTrack] = 20;                                    // Set track input channels to a value outside valid range
                if (loopTrackHighestIndex[myTrack] == 0)                                // If the highest index is still 0 (meaning the deck wasn't played)
                {
                    loopTrackActive[myTrack] = LOW;                                     // Free up the track for more overdubbing
                }
            }
        }

        // Play back the duration of the loop on repeat
        for (int myTrack = 0; myTrack < maxTracks; myTrack++)                                   // For all tracks in the looper
        {
            if (loopTrackActive[myTrack] == HIGH)                                               // If the track is marked as active (HIGH)
            {
                for (int myIndex = 0; myIndex < loopTrackHighestIndex[myTrack]; myIndex++)      // For all indexes up to the highest one recorded for this track
                {
                    if ((loopStartTime + loopTrackEventTime[myTrack][loopTrackIndex[myTrack]]) <= currentTime && loopTrackIndex[myTrack] < loopTrackHighestIndex[myTrack])  // If the pending event's recorded time is less than current time through the loop, send the packet out, and increment the index counter
                    {
                        if (loopTrackEventType[myTrack][loopTrackIndex[myTrack]] == 1)          // If event type is 1, send a noteOn
                        {
                            noteOn(loopTrackOutputChannel[myTrack], loopTrackByte3[myTrack][loopTrackIndex[myTrack]], loopTrackByte4[myTrack][loopTrackIndex[myTrack]]);    // Send saved noteOn
                            hangingNotes[myTrack][hangingNotesIndex[myTrack]] = loopTrackByte3[myTrack][loopTrackIndex[myTrack]];                                           // Save noteOn channel and value to an array
                            hangingNotesIndex[myTrack] = hangingNotesIndex[myTrack] + 1;                                                                                    // Increment in hanging notes check index counter
                        }
                        if (loopTrackEventType[myTrack][loopTrackIndex[myTrack]] == 0)          // 0, send a noteOff
                        {
                            noteOff(loopTrackOutputChannel[myTrack], loopTrackByte3[myTrack][loopTrackIndex[myTrack]], 0);                                                  // Send saved noteOff
                            for (int i = 0; i < hangingNotesIndex[myTrack]; i++)                                                                                            // For all recorded noteOn events
                            {
                                if (loopTrackByte3[myTrack][loopTrackIndex[myTrack]] == hangingNotes[myTrack][i])                                                           // If this noteOff matches a noteOn in the hanging notes check array
                                {
                                    hangingNotes[myTrack][i] = 254;                                                                                                         // Set the value to 254 so we can ignore it.
                                }
                            }
                        }
                        if (loopTrackEventType[myTrack][loopTrackIndex[myTrack]] == 2)          // 2, send a controlChange
                        {
                            controlChange(loopTrackOutputChannel[myTrack], loopTrackByte3[myTrack][loopTrackIndex[myTrack]], loopTrackByte4[myTrack][loopTrackIndex[myTrack]]);     // Send saved controlChange
                        }
                        if (loopTrackEventType[myTrack][loopTrackIndex[myTrack]] == 3)          // 3, send a pitchBendChange
                        {
                            pitchBendChange(loopTrackOutputChannel[myTrack], loopTrackByte3[myTrack][loopTrackIndex[myTrack]], loopTrackByte4[myTrack][loopTrackIndex[myTrack]]);   // Send saved pitchBendChange
                        }
                        loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                  // Increment the index position
                    }
                }
            }
        }
    }

    // Looper button/pedal is held down to force a reset
    if (loopActivationState == HIGH && previousLoopResetState == LOW)                   // If button is active
    {
        previousLoopResetState = HIGH;                                                  // Lock input
        loopResetClock = currentTime + 1500;                                            // Set reset time to currentTime + 1500 milliseconds.  If the button is not released before then, we will reset.
    }
    if (loopActivationState == LOW)                                                     // If the button is released
    {
        loopResetClock = 4294967295;                                                    // Set loopResetClock back to the max value for an unsigned long (49.7 days of milliseconds from power on)
    }
}

void playNotes()
{
    if (metaAState == LOW && metaBState == LOW)                                                             // Only play notes if no meta keys are held, otherwise duplicate notes would sound when running the program selection preview
    {
        // Left Deck
        for (int i = 0; i < leftDeckElementCount; i++)                                                      // For all buttons in the left deck
        {
            if (activeButtonsLeft[i] != previousActiveButtonsLeft[i])                                       // If a change is detected
            {
                if (activeButtonsLeft[i] == 1)                                                              // If the button is active
                {
                    if (leftDeckLegatoModeEnabled == HIGH)                                                  // If legato mode is enabled, blip sustain on left main and layer channels
                    {
                        controlChange(leftDeckMidiChannel, 64, 0);                                          // Disable sustain on the main channel
                        controlChange(leftDeckLayerMidiChannel, 64, 0);                                     // Disable sustain on the layer channel
                        controlChange(leftDeckMidiChannel, 64, 127);                                        // Reenable sustain on the main channel
                        controlChange(leftDeckLayerMidiChannel, 64, 127);                                   // Reenable sustain on the layer channel
                    }
                    if (leftDeckDrumsModeEnabled == HIGH)                                                   // If drums mode is enabled
                    {
                        noteOn(leftDeckMidiChannel, leftDeckDrums[i], leftDeckVelocity);                    // Send a noteOn for the main left deck channel drums layout with no octave modifier
                    }
                    else                                                                                    // If drums mode is disabled
                    {
                        noteOn(leftDeckMidiChannel, leftDeckWickiHayden[i] + octave, leftDeckVelocity);     // Send a noteOn for the main left deck channel
                    }
                    if (leftDeckLayerNotesEnabled == HIGH)                                                  // If layer mode is active
                    {
                        noteOn(leftDeckLayerMidiChannel, leftDeckWickiHayden[i], multiVelocity);            // Send a noteOn for layer as well
                    }
                    previousActiveButtonsLeft[i] = 1;                                                       // Update the "previous" variable for comparison on next loop
                }
                else                                                                                        // If the button is inactive
                {
                    if (leftDeckDrumsModeEnabled == HIGH)                                                   // If drums mode is enabled
                    {
                        noteOff(leftDeckMidiChannel, leftDeckDrums[i], 0);                                  // Send a noteOff for the main left deck channel drums layout with no octave modifier
                    }
                    else                                                                                    // If drums mode is disabled
                    {
                        noteOff(leftDeckMidiChannel, leftDeckWickiHayden[i] + octave, 0);                   // Send a noteOff
                    }
                    if (leftDeckLayerNotesEnabled == HIGH)                                                  // If layer mode is active
                    {
                        noteOff(leftDeckLayerMidiChannel, leftDeckWickiHayden[i], 0);                       // Send a noteOff for layer as well
                    }
                    previousActiveButtonsLeft[i] = 0;                                                       // Update the "previous" variable for comparison on next loop
                }
            }
        }
        // Right Deck
        for (int i = 0; i < rightDeckElementCount; i++)                                                     // For all buttons in the right deck
        {
            if (activeButtonsRight[i] != previousActiveButtonsRight[i])                                     // If a change is detected
            {
                if (activeButtonsRight[i] == 1)                                                             // If the button is active
                {
                    noteOn(rightDeckMidiChannel, rightDeckWickiHayden[i] + octave, rightDeckVelocity);      // Send a noteOn for the main right deck channel
                    if (rightDeckLayerNotesEnabled == HIGH)                                                 // If layer mode is active
                    {
                        noteOn(rightDeckLayerMidiChannel, rightDeckWickiHayden[i], multiVelocity);          // Send a noteOn for layer as well
                    }
                    previousActiveButtonsRight[i] = 1;                                                      // Update the "previous" variable for comparison on next loop
                }
                else                                                                                        // If the button is inactive
                {
                    noteOff(rightDeckMidiChannel, rightDeckWickiHayden[i] + octave, 0);                     // Send a noteOff
                    if (rightDeckLayerNotesEnabled == HIGH)                                                 // If layer mode is active
                    {
                        noteOff(rightDeckLayerMidiChannel, rightDeckWickiHayden[i], 0);                     // Send a noteOff for layer as well
                    }
                    previousActiveButtonsRight[i] = 0;                                                      // Update the "previous" variable for comparison on next loop
                }
            }
        }
    }
}

// MIDI PACKET FUNCTIONS

// Send MIDI Note On
// 1st byte = Event type (0x09 = note on, 0x08 = note off).
// 2nd byte = Event type combined with MIDI channel.
// 3rd byte = MIDI note number.
// 4th byte = Velocity (7-bit range 0-127)
void noteOn(byte channel, byte pitch, byte velocity)
{
    // Looper functions (these need to run every time a MIDI packet function is called, otherwise the looper would only intercept the most recent event in any single program loop)
    if ((channel >= 0 && channel < 4) || channel == 9) // If channel is 0 (left deck), 1 (right deck), 2 (left layer), 3 (right layer), or 9 (drums channel)
    {
        midiPacketIndex = midiPacketIndex + 1;                                                                      // Increment the MIDI packet index for the looper
        byte eventType = 1;                                                                                         // Save event type for looper (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        byte myTrack = 20;                                                                                          // Initialize track to a value outside the valid range
        // Record notes to the looper
        if (loopRecordingEnabled == HIGH && loopInMemory == HIGH && currentLoopIteration == previousLoopIteration)
        {
            if (midiPacketIndex > previousMidiPacketIndex)                                                          // If the MIDI packet index has been incremented (meaning a new event has occurred)
            {
                if (channel == 9 && leftDeckDrumsModeEnabled == HIGH && loopTrackActive[11] == HIGH && loopDrumsInMemory == LOW)    // If drums mode is enabled, and there isn't already a drums track recorded
                {
                    myTrack = 11;                                                                                   // Track 11 reserved for MIDI percussion channel 9
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = pitch;                                       // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = velocity;                                    // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
                if (channel >= 0 && channel < 4)                                                                    // If drums mode is disabled
                {
                    for (byte trackNumber = 0; trackNumber < maxTracks - 1; trackNumber++)                          // For all tracks in the looper (minus the drum track)
                    {
                        if (channel == 0 && loopTrackInputChannel[trackNumber] == 0)                                // If the current packet's channel is 0 (left deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 1 && loopTrackInputChannel[trackNumber] == 1)                                // If the current packet's channel is 1 (right deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 2 && loopTrackInputChannel[trackNumber] == 2)                                // If the current packet's channel is 2 (left layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 3 && loopTrackInputChannel[trackNumber] == 3)                                // If the current packet's channel is 3 (right layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                    }
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = pitch;                                       // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = velocity;                                    // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
            }
            previousMidiPacketIndex = midiPacketIndex;                                                              // Save the "previous" variable for comparison on next program loop
        }
    }

    midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};                 // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(noteOn);                                                           // Send packet to the MIDI USB bus
    Serial1.write(0x90 | channel);                                                      // Send event type/channel to the MIDI serial bus
    Serial1.write(pitch);                                                               // Send note number to the MIDI serial bus
    Serial1.write(velocity);                                                            // Send velocity value to the MIDI serial bus
    // delay(1);                                                                           // Dropping notes.  Testing a 1ms delay before flush to ensure only 1 packet per millisecond (31250bps: / 32(packet size) = 976.5 packets per second)
    // MidiUSB.flush();                                                                    // Flush the USB buffer
    // Serial1.flush();                                                                    // Flush the serial buffer
}

// Send MIDI Note Off
// 1st byte = Event type (0x09 = note on, 0x08 = note off).
// 2nd byte = Event type combined with MIDI channel.
// 3rd byte = MIDI note number.
// 4th byte = Velocity (7-bit range 0-127)
void noteOff(byte channel, byte pitch, byte velocity)
{
    // Looper functions (these need to run every time a MIDI packet function is called, otherwise the looper would only intercept the most recent event in any single program loop)
    if ((channel >= 0 && channel < 4) || channel == 9) // If channel is 0 (left deck), 1 (right deck), 2 (left layer), 3 (right layer), or 9 (drums channel)
    {
        midiPacketIndex = midiPacketIndex + 1;                                                                      // Increment the MIDI packet index for the looper
        byte eventType = 0;                                                                                         // Save event type for looper (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        byte myTrack = 20;                                                                                          // Initialize track to a value outside the valid range
        // Record notes to the looper
        if (loopRecordingEnabled == HIGH && loopInMemory == HIGH && currentLoopIteration == previousLoopIteration)
        {
            if (midiPacketIndex > previousMidiPacketIndex)                                                          // If the MIDI packet index has been incremented (meaning a new event has occurred)
            {
                if (channel == 9 && leftDeckDrumsModeEnabled == HIGH && loopTrackActive[11] == HIGH && loopDrumsInMemory == LOW)    // If drums mode is enabled, and there isn't already a drums track recorded
                {
                    myTrack = 11;                                                                                   // Track 11 reserved for MIDI percussion channel 9
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = pitch;                                       // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = velocity;                                    // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
                if (channel >= 0 && channel < 4)                                                                    // If drums mode is disabled
                {
                    for (byte trackNumber = 0; trackNumber < maxTracks - 1; trackNumber++)                          // For all tracks in the looper (minus the drum track)
                    {
                        if (channel == 0 && loopTrackInputChannel[trackNumber] == 0)                                // If the current packet's channel is 0 (left deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 1 && loopTrackInputChannel[trackNumber] == 1)                                // If the current packet's channel is 1 (right deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 2 && loopTrackInputChannel[trackNumber] == 2)                                // If the current packet's channel is 2 (left layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 3 && loopTrackInputChannel[trackNumber] == 3)                                // If the current packet's channel is 3 (right layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                    }
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = pitch;                                       // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = velocity;                                    // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
            }
            previousMidiPacketIndex = midiPacketIndex;                                                              // Save the "previous" variable for comparison on next program loop
        }
    }

    midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};                // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(noteOff);                                                          // Send packet to the MIDI USB bus
    Serial1.write(0x80 | channel);                                                      // Send event type/channel to the MIDI serial bus
    Serial1.write(pitch);                                                               // Send note number to the MIDI serial bus
    Serial1.write(velocity);                                                            // Send velocity value to the MIDI serial bus
    // delay(1);                                                                           // Dropping notes.  Testing a 1ms delay before flush to ensure only 1 packet per millisecond (31250bps / 32(packet size) = 976.5 packets per second)
    // MidiUSB.flush();                                                                    // Flush the USB buffer
    // Serial1.flush();                                                                    // Flush the serial buffer
}

// Control Change
// 1st byte = Event type (0x0B = Control Change).
// 2nd byte = Event type combined with MIDI channel.
// 3rd byte = MIDI CC number (7-bit range 0-127).
// 4th byte = Control value (7-bit range 0-127).
void controlChange(byte channel, byte control, byte value)
{
    // Looper functions (these need to run every time a MIDI packet function is called, otherwise the looper would only intercept the most recent event in any single program loop)
    if ((channel >= 0 && channel < 4) || channel == 9) // If channel is 0 (left deck), 1 (right deck), 2 (left layer), 3 (right layer), or 9 (drums channel)
    {
        midiPacketIndex = midiPacketIndex + 1;                                                                      // Increment the MIDI packet index for the looper
        byte eventType = 2;                                                                                         // Save event type for looper (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        byte myTrack = 20;                                                                                          // Initialize track to a value outside the valid range
        // Record notes to the looper
        if (loopRecordingEnabled == HIGH && loopInMemory == HIGH && currentLoopIteration == previousLoopIteration)
        {
            if (midiPacketIndex > previousMidiPacketIndex)                                                          // If the MIDI packet index has been incremented (meaning a new event has occurred)
            {
                if (channel == 9 && leftDeckDrumsModeEnabled == HIGH && loopTrackActive[11] == HIGH && loopDrumsInMemory == LOW)    // If drums mode is enabled, and there isn't already a drums track recorded
                {
                    myTrack = 11;                                                                                   // Track 11 reserved for MIDI percussion channel 9
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = control;                                     // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = value;                                       // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
                if (channel >= 0 && channel < 4)                                                                    // If drums mode is disabled
                {
                    for (byte trackNumber = 0; trackNumber < maxTracks - 1; trackNumber++)                          // For all tracks in the looper (minus the drum track)
                    {
                        if (channel == 0 && loopTrackInputChannel[trackNumber] == 0)                                // If the current packet's channel is 0 (left deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 1 && loopTrackInputChannel[trackNumber] == 1)                                // If the current packet's channel is 1 (right deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 2 && loopTrackInputChannel[trackNumber] == 2)                                // If the current packet's channel is 2 (left layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 3 && loopTrackInputChannel[trackNumber] == 3)                                // If the current packet's channel is 3 (right layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                    }
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = control;                                     // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = value;                                       // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
            }
            previousMidiPacketIndex = midiPacketIndex;                                                              // Save the "previous" variable for comparison on next program loop
        }
    }

    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};                   // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(event);                                                            // Send packet to the MIDI USB bus
    Serial1.write(0xB0 | channel);                                                      // Send event type/channel to the MIDI serial bus
    Serial1.write(control);                                                             // Send control change number to the MIDI serial bus
    Serial1.write(value);                                                               // Send control chnage value to the MIDI serial bus
    // delay(1);                                                                           // Dropping notes.  Testing a 1ms delay before flush to ensure only 1 packet per millisecond (31250bps / 32(packet size) = 976.5 packets per second)
    // MidiUSB.flush();                                                                    // Flush the USB buffer
    // Serial1.flush();                                                                    // Flush the serial buffer
}

// Program Change
// 1st byte = Event type (0x0C = Program Change).
// 2nd byte = Event type combined with MIDI channel.
// 3rd byte = Program value (7-bit range 0-127).
void programChange(byte channel, byte value)
{
    midiEventPacket_t event = {0x0C, 0xC0 | channel, value};                            // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(event);                                                            // Send packet to the MIDI USB bus
    Serial1.write(0xC0 | channel);                                                      // Send event type/channel to the MIDI serial bus
    Serial1.write(value);                                                               // Send program change value to the MIDI serial bus
    // delay(1);                                                                           // Dropping notes.  Testing a 1ms delay before flush to ensure only 1 packet per millisecond (31250bps / 32(packet size) = 976.5 packets per second)
    // MidiUSB.flush();                                                                    // Flush the USB buffer
    // Serial1.flush();                                                                    // Flush the serial buffer
}

// Pitch Bend
// (14 bit value 0-16363, neutral position = 8192)
// 1st byte = Event type (0x0E = Pitch bend change).
// 2nd byte = Event type combined with MIDI channel.
// 3rd byte = The 7 least significant bits of the value.
// 4th byte = The 7 most significant bits of the value.
void pitchBendChange(byte channel, byte lowValue, byte highValue)
{
    // Looper functions (these need to run every time a MIDI packet function is called, otherwise the looper would only intercept the most recent event in any single program loop)
    if ((channel >= 0 && channel < 4) || channel == 9) // If channel is 0 (left deck), 1 (right deck), 2 (left layer), 3 (right layer), or 9 (drums channel)
    {
        midiPacketIndex = midiPacketIndex + 1;                                                                      // Increment the MIDI packet index for the looper
        byte eventType = 3;                                                                                         // Save event type for looper (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        byte myTrack = 20;                                                                                          // Initialize track to a value outside the valid range
        // Record notes to the looper
        if (loopRecordingEnabled == HIGH && loopInMemory == HIGH && currentLoopIteration == previousLoopIteration)
        {
            if (midiPacketIndex > previousMidiPacketIndex)                                                          // If the MIDI packet index has been incremented (meaning a new event has occurred)
            {
                if (channel == 9 && leftDeckDrumsModeEnabled == HIGH && loopTrackActive[11] == HIGH && loopDrumsInMemory == LOW)    // If drums mode is enabled, and there isn't already a drums track recorded
                {
                    myTrack = 11;                                                                                   // Track 11 reserved for MIDI percussion channel 9
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = lowValue;                                    // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = highValue;                                   // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
                if (channel >= 0 && channel < 4)                                                                    // If drums mode is disabled
                {
                    for (byte trackNumber = 0; trackNumber < maxTracks - 1; trackNumber++)                          // For all tracks in the looper (minus the drum track)
                    {
                        if (channel == 0 && loopTrackInputChannel[trackNumber] == 0)                                // If the current packet's channel is 0 (left deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 1 && loopTrackInputChannel[trackNumber] == 1)                                // If the current packet's channel is 1 (right deck), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 2 && loopTrackInputChannel[trackNumber] == 2)                                // If the current packet's channel is 2 (left layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                        if (channel == 3 && loopTrackInputChannel[trackNumber] == 3)                                // If the current packet's channel is 3 (right layer), find the loop track assigned to 0, and set myTrack to this
                        {
                            myTrack = trackNumber;
                        }
                    }
                    loopTrackEventType[myTrack][loopTrackIndex[myTrack]] = eventType;                               // Save the event type (0 = noteOff, 1 = noteOn, 2 = controlChange, 3 = pitchBendChange)
                    loopTrackChannel[myTrack][loopTrackIndex[myTrack]] = loopTrackOutputChannel[myTrack];           // Save the channel for this packet
                    loopTrackByte3[myTrack][loopTrackIndex[myTrack]] = lowValue;                                    // Save the note/cc number/pitch bend low byte
                    loopTrackByte4[myTrack][loopTrackIndex[myTrack]] = highValue;                                   // Save the velocity/cc value, pitch bend high byte
                    loopTrackEventTime[myTrack][loopTrackIndex[myTrack]] = currentTime - loopStartTime;             // Save the relative event time vs loopStartTime
                    loopTrackIndex[myTrack] = loopTrackIndex[myTrack] + 1;                                          // Increment the index counter for this track
                    loopTrackHighestIndex[myTrack] = loopTrackIndex[myTrack];                                       // Record the highest index reached thus far
                }
            }
            previousMidiPacketIndex = midiPacketIndex;                                                              // Save the "previous" variable for comparison on next program loop
        }
    }

    midiEventPacket_t bendEvent = {0x0E, 0xE0 | channel, lowValue, highValue};          // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(bendEvent);                                                        // Send packet to the MIDI USB bus
    Serial1.write(0xE0 | channel);                                                      // Send event type/channel to the MIDI serial bus
    Serial1.write(lowValue);                                                            // Send pitch bend low byte to the MIDI serial bus
    Serial1.write(highValue);                                                           // Send pitch bend high byte to the MIDI serial bus
    // delay(1);                                                                           // Dropping notes.  Testing a 1ms delay before flush to ensure only 1 packet per millisecond (31250bps / 32(packet size) = 976.5 packets per second)
    // MidiUSB.flush();                                                                    // Flush the USB buffer
    // Serial1.flush();                                                                    // Flush the serial buffer
}

// END FUNCTIONS SECTION
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

// END OF PROGRAM
// ------------------------------------------------------------------------------------------------------------------------------------------------------------
