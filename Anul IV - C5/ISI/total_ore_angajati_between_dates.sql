SELECT 
  A.id id_angajat, 
  A.nume || ' ' || A.prenume nume_angajat, 
  ACT.id_proiect,
  SUM(ACT.ora_sfarsit::time - ACT.ora_start::time) tot
FROM 
  public."Angajat" A, 
  public."Timesheet" T, 
  public."Fisa_zilnica" F, 
  public."Activitate" ACT
WHERE 
  A.id = T.id_angajat AND
  F.id_timesheet = T.id AND
  F.id = ACT.id_fisa_zilnica AND
  F.data >= '2014-01-01'::date AND
  F.data <= '2014-01-12'::date AND
  A.id = 8
 GROUP BY
  ACT.id_proiect,
  A.id
ORDER BY
  A.id,
  ACT.id_proiect;