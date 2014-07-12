# SDRAM Command Analyzer

For Saleae (https://www.saleae.com/) Logic

This won't work on a real SDRAM clocked at high speed, but can decode commands
from a controller clocked at a low speed (Frequency TBD) to validate wait states.

* Basic command decoding of the /CS, /RAS, /CAS, /WE lines working with generator
* Frames are currently sent every cycle. Need to fix this to only change bubble on transition
* Generator is not currently generating a realistic sequence

I'm working on a basic SDRAM controller on an Altera Cyclone IV (DE0-Nano)
and plan to use this for verifying the wait states and FSM transitions.

This could be done with SignalTap logic analyzer on chip, but compilation is much slower
and doesn't offer the awesome analysis capabilities of the Saleae.


### TODO
* Add display of measured delays (cycles spent in NOP between commands)

