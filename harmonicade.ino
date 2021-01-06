//---------------------------------------------------------------------//
//--                    -----  Harmonicade -----                     --//
//--       A portable dual-deck multi-channel MIDI keyboard          --//
//--  using arcade push-buttons in the Wicki-Hayden button layout.   --//
//--              Copyright (C) 2020 - Michael Koopman               --//
//--          KOOP Instruments (koopinstruments@gmail.com)           --//
//-- https://www.koopinstruments.com/instrument-projects/harmonicade --//
//---------------------------------------------------------------------//
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
//-----------------------------------------------------------------------
//
// Hardware Information:
// Requires Teensy 3.6 or other Arduino compatible micro-controller with sufficient I/O
// I/O Pin requirments:
//      38  pins in total
//          31 digital
//          4 analog
//          2 I2C
//          1 serial TX
// Additional pin requirements:
//      1 +5V for DIN MIDI
//      1 +3.3V/+5V for potentiometers
//      1 GND for DIN MIDI and potentiometers
//
// Deck 1:
// 85 x note buttons (scanning matrix of 10 columns * 9 rows = 90 with 5 unused nodes)
// 10 x control buttons (shares matrix with 10 columns * 1 row)
//  4 x potentiometers
//  1 x 1/4" TS jack
//  1 x 5 pin DIN MIDI serial jack
//  1 x 16x2 character I2C LCD screen
//
// Deck 2:
// 104 x note buttons (scanning matrix of 10 columns * 11 rows = 110 with 6 unused nodes)
//
//
// Device pinout:
// Name                 Teensy 3.6 Pin
// ------------------------------------
// Deck 1 Col 0           2
// Deck 1 Col 1           3
// Deck 1 Col 2           4
// Deck 1 Col 3           5
// Deck 1 Col 4           6
// Deck 1 Col 5           7
// Deck 1 Col 6           8
// Deck 1 Col 7           9
// Deck 1 Col 8          10
// Deck 1 Col 9          11
// --
// Deck 2 Col 0          36
// Deck 2 Col 1          37
// Deck 2 Col 2          38
// Deck 2 Col 3          14
// Deck 2 Col 4          15
// Deck 2 Col 5          16
// Deck 2 Col 6          17
// Deck 2 Col 7          20
// Deck 2 Col 8          21
// Deck 2 Col 9          22
// --
// Decks 1 & 2 Row 0          32
// Decks 1 & 2 Row 1          31
// Decks 1 & 2 Row 2          30
// Decks 1 & 2 Row 3          29
// Decks 1 & 2 Row 4          28
// Decks 1 & 2 Row 5          27
// Decks 1 & 2 Row 6          26
// Decks 1 & 2 Row 7          25
// Decks 1 & 2 Row 8          24
// Decks 1 & 2 Row 9          12
// Deck 2 Row 10              35
// --
// Pot Left/Left         A22
// Pot Left/Middle       A21
// Pot Right/Middle      A15
// Pot Right/Right       A14
// --
// DIN MIDI TX            1
// --
// I2C Clock (SCL)       A4
// I2C Data (SDA)        A5
//
// Button matrix locations in the deck and control panel matrix (square brackets indicate unused locations):
// Deck 1:
//  -------------- Control Panel ----------------
// |   0       2       4       6       8         |
// |       1       3       5       7       9     |
//  ------------------ Deck ---------------------
// |    10  11  12  13  14  15  16  17  18  [19] |
// |  20  21  22  23  24  25  26  27  28  29     |
// |    30  31  32  33  34  35  36  37  38  [39] |
// |  40  41  42  43  44  45  46  47  48  49     |
// |    50  51  52  53  54  55  56  57  58  [59] |
// |  60  61  62  63  64  65  66  67  68  69     |
// |    70  71  72  73  74  75  76  77  78  [79] |
// |  80  81  82  83  84  85  86  87  88  89     |
// |    90  91  92  93  94  95  96  97  98  [99] |
//  ---------------------------------------------
//
// Deck 2:
//  -------------- ---------------------------
// | [0] 1   2   3   4   5  6   7   8   9     |
// |   10  11  12  13  14  15  16  17  18  19 |
// |[20] 21  22  23  24  25  26  27  28  29   |
// |   30  31  32  33  34  35  36  37  38  39 |
// |[40[ 41  42  43  44  45  46  47  48  49   |
// |   50  51  52  53  54  55  56  57  58  59 |
// |[60] 61  62  63  64  65  66  67  68  69   |
// |   70  71  72  73  74  75  76  77  78  79 |
// |[80] 81  82  83  84  85  86  87  88  89   |
// |   90  91  92  93  94  95  96  97  98  99 |
// |[100]101 102 103 104 105 106 107 108 109  |
//  ------------------------------------------
//
// Control Panel Knobs:
// Left/Left: Effect 1 (MIDI CC 12)     Left/Middle: Effect 2 (MIDI CC 13)      Right/Middle: Modulation Button Level (MIDI CC 1)       Right/Right: Note Velocity
//
// Control Panel Buttons:
// ----------------------
// Normal Functions
// Pitch Bend Down              Modulation On               Pitch Bend Up       MIDI Channel Up         Octave Up
// Record Parent Loop (Toggle)  Record Overdub++ (Hold)     [Meta Key]          MIDI Channel Down       Octave Down
// --
// Meta Key Functions
// Drums Mode                   Reverb Toggle               Drone Mode          Program Up   +10        Program Up   +1
// N/A                          Stop/Retart Playback        [Meta Key]          Program Down -10        Program Down -1
//
// Foot Switch (Hold):
// Ham fisted attempt at dynamics, velocity changes between current pot setting (switch is off), or maximum (127) values (switch is on)
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START OF PROGRAM

// Define Teensy 3.x hardware restart variables for use in the controllerReset() function
#define RESTART_ADDR 0xE000ED0C
#define READ_RESTART() (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

// Include required libraries
#include "MIDIUSB.h"                                                    // MIDIUSB v1.0.4 by Gary Grewal
#include "Wire.h"                                                       // Wire - Included with Arduino IDE
#include "LiquidCrystal_I2C.h"                                          // LiquidCrystal I2C v1.1.2 by Frank de Brabander

// Define digital button matrix pins
const byte deck1Columns[]        = { 2,  3,  4,  5,  6,  7,  8,  9, 10, 11}; // Column pins in order from left to right
const byte deck1Rows[]           = {32, 31, 30, 29, 28, 27, 26, 25, 24, 12}; // Row pins in order from top to bottom
const byte deck1ColumnCount      = sizeof(deck1Columns);                          // The number of columns in the matrix
const byte deck1RowCount         = sizeof(deck1Rows);                             // The number of rows in the matrix
const byte deck1ElementCount     = deck1ColumnCount * deck1RowCount;                   // The number of elements in the matrix

const byte deck2Columns[]        = {36, 37, 38, 14, 15, 16, 17, 20, 21, 22}; // Column pins in order from left to right
const byte deck2Rows[]           = {32, 31, 30, 29, 28, 27, 26, 25, 24, 12, 35}; // Row pins in order from top to bottom
const byte deck2ColumnCount      = sizeof(deck2Columns);                          // The number of columns in the matrix
const byte deck2RowCount         = sizeof(deck2Rows);                             // The number of rows in the matrix
const byte deck2ElementCount     = deck2ColumnCount * deck2RowCount;                   // The number of elements in the matrix

const byte velocityPedalPin = 39;                                       // Digital foot pedal pin

// Define analog potentiometer pins
const byte LLPotPin = A22;                                              // Left/Left Pot
const byte LMPotPin = A21;                                              // Left/Middle Pot
const byte RMPotPin = A15;                                              // Right/Middle Pot
const byte RRPotPin = A14;                                              // Right/Right Pot

// MIDI note value tables
const byte deck1WickiHaydenLayout[deck1ElementCount] = {
00,00,00,00,00,00,00,00,00,00,                                          // '00' indicates unused node
   66,  68,  70,  72,  74,  76,  78,  80,  82,  00,
59,  61,  63,  65,  67,  69,  71,  73,  75,  77,
   54,  56,  58,  60,  62,  64,  66,  68,  70,  00,
47,  49,  51,  53,  55,  57,  59,  61,  63,  65,
   42,  44,  46,  48,  50,  52,  54,  56,  58,  00,
35,  37,  39,  41,  43,  45,  47,  49,  51,  53,
   30,  32,  34,  36,  38,  40,  42,  44,  46,  00,
23,  25,  27,  29,  31,  33,  35,  37,  39,  41,
   18,  20,  22,  24,  26,  28,  30,  32,  34,  00

};
const byte deck2WickiHaydenLayout[deck2ElementCount] = {
00,    90,  92,  94,  96,  98, 100, 102, 104, 106,
    83,  85,  87,  89,  91,  93,  95,  97,  99, 101,                     // '00' indicates unused node
00,    78,  80,  82,  84,  86,  88,  90,  92,  94,
    71,  73,  75,  77,  79,  81,  83,  85,  87,  89,
00,    66,  68,  70,  72,  74,  76,  78,  80,  82,
    59,  61,  63,  65,  67,  69,  71,  73,  75,  77,
00,    54,  56,  58,  60,  62,  64,  66,  68,  70,
    47,  49,  51,  53,  55,  57,  59,  61,  63,  65,
00,    42,  44,  46,  48,  50,  52,  54,  56,  58,
    35,  37,  39,  41,  43,  45,  47,  49,  51,  53,
00,    30,  32,  34,  36,  38,  40,  42,  44,  46
};
const byte drumLayout[deck1ElementCount] = {
00,  00,  00,  00,  00,  00,  00,  00,  00,     00,                     // '00' indicates unused node
   00,  00,  00,  00,  28,  00,  00,  00,  00,  00,
00,  25,  26,  29,  30,  86,  87,  78,  79,  00,
   73,  74,  75,  82,  27,  83,  84,  71,  72,  00,
00,  77,  76,  62,  64,  63,  85,  68,  67,  00,
   66,  65,  41,  47,  48,  50,  57,  80,  81,  00,
00,  61,  60,  43,  45,  48,  49,  52,  69,  00,
   70,  58,  42,  46,  46,  42,  51,  59,  54,  00,
25,  39,  40,  37,  38,  38,  37,  40,  31,  25,
   44,  35,  55,  36,  35,  36,  53,  35,  56,  00
};

// Global time variables
unsigned long   currentTime;                                            // Program loop consistent variable for time in milliseconds since power on
const byte      debounceTime = 80;                                      // Global digital button debounce time in milliseconds

// Variables for holding digital button states and activation times
byte            deck1ActiveButtons[deck1ElementCount];                            // Array to hold current note button states
byte            deck1PreviousActiveButtons[deck1ElementCount];                    // Array to hold previous note button states for comparison
unsigned long   deck1ActiveButtonsTime[deck1ElementCount];                        // Array to track last note button activation time for debounce

byte            deck2ActiveButtons[deck2ElementCount];                            // Array to hold current note button states
byte            deck2PreviousActiveButtons[deck2ElementCount];                    // Array to hold previous note button states for comparison
unsigned long   deck2ActiveButtonsTime[deck2ElementCount];                        // Array to track last note button activation time for debounce

// Control button states
byte velocityPedalState;                                                // Velocity pedal - 1/4" TS jack
byte pitchDownState;                                                    // Top row (white) left
byte modulationState;                                                   // Top row (green) left/center
byte pitchUpState;                                                      // Top row (white) center
byte channelUpState;                                                    // Top row (green) right/center
byte octaveUpState;                                                     // Top row (white) right
byte loopRecordState;                                                   // Bottom row (red)   left
byte loopOverdubState;                                                  // Bottom row (blue)  left/center
byte metaKeyState;                                                      // Bottom row (black) center
byte channelDownState;                                                  // Bottom row (green) right/center
byte octaveDownState;                                                   // Bottom row (white) right
byte previousVelocityPedalState;                                        // Comparison variable for input locking
byte previousPitchDownState;                                            // Comparison variable for input locking
byte previousModulationState;                                           // Comparison variable for input locking
byte previousPitchUpState;                                              // Comparison variable for input locking
byte previousChannelUpState;                                            // Comparison variable for input locking
byte previousOctaveUpState;                                             // Comparison variable for input locking
byte previousLoopRecordState;                                           // Comparison variable for input locking
byte previousLoopOverdubState;                                          // Comparison variable for input locking
byte previousChannelDownState;                                          // Comparison variable for input locking
byte previousOctaveDownState;                                           // Comparison variable for input locking


