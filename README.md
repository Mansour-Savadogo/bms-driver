# param-driver

This is an example use of libcsp and libparam to send commands to a remote device.

Before commands can be sent, you must initialize the serial port you are using. The way this is done depends on your hardware. I will add a sample initialization for a Raspberry Pi later.

# Requirements

libcsp/libparam library

sudo apt install git build-essential libsocketcan-dev can-utils libzmq3-dev pkg-config fonts-powerline python3-pip libelf-dev
sudo pip3 install meson ninja

# Installing
meson setup builddir
meson compile -C builddir

By default the application is not installed on your device. An executable is compiled and can be run using ./builddir/can_test.


# Printing the board
Full CAM files are contained in CAM_Files. The Gerber files can be read by most ECAD software or sent directly to a fab to be printed.
