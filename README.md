# Introduction
The use of renewable energy is becoming increasingly common in everyday life. One of the most widely used and popular methods is the use of solar panels. For this reason, Battery Management Systems (BMS) are extremely useful, as they are designed to monitor the batteries within the system by measuring their temperature, voltage, and current. This allows for proper control of charging and discharging processes, thereby extending the overall lifespan of the system.

- Add your project logo.
- ![image alt](image_url_logo)


## :ledger: Index
- [About](#beginner-about)
- [Usage](#zap-usage)
  - [Installation](#electric_plug-installation)
  - [Commands](#package-commands)
- [Development](#wrench-development)
  - [Pre-Requisites](#notebook-pre-requisites)
  - [Developmen Environment](#nut_and_bolt-development-environment)
  - [File Structure](#file_folder-file-structure)
  - [Build](#hammer-build)  
  - [Deployment](#rocket-deployment)  
- [Community](#cherry_blossom-community)
  - [Contribution](#fire-contribution)
  - [Branches](#cactus-branches)
  - [Guideline](#exclamation-guideline)  
- [FAQ](#question-faq)
- [Resources](#page_facing_up-resources)
- [Gallery](#camera-gallery)
- [Credit/Acknowledgment](#star2-creditacknowledgment)
- [License](#lock-license)

##  :beginner: About
To achieve the goal of monitoring battery status and properly balancing them, the BQ76920 component is used. This integrated circuit, developed by Texas Instruments, is designed to monitor voltage, current, and temperature in a set of lithium batteries (from 3 to 5 cells). By using this component, the useful life of the batteries can be significantly extended.

Below is a description of the pinout diagram for this component:

  - Voltage pins (VC5–VC0): six inputs that connect to the battery cells to monitor their voltage (Pins 1–6)
  - Positive current sense pin (SRP): positive input for the sense resistor (Pin 7)
  - Negative current sense pin (SRN): negative input for the sense resistor (Pin 8)
  - Alert pin (ALERT): interrupt pin used for device protection (Pin 9)
  - Power supply pin (BAT): main voltage input (Pin 10)
  - Regulator Source pin (REGSRC): input that provides power to the device’s internal regulator (Pin 11)
  - Regulator Output pin (REGOUT): output from the internal regulator that supplies 3.3V (Pin 12)
  - Capacitor pin (CAP1): input for a capacitor that acts as a reference filter (Pin 13)
  - Temperature pin (TS1): input for a temperature sensor (Pin 14)
  - Communication pins (SCL, SDA): I2C communication interface (Pins 15–16)
  - Ground pin (VSS): connection to GND (Pin 17)
  - MOSFET control pins (CHG, DSG): outputs used to control charging and discharging through MOSFETs (Pins 18–19)


![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/BMS_IO.jpg?raw=true)
  
As mentioned earlier, the BMS requires the use of MOSFET transistors, which are used to enable or disable battery charging or discharging depending on the measured conditions such as temperature, voltage, and current. The following MOSFET was selected for this purpose:

  - IRF1404 (MOSFET N-Channel): 
      Vds = 40V
      Vgs = 1V a 2V 
      Id  = 162A
    
![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/MOSFET_IRF1404.png?raw=true)

Since temperature monitoring is essential, a thermistor is also required to protect the lithium cells. It communicates with the microcontroller via I2C, reporting any temperature rise. If an overheating condition is detected, an alarm can be triggered to disable charging. This component is connected to pin 14 and has the following specifications:

  - NTCLE100E3103JB0 (Negative Temperature Coefficient):
      R0 (Resistencia nominal): 10kOhmios
      Rango de temperatura: –40°C a +125°C
      Tolerancia: ±5% 
      Disipacion termica:  0.75mW/°C

![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/NTCLE100E3103JB0.jpg?raw=true)

On the other hand, the microcontroller used is the Black Pill, which integrates an STM32F411CEU6. This chip supports the previously mentioned communication protocol by configuring pins PB8 and PB9 for alternate functions. Further details on the required connections for this chip, as well as for the previously discussed components, will be provided later. Additionally, these elements are not the only ones needed for optimal circuit operation—this will also be covered in detail in the Installation section.

![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/black-pill.jpg?raw=true)
 

## :zap: Usage

In the previous section, we discussed the data collected through the I2C communication interface. Once the voltage, current, and temperature values are available, it becomes possible to determine the appropriate action: either charging or discharging the batteries, using the previously mentioned MOSFETs as actuators.

Whenever the charging pin is activated, the MOSFET connected to pin 18 is disabled, preventing the battery from charging. If the pin is deactivated, charging will begin. Similarly, when the discharge pin is activated, the MOSFET connected to pin 19 allows discharging; otherwise, discharging is disabled. This behavior can be summarized as follows:
  
  - Pin 18 (CHG) = ON -> IRF1404 OFF -> No charging     
  - Pin 18 (CHG) = OFF -> IRF1404 ON -> Charging
  - Pin 19 (DSG) = ON -> IRF1404 ON -> Discharing
  - Pin 19 (DSG) = OFF -> IRF1404 OFF -> No discharing

Now, what are the conditions that must be met to enable or disable charging?

Regarding temperature: If the temperature in any cell exceeds 45 °C, charging is stopped. If it reaches 60 °C, the MOSFETs are completely disabled as a safety measure.

Regarding voltage: Charging is normally active but will be disabled if the voltage exceeds 4.2 V. Discharging will be allowed when another device is connected and the voltage is above 3.1 V; it will be disabled if the voltage falls below that threshold.

To summarize, here’s a decision diagram:
  - Charging:
    Active: continuously, as long as voltage < 4.2 V and temperature < 45 °C
    Inactive: if voltage ≥ 4.2 V or temperature ≥ 45 °C

  - Discharging:
    Active: if an external load is connected and voltage > 3.1 V
    Inactive: if voltage < 3.1 V

It is important to note that each battery cell is monitored individually. In this setup, four out of six available inputs (cells 1 to 4) will be enabled, and each cell has its own independent readings of temperature, current, and voltage.


###  :electric_plug: Installation
In previous sections, the characteristics of the key components used in the battery management system were discussed. This section explains the connections found in the final circuit, as shown below.

![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/BMS_Schematic.jpg?raw=true)


- Microcontroller
  An external 8 MHz crystal oscillator (Y?) is used to ensure maximum performance of the microcontroller. This choice provides greater stability, which is ideal for communication protocols. Alongside the crystal, two 22 pF capacitors (C3 and C4) are connected—one on each side—to stabilize the oscillation. As shown in the diagram, these are connected to pins PC15 and PC14 of the MCU.

  The microcontroller has a dedicated NRST pin, which resets the chip each time it is activated. To perform this action, a push button (SW1) is connected in parallel between a 10 kΩ resistor (R9) and a 0.1 μF capacitor (C5).

  Regarding pin configuration, three pins are required to interface with the BMS: PB9, PB8, and PB1.
The first two (PB9 and PB8) are used for I2C communication, allowing data exchange between devices. In addition, a set of resistors is placed on both the BMS and MCU I2C lines, acting as EMI filters to reduce electrical noise from the environment (R5, R6, R11, and R12).

  The remaining pin (PB1) functions as a flag, meaning that whenever the ALERT pin of the BMS is triggered, the signal is sent to the MCU through this pin.


- BMS
  To explain the connections of this component, we will start from the left side, where three battery cells (CELL1:3) are used. These cells are connected according to the specifications provided by the BQ76920 manufacturer. On the other hand, the SRN and SRP pins are not implemented, as current measurement is not relevant in this case. Therefore, each pin is connected to a 10kΩ resistor (R7 and R8), which is grounded (GND). Finally, the ALERT pin is connected to a 100kΩ resistor (R17) and to the PB1 pin of the microcontroller. It is important to note that the other end of the resistor is connected to VDD, as shown in the diagram.

  Moving to the right side, we have

```
$ add installations steps if you have to.
```


### :notebook: Pre-Requisites
Two main software tools are required for the implementation of this project: one for programming the microcontroller, and another for designing the circuit on a PCB. Below are the tools used, along with installation details:

  - STM32CubeIDE: This is the official development environment from STMicroelectronics for programming STM32 microcontrollers. It integrates various tools such as a code editor (C/C++), compiler, and debugger. This tool is free to use and can be downloaded from:
https://www.st.com/en/development-tools/stm32cubeide.html

  - Altium Designer: This is a professional PCB design software that provides extensive features for electronic circuit design. It includes a comprehensive library of components, symbols, and 3D models. The software allows schematic and PCB design within a single environment and provides error checking, design rule validation, and Gerber file generation. Unlike the previous tool, Altium is not free. It can be downloaded from: https://www.altium.com/altium-designer/downloads

###  :file_folder: File Structure

```
.
├── assets
│   ├── esp32
│   │   ├── Src
│   │   │   ├── main.c
│   ├── stm32F4
│   │   ├── Src
│   │   │   ├── main.c
│   │   ├── drivers
│   │   │   ├── Inc
│   │   │   │   ├── stm32f446xx.h
│   │   │   │   ├── stm32f446xx_adc_driver.h
│   │   │   │   ├── stm32f446xx_gpio_driver.h
│   │   │   │   ├── stm32f446xx_rcc_driver.h
│   │   │   │   ├── stm32f446xx_usart_driver.h
│   │   │   ├── Src
│   │   │   │   ├── stm32f446xx_adc_driver.c
│   │   │   │   ├── stm32f446xx_gpio_driver.c
│   │   │   │   ├── stm32f446xx_rcc_driver.c
│   │   │   │   ├── stm32f446xx_usart_driver.c
├── index.html
└── README.md
```

| No | File Name | Details 
|----|------------|-------|
| 1  | index | Entry point

###  :hammer: Build
Write the build Instruction here.

### :rocket: Deployment
Write the deployment instruction here.

## :cherry_blossom: Community

 ###  :fire: Contribution

 Your contributions are always welcome and appreciated. Following are the things you can do to contribute to this project.

 1. **Report a bug** <br>
 If you think you have encountered a bug, and I should know about it, feel free to report it [here]() and we will take care of it.

 2. **Request a feature** <br>
 You can also request for a feature [here](), and if it will viable, it will be picked for development.  

 3. **Create a pull request** <br>
 It can't get better then this, your pull request will be appreciated by the community. You can get started by picking up any open issues from [here]() and make a pull request.


 ### :cactus: Branches

 I use an agile continuous integration methodology, so the version is frequently updated and development is really fast.

1. **`stage`** is the development branch.

2. **`master`** is the production branch.

3. No other permanent branches should be created in the main repository, you can create feature branches but they should get merged with the master.

**Steps to work with feature branch**

1. To start working on a new feature, create a new branch prefixed with `feat` and followed by feature name. (ie. `feat-FEATURE-NAME`)
2. Once you are done with your changes, you can raise PR.

**Steps to create a pull request**

1. Make a PR to `stage` branch.
2. Comply with the best practices and guidelines e.g. where the PR concerns visual elements it should have an image showing the effect.
3. It must pass all continuous integration checks and get positive reviews.

After this, changes will be merged.


### :exclamation: Guideline
coding guidelines or other things you want people to follow should follow.


## :question: FAQ
You can optionally add a FAQ section about the project.

##  :page_facing_up: Resources
Add important resources here

##  :camera: Gallery
Pictures of your project.

## :star2: Credit/Acknowledgment
Credit the authors here.

##  :lock: License
Add a license here, or a link to it.