byte loopRecordTrigger;                                                 // Secondary state tracking to engage looper on button press, and disengage on subsequent press
byte previousLoopRecordTrigger;                                         // Previous state variable for comparison
byte loopOverdubTrigger;                                                // Secondary state tracking to blip overdub button if held through a looper rollover

// Variables for rate limiting analog MIDI CC updates to prevent buffer overruns (needed for iOS MIDI)
const byte      analogDelayTime = 25;                                   // Global analog update time (milliseconds)
const byte      analogDeadzone = 2;                                     // The amount an analog reading needs to change before the variable is updated

// Analog input variables
int             LLPotValue;                                             // The value of the Left/Left pot
int             LMPotValue;                                             // The value of the Left/Middle pot
byte            RMPotValue;                                             // The value of the Right/Middle pot (mapped to 7-bits)
byte            RRPotValue;                                             // The value of the Right/Right pot (mapped to 7-bits)
int             previousLLPotValue;                                     // The previous value of the Left/Left pot for comparison
int             previousLMPotValue;                                     // The previous value of the Left/Middle pot for comparison
byte            previousRMPotValue;                                     // The previous value of the Right/Middle pot for comparison
byte            previousRRPotValue;                                     // The previous value of the Right/Right pot for comparison
unsigned long   previousLLPotTime;                                      // The last activation time for rate limiting
unsigned long   previousLMPotTime;                                      // The last activation time for rate limiting
unsigned long   previousRMPotTime;                                      // The last activation time for rate limiting
unsigned long   previousRRPotTime;                                      // The last activation time for rate limiting


// Metronome variables
unsigned int    metronomeSpeed = 0;;
unsigned int    metronomeMin = 1500;        // Delay in ms between beats - 1500 = 40bpm
unsigned int    metronomeMax = 200;         // Delay in ms between beats - 200 = 300bpm
unsigned long   previousMetronomeTime = 0;;

// MIDI channel assignment
byte midiChannel = 0;                                                   // Current MIDI channel (changed via user input)

// MIDI program variables
byte midiProgram[16];                                                   // MIDI program selection per channel (0-15)

// Octave modifier
int octave;                                                             // Apply a MIDI note number offset (changed via user input in steps of 12)

// MIDI CC related variables
byte reverbState[16];                                                   // Toggle for enabling/disabling reverb effect per channel (MIDI CC #91)

// Velocity levels
byte velocity = 95;                                                     // Non-zero default velocity for testing; this will update via analog pot

// Pitch Bend
int pitchOffset             = 0;                                        // Offset amount for pitch bending.  pitchSpeed is added to this each loop up to a max value of 8191
const int pitchSpeed        = 512;                                      // The amount to increment pitchOffset each program loop (must cleanly divide 8192)
const int pitchStep         = 8191;                                     // The maximum amount that pitchOffset is allowed reach (8191 for whole-step or 4095 for half-step)
const int referencePitch    = 8192;                                     // Default neutral pitch position; can potentially exploit for global instrument tuning

// Toggles for drums and sustain modes
byte drumsModeEnabled;                                                  // Toggle optional drums mode on/off, and switch to a note layout to one more comfortable for finger drumming
byte sustainModeEnabled;                                                // Toggle optional sustain mode on/off (sustain enabled on note press, blipped off/on for each subsequent press)

// Looper variables
const byte      loopMaxTracks = 16;                                     // Number of tracks available for recording (MIDI channels 0-15)
const int       loopMaxIndexes = 2048;                                  // Number of indexes allowed per channel
byte            loopRecordingEnabled;                                   // State variable to enable MIDI packet recording
byte            loopPlaybackEnabled;                                    // State variable to enable playback once a loop is recorded
byte            loopInMemory;                                           // State variable indicating (among other things) if arrays need to reinit on record button press
byte            loopInputDetected;                                      // State variable indicating that a recordable input was pressed to kick off loop recording
unsigned long   loopDuration;                                           // Parent loop duration
unsigned long   loopStartTimestamp;                                     // Parent loop start timestamp
int             loopRecordingIndex[loopMaxTracks];                      // Current index of recording per channel
int             loopPlaybackIndex[loopMaxTracks];                       // Current index of playback per channel
unsigned long   loopPacketEventTime[loopMaxTracks][loopMaxIndexes];     // Loop packet event time
byte            loopPacketByte0[loopMaxTracks][loopMaxIndexes];         // Loop packet event type (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
byte            loopPacketByte1[loopMaxTracks][loopMaxIndexes];         // Loop packet pitch/control/highByte
byte            loopPacketByte2[loopMaxTracks][loopMaxIndexes];         // Loop packet velocity/value/lowByte
byte            loopTrackActive[loopMaxTracks];                         // Loop track activation status to prevent overdub notes from feeding back into the hopper before loop completion
byte            loopPercentage;                                         // Variable to track percentage of loop completion for printing to the LCD
byte            previousLoopPercentage;                                 // Previous state variable to prevent unnecessary LCD updates
byte            loopParentChannel = 255;                                // Loop track designated as parent (overdub tracks will utilize this track's duration)
int             looperVelocity; 


// LCD screen related variables
LiquidCrystal_I2C lcd(0x27,16,2);                                                           // Set LCD I2C address to 0x27 and 16 char 2 line display
byte upArrow[]            = { B00100,B01110,B11111,B00100,B00100,B00100,B00100,B00000 };    // Define an up arrow glyph in binary to send to the LCD
byte downArrow[]          = { B00100,B00100,B00100,B00100,B11111,B01110,B00100,B00000 };    // Define a down arrow glyph in binary to send to the LCD
byte recordLeft[]         = { B00011,B01111,B01100,B11001,B11001,B01100,B01111,B00011 };    // Define left side of recording glyph in binary to send to the LCD
byte recordRight[]        = { B11000,B11110,B00110,B10011,B10011,B00110,B11110,B11000 };    // Define right side of recording glyph in binary to send to the LCD
byte pauseLeft[]          = { B00110,B00110,B00110,B00110,B00110,B00110,B00110,B00000 };    // Define left side of pause glyph in binary to send to the LCD
byte pauseRight[]         = { B01100,B01100,B01100,B01100,B01100,B01100,B01100,B00000 };    // Define right side of pause glyph in binary to send to the LCD
byte loopLeft[]           = { B00000,B00000,B00010,B00100,B00010,B00000,B00000,B00000 };    // Define left side of loop track active glyph in binary to send to the LCD
byte loopRight[]          = { B00000,B00000,B01000,B00100,B01000,B00000,B00000,B00000 };    // Define right side of loop track active glyph in binary to send to the LCD
const char startupAnimation[]   = "KOOP Instruments  ";                                     // Text array for used for startup animation

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START SETUP SECTION
void setup()
{
    // Set up the LCD screen
    lcd.init();                                                                     // Initialize the lcd
    lcd.backlight();                                                                // Enable the backlight
    lcd.createChar(0, upArrow);                                                     // Send glyph to LCD memory
    lcd.createChar(1, downArrow);                                                   // Send glyph to LCD memory
    lcd.createChar(2, recordLeft);                                                  // Send glyph to LCD memory
    lcd.createChar(3, recordRight);                                                 // Send glyph to LCD memory
    lcd.createChar(4, pauseLeft);                                                   // Send glyph to LCD memory
    lcd.createChar(5, pauseRight);                                                  // Send glyph to LCD memory
    lcd.createChar(6, loopLeft);                                                    // Send glyph to LCD memory
    lcd.createChar(7, loopRight);                                                   // Send glyph to LCD memory


    // LCD Startup Animation (Strobe letters left to right before revealing full text with added delay to mitigate slow LCD refresh)
    for (int i = 0; i < 16; i = i + 1)
    {
    lcd.setCursor(i - 1,0);                                                         // Set cursor to prior column
    lcd.print(" ");                                                                 // Blank this column
    delay(10);                                                                      // Delay for effect
    lcd.setCursor(i,0);                                                             // Set cursor to current column
    lcd.print(startupAnimation[i]);                                                 // Print the current letter in the array
    delay(10);                                                                      // Delay for effect
    lcd.setCursor(i + 1,0);                                                         // Set cursor to the upcoming column
    lcd.print(startupAnimation[i + 1]);                                             // Print the next letter in the array
    delay(10);                                                                      // Delay for effect
    lcd.setCursor(i + 2,0);                                                         // Set cursor to the upcoming column
    lcd.print(startupAnimation[i + 2]);                                             // Print the next letter in the array
    delay(10);                                                                      // Delay 20 milliseconds and loop incrementing 1 position
    }
    lcd.setCursor(0,0);                                                             // Set cursor to column 0 line 0
    lcd.print("KOOP Instruments");                                                  // Print to LCD
    lcd.setCursor(0,1);                                                             // Set cursor to column 0 line 1
    lcd.print("Harmonicade v2.0");                                                  // Print to LCD
    delay(1250);                                                                    // Delay to allow logo to be displayed
    lcd.clear();                                                                    // Clear LCD
    delay(250);                                                                     // Wait for screen to fade

    // LCD Update Top Menu
    lcd.setCursor(0,0); lcd.print("LP M R PRG CH OC");
    // LCD Update Looper Disabled
    lcd.setCursor(0,1); lcd.print("--");
    // LCD Update Normal Mode Selected
    lcd.setCursor(3,1); lcd.print("N");
    // LCD Update Reverb Disabled
    lcd.setCursor(5,1); lcd.print("0");
    // LCD Update Default MIDI Program
    lcd.setCursor(7,1); lcd.print("001");
    // LCD Update Default MIDI Channel
    lcd.setCursor(11,1); lcd.print("01");
    // LCD Update Default Octave
    lcd.setCursor(14,1); lcd.print(" 0");

    // Enable serial for the MIDI DIN connector
    Serial1.begin(31250);                                                           // Serial MIDI 31250 baud

    // Set pinModes for the digital button matrix.
    for (int pinNumber = 0; pinNumber < deck1ColumnCount; pinNumber++)                   // For each column pin...
    {
        pinMode(deck1Columns[pinNumber], INPUT_PULLUP);                                  // set the pinMode to INPUT_PULLUP (+3.3V / HIGH).
    }
    for (int pinNumber = 0; pinNumber < deck1RowCount; pinNumber++)                      // For each row pin...
    {
        pinMode(deck1Rows[pinNumber], INPUT);                                            // Set the pinMode to INPUT (0V / LOW).
    }
    
    pinMode(velocityPedalPin, INPUT_PULLUP);                                        // Foot pedal

    // Set pinModes for analog inputs.
    pinMode(LLPotPin, INPUT);                                                       // Left/Left pot
    pinMode(LMPotPin, INPUT);                                                       // Left/Middle pot
    pinMode(RMPotPin, INPUT);                                                       // Right/Middle pot
    pinMode(RRPotPin, INPUT);                                                       // Right/Right pot


    // Enable serial for monitor
    // Serial.begin(115200);


}
// END SETUP SECTION
// ------------------------------------------------------------------------------------------------------------------------------------------------------------



// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START LOOP SECTION
void loop()
{
    // Store the current time in a uniform variable for this program loop
    currentTime = millis();

    // Read and store the digital button states of the scanning matrix
    readDeck1DigitalButtons();
    readDeck2DigitalButtons();

    // Read and store the analog potentiometer values
    readAnalogKnobs();

    // Set all states and values related to the control buttons and pots
    runControlModule();

    // Run the metronome function
    runMetronome();

    // Run the main and layer program select function
    runProgramSelect();

    // Run MIDI control change related tasks (modulation/reverb)
    runMidiCC();

    // Run the pitch bend function
    runPitchBend();

    // Run the octave select function
    runOctave();

    // Run the channel select function
    runChannelSelect();

    // Run the drums mode function
    runDrumsMode();

    // Run the sustain mode function
    runSustainMode();

    // Controller Reset (*needs to come before any note sending functions)
    runControllerReset();

    // Looper start detection (*needs to sit between readDigitalButtons() and playNotes() functions)
    if (loopRecordState == HIGH && previousLoopRecordState == HIGH && loopInMemory == LOW && loopInputDetected == HIGH)
    {
        loopStartTimestamp = currentTime;
        loopInMemory = HIGH;
        // LCD Update Looper Recording
        lcd.setCursor(0,1); lcd.write(2);lcd.write(3);
    }

    // Send notes to the MIDI bus
    playNotes();

    // Run the loop pedal function
    runLooper();



    // Print diagnostic troubleshooting information to serial monitor
    // diagnosticTest();



    // Reset input locking variables for next loop where appropriate
    loopInputDetected = LOW;    // Reset the state variable indicating that a recordable input was pressed to kick off loop recording
    if (velocityPedalState   == LOW) { previousVelocityPedalState   = LOW; }
    if (pitchDownState       == LOW) { previousPitchDownState       = LOW; }
    if (modulationState      == LOW) { previousModulationState      = LOW; }
    if (pitchUpState         == LOW) { previousPitchUpState         = LOW; }
    if (channelUpState       == LOW) { previousChannelUpState       = LOW; }
    if (octaveUpState        == LOW) { previousOctaveUpState        = LOW; }
    if (loopRecordState      == LOW) { previousLoopRecordState      = LOW; }
    if (loopOverdubState     == LOW) { previousLoopOverdubState     = LOW; }
    if (channelDownState     == LOW) { previousChannelDownState     = LOW; }
    if (octaveDownState      == LOW) { previousOctaveDownState      = LOW; }
    if (loopRecordTrigger    == LOW) { previousLoopRecordTrigger    = LOW; }

}
// END LOOP SECTION
// ------------------------------------------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// START FUNCTIONS SECTION

