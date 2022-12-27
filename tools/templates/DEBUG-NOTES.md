<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


* `#define DEBUGTRACE(lev, ...)` - use this to create a labeled trace message
* `#define DEBUGPRINT(...)` - use this to print an unlabeled message
* `#define DEBUGON(s, t, p)` - use this to turn on/off trace messages within the programss flow
* `#define DEBUGLOCATION()` - prints file name + function name + line number
* `#define DEBUGSETUP()` - place this macro within the setup() function
* `#define DEBUGLOOP()` - place this macro within the loop() function

# Troubleshooting
If your not getting any DeBug trace message out,
first thing to check for is this at the top of the file:

```c++
#define TDEBUG  true       // activate trace message printing for debugging
```

Also make sure you start `Serial` before you initate `DeBug`, like this:

```c++
// start serial usb i/o services
Serial.begin(9600);
while (!Serial) {}          // wait for serial port to connect

DEBUGSETUP();               // should be right after 'Serial.begin'
OTA.setupOTA();             // should follow 'DEBUGSETUP()'
```


# Debug Trace Message Format
Debugging routings that print trace messages on serial port

Serial.println(val)
Serial.println(val, format)

val: the value to print. Allowed data types: any data type.
format: specifies the number base (for integral data types) or number of decimal places (for floating point types).

# Why Templates?
In C++, a template is a description of how to build a family of classes or functions
that all look basically the same.

# Compiler Errors for Templated Class Functions
When implementing the DeBug class,
I was plagued by “undefined reference to” compiler errors for templated class functions.
Like any typical C++ project,
I want to split my code into a class header file (`*.h`) and source code file (`*.cpp`).
While this works well for most projects,
when you using templates, you can run into limitation of C++ compile & linking,
you will get [the "undefined reference to" error][01].
[The crux of the problem][02] is that compiler needs to know the implementations to be used
(found in the  `*.cpp` files) to make use of the templates,
and the compiler isn't smart enough to look beyond the header file where the template is defined.
While using `inline` code for functions and putting the implementation
in the header file is a popular solution, there are [alternatives][01].

I choose to stick with the traditional design of separate `*.h` and `*.cpp` files,
but explicitly declare instantiate of the template with the `*.cpp` file.
This will ensure that when the compiler is compiling `*.cpp`
the compiler has all the code needed to leverage the templates.

While not elegant, since you have to explicitly declared every function,
this approach works and the code is very readable.
Also, identifing what functions need to be explicitly declared is easy,
since the compiler places a full discription within its error message.
A little cut & paste from the compiler error output and you have what you need.

# Removal of Debug Code When Not Needed
I want to make sure that the debugging code isn't present when I'm done debugging.
I want to use [C++ macros][03] to make this happen so I can bring the debugging code
back if needed later.
Assumes you are using the [C99 C programming language standard][04],
you can create variable argument macros to complement the templeted functions you created.
You'll see the `do { ... } while (0)` used within the macros.
This idiom ensures that the code acts like a statement (i.e. function call).
The `if (DEBUG)` condition used within the macro code ensures that the compiler
always checks that your debug code is valid,
but the compiler optimizer will remove the code when `DEBUG` is 0 (aka `false`).

# Sources
* [What’s the idea behind templates](http://web.archive.org/web/20190621081506/https://isocpp.org/wiki/faq/templates#overview-templates)
* [C++ Undefined Reference to... using Templates](https://www.sololearn.com/Discuss/2184286/c-undefined-reference-to-using-templates)
* [C++: “undefined reference to” templated class function][01]



[01]:https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
[02]:https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
[03]:https://gcc.gnu.org/onlinedocs/cpp/Macros.html
[04]:https://en.wikipedia.org/wiki/C99
[05]:
[06]:
[07]:
[08]:
[09]:
[10]:
