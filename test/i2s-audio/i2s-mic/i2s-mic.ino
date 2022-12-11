
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    Sample sound from I2S microphone, display on Arduino IDE Serial Plotter
    Requires INMP441 I2S microphone

PHYSICAL DESIGN
    Hardware
        ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/
        INMP441 I2S microphone

    Wiring

MONITOR
    To see the values plotted:

        arduino > /dev/null 2>&1

    To monitor the trace messages of the ESP32 development board,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter `CNTR-a :quit`.
    To stop the screen scrolling, enter `CNTR-a`.

TESTING
    To test the

REFERENCE MATERIALS

SOURCES
    https://dronebotworkshop.com/esp32-i2s/

------------------------------------------------------------------------------*/


#define TDEBUG true              // activate trace message printing for debugging

#ifdef ESP32
#include <driver/i2s.h>           // i2s driver
#else
#endif

#include "DeBug.h"



//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "i2s-mic" " - "  __DATE__ " at " __TIME__
static char version[] = VER;

// pin connections to INMP441 and SPH0645 I2S microphone
// https://diyi0t.com/i2s-sound-tutorial-for-esp32/
// https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357
//#define I2S_WS 25
//#define I2S_SD 33
//#define I2S_SCK 32


//#define I2S_SCK GPIO_NUM_32     // serial clock, BCLK on the adafruit breakout board for SPH0645, nodemcu-32s pin 7
//#define I2S_SD  GPIO_NUM_33     // serial data, DOUT on the adafruit breakout board for SPH0645, nodemcu-32s pin 8
//#define I2S_WS  GPIO_NUM_25     // word select, LRCL on the adafruit breakout board for SPH0645, nodemcu-32s pin 9


#define I2S_SD  GPIO_NUM_33     // serial data, DOUT on the adafruit breakout board for SPH0645, nodemcu-32s pin 8
#define I2S_WS  GPIO_NUM_25     // word select, LRCL on the adafruit breakout board for SPH0645, nodemcu-32s pin 9
#define I2S_SCK GPIO_NUM_26     // serial clock, BCLK on the adafruit breakout board for SPH0645, nodemcu-32s pin 10


// use I2S Processor 0
#define I2S_PORT I2S_NUM_0


//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug


// Define input buffer length
#define BUFFERLEN 64
#define BUFFERCNT 8
int16_t sBuffer[BUFFERLEN];


//------------------------------ Helper Routines -------------------------------

/*// set up i2s processor configuration*/
/*void i2s_install(void) {*/

    /*DEBUGTRACE(HEADING, "--------------------------- Entered si2s_install() -----------------------------");*/

    /*const i2s_config_t i2s_config = {*/
        /*.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),                     // receiver, not transfer*/
        /*.sample_rate = 44100,                                                  // 44.1KHz sample rate*/
        /*.bits_per_sample = i2s_bits_per_sample_t(16),                          // 16 bits per sample*/
        /*.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,                           // using left channel*/
        /*.communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),  // i2s communication format*/
        /*.intr_alloc_flags = 0,*/
        /*//.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                              // interrupt level 1*/
        /*.dma_buf_count = 8,                                                    // number of buffers*/
        /*.dma_buf_len = BUFFERLEN,                                              // samples per buffer (8 is minimum)*/
        /*.use_apll = false                                                      // set to false to turn-off PLL*/
        /*//.tx_desc_auto_clear = false,                                           // auto clear tx descriptor when underflow*/
        /*//.fixed_mclk = 0*/
    /*};*/

    /*i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);*/

/*}*/


// set up i2s processor configuration
void i2s_install(void) {

    DEBUGTRACE(HEADING, "--------------------------- Entered si2s_install() -----------------------------");

    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),                     // receiver, not transfer
        //.sample_rate = 44100,                                                  // 44.1KHz sample rate
        .sample_rate = 16000,                                                  // 16KHz sample rate
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,                          // could only get it to work with 32bits
        //.bits_per_sample = i2s_bits_per_sample_t(16),                          // could only get it to work with 32bits
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,                           // using left channel
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),  // i2s communication format
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                              // interrupt level 1
        .dma_buf_count = BUFFERCNT,                                                    // number of buffers
        .dma_buf_len =  BUFFERLEN                                              // samples per buffer (8 is minimum)
        //.use_apll = false                                                      // set to false to turn-off PLL
        //.tx_desc_auto_clear = false,                                           // auto clear tx descriptor when underflow
        //.fixed_mclk = 0
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

}

