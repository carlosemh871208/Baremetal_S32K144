# Baremetal_S32K144
Read readme file for more information about this repository

Repository for S32K144 microcontroller.
It includes startup file, linker script, booloader code, make script for compilation 
and peripherals libraries, this is base in AUTOSAR architecture for BSW and Turing 
Architecture for ASW.

AUTOSAR.
MCAL - low level drivers
HAL - generic abstraction layer
Services - services as diagnostics and RTOS.
Complex driver - Some PID, Fuzzy controllers, neural network.
RTE - Runtime Enviroment. -  Virtual BUS it is necessary to connect BSW and ASW.

Turing. (Model, View, Controller, Configuration, Adapter).
Model - process all the information of the SWC
View - is the block where SWC is connected to HMI.
Controller - Only if SWC need and specifc control that Model cannot handle.
Configuration - Includes configurations and struct for the SWC.
Adapter - Is the block that connect the SWC with the RTE.

Code is implemented following MISRA 2012 rules and coverity.
This project includes unit testing in googletest.

All SWC are modular.

All translations units have comments of their purpose.
Log changes include for tracking, every code documents has it.
