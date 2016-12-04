#!/bin/bash
if [ $# -eq 0 ]
then
	GREEN='\033[1;32m'
	RED='\033[1;31m'
	NC='\033[0m' # No Color
fi

for file in ./valid_input/*.java
do
	echo -e "\n"; ./IFJ16 $file >/dev/null
	ret=$?
	if [[ $ret == 0 ]]; then
		echo -e "${GREEN}$file: SUCCES - interpret returned with $ret"
	else
		echo -e "${RED}$file: ERROR - interpret returned with $ret but should have returned with 0"
		echo -e "${NC}------------------------------------------------------------------------------------"
		cat tmp.txt
		echo "===================================================================================="
	fi
done

for file in ./invalid_lex/*.java
do
	echo -e "\n"; ./IFJ16 $file >/dev/null
	ret=$?
	if [[ $ret == 1 ]]; then
		echo -e "${GREEN}$file: SUCCES - interpret returned with $ret"
	else
		echo -e "${RED}$file: ERROR - interpret returned with $ret but should have returned with 1"
		echo -e "${NC}------------------------------------------------------------------------------------"
		cat tmp.txt
		echo "===================================================================================="
	fi
done

for file in ./invalid_syn/*.java
do
	echo -e "\n"; ./IFJ16 $file >/dev/null
	ret=$?
	if [[ $ret == 2 ]]; then
		echo -e "${GREEN}$file: SUCCES - interpret returned with $ret"
	else
		echo -e "${RED}$file: ERROR - interpret returned with $ret but should have returned with 2"
		echo -e "${NC}------------------------------------------------------------------------------------"
		cat tmp.txt
		echo "===================================================================================="
	fi
done

for file in ./invalid_sem/def/*.java
do
	echo -e "\n"; ./IFJ16 $file >/dev/null
	ret=$?
	if [[ $ret == 3 ]]; then
		echo -e "${GREEN}$file: SUCCES - interpret returned with $ret"
	else
		echo -e "${RED}$file: ERROR - interpret returned with $ret but should have returned with 3"
		echo -e "${NC}------------------------------------------------------------------------------------"
		cat tmp.txt
		echo "===================================================================================="
	fi
done

for file in ./invalid_sem/com/*.java
do
	echo -e "\n"; ./IFJ16 $file >/dev/null
	ret=$?
	if [[ $ret == 4 ]]; then
		echo -e "${GREEN}$file: SUCCES - interpret returned with $ret"
	else
		echo -e "${RED}$file: ERROR - interpret returned with $ret but should have returned with 4"
		echo -e "${NC}------------------------------------------------------------------------------------"
		cat tmp.txt
		echo "===================================================================================="
	fi
done

for file in ./invalid_run/ini/*.java
do
	echo -e "\n"; ./IFJ16 $file >/dev/null
	ret=$?
	if [[ $ret == 8 ]]; then
		echo -e "${GREEN}$file: SUCCES - interpret returned with $ret"
	else
		echo -e "${RED}$file: ERROR - interpret returned with $ret but should have returned with 8"
		echo -e "${NC}------------------------------------------------------------------------------------"
		cat tmp.txt
		echo "===================================================================================="
	fi
done




rm tmp.txt
