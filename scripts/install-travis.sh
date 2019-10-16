#!/usr/bin/env bash

export PATH="$HOME/.local/bin:$PATH"

if hash arduino-builder; then
    echo "Arduino already installed"
else
    echo "Installing Arduino"

    cd /tmp
    wget https://downloads.arduino.cc/arduino-1.8.10-linux64.tar.xz
    mkdir -p ~/opt/
    tar -xf arduino-1.8.10-linux64.tar.xz -C ~/opt/

    mkdir -p $HOME/.local/share/icons/hicolor
    cd $HOME/opt/arduino-1.8.10
    ./install.sh

    mkdir -p ~/.local/bin
    ln -s ../../opt/arduino-1.8.10/arduino ~/.local/bin/
    ln -s ../../opt/arduino-1.8.10/arduino-builder ~/.local/bin/

    arduino --pref "boardsmanager.additional.urls=https://dl.espressif.com/dl/package_esp32_index.json" --save-prefs
    arduino --install-boards esp32:esp32

    cd /tmp
    wget https://www.pjrc.com/teensy/td_148/TeensyduinoInstall.linux64
    chmod +x TeensyduinoInstall.linux64
    ./TeensyduinoInstall.linux64 --dir="$HOME/opt/arduino-1.8.10"

    mkdir -p ~/Arduino/libraries && cd ~/Arduino/libraries
    git clone https://github.com/adafruit/Adafruit-GFX-Library.git &
    git clone https://github.com/tttapa/Adafruit_SSD1306.git &
    git clone https://github.com/PaulStoffregen/Encoder.git &
    git clone https://github.com/arduino-libraries/MIDIUSB.git

    wait

fi

if hash lcov; then
    echo "LCOV already installed"
else
    echo "Installing LCOV"
    cd /tmp
    git clone https://github.com/linux-test-project/lcov.git
    cd lcov
    make install PREFIX=$HOME/.local
fi