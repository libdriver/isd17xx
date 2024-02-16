[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ISD17XX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/isd17xx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Die Nuvoton ISD1700 ChipCorder-Serie ist ein hochwertiges, vollständig integriertes Ein-Chip-Multi-Message-Sprachaufzeichnungs- und Wiedergabegerät, das sich ideal für eine Vielzahl elektronischer Systeme eignet. Die Nachrichtendauer ist vom Benutzer in Bereichen von 26 Sekunden bis 120 Sekunden wählbar, abhängig vom jeweiligen Gerät. Die Abtastfrequenz jedes Geräts kann auch mit einem externen Widerstand von 4 kHz bis 12 kHz eingestellt werden, was dem Benutzer eine größere Flexibilität in Bezug auf die Dauer gegenüber der Aufnahmequalität für jede Anwendung bietet. Die Betriebsspannung erstreckt sich über einen Bereich von 2,4 V bis 5,5 V, um sicherzustellen, dass die ISD1700-Bausteine für eine Vielzahl von batterie- oder netzbetriebenen Anwendungen optimiert sind.

LibDriver ISD17XX ist der voll funktionsfähige Treiber von isd17xx, der von LibDriver gestartet wurde. Es bietet Aufnahme-, Wiedergabe- und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver ISD17XX-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver ISD17XX SPI.

/test enthält den Testcode des LibDriver ISD17XX-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver ISD17XX-Beispielcode.

/doc enthält das LibDriver ISD17XX-Offlinedokument.

/Datenblatt enthält ISD17XX-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/isd17xx/index.html](https://www.libdriver.com/docs/isd17xx/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.