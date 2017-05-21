SELECT 
  "Angajat".nume, 
  "Angajat".id, 
  "Timesheet".status, 
  "Timesheet".data_start, 
  "Timesheet".data_sfarsit, 
  "Fisa_zilnica".data, 
  "Fisa_zilnica".id_timesheet, 
  "Fisa_zilnica".id, 
  "Activitate".id, 
  "Activitate".ora_start, 
  "Activitate".ora_sfarsit, 
  "Proiect".nume, 
  "Proiect".descriere, 
  "Client".nume, 
  "Client".adresa, 
  "Client".descriere
FROM 
  public."Angajat", 
  public."Activitate", 
  public."Proiect", 
  public."Client", 
  public."Fisa_zilnica", 
  public."Timesheet"
WHERE 
  "Angajat".id = "Timesheet".id_angajat AND
  "Proiect".id = "Activitate".id_proiect AND
  "Client".id = "Proiect".id_client AND
  "Fisa_zilnica".id = "Activitate".id_fisa_zilnica AND
  "Timesheet".id = "Fisa_zilnica".id_timesheet;
