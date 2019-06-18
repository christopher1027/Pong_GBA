//gba.h

#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT	160

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

#define MEM_IO			0x04000000
#define VRAM			0x06000000

#define VIDEOMODE		0x0003
#define BGMODE			0x0400

#define REG_DISPLAY			(*(volatile uint32 *) (MEM_IO))
#define REG_DISPLAY_VCOUNT	(*(volatile uint32 *) (MEM_IO + 0x0006))
#define REG_KEY_INPUT		(*(volatile uint32 *) (MEM_IO + 0x0130))

#define A		1
#define B		2
#define SELECT	4
#define START	8
#define RIGHT 	16
#define LEFT	32
#define UP		64
#define DOWN	128
#define R 		256
#define L		512

#define SCREENBUFFER	((volatile uint16 *) VRAM)

struct Rect {
	uint32 x, y;
	uint32 w, h;
} player, prevPlayer, ball, prevBall, pScore, cScore, line, computer, prevComputer;
-------------------------------------------------------------------------------------------------------------------------------
//main.c

#include "gba.h"

void sync()
{
	while(REG_DISPLAY_VCOUNT >= 160);
	while(REG_DISPLAY_VCOUNT < 160);
}

uint16 makeColor(uint8 r, uint8 g, uint8 b)
{
	return (r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10);
}

uint32 clamp(int value, int min, int max)
{
	return(value < min ? min : (value > max ? max : value));
}

void drawRect(struct Rect rect, uint16 color)
{
	for(int y = 0; y < rect.h; y++)
	{
		for(int x = 0; x < rect.w; x++)
		{	
			SCREENBUFFER[(rect.y + y) * SCREEN_WIDTH + rect.x + x] = color;
		}
	}
}

int main()
{
	REG_DISPLAY = VIDEOMODE | BGMODE;
	
	int top = 80;
	int left = 0;
	int ballTop = 76;
	int ballLeft = 126;
	int speedRight = 1;
	int speedDown = 1;
	int AI = 80;
	
	line.x = 0;
	line.y = 10;
	line.w = 240;
	line.h = 1;
	
	computer.x = top;
	computer.y = left;
	computer.w = 8;
	computer.h = 32;
	
	prevComputer = computer;
	
	player.x = top;
	player.y = left;
	player.w = 8;
	player.h = 32;
	
	prevPlayer = player;
	
	ball.x = ballLeft;
	ball.y = ballTop;
	ball.w = 8;
	ball.h = 8;
	
	//Computer Score
	cScore.x = 130;
	cScore.y = 0;
	cScore.h = 10;
	cScore.w = 2;
	
	//Player Score
	pScore.x = 110;
	pScore.y = 0;
	pScore.h = 10;
	pScore.w = 2;
	
	prevBall = ball;
	
	while(1)
	{	
		sync();
		
		drawRect(prevPlayer, makeColor(0,0,0));
		drawRect(prevBall, makeColor(0,0,0));
		drawRect(prevComputer, makeColor(0,0,0));
		
		ballLeft += speedRight;
		ballTop += speedDown;
		
		
		if(!((REG_KEY_INPUT) & DOWN))
			top++;
		if(!((REG_KEY_INPUT) & UP)){
			if(top > 10){
				top--;
			}
		}
		/*
		if(!((REG_KEY_INPUT) & LEFT))
			left--;
		if(!((REG_KEY_INPUT) & RIGHT))
			left++;
		left = clamp(left, 0, SCREEN_WIDTH-8);
		*/
		
		top = clamp(top, 0, SCREEN_HEIGHT-player.h);
		ballLeft = clamp(ballLeft, 0, SCREEN_WIDTH-ball.w);
		ballTop = clamp(ballTop, 0, SCREEN_HEIGHT-ball.h);
		
		//Player Scores
		if(ballLeft == SCREEN_WIDTH-ball.w){
			speedRight = -speedRight;
			drawRect(pScore, makeColor(0x1f, 0x1f,0));
			pScore.x = pScore.x - 4;
		}
		
		//Computer Scores
		if(ballLeft == 0){
			speedRight = -speedRight;
			drawRect(cScore, makeColor(0x1f, 0x1f,0));
			cScore.x = cScore.x + 4;
		}
		
		//Ball bounce off of Player
		if((ball.x >= player.x && ball.x <= player.x + player.w) &&
		    (ball.y >= player.y && ball.y <= player.y + player.h)){
			speedRight = -speedRight;
		}
		
		//Ball bounce off of Computer
		if((ball.x <= computer.x && ball.x >= computer.x + computer.w) &&
		    (ball.y >= computer.y && ball.y <= computer.y + computer.h)){
			speedRight = -speedRight;
		}
		
		if(ballTop == 10 || ballTop == SCREEN_HEIGHT-ball.h)
			speedDown = -speedDown;
		
		//Computer AI
		if(ball.y > AI && AI < 130){
			AI++;
		}else{
			AI--;
		}
		
		player.x = left;
		player.y = top;
		prevPlayer = player;
		
		computer.x = 232;
		computer.y = AI;
		prevComputer = computer;
		
		ball.x = ballLeft;
		ball.y = ballTop;
		prevBall = ball;
		
		drawRect(line, makeColor(0x1f, 0x1f,0));
		drawRect(ball, makeColor(0x1f, 0x1f,0));
		drawRect(player, makeColor(0,0,0x1f));
		drawRect(computer, makeColor(0,0,0x1f));
		
	}
	return 0;
}

