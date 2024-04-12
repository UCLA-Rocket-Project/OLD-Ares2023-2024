# Ares2023-2024
Central Repository for Ares Software and Electronics for the 2023-2024 School Year. This repository contains all software for Avionics and Ground Systems, as well as any additional code/scripts used to support the development, deployment, and/or operation of the software. Additionally, select data and documentation is also version-controlled in this repository, enabling a "one-stop-shop" for everything of interest.


## Base-Level Directory Structure

```
Ares2023-2024
└─── Avionics
  └─── Bodytube
  └─── Ground_Stations
  └─── Nosecone
  └─── Recovery
  └─── Throttle
└─── Data
└─── Ground_Systems
└─── Scripts
  └─── Sensor_Test_Code
```

### Avionics
Contains all Avionics-related software, including microcontroller code from all 5 subsystems and various scripts used for processing data.

### Data
Contains all "important" data that we desire to version-control. This could include logged data from tests and/or full operations (e.g. hotfires, coldflows, launches, etc.).

### Ground_Systems
Contains all Ground Systems-related software, including all software used to enable our Raspberry Pi server, as well as Arduino microcontroller source code and various scripts used for software deployment and system operation.

### Scripts
Contains overall GENERAL scripts, primarily for supporting nominal software development, including various Arduino sensor code. Of course, when implementing into future systems, make sure to check the pinouts.


## Overall Relevant Electronics System Architecture
(ROCKET DRAWING TO BE INSERTED AT LATER DATE)

The architecture diagram omits individual sensors and specific components and only gives a high-level overview of the system. For a more specific component-level description, please refer to the documentation within individual folders.
