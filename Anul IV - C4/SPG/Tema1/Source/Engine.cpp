#include "Engine.h"

enum KEY {
	ESC	= 27
};

namespace Engine {

	// Functions
	void clipCursor(bool state);
	void reshape();
	void onKey();
	void logStart(const WindowInfo &Window, const ContextInfo &Context, 
				const FramebufferInfo &Framebuffer);

	// GLUT
	//WindowInfo Window(string("Engine"), 1280, 720, 200, 165, true);
	WindowInfo Window(string("Engine"), 840, 480, 800, 50, true);
	ContextInfo Context(4, 1, false);
	FramebufferInfo Framebuffer(true, true, true, true);

	// Window
	RECT window;
	RECT edge;
	bool clip_cursor = false;
	bool hide_cursor = false;
	bool full_screen = false;
	int clipping_edge = 5;


	// KeyBuffering arrays for holding key state
	bool *key_states = new bool[256];
	bool *special_key_states = new bool[256];

	// Mouse
	int mouseX = Window.width / 2;
	int mouseY = Window.height / 2;

	// Engine
	int elapsed_time = 0;
	int delta_time = 0;
	bool initialized = false;
	Game *game = NULL;

	// -------------------------------------------------------------------------
	// KEYBOARD CALLBACK FUNCTIONS
	// -------------------------------------------------------------------------

	void keyPressed (unsigned char key, int mouse_x, int mouse_y) {
		//if (key_states[key] == false)
		//	cout << "key:" << key << " pressed " << endl;

		key_states[key] = true;

		if (game)
			game->keyPressed(key, mouse_x, mouse_y);

		if(key == KEY::ESC)
			glutExit();
	}  

	void keyReleased (unsigned char key, int mouse_x, int mouse_y) {
		key_states[key] = false;
		//cout << "key:" << key << " released " << endl;

		if (game)
			game->keyReleased(key, mouse_x, mouse_y);

	}

	void specialKeyPressed (int key, int mouse_x, int mouse_y) {
		//if (key_states[key] == false)
		//	cout << "special key:" << key << " pressed " << endl;

		if (key == GLUT_KEY_F2)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		if (key == GLUT_KEY_F3)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (key == GLUT_KEY_F4)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (key == GLUT_KEY_F6 && special_key_states[key] == false) {
			clip_cursor = !clip_cursor;
			clipCursor(clip_cursor);
		}

		if (key == GLUT_KEY_F7 && special_key_states[key] == false) {
			hide_cursor = !hide_cursor;
			if (hide_cursor == TRUE) {
				cout << "CURSOR HIDDEN" << endl;
				glutSetCursor(GLUT_CURSOR_NONE);
			}
			else {
				cout << "CURSOR VISIBLE" << endl;
				glutSetCursor(0);
			}
		}

		if (key == GLUT_KEY_F11 && special_key_states[key] == false) {
			full_screen = !full_screen;
			full_screen ? glutFullScreen() : glutLeaveFullScreen();
			clipCursor(clip_cursor);
		}

		if (game && special_key_states[key] == false)
			game->specialKeyPressed(key, mouse_x, mouse_y);

		special_key_states[key] = true;

	}  

	void specialKeyRelease (int key, int mouse_x, int mouse_y) {
		special_key_states[key] = false;

		if (game)
			game->specialKeyReleased(key, mouse_x, mouse_y);
	}

	// -------------------------------------------------------------------------
	// MOUSE CALLBACK FUNCTIONS
	// -------------------------------------------------------------------------

	void mouseClick(int button, int state, int mouse_x, int mouse_y) {
		if (game)
			game->mouseClick(button, state, mouse_x, mouse_y);
	}

	void mouseMove(int mouse_x, int mouse_y) {

		int pos_x = mouse_x;
		int pos_y = mouse_y;
		int delta_x = mouse_x - mouseX;
		int delta_y = mouse_y - mouseY;

		if (mouse_x <= edge.left)	pos_x = Window.width / 2;
		if (mouse_y <= edge.top)	pos_y = Window.height / 2;
		if (mouse_x >= edge.right)	pos_x = Window.width / 2;
		if (mouse_y >= edge.bottom)	pos_y = Window.height / 2;

		if (pos_x != mouse_x || pos_y != mouse_y)
			glutWarpPointer(pos_x, pos_y);

		mouseX = pos_x;
		mouseY = pos_y;

		if (game)
			game->mouseMove(mouse_x, mouse_y, delta_x, delta_y);
	}

	void mouseScroll(int wheel, int direction, int mouse_x, int mouse_y) {
		if (game)
			game->mouseScroll(wheel, direction, mouse_x, mouse_y);
	}

	// -------------------------------------------------------------------------
	// WINDOW FUNCTIONS
	// -------------------------------------------------------------------------

