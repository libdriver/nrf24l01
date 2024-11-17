### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

CE Pin: CE PA8.

INT Pin: INT PB0.

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

### 3. NRF24L01

#### 3.1 Command Instruction

1. Show nrf24l01 chip and driver information.

   ```shell
   nrf24l01 (-i | --information)
   ```

2. Show nrf24l01 help.

   ```shell
   nrf24l01 (-h | --help)
   ```

3. Show nrf24l01 pin connections of the current board.

   ```shell
   nrf24l01 (-p | --port)
   ```

4. Run nrf24l01 register test.

   ```shell
   nrf24l01 (-t reg | --test=reg)
   ```

5. Run nrf24l01 send test.

   ```shell
   nrf24l01 (-t send | --test=send)
   ```

6. Run nrf24l01 receive test.

   ```shell
   nrf24l01 (-t receive | --test=receive)
   ```

7. Run nrf24l01 send function, str is the send data and it's length must be less 32.

   ```shell
   nrf24l01 (-e send | --example=send) (--channel=<0 | 1 | 2 | 3 | 4 | 5>) --data=<str>
   ```

8. Run nrf24l01 receive function, ms is the timeout in ms.

   ```shell
   nrf24l01 (-e receive | --example=receive) (--timeout=<ms>)
   ```

#### 3.2 Command Example

```shell
nrf24l01 -i

nrf24l01: chip is Nordic nRF24L01.
nrf24l01: manufacturer is Nordic.
nrf24l01: interface is SPI.
nrf24l01: driver version is 1.0.
nrf24l01: min supply voltage is 1.9V.
nrf24l01: max supply voltage is 3.6V.
nrf24l01: max current is 13.50mA.
nrf24l01: max temperature is 85.0C.
nrf24l01: min temperature is -40.0C.
```

```shell
nrf24l01 -p

nrf24l01: SCK connected to GPIOA PIN5.
nrf24l01: MISO connected to GPIOA PIN6.
nrf24l01: MOSI connected to GPIOA PIN7.
nrf24l01: CS connected to GPIOA PIN4.
nrf24l01: CE connected to GPIOA PIN8.
nrf24l01: INT connected to GPIOB PIN0.
```

