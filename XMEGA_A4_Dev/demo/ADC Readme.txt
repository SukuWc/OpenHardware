These examples demonstrate the basic use of the ATXMEGA ADC.

USE ASF Wizzard to include the following modules:

 - ADC
 - TC


The reference voltage for the ADC cannot be higher than VCC-0.6V.
For the sake of simplicity, the following examples use VCC/2 as reference.

When using potentiometer connect series resistor to reduce voltage.


    POTENTIOMETER  SERIES-RESISTOR

        VR=10k   (VCC/2)     R=10k    VCC
   ____/\/\/\/\_____|______/\/\/\/\____|
  |        |
 GND    AnalogIN