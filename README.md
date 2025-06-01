# Introduction
El uso de energias renovables es cada vez mas comun en el dia a dia, uno de los mas usados y famosos es el uso de paneles solares, es por esto
que los sistemas de gestion de baterias son muy utiles (BMS) ya que tiene la finalidad de poder monitorizar las baterias que se encuentran en 
el sistema, conocidendo su temperatura, voltaje y corriente que existe atraves de ellos, y de esta forma poder cargar y descragar las mismas, 
extendiendo la vida util del sistema.

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
Para lograr el objetivo de conocer el estado de las baterias y de esta manera poder realizar un balanceo de las baterias, se utiliza el 
componente BQ76920, el cual es un circuito integrado de la familia de Texas Instruments. Su funcion principal es la de monitorear
voltaje, corriente y termperatura en un conjunto de baterias de litio (3 a 5 celdas), con este componente se podra extender la vida util de 
las baterias.

A continuacion se explicaran el diagrama de pines de este componente:

  - Pin de volatje (VC5-VC0): cuenta con seis entradas que se conectan a la celda de la bateria para monitorear el voltaje (Pin 1:6)
  - Pin de corrient de entrada positiva (SRP): entrada positiva del Sense Resistor (Pin 7)
  - Pin de corrient de entrada negativa (SRN): entrada negativa del Sense Resistor (Pin 8)
  - Pin de alertas (ALERT): cuenta con un pin de interrupcion para proteger el dispositivo (Pin 9)
  - Pin de alimentacion (BAT): entrada principal de volatje (Pin 10)
  - Pin de (REGSRC): Regulator Source, entrada que suministra energia al regulador interno del dispositivo(Pin 11)
  - Pin de regulador interno (REGOUT): Regulator Output, salida del regulador interno que proporciona 3.3V (Pin 12)
  - Pin de capacitor (CAP1): entrada para conectar un capacitor que actue como filtro de referencia interna (Pin 13)
  - Pin de temperatura (TS1): entrada del sensor de temperatura (Pin 14)
  - Pin de comunicacion (SCL-SDA): protocolo de comunicacion I2C (Pin 15:16)
  - Pin de tierra (VSS): entrada para conectar a GND (Pin 17)
  - Pin de control de mosfte (CHG-DSG): salidas de carga y descarga controlada por mosfet (Pin 18:19)

        ![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/BMS_IO.jpg?raw=true)
  
Como se menciono anteriormente para el funcionamiento del BMS, se requiere el uso de trasnsitores MOSFET, que seran de 
utilida para habilitar la carga o descraga de las baterias depeniendo de las condiciones que se hayan leido de temperatura, voltaje y 
corriente, es por esto que se opta por usar el sigueinte mosfte:

  - IRF1404 (MOSFET N-Channel): 
      Vds = 40V
      Vgs = 1V a 2V 
      Id  = 162A
    
        ![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/MOSFET_IRF1404.png?raw=true)

Debido a que se monitorea la temperatura, es necesario el uso de un termistor que funcione como proteccion de las celdas de litio al 
informar, por medio de la comunicacion I2C al microcontrolador, sobre un aumento de temperatura logrando activar una alarma que actuara 
para desactivar la carga. Este componente se conecta a la pin 14 y en este caso cuenta con las sigyientes caracteristivas:

  - NTCLE100E3103JB0 (Negative Temperature Coefficient):
      R0 (Resistencia nominal): 10kOhmios
      Rango de temperatura: –40°C a +125°C
      Tolerancia: ±5% 
      Disipacion termica:  0.75mW/°C

        ![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/NTCLE100E3103JB0.jpg?raw=true)

Por otro lado, el microcontrolador que se implementa es la black-pill que cuenta con un STM32F411CEU6, este chip permite usar la comunicacion
anterioemente mencionada al configurar los pines PB8 y PB9 como funciones especiales. Mas adelante, se indagara en amyor medida sobre las
conexion que se requieren, tanto en este chip como en los mencionados previamente. Ademas, estos elementos, no son los unico que se requieren 
para obtener un optimo funcionamiento del circuito, esto tambien se profundorizara en la seccion de Installation.

        ![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/black-pill.jpg?raw=true)
 