```shell
nrf24l01 -t reg

nrf24l01: chip is Nordic nRF24L01.
nrf24l01: manufacturer is Nordic.
nrf24l01: interface is SPI.
nrf24l01: driver version is 1.0.
nrf24l01: min supply voltage is 1.9V.
nrf24l01: max supply voltage is 3.6V.
nrf24l01: max current is 13.50mA.
nrf24l01: max temperature is 85.0C.
nrf24l01: min temperature is -40.0C.
nrf24l01: start register test.
nrf24l01: nrf24l01_set_config/nrf24l01_get_config test.
nrf24l01: disable power up.
nrf24l01: check config ok.
nrf24l01: enable power up.
nrf24l01: check config ok.
nrf24l01: disable encoding 2 bytes.
nrf24l01: check config ok.
nrf24l01: enable encoding 2 bytes.
nrf24l01: check config ok.
nrf24l01: disable crc.
nrf24l01: check config ok.
nrf24l01: enable crc.
nrf24l01: check config ok.
nrf24l01: disable max rt.
nrf24l01: check config ok.
nrf24l01: enable max rt.
nrf24l01: check config ok.
nrf24l01: disable tx ds.
nrf24l01: check config ok.
nrf24l01: enable tx ds.
nrf24l01: check config ok.
nrf24l01: disable rx dr.
nrf24l01: check config ok.
nrf24l01: enable rx dr.
nrf24l01: check config ok.
nrf24l01: nrf24l01_set_mode/nrf24l01_get_mode test.
nrf24l01: set tx mode.
nrf24l01: check mode ok.
nrf24l01: set rx mode.
nrf24l01: check mode ok.
nrf24l01: nrf24l01_set_auto_acknowledgment/nrf24l01_get_auto_acknowledgment test.
nrf24l01: disable pipe 0 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: enable pipe 0 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: disable pipe 1 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: enable pipe 1 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: disable pipe 2 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: enable pipe 2 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: disable pipe 3 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: enable pipe 3 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: disable pipe 4 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: enable pipe 4 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: disable pipe 5 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: enable pipe 5 auto acknowledgment.
nrf24l01: check auto acknowledgment ok.
nrf24l01: nrf24l01_set_rx_pipe/nrf24l01_get_rx_pipe test.
nrf24l01: disable rx pipe 0.
nrf24l01: check rx pipe ok.
nrf24l01: enable rx pipe 0.
nrf24l01: check rx pipe ok.
nrf24l01: disable rx pipe 1.
nrf24l01: check rx pipe ok.
nrf24l01: enable rx pipe 1.
nrf24l01: check rx pipe ok.
nrf24l01: disable rx pipe 2.
nrf24l01: check rx pipe ok.
nrf24l01: enable rx pipe 2.
nrf24l01: check rx pipe ok.
nrf24l01: disable rx pipe 3.
nrf24l01: check rx pipe ok.
nrf24l01: enable rx pipe 3.
nrf24l01: check rx pipe ok.
nrf24l01: disable rx pipe 4.
nrf24l01: check rx pipe ok.
nrf24l01: enable rx pipe 4.
nrf24l01: check rx pipe ok.
nrf24l01: disable rx pipe 5.
nrf24l01: check rx pipe ok.
nrf24l01: enable rx pipe 5.
nrf24l01: check rx pipe ok.
nrf24l01: nrf24l01_set_address_width/nrf24l01_get_address_width test.
nrf24l01: set address width illegal.
nrf24l01: check address width ok.
nrf24l01: set address width 3 bytes.
nrf24l01: check address width ok.
nrf24l01: set address width 4 bytes.
nrf24l01: check address width ok.
nrf24l01: set address width 5 bytes.
nrf24l01: check address width ok.
nrf24l01: nrf24l01_set_address_width/nrf24l01_get_address_width test.
nrf24l01: set auto retransmit delay 13.
nrf24l01: check auto retransmit delay ok.
nrf24l01: nrf24l01_auto_retransmit_delay_convert_to_register/nrf24l01_auto_retransmit_delay_convert_to_data test.
nrf24l01: check auto retransmit delay convert 3000 12 3000.
nrf24l01: nrf24l01_set_auto_retransmit_count/nrf24l01_get_auto_retransmit_count test.
nrf24l01: set auto retransmit count 4.
nrf24l01: check auto retransmit count ok.
nrf24l01: nrf24l01_set_channel_frequency/nrf24l01_get_channel_frequency test.
nrf24l01: set channel frequency 5.
nrf24l01: check channel frequency ok.
nrf24l01: nrf24l01_set_continuous_carrier_transmit/nrf24l01_get_continuous_carrier_transmit test.
nrf24l01: disable continuous carrier transmit.
nrf24l01: check continuous carrier transmit ok.
nrf24l01: enable continuous carrier transmit.
nrf24l01: check continuous carrier transmit ok.
nrf24l01: nrf24l01_set_force_pll_lock_signal/nrf24l01_get_force_pll_lock_signal test.
nrf24l01: enable force pll lock signal.
nrf24l01: check force pll lock signal ok.
nrf24l01: disable force pll lock signal.
nrf24l01: check force pll lock signal ok.
nrf24l01: nrf24l01_set_data_rate/nrf24l01_get_data_rate test.
nrf24l01: data rate 1M.
nrf24l01: check data rate ok.
nrf24l01: data rate 2M.
nrf24l01: check data rate ok.
nrf24l01: data rate 250K.
nrf24l01: check data rate ok.
nrf24l01: nrf24l01_set_output_power/nrf24l01_get_output_power test.
nrf24l01: set output power -18 dBm.
nrf24l01: check output power ok.
nrf24l01: set output power -12 dBm.
nrf24l01: check output power ok.
nrf24l01: set output power -6 dBm.
nrf24l01: check output power ok.
nrf24l01: set output power 0 dBm.
nrf24l01: check output power ok.
nrf24l01: nrf24l01_set_rx_pipe_0_address/nrf24l01_get_rx_pipe_0_address test.
nrf24l01: set rx pipe 0 addr 0x07 0x45 0xC8 0x7F 0x13.
nrf24l01: check address ok with 5.
nrf24l01: nrf24l01_set_rx_pipe_1_address/nrf24l01_get_rx_pipe_1_address test.
nrf24l01: set rx pipe 1 addr 0xC4 0x6C 0x2B 0xBA 0x61.
nrf24l01: check address ok with 5.
nrf24l01: nrf24l01_set_rx_pipe_2_address/nrf24l01_get_rx_pipe_2_address test.
nrf24l01: set rx pipe 2 addr 0x65.
nrf24l01: check address ok.
nrf24l01: nrf24l01_set_rx_pipe_3_address/nrf24l01_get_rx_pipe_3_address test.
nrf24l01: set rx pipe 3 addr 0xA1.
nrf24l01: check address ok.
nrf24l01: nrf24l01_set_rx_pipe_4_address/nrf24l01_get_rx_pipe_4_address test.
nrf24l01: set rx pipe 4 addr 0x72.
nrf24l01: check address ok.
nrf24l01: nrf24l01_set_rx_pipe_5_address/nrf24l01_get_rx_pipe_5_address test.
nrf24l01: set rx pipe 5 addr 0x64.
nrf24l01: check address ok.
nrf24l01: nrf24l01_set_tx_address/nrf24l01_get_tx_address test.
nrf24l01: set tx addr 0x39 0x3D 0xEE 0xD2 0xB5.
nrf24l01: check address ok with 5.
nrf24l01: nrf24l01_set_pipe_0_payload_number/nrf24l01_get_pipe_0_payload_number test.
nrf24l01: set pipe 0 payload number 26.
nrf24l01: check payload number ok.
nrf24l01: nrf24l01_set_pipe_1_payload_number/nrf24l01_get_pipe_1_payload_number test.
nrf24l01: set pipe 1 payload number 25.
nrf24l01: check payload number ok.
nrf24l01: nrf24l01_set_pipe_2_payload_number/nrf24l01_get_pipe_2_payload_number test.
nrf24l01: set pipe 2 payload number 8.
nrf24l01: check payload number ok.
nrf24l01: nrf24l01_set_pipe_3_payload_number/nrf24l01_get_pipe_3_payload_number test.
nrf24l01: set pipe 3 payload number 8.
nrf24l01: check payload number ok.
nrf24l01: nrf24l01_set_pipe_4_payload_number/nrf24l01_get_pipe_4_payload_number test.
nrf24l01: set pipe 4 payload number 9.
nrf24l01: check payload number ok.
nrf24l01: nrf24l01_set_pipe_5_payload_number/nrf24l01_get_pipe_5_payload_number test.
nrf24l01: set pipe 5 payload number 10.
nrf24l01: check payload number ok.
nrf24l01: nrf24l01_set_pipe_dynamic_payload/nrf24l01_get_pipe_dynamic_payload test.
nrf24l01: disable pipe 0 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: enable pipe 0 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: disable pipe 1 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: enable pipe 1 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: disable pipe 2 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: enable pipe 2 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: disable pipe 3 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: enable pipe 3 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: disable pipe 4 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: enable pipe 4 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: disable pipe 5 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: enable pipe 5 dynamic payload.
nrf24l01: check pipe dynamic payload ok.
nrf24l01: nrf24l01_set_dynamic_payload/nrf24l01_get_dynamic_payload test.
nrf24l01: disable dynamic payload.
nrf24l01: check dynamic payload ok.
nrf24l01: enable dynamic payload.
nrf24l01: check dynamic payload ok.
nrf24l01: nrf24l01_set_payload_with_ack/nrf24l01_get_payload_with_ack test.
nrf24l01: disable payload with ack.
nrf24l01: check payload with ack ok.
nrf24l01: enable payload with ack.
nrf24l01: check payload with ack ok.
nrf24l01: nrf24l01_set_tx_payload_with_no_ack/nrf24l01_get_tx_payload_with_no_ack test.
nrf24l01: disable tx payload with no ack.
nrf24l01: check tx payload with no ack ok.
nrf24l01: enable tx payload with no ack.
nrf24l01: check tx payload with no ack ok.
nrf24l01: nrf24l01_read_rx_payload test.
nrf24l01: check rx payload ok.
nrf24l01: nrf24l01_write_tx_payload test.
nrf24l01: check tx payload ok.
nrf24l01: nrf24l01_get_interrupt test.
nrf24l01: check rx_dr enable.
nrf24l01: check tx_ds enable.
nrf24l01: check max_rt enable.
nrf24l01: check tx_full enable.
nrf24l01: nrf24l01_clear_interrupt test.
nrf24l01: check rx_dr ok.
nrf24l01: check tx_ds ok.
nrf24l01: check max_rt ok.
nrf24l01: check tx_full ok.
nrf24l01: nrf24l01_get_data_pipe_number test.
nrf24l01: get data pipe number 7.
nrf24l01: nrf24l01_get_lost_packet_count test.
nrf24l01: get lost packet count 0.
nrf24l01: nrf24l01_get_retransmitted_packet_count test.
nrf24l01: get retransmitted packet count 0.
nrf24l01: nrf24l01_get_received_power_detector test.
nrf24l01: get received power detector disable.
nrf24l01: nrf24l01_get_fifo_status test.
nrf24l01: get fifo status 0x01.
nrf24l01: nrf24l01_get_rx_payload_width test.
nrf24l01: get rx payload width 0x00.
nrf24l01: nrf24l01_flush_tx test.
nrf24l01: flush tx ok.
nrf24l01: nrf24l01_flush_rx test.
nrf24l01: flush rx ok.
nrf24l01: nrf24l01_reuse_tx_payload test.
nrf24l01: reuse tx payload ok.
nrf24l01: nrf24l01_write_payload_with_ack test.
nrf24l01: write payload with ack ok.
nrf24l01: write payload with ack ok.
nrf24l01: write payload with ack ok.
nrf24l01: write payload with ack ok.
nrf24l01: write payload with ack ok.
nrf24l01: write payload with ack ok.
nrf24l01: nrf24l01_write_payload_with_no_ack test.
nrf24l01: write payload with no ack ok.
nrf24l01: nrf24l01_nop test.
nrf24l01: nop ok.
nrf24l01: finish register test.
```

