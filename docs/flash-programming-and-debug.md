# Flash Programming and Debug
## Flash Programming
STM32F411 black pill have several possible way to be programmed that can be sorted into two categories: ISP for In-System Programming and IAP for In-Application Programming. 

For STM32F411, the user-writtable flash starts from address 0x80000000. In a normal boot, the processor starts reading instruction from that address. (Precisely, the interruption handler of RESET is **b 0x80000000**). 

### The IAP mode
In the case of IAP, the chip always starts from 0x80000000, but a snippet is inserted between 0x80000000 and the real application code. This code may fetch firmware update in different way and re-program the real application code. As the remote update is not a useful feature for this project, we will leave it.

### The ISP mode
In the case of ISP, the application code is written directly from 0x80000000. As we know so far, there are 2 principal ways to do that: by debug interface or by serial communication interface.

### ISP via Serial Interface.
The chip is also programmable by the serial interface. This can be done by choosing the boot mode. For more detail, check the section 2.4 of the reference manual. Here I quote some key sentences.

```
The values on the BOOT pins are latched on the 4th rising edge of SYSCLK after a reset. It is up to the user to set the BOOT1 and BOOT0 pins after reset to select the required boot
mode.

...

BOOT1=0, BOOT0=1: System memory System memory is selected as the boot space.

The embedded bootloader mode is used to reprogram the Flash memory using one of the following serial interfaces:
    • USB OTG FS (PA11/12) in Device mode (DFU: device firmware upgrade).
    ...
```
So one the black pill, one has to connect the chip via USB-C as it uses the USB OTG FS interface. Then one has to hold the NRST button and the BOOT button simultaneously, then release the NRST button while pressing the BOOT button for like 0.5 second. Now the processor is executing the code that is pre-installed by ST that communicates with the USB controller in USB mode.

If your ST-Link USB controller is plugged into your computer, you should see the device detected.

#### STM32Prog

I have tested downloading the program using STM32Prog software. Just chose the USB mode. The UI is quite understandable so that we will not present its usage in detail. 


#### Trouble shooting
```
Error: failed to download Segment[0]
Error: failed to download the file
```
From the end of the section 2.4 of the reference manual:
```
The USART peripherals operate at the internal 16 MHz oscillator (HSI) frequency, while the USB OTG FS require an external clock (HSE) multiple of 1 MHz (ranging from 4 to 26 MHz).
```

From [the documentation of WeAct](https://github.com/WeActTC/MiniSTM32F4x1/blob/master/General%20document/STM32%E9%83%A8%E5%88%86%E4%B8%8B%E8%BD%BD%E7%83%A7%E5%BD%95%E9%97%AE%E9%A2%98%E6%B1%87%E6%80%BB%20_WeAct%E5%B7%A5%E4%BD%9C%E5%AE%A4.pdf) (the designer of black pills), they said in Chinese, I translated here
```
When the temperature is low, the HSI becomes imprecise. As the ISP program uses HSI to measure the HSE and configure the clock, when the imprecision is big enough, the serial communication (USB) suffers from the timing problem.
```

The USB OTG FS requires an external clock (HSE), the crystal on the black pill seems to be sensible to the temperature and may be not precise under low temperature.


### ISP via Debug Interface
The STM32F4 has two debug interfaces: JTAG or SWD. We will use the [SWD interface](https://developer.arm.com/architectures/cpu-architecture/debug-visibility-and-trace/coresight-architecture/serial-wire-debug) since is the most used for the STM32 series.

To connect build up the communication with the STM32 chip, one need a SWD controller. It is quite easy to be found on [google](https://www.google.com/search?q=st-link+v2+usb). So one has to connect the pins from the usb SWD controller to the STM32 chip.

The 

