-- Sa se faca un raport dupa modelul celui de pe tabla cu sefii de departament 
-- care au cel putin 2 subalterni
-- Denumire departament, nume sef, nume subaltern, data angajare subaltern salariu subaltern


TTITLE 'LISTA ANGAJATI'
BTITLE OFF
COLUMN SALARIU FORMAT 999999.99

BREAK ON DEPARTAMENT ON REPORT
COMPUTE SUM LABEL 'TATAL SALARY' OF SALARIU ON DEPARTAMENT
COMPUTE SUM LABEL 'TATAL FIRMA' OF SALARIU ON REPORT

define date = '(select sysdate from dual)';
TTITLE CENTER 'LISTA ANGAJATI' LEFT 'pagina:'SQL.PNO RIGHT _date
BTITLE RIGHT 'Manager'


SELECT c.dname DEPARTAMENT,
	(SELECT ename FROM emp WHERE a.mgr = empno) sef,
	a.ename SUBALTERN, a.hiredate DATA, a.sal SALARIU
	FROM EMP a, DEPT c 
	WHERE (SELECT count(*) FROM emp b WHERE b.mgr = a.mgr) >= 2
		AND c.deptno = a.deptno
	ORDER BY c.dname;














