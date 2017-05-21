SRC=tema
TESTS=tests
OUT=out

if [ -f ${OUT} ]
then
    rm ${OUT}
fi
python ${SRC}/tester.py -f ${TESTS}/test1 -o ${OUT} -t 100
python ${SRC}/tester.py -f ${TESTS}/test2 -o ${OUT} -t 10
python ${SRC}/tester.py -f ${TESTS}/test3 -o ${OUT} -t 10
python ${SRC}/tester.py -f ${TESTS}/test4 -o ${OUT} -t 4

echo ""
echo "-----------------------------------------------------------------------"
echo ""

if [ -f ${OUT} ]
then
    cat ${OUT}
else
    echo "Tests failed"
fi
