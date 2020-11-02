#include "tga.cpp"
#include "glut.h"
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <Windows.h>//For Sound
#include <mmsystem.h>//For Sound


int g_Width=800;		//define size of the window
int g_Height=750;

//global mouse position
float 
birdX = 50, birdY = 10, birdVX = 3, birdVY = 0,
manX = 400, manY = 445, manVX = 0,
mouse_X, mouse_Y,
shitV[4], shitX[4], shitY[4], 
bulletV[4], bulletVx[4], bulletVy[4], bulletX[4], bulletY[4],
sprite = 0;
int ManScore = 0, BirdScore = 0;
bool
keyStates[256],
manDir = true,
game_pause = false;

GLuint birdId[8], manId, backId, bulletId, shitId;//The id of the textures


void init();
void myMouseFunction( int button, int state, int mouseX, int mouseY );
void myKeyboardFunction( unsigned char key, int mouseX, int mouseY );
void mySpecialKeysFunction(int key, int x, int y);
void myReshapeFunction( int width, int height );
void myTimerFunction( int val );
void myDisplayFunction();
void KeyUp(unsigned char , int , int );
void SpecialKeyUp(int , int , int );


void BackDisplay();
void manDisplay();
void birdDisplay();
void bulletDisplay();
void shitDisplay();
void TextDisplay();
void initShit();
void initBullet();
void collisioncheck();
void EndGame();
void Reset();


GLuint loadtexture(char *name)
{
	GLuint temp ;
	glGenTextures(1,&temp);
	loadTGA(name,temp);
	return temp;
}

void renderBitmapString(float x, float y, char *string) {

	char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
}

void renderBitmapString2(float x, float y, char *string) {

	char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

int main(int argc, char** argv)
{
	glutInit( &argc, argv );		//initilize the GLUT library

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );		//has four colour components red, green,blue, and alpha 
	glutInitWindowSize( 800, 700 );	//size of window
	glutInitWindowPosition( 0, 0 );		//poistion of window
	glutCreateWindow( "SHIT the man!" );

	init();
	birdId[0] = loadtexture("0.tga");
	birdId[1] = loadtexture("1.tga");
	birdId[2] = loadtexture("2.tga");
	birdId[3] = loadtexture("3.tga");
	birdId[4] = loadtexture("4.tga");
	birdId[5] = loadtexture("5.tga");
	birdId[6] = loadtexture("6.tga");
	birdId[7] = loadtexture("7.tga");
	manId = loadtexture("MAN.tga");
	backId = loadtexture("background.tga");
	bulletId = loadtexture("BULLET.tga");
	shitId = loadtexture("SHIT.tga");

	glutMouseFunc( myMouseFunction );
	glutKeyboardFunc( myKeyboardFunction );
	glutKeyboardUpFunc(KeyUp);
	glutSpecialFunc( mySpecialKeysFunction );
	glutSpecialUpFunc(SpecialKeyUp);


	glutReshapeFunc( myReshapeFunction );
	glutDisplayFunc( myDisplayFunction ); 
	glutTimerFunc( 33, myTimerFunction, 0 ); 	

	glutSetCursor( GLUT_CURSOR_INFO );	//Displays the type of cursor u want
	
	glutMainLoop();

	return 0;
}

void BackDisplay() // SHOW THE BACKGROUND IMAGE
{
	bool check = true;
	if (check)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, backId);

		//Bottom
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glScalef(1, 1, 1);
		glColor4ub(255, 255, 255, 255);

		glBegin(GL_POLYGON);

		glTexCoord2f(0.0, 1.0);
		glVertex2f(0, 0);//draw square for menu
		glTexCoord2f(1.0, 1.0);
		glVertex2f(800, 0);//2nd
		glTexCoord2f(1.0, 0.0);
		glVertex2f(800, 700);//3rd 
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0, 700);//4th

		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		check = false;
	}
}

