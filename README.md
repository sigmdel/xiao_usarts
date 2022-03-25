# xiao_usarts
Extra hardware serial ports on the SAM D21 XIAO

Three PlatformIO projects that show how to enable up to 3 extra hardware serial ports for a total of four on the SAM D21 XIAO by SeeedStudio.

March 25, 2022

**Table of Content**
<!-- TOC -->

- [1. xiao_usarts.cpp](#1-xiao_usartscpp)
- [2. 3usarts.cpp](#2-3usartscpp)
- [3. 4usarts.cpp](#3-4usartscpp)
- [4. Arduino IDE](#4-arduino-ide)
- [5. References](#5-references)
- [6. License](#6-license)

<!-- /TOC -->
## 1. xiao_usarts.cpp 
(in `./xiao_usarts/src/`)

This is a one file project that will create 2 additional instances of the `Uart` class.

`Serial2`: Uses the ALT-SERCOM0 module which, normally, is set up as a SPI interface.
- TX pin: A10 (= ALT-SERCOM0 Pad 2)
- RX pin: A9 (= ALT-SERCOM0 Pad 1)

`Serial3`: Uses the ALT-SERCOM2 module which, normally, is set up as an I²C interface.
- TX pin: A4 (= ALT-SERCOM2 Pad 0)
- RX pin: A5 (= ALT-SERCOM2 Pad 1)

Alternatively, defining the `USE_ALT_SERIAL` macro will set the `Serial3` pin assignment as follows:

- TX pin: A2 (= ALT-SERCOM2 Pad 2)
- RX pin: A3 (= ALT-SERCOM2 Pad 3)

Another macro, `ORDER_MATTERS` sets the order in which the serial port is initialized:

If `ORDER_MATTERS` is defined, then the sequence is as follows
```
  Serial3.begin(USART_BAUD);
  pinPeripheral(A2, PIO_SERCOM_ALT);
  pinPeripheral(A3, PIO_SERCOM_ALT);
```  
which is the correct order. If the macro is not defined, the sequence will be
```
  pinPeripheral(A2, PIO_SERCOM_ALT);
  pinPeripheral(A3, PIO_SERCOM_ALT);
  Serial3.begin(USART_BAUD);
```
and the A2 and A3 peripheral function will be reset by `Serial3.begin()` and fail.


## 2. 3usarts.cpp 
(in `./xiao_usarts/3usarts/src/`)

Basically the same project as `xio_usarts` except for two changes.
- The definitions of the `Uart` instances have been moved to libraries
  `Serial2`,  `Serial3` and `Serial3Alt` in the `lib` library (`.xio_usarts/3usarts/lib/XIAO_extra_serail`.
- The `ORDER_MATTERS` macro is no longer use and the correct sequence when initializing a serial port is always used. 

## 3. 4usarts.cpp
(in `./xiao_usarts/3usarts/src/`)

Added yet another extra serial port whose TX and RX signals are brought out on the SWD pads on the back of the XIAO.

This requires two additional entries in the pin description table in `variants.cpp`:

```C++
 const PinDescription g_APinDescription[]=
 
  ...
  { PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DM
  { PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE } // USB/DP
 
  // 17,18 - SWD Interface - added for 4th hardware USART
  ,
  { PORTA, 30, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, //SWCLK, TX: SERCOM1/PAD[2]
  { PORTA, 31, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE } // SWDIO, RX: SERCOM1/PAD[3]
  } ;
```

## 4. Arduino IDE

If the Arduino IDE is the preferred development environment, then for each of the three  `<proj>usarts`   (where `<proj>` = `xiao_`, `3` and `4`) :

1. Create a directory name `<proj>usarts`
2. Copy the file `<proj>usarts/src/<proj>usarts.cpp` to the newly created directory.
3. Rename the copied file so that its execption is `.ino` instead of `.cpp`.
4. For the `3usarts` and `4usarts` project, copy the content of the `<proj>usarts/src/XIAO_extra_serial/` directory into the newly created directory.
5. For the `3usarts` project, 
     - rename the `Serial3Alt` library
       - `Serial3Alt.h` --> `Serial3Alt.h.hide`
       - `Serial3Alt.cpp` --> `Serial3Alt.cpp.hide`
     - make sure that `USE_ALT_SERIAL3` is NOT defined in `3usarts.ino`.  
      
Here is the directory tree after all these changes.

```
.
├── 3usarts
│   ├── 3usarts.ino
│   ├── Serial2.cpp
│   ├── Serial2.h
│   ├── Serial3Alt.cpp.hide
│   ├── Serial3Alt.h.hide
│   ├── Serial3.cpp
│   └── Serial3.h
├── 4usarts
│   ├── 4usarts.ino
│   ├── Serial2.cpp
│   ├── Serial2.h
│   ├── Serial3.cpp
│   ├── Serial3.h
│   ├── Serial4.cpp
│   └── Serial4.h
└── xiao_usarts
    └── xiao_usarts.ino

3 directories, 15 files
```

When the `Serial3` alternate pin assignement is to be used, "hide" the `Serial3` library and unhide the 
`Serial3Alt` library
```
.
├── 3usarts
│   ├── 3usarts.ino
│   ├── Serial2.cpp
│   ├── Serial2.h
│   ├── Serial3Alt.cpp
│   ├── Serial3Alt.h
│   ├── Serial3.cpp.hide
│   └── Serial3.h.hide
```

and define `USE_ALT_SERIAL3` in `3usarts.ino`.  It may be necessary to close and restart the IDE if there's a complaint about a twice defined `SERCOM2_Handler`; a lot of things are cached in that environment.

See [Getting Started with Seeeduino XIAO](https://wiki.seeedstudio.com/Seeeduino-XIAO/#software) on the SeeedStuoio Wiki for details about using the Arduino IDE and obtaining the correct board defintions.


## 5. References

- [Three, Nay Four Hardware Serial Ports on a SAM D21 XIAO](https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_3usarts_en.html) (2022/03/23) by Michel Deslierres

 - [XIAO Serial Extension 2](http://kio-denshi.com/hp/index.php?Technical%20Information%2Fetc.%2FXIAO%20Serial%20Extension%202) (2021/03/03) by kio denshi

 - [Seeeduino XIAO Serial Communication Interfaces (SERCOM)](https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_sercom_en.html) (2020/05/04-2022/03/23) by Michel Deslierres

## 6. License
Copyright 2022, Michel Deslierres, no rights reserved.

In those jurisdictions where releasing a work into the public domain may be a problem,
the **BSD Zero Clause License** [SPDX:0BSD](https://spdx.org/licenses/0BSD.html) applies.