```shell
nrf24l01 -t send  

nrf24l01: chip is Nordic nRF24L01.
nrf24l01: manufacturer is Nordic.
nrf24l01: interface is SPI.
nrf24l01: driver version is 1.0.
nrf24l01: min supply voltage is 1.9V.
nrf24l01: max supply voltage is 3.6V.
nrf24l01: max current is 13.50mA.
nrf24l01: max temperature is 85.0C.
nrf24l01: min temperature is -40.0C.
nrf24l01: start send test.
nrf24l01: irq send ok.
nrf24l01: irq send ok.
nrf24l01: irq send ok.
nrf24l01: irq send ok.
nrf24l01: irq send ok.
nrf24l01: irq send ok.
nrf24l01: finish send test.
```

```shell
nrf24l01 -t receive 

nrf24l01: chip is Nordic nRF24L01.
nrf24l01: manufacturer is Nordic.
nrf24l01: interface is SPI.
nrf24l01: driver version is 1.0.
nrf24l01: min supply voltage is 1.9V.
nrf24l01: max supply voltage is 3.6V.
nrf24l01: max current is 13.50mA.
nrf24l01: max temperature is 85.0C.
nrf24l01: min temperature is -40.0C.
nrf24l01: start receive test.
nrf24l01: irq tx full.
nrf24l01: irq receive with pipe 0 with 32.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F .
nrf24l01: irq tx full.
nrf24l01: irq receive with pipe 1 with 32.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F .
nrf24l01: irq tx full.
nrf24l01: irq receive with pipe 2 with 32.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F .
nrf24l01: irq tx full.
nrf24l01: irq receive with pipe 3 with 32.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F .
nrf24l01: irq tx full.
nrf24l01: irq receive with pipe 4 with 32.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F .
nrf24l01: irq tx full.
nrf24l01: irq receive with pipe 5 with 32.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F .
nrf24l01: finish receive test.
```

