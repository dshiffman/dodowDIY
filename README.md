# dodowDIY
**An ATTiny85 implementation of the well known sleep aid. Includes circuit, software and 3d printed case design**  
The STL shells are desiged around parts I had hanging around - easy to remodel for different batteries, switches, etc.

Parts list:  
* ATTiny85 (I used a DIP8 version)  
* 8 pin socket  
* 47 ohm resistor  
* micro momentary push button switch  
* micro spst switch  
* 3x 5mm blue leds  
* CR17345 or DL123A battery
* battery terminals  
* Some proto-board (20mm wide if you want to use my STLs unaltered)

you'll also need a way to program the MCU (I used a Tiny Programmer)

By-the-way, if everything is running 8 times too slow, it's probably because the 'low' fuse in your ATTiny is programmed to divide the clock by 8 (fuse - 0x62) to get the device to run at 8mhz, burn the low fuse to 0xE2 instead.

There's no on/off switch because at the end of the cycle the ATTiny is put to sleep and only draws a few micro-amps.  Given the 1.4 amp-hour capacity of the battery it will last for years.

That's it!

STL files, Source Files are in the obvious places.

The Pics folder has the assembly at various stages to give you an idea of the approach.

![here are the parts I used](https://github.com/dshiffman/dodowDIY/blob/main/dodowdiy%20parts.jpg)

https://www.thingiverse.com/thing:4761399



