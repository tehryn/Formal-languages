if [ $# -eq 0 ]
then
    leak=""
else
    leak="--leak-check=full"
fi

GREEN='\033[1;32m'
RED='\033[1;31m'
NC='\033[0m' # No Color
valide="ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)"

for file in ./valid_input/*.java
do
    var=$(valgrind $leak ./IFJ16 $file 2>&1 | tail -1)

    if [[ $var == *"$valide"* ]]
    then
        echo -e "${file}: ${GREEN}$var${NC}"
    else
        echo -e "${file}: ${RED}$var${NC}"
    fi

done

for file in ./invalid_lexical/*.java
do
    var=$(valgrind $leak ./IFJ16 $file 2>&1 | tail -1)

    if [[ $var == *"$valide"* ]]
    then
        echo -e "${file}: ${GREEN}$var${NC}"
    else
        echo -e "${file}: ${RED}$var${NC}"
    fi

done

for file in ./invalid_syntax/*.java
do
    var=$(valgrind $leak ./IFJ16 $file 2>&1 | tail -1)

    if [[ $var == *"$valide"* ]]
    then
        echo -e "${file}: ${GREEN}$var${NC}"
    else
        echo -e "${file}: ${RED}$var${NC}"
    fi

done

for file in ./invalid_semantic/def/*.java
do
    var=$(valgrind $leak ./IFJ16 $file 2>&1 | tail -1)

    if [[ $var == *"$valide"* ]]
    then
        echo -e "${file}: ${GREEN}$var${NC}"
    else
        echo -e "${file}: ${RED}$var${NC}"
    fi

done

for file in ./invalid_semantic/compat/*.java
do
    var=$(valgrind $leak ./IFJ16 $file 2>&1 | tail -1)

    if [[ $var == *"$valide"* ]]
    then
        echo -e "${file}: ${GREEN}$var${NC}"
    else
        echo -e "${file}: ${RED}$var${NC}"
    fi

done

for file in ./invalid_runtime/not_init/*.java
do
    var=$(valgrind $leak ./IFJ16 $file 2>&1 | tail -1)

    if [[ $var == *"$valide"* ]]
    then
        echo -e "${file}: ${GREEN}$var${NC}"
    else
        echo -e "${file}: ${RED}$var${NC}"
    fi

done
