#!/bin/bash
if [ $# -eq 0 ]; then
    echo -e "Usage : ./script fichier.tex"
    echo -e "[!] Compilation aborted"
    exit 1
fi

echo -e "[-] Compiling ..."

if ! make; then
    echo -e "[!] Compilation failed"
    exit 1
fi

echo -e "\n[+] Compiling ended"


echo -e "[-] Generating the .asm file \n Table of symbols : \n"
./build/compiler > expr.asm < "$1"
echo -e ""

if [ ! -f asipro ] || [ ! -f sipro ]; then
    echo -e "[!] 'asipro' or 'sipro' not found in current directory."
    read -p "Clone and build them from GitHub? (Y/n): " choice

    if [[ "$choice" == "Y" || "$choice" == "y" || "$choice" == "" ]]; then
        echo -e "[-] Cloning asipro repo..."
        git clone https://github.com/NicolasBedon/asipro.git temp_asipro || { echo "[!] Git clone failed"; exit 1; }

        echo -e "[-] Building asipro and sipro..."
        make -C temp_asipro/emul || { echo "[!] Make of sipro failed"; exit 1; }
        make -C temp_asipro/asm  || { echo "[!] Make of asipro failed"; exit 1; }

        cp temp_asipro/emul/sipro ./ || { echo "[!] Copy of sipro failed"; exit 1; }
        cp temp_asipro/asm/asipro ./ || { echo "[!] Copy of asipro failed"; exit 1; }

        echo -e "[+] asipro and sipro are ready"
        rm -rf temp_asipro
    else
        echo -e "[!] Required tools not found. Aborting."
        exit 1
    fi
fi

# Une fois qu'on est sûr qu'on a les bons binaires :
echo -e "[-] Running asipro..."
./asipro expr.asm expr 2> err.asipro.log
echo -e "[+] Executable generated"

echo -e "\n[-] Executing with sipro...\n"
./sipro expr

