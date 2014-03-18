// John McDonnell
// 23880099

#include "assignment3.h"

// Constants for code clarity
const int NO_ACTIVE_CLICK = -1;
const int LEFT_MOUSE_CLICK = 0;
const int MIDDLE_MOUSE_CLICK = 1;
const int RIGHT_MOUSE_CLICK = 2;
const int KEY_DOWN = 0;
const int KEY_UP = 1;
const float PI = 3.14159265358979;
const float HORIZONTAL_MAX = 360;
const float HORIZONTAL_MIN = 0;
const float VERTICAL_MAX = 179.99;
const float VERTICAL_MIN = 0.01;
const float RADIUS_MAX = 50000.0;
const float RADIUS_MIN = 1.0;
const float TRANSLATE_SCALE = 0.005;


// Global variables for across events
int g_width;
int g_height;
int g_activeKey = -1;
int g_activeX;
int g_activeY;
float g_radius = 200.0;
float g_horizontalAngle = 0;
float g_verticalAngle = 90;
float g_focusX = 0;
float g_focusY = 80;
float g_focusZ = -200;
ModelIO g_model = ModelIO();

// Renders my name and student number in the lower right of the screen
void RenderText() 
{
	// Push the matrix to apply a 2d overlay
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
		

	// Text colour (white)
	glColor3f(1.0,1.0,1.0);

	// First line
	std::string str = "John McDonnell";
	glRasterPos2i(80,5);
	for (std::string::iterator i = str.begin(); i != str.end(); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *i);
	}

	// Second line
	str = "23880099";
	glRasterPos2i(80, 2);
	for (std::string::iterator i = str.begin(); i != str.end(); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *i);
	}

	// Restore the original matrices
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// Display callback for rendering
void Display()
{
    // Clear the display and define a colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Do model rotatations and translations
	
    float lookZ = sin(g_verticalAngle * PI / 180.0) * cos(g_horizontalAngle * PI / 180.0);
    float lookX = sin(g_verticalAngle * PI / 180.0) * sin(g_horizontalAngle * PI / 180.0);
    float lookY = cos(g_verticalAngle * PI / 180.0);

	gluLookAt(g_focusX, g_focusY, g_focusZ, g_focusX + lookX, g_focusY + lookY, g_focusZ + lookZ, 0, 1, 0);
	//glTranslatef(g_focusX, g_focusY, g_focusZ);
	//glRotatef(g_verticalAngle, 1, 0, 0);
	//glRotatef(g_horizontalAngle, 0, 1, 0);
	//glTranslatef(-g_focusX, -g_focusY, -g_focusZ);

	g_model.RenderModel();

    // Render my name and student number
	RenderText();

    glutSwapBuffers();
}

