-- cerere sefii de departament care au cel putin 2 angajati in subordine care nu au primit comision
-- nume shef, functie sef, denumire departament subaltern,
-- nume subaltern, comision subaltern

SELECT b.ename MANAGER, b.job, c.DNAME DEPT_SUB, a.ename SUBALTERN, a.comm
	FROM EMP a, EMP b, DEPT c 
	WHERE b.empno = a.mgr
		AND a.deptno = c.deptno
		AND NVL(a.comm, 0) = 0
		AND (SELECT count(*) FROM emp a WHERE b.empno = a.mgr GROUP BY b.empno) >= 2;
		
SELECT count(*) FROM emp a, emp b WHERE b.empno = a.mgr GROUP BY b.empno;


