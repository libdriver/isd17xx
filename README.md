[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ISD17XX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/isd17xx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The Nuvoton ISD1700 ChipCorder Series is a high quality, fully integrated, single-chip multi-message voice record and playback device ideally suited to a variety of electronic systems. The message duration is user selectable in ranges from 26 seconds to 120 seconds, depending on the specific device. The sampling frequency of each device can also be adjusted from 4 kHz to 12 kHz with an external resistor, giving the user greater flexibility in duration versus recording quality for each application. Operating voltage spans a range from 2.4 V to 5.5 V to ensure that the ISD1700 devices are optimized for a wide range of battery or line-powered applications.

LibDriver ISD17XX is the full function driver of isd17xx launched by LibDriver. It provides record, play and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver ISD17XX source files.

/interface includes LibDriver ISD17XX SPI platform independent template.

/test includes LibDriver ISD17XX driver test code and this code can test the chip necessary function simply.

/example includes LibDriver ISD17XX sample code.

/doc includes LibDriver ISD17XX offline document.

/datasheet includes ISD17XX datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface SPI platform independent template and finish your platform SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_isd17xx_basic.h"

uint8_t res;

/* basic init */
res = isd17xx_basic_init(chip_type);
if (res != 0)
{
    isd17xx_interface_debug_print("isd17xx: basic init failed.\n");

    return 1;
}

/* erase */
res = isd17xx_basic_erase(start, end);
if (res != 0)
{
    isd17xx_interface_debug_print("isd17xx: basic erase failed.\n");
    (void)isd17xx_basic_deinit();

    return 1;
}

/* wait erase finished */
res = isd17xx_basic_poll(ISD17XX_STATUS2_ERASE);
if (res != 0)
{
    isd17xx_interface_debug_print("isd17xx: basic poll failed.\n");
    (void)isd17xx_basic_deinit();

    return 1;
}

/* output */
isd17xx_interface_debug_print("isd17xx: recording...\n");

/* record */
res = isd17xx_basic_record(start, end);
if (res != 0)
{
    isd17xx_interface_debug_print("isd17xx: basic record failed.\n");
    (void)isd17xx_basic_deinit();

    return 1;
}

/* wait rec finished */
res = isd17xx_basic_poll(ISD17XX_STATUS2_REC);
if (res != 0)
{
    isd17xx_interface_debug_print("isd17xx: basic poll failed.\n");
    (void)isd17xx_basic_deinit();

    return 1;
}

/* output */
isd17xx_interface_debug_print("isd17xx: playing...\n");

/* play */
res = isd17xx_basic_play(start, end);
if (res != 0)
{
    isd17xx_interface_debug_print("isd17xx: basic play failed.\n");
    (void)isd17xx_basic_deinit();

    return 1;
}

/* wait play finished */
res = isd17xx_basic_poll(ISD17XX_STATUS2_PLAY);
if (res != 0)
{
    isd17xx_interface_debug_print("isd17xx: basic poll failed.\n");
    (void)isd17xx_basic_deinit();

    return 1;
}

/* output */
isd17xx_interface_debug_print("isd17xx: finish.\n");

/* deinit */
(void)isd17xx_basic_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/isd17xx/index.html](https://www.libdriver.com/docs/isd17xx/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.