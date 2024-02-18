# f072_voltmeter

This project uses an [STM32F072](https://st.com/en/microcontrollers-microprocessors/stm32f072rb.html) Cortex M0 based MCU in combination with a [CS5509](https://pdf1.alldatasheet.com/datasheet-pdf/view/85317/CIRRUS/CS5509-AS.html) 16 bit ADC to read voltages from 0V up to 3V in unipolar mode. <br>
In this current configuration, fed by a 32.768kHz clock, the ADC sends data at 20 Samples/s through the SPI protocol. <br>
The microcontroller then sends this data to the USART peripheral.
For more precise applications, look at this [application note](https://www.manualsdir.com/manuals/466447/cirrus-logic-an31.html)

### Pin connections
| STM32F072   | Connection | 
|-------------|------------|
| PA2         | USART TX   | 
| PA3         | USART RX   |
| PA5         | SPI SCK    |
| PA6         | SPI MISO   |
| PA7 (unused)| SPI MOSI   |
| PC5         | SPI CS     |
| PC6         | ADC CONV   |
| PC7         | ADC DRDY   |
| PC8         | ADC CAL    |
| PA8         | ADC CLOCK  |