	// Clip cursor inside window
	void clipCursor(bool state) {

		cout << "CLIP CURSOR: " << (state ? "TRUE" : "FALSE") << endl;
		clip_cursor = state;

		if (state == false) {
			ClipCursor(0);
			return;
		}

		window.left = glutGet(GLUT_WINDOW_X) + clipping_edge;
		window.top = glutGet(GLUT_WINDOW_Y) + clipping_edge;
		window.bottom = window.top + Window.height - 2 * clipping_edge;
		window.right = window.left + Window.width - 2 * clipping_edge;

		ClipCursor(&window);
	}

	// Reshape window
	void reshapeWindow(int width, int height) {

		Window.width = width;
		Window.height = height;

		edge.top = clipping_edge;
		edge.left = clipping_edge;
		edge.right = Window.width - 3 * clipping_edge;
		edge.bottom = Window.height - 3 * clipping_edge;

		clipCursor(clip_cursor);

		//set viewport
		glViewport(0, 0, width, height);
	}

	// -------------------------------------------------------------------------
	// ENGINE FUNCTIONS
	// -------------------------------------------------------------------------

	// Get Elapsed time from when engine start
	int getElapseTime() {
		 return glutGet(GLUT_ELAPSED_TIME);
	}

	int computeFrameDeltaTime() {
		static int old_time = 0;
		elapsed_time = getElapseTime();
		delta_time =  elapsed_time - old_time;
		old_time = elapsed_time;
		return delta_time;
	}

	void resetKeys() {
		for (int i=0; i<256; i++) {
			key_states[i] = false;
			special_key_states[i] = false;
		}
	}

	void close() {
		cout << "=====================================================" << endl;
		cout << "Engine closed. Exit" << endl;
		glutLeaveMainLoop();
	}

	void display() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float d_time = delta_time / 1000.0f;
		float t_start = elapsed_time / 1000.0f;

		// Render Frame
		if (game)
			game->displayFrame(t_start, d_time);

		// Swap Buffers
		glutSwapBuffers();
	}

	void glutIdle() {
		computeFrameDeltaTime();
		float time = delta_time / 1000.0f;

		if (game) {
			game->onKeyHold(key_states, time);
			game->onSpecialKeyHold(special_key_states, time);
		}

		glutPostRedisplay();
	}

	void run() {
		if (initialized == false)
			return;

		glutMainLoop();	
	}

	void setGameObject(Game *game_instance) {
		game = game_instance;
	}

	void init() {

		initialized = true;
	
		glutInitContextVersion(Context.major_version, Context.minor_version);
		glutInitContextFlags(GLUT_DEBUG);		

		if (Context.core)
			glutInitContextProfile(GLUT_CORE_PROFILE);
		else
			glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

		int fakeargc = 1;
		char *fakeargv[] = {"", NULL};
		glutInit(&fakeargc, fakeargv);

		glutInitDisplayMode(Framebuffer.flags);	
		glutInitWindowPosition(Window.start_position_x, Window.start_position_y);
		glutInitWindowSize(Window.width,Window.height);
		glutCreateWindow(Window.name.c_str());
			
		glutIdleFunc(glutIdle);
		glutCloseFunc(close);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
			
		glutDisplayFunc(display);
		glutReshapeFunc(reshapeWindow);
		glutKeyboardFunc(keyPressed);
		glutKeyboardUpFunc(keyReleased);
		glutSpecialFunc(specialKeyPressed);
		glutSpecialUpFunc(specialKeyRelease);
		glutMouseFunc(mouseClick);
		glutMotionFunc(mouseMove);
		glutPassiveMotionFunc(mouseMove);
		glutMouseWheelFunc(mouseScroll); 
		glutWarpPointer(mouseX, mouseY);	

		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		clipCursor(clip_cursor);
		resetKeys();

		glewInit();
		glewExperimental = true;
		wglSwapIntervalEXT(1);

		logStart(Window, Context, Framebuffer);
	}

	// -------------------------------------------------------------------------
	// DEBUGGING AND LOGGING FUNCTIONS
	// -------------------------------------------------------------------------

	void logStart(const WindowInfo &window,const ContextInfo &context, const FramebufferInfo &framebuffer) {
		// Get unit details
		const unsigned char* renderer = glGetString( GL_RENDERER );
		const unsigned char* vendor = glGetString( GL_VENDOR );
		const unsigned char* version = glGetString( GL_VERSION );

		cout << "*******************************************************************************" << endl;
		cout << "GLUT"<<endl;
		cout << "\t Vendor : " << vendor << endl;
		cout << "\t Renderer : " << renderer << endl;
		cout << "\t OpenGl : " << version << endl;

		if (framebuffer.flags & GLUT_MULTISAMPLE) 
			cout << "\t MSAA 4x"<<endl;
		cout << "*******************************************************************************" << endl;
	}
}