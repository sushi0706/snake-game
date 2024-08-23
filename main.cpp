#include<iostream>
#include<windows.h>
#include <stdlib.h>
#include<time.h>
#include<vector>
#include<conio.h>
using namespace std;

//Configurable parameters
#define GRID_HEIGHT 20
#define GRID_WIDTH 50
#define SNAKE_RIGHT 1
#define SNAKE_LEFT 2
#define SNAKE_BOTTOM 3
#define SNAKE_TOP 4
#define SPAWN_ROW 5
#define SPAWN_COL 5
#define LEVEL_EASY 1
#define LEVEL_MEDIUM 2
#define LEVEL_HARD 3
#define TIME_SLOW 0.5
#define TIME_MED 0.3
#define TIME_FAST 0.1
#define COLOR_SNAKE BLUE
#define COLOR_FRUIT RED

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

struct gridCoord{
	int row;
	int col;
};

void gotoxy(int row,int col){
	COORD target;
	target.X = col;
	target.Y = row;
	SetConsoleCursorPosition(console,target);
}

void wait(float seconds){
	clock_t endwait;
	endwait=clock()+seconds*CLOCKS_PER_SEC;
	while (clock()<endwait);
}

void hideCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

class Snake{
	
	private:
		int snakeLength;
	    vector<gridCoord> snakePos;
	    int direction;
	    
	public:
		Snake(){
			snakeLength = 1;
			gridCoord spawnPos;
			spawnPos.row = SPAWN_ROW;
			spawnPos.col = SPAWN_COL;
			snakePos.push_back(spawnPos);
			direction = 1;
		}
		
		int getSnakeLength(){
			return snakeLength;
		}
		
		vector<gridCoord> getSnakePos(){
			return snakePos;
		}
		
		void updatePos(){
			if(snakeLength > 1){
				int n = snakeLength;
				
				for(int i=n-1;i>0;i--){
				   snakePos[i].row = snakePos[i-1].row;
				   snakePos[i].col = snakePos[i-1].col;
				}
			}
			
			gridCoord &head = snakePos[0];
			
			if(direction == SNAKE_RIGHT){
				head.col = head.col+1;
			}
			
			if(direction == SNAKE_LEFT){
				head.col = head.col-1;
			}
			
			if(direction == SNAKE_BOTTOM){
				head.row = head.row+1;
			}
			
			if(direction == SNAKE_TOP){
				head.row = head.row-1;
			}
		}
		
		gridCoord getSnakeHead(){
			return snakePos[0];
		}
		
		void setDirection(int dir){
			if((direction == SNAKE_LEFT && dir == SNAKE_RIGHT)|| (direction == SNAKE_RIGHT &&
			dir == SNAKE_LEFT ) || (direction == SNAKE_TOP && dir == SNAKE_BOTTOM) || 
			(direction == SNAKE_BOTTOM && dir == SNAKE_TOP))
			    return;
			    
			direction = dir;
		}
		
		void addToSnake(){
			gridCoord newPartCoord;
			newPartCoord.row = snakePos[snakeLength-1].row-1;
			newPartCoord.col = snakePos[snakeLength-1].col;
			snakePos.push_back(newPartCoord);
			snakeLength += 1;
		}
};

class Grid{
	private:
		char gameGrid[GRID_HEIGHT][GRID_WIDTH];
		int fruitRow = 1;
		int fruitCol = 1;
		Snake mySnake;
		bool gameOver = false;
		
	public:
		
		void generateGrid(){
			for(int i=0;i<GRID_WIDTH;i++){
				gotoxy(i,0);
				gameGrid[0][i] = '#';
				gotoxy(i,GRID_HEIGHT-1);
				gameGrid[GRID_HEIGHT-1][i] = '#';
			}//Top and bottom border
			
			for(int i=0;i<GRID_HEIGHT;i++){
				gotoxy(0,i);
				gameGrid[i][0] = '#';
				gotoxy(GRID_WIDTH-1,i);
				gameGrid[i][GRID_WIDTH-1] = '#';
			}
			
			for(int i=1;i<GRID_HEIGHT-1;i++){
				for(int j=1;j<GRID_WIDTH-1;j++){
					gameGrid[i][j] = '.';
				}
			}//Content
		}
		
		void generateFruit(){
			gameGrid[fruitRow][fruitCol] = '.';
			fruitRow = (rand()%(GRID_HEIGHT-2)) + 1;
			fruitCol = (rand()%(GRID_WIDTH-2))+1;
			if(gameGrid[fruitRow][fruitCol] != '*')
			   gameGrid[fruitRow][fruitCol] = '@';
			else
			   generateFruit(); //Regenerate it
		}
		
