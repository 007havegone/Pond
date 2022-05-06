#include <string>
#include <stdio.h>
#include <cstring>          // strcmp
#include <cstdlib>          // exit, atoi

using namespace std;

int MapWidth = 10;
int MapHeight = 10;

bool combine_coords = false;
bool prev_position = true;
bool rewards = false;

//IsValid:
//returns wether a given cell is part of the map
bool IsValid(int x, int y)
{
	if(y >= MapHeight || x >= MapWidth)
		return false;
	
	if(y < 0 || x < 0)
	  return false;

	if(x >= 0 && x < 10  && y >= 0 && y < 2)
		return true;
	if(x >= 5 && x < 8 && y >= 2 && y < 6)
		return true;

	return false;
}

bool CanMove(int x1, int y1, int x2, int y2)
{
	if(x1 == x2 && y1 + 1 == y2)
		return true;
	if(x1 == x2 && y1 - 1 == y2)
		return true;
	if(x1 + 1 == x2 && y1 == y2)
		return true;
	if(x1 - 1 == x2 && y1 == y2)
		return true;
	return false;
}

//PrintOpponentMove:
//prints the part of each action that specifies how the opponent moves 
//with relation to the agent
void PrintOpponentMove(int x, int y, int OpponentX, int OpponentY)
{

	//count the possible directions the agent can move...
	int directions = 0;
	//Check if we need to consider moving in each direction
	bool right = false,
		 left = false,
		 up = false,
		 down = false;
	//Right?
	if(x <= OpponentX && IsValid(OpponentX + 1, OpponentY)){
		directions++;
		right = true;
	}
	//Left?
	if(x >= OpponentX && IsValid(OpponentX - 1, OpponentY)){
		directions++;
		left = true;
	}
	//Up?
	if(y <= OpponentY && IsValid(OpponentX, OpponentY + 1)){
		directions++;
		up = true;
	}
	//Down?
	if(y >= OpponentY && IsValid(OpponentX, OpponentY - 1)){
		directions++;
		down = true;
	}

	if(directions > 0){
		if(combine_coords)
			printf("\t(when (opponent_at_%d_%d)\n", OpponentX, OpponentY);
		else
			printf("\t(when (and (opponent_atx_%d) (opponent_aty_%d))\n", OpponentX, OpponentY);
		printf("\t\t\t(probabilistic ");
	}
	else{
		//printf("\t\t\t(probabilistic 1 (and)", OpponentX, OpponentY);
	}

	if(right){
		if(combine_coords)
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_at_%d_%d))(opponent_at_%d_%d))", .8 / (float)directions,  OpponentX, OpponentY, OpponentX + 1, OpponentY);
		else
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_atx_%d)) (opponent_atx_%d))", .8 / (float)directions,  OpponentX, OpponentX + 1);
	}
	if(left){
		if(combine_coords)
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_at_%d_%d))(opponent_at_%d_%d))", .8 / (float)directions,  OpponentX, OpponentY, OpponentX - 1, OpponentY);
		else
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_atx_%d))(opponent_atx_%d))", .8 / (float)directions,  OpponentX, OpponentX - 1);
	}
	if(up){
		if(combine_coords)
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_at_%d_%d))(opponent_at_%d_%d))", .8 / (float)directions,  OpponentX, OpponentY, OpponentX, OpponentY+1);
		else
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_aty_%d))(opponent_aty_%d))", .8 / (float)directions,  OpponentY, OpponentY+1);
	}
	if(down){
		if(combine_coords)
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_at_%d_%d))(opponent_at_%d_%d))", .8 / (float)directions,  OpponentX, OpponentY, OpponentX, OpponentY-1);
		else
			printf("\n\t\t\t\t\t\t   %f (and (not (opponent_aty_%d))(opponent_aty_%d))", .8 / (float)directions,  OpponentY, OpponentY-1);
	}
	if(directions > 0){
		printf("))\n");
	}
}

