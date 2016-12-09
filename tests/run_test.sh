#!/bin/bash
if [ $# -eq 0 ]
then
	GREEN='\033[1;32m'
	RED='\033[1;31m'
	NC='\033[0m' # No Color
fi

for file in ./tests/invalid_lex/*.java
do
	./IFJ16 $file >tmp.txt 2>&1
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

for file in ./tests/invalid_syn/*.java
do
	./IFJ16 $file >tmp.txt 2>&1
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

for file in ./tests/invalid_sem/def/*.java
do
	./IFJ16 $file >tmp.txt 2>&1
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

for file in ./tests/invalid_sem/com/*.java
do
	./IFJ16 $file >tmp.txt 2>&1
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

for file in ./tests/invalid_run/ini/*.java
do
	./IFJ16 $file >tmp.txt 2>&1
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

for file in ./tests/valid_input/*.java
do
	./IFJ16 $file 2>tmp.txt
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

for file in ./tests/diff_check/*.java
do
	./IFJ16 $file >tmp.txt 2>&1
	ret=$?
	if [[ $ret == 0 ]]; then
		ok=$(cat  ${file/java/output})
		ret=$(cat tmp.txt)
		if [[ "$ret" == "$ok" ]]; then
			echo -e "${GREEN}$file: SUCCES - output is correct"
		else
			echo -e "${RED}$file: ERROR - output is not correct"
			echo -e "${NC}------------------------------------------------------------------------------------"
			echo "${ok}"
			echo "------------------------------------------------------------------------------------"
			echo "${ret}"
			echo "===================================================================================="
		fi
	else
		echo -e "${RED}$file: ERROR - interpret returned with $ret but should have returned with 0"
		echo -e "${NC}------------------------------------------------------------------------------------"
		cat tmp.txt
		echo "===================================================================================="
	fi
done


rm tmp.txt
