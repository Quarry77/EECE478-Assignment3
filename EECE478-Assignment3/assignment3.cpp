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
float g_horizontalAngle = 0;
float g_verticalAngle = 100;
float g_camX = 0;
float g_camY = 200;
float g_camZ = 100;
vector<GLuint> g_skyboxTextures;
CityIO g_city;

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

	gluLookAt(g_camX, g_camY, g_camZ, g_camX + lookX, g_camY + lookY, g_camZ + lookZ, 0, 1, 0);

	for(int i = 0; i < g_city.buildings.size(); i++) {
		glPushMatrix();
		glTranslatef(g_city.buildings[i].tx, g_city.buildings[i].ty, g_city.buildings[i].tz);
		glScalef(g_city.buildings[i].sx, g_city.buildings[i].sy, g_city.buildings[i].sz);
		glRotatef(g_city.buildings[i].rz, 0, 0, 1);
		glRotatef(g_city.buildings[i].ry, 0, 1, 0);
		glRotatef(g_city.buildings[i].rx, 1, 0, 0);
		g_city.buildings[i].model.RenderModel();
		glPopMatrix();
	}

	// Skybox 5000 ups
	DrawSkybox();

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
	glEnable(GL_TEXTURE_2D);
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

		glutPostRedisplay();

	// Translate model while right clicking
	} else if(g_activeKey == RIGHT_MOUSE_CLICK) {
		g_camX += (g_activeX - x) * TRANSLATE_SCALE * cos(g_horizontalAngle * PI / 180.0);
		g_camZ += (g_activeX - x) * TRANSLATE_SCALE * sin(g_horizontalAngle * PI / 180.0);
		g_camY += (y - g_activeY) * TRANSLATE_SCALE;

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
		g_verticalAngle = 90;
		g_camX = 0;
		g_camY = 0;
		g_camZ = 0;

		glutPostRedisplay();

	// Exit
	} else if(key == 27) {
		exit(0);
	} else if(key == 'w') {
		g_camZ += 50 * sin(g_verticalAngle * PI / 180.0) * cos(g_horizontalAngle * PI / 180.0);
		g_camX += 50 * sin(g_verticalAngle * PI / 180.0) * sin(g_horizontalAngle * PI / 180.0);
		g_camY += 50 * cos(g_verticalAngle * PI / 180.0);
		glutPostRedisplay();
	} else if(key == 's') {
		g_camZ -= 50 * sin(g_verticalAngle * PI / 180.0) * cos(g_horizontalAngle * PI / 180.0);
		g_camX -= 50 * sin(g_verticalAngle * PI / 180.0) * sin(g_horizontalAngle * PI / 180.0);
		g_camY -= 50 * cos(g_verticalAngle * PI / 180.0);
		glutPostRedisplay();
	}
}

// Callback for special keyboard key presses (arrow keys)
void SpecialKeyboard(int key, int x, int y)
{
	// Zoom in with up arrow
	if(key == GLUT_KEY_UP) {

	// Zoom out with down arrow
	} else if(key == GLUT_KEY_DOWN) {
	}
}