void diagnosticTest()
{
    // Scratch area for printing diagnostics information to the serial console
    // Serial.print(LLPotValue);
    // Serial.print(" ");
    // Serial.print(LMPotValue);
    // Serial.print(" ");
    // Serial.print(RMPotValue);
    // Serial.print(" ");
    // Serial.println(RRPotValue);

    // for (int i=0; i<deck2ElementCount; i++)
    // {
    //     Serial.print(deck2ActiveButtons[i]);
    // }
    // Serial.println();

}

void readDeck1DigitalButtons()
{
    // Button Deck
    for (byte columnIndex = 0; columnIndex < deck1ColumnCount; columnIndex++)                                // Iterate through each of the column pins.
    {
        byte currentColumn = deck1Columns[columnIndex];                                                      // Hold the currently selected column pin in a variable.
        pinMode(currentColumn, OUTPUT);                                                                 // Set that column pin to OUTPUT mode and...
        digitalWrite(currentColumn, LOW);                                                               // set the pin state to LOW turning it into a temporary ground.
        for (byte rowIndex = 0; rowIndex < deck1RowCount; rowIndex++)                                        // Now iterate through each of the row pins that are connected to the current column pin.
        {                       
            byte currentRow = deck1Rows[rowIndex];                                                           // Hold the currently selected row pin in a variable.
            pinMode(currentRow, INPUT_PULLUP);                                                          // Set that row pin to INPUT_PULLUP mode (+3.3V / HIGH).
            byte buttonNumber = columnIndex + (rowIndex * deck1ColumnCount);                                 // Assign this location in the matrix a unique number.
            delayMicroseconds(25);                                                                      // Delay to give the pin modes time to change state (false readings are caused otherwise).
            byte buttonState = !digitalRead(currentRow);                                                // Invert reading due to INPUT_PULLUP, and store the currently selected pin state.
            if (buttonState == HIGH && (millis() - deck1ActiveButtonsTime[buttonNumber]) > debounceTime)     // If button is active and passes debounce
            {
                deck1ActiveButtons[buttonNumber] = 1;                                                        // write a 1 to the storage variable
                deck1ActiveButtonsTime[buttonNumber] = millis();                                             // and save the last button press time for later debounce comparison.
                if (buttonNumber == 0 || buttonNumber == 2 || buttonNumber == 4 || buttonNumber > 9) { loopInputDetected = HIGH; }  // Activate loopInputDetected variable for looper ignoring non recorded buttons
            }
            if (buttonState == LOW)
            {
                deck1ActiveButtons[buttonNumber] = 0;                                                        // Or if the button is inactive, write a 0.
            }
            pinMode(currentRow, INPUT);                                                                 // Set the selected row pin back to INPUT mode (0V / LOW).
        }
        pinMode(currentColumn, INPUT);                                                                  // Set the selected column pin back to INPUT mode (0V / LOW) and move onto the next column pin.
    }
    // Foot Pedal
    velocityPedalState = !digitalRead(velocityPedalPin);                                                // Invert the reading due to INPUT_PULLUP
}

void readDeck2DigitalButtons()
{
    // Button Deck
    for (byte columnIndex = 0; columnIndex < deck2ColumnCount; columnIndex++)                                // Iterate through each of the column pins.
    {
        byte currentColumn = deck2Columns[columnIndex];                                                      // Hold the currently selected column pin in a variable.
        pinMode(currentColumn, OUTPUT);                                                                 // Set that column pin to OUTPUT mode and...
        digitalWrite(currentColumn, LOW);                                                               // set the pin state to LOW turning it into a temporary ground.
        for (byte rowIndex = 0; rowIndex < deck2RowCount; rowIndex++)                                        // Now iterate through each of the row pins that are connected to the current column pin.
        {                       
            byte currentRow = deck2Rows[rowIndex];                                                           // Hold the currently selected row pin in a variable.
            pinMode(currentRow, INPUT_PULLUP);                                                          // Set that row pin to INPUT_PULLUP mode (+3.3V / HIGH).
            byte buttonNumber = columnIndex + (rowIndex * deck2ColumnCount);                                 // Assign this location in the matrix a unique number.
            delayMicroseconds(25);                                                                      // Delay to give the pin modes time to change state (false readings are caused otherwise).
            byte buttonState = !digitalRead(currentRow);                                                // Invert reading due to INPUT_PULLUP, and store the currently selected pin state.
            if (buttonState == HIGH && (millis() - deck2ActiveButtonsTime[buttonNumber]) > debounceTime)     // If button is active and passes debounce
            {
                deck2ActiveButtons[buttonNumber] = 1;                                                        // write a 1 to the storage variable
                deck2ActiveButtonsTime[buttonNumber] = millis();                                             // and save the last button press time for later debounce comparison.
                if (buttonNumber == 0 || buttonNumber == 2 || buttonNumber == 4 || buttonNumber > 9) { loopInputDetected = HIGH; }  // Activate loopInputDetected variable for looper ignoring non recorded buttons
            }
            if (buttonState == LOW)
            {
                deck2ActiveButtons[buttonNumber] = 0;                                                        // Or if the button is inactive, write a 0.
            }
            pinMode(currentRow, INPUT);                                                                 // Set the selected row pin back to INPUT mode (0V / LOW).
        }
        pinMode(currentColumn, INPUT);                                                                  // Set the selected column pin back to INPUT mode (0V / LOW) and move onto the next column pin.
    }
    // Foot Pedal
    velocityPedalState = !digitalRead(velocityPedalPin);                                                // Invert the reading due to INPUT_PULLUP
}

void readAnalogKnobs()
{
    LLPotValue = analogRead(LLPotPin);                                                                  // Store full 10 bit value
    LMPotValue = analogRead(LMPotPin);                                                                  // Store full 10 bit value
    RMPotValue = map(analogRead(RMPotPin), 0, 1023, 0, 127);                                            // Map to a 7 bit value for MIDI
    RRPotValue = map(analogRead(RRPotPin), 0, 1023, 0, 127);                                            // Map to a 7 bit value for MIDI
}

void runControlModule()
{
    // Analog MIDI CC Knobs
    if (LLPotValue > (analogDeadzone * 6))
    {
        if ((LLPotValue > (previousLLPotValue + (analogDeadzone * 6)) || LLPotValue < (previousLLPotValue - (analogDeadzone * 6)))  // Deadzone to squelch noise * 6 due to greater range
           && ((millis() - previousLLPotTime) > analogDelayTime) )                                                     // Rate limit analog readings in case we are sending values to MIDI so that we don't overrun the buffer
        {
           metronomeSpeed = map(LLPotValue, (analogDeadzone * 6), 1023, metronomeMax, metronomeMin);                   // Map metronome speed to defined boundaries
           previousLLPotValue = LLPotValue;                                                                            // Update "previous" variable for comparison on next loop
           previousLLPotTime = millis();                                                                               // Update "previous" variable for comparison on next loop
        }
    }
    if (LLPotValue <= (analogDeadzone * 6))
    {
        metronomeSpeed = 0;
    }


    if ((LMPotValue > (previousLMPotValue + (analogDeadzone * 6)) || LMPotValue < (previousLMPotValue - (analogDeadzone * 6)))  // Deadzone to squelch noise * 6 due to greater range
        && ((millis() - previousLMPotTime) > analogDelayTime) )                                                     // Rate limit analog readings in case we are sending values to MIDI so that we don't overrun the buffer
    {
        looperVelocity = map(LMPotValue, 0, 1023, 0, 2000);
        previousLMPotValue = LMPotValue;                                                                            // Update "previous" variable for comparison on next loop
        previousLMPotTime = millis();                                                                               // Update "previous" variable for comparison on next loop
    }


    if ((RMPotValue > (previousRMPotValue + analogDeadzone) || RMPotValue < (previousRMPotValue - analogDeadzone))  // Deadzone to squelch noise
        && ((millis() - previousRMPotTime) > analogDelayTime) )                                                     // Rate limit analog readings in case we are sending values to MIDI so that we don't overrun the buffer
    {
        if (modulationState == HIGH && previousModulationState == HIGH)                                             // If the modulation button is currently being held, allow updating
        {
            controlChange(midiChannel, 1, constrain(RMPotValue, 0, 127));                                           // Send modulation (MIDI CC #1) - Constrain to 7 bit value
        }
        previousRMPotValue = RMPotValue;                                                                            // Update "previous" variable for comparison on next loop
        previousRMPotTime = millis();                                                                               // Update "previous" variable for comparison on next loop
    }
    if ((RRPotValue > (previousRRPotValue + analogDeadzone) || RRPotValue < (previousRRPotValue - analogDeadzone))  // Deadzone to squelch noise
        && ((millis() - previousRRPotTime) > analogDelayTime) )                                                     // Rate limit analog readings in case we are sending values to MIDI so that we don't overrun the buffer
    {
        velocity = constrain(RRPotValue, 0, 127);                                                                   // Velocity level - Constrain to 7-bit value
        previousRRPotValue = RRPotValue;                                                                            // Update "previous" variable for comparison on next loop
        previousRRPotTime = millis();                                                                               // Update "previous" variable for comparison on next loop
    }
    // Digital Buttons
    for (int buttonNumber = 0; buttonNumber < 10; buttonNumber++)                                                   // Limit to the 10 buttons in the control panel
    {
        if (deck1ActiveButtons[buttonNumber] != deck1PreviousActiveButtons[buttonNumber])                                     // Compare current button state to the previous state, and if a difference is found...
        {
            if (deck1ActiveButtons[buttonNumber] == 1)                                                                   // If the buttons is active
            {
                if (buttonNumber == 0) { pitchDownState     = HIGH; }                                               // Set the state to HIGH (active)
                if (buttonNumber == 1) { loopRecordTrigger  = HIGH; }
                if (buttonNumber == 2) { modulationState    = HIGH; }
                if (buttonNumber == 3) { loopOverdubState   = HIGH; loopOverdubTrigger = HIGH; }                    // Secondary state tracking to blip overdub button if held through a looper rollover 
                if (buttonNumber == 4) { pitchUpState       = HIGH; }
                if (buttonNumber == 5) { metaKeyState       = HIGH; }
                if (buttonNumber == 6 && deck1ActiveButtons[1] == 0 && deck1ActiveButtons[3] == 0) { channelUpState     = HIGH; } // Disallow channel changes while recording to loop
                if (buttonNumber == 7 && deck1ActiveButtons[1] == 0 && deck1ActiveButtons[3] == 0) { channelDownState   = HIGH; } // Disallow channel changes while recording to loop
                if (buttonNumber == 8) { octaveUpState      = HIGH; }
                if (buttonNumber == 9) { octaveDownState    = HIGH; }
                deck1PreviousActiveButtons[buttonNumber] = 1;                                                            // Update the "previous" variable for comparison next loop
            }
            if (deck1ActiveButtons[buttonNumber] == 0)                                                                   // If the button is inactive
            {
                if (buttonNumber == 0) { pitchDownState     = LOW; }                                                // Set the state to LOW (inactive)
                if (buttonNumber == 1) { loopRecordTrigger  = LOW; }
                if (buttonNumber == 2) { modulationState    = LOW; }
                if (buttonNumber == 3) { loopOverdubState   = LOW; loopOverdubTrigger = LOW; }                      // Secondary state tracking to blip overdub button if held through a looper rollover 
                if (buttonNumber == 4) { pitchUpState       = LOW; }
                if (buttonNumber == 5) { metaKeyState       = LOW; }
                if (buttonNumber == 6) { channelUpState     = LOW; }
                if (buttonNumber == 7) { channelDownState   = LOW; }
                if (buttonNumber == 8) { octaveUpState      = LOW; }
                if (buttonNumber == 9) { octaveDownState    = LOW; }
                deck1PreviousActiveButtons[buttonNumber] = 0;                                                            // Update the "previous" variable for conparison next loop
            }
        }
    }

    if (loopRecordTrigger == HIGH && previousLoopRecordTrigger == LOW && loopRecordState == LOW)                    // Secondary state tracking to engage looper on button press, and disengage on subsequent press.
    {                                                                                                               // Prevents the user from needing to hold the record button down while recording the parent loop track.
        previousLoopRecordTrigger = HIGH;                                                                           // The OVERDUB button must still be held, because if that button remains engaged during a loop
        loopRecordState = HIGH;                                                                                     // rollover, the MIDI channel is incremented, and a new overdub track will immediately begin recording.
    }                                                                                                               // Needing to remember to hit THAT button again to disable recording in the heat of the moment is not
    if (loopRecordTrigger == HIGH && previousLoopRecordTrigger == LOW && loopRecordState == HIGH)                   // ideal from a user interface perspective, as tracks will continue incrementing forever otherwise.
    {
        previousLoopRecordTrigger = HIGH;
        loopRecordState = LOW;
    }

}


