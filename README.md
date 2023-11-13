# Stop-Watch
This is a simple application on Timer1 module working in timermode.
A simple Stopwatch where the user can start/stop, pause/continue, and reset the stopwatch if needed.
The stopwatch starts when the user presses on start and then the Timer1 module is turned on counting exactly 1 second and with each count a variable is incremented in the Interrupt Service Routine (ISR handler).
This variable is then taken and added to multiple equations to extract the required digits (thousands, hundreds, tens, etc....) and then this digit is used to be displayed on the 7-Segment display.
