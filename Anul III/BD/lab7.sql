define avgsal = '(SELECT AVG(SAL) FROM emp)';

SELECT a.deptno, b.dname, ROUND(MIN(a.sal), 0) SALMIN, 
		ROUND(MAX(a.sal), 0) SALMAX, 
		ROUND(AVG(a.sal), 0) SALAVG,
		ROUND(&avgsal, 0) AVGFIRMA
	FROM emp a, dept b 
	WHERE a.deptno = b.deptno 
	GROUP BY a.deptno, b.dname
	HAVING AVG(a.sal) > &avgsal;

define avgsal = '(SELECT AVG(SAL) FROM emp)';
SELECT a.deptno, b.dname, ROUND(MIN(a.sal), 0) SALMIN, 
		ROUND(MAX(a.sal), 0) SALMAX, 
		ROUND(AVG(a.sal), 0) SALAVG,
		ROUND(&avgsal, 0) AVGFIRMA
	FROM emp a, dept b 
	WHERE a.deptno = b.deptno 
	GROUP BY a.deptno, b.dname
	HAVING 


-- Problema
-- Sa se scrie o cerere SQL care face o lista cu salariul minim si salariul maxim
-- din departamentele care au salariul mediu pe departament mai mare decat salariul
-- mediu pe firma
-- Antet:
-- IdDep, DenDep, SalMinDep, SalMaxDep, SalMedDep, SalMedFirma