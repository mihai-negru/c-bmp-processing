#!/bin/bash

EXEC="bmp"

function init {
	total_score=0
	valgrind_err=0
	pad=$(printf '%.1s' "."{1..70})
	padlength=71

	make
	if [ $? -ne 0 ]; then
		echo "Makefile failed!"
		exit 1
	fi

	mkdir -p output
	mkdir -p output/Task1
	mkdir -p output/Task2
	mkdir -p output/Task3
	mkdir -p output/Task4
	mkdir -p output/Task5
}

function print_result {
	printf "%s" "$1"
	printf "%*.*s" 0 $((padlength - ${#1} - ${#2} )) "$pad"
	printf "%s\n" "$2"
}

function check_task {
	task_id=$1
	start_test_id=$2
	end_test_id=$3
	test_score=$4
	echo "..............................TASK $task_id..................................."

	for test_id in $(seq $start_test_id $end_test_id); do
		test_file="./input/Task${task_id}/input${test_id}.txt"
		ref_file="./ref/Task${task_id}/output${test_id}.bmp"
		output_file="./output/Task${task_id}/output${test_id}.bmp"
	
		./$EXEC < "$test_file"

		diff "$output_file" "$ref_file" &> /dev/null
		ret=$?

		if [ $ret == 0 ]; then
			let "total_score += $test_score"
			print_result "Test ${test_id}" "${test_score}/${test_score}p passed"
		else 
			print_result "Test ${test_id}" "0/${test_score}p failed"
		fi

		if [ $ret == 0 ]; then
			valgrind --tool=memcheck --leak-check=full --error-exitcode=1 "./$EXE < $input_file" &>/dev/null

			if [ $? == 1 ]; then
				print_result "Test ${test_id} valgrind" "failed"
				valgrind_err=1
			else 
				print_result "Test ${test_id} valgrind" "passed"
			fi
		else 
			print_result "Test ${test_id} valgrind" "failed"
				valgrind_err=1
		fi

		rm -f "$output_file"
	done

    echo " "
}

function check_readme {
	echo "..............................TASK README.............................."
    
	ls . | grep -i "readme" &>/dev/null
	if [ $? -eq 0 ]; then
		print_result "Test README" "$1/$1p passed"
		let "total_score += $1"
	else
		print_result "Test README" "0/$1p failed"
	fi
    echo " "
}

function check_task_bonus {
	if [ $valgrind_err == 0 ]; then
		let "total_score += $1"
	fi
}

init
check_task 1 0 0 10 # task1, test0 score per test = 10 - 10 pct
check_task 2 0 4 4 # task2, test0-4 score per test = 4 - 20 pct
check_task 3 0 4 6 # task3, test0-4, score per test = 6 - 30 pct
check_task 4 0 4 6 # task4, test0-4, score per test = 6 - 30 pct
check_task 5 0 4 8 # task5, test0-4, score per test = 8 - 40 pct
check_task_bonus 20 # 20 pct

print_result "Final score" "${total_score}/150"
make clean &> /dev/null
