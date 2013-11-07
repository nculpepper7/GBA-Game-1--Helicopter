#include <stdlib.h>
#include <stdio.h>

#include "myLib.h"
#include "redcar.h"
#include "menu.h"
#include "text.h"
#include "loseScreen.h"
#include "win_screen.h"

// 4 different states for the game.
enum {START, GAME, WIN, LOSE};

// Define a structure for the top and bottom obstacles.
typedef struct{
		int row;
		int col;
		int color;
		int size;
		int gapStart;
	} WALL;
	
// Prototypes	
void win();
void lose();
void start();
int game();	

// Main function with switch that controls the flow of the game.
int main(){
	int state = START;
	while(1){
		switch(state){
		case START:
			start();
			state = GAME;
			break;
		case GAME:
			state = game();
			break;
		case WIN:
			win();
			state = START;
			break;
		case LOSE:
			lose();
			state = START;
			break;
		default:
			break;
		}
	}
	return 0;
}

// Start menu.
void start(){
	
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	drawImage3(0,0,240,160, menu);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

// Game mode.
int game(){
	
	int row = 80;
	int col = 5;
	int oldrow = row;
	int oldcol = col;
	char buffer[41];
	char buffer1[41];
	int distance = 0;
	int oldDistance = 0;
	int move[] = {-4,-3,-2,-1,0,1,2,3,4};
	int numObj = 47;
	int check1;
	int check2;
	int color = 0x33cc; // color of walls
	
	// Initializing obstacles.
	fillScreen(BLACK);
	WALL obst[numObj];
	for(int j=0; j<numObj; j++){
		obst[j].row = 5;
		obst[j].col = 5 + 5*j;
		obst[j].size = 3*REDCAR_HEIGHT;
		obst[j].color = color;
		obst[j].gapStart = 70;
	}
	
	// Initial drawing.
	for(int l=0; l<numObj; l++){
		drawRect(obst[l].row, obst[l].col, obst[l].gapStart - 5, 5, obst[l].color);
		drawRect(obst[l].gapStart + 3*REDCAR_HEIGHT, obst[l].col, 150-obst[l].gapStart - 3*REDCAR_HEIGHT, 5, obst[l].color);
		drawRect(obst[l].gapStart, obst[l].col, 3*REDCAR_HEIGHT, 5, BLACK);
	}
	
	// Game loop
	while(1){
		
		// Decreases the gap every 50 points.  
		if(distance % 50 == 0){
				obst[numObj-1].size = obst[numObj-1].size - 1;
		}
		
		if(KEY_DOWN_NOW(BUTTON_UP)){
			row--;
			if(row < 5){
				row = 5;
			}	
		}
		
		if(KEY_DOWN_NOW(BUTTON_DOWN)){
			row++;
			if(row > (155 - REDCAR_HEIGHT)){
				row = 155 - REDCAR_HEIGHT;
			}
		}
		
		if(KEY_DOWN_NOW(BUTTON_LEFT)){
			col--;
			if(col < 5){
				col = 5;
			}	
		}
		
		if(KEY_DOWN_NOW(BUTTON_RIGHT)){
			col++;
			if(col > (235 - REDCAR_WIDTH)){
				col = 235 - REDCAR_WIDTH;
			}
		}
		
		// Draws helicopter.
		drawRect(oldrow, oldcol, REDCAR_HEIGHT, REDCAR_WIDTH, BLACK);
		drawImage3(row, col, REDCAR_WIDTH, REDCAR_HEIGHT, redcar);
		oldrow = row;
		oldcol = col;
		
		// Updates and draws the score to the screen.
		distance++;
		drawString(150, 5, "Distance: ", BLUE);
		sprintf(buffer, "%d", distance);
		sprintf(buffer1, "%d", oldDistance);
		drawString(150, 60, buffer1, BLACK);
		drawString(150, 60, buffer, BLUE);
		oldDistance = distance;
		
		// Moves the last obstacle up or down some random amount then moves all obstacles forward.
		obst[numObj-1].gapStart = obst[numObj-1].gapStart+move[rand()%9];
		if(obst[numObj-1].gapStart + obst[numObj-1].size > 150){
			obst[numObj-1].gapStart = 150 - obst[numObj-1].size;
		}
		
		if(obst[numObj-1].gapStart < 5){
			obst[numObj-1].gapStart = 5;
		}
		
		for(int k=0; k<numObj-1; k++){
			obst[k].gapStart = obst[k+1].gapStart;
			obst[k].size = obst[k+1].size;
		}

		// Redraws updated obstacles.
		waitForVblank();
		for(int l=0; l<numObj; l++){
			drawRect(obst[l].row, obst[l].col, obst[l].gapStart - 5, 5, obst[l].color);
			drawRect(obst[l].gapStart + obst[l].size, obst[l].col, 150-obst[l].gapStart - obst[l].size, 5, obst[l].color);
			drawRect(obst[l].gapStart, obst[l].col, obst[l].size, 5, BLACK);
			drawImage3(row, col, REDCAR_WIDTH, REDCAR_HEIGHT, redcar);
		}
		
		// You've won!
		if(distance >= 500){
			return WIN;
		}
		
		// Quitter...
		if(KEY_DOWN_NOW(BUTTON_SELECT)){
			return LOSE;
		}
		
		// Better luck next time!
		for(int m = col/5; m<=6 + col/5; m++){
			check1 = row - obst[m].gapStart;
			check2 = (obst[m].gapStart + obst[m].size)-(row+REDCAR_HEIGHT);
			if(check1<0 || check2<0){
				return LOSE;
			}
		}
	}
	return 0;
}	

// This displays the winning screen.
void win(){
	drawImage3(0,0, 240,160,win_screen);
	while(!KEY_DOWN_NOW(BUTTON_START)); 
	while(KEY_DOWN_NOW(BUTTON_START));  
}

// This displays the losing screen.	
void lose(){
	drawImage3(0,0, 240,160,loseScreen);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}
			

