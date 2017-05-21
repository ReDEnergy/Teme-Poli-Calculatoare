EGC - Tema 1
Gabriel Ivanica 342C5

--------------------------------------------------------------------------------
1. Cerinta

	BomberMap - Dyna Blaster

	Implementarea unui joc gen Dyna Blaster

--------------------------------------------------------------------------------

2. Implementare

	Platforma: Windows 7 x64 SP1, Microsoft Visual Studio 2012
	Biblioteci:
		- freeglut + glew
		- assimp : import de meshes
		- ImageMagick : import de texturi
		- glm : librarie de matematica
		
	Pentru biblioteci am folosit fisiere de configurare specifice Visual Studio
	(Property Sheets)
	Pentru a putea rula trebuie instalat ImageMagik si configurat path-ul catre
	exe, lib, dll in Property Sheet

	
	Clasa Engine
		exporta toate functionalitatile OpenGL catre un listener clasa Game.
	
	Clasa Shader
		- instantiere shader program
		
	Clasa Object
		- Mesh + Textura + Shader
	
	
--------------------------------------------------------------------------------

3. Utilizare

	2.1 Input Tastatura

		Miscare FPS camera:
			- mouse 
			- sageti

		Trnaslatare camera:
			i - forward
			j - left
			k - backward
			l - right
			u - down
			o - up

		Gameplay:
			W - miscare in fata
			A - miscare in stanga
			S - miscare in jos
			D - miscare in dreapta
			Space -	plasare bomba

		Engine:
		  ESC -	iesire aplicatie
		  F2 - GL_POINT
		  F3 - GL_LINE
		  F4 - GL_FILL
		  F6 - Clip mouse cursor
		  F7 - Hide mouse cursor
		  F8 - Toggle Light debugging


--------------------------------------------------------------------------------

4. Functionalitate

	- conform specificatiilor din cerinta
		- harti generate random
		- miscare inamici random
		- coliziuni

	- Camera completa
	- Phong Lighting
	- Import cu assim de obiecte si texturi cu ImageMagick
	- VAO + VBO + IBO + UBO (pentru proprietatile materialelor)
	
	
	
	
	
	
	
	
	
	
	
	