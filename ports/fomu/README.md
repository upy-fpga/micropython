# Fomu: Python in your USB Port

This is a minimal port of Micropython to Fomu -- an FPGA in your USB port.

It currently is designed to run on the RISC-V that is run as part of the bootloader.

This has no MUL/DIV unit, and is a simple, two-stage pipeline.  It emulates a CDC-ACM
device as a serial port, and connects that up to the REPL.

## Building and running

Ensure you have a RISC-V compiler in your path.  Then run:

    $ make

To run the executable and get a basic working REPL, load `build/firmware.bin` onto Fomu:

    $ dfu-util -D build/firmware.bin

When you unplug Fomu and plug it back in, you can skip loading firmware and boot straight to Python:

    $ dfu-util -e

You can open a serial terminal to the device and get a Python REPL.  You may select any baudrate you like.