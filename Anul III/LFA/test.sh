#!/bin/bash
# credits to AI :D
EXE_NAME=./mtx
TEST_DIR=test
TEST_LIST=$TEST_DIR/tests

# Compile student homework
# comment this if you use Java or Haskell - this means that the executable should be generated before running this script
make

# Run tests
score=0
total_score=0
while read test_case
do
	# Parse the components of the line
	weight="$(echo $test_case | cut -d' ' -f 1)"
	file_name="$(echo $test_case | cut -d' ' -f 2)"
	turing_machine="$(echo $test_case | cut -d' ' -f 3)"
	tape_input="$(echo $test_case | cut -d' ' -f 4)"
	tape_output="$(echo $test_case | cut -d' ' -f 5)"

	# Run the student homework
	tape_student_output="$($EXE_NAME $TEST_DIR/$file_name $turing_machine $tape_input)"

	# Update scores
	total_score=$[$total_score + $weight]
	if [ "$tape_output" = "$tape_student_output" ]
		then echo "Test $file_name/$turing_machine $tape_input PASSED"; score=$[$score + $weight];
		else echo "Test $turing_machine $tape_input FAILED"; echo "failed output = "$tape_student_output; echo "official output = "$tape_output
	fi
done < $TEST_LIST

grade=$[$score * 100 / $total_score]
echo "Score: " $score " out of 100"
