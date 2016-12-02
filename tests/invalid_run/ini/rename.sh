i=1
for file in ./*.java
do
	if [ $i -le 9 ]
	then
		new_name="com0$i.java"
	else
		new_name="com$i.java"
	fi
	
	i=$(($i+1))
	mv $file $new_name
done
