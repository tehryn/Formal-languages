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

for entry in ./*.java
do
    var=$(valgrind $leak ./IFJ16 $entry 2>&1 | tail -1)

    if [[ $var == *"$valide"* ]]
    then
        echo -e "${entry}: ${GREEN}$var${NC}"
    else
        echo -e "${entry}: ${RED}$var${NC}"
    fi

done