## :zap: Usage
Write about how to use this project.

En el punto anterior, se menciono sobre los datos que se recolectan por medio de la comunicacion I2C. Una vez proporcionada la informacion del 
voltaje, corriente y tmperatura que se tiene, se podra determinar la accion a realizar, ya sea carga o descarga de las baterias, teniendo 
como actuadores el MOSFTE anterioemente mencionado. 

Cada vez que se active el pin de carga, el MOSFET conctado al pin 18 estara deshabilitado provocando que no exista una carga de bateria, 
en dado caso de que fuera controario, la carga empezaria. Por otro lado, si se activa el pin de descarga, el MOSFTE conecatdo al pin 19 
procede con la descarga, caso opuesto estara deshabilitada. En resumen, tendremos un diagram como el siguiente:
  
  - Pin 18 (CHG) = ON -> IRF1404 OFF -> No charging     
  - Pin 18 (CHG) = OFF -> IRF1404 ON -> Charging
  - Pin 19 (DSG) = ON -> IRF1404 ON -> Discharing
  - Pin 19 (DSG) = OFF -> IRF1404 OFF -> No discharing

Ahora, cuales son las condiciones que se tienen que cumplir para la activacion o desactivacion de la carga?
Tomando en cuenta unicamente la termperatura, si esta, en cualquiera de las celdas, es mayor a 45°C la opcion de carga se dentra 
y en caso de que esta pase los 60°C deshabilita los MOSFET.
En el caso del voltaje, se tendra que la carga esta en constante activacion pero se desactivara cuando sea mayor a 4.2V. Por otro lado,
la desacraga estara activa al momento de tener conexion a otro elemento pero el voltaje sea mayor a 3.1V y se desactivara si este voltaje 
es menor que.

En conclusion se tiene el siguiente diagrama:
  - Carga:
      Activa: constante mientras sea > 4.2V & temepratura > 45°C
      Desactiva: voltaje < 4.2V

  - Descarga:
      Activa: elemento conectado y voltaje < 3.1V 
      Desactiva: si es > a 3.1V

Cabe recalcar que las celdas de voltaje se monitorean individualemente, en este caso se habilitaran cuatro de las seis opciones (celda 1:4), 
cada una cuenta con su propio valor de termperatura, corriente y voltaje.


###  :electric_plug: Installation
En puntos previos, se ha explicado las caracteristicas de los compenentes mas especificos que se usaron para la operacion del sistema de 
gestion de baterias. En este nuevo apartado, se explican las conexiones presentes en el circuito final que se muestra a continuacion.

