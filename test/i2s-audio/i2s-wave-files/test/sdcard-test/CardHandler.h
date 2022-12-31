
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.1.0

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

    File root;                    // root directory of the filesystem
    uint8_t cardType;             // type of SD Card: MMC, SDSC, SDHC

    const uint64_t bytes_per_megabytes = 1024 * 1024;

    //-------------- private methods ---------------
    bool blinkLED(unsigned long);
    void errorHandler(int, char *);
    bool initSDCard();
    void descendDir(File, int);

#ifdef XXX
    void unMount(fs::SDFS &);
    bool listDir(fs::SDFS &, const char *, uint8_t);
    bool createDir(fs::SDFS &, const char *);
    bool removeDir(fs::SDFS &, const char *);
    bool writeFile(fs::SDFS &, const char *, const char *);
    bool appendFile(fs::SDFS &, const char *, const char *);
    bool readFile(fs::SDFS &, const char *);
    bool renameFile(fs::SDFS &, const char *, const char *);
    bool deleteFile(fs::SDFS &, const char *);
    bool testFileIO(fs::SDFS &, const char *);
#else
#endif

  public:
    //-- constructors & destructors for the class --
    CardHandler(void);
    ~CardHandler(void);

    //--------------- public methods ---------------
#ifdef XXX
    inline void unmount(fs::SDFS &) { unMount(SD); };
    inline bool listDir(const char *dirname, uint8_t levels) { listDir(SD, dirname, levels); };
    inline bool createDir(const char *path) { createDir(SD, path); };
    inline bool removeDir(const char *path) { removeDir(SD, path); };
    inline bool writeFile(const char *path, const char *message) { writeFile(SD, path, message); };
    inline bool appendFile(const char *path, const char *message) { appendFile(SD, path, message); };
    inline bool readFile(const char *path) { readFile(SD, path); };
    inline bool renameFile(const char *path1, const char *path2) { renameFile(SD, path1, path2); };
    inline bool deleteFile(const char *path) { deleteFile(SD, path); };
    inline bool testFileIO(const char *path) { testFileIO(SD, path); };
#else
    void unMount();
    bool existsFile(const char *);
    bool listDir(const char *, uint8_t);
    bool createDir(const char *);
    bool removeDir(const char *);
    bool writeFile(const char *, const char *);
    bool appendFile(const char *, const char *);
    bool readFile(const char *);
    bool renameFile(const char *, const char *);
    bool deleteFile(const char *);
    bool testFileIO(const char *);
#endif
};

