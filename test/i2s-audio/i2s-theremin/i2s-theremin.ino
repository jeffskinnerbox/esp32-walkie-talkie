
#include <I2S.h>



//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "i2s-theremin" " - "  __DATE__ " at " __TIME__
static char version[] = VER;



//-------------------------- Static Scoped Variables ---------------------------

const int maxFrequency = 5000;        // maximum generated frequency
const int minFrequency = 220;         // minimum generated frequency
const int maxVolume = 100;            // max volume of the generated frequency
const int minVolume = 0;              // min volume of the generated frequency
const int sampleRate = 44100;         // sample rate of the generated frequency
const int frequencyPin = GPIO_NUM_10; // pin connected to the pot which determines the frequency of the signal
const int amplitudePin = GPIO_NUM_11; // pin connected to the pot which determines the amplitude of the signal
const int button = 6;                 // pin connected to the button control to display the frequency
const int wavSize = 256;              // buffer size

short sine[wavSize];                  // buffer in which the sine values are stored



//------------------------------- Main Routines --------------------------------

void setup(void) {

    Serial.begin(9600);
    while (!Serial) {}                   // wait for serial port to connect

    // Initialize the I2S transmitter.
    if (!I2S.begin(I2S_PHILIPS_MODE, sampleRate, 16)) {
        Serial.println("Failed to initialize I2S");
        while (1);
    }

    generateSine();                   // fill buffer with sine values
    pinMode(button, INPUT_PULLUP);    // put the button pin in input pull-up

}


void loop(void) {

    // arduino map() function - https://techexplorations.com/guides/arduino/programming/map-function/
    if (digitalRead(button) == LOW) {
        //float frequency = map(analogRead(frequencyPin), 0, 1023, minFrequency, maxFrequency);
        //int amplitude = map(analogRead(amplitudePin), 0, 1023, minVolume, maxVolume);

        // apply the calibration of the potentiometer and constrain the value to the range
        float frequency = analogRead(frequencyPin);
        frequency = map(frequency, 0, 1023, minFrequency, maxFrequency);
        frequency = constrain(frequency, 0, 1023);

        // apply the calibration of the potentiometer and constrain the value to the range
        int amplitude = analogRead(amplitudePin);
        amplitude = map(amplitude, 0, 1023, minVolume, maxVolume);
        amplitude = constrain(amplitude, 0, 1023);

        // play sound via max98357 audio breakout board
        playWave(amplitude, frequency, 0.1);

        //print values on serial
        Serial.print("Frequency = ");
        Serial.println(frequency);
        Serial.print("Amplitude = ");
        Serial.println(amplitude);
    }

}



//------------------------------ Helper Routines -------------------------------

void generateSine(void) {

    for (int i = 0; i < wavSize; ++i)
        sine[i] = ushort(float(100) * sin(2.0 * PI * (1.0 / wavSize) * i)); // 100 is used to not have small numbers

}


// Play back the provided waveform buffer for the specified amount of seconds.
void playWave(int amplitude, float frequency, float seconds) {

    // calculate number of samples need to play back the desired amount of seconds
    unsigned int iterations = seconds * sampleRate;

    // Then calculate the ‘speed’ at which we move through the wave
    // buffer based on the frequency of the tone being played.
    float delta = (frequency * wavSize) / float(sampleRate);

    // Now loop through all the samples and play them, calculating the
    // position within the wave buffer for each moment in time.
    for (unsigned int i = 0; i < iterations; ++i) {
        short pos = (unsigned int)(i * delta) % wavSize;
        short sample = amplitude * sine[pos];

        // Duplicate the sample so it’s sent to both the left and right channels.
        // It appears the order is the right channel, left channel if you want to write
        // stereo sound.
        while (I2S.availableForWrite() < 2);
        I2S.write(sample);
        I2S.write(sample);
    }

}

