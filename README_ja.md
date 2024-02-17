[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ISD17XX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/isd17xx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

Nuvoton ISD1700 ChipCorder シリーズは、さまざまな電子システムに最適な、高品質で完全に統合されたシングルチップ マルチメッセージ音声録音および再生デバイスです。 メッセージの長さは、特定のデバイスに応じて、26 秒から 120 秒の範囲でユーザーが選択できます。 各デバイスのサンプリング周波数は、外部抵抗器を使用して 4 kHz から 12 kHz まで調整することもできます。これにより、ユーザーはアプリケーションごとに録音時間と録音品質の柔軟性を高めることができます。 動作電圧範囲は 2.4 V ～ 5.5 V で、ISD1700 デバイスは幅広いバッテリまたはライン電源アプリケーション向けに最適化されています。

LibDriver ISD17XX は、LibDriver によって起動される isd17xx のフル機能ドライバーです。 録音、再生、その他の機能を提供します。 LibDriver は MISRA 準拠です。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver ISD17XXのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver ISD17XX用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver ISD17XXドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver ISD17XXプログラミング例が含まれています。

/ docディレクトリには、LibDriver ISD17XXオフラインドキュメントが含まれています。

/ datasheetディレクトリには、ISD17XXデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/isd17xx/index.html](https://www.libdriver.com/docs/isd17xx/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。