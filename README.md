# Matrix-16

A project using 4 Colorduino (8x8) boards to drive a 16x16 matrix. Using I2C with a Sparkfun Microview as the master.




## Loading firmware

Using platformio we can develop in Atom and load the firmware with the aid of a FTDI basic breakout board

### Connecting FTDI Breakout to Colorduino

**n.b.** due to fault on FTDI board Txd is labelled and RxD and via versa, hence we wire TxD to TxD rather than the correct Txd -> RxD. 

@todo replace table

| Cable Colour | FTDI |                         | Colorduino |
|:------------:|:----:|:------------------------|:----------:|
|    green     | DTR  | ─────────────────────── |    DTR     |
|    orange    | TxD  | ────────────────        |    GND     |
|    yellow    | RxD  | ───────────    └─────── |    TxD     |
|     red      | Vcc  | ──────    └──────────── |    RxD     |
|              | CTS  | ─    └───────────────── |    Vdd     |
|    black     | GND  | ─────────────────────── |    GND     |
|              |      |                         |    SCL     |
|              |      |                         |    SDA     |


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
