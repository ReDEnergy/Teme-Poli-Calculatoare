
SELECT concat(A.nume, ' ', A.prenume) nume, D.nume dept, R.id rol 
FROM "Angajat" A, "Departament" D, "Rol" R, "Divizie" 
WHERE A.id_departament = D.id 
AND A.id_rol = R.id ORDER BY A.nume;

SELECT * FROM "Angajat" A;

SELECT A.id, concat(A.nume, ' ', A.prenume) nume, Dv.nume divizie, D.nume dept, R.nume rol, R.id id_rol, Dv.id id_divizie, D.id id_departament
FROM "Angajat" A, "Departament" D, "Divizie" Dv, "Rol" R
WHERE A.id_departament = D.id 
AND A.id_rol = R.id
AND D.id_divizie = Dv.id;

SELECT A.id, concat(A.nume, ' ', A.prenume) nume, R.nume
FROM "Angajat" A, "Departament" D, "Rol" R
WHERE A.id_departament = D.id 
AND A.id_rol = R.id
AND R.id = 5
AND D.id = 1;

SELECT A.id, CONCAT(A.nume, ' ', A.prenume) nume, R.nume FROM "Angajat" A, "Departament" D, "Rol" R 
WHERE A.id_departament = D.id AND A.id_rol = R.id AND R.id = 5 AND D.id = 3