![image alt](https://github.com/davidHdz1425/stm32F4xx_esp32/blob/main/BMS_Schematic.jpg?raw=true)


- Microcontrolador
  Se opta por la implmentacion de un cristal externo de 8MHz (Y?) para garantizar un maximo rendimiento por parte del dispositivo,
  debido a que cuentan con una mayor establidad y son ideales para usar en protocolos de comunicacion. Junto con el cristal, deben de 
  ir conectados un capacitor por lado de un valor de 22pF (C3 & C4) que actuan como estabilizadores de la oscilacion. Como se aprecia en 
  el diagrama se conectan a PC15 y  PC14 del MCU.

  El microcontrolador cuenta con un pin especifico que se denomina NRST, el cual cada vez que se active reiniciara el chip y para ello se 
  cuenta con con un Push Button (SW1) que se conecta en paralelo entre la resistencia de 10kOhmios (R9) y el capacitor de .1uF (C5)
  
  En el tema de la configuracion de los pines del microcontrolador, se requiere el uso de tres pines que se conectan al BMS PB9, PB8 y PB1.
  Los primero dos son los requeridos para la comunicacion I2C y de esta manera transmitir los datos entre los dispotivos. Ademas, se requiere
  un arreglo de resistencias entre cada uno de los pines del BMS como del MCU que actuara como filtro EMI, que se encarga de la reduccion
  de ruido electrico proveniente del entorno (R5, R6, R11 & R12). En cambio el pin sobrante (PB1) tiene la funcionalidad de bandera, esto 
  quiere decir que cada vez que se active el pin de ALERT del BMS le llegara por este medio al MCU.

- BMS
  Se cuenta con el uso de tres celdas, las cuales estaran conectadas acorde a lo establezido por el fabricante del componente BQ76920. Se 
  indica que 
  Los componentes que se requieren en el gestor de baterias 

Se usan cuatro celdas de voltaje, las cuales 


Uso de capacitores en REGOUT, REGSRC: colocar un capacitor entre el pin y VSS para estabilizar el regulador.

BAT, CAP1
BAT: tierra de la bateria

REGOUT:es la salida del regulador interno del BQ76920. Este pin proporciona una tensiOn de 3.3V que alimenta diversos circuitos 
internos del dispositivo, incluyendo:
  - Buffers de comunicaciOn I2C
si no se utiliza REGOUT para alimentar circuitos externos, es obligatorio colocar un capacitor entre REGOUT y VSS para garantizar 
la estabilidad del regulador.

REGSRC: pin de entrada que suministra energia al regulador interno del BQ76920. Este regulador genera una tensiOn de salida estable 
(a traves del pin REGOUT) que se utiliza para alimentar los circuitos internos del dispositivo.
Se recomienda colocar un capacitor de aproximadamente 1uF entre REGSRC y VSS  para estabilizar la entrada del regulador.




- Capacitor: elemento que se conecta entre tierra y el pin 13. Tiene un valor de 1mF y funciona para filtrar la referencia interna
- Para despues (Explicacion del uso de filtros)


- Steps on how to install this project, to use it.
- Be very detailed here, For example, if you have tools which run on different operating systems, write installation steps for all of them.

```
$ add installations steps if you have to.
```


### :notebook: Pre-Requisites
Son necesarios dos softwares para la implemntacion del proyecto, el primero es un software que permita la porgramacion del microcontrolador en
cuestion y el segundo uno que funcione para el diseño del circuito en una PCB. A continuacion se mencionan los utilizados, asi como la forma de instalcion:

- STM32CubeIDE: entorno de programacion de microcontroladores oficial de STMicroelectronics. Este software permite combinar varias herramintas, editor de codigo (C/C++), compliar y debuggear codigo. Esta herrmiantas es gratuita y se puede descaragr de  https://www.st.com/en/development-tools/stm32cubeide.html

- Altium: software de diseño electronico que permite la creacion de PCB. Cuenta con una amplia biblioteca de componentes, simbolos y modelos 3D. Cuenta con un diseño de esquematico y PCB en el mismo enterno y permite visualizar errores electricos, reglas de diseño y Gerbers desde el mismo lugar. A diferecia del anterior, Altium no es gartuito, https://www.altium.com/altium-designer/downloads



###  :file_folder: File Structure
Add a file structure here with the basic details about files, below is an example.

```
.
├── assets
│   ├── css
│   │   ├── index-ui.css
│   │   └── rate-ui.css
│   ├── images
│   │   ├── icons
│   │   │   ├── shrink-button.png
│   │   │   └── umbrella.png
│   │   ├── logo_144.png
│   │   └── Untitled-1.psd
│   └── javascript
│       ├── index.js
│       └── rate.js
├── CNAME
├── index.html
├── rate.html
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
 If you think you have encountered a bug, and I should know about it, feel free to report it [here]() and I will take care of it.

 2. **Request a feature** <br>
 You can also request for a feature [here](), and if it will viable, it will be picked for development.  

 3. **Create a pull request** <br>
 It can't get better then this, your pull request will be appreciated by the community. You can get started by picking up any open issues from [here]() and make a pull request.

 > If you are new to open-source, make sure to check read more about it [here](https://www.digitalocean.com/community/tutorial_series/an-introduction-to-open-source) and learn more about creating a pull request [here](https://www.digitalocean.com/community/tutorials/how-to-create-a-pull-request-on-github).


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
