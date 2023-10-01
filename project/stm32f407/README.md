### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

RESET Pin: RST PA8.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. ISD17XX

#### 3.1 Command Instruction

1. Show isd17xx chip and driver information.

   ```shell
   isd17xx (-i | --information)
   ```

2. Show isd17xx help.

   ```shell
   isd17xx (-h | --help)
   ```

3. Show isd17xx pin connections of the current board.

   ```shell
   isd17xx (-p | --port)
   ```

4. Run isd17xx reg test.

   ```shell
   isd17xx (-t reg | --test=reg) [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
   ```

5. Run isd17xx audio test.

   ```shell
   isd17xx (-t audio | --test=audio) [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
   ```

6. Run isd17xx record function. start and end is the record start and end address and both are hexadecimal. 

   ```shell
   isd17xx (-e record | --example=record) [--start=<hex>] [--end=<hex>] [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
   ```

7. Run isd17xx play function. start and end is the play start and end address and both are hexadecimal. 

   ```shell
   isd17xx (-e play | --example=play) [--start=<hex>] [--end=<hex>] [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
   ```

8. Run isd17xx global erase function.

   ```shell
   isd17xx (-e global-erase | --example=global-erase) [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
   ```

#### 3.2 Command Example

```shell
isd17xx -i

isd17xx: chip is Nuvoton ISD17XX.
isd17xx: manufacturer is Nuvoton.
isd17xx: interface is SPI.
isd17xx: driver version is 1.0.
isd17xx: min supply voltage is 2.4V.
isd17xx: max supply voltage is 5.5V.
isd17xx: max current is 20.00mA.
isd17xx: max temperature is 85.0C.
isd17xx: min temperature is -40.0C.
```

```shell
isd17xx -p

isd17xx: SPI interface SCK connected to GPIOA PIN5.
isd17xx: SPI interface MISO connected to GPIOA PIN6.
isd17xx: SPI interface MOSI connected to GPIOA PIN7.
isd17xx: SPI interface CS connected to GPIOA PIN4.
isd17xx: RESET GPIO connected to GPIOA PIN8.
```

```shell
isd17xx -t reg --type=ISD1760

isd17xx: chip is Nuvoton ISD17XX.
isd17xx: manufacturer is Nuvoton.
isd17xx: interface is SPI.
isd17xx: driver version is 1.0.
isd17xx: min supply voltage is 2.4V.
isd17xx: max supply voltage is 5.5V.
isd17xx: max current is 20.00mA.
isd17xx: max temperature is 85.0C.
isd17xx: min temperature is -40.0C.
isd17xx: start register test.
isd17xx: isd17xx_set_type/isd17xx_get_type test.
isd17xx: set type isd1730.
isd17xx: check type ok.
isd17xx: set type isd1740.
isd17xx: check type ok.
isd17xx: set type isd1750.
isd17xx: check type ok.
isd17xx: set type isd1760.
isd17xx: check type ok.
isd17xx: set type isd1790.
isd17xx: check type ok.
isd17xx: set type isd17120.
isd17xx: check type ok.
isd17xx: set type isd17150.
isd17xx: check type ok.
isd17xx: set type isd17180.
isd17xx: check type ok.
isd17xx: set type isd17210.
isd17xx: check type ok.
isd17xx: set type isd17240.
isd17xx: check type ok.
isd17xx: isd17xx_set_volume_control/isd17xx_get_vol_control test.
isd17xx: set button volume control.
isd17xx: check volume control ok.
isd17xx: set register volume control.
isd17xx: check volume control ok.
isd17xx: isd17xx_set_volume/isd17xx_get_volume test.
isd17xx: set volume 0x0.
isd17xx: check volume ok.
isd17xx: isd17xx_set_monitor_input/isd17xx_get_monitor_input test.
isd17xx: enable monitor input.
isd17xx: check monitor input ok.
isd17xx: disable monitor input.
isd17xx: check monitor input ok.
isd17xx: isd17xx_set_mix_input/isd17xx_get_mix_input test.
isd17xx: enable mix input.
isd17xx: check mix input ok.
isd17xx: disable mix input.
isd17xx: check mix input ok.
isd17xx: isd17xx_set_sound_effect_editing/isd17xx_get_sound_effect_editing test.
isd17xx: enable sound effect editing.
isd17xx: check sound effect editing ok.
isd17xx: disable sound effect editing.
isd17xx: check sound effect editing ok.
isd17xx: isd17xx_set_spi_ft/isd17xx_get_spi_ft test.
isd17xx: enable spi ft.
isd17xx: check spi ft ok.
isd17xx: disable spi ft.
isd17xx: check spi ft ok.
isd17xx: isd17xx_set_analog_output/isd17xx_get_analog_output test.
isd17xx: set analog output aud.
isd17xx: check analog output ok.
isd17xx: set analog output aux.
isd17xx: check analog output ok.
isd17xx: isd17xx_set_pwm_speaker/isd17xx_get_pwm_speaker test.
isd17xx: enable pwm speaker.
isd17xx: check pwm speaker ok.
isd17xx: disable pwm speaker.
isd17xx: check pwm speaker ok.
isd17xx: isd17xx_set_power_up_analog_output/isd17xx_get_power_up_analog_output test.
isd17xx: enable power up analog output.
isd17xx: check power up analog output ok.
isd17xx: disable power up analog output.
isd17xx: check power up analog output ok.
isd17xx: isd17xx_set_v_alert/isd17xx_get_v_alert test.
isd17xx: enable v alert.
isd17xx: check v alert ok.
isd17xx: disable v alert.
isd17xx: check v alert ok.
isd17xx: isd17xx_set_eom/isd17xx_get_eom test.
isd17xx: enable eom.
isd17xx: check eom ok.
isd17xx: disable eom.
isd17xx: check eom ok.
isd17xx: isd17xx_load_from_nv test.
isd17xx: load from nv.
isd17xx: check load from nv ok.
isd17xx: isd17xx_get_device_id test.
isd17xx: device id is 0x14.
isd17xx: isd17xx_read_play_point/isd17xx_read_record_point test.
isd17xx: play point is 0x0010.
isd17xx: record point is 0x0010.
isd17xx: isd17xx_get_status test.
isd17xx: status1 is 0x0400.
isd17xx: status2 is 0x01.
isd17xx: isd17xx_clear_interrupt test.
isd17xx: check clear interrupt ok.
isd17xx: isd17xx_toggle_extern_clock test.
isd17xx: check toggle extern clock ok.
isd17xx: check toggle extern clock ok.
isd17xx: isd17xx_reset test.
isd17xx: check reset ok.
isd17xx: finish register test.
```

