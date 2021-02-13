#ifndef DEBUG_H_
#define DEBUG_H_

#define NO_PRINT // Comment this out to get diagnostic output

#ifdef NO_PRINT
  #define diagPins(rx, tx)
  #define diagBegin(arg)
  #define diagAvail() true
  #define diagPrint(args...)
  #define diagPrintln(args...)
#else
  #include <SoftwareSerial.h>

  #define diagPins(rx, tx) SoftwareSerial ss (rx, tx)
  #define diagBegin(baud) ss.begin(baud)
  #define diagAvail() ss.available()
  #define diagPrint(args...) ss.print(args)
  #define diagPrintln(args...) ss.println(args)
#endif

#endif /* DEBUG_H_ */