// Draws the skybox for the city
void DrawSkybox() {
	float textureCoords[][2] = {
		{0, 1},
		{1, 1},
		{0, 0},
		{1, 0},
		{0, 0.5},
		{1, 0.5},
		{0, 40},
		{40, 40},
		{40, 0}
	};
	int textureCoordOrder[][4] = {
		{6, 7, 2, 8},
		{4, 5, 2, 3},
		{4, 5, 2, 3},
		{4, 5, 2, 3},
		{4, 5, 2, 3},
		{0, 1, 2, 3}
	};
	float vertices[][3] = {
		{-10000, 0, -10000},
		{-10000, 0, 10000},
		{10000, 0, 10000},
		{10000, 0, -10000},
		{-10000, 10000, -10000},
		{-10000, 10000, 10000},
		{10000, 10000, 10000},
		{10000, 10000, -10000}
	};
	int verticeOrder[][4] = {
		{0, 3, 1, 2},
		{3, 0, 7, 4},
		{0, 1, 4, 5},
		{1, 2, 5, 6},
		{2, 3, 6, 7},
		{5, 6, 4, 7}
	};

	for( int i = 0; i < g_skyboxTextures.size(); i++ ) {
		float texOffsetX = 0;
		float texOffsetZ = 0;
		if(i == 0) {
			texOffsetX += g_camX / 500.0;
			texOffsetZ -= g_camZ / 500.0;
		}

		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glBindTexture(GL_TEXTURE_2D, g_skyboxTextures.at(i));
		glBegin(GL_TRIANGLES);
		
		glTexCoord2f(textureCoords[textureCoordOrder[i][1]][0] + texOffsetX, textureCoords[textureCoordOrder[i][1]][1] + texOffsetZ);
		glVertex3f(vertices[verticeOrder[i][1]][0] + g_camX, vertices[verticeOrder[i][1]][1], vertices[verticeOrder[i][1]][2] + g_camZ);
		glTexCoord2f(textureCoords[textureCoordOrder[i][0]][0] + texOffsetX, textureCoords[textureCoordOrder[i][0]][1] + texOffsetZ);
		glVertex3f(vertices[verticeOrder[i][0]][0] + g_camX, vertices[verticeOrder[i][0]][1], vertices[verticeOrder[i][0]][2] + g_camZ);
		glTexCoord2f(textureCoords[textureCoordOrder[i][2]][0] + texOffsetX, textureCoords[textureCoordOrder[i][2]][1] + texOffsetZ);
		glVertex3f(vertices[verticeOrder[i][2]][0] + g_camX, vertices[verticeOrder[i][2]][1], vertices[verticeOrder[i][2]][2] + g_camZ);

		glEnd();

		glBegin(GL_TRIANGLES);
		
		glTexCoord2f(textureCoords[textureCoordOrder[i][3]][0] + texOffsetX, textureCoords[textureCoordOrder[i][3]][1] + texOffsetZ);
		glVertex3f(vertices[verticeOrder[i][3]][0] + g_camX, vertices[verticeOrder[i][3]][1], vertices[verticeOrder[i][3]][2] + g_camZ);
		glTexCoord2f(textureCoords[textureCoordOrder[i][1]][0] + texOffsetX, textureCoords[textureCoordOrder[i][1]][1] + texOffsetZ);
		glVertex3f(vertices[verticeOrder[i][1]][0] + g_camX, vertices[verticeOrder[i][1]][1], vertices[verticeOrder[i][1]][2] + g_camZ);
		glTexCoord2f(textureCoords[textureCoordOrder[i][2]][0] + texOffsetX, textureCoords[textureCoordOrder[i][2]][1] + texOffsetZ);
		glVertex3f(vertices[verticeOrder[i][2]][0] + g_camX, vertices[verticeOrder[i][2]][1], vertices[verticeOrder[i][2]][2] + g_camZ);

		glEnd();
	}
}

// Main application entry point
int main(int argc, char** argv)
{
	// Set model to render
	if(argc > 1) {
		cout << argv[1] << endl;
		g_city = CityIO(argv[1]);
	} else {
		cout << "./JohnCity.city" << endl;
		g_city = CityIO("./JohnCity.city");
	}

	// Initiate graphics
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1280,720);
    glutInitWindowPosition(0,0);
	glutCreateWindow(g_city.name.c_str());
    Init();

	// Set glut event callbacks
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutKeyboardFunc(Keyboard);
    glutDisplayFunc(Display);
	glutSpecialFunc(SpecialKeyboard);
    
	for(int i = 0; i < g_city.buildings.size(); i++) {
		g_city.buildings[i].model.LoadTextures();
	}
	g_city.LoadSkyBoxTextures(g_skyboxTextures);

    glutMainLoop();
    return 0;
}