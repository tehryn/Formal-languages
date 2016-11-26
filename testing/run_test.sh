declare -a valide_tests=("ok1.java" "ok2.java")
declare -a invalide_syntax=("err2.java" "err3.java" "err4.java" "err5.java" "err10.java")
declare -a invalide_lexical=("err6.java" "err7.java")
declare -a invalide_semantic_def=("err1.java" "err8.java" "err9.java" "err11.java")
declare -a invalide_semantic_compat=()
declare -a invalide_semantic_other=()

for file in ${valide_tests[@]}
do
	echo "Running test for $file:"
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 0 ]]; then
		echo "SUCCES - interpret returned with $ret"
	else
		echo "ERROR - interpret returned with $ret but should have returned with 0"
	fi
done

for file in ${invalide_lexical[@]}
do
	echo "Running test for $file:"
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 1 ]]; then
		echo "SUCCES - interpret returned with $ret"
	else
		echo "ERROR - interpret returned with $ret but should have returned with $ret"
	fi
done

for file in ${invalide_syntax[@]}
do
	echo "Running test for $file:"
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 2 ]]; then
		echo "SUCCES - interpret returned with $ret"
	else
		echo "ERROR - interpret returned with $ret but should have returned with 2"
	fi
done

for file in ${invalide_semantic_def[@]}
do
	echo "Running test for $file:"
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 3 ]]; then
		echo "SUCCES - interpret returned with $ret"
	else
		echo "ERROR - interpret returned with $ret but should have returned with 3"
	fi
done

for file in ${invalide_semantic_compat[@]}
do
	echo "Running test for $file:"
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 4 ]]; then
		echo "SUCCES - interpret returned with $ret"
	else
		echo "ERROR - interpret returned with $ret but should have returned with 4"
	fi
done

for file in ${invalide_semantic_other[@]}
do
	echo "Running test for $file:"
	./IFJ16 $file >/dev/null 2>&1
	ret=$?
	if [[ $ret == 6 ]]; then
		echo "SUCCES - interpret returned with $ret"
	else
		echo "ERROR - interpret returned with $ret but should have returned with 6"
	fi
done
