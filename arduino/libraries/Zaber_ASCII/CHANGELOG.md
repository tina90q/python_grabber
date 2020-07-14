# Changelog
## v1.2.0 (2019-04)

* Added support for the Zaber shield. The new ZaberShield class can be used instead of Serial
  to communicate with devices using the Zaber shield.
* Added an isIdle() method that tells you if a device has stopped moving without blocking program
  execution until the device does stop moving.
* Moved the getting started documentation from the Zaber wiki to local markdown format.
* Added a new example program, "Track Manual Moves", that demonstrates how to process multiple responses
  to a broadcast command and how to avoid blocking your program while waiting for a response.
* Added a new example program, "2D Scan", that demonstrates how to control two axes to visit locations
  on a grid pattern.

## v1.1.2 (2019-02)

* Fixed send() helper methods only accepting a 16-bit data value; they now take 32-bit input for the data payload.

## v1.1.1 (2018-09)

* Fixed an infinite loop bug in pollUntilIdle().
* Removed some superfluous include statements that caused problems under Linux.
* Improved examples to demonstrate consumption of devices' replies to commands; not consuming the
  replies can lead to serial port receive buffer overflow and corruption of subsequent replies.

## v1.1 (2018-05)

* Updated link to user guide.

## v1.0 (2018-03)

* Initial release