void PrintMove(int x1, int y1, int x2, int y2)
{
	printf("(:action move_%d_%d_%d_%d ;;Move from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2, x1, y1, x2, y2);
	if(combine_coords){
		printf(" :precondition (and (not (done)) (at_%d_%d))\n", x1, y1);
		if(rewards){
		  printf(" :effect (and (decrease (reward) 1) (at_%d_%d) (not (at_%d_%d))\n", x2, y2, x1, y1);
		}
		else{
		  printf(" :effect (and (at_%d_%d) (not (at_%d_%d))\n", x2, y2, x1, y1);
		}
	}
	else
	{
	  printf(" :precondition (and (not (done)) (atx_%d) (aty_%d))\n", x1, y1);
	  if(x1 != x2){
	    if(rewards){
	      printf(" :effect (and (atx_%d) (not (atx_%d)) (decrease (reward) 1)\n", x2, x1);
	    }
	    else{
	      printf(" :effect (and (atx_%d) (not (atx_%d))\n", x2, x1);
	    }
	  }
	  else{	//y1 != y2
	    if(rewards){
	      printf(" :effect (and (aty_%d) (not (aty_%d))(decrease (reward) 1)\n", y2, y1);
	    }
	    else{
	      printf(" :effect (and (aty_%d) (not (aty_%d))\n", y2, y1);
	    }
	  }
	}
	for(int opponentX = 0; opponentX < MapWidth; opponentX++)
	{
		for(int opponentY = 0; opponentY < MapHeight; opponentY++)
			if(IsValid(opponentX, opponentY))
				if(prev_position)
					PrintOpponentMove(x1, y1, opponentX, opponentY);
				else
					PrintOpponentMove(x2, y2, opponentX, opponentY);
	}
	printf("         )\n");
	if(combine_coords)
		printf(" :observation (((opponent_at_%d_%d) 1 1))\n\n", x2, y2);
	else
		printf(" :observation (((and (opponent_atx_%d) (opponent_aty_%d)) 1 1))\n\n", x2, y2);
	printf(" )\n");
}

int main(int argc, char* argv[])
{
	for(int i = 1; i < argc; i++)
	{
		const char* cmd = argv[i];
		if(strcmp(cmd, "-h") == 0 || strcmp(cmd, "--help") == 0)
		{
			printf("USAGE: MapGen [options]\n");
			printf("\n");
			printf("\t-h, --help\t\tDisplay Help (how'd you get here if you didn't know this command?)\n");
			printf("\t-c, --combine\t\tCombine coordinates (not set by default)\n");
			printf("\t-d, --dest\t\tMove opponent according to the player's destination (the opponent moves according to player's current position by default)\n");
			printf("\t-x, --width <Number>\tMap width (default = 10)\n");
			printf("\t-y, --height <Number>\tMap height (default = 10)\n");
			printf("\t-r, --rewards <0|1>\tUse Rewards (default = 0)\n");
			exit(-1);
		}
		else if(strcmp(cmd, "-c") == 0 || strcmp(cmd, "--combine") == 0)
			combine_coords = true;
		else if(strcmp(cmd, "-d") == 0 || strcmp(cmd, "--dest") == 0)
			prev_position = false;
		else if(strcmp(cmd, "-x") == 0 || strcmp(cmd, "--width") == 0)
			MapWidth = atoi(argv[++i]);
		else if(strcmp(cmd, "-y") == 0 || strcmp(cmd, "--height") == 0)
			MapHeight = atoi(argv[++i]);
		else if(strcmp(cmd, "-r") == 0 || strcmp(cmd, "--rewards") == 0)
		  rewards = (bool)atoi(argv[++i]);
	}

	int num_squares = 0;
	freopen("tag.pddl", "w", stdout);
	printf("(define (domain tag)\n");
	printf(" (:requirements :probabilistic-effects");
	if(rewards){
	  printf(" :rewards)\n");
	}
	else{
	  printf(")\n");
	}
	printf(" (:predicates\n");
	if(combine_coords){
		for (int x = 0; x < MapWidth; x++)
		{
			for (int y = 0; y < MapHeight; y++)
			{
				printf("  (opponent_at_%d_%d)\n", x, y);
				printf("  (at_%d_%d)\n", x, y);
			}
		}
	}
	else
	{
		for (int x = 0; x < MapWidth; x++)
		{
			printf("  (opponent_atx_%d)\n", x);
			printf("  (atx_%d)\n", x);	  
		}
		for (int y = 0; y < MapHeight; y++)
		{
			printf("  (opponent_aty_%d)\n", y);
			printf("  (aty_%d)\n", y);
		}
	}
	for (int x = 0; x < MapWidth; x++)
		for (int y = 0; y < MapHeight; y++)
			if(IsValid(x, y))
				num_squares++;
	printf("   (tagged)\n");
	printf("   (done)\n");
	printf(")\n"); //End Predicates
	for (int x = 0; x < MapWidth; x++)
	{
		for(int y = 0; y < MapHeight; y++)
		{
			if(IsValid(x, y))
			{
			  printf("(:action tag_%d_%d ;;Tag the opponent in (%d, %d)\n", x, y, x, y);
			  if(combine_coords){
			    printf(" :precondition (and (not (done)) (at_%d_%d) )\n", x, y, x, y);
			    if(rewards){
			      printf(" :effect (and (done) (when (opponent_at_%d_%d) (and (increase (reward) 10) (tagged))) (when (not (opponent_at_%d_%d))  (decrease (reward) 10))))\n\n",x,y, x, y);
			    }
			    else{
			      printf(" :effect (and (done) (when (opponent_at_%d_%d) (tagged))))\n\n",x,y);
			    }
			  }
			  else
			    {
			      printf(" :precondition (and (not (done)) (atx_%d) (aty_%d) )\n", x, y, x, y);
			      if(rewards){
				printf(" :effect (and (done) (when (and (opponent_atx_%d)(opponent_aty_%d))  (and (increase (reward) 10) (tagged)) ) (when (not (and (opponent_atx_%d)(opponent_aty_%d)))   (decrease (reward) 10)  )))\n\n",x,y, x,y);
			      }
			      else{
				printf(" :effect (and (done) (when (and (opponent_atx_%d)(opponent_aty_%d))  (tagged))))\n\n",x,y);
			      }
			    }
			}
		}
	}
	for (int x1 = 0; x1 < MapWidth; x1++)
	{
		for(int y1 = 0; y1 < MapHeight; y1++)
		{
			for(int x2 = 0; x2 < MapWidth; x2++)
			{
				for(int y2 = 0; y2 < MapHeight; y2++)
				{
					if(CanMove(x1, y1, x2, y2) && IsValid(x1, y1) && IsValid(x2, y2))
						PrintMove(x1, y1, x2, y2);
				}
			}
		}
	}
	printf(")"); //End domain

	printf("(define (problem tag_prob)\n");
	printf("  (:domain tag)\n");
	printf("  (:requirements :probabilistic-effects)\n");
	printf("  (:init \n");
	if(combine_coords)
		printf("     (at_0_0)\n");
	else
		printf("     (atx_0) (aty_0)\n");
	printf("     (probabilistic \n");

	for (int x = 0; x < MapWidth; x++)
		for (int y = 0; y < MapHeight; y++)
			if(IsValid(x, y))
			{				
				if(combine_coords)
					printf("      %f (opponent_at_%d_%d)\n", 1.0 / (double)num_squares, x, y);
				else
					printf("      %f (and (opponent_atx_%d) (opponent_aty_%d))\n", 1.0 / (double)num_squares, x, y);
			}

	printf("     )\n");
	printf("  )\n");
	if(rewards){
	  printf("  (:goal (tagged)) (:discount .9) (:goal-reward 0))");
	}
	else{
	  printf("  (:goal (tagged)))");
	}
}
