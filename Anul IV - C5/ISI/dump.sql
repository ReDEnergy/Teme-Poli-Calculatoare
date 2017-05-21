SELECT A.nume, A.prenume, A.email, A.username, R.nume rol, D.nume departament
FROM "Angajat" A, "Rol" R, "Departament" D
WHERE A.id_rol = R.id
AND A.id_departament = D.id;

SELECT A.nume, R.nume, R.descriere, R.id, D.nume 
FROM "Angajat" A, "Rol" R, "Departament" D
WHERE A.id_rol = R.id 
and   A.id_departament = D.id
and   R.nume = 'Sef Departament';

SELECT R.id, R.nume
FROM "Rol" R;

SELECT A.nume, A.prenume, A.email, A.username, R.nume rol, D.nume departament
FROM "Angajat" A, "Rol" R, "Departament" D
WHERE A.id_rol = R.id
AND A.id_departament = D.id;


SELECT A.nume, A.prenume, R.id
FROM  "Angajat" A, "Rol" R
WHERE A.id_rol = R.id
and   R.id = 5;

SELECT *
from "Departament" D;

SELECT A.nume, A.prenume, A.email, A.username, R.nume rol, D.nume departament
FROM "Angajat" A, "Rol" R, "Departament" D
WHERE A.id_rol = R.id
AND A.id_departament = D.id;



SELECT R.id, R.nume
FROM "Rol" R;


SELECT A.nume, A.prenume, A.email, A.username, R.nume rol, D.nume departament
FROM "Angajat" A, "Rol" R, "Departament" D
WHERE A.id_rol = R.id
AND A.id_departament = D.id;




SELECT R.id, R.nume
FROM "Rol" R;

select * from "Angajat";

select * from "Divizie";

select * from "Departament";




SELECT D.id_divizie smen
FROM   "Departament" D
WHERE  d.id = 2;



SELECT A.nume 
FROM   "Angajat" A
where   A.id_rol = 3 
and	A.id_departament in ( SELECT B.id
			     FROM   "Departament" B
			     WHERE  B.id_divizie = 2 );

	


SELECT A.id
FROM   "Angajat" A, "Timesheet" T
WHERE  A.id = T.id_angajat;



		   



SELECT * from "Timesheet";


SELECT id
FROM   "Angajat"
WHERE id not in (
		SELECT id_angajat
		FROM   "Timesheet"
		where  data_start between date_trunc('month', now())::date - 1
				    and now()::date );

--SELECT EXTRACT (DAY FROM Now());

--WHERE  (now() - A.data_start) >= 30;



select * from "Audit";


select * from "Tip_activitate";

select * from "Angajat";




