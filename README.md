### PSEFM-C

PSEFM-C is a coordinate real time programming language which is based on c language.


### How to Install

#### Prepare

Simulation Environment: Ubuntu 12.04(above), 32 bit required
Simulator: qemu
RTOS: FreeRTOS
Architecture: STM32-P103

#### Reliable libraries

```
sudo apt-get install git zlib1g-dev libsdl1.2-dev automake*  autoconf* libtool libpixman-1-dev
```

#### Compiler

GNU Toolchain: Toolchain-2014_05.tar.bz2

```
mv Toolchain-2014_05.tar.bz2 /tmp
cd /
sudo tar jxvf /tmp/Toolchain-2014_05.tar.bz2
```

After Install, there will be a directory called arm-2014.05 under /usr/local/csl/
```
ls /usr/local/csl/
```

Then set the PATH:
```
export PATH=/usr/local/csl/arm-2014.05/bin:$PATH
```
(You can also write it to ~/.bashrc, then source ~/.bashrc)
```

#### Install Qemu
```
git clone git://github.com/beckus/qemu_stm32.git
cd qemu_stm32
git submodule update --init dtc || sudo apt-get install libfdt-dev
./configure --disable-werror --enable-debug \
    --target-list="arm-softmmu" \
    --extra-cflags=-DDEBUG_CLKTREE \
    --extra-cflags=-DDEBUG_STM32_RCC \
    --extra-cflags=-DDEBUG_STM32_UART \
    --extra-cflags=-DSTM32_UART_NO_BAUD_DELAY \
    --extra-cflags=-DSTM32_UART_ENABLE_OVERRUN \
    --disable-gtk
make -j4
```

#### install PSEFM
```
git clone git://github.com/wanborj/PSEFM-C.git
cd PSEFM-C
make qemu
```

The PSEFM example app in ./apps will run after that.


### How to use PSEFM-C

#### Programming API