void runMetronome()
{
    if (metronomeSpeed > 0 && (millis() - previousMetronomeTime) > metronomeSpeed)
    {
        loopNoteOff(9, 56, 0);
        loopNoteOn(9, 56, 63);
        previousMetronomeTime = millis();
    }
}


void runProgramSelect()
{
    if (metaKeyState == HIGH)                                                                                   // If the meta key is held
    {
        if (channelUpState == HIGH && previousChannelUpState == LOW && midiProgram[midiChannel] < 118)          // Keep program in bounds (0-127)
        {
            previousChannelUpState = HIGH;                                                                      // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                             // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                           // Pop any active notes to allow for quick sampling when changing programs
            }
            playNotes();
            midiProgram[midiChannel] = midiProgram[midiChannel] + 10;                                           // Increment program by 10
            programChange(midiChannel, midiProgram[midiChannel]);                                               // Change the program for this channel
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
        }
        if (channelDownState == HIGH && previousChannelDownState == LOW && midiProgram[midiChannel] > 9)        // Keep program in bounds (0-127)
        {
            previousChannelDownState = HIGH;                                                                    // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                             // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                           // Pop any active notes to allow for quick sampling when changing programs
            }
            playNotes();
            midiProgram[midiChannel] = midiProgram[midiChannel] - 10;                                           // Decrement program by 10
            programChange(midiChannel, midiProgram[midiChannel]);                                               // Change the program for this channel
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
        }
        if (channelUpState == HIGH && channelDownState == HIGH && midiProgram[midiChannel] != 0)                // If both keys are pressed simultaneously and program is not 0
        {
            for (int i = 10; i < deck1ElementCount; i++)                                                             // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                           // Pop any active notes to allow for quick sampling when changing programs
            }
            playNotes();
            midiProgram[midiChannel] = 0;                                                                       // Reset program to 0
            programChange(midiChannel, midiProgram[midiChannel]);                                               // Change the program for this channel
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
        }

        if (octaveUpState == HIGH && previousOctaveUpState == LOW && midiProgram[midiChannel] < 127)            // Keep program in bounds (0-127)
        {
            previousOctaveUpState = HIGH;                                                                       // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                             // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                           // Pop any active notes to allow for quick sampling when changing programs
            }
            playNotes();
            midiProgram[midiChannel] = midiProgram[midiChannel] + 1;                                            // Increment program by 1
            programChange(midiChannel, midiProgram[midiChannel]);                                               // Change the program for this channel
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
        }
        if (octaveDownState == HIGH && previousOctaveDownState == LOW && midiProgram[midiChannel] > 0)          // Keep program in bounds (0-127)
        {
            previousOctaveDownState = HIGH;                                                                     // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                             // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                           // Pop any active notes to allow for quick sampling when changing programs
            }
            playNotes();
            midiProgram[midiChannel] = midiProgram[midiChannel] - 1;                                            // Decrement program by 1
            programChange(midiChannel, midiProgram[midiChannel]);                                               // Change the program for this channel
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
        }
        if (octaveUpState == HIGH && octaveDownState == HIGH && midiProgram[midiChannel] != 0)                  // If both keys are pressed simultaneously and program is not 0
        {
            for (int i = 10; i < deck1ElementCount; i++)                                                             // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                           // Pop any active notes to allow for quick sampling when changing programs
            }
            playNotes();
            midiProgram[midiChannel] = 0;                                                                       // Reset program to 0
            programChange(midiChannel, midiProgram[midiChannel]);                                               // Change the program for this channel
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
        }
    }
}

void runMidiCC()
{
    // Velocity Pedal
    if (velocityPedalState == HIGH && previousVelocityPedalState == LOW)
    {
        previousVelocityPedalState = HIGH;                                                                      // Lock input until released
        velocity = 127;                                                                                         // Set max velocity while the pedal is held
    }
    if (velocityPedalState == LOW && previousVelocityPedalState == HIGH)
    {
        previousVelocityPedalState = LOW;                                                                       // Lock input until released
        velocity = constrain(RRPotValue, 0, 127);                                                               // Constrain to 7-bit value
    }
    // Modulation
    if (metaKeyState == LOW)                                                                                    // If no meta keys are held
    {
        if (modulationState == HIGH && previousModulationState == LOW)                                          // and the modulation button is pressed
        {
            previousModulationState = HIGH;                                                                     // Lock input until released
            controlChange(midiChannel, 1, constrain(RMPotValue, 0, 127));                                       // Enable modulation (MIDI CC #1) - Constrain to 7 bit value
        }
        if (modulationState == LOW && previousModulationState == HIGH)                                          // if the modulation button is released
        {
            previousModulationState = HIGH;                                                                     // Lock input until released
            controlChange(midiChannel, 1, 0);                                                                   // Disable modulation (MIDI CC #1) - 7 bit value
        }
    }
    // Reverb
    if (metaKeyState == HIGH)                                                                                   // If meta key is held and the modulation button is pressed
    {
        if (modulationState == HIGH && previousModulationState == LOW && reverbState[midiChannel] == LOW)       // If reverb is currently disabled
        {
            previousModulationState = HIGH;                                                                     // Lock input until released
            controlChange(midiChannel, 91, 79);                                                                 // Enable Reverb (MIDI CC #91) - 7 bit value
            reverbState[midiChannel] = HIGH;                                                                    // Set reverb state to enabled
            // LCD Update Reverb Status
            lcd.setCursor(5,1);
            lcd.print("1");
        }
        if (modulationState == HIGH && previousModulationState == LOW && reverbState[midiChannel] == HIGH)      // If reverb is currently enabled
        {
            previousModulationState = HIGH;                                                                     // Lock input until released
            controlChange(midiChannel, 91, 0);                                                                  // Disable Reverb (MIDI CC #91) - 7 bit value
            reverbState[midiChannel] = LOW;                                                                     // Set reverb state to disabled
            // LCD Update Reverb Status
            lcd.setCursor(5,1);
            lcd.print("0");
        }
    }
}

void runPitchBend()
{
    // Pitch Bend
    if (metaKeyState == LOW)                                                                                                        // If the meta key is not held
    {
        // Pitch bend up
        if (pitchUpState == HIGH && pitchOffset < 8192)                                                                             // If button is pressed - Stay inside max values of 8192 (whole step) or 4096 (half step)
        {
            pitchOffset = pitchOffset + pitchSpeed;                                                                                 // Increment the pitchOffset by the amount of pitchSpeed for this program loop
            pitchBendChange(midiChannel, ((referencePitch - 1) + pitchOffset) & 0x7F, ((referencePitch - 1) + pitchOffset) >> 7);   // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
        }
        if (pitchUpState == LOW && pitchDownState == LOW && pitchOffset > 0)                                                        // If the button is released and not currently at neutral
        {
            pitchOffset = pitchOffset - pitchSpeed;                                                                                 // Decrement the pitchOffset by the amount of pitchSpeed for this program loop
            pitchBendChange(midiChannel, ((referencePitch - 1) + pitchOffset) & 0x7F, ((referencePitch - 1) + pitchOffset) >> 7);   // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
        }
        // Pitch bend down
        if (pitchDownState == HIGH && pitchOffset > -8192)                                                                          // If button is pressed - Stay inside max values of -8192 (whole step) or -4096 (half step)
        {
            pitchOffset = pitchOffset - pitchSpeed;                                                                                 // Decrement the pitchOffset by the amount of pitchSpeed for this program loop
            pitchBendChange(midiChannel, (referencePitch + pitchOffset) & 0x7F, (referencePitch + pitchOffset) >> 7);               // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
        }
        if (pitchDownState == LOW && pitchUpState == LOW && pitchOffset < 0)                                                        // If the button is released and not currently at neutral
        {
            pitchOffset = pitchOffset + pitchSpeed;                                                                                 // Increment the pitchOffset by the amount of pitchSpeed for this program loop
            pitchBendChange(midiChannel, (referencePitch + pitchOffset) & 0x7F, (referencePitch + pitchOffset) >> 7);               // Send a pitchBendChange event with our 14 bit value broken into two 7 bit bytes for the looper
        }
    }
}

void runOctave()
{
    if (metaKeyState == LOW)                                                                        // If the meta key is not held
    {
        if (octaveUpState == HIGH && previousOctaveUpState == LOW && octave < 12)                   // Highest current Wicki-Hayden layout pitch is 94 - Keep pitch in bounds of 7 bit value range (0-127)
        {
            previousOctaveUpState = HIGH;                                                           // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing octave modifier
            }
            for (int i = 10; i < deck2ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck2ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing octave modifier
            }
            playNotes();
            octave = octave + 12;                                                                   // Increment octave modifier
            // LCD Update Octave Info
            if (octave ==  0) { lcd.setCursor(14,1); lcd.print(" 0"); }
            if (octave == 12) { lcd.setCursor(14,1); lcd.write(0); lcd.print("1"); }
            if (octave == 24) { lcd.setCursor(14,1); lcd.write(0); lcd.print("2"); }
            if (octave == -12) { lcd.setCursor(14,1); lcd.write(1);lcd.print("1"); }
            if (octave == -24) { lcd.setCursor(14,1); lcd.write(1);lcd.print("2"); }
        }
        if (octaveDownState == HIGH && previousOctaveDownState == LOW && octave > -11)              // Lowest current Wicki-Hayden layout pitch is 30 - Keep pitch in bounds of 7 bit value range (0-127)
        {
            previousOctaveDownState = HIGH;                                                         // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing octave modifier
            }
            for (int i = 10; i < deck2ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck2ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing octave modifier
            }
            playNotes();
            octave = octave - 12;                                                                   // Decrement octave modifier
            // LCD Update Octave Info
            if (octave ==  0) { lcd.setCursor(14,1); lcd.print(" 0"); }
            if (octave == 12) { lcd.setCursor(14,1); lcd.write(0); lcd.print("1"); }
            if (octave == 24) { lcd.setCursor(14,1); lcd.write(0); lcd.print("2"); }
            if (octave == -12) { lcd.setCursor(14,1); lcd.write(1);lcd.print("1"); }
            if (octave == -24) { lcd.setCursor(14,1); lcd.write(1);lcd.print("2"); }
        }
        if (octaveUpState == HIGH && octaveDownState == HIGH && octave != 0)                        // If both keys are pressed simultaneously and octave is not default
        {
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing octave modifier
            }
            for (int i = 10; i < deck2ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck2ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing octave modifier
            }
            playNotes();
            octave = 0;                                                                             // Reset octave modifier to 0
            // LCD Update Octave Info
            if (octave ==  0) { lcd.setCursor(14,1); lcd.print(" 0"); }
            if (octave == 12) { lcd.setCursor(14,1); lcd.write(0); lcd.print("1"); }
            if (octave == 24) { lcd.setCursor(14,1); lcd.write(0); lcd.print("2"); }
            if (octave == -12) { lcd.setCursor(14,1); lcd.write(1);lcd.print("1"); }
            if (octave == -24) { lcd.setCursor(14,1); lcd.write(1);lcd.print("2"); }
        }
    }
}

