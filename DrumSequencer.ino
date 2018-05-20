#include <MIDI.h>
#include <avr\io.h>
#include <avr\interrupt.h>

// Create MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();

// Declare constants
float frequencyClock = 16000000;
float interruptPrescale = 1024;
long frequencyMIDI = 31250;
int pulsesPerQuarterNote = 24;
int secondsPerMinute = 60;

// Declare vars
float frequencyPulse;
float periodQuarterNote;
float periodPulse;
float syncopationRatio;
int currentStep;
int tempo;
int octave;
int stepSixteenthNote;

// Declare note indices
int C = 0;
int Cs = 1;
int D = 2;
int Ds = 3;
int E = 4;
int F = 5;
int Fs = 6;
int G = 7;
int Gs = 8;
int A = 9;
int As = 10;
int B = 11;

// Declare hit/velocity arrays - one octave, sixteen steps
bool hitArray[12][16];
int velocityArray[12][16];
int sixteenthArray[16];

//---------------------------- Function Definitions ----------------------------
void CalculatePeriods()
{

	periodQuarterNote = (float)secondsPerMinute / (float)tempo;
	periodPulse = periodQuarterNote / pulsesPerQuarterNote;

}

void SetInterrupt()
{
	// Get pulse frequency - this should be the interrupt rate
	float frequencyInterruptTarget = 1 / periodPulse;

	// Set the compare match register
	float compareMatch = (frequencyClock / (frequencyInterruptTarget * interruptPrescale)) - (float)1;
	OCR1A = (int)compareMatch;

	Serial.println((int)frequencyInterruptTarget);

}

void InitialiseTimerInterrupts()
{

	// Initialise registers
	TCCR1A = 0; // Set TCCR1A register to 0
	TCCR1B = 0; // Same for TCCR1B
	TCNT1 = 0; // Set counter to 0

	// Set the interrupt
	SetInterrupt();

	// Enable 'Clear Timer on Compare Match' mode - CTC
	TCCR1B |= (1 << WGM12);

	// Set CS10 / CS12 bits for prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// Enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	// Start
	sei();
}
//------------------------------------------------------------------------------

void setup()
{

	// Set up initial time, tempo, octave number
	currentStep = 0;
	tempo = 120;
	octave = 3;

	// Calculate intervals, initialise and set interrupt frequency
	CalculatePeriods();
	InitialiseTimerInterrupts();

	//Create sixteenth array
	int stepSingleSixteenth = pulsesPerQuarterNote / 4;
	sixteenthArray[0] = stepSingleSixteenth * 0;
	sixteenthArray[1] = stepSingleSixteenth * 1;
	sixteenthArray[2] = stepSingleSixteenth * 2;
	sixteenthArray[3] = stepSingleSixteenth * 3;
	sixteenthArray[4] = stepSingleSixteenth * 4;
	sixteenthArray[5] = stepSingleSixteenth * 5;
	sixteenthArray[6] = stepSingleSixteenth * 6;
	sixteenthArray[7] = stepSingleSixteenth * 7;
	sixteenthArray[8] = stepSingleSixteenth * 8;
	sixteenthArray[9] = stepSingleSixteenth * 9;
	sixteenthArray[10] = stepSingleSixteenth * 10;
	sixteenthArray[11] = stepSingleSixteenth * 11;
	sixteenthArray[12] = stepSingleSixteenth * 12;
	sixteenthArray[13] = stepSingleSixteenth * 13;
	sixteenthArray[14] = stepSingleSixteenth * 14;
	sixteenthArray[15] = stepSingleSixteenth * 15;

	// Begin
	MIDI.begin(1);
	Serial.begin(frequencyMIDI);

}

void loop()
{

	// TODO - read from pushbuttons/pots to get target states

	// Quick hack - hard-code a pattern
	hitArray[C][0] = true;
	hitArray[C][1] = false;
	hitArray[C][2] = false;
	hitArray[C][3] = false;
	hitArray[C][4] = true;
	hitArray[C][5] = false;
	hitArray[C][6] = false;
	hitArray[C][7] = false;
	hitArray[C][8] = true;
	hitArray[C][9] = false;
	hitArray[C][10] = false;
	hitArray[C][11] = false;
	hitArray[C][12] = true;
	hitArray[C][13] = false;
	hitArray[C][14] = false;
	hitArray[C][15] = false;

	hitArray[Fs][0] = false;
	hitArray[Fs][1] = false;
	hitArray[Fs][2] = true;
	hitArray[Fs][3] = false;
	hitArray[Fs][4] = false;
	hitArray[Fs][5] = false;
	hitArray[Fs][6] = true;
	hitArray[Fs][7] = false;
	hitArray[Fs][8] = false;
	hitArray[Fs][9] = false;
	hitArray[Fs][10] = true;
	hitArray[Fs][11] = false;
	hitArray[Fs][12] = false;
	hitArray[Fs][13] = false;
	hitArray[Fs][14] = true;
	hitArray[Fs][15] = false;

	// Hard code velocities
	velocityArray[C][0] = 120;
	velocityArray[C][1] = 0;
	velocityArray[C][2] = 0;
	velocityArray[C][3] = 0;
	velocityArray[C][4] = 120;
	velocityArray[C][5] = 0;
	velocityArray[C][6] = 0;
	velocityArray[C][7] = 0;
	velocityArray[C][8] = 120;
	velocityArray[C][9] = 0;
	velocityArray[C][10] = 0;
	velocityArray[C][11] = 0;
	velocityArray[C][12] = 120;
	velocityArray[C][13] = 0;
	velocityArray[C][14] = 0;
	velocityArray[C][15] = 0;

	velocityArray[Fs][0] = 0;
	velocityArray[Fs][1] = 0;
	velocityArray[Fs][2] = 120;
	velocityArray[Fs][3] = 0;
	velocityArray[Fs][4] = 0;
	velocityArray[Fs][5] = 0;
	velocityArray[Fs][6] = 120;
	velocityArray[Fs][7] = 0;
	velocityArray[Fs][8] = 0;
	velocityArray[Fs][9] = 0;
	velocityArray[Fs][10] = 120;
	velocityArray[Fs][11] = 0;
	velocityArray[Fs][12] = 0;
	velocityArray[Fs][13] = 0;
	velocityArray[Fs][14] = 120;
	velocityArray[Fs][15] = 0;

}

// Timer 1 callback
ISR(TIMER1_COMPA_vect)
{
	// Send midi clock signal
	Serial.write(248);

	// Reset step counter after each bar - check types here for rounding errors
	if (currentStep == pulsesPerQuarterNote * 4)
	{
		currentStep = 0;
	}

	// Loop for the sixteenth note times
	for (int j = 0; j < 16; j++)
	{
		if (currentStep == sixteenthArray[j])
		{
			for (int i = 0; i < 12; i++)
			{
				// Get velocity
				int velocity = velocityArray[i][j];

				// Notes on/off
				if (hitArray[i][j])
				{
					MIDI.sendNoteOn(i + octave * 12, velocity, 1);
				}

				if (hitArray[i][(j - 1)])
				{
					MIDI.sendNoteOff(i + octave * 12, velocity, 1);
				}
			}
		}
	}

	// Increment time
	currentStep += 1;
}
