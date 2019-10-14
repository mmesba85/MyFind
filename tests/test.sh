#! /bin/sh

while read line; do
    tput bold; tput setaf 4;
    echo "Test: $line"
    eval '$line' > /dev/null
    if [ $? = 0 ]; then
	    tput setaf 2; echo "OK"
    else
	    tput setaf 5; echo "NOK :("
    fi
done < tests/cases
