#include "myLib.h"

unsigned short *videoBuffer = (u16 *)0x6000000;

/* setPixel
 * A function that will set a pixel to a specified color
 * @param r row
 * @param c column
 * @param color color that the pixel will be 
 */
 
void setPixel(int row, int col, u16 color){

	videoBuffer[OFFSET(row, col)] = color;
}

/* drawRect
 * A function that will draw an arbitrary sized rectangle
 * onto the screen (with DMA).
 * @param r row
 * @param c column
 * @param width width of rectangle
 * @param height height of rectangle
 * @param color color that will be used to fill rectangle
 */
 
void drawRect(int row, int col, int height, int width, volatile u16 color){

	int r;
	for(r=0; r<height; r++)	{
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(row+r, col);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | width;
	}
}

/* drawImage3
 * A function that will draw an arbitrary sized image
 * onto the screen (with DMA).
 * @param r row
 * @param c column
 * @param width width of image
 * @param height height of image
 * @param image pointer to the first element of the image.
 */
 
void drawImage3(int row, int col, int width, int height, const u16* image){
 	for(int r=0; r<height; r++){
		 	DMA[3].src = image + width*r;
		 	DMA[3].dst = videoBuffer + OFFSET(row+r, col);
		 	DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | width;
	 }
}

void waitForVblank(){

	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
	
/* fillScreen
 * A function that will fill the screen with a specified color (with DMA).
 * @param color color that the screen will become
 */
 	
void fillScreen(volatile u16 color){	

	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | 38400;	
}

void delay(int n){

	volatile int i;
	for(i=0; i<n*10000; i++){
	
	}
}



