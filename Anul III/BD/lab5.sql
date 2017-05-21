SELECT * 
	FROM emp a, dept b 
	WHERE a.deptno = b.deptno;

SELECT a.ename, a.sal, b.grade 
	FROM emp a, salgrade b
	WHERE a.deptno = 20;
	
SELECT a.ename, a.sal, b.grade, c.deptno
	FROM emp a, salgrade b, dept c
	WHERE a.sal BETWEEN b.losal AND b.hisal
		AND a.deptno = c.deptno
		AND a.deptno = 20;
	
SELECT a.ename NAMEA, a.job JOBA,
		b.ename NAMEB, B.job JOBB
	FROM emp a, emp b
	WHERE a.mgr = b.empno
		AND a.deptno = 10;
		
SELECT DISTINCT a.ename, a.job, a.deptno
	FROM emp a, dept b
	WHERE a.job = 'MANAGER';

---	OUTER JOIN

SELECT a.deptno, a.dname, b.ename, b.job
	FROM dept a, emp b
	WHERE a.deptno = b.deptno(+);	
	
SELECT a.deptno, a.dname, b.ename, b.job
	FROM dept a, emp b
	WHERE b.deptno(+) LIKE substr(a.deptno, 1, 1) || '%';
	
---	VERTICAL JOIN

SELECT deptno, ename, job, sal
	FROM emp WHERE deptno = 10
UNION
SELECT deptno, ename, job, sal
	FROM emp WHERE deptno = 30;


SELECT deptno, ename, job, sal SAL_COM, 'are salariu' info
	FROM emp WHERE deptno = 10
UNION
SELECT deptno, ename, job, comm SAL_COM, 'are comision' info
	FROM emp WHERE deptno = 30;

SELECT job FROM emp WHERE deptno = 10
UNION ALL	
SELECT job FROM emp WHERE deptno = 30;


SELECT job, comm FROM emp WHERE deptno = 10
INTERSECT
SELECT job, comm FROM emp WHERE deptno = 20
INTERSECT
SELECT job, comm FROM emp WHERE deptno = 30;

SELECT job, comm FROM emp WHERE deptno = 10
MINUS
SELECT job, comm FROM emp WHERE deptno = 30;



--- Sa se scrie o cerere SQL care face o lista cu toti angajatii care 
--- au salariu mai mare decat jumatate din salariu sefului direct 
--- si au venit in firma in urma lui

--- DENUMIRE DEPARTAMENT SUBALTERN
--- NUME SUBALTERN
--- DATA ANGJARARE SUBALTERN
--- SALARIU SUBALTERN
--- NUME SEF
--- DATA ANGAJARE SEF
--- SALARIU SEF

--- 1
SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.hiredate > b.hiredate
		AND a.sal > b.sal / 2;

--- 2		
SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.sal > b.sal / 2
INTERSECT
SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.hiredate > b.hiredate

--- 3		
SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.hiredate > b.hiredate
MINUS
SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.hiredate > b.hiredate
		AND a.sal <= b.sal / 2;
		
--- 4

SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.hiredate > b.hiredate
		AND a.sal BETWEEN b.sal/2 AND b.sal
UNION
SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.hiredate > b.hiredate
		AND a.sal > b.sal;
		
--- 5
define B = "SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM emp a, emp b, dept c
	WHERE a.deptno = c.deptno
		AND a.mgr = b.empno
		AND a.hiredate > b.hiredate"

		
SELECT a.ename SUBALTERN, c.dname, a.hiredate, a.sal, b.ename SEF, b.hiredate, b.sal
	FROM &B
	WHERE AND a.sal >= b.sal / 2;














	