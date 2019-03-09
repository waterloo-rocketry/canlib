# CANLIB

All of the subprojects of Waterloo Rocketry's CAN bus project need the
ability to communicate over CAN. This repository contains the library that all
of them use in order to do so.

At present, this library is primarily only useful for developing new subsystems
for Waterloo Rocketry's CAN bus system. However, all of the code is released
under GPL, so if you want to adapt any of it for your own project, go nuts.

## RocketCAN

RocketCAN is Waterloo Rocketry's project to network several independent electrical
systems onboard our rocket together over a CAN bus. The goal is to encourage
separation of concerns of different subsystems, and to decrease the complexity
of adding additional functionality to the rocket in the future.

For example, several systems onboard the rocket need the ability to communicate
with the ground station before and during flight. Instead of equipping every
subsystem with its own radio transceiver, we have a single board responsible for
all radio communications. All CAN networked systems can send and receive radio
data through it.

At present, there are 5 independent systems that can communicate over this
CAN bus. The PCB designs for each can be found at [this repository]
(https://github.com/waterloo-rocketry/canhw.git). The software, as well as
readme descriptions for each subsystem are developed in the following
independent repositories:

* [Radio Subsystem] (https://github.com/waterloo-rocketry/cansw_radio)
* [Logging Subsystem] (https://github.com/waterloo-rocketry/cansw_logger)
* [Nitrous Vent Valve Subsystem] (https://github.com/waterloo-rocketry/cansw_vent)
* [Injector Valve Subsystem] (https://github.com/waterloo-rocketry/cansw_injector)
* [Sensor Suite Subsystem] (https://github.com/waterloo-rocketry/cansw_sensor)

## Supported Platforms

Canlib is designed to run on all CAN subsystems. All of those systems run on
one of three CAN modules, so those are the three that we support:

1. pic18f26k83: the cheap, low powered 8 bit microcontroller that the majority
of the subsystems run on. The platform specific code for this microcontroller is
found in `pic18f26k83/`. It is meant to be compiled with Microchip's XC8
compiler.
2. dspic33ep512gp502: A higher powered 16 bit digital signal processing
microcontroller that the logging and sensor subsystems run on. The platform
specific code for this microcontroller is found in `dspic33epxxxgp50x/`. It is
meant to be compiled with Microchip's XC16 compiler.
3. MCP2515: A standalone CAN module that interfaces with a host microcontroller
over SPI. This module should be useable with any microcontroller, as long as it
supports SPI. This code is found int `mcp2515/`, and should be buildable by any
compiler.

## Minimum Viable Setup

To see a minimum viable program for each of these platforms, look in the
`tests/` directory. There is a MPLAB project for each platform which contains
the required initialization code that needs to be run by any project using
Canlib on that platform. Each should also show how CAN messages can be sent on
that platform.

## Utilties

The `util/` directory contains some platform independent utilities that may be
helpful in some instances. Each utility module is described separately:

### timing\_util

This module contains some preset `can_timing_params` for the bitrate that the
library is currently running at. At present, all Canlib devices run a CAN bit
time of 24us (4.16 kHz). To initialize a `can_timings_t`, call
`can_generate_timing_params` with the frequency that the CAN module is running
at. In most instances, this will be the same frequency as the microcontroller
core, but can vary depending on project setup. All functions are described in
`util/timing_util.h`

### can\_rcv\_buffer

This module contains a thread safe ring buffer that can be populated with
incoming CAN messages during the CAN ISR, which can later be read out by the
application main thread. The module must be initialized with an external memory
pool. All functions in this module are described in `util/can_rcv_buffer.h`
