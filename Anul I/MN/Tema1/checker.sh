#!/bin/bash

function printUsage() {
	echo
	echo "Utilizare: $0 <nume_arhiva>"
	echo "unde:"
	echo -e "\t<nume_arhiva>\tNumele arhivei care contine rezolvarea temei de casa"
	echo
}

function printReadmeStructure() {
	echo "Fisierul README trebuie sa aiba primele 3 linii de forma:"
	echo "Nume: <nume>"
	echo "Prenume: <prenume>"
	echo "Grupa: <grupa>"
	echo
	echo "Pentru un exemplu concret de fisier README corect, vezi proiectul"
	echo "HelloPC, pe care-l gasesti atat in masina virtuala, cat si pe"
	echo "site-ul cursului." 
}

function printArchiveNaming() {
	echo "Arhiva trebuie sa fie de forma:"
	echo "Grupa_Nume_TemaX.zip"
}

function cleanup() {
	if [ ! -z "$TMP_DIR" ]; then
		rm -rf "$TMP_DIR"
	fi
}

# The archive name
ARCH_NAME=$1
LOG_FILE="check.txt"

# Checking parameters...

if [ -z "$ARCH_NAME" ]; then
	echo "EROARE: Arhiva nespecificata"
	printUsage
	exit 1
fi

echo
echo "Verificare extensie arhiva..."

ARCH_NAME_MATCH=$(expr match "$ARCH_NAME" "\(.*\zip\)")
if [ -z "$ARCH_NAME_MATCH" ]; then
	echo "EROARE: Arhiva nu are extensia corespunzatoare (.zip)"
	exit 1
elif [ "$ARCH_NAME_MATCH" != "$ARCH_NAME" ]; then
	echo "EROARE: Arhiva nu are extensia corespunzatoare (.zip)"
	exit 1
fi

echo
echo "Verificare tip arhiva..."

if [ $(file "$ARCH_NAME" | grep -c "Zip archive") != "1" ]; then
	echo "EROARE: Arhiva nu exista sau nu este de tipul specificat"
	exit 1
fi

echo
echo "Verificare format pentru numele arhivei..."

NAME_SURNAME_GROUP=${ARCH_NAME%%\.zip}
GROUP=$(echo $NAME_SURNAME_GROUP | cut -d "_" -f 1)
NAME=$(echo $NAME_SURNAME_GROUP | cut -d "_" -f 2)
HOMEWORK=$(echo $NAME_SURNAME_GROUP | cut -d "_" -f 3 | cut -d "a" -f 2)
echo "Informatii identificate:"
echo "Nume: $NAME"
echo "Grupa: $GROUP"
echo "Tema: $HOMEWORK"

if [ -z "$GROUP" -o -z "$NAME" -o -z "$HOMEWORK" ]; then
	echo "EROARE: Formatul numelui arhivei nu este respectat"
	printArchiveNaming
	exit 1
fi

echo
echo "Se extrage continutul arhivei..."

TMP_DIR="/tmp/$NAME_SURNAME_GROUP"
if [ -d $TMP_DIR ]; then
	rm -rf "$TMP_DIR"
fi

mkdir -p $TMP_DIR
if [ ! $? ]; then
	echo "EROARE: Arhiva nu poate fi extrasa"
	exit 1
fi

unzip -d $TMP_DIR "$ARCH_NAME" >> /dev/null
if [ ! $? ]; then
	echo "EROARE: Arhiva nu poate fi extrasa"
	cleanup
	exit 1
fi

echo
echo "Verificare fisier README..."
if [ ! -f "$TMP_DIR/README" ]; then
	echo "EROARE: Fisierul README nu exista"
	cleanup
	exit 1	
fi

if [ 0 -eq $(stat --printf="%s" "$TMP_DIR/README") ]; then
	echo "EROARE: Fisierul README este gol"
	cleanup
	exit 1
fi

echo

echo
echo "Verificare existenta fisiere binare..."
if [ $(find $TMP_DIR -type f -exec file -b '{}'  \; | grep "ELF" | wc -l) -gt 0 ]; then
	echo "EROARE: Arhiva contine fisiere binare"
	cleanup
	exit 1
fi

cleanup

echo
echo
echo "The archive seems to be OK :)"
echo
echo


