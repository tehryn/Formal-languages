declare -a valide_tests=("ok1.java" "ok2.java" "ok3.java")
declare -a invalide_syntax=("err2.java" "err3.java" "err4.java" "err5.java" "err10.java" "err26.java" "err28.java" "err29.java")
declare -a invalide_lexical=("err6.java" "err7.java")
declare -a invalide_semantic_def=("err1.java" "err8.java" "err9.java" "err11.java" "err13.java" "err14.java" "err15.java" "err18.java" "err20.java" "err27.java")
declare -a invalide_semantic_compat=("err12.java" "err16.java" "err17.java" "err19.java" "err21.java" "err22.java" "err23.java" "err24.java" "err25.java")
declare -a invalide_semantic_other=("err30.java")

for file in ${valide_tests[@]}
do
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 0 ]]; then
		echo "$file: SUCCES - interpret returned with $ret"
	else
		echo "$file: ERROR - interpret returned with $ret but should have returned with 0"
	fi
done

for file in ${invalide_lexical[@]}
do
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 1 ]]; then
		echo "$file: SUCCES - interpret returned with $ret"
	else
		echo "$file: ERROR - interpret returned with $ret but should have returned with $ret"
	fi
done

for file in ${invalide_syntax[@]}
do
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 2 ]]; then
		echo "$file: SUCCES - interpret returned with $ret"
	else
		echo "$file: ERROR - interpret returned with $ret but should have returned with 2"
	fi
done

for file in ${invalide_semantic_def[@]}
do
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 3 ]]; then
		echo "$file: SUCCES - interpret returned with $ret"
	else
		echo "$file: ERROR - interpret returned with $ret but should have returned with 3"
	fi
done

for file in ${invalide_semantic_compat[@]}
do
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 4 ]]; then
		echo "$file: SUCCES - interpret returned with $ret"
	else
		echo "$file: ERROR - interpret returned with $ret but should have returned with 4"
	fi
done

for file in ${invalide_semantic_other[@]}
do
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 6 ]]; then
		echo "$file: SUCCES - interpret returned with $ret"
	else
		echo "$file: ERROR - interpret returned with $ret but should have returned with 6"
	fi
done
