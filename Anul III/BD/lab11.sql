-- Sa Se creeze o tabela :
-- SEFI_DEPT cu urmatoarea structura

-- DEN_DEPT, NUME_SEF, NUMAR_SUBALTERNI

-- Tabela se va popula cu data in momentul crearii si contine
-- numele serfilor de departament care fac parte din deptartamentul cu cei mai multi angajati

define maxA = '(SELECT MAX(COUNT(*)) FROM EMP GROUP BY DEPTNO)';
define dept =  '(SELECT DEPTNO FROM EMP HAVING COUNT(*) = &maxA GROUP BY DEPTNO)';


CREATE TABLE SEFI_DEPT2 AS 
SELECT DISTINCT a.ENAME, c.DNAME, COUNT(*) NR_ANGAJATI
	FROM EMP a, EMP b, dept c 
	WHERE a.DEPTNO = &dept 
		AND a.empno = b.mgr
		AND a.deptno = c.deptno
		GROUP BY a.ENAME, c.DNAME;
 