void runChannelSelect()
{
    if (metaKeyState == LOW)                                                                        // If the meta key is not held
    {
        if (channelUpState == HIGH && previousChannelUpState == LOW && midiChannel < 8)             // Keep channel in bounds (0-15) and skip drums channel (9)
        {
            previousChannelUpState = HIGH;                                                          // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing channel
            }
            playNotes();
            midiChannel = midiChannel + 1;                                                          // Increment channel
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }
        if (channelUpState == HIGH && previousChannelUpState == LOW && midiChannel == 8)            // Keep channel in bounds (0-15) and skip drums channel (9)
        {
            previousChannelUpState = HIGH;                                                          // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing channel
            }
            playNotes();
            midiChannel = midiChannel + 2;                                                          // Increment channel
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }
        if (channelUpState == HIGH && previousChannelUpState == LOW && midiChannel > 9 && midiChannel < 15) // Keep channel in bounds (0-15) and skip drums channel (9)
        {
            previousChannelUpState = HIGH;                                                                  // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                         // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                       // Pop any active notes to prevent hangs when abruptly changing channel
            }
            playNotes();
            midiChannel = midiChannel + 1;                                                                  // Increment channel
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }

        if (channelDownState == HIGH && previousChannelDownState == LOW && midiChannel > 0 && midiChannel < 9)  // Keep channel in bounds (0-15) and skip drums channel (9)
        {
            previousChannelDownState = HIGH;                                                                    // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                             // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                                           // Pop any active notes to prevent hangs when abruptly changing channel
            }
            playNotes();
            midiChannel = midiChannel - 1;                                                                      // Decrement channel
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }
        if (channelDownState == HIGH && previousChannelDownState == LOW && midiChannel == 10)       // Keep channel in bounds (0-15) and skip drums channel (9)
        {
            previousChannelDownState = HIGH;                                                        // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing channel
            }
            playNotes();
            midiChannel = midiChannel - 2;                                                          // Decrement channel
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }
        if (channelDownState == HIGH && previousChannelDownState == LOW && midiChannel > 10)        // Keep channel in bounds (0-15) and skip drums channel (9)
        {
            previousChannelDownState = HIGH;                                                        // Lock input until released
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing channel
            }
            playNotes();
            midiChannel = midiChannel - 1;                                                          // Decrement channel
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }

        if (channelUpState == HIGH && channelDownState == HIGH && midiChannel != 0)                 // If both keys are pressed simultaneously and channel is not default 0
        {
            for (int i = 10; i < deck1ElementCount; i++)                                                 // For all note buttons in the deck
            {
                deck1ActiveButtons[i] = 0;                                                               // Pop any active notes to prevent hangs when abruptly changing channel
            }
            playNotes();
            midiChannel = 0;                                                                        // Reset channel to 0
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }
    }
}

void runDrumsMode()
{
    if (metaKeyState == HIGH)                                                                                                   // If meta key is held and pitch down is pressed
    {
        if (pitchDownState == HIGH && previousPitchDownState == LOW && drumsModeEnabled == LOW && sustainModeEnabled == LOW)    // If drums mode is currently disabled
        {
            previousPitchDownState = HIGH;                                                                                      // Lock input until released
            drumsModeEnabled = HIGH;                                                                                            // Enable drums mode
            midiChannel = 9;                                                                                                    // Set the current channel to drums (MIDI channel 9)
            programChange(midiChannel, midiProgram[midiChannel]);                                                               // Change the program to what was saved for this channel
            // LCD Update Current Mode Selection
            lcd.setCursor(3,1);
            lcd.print("D");
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            lcd.print("10");

            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }
        }

        if (pitchDownState == HIGH && previousPitchDownState == LOW && drumsModeEnabled == HIGH)    // If drums mode is currently enabled
        {
            previousPitchDownState = HIGH;                                                          // Lock input until released
            drumsModeEnabled = LOW;                                                                 // Disable drums mode
            midiChannel = 0;                                                                        // Set the current channel back to default (MIDI channel 0)
            programChange(midiChannel, midiProgram[midiChannel]);                                   // Change the program back to what was saved for this channel
            // LCD Update Current Mode Selection
            lcd.setCursor(3,1);
            lcd.print("N");
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            lcd.print("01");

            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)           // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)           // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                                       // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                                       // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                                // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }
        }
    }
}

void runSustainMode()
{
    if (metaKeyState == HIGH)                                                                                                   // If meta key is held and pitch up is pressed
    {
        if (pitchUpState == HIGH && previousPitchUpState == LOW && sustainModeEnabled == LOW && drumsModeEnabled == LOW)        // If sustain mode is currently disabled
        {
            previousPitchUpState = HIGH;                                                                                        // Lock input until released
            sustainModeEnabled = HIGH;                                                                                          // Enable sustain mode
            // LCD Update Current Mode Selection
            lcd.setCursor(3,1);
            lcd.print("S");
        }
        if (pitchUpState == HIGH && previousPitchUpState == LOW && sustainModeEnabled == HIGH)                                  // If sustain mode is currently enabled
        {
            controlChange(midiChannel, 64, 0);                                                                                  // Send a sustain off command (MIDI CC #64)  - 0-63 OFF / 64-127 ON
            previousPitchUpState = HIGH;                                                                                        // Lock input until released
            sustainModeEnabled = LOW;                                                                                           // Disable sustain mode
            // LCD Update Current Mode Selection
            lcd.setCursor(3,1);
            lcd.print("N");
        }
    }
}

void runControllerReset()
{
    if (metaKeyState == HIGH && channelDownState == HIGH && octaveUpState == HIGH)          // If the 3 key reset command is pressed
    {
        for (int myChannel = 0; myChannel <= 15; myChannel++)                               // For MIDI channels 0 through 15
        {
            controlChange(myChannel, 120, 0);                                               // All Sound Off (MIDI CC #120) - Mute all currently sounding notes
            programChange(myChannel, 0);                                                    // Reset program on all channels to 0
            controlChange(myChannel, 91, 0);                                                // Disable reverb on all channels (MIDI CC #91)
            controlChange(myChannel, 64, 0);                                                // Disable sustain on all channels (MIDI CC #64)
        }
        // LCD Controller Reset Message
        lcd.setCursor(0,0);                                                                 // Set cursor to column 0 line 0
        lcd.print("   CONTROLLER   ");                                                      // Print the top line
        lcd.setCursor(0,1);                                                                 // Set cursor to column 0 line 0
        lcd.print("     RESET      ");                                                      // Print the bottom line
        delay(500);                                                                         // Delay to allow the message to be read
        lcd.clear();                                                                        // Clear the LCD screen
        WRITE_RESTART(0x5FA0004);                                                           // Write value to memory location to trigger a restart
    }
}

void playNotes()
{
    // Deck 1
    for (int i = 0; i < deck1ElementCount; i++)                                                  // For all buttons in the deck
    {
        if (deck1ActiveButtons[i] != deck1PreviousActiveButtons[i])                                   // If a change is detected
        {
            if (deck1ActiveButtons[i] == 1)                                                      // If the button is active
            {
                if (sustainModeEnabled == HIGH)                                             // If sustain mode is enabled, blip sustain
                {
                    controlChange(midiChannel, 64, 0);                                      // Disable sustain on the current channel
                    controlChange(midiChannel, 64, 127);                                    // Reenable sustain on the current channel
                }
                if (drumsModeEnabled == HIGH)                                               // If drums mode is enabled
                {
                    noteOn(midiChannel, drumLayout[i], velocity);                           // Send a noteOn using drum layout with no octave modifier
                }
                if (drumsModeEnabled == LOW)                                                // If drums mode is disabled
                {
                    noteOn(midiChannel, deck1WickiHaydenLayout[i] + octave, velocity);           // Send a noteOn using the Wicki-Hayden layout
                }
                deck1PreviousActiveButtons[i] = 1;                                               // Update the "previous" variable for comparison on next loop
            }
            if (deck1ActiveButtons[i] == 0)                                                      // If the button is inactive
            {
                if (drumsModeEnabled == HIGH)                                               // If drums mode is enabled
                {
                    noteOff(midiChannel, drumLayout[i], 0);                                 // Send a noteOff using drum layout with no octave modifier
                }
                if (drumsModeEnabled == LOW)                                                // If drums mode is disabled
                {
                    noteOff(midiChannel, deck1WickiHaydenLayout[i] + octave, 0);                 // Send a noteOff using the Wicki-Hayden layout
                }
                deck1PreviousActiveButtons[i] = 0;                                               // Update the "previous" variable for comparison on next loop
            }
        }
    }
    // Deck 2
    for (int i = 0; i < deck2ElementCount; i++)                                                  // For all buttons in the deck
    {
        if (deck2ActiveButtons[i] != deck2PreviousActiveButtons[i])                                   // If a change is detected
        {
            if (deck2ActiveButtons[i] == 1)                                                      // If the button is active
            {
                if (sustainModeEnabled == HIGH)                                             // If sustain mode is enabled, blip sustain
                {
                    controlChange(midiChannel + 1, 64, 0);                                      // Disable sustain on the current channel
                    controlChange(midiChannel + 1, 64, 127);                                    // Reenable sustain on the current channel
                }
                if (drumsModeEnabled == HIGH)                                               // If drums mode is enabled
                {
                    noteOn(midiChannel + 1, deck2WickiHaydenLayout[i] + octave, velocity);                           // Send a noteOn using drum layout with no octave modifier
                }
                if (drumsModeEnabled == LOW && midiChannel != 8)                                                // If drums mode is disabled
                {
                    noteOn(midiChannel + 1, deck2WickiHaydenLayout[i] + octave, velocity);           // Send a noteOn using the Wicki-Hayden layout
                }
                if (drumsModeEnabled == LOW && midiChannel == 8)                                                // If drums mode is disabled
                {
                    noteOn(midiChannel + 2, deck2WickiHaydenLayout[i] + octave, velocity);           // Send a noteOn using the Wicki-Hayden layout
                }
                deck2PreviousActiveButtons[i] = 1;                                               // Update the "previous" variable for comparison on next loop
            }
            if (deck2ActiveButtons[i] == 0)                                                      // If the button is inactive
            {
                if (drumsModeEnabled == HIGH)                                               // If drums mode is enabled
                {
                    noteOff(midiChannel + 1, deck2WickiHaydenLayout[i] + octave, 0);                                 // Send a noteOff using drum layout with no octave modifier
                }
                if (drumsModeEnabled == LOW && midiChannel != 8)                                                // If drums mode is disabled
                {
                    noteOff(midiChannel + 1, deck2WickiHaydenLayout[i] + octave, 0);                 // Send a noteOff using the Wicki-Hayden layout
                }
                if (drumsModeEnabled == LOW && midiChannel == 8)                                                // If drums mode is disabled
                {
                    noteOff(midiChannel + 2, deck2WickiHaydenLayout[i] + octave, 0);           // Send a noteOn using the Wicki-Hayden layout
                }
                deck2PreviousActiveButtons[i] = 0;                                               // Update the "previous" variable for comparison on next loop
            }
        }
    }

}

