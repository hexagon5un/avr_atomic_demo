# avr_atomic_demo
Demonstrates problems with interrupts and failure of atomicity / race condition

## What?

This is a quick demo of why you don't want to be sharing 16-bit variables between an interrupt service routine (ISR) and the main body of your code on an AVR, unless you know what you're doing. 

For detailed explanation, see [this Hackaday article](http://wp.me/pk3lN-I79).

(TL;DR: If you don't want to know what you're doing, just wrap it in an `ATOMIC_BLOCK`.)

## No really, what??

Flash the code into an AVR, and notice that the LED is blinking.  Now look through the code and see if you can figure out why.

An ISR is changing a variable between 0x2222 and 0x7777.  We copy that global variable into another local variable, and the LED only lights up if the local variable's value is 0x2277 or 0x7722.  But yet the LED is blinking.  Something is deeply wrong.

The gimmick is that, since it's a 16-bit variable on an 8-bit machine, copying it over takes two machine-language instructions and the interrupt can change the variable when we're halfway through assigning it.  Aha!  But that's not very likely, right?  Have a look at the blinking LED.

The code example also includes the fix: the commented-out `ATOMIC_BLOCK` section.  Un-comment that and you'll never see the LED ever blink again.  Corruption averted, problem solved.

This code snippet gives you a quick way (short of dissassembling the code and thinking real hard) to test out other operations that you fear may not be atomic, or test other solutions to the 16-bit atomicity problem.  Don't hesitate to add some more examples and submit a pull request!

### Arduino folks!

Arduinos run C just fine, and this code will work on any of the AVR-based ones (UNO and older?).  Open up a new sketch, delete the `setup()` and `loop()` functions, and paste the code from `main.c` in its place.  Upload and enjoy.  

Also bask in the glory of having just programmed your Arduino in pure C. 



