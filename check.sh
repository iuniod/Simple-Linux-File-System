#!/bin/bash

executable="./sfls"

total=0
CODING_STYLE_BONUS=0
MAX_BONUS=1

valgrind_test() {
    (time timeout 50 valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 -q "$executable" $1) &>/dev/null
    if [ "$?" != "0" ]; then
        echo -n "DETECTED MEMORY LEAKS - "
        return 1
    else
        return 0
    fi
}


test_batch() {
    batch_name="$1"
    printf '%*s' "${COLUMNS:-$(tput cols)}" '' | tr ' ' .
    echo "testing $batch_name"
    
    for test_in in tests/in/test_"$batch_name"_*.in; do
        test_ref=`basename -s .in "$test_in"`
        test_ref="tests/ref/${test_ref}.ref"

        "${executable}" < "$test_in" | diff -wB - "$test_ref"
        if [[ $? -ne 0 ]]; then
            echo "$test_in failed"
            return 1
        else
            valgrind_test "$test_in"
            if [[ $? -ne 0 ]]; then
                echo "$test_in valgrind failed"
                return 1
            fi
        fi

        echo "$test_in passed"
    done

    return 0
}


run_tests() {
    make
    if [[ $? -ne 0 ]]; then
        echo "executable doesn't build, exiting..."
        echo "0 points"
        exit 1
    fi

    batches=("touch_ls,18" "mkdir,9" "cd,9" "tree,9" "pwd,9" "rmdir,9" "rm,9" "rmrec,9" "cp,9" "mv,9")
    for batch in ${batches[@]}; do
        batch_name="${batch%,*}"
        batch_points="${batch#*,}"

        test_batch "$batch_name"
        if [[ $? -eq 0 ]]; then
            echo "$batch_name passed    +${batch_points}p"
            total=$(( $total + $batch_points ))
        fi
    done
}


function checkBonus {
    echo "" > checkstyle.txt

    echo -ne "Coding style Errors:\n"
    for entry in *.{c,h}; do
    echo $entry
        if [[ -f $entry ]]; then
            python2 cpplint.py --filter=-build/header_guard,-legal/copyright,-runtime/printf,-build/include,-runtime/threadsafe_fn "$entry" > checkstyle.txt
            YOUR_BONUS=`cat checkstyle.txt | grep "Total errors found: "| tr -dc '0-9'`
            echo $YOUR_BONUS
            ERRORS_NUMBER=$(( $ERRORS_NUMBER+$YOUR_BONUS ))
        fi
    done

    if [[ $ERRORS_NUMBER != "0" ]]; then
        printf '\nBonus: FAIL'
        echo -ne "\nYou have a total of $ERRORS_NUMBER coding style errors.\n"
    else
        printf '\nBonus: OK'

        CODING_STYLE_BONUS=1
        CODING_STYLE_BONUS=1
    fi
    rm -f checkstyle.txt
}

function printBonus {
    echo -ne "\nYou got a bonus of $CODING_STYLE_BONUS/$MAX_BONUS.\n"
    printf '%*s' "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
}

run_tests;
printf '%*s' "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
checkBonus;
printBonus;

echo "tests = ${total}/99";
echo "total = $(( $total + $CODING_STYLE_BONUS ))/100";

make clean &> /dev/null