void runLooper()
{   // Here be dragons...

    // Stop/restart playback
    if (metaKeyState == HIGH && loopOverdubState == HIGH && previousLoopOverdubState == LOW && loopPlaybackEnabled == HIGH && loopInMemory == HIGH) // If playback is currently enabled, stop it
    {
        previousLoopOverdubState = HIGH;                                                                                    // Lock input until released
        loopPlaybackEnabled = LOW;                                                                                          // Disable loop playback
        for (int myTrack = 0; myTrack < loopMaxTracks; myTrack++)                                                           // For all looper tracks
        {
            for (int myIndex = loopPlaybackIndex[myTrack]; myIndex < loopRecordingIndex[myTrack]; myIndex++)                // From the current playback index forward
            {
                if (loopPacketByte0[myTrack][myIndex] == 0)                                                                 // If packet type is noteOff
                {
                    loopNoteOff(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);             // Spool out remaining noteOffs to close notes and prevent hangs
                }
            }
        }
        // LCD Update Looper
        lcd.setCursor(0,1);
        lcd.write(4);lcd.write(5);                                                                                          // Write left and right side of pause glyph to LCD
    }
    if (metaKeyState == HIGH && loopOverdubState == HIGH && previousLoopOverdubState == LOW && loopPlaybackEnabled == LOW && loopInMemory == HIGH)  // If playback is currently disabled, start it
    {
        previousLoopOverdubState = HIGH;                                                                                    // Lock input until released
        loopPlaybackEnabled = HIGH;                                                                                         // Enable loop playback
        loopStartTimestamp = currentTime;                                                                                   // Reset the loopStartTimestamp to restart playback
        memset(loopPlaybackIndex, 0, sizeof(loopPlaybackIndex));                                                            // Return playback index to 0 for all tracks
    }

    // Loop index overflow detection
    if (loopRecordState == HIGH && loopInMemory == HIGH)
    {
        // If the end of the array is reached, force a reset
        for (int myTrack = 0; myTrack < loopMaxTracks; myTrack++)                                                           // For all looper tracks
        {
            if (loopRecordingIndex[myTrack] >= loopMaxIndexes)                                                              // If the max index was reached
            {
                loopRecordingEnabled = LOW;                                                                                 // Disable loop packet recording
                loopPlaybackEnabled = LOW;                                                                                  // Disable playback
                loopInMemory = LOW;                                                                                         // Mark loop free for writing
                loopInputDetected = LOW;                                                                                    // Reset loop input detection variable
                loopDuration = 0;                                                                                           // Clear loop duration
                loopStartTimestamp = 0;                                                                                     // Clear loop start timestamp
                memset(loopRecordingIndex, 0, sizeof(loopRecordingIndex));                                                  // Flatten all looper arrays
                memset(loopPlaybackIndex, 0, sizeof(loopPlaybackIndex));                                                    // Flatten all looper arrays
                memset(loopPacketEventTime, 0, sizeof(loopPacketEventTime));                                                // Flatten all looper arrays
                memset(loopPacketByte0, 0, sizeof(loopPacketByte0));                                                        // Flatten all looper arrays
                memset(loopPacketByte1, 0, sizeof(loopPacketByte1));                                                        // Flatten all looper arrays
                memset(loopPacketByte2, 0, sizeof(loopPacketByte2));                                                        // Flatten all looper arrays
                memset(loopTrackActive, 0, sizeof(loopTrackActive));                                                        // Flatten all looper arrays
                loopParentChannel = 255;                                                                                    // Reset loop parent channel
                lcd.setCursor(0,0);                                                                                         // Set cursor to column 0 line 0
                lcd.print(" INDEX OVERFLOW ");                                                                              // Print the top line
                delay(1500);                                                                                                // Delay to allow the message to be read
                // LCD Update Top Menu
                lcd.setCursor(0,0);
                lcd.print("LP M R PRG CH OC");
                // LCD Update Looper
                lcd.setCursor(0,1);
                lcd.print("--");
            }
        }
    }

    // Step 1a - Recording is enabled and there is no loop already in memory - Initialize variables
    if (loopRecordState == HIGH && previousLoopRecordState == LOW && loopInMemory == LOW)
    {
        previousLoopRecordState = HIGH;                                                                                     // Lock input until released
        loopRecordingEnabled = HIGH;                                                                                        // Enable loop packet recording
        loopPlaybackEnabled = LOW;                                                                                          // Disable playback
        loopInMemory = LOW;                                                                                                 // Mark loop free for writing
        loopInputDetected = LOW;                                                                                            // Reset loop input detection variable
        loopDuration = 0;                                                                                                   // Clear loop duration
        loopStartTimestamp = 0;                                                                                             // Clear loop start timestamp
        memset(loopRecordingIndex, 0, sizeof(loopRecordingIndex));                                                          // Flatten all looper arrays
        memset(loopPlaybackIndex, 0, sizeof(loopPlaybackIndex));                                                            // Flatten all looper arrays
        memset(loopPacketEventTime, 0, sizeof(loopPacketEventTime));                                                        // Flatten all looper arrays
        memset(loopPacketByte0, 0, sizeof(loopPacketByte0));                                                                // Flatten all looper arrays
        memset(loopPacketByte1, 0, sizeof(loopPacketByte1));                                                                // Flatten all looper arrays
        memset(loopPacketByte2, 0, sizeof(loopPacketByte2));                                                                // Flatten all looper arrays
        memset(loopTrackActive, 0, sizeof(loopTrackActive));                                                                // Flatten all looper arrays
        loopParentChannel = 255;                                                                                            // Reset loop parent channel
        // LCD Update Looper
        lcd.setCursor(0,1);
        lcd.print("??");

    }

    // Step 2a - Loop start detection -  Located in main program loop (needs to sit between readDigitalButtons() and playNotes() functions)

    // Step 2b - Recording was disabled with no notes having been pressed
    if (loopRecordState == LOW && previousLoopRecordState == HIGH && loopInMemory == LOW)
    {
        // LCD Update Looper
        lcd.setCursor(0,1);
        lcd.print("--");

        // LCD Update Loop Track status
        lcd.setCursor(10,1);
        lcd.print(" ");
        lcd.setCursor(13,1);
        lcd.print(" ");
    }

    // Step 3a - Recording is deactivated and there is data in memory
    if (loopRecordState == LOW && previousLoopRecordState == HIGH && loopInMemory == HIGH)
    {
        previousLoopRecordState = LOW;                                          // Lock input until released

        // In case a note button was still being held while the record button was released
        for (int i = 0; i < deck1ElementCount; i++)                                  // For all buttons in the deck
        {
            if (deck1ActiveButtons[i] == 1 && i != 1 && i != 3 && i != 5 && i != 6 && i != 7 && i != 8 && i != 9)    // If a recordable button is active
            {
                if (drumsModeEnabled == HIGH)                                   // If drums mode is enabled
                {
                    noteOff(midiChannel, drumLayout[i], 0);                     // Send a noteOff using drum layout with no octave modifier
                }
                if (drumsModeEnabled == LOW)                                    // If drums mode is disabled
                {
                    noteOff(midiChannel, deck1WickiHaydenLayout[i] + octave, 0);     // Send a noteOff using the Wicki-Hayden layout
                }
            }
        }
        loopDuration = (currentTime - loopStartTimestamp);                      // Set loop duration
        loopStartTimestamp = currentTime;                                       // Set loop start time to current time to prepare for a new playback loop
        loopRecordingEnabled = LOW;                                             // Disable loop packet recording
        loopPlaybackEnabled = HIGH;                                             // Enable loop playback
        loopTrackActive[midiChannel] = HIGH;                                    // Mark track as active so that playback packets aren't recorded again
        loopParentChannel = midiChannel;                                        // Designate loop parent channel

        // LCD Update Loop Track status
        lcd.setCursor(10,1);
        lcd.print("(");
        lcd.setCursor(13,1);
        lcd.print(")");

    }


    // Step 3b - Loop overdub is enabled before the parent loop has finished recording.  Close the loop, and immediately start overdub recording on the next higher channel.
    if (loopOverdubState == HIGH && previousLoopOverdubState == LOW && loopRecordState == HIGH && previousLoopRecordState == HIGH && loopInMemory == HIGH)
    {
        previousLoopOverdubState = LOW;                                          // Lock input until released
        loopRecordState = LOW;
        previousLoopRecordState = LOW;

        // In case a note button was still being held while the record button was released
        for (int i = 0; i < deck1ElementCount; i++)                                  // For all buttons in the deck
        {
            if (deck1ActiveButtons[i] == 1 && i != 1 && i != 3 && i != 5 && i != 6 && i != 7 && i != 8 && i != 9)    // If a recordable button is active
            {
                if (drumsModeEnabled == HIGH)                                   // If drums mode is enabled
                {
                    noteOff(midiChannel, drumLayout[i], 0);                     // Send a noteOff using drum layout with no octave modifier
                }
                if (drumsModeEnabled == LOW)                                    // If drums mode is disabled
                {
                    noteOff(midiChannel, deck1WickiHaydenLayout[i] + octave, 0);     // Send a noteOff using the Wicki-Hayden layout
                }
            }
        }
        loopDuration = (currentTime - loopStartTimestamp);                      // Set loop duration
        loopStartTimestamp = currentTime;                                       // Set loop start time to current time to prepare for a new playback loop
        loopRecordingEnabled = LOW;                                             // Disable loop packet recording
        loopPlaybackEnabled = HIGH;                                             // Enable loop playback
        loopTrackActive[midiChannel] = HIGH;                                    // Mark track as active so that playback packets aren't recorded again
        loopParentChannel = midiChannel;                                        // Designate loop parent channel

        // LCD Update Loop Track status
        lcd.setCursor(10,1);
        lcd.print("(");
        lcd.setCursor(13,1);
        lcd.print(")");

        // Increment MIDI Channel
        if ((midiChannel < 8 || midiChannel > 9) && midiChannel < 15)           // Don't auto increment into the drums channel
        {
            midiChannel = midiChannel + 1;
        }
        if (midiChannel == 8)
        {
            midiChannel = midiChannel + 2;
        }
        if (midiChannel == 9)                                                   // If current track IS the drums channel
        {
            drumsModeEnabled = LOW;                                             // Disable drums mode
            midiChannel = 0;                                                    // Go directly to channel 0 instead of incrementing to channel 10
            programChange(midiChannel, midiProgram[midiChannel]);               // Change the program back to what was saved for this channel
            // LCD Update Current Mode Selection
            lcd.setCursor(3,1);
            lcd.print("N");
            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            lcd.print("01");

            if (loopTrackActive[midiChannel] == HIGH)
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == LOW)
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }
        }
        if (midiChannel == 15)                                                  // If MIDI channel 15, wrap back around to 0
        {
            midiChannel = 0;
        }

        // LCD Update MIDI Channel Info
        lcd.setCursor(11,1);
        if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
        if (midiChannel > 9) { lcd.print(midiChannel + 1); }
        // LCD Update MIDI Program Info
        lcd.setCursor(7,1);
        if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
        if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
        if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
        // LCD Update Reverb State
        lcd.setCursor(5,1);
        if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
        if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

        if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)   // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
        {
            // LCD Update Loop Track status
            lcd.setCursor(10,1);
            lcd.print("(");
            lcd.setCursor(13,1);
            lcd.print(")");
        }
        if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)   // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
        {
            // LCD Update Loop Track status
            lcd.setCursor(10,1);
            lcd.write(6);                                                               // Left side of loop track active glyph
            lcd.setCursor(13,1);
            lcd.write(7);                                                               // Right side of loop track active glyph
        }
        if (loopTrackActive[midiChannel] == LOW)                                        // If looper is inactive on this channel, clear the indicators at the cursor position.
        {
            // LCD Update Loop Track status
            lcd.setCursor(10,1);
            lcd.print(" ");
            lcd.setCursor(13,1);
            lcd.print(" ");
        }


        if (previousLoopOverdubState == LOW)                                                                                            // Reinit loop to clear playback.  Broken out here to allow for percentage checking below.
        {
            previousLoopOverdubState = HIGH;                                                                                            // Lock input until released

            for (int myIndex = loopPlaybackIndex[midiChannel]; myIndex < loopRecordingIndex[midiChannel]; myIndex++)                    // From the current playback index forward
            {
                if (loopPacketByte0[midiChannel][myIndex] == 0)                                                                         // If the packet type is 0 = noteOff
                {
                    loopNoteOff(midiChannel, loopPacketByte1[midiChannel][myIndex], loopPacketByte2[midiChannel][myIndex]);             // Spool out remaining noteOffs to close notes and prevent hangs
                }
            }

            loopRecordingIndex[midiChannel] = 0;                                        // Reset recording index for current track to 0
            loopPlaybackIndex[midiChannel] = 0;                                         // Reset playback index for current track to 0
            for (int myIndex = 0; myIndex < loopMaxIndexes; myIndex++)                  // For all indexes allocated to a looper track
            {
                loopPacketEventTime[midiChannel][myIndex] = 0;                          // Reset event time for current track to 0
                loopPacketByte0[midiChannel][myIndex] = 0;                              // Reset event type for current track to 0
                loopPacketByte1[midiChannel][myIndex] = 0;                              // Reset pitch/control/highByte for current track to 0
                loopPacketByte2[midiChannel][myIndex] = 0;                              // Reset velocity/value/lowByte for current track to 0
            }

            loopTrackActive[midiChannel] = LOW;                                         // Mark track available for recording

            // LCD Update Loop Track status
            lcd.setCursor(10,1);
            lcd.print(" ");
            lcd.setCursor(13,1);
            lcd.print(" ");
        }
        loopRecordingEnabled = HIGH;                                                    // Enable loop packet recording

    }


    // Step 4 - Play the loop repeatedly
    if (loopPlaybackEnabled == HIGH && loopInMemory == HIGH)
    {
        if ((loopStartTimestamp + loopDuration) >= currentTime)                                                                     // If we are still inside of the current loop duration
        {
            for (int myTrack = 0; myTrack < loopMaxTracks; myTrack++)                                                               // For all looper tracks
            {
                if (loopTrackActive[myTrack] == HIGH)
                {
                    for (int myIndex = loopPlaybackIndex[myTrack]; myIndex < loopRecordingIndex[myTrack]; myIndex++)                // For all indexes from current the playback index forward
                    {
                        if (loopPacketEventTime[myTrack][myIndex] <= (currentTime - loopStartTimestamp))                            // If the packet event time is right now, or is in the past
                        {
                            if (loopPacketByte0[myTrack][myIndex] == 1)                                                             // If the packet type is 1 = noteOn
                            {                                                                                                       
                                loopNoteOn(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);          // Send a noteOn bypassing the normal function (needed for multitrack overdubbing)
                                loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                        // Increment the playback index
                            }
                            if (loopPacketByte0[myTrack][myIndex] == 0)                                                             // If the packet type is 0 = noteOff
                            {                                                                                                       
                                loopNoteOff(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);         // Send a noteOff bypassing the normal function (needed for multitrack overdubbing)
                                loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                        // Increment the playback index
                            }
                            if (loopPacketByte0[myTrack][myIndex] == 2)                                                             // If the packet type is 2 = controlChange
                            {
                                loopControlChange(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);   // Send a controlChange bypassing the normal function (needed for multitrack overdubbing)
                                loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                        // Increment the playback index
                            }
                            if (loopPacketByte0[myTrack][myIndex] == 3)                                                             // If the packet type is 3 = pitchBendChange
                            {
                                loopPitchBendChange(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]); // Send a pitchBendChange bypassing the normal function (needed for multitrack overdubbing)
                                loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                        // Increment the playback index
                            }
                        }
                    }
                }
            }
            // LCD Looper Playback Percentage
            loopPercentage = ((currentTime - loopStartTimestamp) * 100) / loopDuration;     // Find percentage of loop completion and store in a variable.
            if (loopPercentage < 10 && loopPercentage != previousLoopPercentage)            // If less than 10 (i.e. less than 2 digits), and only if the number has changed.
            {                                                                               // Prevents updating the LCD unnecessarily.
                lcd.setCursor(0,1);
                lcd.print("0");                                                             // Pad a leading "0"
                lcd.print(loopPercentage);
                previousLoopPercentage = loopPercentage;
            }
            if (loopPercentage >= 10 && loopPercentage < 100 && loopPercentage != previousLoopPercentage)   // If two digits
            {
                lcd.setCursor(0,1);
                lcd.print(loopPercentage);                                                                  // Print the actual value
                previousLoopPercentage = loopPercentage;
            }
        }
        else                                                                                                                        // If we have exceeded the current loop duration
        {
            // In case a note button was still being held when overdub reached the end of parent loop duration
            if (loopOverdubState == HIGH && loopRecordingIndex[midiChannel] > 0 && loopParentChannel != midiChannel)
            {
                for (int i = 0; i < deck1ElementCount; i++)                                      // For all buttons in the deck
                {
                    if (deck1ActiveButtons[i] == 1 && i != 1 && i != 3 && i != 5 && i != 6 && i != 7 && i != 8 && i != 9)                // If a recordable button is active
                    {
                        if (drumsModeEnabled == HIGH)                                       // If drums mode is enabled
                        {
                            noteOff(midiChannel, drumLayout[i], 0);                         // Send a noteOff using drum layout with no octave modifier
                        }
                        if (drumsModeEnabled == LOW)                                        // If drums mode is disabled
                        {
                            noteOff(midiChannel, deck1WickiHaydenLayout[i] + octave, 0);         // Send a noteOff using the Wicki-Hayden layout
                        }
                    }
                }
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                               // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                               // Right side of loop track active glyph
            }

            loopOverdubState = LOW;                                                         // Blip the overdub button if it's being held on rollover to immediately start recording a new overdub track

            // Send any potentially remaining unplayed events
            for (int myTrack = 0; myTrack < loopMaxTracks; myTrack++)                                                               // For all looper tracks
            {
                if (loopTrackActive[myTrack] == HIGH)
                {
                    for (int myIndex = loopPlaybackIndex[myTrack]; myIndex < loopRecordingIndex[myTrack]; myIndex++)                // For all indexes from current the playback index forward
                    {
                        if (loopPacketByte0[myTrack][myIndex] == 1)                                                                 // If the packet type is 1 = noteOn
                        {
                            loopNoteOn(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);              // Send a noteOn bypassing the normal function (needed for multitrack overdubbing)
                            loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                            // Increment the playback index
                        }
                        if (loopPacketByte0[myTrack][myIndex] == 0)                                                                 // If the packet type is 0 = noteOff
                        {
                            loopNoteOff(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);             // Send a noteOff bypassing the normal function (needed for multitrack overdubbing)
                            loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                            // Increment the playback index
                        }
                        if (loopPacketByte0[myTrack][myIndex] == 2)                                                                 // If the packet type is 2 = controlChange
                        {
                            loopControlChange(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);       // Send a controlChange bypassing the normal function (needed for multitrack overdubbing)
                            loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                            // Increment the playback index
                        }
                        if (loopPacketByte0[myTrack][myIndex] == 3)                                                                 // If the packet type is 3 = pitchBendChange
                        {
                            loopPitchBendChange(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);     // Send a pitchBendChange bypassing the normal function (needed for multitrack overdubbing)
                            loopPlaybackIndex[myTrack] = loopPlaybackIndex[myTrack] + 1;                                            // Increment the playback index
                        }
                    }
                }
            }

            loopStartTimestamp = currentTime;                                                                                       // Set the loop start timestamp to current time to begin a new trip through the loop

            for (int myTrack = 0; myTrack < loopMaxTracks; myTrack++)
            {
                if (loopRecordingIndex[myTrack] > 0)                                                                                // If loopRecordingIndex is > 0 indicating notes have been recorded
                {
                    loopTrackActive[myTrack] = HIGH;                                                                                // Enable the loop track for playback on next loop
                }
            }
            memset(loopPlaybackIndex, 0, sizeof(loopPlaybackIndex));                                                                // Return playback index to 0 for all tracks

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)   // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)   // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                               // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                               // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                        // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }

        }
    }



    // Step 5b - Loop overdub recording is disabled
    if (loopOverdubState == LOW && previousLoopOverdubState == HIGH && loopInMemory == HIGH && loopPlaybackEnabled == HIGH)
    {
        previousLoopOverdubState = LOW;                                                     // Lock input until released

        if (loopOverdubTrigger == HIGH)                                                     // Enable loop overdub if it's actually being held through loop restart
        {
            loopOverdubState = HIGH;
        }
        // In case a note button was still being held while the overdub button was released
        for (int i = 0; i < deck1ElementCount; i++)                                              // For all buttons in the deck
        {
            if (deck1ActiveButtons[i] == 1 && i != 1 && i != 3 && i != 5 && i != 6 && i != 7 && i != 8 && i != 9)    // If a recordable button is active
            {
                if (drumsModeEnabled == HIGH)                                               // If drums mode is enabled
                {
                    noteOff(midiChannel, drumLayout[i], 0);                                 // Send a noteOff using drum layout with no octave modifier
                }
                if (drumsModeEnabled == LOW)                                                // If drums mode is disabled
                {
                    noteOff(midiChannel, deck1WickiHaydenLayout[i] + octave, 0);                 // Send a noteOff using the Wicki-Hayden layout
                }
            }
        }

        loopRecordingEnabled = LOW;                                                         // Disable loop packet recording


        if (loopTrackActive[midiChannel] == HIGH)                                           // If there is already an active looper track at this channel
        {

            // Increment MIDI Channel
            if ((midiChannel < 8 || midiChannel > 9) && midiChannel < 15)                   // Don't auto increment into the drums channel
            {
                midiChannel = midiChannel + 1;                                              // Increment channel
            }
            if (midiChannel == 8)
            {
                midiChannel = midiChannel + 2;                                              // Increment channel
            }
            if (midiChannel == 9)                                                           // If current track IS the drums channel
            {
                drumsModeEnabled = LOW;                                                     // Disable drums mode
                midiChannel = 0;                                                            // Go directly to channel 0 instead of incrementing to channel 10
                programChange(midiChannel, midiProgram[midiChannel]);                       // Change the program back to what was saved for this channel
                // LCD Update Current Mode Selection
                lcd.setCursor(3,1);
                lcd.print("N");
                // LCD Update MIDI Channel Info
                lcd.setCursor(11,1);
                lcd.print("01");

                if (loopTrackActive[midiChannel] == HIGH)
                {
                    // LCD Update Loop Track status
                    lcd.setCursor(10,1);
                    lcd.print("(");
                    lcd.setCursor(13,1);
                    lcd.print(")");
                }
                if (loopTrackActive[midiChannel] == LOW)
                {
                    // LCD Update Loop Track status
                    lcd.setCursor(10,1);
                    lcd.print(" ");
                    lcd.setCursor(13,1);
                    lcd.print(" ");
                }
            }
            if (midiChannel == 15)                                                          // If MIDI channel 15, wrap back around to 0
            {
                midiChannel = 0;
            }

            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)   // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)   // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                               // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                               // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                        // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }
        }

    }


    // Step 5a - Loop overdub recording is enabled and there is currently a parent loop playing
    if (loopOverdubState == HIGH && loopInMemory == HIGH && loopPlaybackEnabled == HIGH)
    {

        if (midiChannel == loopParentChannel)                                               // Auto increment channel if this is the looper parent channel
        {

            // Increment MIDI Channel
            if ((midiChannel < 8 || midiChannel > 9) && midiChannel < 15)                   // Don't auto increment into the drums channel
            {
                midiChannel = midiChannel + 1;                                              // Increment channel
            }
            if (midiChannel == 8)
            {
                midiChannel = midiChannel + 2;                                              // Increment channel
            }
            if (midiChannel == 9)                                                           // If current track IS the drums channel
            {
                drumsModeEnabled = LOW;                                                     // Disable drums mode
                midiChannel = 0;                                                            // Go directly to channel 0 instead of incrementing to channel 10
                programChange(midiChannel, midiProgram[midiChannel]);                       // Change the program back to what was saved for this channel
                // LCD Update Current Mode Selection
                lcd.setCursor(3,1);
                lcd.print("N");
                // LCD Update MIDI Channel Info
                lcd.setCursor(11,1);
                lcd.print("01");

                if (loopTrackActive[midiChannel] == HIGH)
                {
                    // LCD Update Loop Track status
                    lcd.setCursor(10,1);
                    lcd.print("(");
                    lcd.setCursor(13,1);
                    lcd.print(")");
                }
                if (loopTrackActive[midiChannel] == LOW)
                {
                    // LCD Update Loop Track status
                    lcd.setCursor(10,1);
                    lcd.print(" ");
                    lcd.setCursor(13,1);
                    lcd.print(" ");
                }
            }
            if (midiChannel == 15)                                                          // If MIDI channel 15, wrap back around to 0
            {
                midiChannel = 0;
            }

            // LCD Update MIDI Channel Info
            lcd.setCursor(11,1);
            if (midiChannel < 9) { lcd.print("0"); lcd.print(midiChannel + 1); }
            if (midiChannel > 9) { lcd.print(midiChannel + 1); }
            // LCD Update MIDI Program Info
            lcd.setCursor(7,1);
            if (midiProgram[midiChannel] < 9) { lcd.print("00"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 9 && midiProgram[midiChannel] < 99) { lcd.print("0"); lcd.print(midiProgram[midiChannel] + 1); }
            if (midiProgram[midiChannel] >= 99) { lcd.print(midiProgram[midiChannel] + 1); }
            // LCD Update Reverb State
            lcd.setCursor(5,1);
            if (reverbState[midiChannel] == HIGH) { lcd.print("1"); }
            if (reverbState[midiChannel] == LOW)  { lcd.print("0"); }

            if (loopTrackActive[midiChannel] == HIGH && midiChannel == loopParentChannel)   // If the looper is active on this channel, and this is the parent loop's channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print("(");
                lcd.setCursor(13,1);
                lcd.print(")");
            }
            if (loopTrackActive[midiChannel] == HIGH && midiChannel != loopParentChannel)   // If the looper is active on this channel, and this is an overdub loop channel, print indicator.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.write(6);                                                               // Left side of loop track active glyph
                lcd.setCursor(13,1);
                lcd.write(7);                                                               // Right side of loop track active glyph
            }
            if (loopTrackActive[midiChannel] == LOW)                                        // If looper is inactive on this channel, clear the indicators at the cursor position.
            {
                // LCD Update Loop Track status
                lcd.setCursor(10,1);
                lcd.print(" ");
                lcd.setCursor(13,1);
                lcd.print(" ");
            }
        }


        if (previousLoopOverdubState == LOW)                                                                                            // Reinit loop to clear playback.  Broken out to allow percentage checking below.
        {
            previousLoopOverdubState = HIGH;                                                                                            // Lock input until released

            for (int myIndex = loopPlaybackIndex[midiChannel]; myIndex < loopRecordingIndex[midiChannel]; myIndex++)                    // From the current playback index forward
            {
                if (loopPacketByte0[midiChannel][myIndex] == 0)                                                                         // If the packet type is 0 = noteOff
                {
                    loopNoteOff(midiChannel, loopPacketByte1[midiChannel][myIndex], loopPacketByte2[midiChannel][myIndex]);             // Spool out remaining noteOffs to close notes and prevent hangs
                }
            }

            loopRecordingIndex[midiChannel] = 0;                                // Reset recording index for current track to 0
            loopPlaybackIndex[midiChannel] = 0;                                 // Reset playback index for current track to 0
            for (int myIndex = 0; myIndex < loopMaxIndexes; myIndex++)          // For all indexes allocated to a looper track
            {
                loopPacketEventTime[midiChannel][myIndex] = 0;                  // Reset event time for current track to 0
                loopPacketByte0[midiChannel][myIndex] = 0;                      // Reset event type for current track to 0
                loopPacketByte1[midiChannel][myIndex] = 0;                      // Reset pitch/control/highByte for current track to 0
                loopPacketByte2[midiChannel][myIndex] = 0;                      // Reset velocity/value/lowByte for current track to 0
            }
            
            loopTrackActive[midiChannel] = LOW;                                 // Mark track available for recording

            // LCD Update Loop Track status
            lcd.setCursor(10,1);
            lcd.print(" ");
            lcd.setCursor(13,1);
            lcd.print(" ");
        }

        if (loopPercentage < 25)                                                // Prevent recording from enabling if overdub was pressed when we're already well into the track.
        {                                                                       // Otherwise if the gun is jumped, and a note was hit just before loop reset, this would
            loopRecordingEnabled = HIGH;                                        // cause a single note playback at 99%, and a worthless track for a full loop.
        }
    }


    // Step 6 - Recording is enabled again, and there is data in memory.  Flush data and reinitialize
    if (loopRecordState == HIGH && previousLoopRecordState == LOW && loopInMemory == HIGH)
    {
        previousLoopRecordState = HIGH;                                                                                 // Lock input until released
        for (int myTrack = 0; myTrack < loopMaxTracks; myTrack++)                                                       // For all looper tracks
        {
            for (int myIndex = loopPlaybackIndex[myTrack]; myIndex < loopRecordingIndex[myTrack]; myIndex++)            // From the current playback index forward
            {
                if (loopPacketByte0[myTrack][myIndex] == 0)                                                             // If packet type is noteOff
                {
                    loopNoteOff(myTrack, loopPacketByte1[myTrack][myIndex], loopPacketByte2[myTrack][myIndex]);         // Spool out remaining noteOffs to close notes and prevent hangs
                }
            }
        }
        loopRecordingEnabled = HIGH;                                            // Enable loop packet recording
        loopPlaybackEnabled = LOW;                                              // Disable playback
        loopInMemory = LOW;                                                     // Mark loop free for writing
        loopInputDetected = LOW;                                                // Reset loop input detection variable
        loopDuration = 0;                                                       // Clear loop duration
        loopStartTimestamp = 0;                                                 // Clear loop start timestamp
        memset(loopRecordingIndex, 0, sizeof(loopRecordingIndex));              // Flatten all looper arrays
        memset(loopPlaybackIndex, 0, sizeof(loopPlaybackIndex));                // Flatten all looper arrays
        memset(loopPacketEventTime, 0, sizeof(loopPacketEventTime));            // Flatten all looper arrays
        memset(loopPacketByte0, 0, sizeof(loopPacketByte0));                    // Flatten all looper arrays
        memset(loopPacketByte1, 0, sizeof(loopPacketByte1));                    // Flatten all looper arrays
        memset(loopPacketByte2, 0, sizeof(loopPacketByte2));                    // Flatten all looper arrays
        memset(loopTrackActive, 0, sizeof(loopTrackActive));                    // Flatten all looper arrays
        loopParentChannel = 255;                                                // Reset loop parent channel

        // LCD Update Looper
        lcd.setCursor(0,1);
        lcd.print("??");

        // LCD Update Loop Track status
        lcd.setCursor(10,1);
        lcd.print("(");
        lcd.setCursor(13,1);
        lcd.print(")");
    }
}

