
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.6

DESCRIPTION:
    Debugging routings that print trace messages on serial port

    Serial.println(val)
    Serial.println(val, format)

    val: the value to print. Allowed data types: any data type.
format: specifies the number base (for integral data types) or number of decimal places (for floating point types).

    The code here assumes you are using the C99 C programming language standard (see https://en.wikipedia.org/wiki/C99).
    The variable argument list notation is used within macros and this is not supported in versions earlier than C99.
    You'll see the "do { ... } while (0)" used here and this idiom ensures that the code acts like a statement (i.e. function call).
    The "if (SERIAL)" use of the macro code ensures that the compiler always checks that your debug code is valid — but the optimizer will remove the code when SERIAL is 0.

REFERENCE MATERIALS:
    https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
    https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
    Passing Different Types to Functions in C++ - https://www.dummies.com/programming/cpp/passing-different-types-to-functions-in-c/
    Templates in C++ - https://www.dummies.com/programming/cpp/templates-in-c/
    Function templates - http://www.cplusplus.com/doc/oldtutorial/templates/
    Template member functions - https://blog.feabhas.com/2014/07/template-member-functions/
    #define macro for debug printing in C? - https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c

    C++: “undefined reference to” templated class function - https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function

    C++ template definitions in a .cpp file (instead of a header file) - https://raymii.org/s/snippets/Cpp_template_definitions_in_a_cpp_file_instead_of_header.html

    How can I change the colors of my xterm using Ansi escape sequences? - https://superuser.com/questions/270214/how-can-i-change-the-colors-of-my-xterm-using-ansi-escape-sequences

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#pragma once                // compiler to skip subsequent includes of this file


#define INFO       0        // index into labels for printing information trace message
#define STAT       1        // index into labels for printing status change message
#define WARN       2        // index into labels for printing warning trace message
#define ERROR      3        // index into labels for printing error trace message
#define FATAL      4        // index into labels for printing fatal trace message
#define NOOP       5        // index into labels for printing operation not implemented yet
#define HEADING    6        // index into labels for printing a simple heading for organizing output
#define UNLABELED  7        // index into labels for printing unformatted text

#define LABEL_COLS 30       // max characters in labels
#define LABEL_ROWS 8        // number of labels (see list in constructor below)


class DeBug {
  private:
    //-------------- private variables -------------
    bool serial = false;     // flag to turn on/off serial trace messages
    bool telnet = false;     // flag to turn on/off telnet trace messages
    bool prefix = false;     // flag to turn on/off prefix for trace messages
    char **label = NULL;     // memory array used to store labels
    int cols = 0;            // max characters in labels
    int rows = 0;            // number of labels

    //--------------- private methods --------------
    void setLables(void);
    bool checkWiFi(void);
    void commandParser(char);

  public:
    //-- constructors & destructors for the class --
    DeBug(void);
    DeBug(bool, bool, bool);
    ~DeBug(void);

    //--------------- public methods ---------------
    void SetupHandler(void);
    void SetupHandler(bool, bool, bool);
    void LoopHandler(void);
    void wifiScan(void);
    void serialOnOff(bool);
    void telnetOnOff(bool);
    void prefixOnOff(bool);
    void OnOff(bool, bool, bool);
    void printStatus(void);
    void printInfo(void);
    void location(void);

    void traceMsg(int, char*);
    template<typename T> void printMsg(T);
    template<typename T> void traceMsg(int, char*, T);
    template<typename T, typename U> void printMsg(T, U);
    template<typename T, typename U> void traceMsg(int, char*, T, U);
    template<typename T, typename U, typename Z> void printMsg(T*, U, Z);

};


// -----------------------------------------------------------------------------


#if TDEBUG
    #define DEBUGTRACE(lev, ...) DB.traceMsg(lev, __VA_ARGS__); // create a labeled trace message
    #define DEBUGPRINT(...) DB.printMsg(__VA_ARGS__);           // print an unlabeled message
    #define DEBUGON(s, t, p) DB.OnOff(s, t, p);                 // turn on/off trace messages within the programs flow
    #define DEBUGSTATUS() DB.printStatus();                     // print information concerning status of DeBug object
    #define DEBUGINFO(...) DB.printInfo();                      // print information concerning ESP & flash memory chip
    #define DEBUGWIFISCAN() DB.wifiScan();                      // perfrom a single scan for nearby wifi networks
    #define DEBUGSETUP() DB.SetupHandler();                     // macro within the setup() function, must be after Serial.begin()
    #define DEBUGLOOP() DB.LoopHandler();                       // macro within the loop() function
    #define DEBUGLOCATION() DB.location();                      // NOT IMPLEMENTED YET
#else
    #define DEBUGTRACE(lev, ...)
    #define DEBUGPRINT(...)
    #define DEBUGON(s, t, p)
    #define DEBUGSTATUS()
    #define DEBUGINFO(...)
    #define DEBUGWIFISCAN()
    #define DEBUGSETUP()
    #define DEBUGLOOP()
    #define DEBUGLOCATION()
#endif

// -----------------------------------------------------------------------------


