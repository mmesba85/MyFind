#! /bin/sh

set -x
./myfind tests/qux
./myfind -H tests
./myfind -H tests/link
./myfind -L tests/link
./myfind -d tests/qux
./myfind -L tests/qux
./myfind tests/qux tests/tmp
./myfind no_existing_dir
echo $?
./myfind -vfkjvn
echo $?
./myfind tests/qux -name 'f*'
./myfind tests/qux -type d
./myfind tests/qux -type d -name 'foo*'
./myfind tests -type d -o -name 'f*'
./myfind tests/tmp -print -a -print
./myfind tests/tmp -print -o -print
./myfind tests/tmp -type d -o -print
./myfind tests/tmp -exec pwd fff \;
./myfind tests/tmp -type d -exec echo ok \;
./myfind tests/tmp -exec ekfgrelk \;
./myfind tests/tmp -exec pwd \;
