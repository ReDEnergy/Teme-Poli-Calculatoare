#!/bin/bash

function printUsage() {
	echo
	echo "Utilizare: $0 <nume_arhiva>"
	echo
}

function printArchiveNaming() {
	echo "Arhiva trebuie sa fie de forma:"
	echo "Nume_Prenume_Grupa.zip"
}

function cleanup() {
	if [ ! -z "$tmp_dir" ]; then
		echo "Se curata spatiul de lucru..."
		rm -rf tmp000xcf
	fi
}

arch_name=$1

if [ -z "$arch_name" ]; then
	echo "Eroare: Arhiva nespecificata"
	printUsage
	exit 1
fi

echo
echo "Verificare extensie arhiva..."

ext=${arch_name#*.}
if [ $ext != "zip" ]; then
	echo "EROARE: Arhiva nu are extensia corespunzatoare (.zip)"
	exit 1
fi

echo "Verificare tip arhiva..."

if [ $(file "$arch_name" | grep -c "Zip archive") != "1" ]; then
	echo "EROARE: Arhiva nu exista sau nu este de tipul specificat"
	exit 1
fi

echo "Verificare format pentru numele arhivei..."

arch_basename=$(basename $arch_name ".$ext")

IFS="_"
arr=( $arch_basename )
IFS="$OLD_IFS"

if [ ${#arr[@]} != 3 ]; then
	echo "EROARE: Formatul numelui arhivei nu este respectat"
	printArchiveNaming
	exit 1
fi

nume=${arr[0]}
prenume=${arr[1]}
grupa=${arr[2]}

echo "Informatii identificate:"
echo "Nume: $nume"
echo "Prenume: $prenume"
echo "Grupa: $grupa"
echo

echo "Se extrage continutul arhivei..."

tmp_dir="./tmp000xcf/$arch_basename"

if [ -d $tmp_dir ]; then
	rm -rf "$tmp_dir"
fi

mkdir -p $tmp_dir
if [ ! $? ]; then
	echo "EROARE: Arhiva nu poate fi extrasa"
	exit 1
fi

unzip -d $tmp_dir "$arch_name" >> /dev/null
if [ ! $? ]; then
	echo "EROARE: Arhiva nu poate fi extrasa"
	cleanup
	exit 1
fi

echo "Verificare fisier README..."
if [ ! -f "$tmp_dir/README" ]; then
	echo "EROARE: Fisierul README nu exista"
	cleanup
	exit 1
fi

if [ $(stat --printf="%s" "$tmp_dir/README") == 0 ]; then
	echo "EROARE: Fisierul README este gol"
	cleanup
	exit 1
fi

echo "Verificare fisier makefile..."
if [ ! \( -f "$tmp_dir/makefile" -o -f "$tmp_dir/Makefile" \) ]; then
	echo "EROARE: Fisierul makefile nu exista"
	cleanup
	exit 1
fi

echo "Verificare existenta fisiere binare..."
if [ $(find $tmp_dir -type f -exec file -b '{}'  \; | grep "ELF" | wc -l) -gt 0 ]; then
	echo "EROARE: Arhiva contine fisiere binare"
	cleanup
	exit 1
fi

cleanup

echo
echo
echo "Arhiva este OK :)"
echo
echo
