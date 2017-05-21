EGC - Tema 1
Gabriel Ivanica 342C5

--------------------------------------------------------------------------------
1. Cerinta

	Terrain Generation

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
	
	Clasa Terrain (de fapt Patch)
	
	Clasa FrameBuffer
		- se poate crea un nou framebuffer, pentru a putea face Render2Texture
	
--------------------------------------------------------------------------------

3. Utilizare

	2.1 Input Tastatura

		Miscare FPS camera:
			- mouse 
			- sageti

		Trnaslatare camera:
			w - forward
			a - left
			s - backward
			d - right
			q - down
			e - up

		Engine:
		  ESC -	iesire aplicatie
		  F3 - GL_FILL / GL_LINE / GL_POINT
		  F4 - Post-procesare - randare pe texturi
		  F6 - Clip mouse cursor
		  F7 - Hide mouse cursor
		  F8 - Toggle Light debugging


--------------------------------------------------------------------------------

4. Functionalitate

	- Camera completa
	- Phong Lighting (Directional + SpotLight)
	- VAO + VBO + IBO
	- FBO
	
	- Doar gridul initial este generat pe CPU
	- Displacement pe GPU (tesselation evaluation)
	- Ranadare NormalMap pe baza HeightMap intr-o textura pe GPU
	- Tesselare in functie de distata
	- Frustrum culling (pastreaza FPS-ul mic, dar e implementat varza)
	- DefferedLighting partial - nu am aplicat Phong in zonele de luminare
	
	
	
	
	
	
	
	
	
	
	
	