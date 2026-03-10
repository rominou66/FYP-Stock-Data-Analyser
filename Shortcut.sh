#!/bin/bash
while true; do
    CHOICE=$(zenity --list --title="Shortcut" \
        --column="Option" \
        "Format" \
        "Clean" \
        "Build" \
        "Run" \
        --height=400 --width=200)
    if [ $? -ne 0 ]; then
        break
    fi

    clear
    cd ~/Documents/XJTLU/Y4S2-EIB/BUS303/FYP-Stock-Data
    case $CHOICE in
        "Format")
            astyle -A14 --recursive "src/*" &
            astyle -A14 --recursive "include/*" &
            rm src/*.orig
            rm include/*.orig
            ;;
        "Clean")
            sudo rm -r build
            mkdir -p build
            cd build
            cmake .. -G Ninja
            ;;
        "Build")
            #cmake --build build
            cd build
            ninja
            ;;
        "Run")
            cd build
            ./matplot_lib
            ;;
        *)
            zenity --error --text="Invalid option, please try again."
            ;;
    esac
done
