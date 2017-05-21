EGC - Tema 3
Gabriel Ivanica 342C5

--------------------------------------------------------------------------------
1. Cerinta

	Canny + Hough Transform 

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

3. Descriere implementare

Algoritmul Canny este implementat pe GPU utilizand mai multe Framebuffere si shadere de prelucrare
Am utilizat 2 FrameBuffere pentru a putea aplica mai multe filtre
	Am aplicat pe rand urmatoarele filtre
		- grayscale
		- guassian blur cu dimensiunea de 5 * 5
		- Sobel edge detection
			- in acelasi shader am calculat si directiile
			- utilizate 2 texturi pentru a pastra ambele informatii
		- NonMaximum Surpression + Histeresys Threshold
			- utilizand textura cu directiile din pasul Sobel am aplicat simplu 
				NonMaximum Surpression asupra tuturor pixelilor
			- pentru Histeresys Threshold am aplicat o versiune aproximativa pentru 
			a putea implementa eficient pe GPU

Dupa aplicarile filtrelor, am copiat imaginea pe CPU, pe care am aplicat transformatele Hough pentru
CERCURI respectiv LINII

Pentru codificarea LINIILE am folosit culoarea ROSU
Pentru codificarea CERCURILOR am utilizat culoarea ALBASTRU

Formele cerute de tema sunt codificate utilizand culoarea VERDE

Pentru a simplifica detectia, am rasterizat liniile cu o grosime de 3 PX.
Intersectiile dintre linii si cercuri stau la baza construirii formelor finale.
Am folosit un BFS pentru a colora toate liniile si cercurile ce se intersectau corespunzator
Asupra imaginii finale am aplicat masca Canny.


--------------------------------------------------------------------------------

4. Utilizare

	I. Input Tastatura

		F2
			- aplicare transformatei Hough asupra imaginii rezultate dupa Canny
		SHIFT + M
			- redimensionarea corespunazatoare a viewport-ului dupa dimensiunea imaginii
		H
			- vizualizarea mastii Canny / Canny + HoughTransform
	
		Miscare FPS camera:
			- mouse 
			- sageti

		Trnaslatare camera: (in modul Scena) - F4
			w - forward
			a - left
			s - backward
			d - right
			q - down
			e - up
			
		Trnaslatare lumina globala: (mod Scena)
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

5. Functionalitate

	- Cerintele temei
	- Canny pe shadere
	
	
	
	
	
	