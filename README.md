# Matrix-16

A project using 4 Colorduino (8x8) boards to drive a 16x16 matrix. Using I2C with a Sparkfun Microview as the master.

## Setup

### Persistent USB serial device names

**Reference**: <http://hintshop.ludvig.co.nz/show/persistent-names-usb-serial-devices/>

Generally when the devices are plugged in they are given serial ports of the form `/dev/ttyUSBx`. To allow for the `platformio.ini` file to contain generic names for the master and slave devices we'll need to do the following.

1. First if they're already plugged in remove the master and slave devices from the USB ports, then
2. Plug in the master device and run `udevadm info -a -n /dev/ttyUSB0 | grep '{serial}' | head -n1`
3. Copy the serial that is displayed e.g. for my Microview acting as I2C master it is **DA00T1QJ**
4. Plug in a slave device and run `udevadm info -a -n /dev/ttyUSB1 | grep '{serial}' | head -n1`
5. Copy the serial that is displayed e.g. for my Colorduino acting as an I2C slave it is **AM01WJQJ**
6. Create a new udev rules file `sudo nano /etc/udev/rules.d/98-matrix-usb-serial.rules` and insert the following

    ```
    # colorduino
    SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001",  ATTRS{serial}=="AM01WJQJ", SYMLINK+="slave"
    # uview
    SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015",  ATTRS{serial}=="DA00T1QJ", SYMLINK+="master"
    ```

## Loading firmware

Using platformio we can develop in Atom and load the firmware with the aid of a FTDI basic breakout board

### Connecting FTDI Breakout to Colorduino

**n.b.** due to fault on FTDI board Txd is labelled and RxD and via versa, hence we wire TxD to TxD rather than the correct Txd -> RxD.

@todo replace table

| Cable Colour |         FTDI         |                         | Colorduino |
|:------------:|:--------------------:|:------------------------|:----------:|
|    green     |         DTR          | ─────────────────────── |    DTR     |
|    orange    |         TxD          | ────────────────        |    GND     |
|    yellow    |         RxD          | ─────────── └───────    |    TxD     |
|     red      |         Vcc          | ────── └────────────    |    RxD     |
|     CTS      | ─ └───────────────── | Vdd                     |            |
|    black     |         GND          | ─────────────────────── |    GND     |
|     SCL      |                      |                         |            |
|     SDA      |                      |                         |            |

### Setup Project

Initialise the project with:

```sh
platformio init -b diecimilaatmega328 --ide atom
```

### Compile Project

```sh
platformio run --environment diecimilaatmega328
```

### Upload project

```sh
platformio run --target upload --environment diecimilaatmega328
```

### Monitor Serial

**n.b.** Can't flash device while running

```sh
platformio device monitor -b 115200
```

## Colorduino API

Init()

GetPixel(char x, char y)

GetDrawPixel(char x, char y)

SetPixel(char x, char y, unsigned char r, unsigned char g, unsigned char b)

SetPixelchar x, char y, const PixelRGB & color()

FlipPage()

ColorFill()
