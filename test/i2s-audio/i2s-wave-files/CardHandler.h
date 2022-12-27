
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#pragma once                // compiler to skip subsequent includes of this file


#include <SD.h>


class CardHandler {
  private:
    //-------------- private variables -------------
    int cs_pin = CS_PIN;          // pin used to provide chip select (CS)
    int sck_pin = SCK_PIN;        // pin used to provide clock (SCK)
    int mosi_pin = MOSI_PIN;      // pin used to provide master output / slave input (MOSI)
    int miso_pin = MISO_PIN;      // pin used to provide master input / slave output (MISO)

    uint8_t cardType;             // type of SD Card: MMC, SDSC, SDHC

    const uint64_t bytes_per_megabytes = 1024 * 1024;

    //-------------- private methods ---------------
    bool blinkLED(unsigned long);
    void errorHandler(int, char *);
    bool initSDCard();

  public:
    //-- constructors & destructors for the class --
    CardHandler(void);
    ~CardHandler(void);

    //--------------- public methods ---------------
    void listDir(fs::SDFS &, const char *, uint8_t);
    void createDir(fs::SDFS &, const char *);
    void removeDir(fs::SDFS &, const char *);
    void writeFile(fs::SDFS &, const char *, const char *);
    void appendFile(fs::SDFS &, const char *, const char *);
    void readFile(fs::SDFS &, const char *);
    void renameFile(fs::SDFS &, const char *, const char *);
    void deleteFile(fs::SDFS &, const char *);
    void testFileIO(fs::SDFS &, const char *);

    inline void listDir(const char * dirname, uint8_t levels) { listDir(SD, dirname, levels); };
    inline void createDir(const char * path) { createDir(SD, path); };
    inline void removeDir(const char * path) { removeDir(SD, path); };
    inline void writeFile(const char * path, const char * message) { writeFile(SD, path, message); };
    inline void appendFile(const char *path, const char *message) { appendFile(SD, path, message); };
    inline void readFile(const char * path) { readFile(SD, path); };
    inline void renameFile(const char * path1, const char * path2) { renameFile(SD, path1, path2); };
    inline void deleteFile(const char * path) { deleteFile(SD, path); };
    inline void testFileIO(const char * path) { testFileIO(SD, path); };
};

