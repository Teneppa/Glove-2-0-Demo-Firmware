# Glove-2-0-Demo-Firmware
This is a demo firmware for the Glove 2.0


## Updates:
03.12.2020:
   * The communication is now syncronous
     You need to send either "f;" or "r;" from unity to get the finger values or quaternions
   * The calibration is now done manually so that the
     Unity script doesn't time out. It seems that the lag had something
     to do with the timeouts, and now it's a lot faster.
     
9.12.2020
   * Fix setCalibrationArray.
   * Add nSensor -variable to store the pressure sensor count
   * So much other stuff that I don't even remember what I've done

This is an open source project, and all of the details are either on my
[YouTube Channel](https://www.youtube.com/channel/UCPtfAE6_21oqjO7Bh2bJcog) or [Blog @ Teukkaniikka.fi](https://www.teukkaniikka.fi/).