```shell
nrf24l01 -e send --channel=0 --data=libdriver-nrf24l01

nrf24l01: send libdriver-nrf24l01.
nrf24l01: irq send ok.
```

```shell
nrf24l01 -e receive --timeout=5000

nrf24l01: receiving with timeout 5000 ms.
nrf24l01: irq receive with pipe 0 with 18.
0x6C 0x69 0x62 0x64 0x72 0x69 0x76 0x65 0x72 0x2D 0x6E 0x72 0x66 0x32 0x34 0x6C 0x30 0x31 .
nrf24l01: finish receiving.
```

```shell
nrf24l01 -h

Usage:
  nrf24l01 (-i | --information)
  nrf24l01 (-h | --help)
  nrf24l01 (-p | --port)
  nrf24l01 (-t reg | --test=reg)
  nrf24l01 (-t send | --test=send)
  nrf24l01 (-t receive | --test=receive)
  nrf24l01 (-e send | --example=send) [--channel=<0 | 1 | 2 | 3 | 4 | 5>] [--data=<str>]
  nrf24l01 (-e receive | --example=receive) [--timeout=<ms>]

Options:
      --channel=<0 | 1 | 2 | 3 | 4 | 5>
                        Set the send channel.([default: 0])
      --data=<str>      Set the send data and it's length must be less 32.([default: LibDriver])
  -e <send | receive>, --example=<send | receive>
                        Run the driver example.
  -h, --help            Show the help.
  -i, --information     Show the chip information.
  -p, --port            Display the pin connections of the current board.
  -t <reg | send | receive>, --test=<reg | send | receive>
                        Run the driver test.
      --timeout=<ms>    Set the receive timeout in ms.([default: 5000])
```

