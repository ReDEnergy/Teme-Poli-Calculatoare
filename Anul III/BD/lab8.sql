SELECT ENAME, SAL, DEPTNO
	FROM EMP
	WHERE SAL IN (SELECT MIN(SAL)
		FROM EMP
		GROUP BY DEPTNO);
	
		
SELECT ENAME, SAL, DEPTNO
	FROM EMP
	WHERE (SAL, DEPTNO) IN
		(SELECT MIN(SAL), DEPTNO
		FROM EMP
		GROUP BY DEPTNO);
	
	
SELECT ENAME, SAL, JOB, DEPTNO
	FROM EMP
	WHERE SAL > SOME (SELECT DISTINCT SAL
					FROM EMP
					WHERE DEPTNO = 30)
	ORDER BY SAL DESC;


-- SA se scrie o cerere SQL folosind subcereri
-- Lista cu angadjatii care au venit in firma in anul in care sa
-- angalat cel mai recent manager si au salariu mai mare decat
-- salariu mediu pe departamentul din care fac parte
-- nume manager, data angajare manager, departament, nume angajat,
-- data angajare, salariu, salariu mediu pe departament

SELECT AVG(sal) SAL_MEDIU_DEPT, deptno from emp 
	GROUP BY deptno;

define data_manager = "(SELECT MAX(HIREDATE) FROM EMP WHERE JOB = 'MANAGER')";
define sal_med = "(SELECT AVG(sal) FROM emp WHERE deptno = a.deptno)"
define nume_manager = "(SELECT ename FROM emp WHERE hiredate = &data_manager)";

SELECT &data_manager HMAN, &nume_manager, a.ename ANGAJAT, a.deptno, c.dname DEPT, a.hiredate DATA, 
	a.sal, &sal_med MED_DEPT
	FROM emp a, dept c
	WHERE a.deptno = c.deptno
		AND TO_CHAR(a.hiredate, 'YYYY') =  TO_CHAR(&data_manager, 'YYYY')
		AND a.sal > (SELECT AVG(sal) FROM emp WHERE deptno = a.deptno);
		
