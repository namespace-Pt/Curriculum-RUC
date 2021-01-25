compile_run_C(){
	for file in $(ls $1) 
	do
		path="$1$file"
		#echo $path
		if [ -f $path ];then
			exe=${file%.*}
			gcc -W -o $exe $path
			result=$?
			if [ $result -eq 0 ];then
				./$exe
				rm $exe
			else
				echo "$path is not a C program"
			fi
			
		else
			echo $path is a Directory
			compile_run_C "$path/"
		fi
	done
	return 0
}
if [ $# -eq 0 ]
then
	echo "This command requires more arguments"
	fileDiction=$(pwd)  
	echo "FIlE DIRECTION:$fileDiction"
fi
for DIR in $@ 
do
	echo $DIR
	if [ ! -d "$DIR" ];then
		echo "Sorry,directory $DIR does not exist"
		exit 1
	else
		compile_run_C $DIR 
	fi
done

dele(){
        cd $1
        for file in $(ls $1)
        do
        if [ -f $file ];then
                rm !(*.c|*.txt|*.json|*.shell)
        fi

        elif [ -d $file ];then
                dele "$1$file/"
        fi
        done
        return 0
}
