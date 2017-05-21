SELECT 
  ang.id AS id_ang, 
  ang.nume AS nume_ang, 
  ang.prenume AS prenume_ang, 
  ang.email, 
  ang.username, 
  ang.parola, 
  ang.telefon, 
  ang.adresa, 
  div.id AS id_divizie, 
  div.nume AS nume_divizie, 
  dept.id AS id_dept, 
  dept.nume AS nume_dept, 
  rol.id AS id_rol, 
  rol.descriere AS descriere_rol, 
  rol.nume AS nume_rol
FROM 
  public."Rol" rol, 
  public."Divizie" div, 
  public."Departament" dept, 
  public."Angajat" ang
WHERE 
  rol.id = ang.id_rol AND
  div.id = dept.id_divizie AND
  dept.id = ang.id_departament AND
  (rol.nume LIKE 'Angajat' OR rol.nume LIKE 'Administrator');
