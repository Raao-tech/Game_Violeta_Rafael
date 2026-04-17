#!/bin/bash
choice=3
name_file="*_test.c"

echo -e "Hello! Please, tell me what do yo need to make?\n"
while [ $choice -q 3 ]; do
    echo -e "0 . Without Valgrind\n"
    echo -e "1 . With    Valgrind\n"
    echo -e "Format of the input:   <your choice> <name_test_file_without_extension>\n"
    read -p "prompt >  " choice name_file _; #lo primero se guarda en choice, lo segundo e name_file como str, y lo que le sigue se ignora

    if [ [$choice -q 0] || [$choice -q 1] ]; then
        
    fi
done