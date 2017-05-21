EGC - Tema 3
Gabriel Ivanica 342C5

--------------------------------------------------------------------------------
1. Cerinta

	Shadow Mapping

--------------------------------------------------------------------------------

2. Implementare

	Platforma: Windows 7 x64 SP1, Microsoft Visual Studio 2012
	Biblioteci:
		- freeglut + glew
		- assimp : import de modele/obiecte
		- stb_image : import de texturi
		- glm : librarie de matematica
		
	Pentru biblioteci am folosit fisiere de configurare specifice Visual Studio
	(Property Sheets)
	
	Clasa Engine
		exporta toate functionalitatile OpenGL catre un listener clasa Game.
	
	Clasa Shader
		- instantiere shader program
		
	Clasa Object
		- Mesh + Textura + Shader
	
	Clasa FrameBuffer
		- se poate crea un nou framebuffer, pentru a putea face Render2Texture
		- se pot atasa un numar de texturi de culoare >=0 
		( pt 0 doar Depth Buffer)
	
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
			
		Trnaslatare lumina globala:
			8 - forward
			4 - left
			5 - backward
			6 - right
			7 - down
			9 - up			

		Engine:
		  ESC -	iesire aplicatie
		  F1 - toggle DepthBuffer debug panel
		  F3 - toggle WIREFRAME
		  F4 - Post-procesare - randare pe texturi
		  F6 - Clip mouse cursor
		  F7 - Hide mouse cursor
		  F8 - Toggle Light debugging


--------------------------------------------------------------------------------

4. Functionalitate

	- Camera completa
	- Phong Lighting (Directional/SpotLight)
	- VAO + VBO + IBO
	- FBO pentru DepthBuffer

	- Pentru vizualizarea DepthBuffer-ului F1
	- Shadow mapping
		+ functioneaza cu modele cu transparenta (vegetatie)
	- PCF
		kernel [4][4]
	
	
	
	
	
	
	
	
	
	
	