		void setGridSnake(){
		   vector<gridCoord> prevSnake(mySnake.getSnakePos());
		   for(int i=0;i<prevSnake.size();i++){
		   	    gridCoord snakePartCoord = prevSnake[i];
		   	    gameGrid[snakePartCoord.row][snakePartCoord.col] = '.';
		   }
		   
		   mySnake.updatePos();
			
		   vector<gridCoord> snake(mySnake.getSnakePos());
		   for(int i=0;i<snake.size();i++){
		   	    gridCoord snakePartCoord = snake[i];
		   	    if(gameGrid[snakePartCoord.row][snakePartCoord.col] == '*'){
		   	    	gameOver = true;
		   	    	return;
				   }
				   
		   	    gameGrid[snakePartCoord.row][snakePartCoord.col] = '*';
		   }
		}
		
		void printGrid(){
			
			for(int i=0;i<GRID_HEIGHT;i++){
				for(int j=0;j<GRID_WIDTH;j++){
					gotoxy(i,j);
					cout<<gameGrid[i][j];
				}
			}
		}
		
		gridCoord getGridSnakeHead(){
			return mySnake.getSnakeHead();
		}
		
		void updateGridSnakeDirection(int newDir){
			mySnake.setDirection(newDir);
		}
		
		bool fruitGrasped(){
			gridCoord snakeHead = mySnake.getSnakeHead();
			if(fruitRow == snakeHead.row && fruitCol == snakeHead.col){
				return true;
			}
			
			return false;
		}
		
		void upgradeSnake(){
			mySnake.addToSnake();
		}
		
		bool checkGameOver(){
			if(gameOver)
			  return true;
			  
			gridCoord snakeHead = mySnake.getSnakeHead();
			
			gameOver = snakeHead.row >= GRID_HEIGHT-1
	        || snakeHead.row <= 0|| snakeHead.col <= 0 ||
	        snakeHead.col >= GRID_WIDTH-1;
	        
	        return gameOver;
		}
		
		int currentScore(){
		   return mySnake.getSnakeLength();	
		}
		
		void showcurrentStatus(int difficultyLevel){
			string difficulty;
			
			switch(difficultyLevel){
				case 1:
					difficulty = "Easy";
					break;
					
				case 2: 
				    difficulty = "Medium";
				    break;
				    
				case 3:
					difficulty = "Hard";
					break;
			}
			gotoxy(GRID_HEIGHT/2,GRID_WIDTH+20);
			cout<<"Difficulty: "<<difficulty;
			gotoxy(GRID_HEIGHT/2 + 1,GRID_WIDTH+20);
			cout<<"Your current score: "<<currentScore();
		}
};

int playGame(int difficultyLevel){
	float waitTime;
	
	switch(difficultyLevel){
		case 1:
			waitTime = TIME_SLOW;
			break;
			
		case 2:
			waitTime = TIME_MED;
			break;
			
		case 3:
			waitTime = TIME_FAST;
			break;
	}
	
	Grid matchGrid;
	matchGrid.generateGrid();
	matchGrid.generateFruit();
	hideCursor();
	
	while(!matchGrid.checkGameOver()){
		matchGrid.printGrid();
		matchGrid.showcurrentStatus(difficultyLevel);
		
		if(_kbhit()){
			char keyPressed = char(_getch());
			
			switch(keyPressed){
				case 'w':
					matchGrid.updateGridSnakeDirection(SNAKE_TOP);
					break;
					
				case 'a':
					matchGrid.updateGridSnakeDirection(SNAKE_LEFT);
					break;
					
				case 's':
					matchGrid.updateGridSnakeDirection(SNAKE_BOTTOM);
					break;
					
				case 'd':
					matchGrid.updateGridSnakeDirection(SNAKE_RIGHT);
					break;
			}
		}
		
		matchGrid.setGridSnake();
		
		if(matchGrid.fruitGrasped()){
			matchGrid.generateFruit();
			matchGrid.upgradeSnake();
		}
		
		wait(waitTime);
	}
	
	system("cls");
	return matchGrid.currentScore();
}

int printInstructions(){
	cout<<"Welcome to the snake game"<<endl;
	cout<<"Press \'w\' to move the snake upwards"<<endl;
	cout<<"Press \'a\' to move the snake leftwards"<<endl;
	cout<<"Press \'s\' to move the snake bottomwards"<<endl;
	cout<<"Press \'d\' to move the snake rightwards"<<endl;
	cout<<"Choose the difficulty level:-"<<endl;
	cout<<"1 - Easy"<<endl;
	cout<<"2 - Medium"<<endl;
	cout<<"3 - Hard"<<endl;
	int level;bool validLevel = false;
	
	while(!validLevel){
		cin>>level;
		
		validLevel = level == LEVEL_EASY || level == LEVEL_MEDIUM || level == LEVEL_HARD;
		if(!validLevel){
			cout<<"Please select the valid level"<<endl;
		}
	}
	
	return level;
}

void printScore(int score){
	cout<<"Game Over"<<endl;
	cout<<"Your Score: "<<score<<endl;
}

int main(){
	int difficultyLevel = printInstructions();
	int score = playGame(difficultyLevel);
	printScore(score);
	getch();
	return(0);
}