sa se sql angjatii care primesc prima  (10 % din salariu + comision) si se acorda angajatiilor care sunt din acelasi departament cu presedintele si nu au primit comision presedintele si managerii nu primesc prima

nume, functie, data angajare, venit, comision, prima

cautati cat mai multe solutii

DEFINE departament ="SELECT deptno FROM emp WHERE job like 'PRESIDENT'"

1) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE nvl(COMM, 0) = 0 AND DEPTNO = 10 AND JOB NOT IN ('PRESIDENT', 'MANAGER');

2) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE nvl(COMM, 0) = 0 AND DEPTNO = 10 AND (JOB NOT LIKE 'PRESIDENT' AND JOB NOT LIKE 'MANAGER');

3) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE nvl(COMM, 0) = 0 AND DEPTNO = 10 AND (JOB != 'PRESIDENT' AND JOB != 'MANAGER');

4) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE (COMM is NULL OR COMM = 0) AND DEPTNO = 10 AND (JOB != 'PRESIDENT' AND JOB != 'MANAGER');

5) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE (nvl(COMM, 0) BETWEEN 0 AND 0) AND(COMM is NULL or COMM = 0) AND DEPTNO = 10 AND (JOB != 'PRESIDENT' AND JOB != 'MANAGER');

6) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE (COMM is NULL OR COMM IN (0)) AND DEPTNO = 10 AND (JOB != 'PRESIDENT' AND JOB != 'MANAGER');

7) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE NOT nvl(COMM, 0) > 0 AND DEPTNO = 10 AND (JOB != 'PRESIDENT' AND JOB != 'MANAGER');

8) SELECT ename, job, hiredate, sal, sal * 1.1 AS prima FROM emp WHERE nvl(COMM, 0) = 0 AND DEPTNO = &depart AND JOB NOT IN ('PRESIDENT', 'MANAGER');


