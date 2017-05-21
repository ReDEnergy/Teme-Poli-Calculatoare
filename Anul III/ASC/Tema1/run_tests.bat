@echo off

set SRC=tema
set TESTS=tests
set OUT=out

if exist %OUT% (
    del %OUT%
)

python %SRC%/tester.py -f %TESTS%/test1 -o %OUT% -t 100
python %SRC%/tester.py -f %TESTS%/test2 -o %OUT% -t 10
python %SRC%/tester.py -f %TESTS%/test3 -o %OUT% -t 10
python %SRC%/tester.py -f %TESTS%/test4 -o %OUT% -t 4

echo ""
echo "-----------------------------------------------------------------------"
echo ""

if exist %OUT% (
    type %OUT%
) else (
    echo "Tests failed"
)

pause