// set i2s pin configuration
// this structure defines the GPIO pin numbers to which the i2s driver should route
// the MCK, BCK, WS, DATA out, and DATA in signals. If you want to keep a currently
// allocated pin number for a specific signal, or if this signal is unused,
// set I2S_PIN_NO_CHANGE for any pin where the current configuration should not be changed
void i2s_setpin(void) {

    DEBUGTRACE(HEADING, "---------------------------- Entered i2s_setpin() ------------------------------");

    const i2s_pin_config_t pin_config = {
//        .mck_io_num = I2S_PIN_NO_CHANGE,    // MCK input/output pin - never set in examples
        .bck_io_num = I2S_SCK,              // BCK (aka SCK) serial clock input/output pin
        .ws_io_num = I2S_WS,                // WS word select input/output pin
//        .data_out_num = -1
        .data_out_num = I2S_PIN_NO_CHANGE,  // DATA serial data output pin, not used (only for speakers)
        .data_in_num = I2S_SD               // DATA serial data input pin
    };

    i2s_set_pin(I2S_PORT, &pin_config);

    DEBUGTRACE(INFO, "I2S_PORT = ", I2S_PORT);
    DEBUGTRACE(INFO, "I2S_SCK = ", I2S_SCK);
    DEBUGTRACE(INFO, "I2S_WS = ", I2S_WS);
    DEBUGTRACE(INFO, "I2S_SD = ", I2S_SD);

}



//------------------------------- Main Routines --------------------------------

void setup(void) {

    Serial.begin(9600);
    //Serial.begin(115200);     // baud rate of 115200 is used instead of 9600 for a faster data rate
    while (!Serial) {}        // wait for serial port to connect

    Serial.println(" ");

    DEBUGSETUP();                        // should be right after 'Serial.begin' and before any debug message
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);
    DEBUGINFO();

    delay(2000);      // pause so you can read trace messages

    // set up i2s
    i2s_install();
    i2s_setpin();
    i2s_start(I2S_PORT);

    delay(2000);      // pause so you can read trace messages

    DEBUGTRACE(HEADING, "------------------------------- Leaving setup() --------------------------------");

}


void loop(void) {

    //int rangelimit = 3000;             // change rangelimit value to adjust "sensitivity"
    int rangelimit = 3;             // change rangelimit value to adjust "sensitivity"

    DEBUGLOOP();

    // false print statements to "lock range" on serial plotter display
    Serial.print(rangelimit * -1);
    Serial.print(" ");
    Serial.print(rangelimit);
    Serial.print(" ");

    // 'i2s_read' retrieves the data from the i2s dma recieve buffer, once the data is received by the i2s controller
    //
    // esp_err_t i2s_read(i2s_port_ti2s_num, void *dest, size_t size, size_t *bytes_read, TickType_t ticks_to_wait)
    //     'i2s_num'         is the i2s port number
    //     'dest'            is the address of the buffer to read the data into
    //     'size'            is the size of the buffer
    //     'bytes_read'      is a pointer to a size_t which will have the number of bytes read written to it
    //     'ticks_to_wait'   is a timeout
    size_t bytesIn = 0;
    esp_err_t result = i2s_read(I2S_PORT, &sBuffer, BUFFERLEN, &bytesIn, portMAX_DELAY);

    // read i2s data buffer, calculate mean value, then print that mean
    if (result == ESP_OK) {
        int16_t samples_read = bytesIn / 8;
        if (samples_read > 0) {
            float mean = 0;
            for (int16_t i = 0; i < samples_read; ++i) {
            //mean += (sBuffer[i]);
            mean += float(sBuffer[i]);
        }

        // average the data reading
        //mean /= samples_read;
        mean /= float(samples_read);

        // Print to serial plotter
        Serial.println(mean);            // 'println' newline means end of data points for serial plotter
        }
    }
}