// MIDI PACKET FUNCTIONS

// Send MIDI Note On
// 1st byte = Event type (0x09 = note on, 0x08 = note off).
// 2nd byte = Event type bitwise ORed with MIDI channel.
// 3rd byte = MIDI note number.
// 4th byte = Velocity (7-bit range 0-127)
void noteOn(byte channel, byte pitch, byte velocity)
{
    if (loopRecordingEnabled == HIGH && loopTrackActive[channel] == LOW)                                    // If loop recording enabled, and track available for recording
    {
        loopPacketEventTime[channel][loopRecordingIndex[channel]] = (currentTime - loopStartTimestamp);     // Record event time in relation to loop start
        loopPacketByte0[channel][loopRecordingIndex[channel]] = 1;                                          // Record event type (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        loopPacketByte1[channel][loopRecordingIndex[channel]] = pitch;                                      // Record pitch/control/highByte
        loopPacketByte2[channel][loopRecordingIndex[channel]] = velocity;                                   // Record velocity/value/lowByte
        loopRecordingIndex[channel] = loopRecordingIndex[channel] + 1;                                      // Increment the recording index
    }
    channel = 0x90 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t noteOn = {0x09, channel, pitch, velocity};                // noteOn Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(noteOn);                                                   // Send packet to the MIDI USB bus
    Serial1.write(0x90 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(pitch);                                                       // Send note number to the MIDI serial bus
    Serial1.write(velocity);                                                    // Send velocity value to the MIDI serial bus
}
void loopNoteOn(byte channel, byte pitch, byte velocity)
{
    byte myVelocity = constrain( ( (velocity * looperVelocity) / 1000), 0, 127);
    channel = 0x90 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t noteOn = {0x09, channel, pitch, myVelocity};                // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(noteOn);                                                   // Send packet to the MIDI USB bus
    Serial1.write(0x90 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(pitch);                                                       // Send note number to the MIDI serial bus
    Serial1.write(velocity);                                                    // Send velocity value to the MIDI serial bus
}


// Send MIDI Note Off
// 1st byte = Event type (0x09 = note on, 0x08 = note off).
// 2nd byte = Event type bitwise ORed with MIDI channel.
// 3rd byte = MIDI note number.
// 4th byte = Velocity (7-bit range 0-127)
void noteOff(byte channel, byte pitch, byte velocity)
{
    if (loopRecordingEnabled == HIGH && loopTrackActive[channel] == LOW)                                    // If loop recording enabled, and track available for recording
    {
        loopPacketEventTime[channel][loopRecordingIndex[channel]] = (currentTime - loopStartTimestamp);     // Record event time in relation to loop start
        loopPacketByte0[channel][loopRecordingIndex[channel]] = 0;                                          // Record event type (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        loopPacketByte1[channel][loopRecordingIndex[channel]] = pitch;                                      // Record pitch/control/highByte
        loopPacketByte2[channel][loopRecordingIndex[channel]] = velocity;                                   // Record velocity/value/lowByte
        loopRecordingIndex[channel] = loopRecordingIndex[channel] + 1;                                      // Increment the recording index
    }
    channel = 0x80 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t noteOff = {0x08, channel, pitch, velocity};               // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(noteOff);                                                  // Send packet to the MIDI USB bus
    Serial1.write(0x80 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(pitch);                                                       // Send note number to the MIDI serial bus
    Serial1.write(velocity);                                                    // Send velocity value to the MIDI serial bus
}
void loopNoteOff(byte channel, byte pitch, byte velocity)
{
    channel = 0x80 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t noteOff = {0x08, channel, pitch, velocity};               // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(noteOff);                                                  // Send packet to the MIDI USB bus
    Serial1.write(0x80 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(pitch);                                                       // Send note number to the MIDI serial bus
    Serial1.write(velocity);                                                    // Send velocity value to the MIDI serial bus
}

// Control Change
// 1st byte = Event type (0x0B = Control Change).
// 2nd byte = Event type bitwise ORed with MIDI channel.
// 3rd byte = MIDI CC number (7-bit range 0-127).
// 4th byte = Control value (7-bit range 0-127).
void controlChange(byte channel, byte control, byte value)
{
    if (loopRecordingEnabled == HIGH && loopTrackActive[channel] == LOW)                                    // If loop recording enabled, and track available for recording
    {
        loopPacketEventTime[channel][loopRecordingIndex[channel]] = (currentTime - loopStartTimestamp);     // Record event time in relation to loop start
        loopPacketByte0[channel][loopRecordingIndex[channel]] = 2;                                          // Record event type (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        loopPacketByte1[channel][loopRecordingIndex[channel]] = control;                                    // Record pitch/control/highByte
        loopPacketByte2[channel][loopRecordingIndex[channel]] = value;                                      // Record velocity/value/lowByte
        loopRecordingIndex[channel] = loopRecordingIndex[channel] + 1;                                      // Increment the recording index
    }
    channel = 0xB0 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t event = {0x0B, channel, control, value};                  // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(event);                                                    // Send packet to the MIDI USB bus
    Serial1.write(0xB0 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(control);                                                     // Send control change number to the MIDI serial bus
    Serial1.write(value);                                                       // Send control chnage value to the MIDI serial bus
}
void loopControlChange(byte channel, byte control, byte value)
{
    channel = 0xB0 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t event = {0x0B, channel, control, value};                  // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(event);                                                    // Send packet to the MIDI USB bus
    Serial1.write(0xB0 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(control);                                                     // Send control change number to the MIDI serial bus
    Serial1.write(value);                                                       // Send control chnage value to the MIDI serial bus
}

// Program Change
// 1st byte = Event type (0x0C = Program Change).
// 2nd byte = Event type bitwise ORed with MIDI channel.
// 3rd byte = Program value (7-bit range 0-127).
void programChange(byte channel, byte value)
{
    channel = 0xC0 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t event = {0x0C, channel, value};                           // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(event);                                                    // Send packet to the MIDI USB bus
    Serial1.write(0xC0 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(value);                                                       // Send program change value to the MIDI serial bus
}

// Pitch Bend
// (14 bit value 0-16363, neutral position = 8192)
// 1st byte = Event type (0x0E = Pitch bend change).
// 2nd byte = Event type bitwise ORed with MIDI channel.
// 3rd byte = The 7 least significant bits of the value.
// 4th byte = The 7 most significant bits of the value.
void pitchBendChange(byte channel, byte lowValue, byte highValue)
{
    if (loopRecordingEnabled == HIGH && loopTrackActive[channel] == LOW)                                    // If loop recording enabled, and track available for recording
    {
        loopPacketEventTime[channel][loopRecordingIndex[channel]] = (currentTime - loopStartTimestamp);     // Record event time in relation to loop start
        loopPacketByte0[channel][loopRecordingIndex[channel]] = 3;                                          // Record event type (1 = noteOn, 0 = noteOff, 2 = controlChange, 3 = pitchBendChange)
        loopPacketByte1[channel][loopRecordingIndex[channel]] = lowValue;                                   // Record pitch/control/highByte
        loopPacketByte2[channel][loopRecordingIndex[channel]] = highValue;                                  // Record velocity/value/lowByte
        loopRecordingIndex[channel] = loopRecordingIndex[channel] + 1;                                      // Increment the recording index
    }
    channel = 0xE0 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t bendEvent = {0x0E, channel, lowValue, highValue};         // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(bendEvent);                                                // Send packet to the MIDI USB bus
    Serial1.write(0xE0 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(lowValue);                                                    // Send pitch bend low byte to the MIDI serial bus
    Serial1.write(highValue);                                                   // Send pitch bend high byte to the MIDI serial bus
}
void loopPitchBendChange(byte channel, byte lowValue, byte highValue)
{
    channel = 0xE0 | channel;                                                   // Bitwise OR outside of the struct to prevent compiler warnings
    midiEventPacket_t bendEvent = {0x0E, channel, lowValue, highValue};         // Build a struct containing all of our information in a single packet
    MidiUSB.sendMIDI(bendEvent);                                                // Send packet to the MIDI USB bus
    Serial1.write(0xE0 | channel);                                              // Send event type/channel to the MIDI serial bus
    Serial1.write(lowValue);                                                    // Send pitch bend low byte to the MIDI serial bus
    Serial1.write(highValue);                                                   // Send pitch bend high byte to the MIDI serial bus
}

// END FUNCTIONS SECTION
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

// END OF PROGRAM
// ------------------------------------------------------------------------------------------------------------------------------------------------------------