```shell
isd17xx -t audio --type=ISD1760

isd17xx: chip is Nuvoton ISD17XX.
isd17xx: manufacturer is Nuvoton.
isd17xx: interface is SPI.
isd17xx: driver version is 1.0.
isd17xx: min supply voltage is 2.4V.
isd17xx: max supply voltage is 5.5V.
isd17xx: max current is 20.00mA.
isd17xx: max temperature is 85.0C.
isd17xx: min temperature is -40.0C.
isd17xx: start play record test.
isd17xx: start global erase.
isd17xx: start erase.
isd17xx: please speak...
isd17xx: start play.
isd17xx: goto next.
isd17xx: finish play record test.
```

```shell
isd17xx -e record --type=ISD1760 --start=0x00 --end=0xFF

isd17xx: recording...
isd17xx: finish.
```

```shell
isd17xx -e play --type=ISD1760 --start=0x00 --end=0xFF

isd17xx: playing...
isd17xx: finish.
```

```shell
isd17xx -e global-erase --type=ISD1760

isd17xx: global erase.
isd17xx: finish.
```

```shell
isd17xx -h

Usage:
  isd17xx (-i | --information)
  isd17xx (-h | --help)
  isd17xx (-p | --port)
  isd17xx (-t reg | --test=reg) [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
  isd17xx (-t audio | --test=audio) [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
  isd17xx (-e record | --example=record) [--start=<hex>] [--end=<hex>]
          [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
  isd17xx (-e play | --example=play) [--start=<hex>] [--end=<hex>]
          [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]
  isd17xx (-e global-erase | --example=global-erase)
          [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]

Options:
  -e <record | play | global-erase>, --example=<record | play | global-erase>
                       Run the driver example.
      --end=<hex>      Set the end address and it is hexadecimal.([default: 0xFF])
  -h, --help           Show the help.
  -i, --information    Show the chip information.
  -p, --port           Display the pin connections of the current board.
      --start=<hex>    Set the start address and it is hexadecimal.([default: 0x00])
  -t <reg | audio>, --test=<reg | audio>
                       Run the driver test.
      --type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>
                       Set the chip type.([default: ISD1760])
```