// Initialization of opengl camera matrix
void Init()
{
    // Clear screen to black and set up projection camera with depth test enabled
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

// Callback for when the application window is altered
void Reshape(int width, int height)
{
	// Reset the width and height of the application window
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);

	// Set up the perspective camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, float(width)/height, 0.1, 100000);

	// Reset to model view mode
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Callback for when a mouse key is pressed or released
void MouseClick(int button, int state, int x, int y)
{
	// Set either translate or rotate mode or neither
	if(g_activeKey == button && state == KEY_UP) {
		g_activeKey = -1;
	} else if(g_activeKey == NO_ACTIVE_CLICK && state == KEY_DOWN){
		g_activeKey = button;
		g_activeX = x;
		g_activeY = y;
	}
}

// Callback for when the mouse moves within the application window with a mouse button pressed
void MouseMove(int x, int y)
{
	// Rotate model while left clicking
	if(g_activeKey == LEFT_MOUSE_CLICK) {
		// Change the positioning angles for viewing the object
		g_horizontalAngle -= (x - g_activeX) * 0.6;
		while(g_horizontalAngle > HORIZONTAL_MAX) g_horizontalAngle -= HORIZONTAL_MAX;
		while(g_horizontalAngle < HORIZONTAL_MIN) g_horizontalAngle += HORIZONTAL_MAX;

		g_verticalAngle += (y - g_activeY) * 0.6;
		if(g_verticalAngle > VERTICAL_MAX) g_verticalAngle = VERTICAL_MAX;
		if(g_verticalAngle < VERTICAL_MIN) g_verticalAngle = VERTICAL_MIN;
		cout << "Horizontal: " << g_horizontalAngle << ", Vertical: " << g_verticalAngle << endl;

		glutPostRedisplay();

	// Translate model while right clicking
	} else if(g_activeKey == RIGHT_MOUSE_CLICK) {
		g_focusX += (g_activeX - x) * TRANSLATE_SCALE * g_radius * cos(g_horizontalAngle * PI / 180.0);
		g_focusZ += (g_activeX - x) * TRANSLATE_SCALE * g_radius * sin(g_horizontalAngle * PI / 180.0);
		g_focusY += (y - g_activeY) * TRANSLATE_SCALE * g_radius;

		glutPostRedisplay();
	}

	// Update the active coordinates
		g_activeX = x;
		g_activeY = y;
}

// Callback for keyboard key presses
void Keyboard(unsigned char key, int x, int y)
{
	// Reset
	if(key == 'r') {
		g_horizontalAngle = 0;
		g_verticalAngle = 0;
		g_radius = g_model.defaultRadius;
		g_focusX = 0;
		g_focusY = 0;
		g_focusZ = 0;

		glutPostRedisplay();

	// Exit
	} else if(key == 27) {
		exit(0);
	} else if(key == 'w') {
		g_focusZ += 5 * sin(g_verticalAngle * PI / 180.0) * cos(g_horizontalAngle * PI / 180.0);
		g_focusX += 5 * sin(g_verticalAngle * PI / 180.0) * sin(g_horizontalAngle * PI / 180.0);
		g_focusY += 5 * cos(g_verticalAngle * PI / 180.0);
		glutPostRedisplay();
	} else if(key == 's') {
		g_focusZ -= 5 * sin(g_verticalAngle * PI / 180.0) * cos(g_horizontalAngle * PI / 180.0);
		g_focusX -= 5 * sin(g_verticalAngle * PI / 180.0) * sin(g_horizontalAngle * PI / 180.0);
		g_focusY -= 5 * cos(g_verticalAngle * PI / 180.0);
		glutPostRedisplay();
	}
}

// Callback for special keyboard key presses (arrow keys)
void SpecialKeyboard(int key, int x, int y)
{
	// Zoom in with up arrow
	if(key == GLUT_KEY_UP) {
		g_radius *= 10.0/11.0;
		if(g_radius < RADIUS_MIN) g_radius = RADIUS_MIN;
		glutPostRedisplay();

	// Zoom out with down arrow
	} else if(key == GLUT_KEY_DOWN) {
		g_radius *= 1.1;
		if(g_radius > RADIUS_MAX) g_radius = RADIUS_MAX;
		glutPostRedisplay();
	}
}

// Main application entry point
int main(int argc, char** argv)
{
	// Set model to render
	if(argc > 1) {
		cout << argv[1] << endl;
		g_model = ModelIO(argv[1]);
	} else {
		cout << "./GasStation/GasStation.model" << endl;
		g_model = ModelIO("./GasStation/GasStation.model");
	}

	// Initiate graphics
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1280,720);
    glutInitWindowPosition(0,0);
	glutCreateWindow(g_model.name.c_str());
    Init();

	// Set glut event callbacks
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutKeyboardFunc(Keyboard);
    glutDisplayFunc(Display);
	glutSpecialFunc(SpecialKeyboard);

	// Set the default viewing distance based the furthest distance in the model data
	g_radius = g_model.defaultRadius;

	// Load the texture files into OpenGL
	g_model.LoadTextures();
    
    glutMainLoop();
    return 0;
}