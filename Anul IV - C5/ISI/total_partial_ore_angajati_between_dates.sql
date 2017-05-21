SELECT 
  A.id id_angajat, 
  A.nume || ' ' || A.prenume nume_angajat, 
  ACT.id_proiect,
  ACT.id_tip_activitate,
  TA.nume,
  SUM(ACT.ora_sfarsit::time - ACT.ora_start::time) part,
	(
       SELECT
	 SUM(ACTIV.ora_sfarsit::time - ACTIV.ora_start::time) tot
       FROM
	 "Activitate" ACTIV,
	 "Angajat" ANG,
	 "Timesheet" TIM,
	 "Fisa_zilnica" FIS
       WHERE
         ACTIV.id_proiect = ACT.id_proiect AND
         ANG.id = A.id AND
         ANG.id = TIM.id_angajat AND
         FIS.id_timesheet = TIM.id AND
	 FIS.id = ACTIV.id_fisa_zilnica AND
	 FIS.data >= '2014-01-01'::date AND
	 FIS.data <= '2014-01-12'::date AND
	 ANG.id = 8
       GROUP BY
	ACTIV.id_proiect,
	ANG.id
	)
FROM 
  public."Angajat" A, 
  public."Timesheet" T, 
  public."Fisa_zilnica" F, 
  public."Activitate" ACT,
  public."Tip_activitate" TA
WHERE 
  A.id = T.id_angajat AND
  F.id_timesheet = T.id AND
  F.id = ACT.id_fisa_zilnica AND
  TA.id = ACT.id_tip_activitate AND
  F.data >= '2014-01-01'::date AND
  F.data <= '2014-01-12'::date AND
  A.id = 8
 GROUP BY
  ACT.id_proiect,
  A.id,
  ACT.id_tip_activitate,
  TA.nume 
ORDER BY
  A.id,
  ACT.id_proiect;