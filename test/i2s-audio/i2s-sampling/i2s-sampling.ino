
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    This a ESP32 test routine


PHYSICAL DESIGN
    Hardware
        ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

    Wiring

MONITOR
    To monitor the trace messages of the ESP32 development board,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter `CNTR-a :quit`.
    To stop the screen scrolling, enter `CNTR-a`.

TESTING
    To test the

REFERENCE MATERIALS
    for use of ADC, see https://embeddedexplorer.com/esp32-adc-esp-idf-tutorial/

SOURCES
    https://github.com/atomic14/esp32_audio

------------------------------------------------------------------------------*/


#define TDEBUG true              // activate trace message printing for debugging

#ifdef ESP32
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#else
#endif

#include "WiFiCredentials.h"
#include "I2SMEMSSampler.h"
#include "ADCSampler.h"
#include "DeBug.h"



//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "i2s-sampler" " - "  __DATE__ " at " __TIME__
static char version[] = VER;



//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug

WiFiClient *wifiClientADC = NULL;
HTTPClient *httpClientADC = NULL;
WiFiClient *wifiClientI2S = NULL;
HTTPClient *httpClientI2S = NULL;
ADCSampler *adcSampler = NULL;
I2SSampler *i2sSampler = NULL;

// replace this with your machines IP Address
#define ADC_SERVER_URL "http://192.168.1.109:3232/adc_samples"
#define I2S_SERVER_URL "http://192.168.1.109:3232/i2s_samples"

// i2s config for using the internal ADC
i2s_config_t adcI2SConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_LSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};

// i2s config for reading from left channel of i2s
i2s_config_t i2sMemsConfigLeftChannel = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),             // receiver, not transfer
    .sample_rate = 16000,                                            // 16KHz sample rate
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,                    // 32 bits per sample
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,                     // using left channel
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),  // i2s communication format
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                        // interrupt level 1
    .dma_buf_count = 4,                                              // number of buffers
    .dma_buf_len = 1024,                                             // 1024 samples per buffer (8 is minimum)
    .use_apll = false,                                               // set to false to turn-off PLL
    .tx_desc_auto_clear = false,                                     // auto clear tx descriptor when underflow
    .fixed_mclk = 0
};

// i2s pins
// this structure defines the GPIO pin numbers to which the i2s driver should route
// the MCK, BCK, WS, DATA out, and DATA in signals. If you want to keep a currently
// allocated pin number for a specific signal, or if this signal is unused,
// set I2S_PIN_NO_CHANGE for any pin where the current configuration should not be changed
i2s_pin_config_t i2sPins = {
//    .mck_io_num = I2S_PIN_NO_CHANGE,              // MCK input/output pin - never set in examples
    .bck_io_num = GPIO_NUM_32,                      // BCK (aka SCK) serial clock input/output pin
    .ws_io_num = GPIO_NUM_25,                       // WS word select input/output pin
    .data_out_num = I2S_PIN_NO_CHANGE,              // DATA serial data output pin, not used (only for speakers)
    .data_in_num = GPIO_NUM_33                      // DATA serial data input pin
};

// how many samples to read at once
const int SAMPLE_SIZE = 16384;



//------------------------------ Helper Routines -------------------------------

// send data to a remote address
void sendData(WiFiClient *wifiClient, HTTPClient *httpClient, const char *url, uint8_t *bytes, size_t count) {

    digitalWrite(2, HIGH);          // set board led to high when sending data to server

    httpClient->begin(*wifiClient, url);
    httpClient->addHeader("content-type", "application/octet-stream");
    httpClient->POST(bytes, count);
    httpClient->end();

    digitalWrite(2, LOW);          // set board led to low when you complete sending data to server

    DEBUGTRACE(INFO, "I'm in sendData()");
    DEBUGTRACE(INFO, "count = ", count);
//    for (int i = 0; i < count; i++) {
//        Serial.printf("%c", bytes[i]);
//    }

}


// Task to write samples from ADC to our server
void adcWriterTask(void *param) {

    I2SSampler *sampler = (I2SSampler *)param;
    int16_t *samples = (int16_t *)malloc(sizeof(uint16_t) * SAMPLE_SIZE);

    if (!samples) {
        Serial.println("Failed to allocate memory for samples");
        return;
    }

    while (true) {
        int samples_read = sampler->read(samples, SAMPLE_SIZE);
        sendData(wifiClientADC, httpClientADC, ADC_SERVER_URL, (uint8_t *)samples, samples_read * sizeof(uint16_t));
    }

    DEBUGTRACE(INFO, "I'm in adcWriterTask()");

}


// Task to write samples to our server
void i2sMemsWriterTask(void *param) {

    I2SSampler *sampler = (I2SSampler *)param;
    int16_t *samples = (int16_t *)malloc(sizeof(uint16_t) * SAMPLE_SIZE);

    if (!samples) {
        Serial.println("Failed to allocate memory for samples");
        return;
    }

    while (true) {
        int samples_read = sampler->read(samples, SAMPLE_SIZE);
        sendData(wifiClientI2S, httpClientI2S, I2S_SERVER_URL, (uint8_t *)samples, samples_read * sizeof(uint16_t));
    }

    DEBUGTRACE(INFO, "I'm in i2sMemsWriterTask()");

}



//------------------------------- Main Routines --------------------------------

void setup() {

    Serial.begin(9600);
    while (!Serial) {}                   // wait for serial port to connect

    DEBUGSETUP();                        // should be right after 'Serial.begin' and before any debug message
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);

    // launch WiFi
    Serial.printf("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
    Serial.println("WiFi Connected");
    Serial.println("Started up");

    DEBUGTRACE(INFO, "ADC_SERVER_URL = ", ADC_SERVER_URL);
    DEBUGTRACE(INFO, "I2S_SERVER_URL = ", I2S_SERVER_URL);
    DEBUGTRACE(WARN, "Above URLs need to be the same as ESP IP Address below");
    DEBUGINFO();

    // indicator LED
    pinMode(2, OUTPUT);

    // setup the HTTP Client
    wifiClientADC = new WiFiClient();
    httpClientADC = new HTTPClient();

    wifiClientI2S = new WiFiClient();
    httpClientI2S = new HTTPClient();

    // input from analog microphones such as the MAX9814 or MAX4466
    // internal analog to digital converter sampling using i2s
    // create our samplers
    // adcSampler = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, adcI2SConfig);

    // set up the adc sample writer task
    // TaskHandle_t adcWriterTaskHandle;
    // adcSampler->start();
    // xTaskCreatePinnedToCore(adcWriterTask, "ADC Writer Task", 4096, adcSampler, 1, &adcWriterTaskHandle, 1);

    // Direct i2s input from INMP441 or the SPH0645
    i2sSampler = new I2SMEMSSampler(I2S_NUM_0, i2sPins, i2sMemsConfigLeftChannel, false);
    i2sSampler->start();

    // set up the i2s sample writer task
    TaskHandle_t i2sMemsWriterTaskHandle;
    xTaskCreatePinnedToCore(i2sMemsWriterTask, "I2S Writer Task", 4096, i2sSampler, 1, &i2sMemsWriterTaskHandle, 1);

    // start sampling from i2s device

    DEBUGTRACE(HEADING, "------------------------------- Leaving setup() --------------------------------");

}


void loop() {

    DEBUGLOOP();

    // nothing to do here - everything is taken care of by tasks

}