void collisioncheck()
{
	for (int i = 0; i < 4; i++) //CHECKING COLLISION FOR EVERY BULLET
	{
		if (bulletV[i] != 0) //IF VELOCITY IS ZERO, BULLET DOESNT EXIST
		{
			if ((bulletX[i]>birdX) && (bulletX[i] < birdX + 54))
			if ((bulletY[i]>birdY + 10) && (bulletY[i] < birdY + 64))
			{
				bulletV[i] = 0;	
				ManScore++;
				PlaySound(TEXT("HIT.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
	}

	for (int i = 0; i < 4; i++)	// CHECKING FOR EVERY SHIT
	{
		if (shitV[i] != 0) // IF VELOCITY ZERO, IT DOESNT EXIST
		{
			if ((shitX[i]>manX) && (shitX[i] < manX + 115))
			if ((shitY[i]>manY) && (shitY[i] < manY + 160))
			{
				shitV[i] = 0;
				BirdScore++;
				PlaySound(TEXT("HIT.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
	}
}

void birdDisplay()
{
								//EQUATIONS FOR MOVEMENT
	birdX = birdX + birdVX;
	birdY = birdY + birdVY;
	if (birdY<0)				//CONDITIONS TO STOP FROM GOING OUT OF SCREEN
	{
		birdVY = 0;
		birdY = 0;
	}
	else if (birdY>300)
	{
		birdVY = 0;
		birdY = 300;
	}
	if (birdX>740 || birdX < 0)
		birdVX = -birdVX;
	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, birdId[(int)sprite%8]);
	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glScalef(1, 1, 1);
	glColor4ub(255, 255, 255, 255);

	glBegin(GL_POLYGON);
	if (birdVX > 0)										//INVERTING THE IMAGE ACCORDING TO DIRECTION
	{
		glTexCoord2f(1.0, 1.0);
		glVertex2f(birdX, birdY);//draw square for menu
		glTexCoord2f(0.0, 1.0);
		glVertex2f(birdX + 64.0, birdY + 0.0);//2nd
		glTexCoord2f(0.0, 0.0);
		glVertex2f(birdX + 64.0, birdY + 70.0);//3rd 
		glTexCoord2f(1.0, 0.0);
		glVertex2f(birdX + 0.0, birdY + 70.0);//4th
	}
	else
	{
		glTexCoord2f(0.0, 1.0);
		glVertex2f(birdX, birdY);//draw square for menu
		glTexCoord2f(1.0, 1.0);
		glVertex2f(birdX + 64.0, birdY + 0.0);//2nd
		glTexCoord2f(1.0, 0.0);
		glVertex2f(birdX + 64.0, birdY + 70.0);//3rd 
		glTexCoord2f(0.0, 0.0);
		glVertex2f(birdX + 0.0, birdY + 70.0);//4th
	}

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void manDisplay()
{
	
	manX = manX + manVX;
	if (manX<0)				//CONDITIONS TO STOP FROM GOING OUT OF BOUNDS
	{
		manVX = 0;
		manX = 0;
	}
	else if (manX>672)
	{
		manVX = 0;
		manX = 672;
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, manId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_BLEND);											//ENABLES 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			//		  TRANSPARENCY
	glScalef(1, 1, 1);
	glColor4ub(255, 255, 255, 255);
	
	
	
	glBegin(GL_POLYGON);
	if (manDir){									//INVERTING IMAGE ACCORDING TO DIRECTION
		glTexCoord2f(0.0, 1.0);//TOP LEFT
		glVertex2f(manX, manY);//TOP LEFT
		glTexCoord2f(1.0, 1.0);//TOP RIGHT
		glVertex2f(manX + 128.0, manY + 0.0);//TOP RIGHT
		glTexCoord2f(1.0, 0.0);//BOTTOM RIGHT
		glVertex2f(manX + 128.0, manY + 170.0);//BOTTOM RIGHT 
		glTexCoord2f(0.0, 0.0);//BOTTOM LEFT
		glVertex2f(manX + 0.0, manY + 170.0);//BOTTOM LEFT
	}
	else{
		glTexCoord2f(1.0, 1.0);//TOP RIGHT
		glVertex2f(manX, manY);//TOP LEFT
		glTexCoord2f(0.0, 1.0);//TOP LEFT
		glVertex2f(manX + 128.0, manY + 0.0);//TOP RIGHT
		glTexCoord2f(0.0, 0.0);//BOTTOM LEFT
		glVertex2f(manX + 128.0, manY + 170.0);//BOTTOM RIGHT 
		glTexCoord2f(1.0, 0.0);//BOTTOM RIGHT
		glVertex2f(manX + 0.0, manY + 170.0);//BOTTOM LEFT
	}
	glEnd();
	//glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void keyOperations()			// FUNCTION FOR KEY OPERATIONS THAT ARE NOT OF TOGGLE TYPE
{
	if (keyStates['w'])
		birdVY = -2.5;
	else
	{
		if (keyStates['s'])
			birdVY = 2.5;
		else
			birdVY = 0;
	}
	if (keyStates[GLUT_KEY_LEFT])
		manVX = -2.5;
	else
	{
		if (keyStates[GLUT_KEY_RIGHT])
			manVX = 2.5;
		else
			manVX = 0;
	}
}

void init(void) 
{
	glClearColor( 0.5, 0.2, 0.6, 1.0 );		//background color of openGl window
	//			 ^red, green, blue, alpha(opaquenss) 
	glMatrixMode( GL_PROJECTION );		//glMatrixMode — specify which matrix is the current matrix ????
	glLoadIdentity();		//glLoadIdentity — replace the current matrix with the identity matrix	????

	glOrtho(0.0, g_Width, 0.0, g_Height, -1.0, 1.0);	//set (0,0) on left top
}

void myMouseFunction( int button, int state, int mouseX, int mouseY ) 
{
	mouse_X=mouseX;
	mouse_Y=mouseY;

	if(button==0 && state==0) //Click
	{
		initBullet();							// CREATE A BULLET ON CLICK
		printf("mouse clicked \n");
		printf("\nmouseX: %d mouseY: %d, State: %d",mouseX,mouseY, state);
	}

}

void myKeyboardFunction( unsigned char key, int mouseX, int mouseY )
{
	switch (key)
	{
	case 'a':
		birdVX = -3;
		break;
	case 'd':
		birdVX = 3;
		break;
	case 'e':
		initShit();						//CREATE A SHIT
		break;
	case 'w':
		keyStates[key] = true;			//CHANGING THE VALUE OF ARRAY FOR USE IN KEYOPERATIONS() FUNCTION
		break;
	case 's':
		keyStates[key] = true;
		break;
	case 'n':
		Reset();
		break;
	case 'p':
		if (game_pause)
			game_pause = false;
		else if (!game_pause)
		{
			game_pause = true;
			glColor3f(1.0f, 0.1f, 0.1f);
			renderBitmapString2(300, 200, "Press 'P' to unpause.");
			glutSwapBuffers();
		}
		break;
	case 27:
		exit(0);				//EXIT THE PROGRAM IF 'esc' PRESSED
		break;
	}
}

void KeyUp(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}

void SpecialKeyUp(int key, int x, int y)
{
	keyStates[key] = false;
}

void mySpecialKeysFunction(int key, int x, int y)
{
	keyStates[key] = true;
	switch( key )
	{
	case GLUT_KEY_LEFT:
		manDir = true;			//manDir=Direction of man
		break;					//true=left		false=right
	case GLUT_KEY_RIGHT:
		manDir = false;
		break;
	}
}

void initShit()
{
	if (!game_pause)				//CHECKS IF GAME IS PAUSED
		for (int i = 0; i < 4; i++)			//FINDS AN EMPTY SLOT, IF THERE IS
		{									
			if (shitV[i] == 0)				//IF VELOCITY ZERO THEN SLOT IS EMPTY
			{
				shitV[i] = 4.25;				//GIVING INITIAL VALUES
				shitX[i] = birdX + 32;
				shitY[i] = birdY + 64;
				PlaySound(TEXT("FART.wav"), NULL, SND_FILENAME | SND_ASYNC);
				break;							//STOPS FINDING OTHER EMPTY SLOTS
			}
		}
}

void initBullet()
{
	if (!game_pause)			//CHECKS IF GAME IS PAUSED
		for (int i = 0; i < 4; i++)		//SEARCHES FOR EMPTY SLOT IF THERE IS
		{								
			if (bulletV[i] == 0)		//IF VELOCITY IS ZERO THEN SLOT IS EMPTY
			{
				float rel_X, rel_Y;
				if (mouse_X <= manX + 60)				//CONDITIONS TO DETERMINE IF LEFT GUN WILL SHOOT OR RIGHT
					rel_X = mouse_X - manX;
				else
					rel_X = mouse_X - manX - 120;
				rel_Y = mouse_Y - manY;
				float angle = atan2f(rel_Y, rel_X);
				bulletV[i] = 6.5;						//GIVING INITIAL VELOCITY
				if (mouse_X<=manX + 60)
					bulletX[i] = manX;				//AGAIN DETERMINING WHICH GUN SHOOTS
				else 
					bulletX[i] = manX + 120;
				bulletY[i] = manY+10;
				bulletVx[i] = bulletV[i] * cosf(angle); //CALCULATE X, Y COMPONENT OF VELOCITY
				bulletVy[i] = bulletV[i] * sinf(angle);
				PlaySound(TEXT("GUNSHOT.wav"), NULL, SND_FILENAME | SND_ASYNC);
				break;
			}
		}
}

void bulletDisplay()
{
	for (int i = 0; i < 4; i++)	//SEARCHES FOR SLOTS CONTAINING BULLETS
	{
		if (bulletV[i]!= 0)		//IF VELOCITY IS NOT ZERO, BULLET EXISTS
		{
			bulletX[i] = bulletX[i] + bulletVx[i] ;			//EQUATIONS FOR MOVEMENT
			bulletY[i] = bulletY[i] + bulletVy[i] ;
			
			
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);							//ENABLING 
			glBindTexture(GL_TEXTURE_2D, bulletId);		//			TRANSPARENCY
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glScalef(1, 1, 1);
			glColor4ub(255, 255, 255, 255);

			glBegin(GL_POLYGON);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(bulletX[i], bulletY[i]);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(bulletX[i] + 7.0, bulletY[i] + 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(bulletX[i] + 7.0, bulletY[i] + 7.0); 
			glTexCoord2f(1.0, 0.0);
			glVertex2f(bulletX[i] + 0.0, bulletY[i] + 7.0);

			glEnd();
			glPopMatrix();

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			if (bulletX[i] <= 0 || bulletX[i] >= 800)		//DELETE BULLET IF IT GOES OUT OF BOUNDS
				bulletV[i] = 0;
			if (bulletY[i] <= 0 || bulletY[i] >= 590)
				bulletV[i] = 0;
		}
	}
}

void shitDisplay()
{

	for (int i = 0; i < 4; i++)	//SEARCHES FOR SLOTS CONTAINING SHIT
	{
		if (shitV[i] != 0)		//IF VELOCITY IS NOT ZERO, SHIT EXISTS
		{
			shitY[i] = shitY[i] + shitV[i];			//EQUATIONS FOR MOVEMENT
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, shitId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glScalef(1, 1, 1);
			glColor4ub(255, 255, 255, 255);

			glBegin(GL_POLYGON);

			glTexCoord2f(0.0, 1.0);
			glVertex2f(shitX[i], shitY[i]);//draw square for menu
			glTexCoord2f(1.0, 1.0);
			glVertex2f(shitX[i] + 7.0, shitY[i] + 0.0);//2nd
			glTexCoord2f(1.0, 0.0);
			glVertex2f(shitX[i] + 7.0, shitY[i] + 10.0);//3rd 
			glTexCoord2f(0.0, 0.0);
			glVertex2f(shitX[i] + 0.0, shitY[i] + 10.0);//4th

			glEnd();
			glPopMatrix();

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			if (shitY[i] >= 590)			//DELETE SHIT WHEN IT HITS GROUND
			{
				shitV[i] = 0;
				PlaySound(TEXT("HIT.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
	}
}

void TextDisplay()
{
	char score[4];
	
	glColor3f(1.0f, 0.1f, 0.1f);
	//renderBitmapString(10, 20, "CREATED BY:");
	//renderBitmapString(250, 20, "Students of BESE-6B");
	//renderBitmapString(100, 20, "Sohaib Zahid");
	//renderBitmapString(100, 35, "Muhammad Ali");
	//renderBitmapString(100, 50, "Muhammad Junaid Nadeem");

	glColor3f(0.91f, 0.76f, 0.65f);
	renderBitmapString(50, 640, "Instructions:");
	renderBitmapString(50, 660, "First player to 5 wins.");
	renderBitmapString(50, 675, "Press 'N' to reset.");
	renderBitmapString(85, 690, "'P' to pause.");
	renderBitmapString(200, 640, "MAN:");
	renderBitmapString(400, 640, "BIRD:");
	renderBitmapString(200, 660, "Arrow keys to move.");
	renderBitmapString(400, 660, "'W','A','S','D' to move.");
	renderBitmapString(200, 675, "Mouse to aim.");
	renderBitmapString(400, 675, "'E' to Shit.");
	renderBitmapString(200, 690, "Click to shoot.");
	renderBitmapString(600, 640, "SCORE:");
	renderBitmapString(600, 660, "Man:");
	renderBitmapString(630, 660, _itoa(ManScore, score, 10));
	renderBitmapString(600, 675, "Bird:");
	renderBitmapString(630, 675, _itoa(BirdScore, score, 10));
}

void Reset()
{
	//RESET ALL VARIABLES

	birdX = 50, birdY = 10, birdVX = 3, birdVY = 0;
	manX = 400, manY = 445, manVX = 0;
	for (int i = 0; i < 4; i++)
	{
		shitV[i] = 0;
		bulletV[i] = 0;
	}
	sprite = 0;
	ManScore = 0, BirdScore = 0;
	manDir = true;
	game_pause = false;
}

void EndGame()
{
	if (ManScore >= 5)
	{
		game_pause = true;		//stops the game
		glColor3f(1.0f, 0.1f, 0.1f);
		renderBitmapString2(350, 400, "MAN WON!");
		renderBitmapString(350, 430, "Press 'N' for new game.");
	}
	else if (BirdScore >= 5)
	{
		game_pause = true;
		glColor3f(1.0f, 0.1f, 0.1f);
		renderBitmapString2(350, 400, "BIRD WON!");
		renderBitmapString(350, 430, "Press 'N' for new game.");
	}
}

void myReshapeFunction( int width, int height )
{
	glClear(GL_COLOR_BUFFER_BIT);

	g_Width  = width; 
	g_Height = height;

	glViewport (0, 0, g_Width, g_Height);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0.0, g_Width, g_Height, 0.0, -1.0, 1.0);	//change the (0,0) to top left
}

void myTimerFunction( int val )
{
	glutTimerFunc( 33, myTimerFunction, 0 );
	if (!game_pause)						//WORKS IF GAME NOT PAUSED... IF PAUSED, EVERYTHING STOPS
	{
		myDisplayFunction();
		sprite += 1.5;
		keyOperations();
	}
}

void myDisplayFunction()
{
	
	glClear( GL_COLOR_BUFFER_BIT );		//clers the screen after display
	BackDisplay();						//display background
	TextDisplay();						//display all text
	EndGame();							//check for game end
	manDisplay();						//display man
	birdDisplay();						//display bird
	collisioncheck();					//check for collision of bullets and shits
	bulletDisplay();					//display bullet
	shitDisplay();						//display shit
	

	glutSwapBuffers();		//brings the openGl window on the front


}
