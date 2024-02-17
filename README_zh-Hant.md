[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ISD17XX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/isd17xx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

Nuvoton ISD1700 ChipCoder系列是一款高品質、完全集成的單晶片多消息語音記錄和播放設備，非常適合各種電子系統。 消息持續時間可由用戶選擇，範圍從26秒到120秒，具體取決於設備類型。 每個設備的採樣頻率也可以通過外部電阻器從4kHz調整到12kHz，使用戶在每次應用的持續時間和記錄質量方面具有更大的靈活性。 工作電壓範圍從2.4 V到5.5 V，以確保ISD1700設備針對廣泛的電池或線路供電應用進行優化。

LibDriver ISD17XX是LibDriver推出的ISD17XX全功能驅動，該驅動提供錄製、播放等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver ISD17XX的源文件。

/interface目錄包含了LibDriver ISD17XX與平台無關的SPI總線模板。

/test目錄包含了LibDriver ISD17XX驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver ISD17XX編程範例。

/doc目錄包含了LibDriver ISD17XX離線文檔。

/datasheet目錄包含了ISD17XX數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的SPI總線模板，完成指定平台的SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/isd17xx/index.html](https://www.libdriver.com/docs/isd17xx/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。