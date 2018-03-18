/* Name       :- BHAVI CHAWLA
   Roll Number:- 201601011
   Branch	  :- CSE 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct matrix{
	int p[16];
}MATRIX;

typedef struct state{
	MATRIX current,parent;
	int g, h, f;
	int x, y;
	int pos;
	int remove;
}STATE;

typedef struct node{
	STATE *list;
	struct node *next;

}NODE;

long long int numberOfNodesGenerated;

void disp(NODE *open)
{
	struct node *temp;
	temp = open;
	printf("\nPriority Queue: ");
	while(temp!=NULL)
	{
		printf("\n$$\nh is %d g is %d f is %d pos is %d \n", temp->list->h, temp->list->g, temp->list->f, temp->list->pos);
		for(int i=0;i<16;i++){
			if(i%4==0) printf("\n");
			printf("%d ", temp->list->current.p[i]);
		}
		printf("\n");
		temp=temp->next;
	}
printf("\n");
} 

int numberOfMisplacedTiles(STATE *cur_state){
	int count = 0;
	for(int i = 0; i<15; i++){
		if(cur_state->current.p[i]!=i+1){
			count++;
		}
	}
	if(cur_state->current.p[15]!=0) count++;
	return count;
}

int positionOfBlank(STATE *cur_state){
	int pos;
	for(int i = 0; i<16; i++){
		if(cur_state->current.p[i]==0){
			pos = i;
			break;
		}
	}
	return pos;
}

NODE *push(NODE *open, STATE *state, int pri){
	//printf("inside push\n");
	struct node *temp, *t;
	temp = (struct node *)malloc(sizeof(struct node));
//	printf("%d %d\n", state->h, state->pos);
	temp->list = state;

	// temp->list->current = state->current;
	// temp->list->h = state->h;
	// temp->list->pos = state->pos;
	
	/*for(int i=0; i<16; i++){
		if(i%4 == 0) printf("\n");
		printf("%d ", state->current.p[i]);
	}*/
	//printf("OPENING2 - %d\n", open->list->current.p[0]);
	if(open==NULL){
			open = temp;
			//printf("gugu\n");
	}
	else if(open->list->f > pri)
	{
		//printf("greater\n");
		temp->next=open;
		open=temp;
	}
	else
	{
		// printf("OPENING3 - %d\n", open->list->current.p[0]);
		// printf("OPEN initially\n");
		// for(int i=0; i<16; i++){
		// 	printf("%d ",open->list->current.p[i]);
		// }
	//	printf("smaller\n");
		t=open;
		//disp(t);
		while(t->next!=NULL && (t->next)->list->f <= pri ){
			if((t->next)->list->f == pri){
				if((t->next)->list->h > state->h) break;
			}
			t=t->next;
		}
		temp->next=t->next;
		t->next=temp;
	}
	return open;
}

void append(STATE *new_data, NODE** head_ref){
    NODE* new_node = (NODE*) malloc(sizeof(NODE));
 
    NODE *last = *head_ref;
 
    new_node->list = new_data;
    // ->current  = new_data->current;
    // new_node->list->h = new_data->h;
    // new_node->list->f = new_data->f;
    // new_node->list->g = new_data->g;
    // new_node->list->pos = new_data->pos;
 
    new_node->next = NULL;
 
    if (*head_ref == NULL){
       *head_ref = new_node;
       return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
    return;
}
 



int getInvCount(STATE *initial){
    int inv_count = 0;
    for (int i = 0; i < 15; i++){
        for (int j = i + 1; j <16; j++){            
            if (initial->current.p[j] && initial->current.p[i] && initial->current.p[i] > initial->current.p[j])
                inv_count++;
        }
    }
    return inv_count;
}
int findXPosition(STATE *initial){
    for (int i = 4 - 1; i >= 0; i--)
        for (int j = 4 - 1; j >= 0; j--)
            if (initial->current.p[4*i + j] == 0)
                return 4 - i;
}
int isSolvable(STATE *initial){
    int invCount = getInvCount(initial);
    
        int pos = findXPosition(initial);
        if (pos%2==0 && invCount%2!=0)
            return 1;
        else if(pos%2==1 && invCount%2==0)
            return 1;
        else
        	return 0;    
}


void take_input(STATE *initial){
	//STATE initial;
	int a;
	initial->h = 0;
	printf("input 0 for blank space\nand all inputs between numbers 1 to 15\n");
	for(int i=0;i<16;i++){
		scanf("%d",&a);
		initial->current.p[i]=a;
		if(a==0){
			initial->pos=i;
			initial->x = i/4;
			initial->y = i%4;
		}

		if(a!=i+1){
			initial->h += 1; 
		}
	}
	initial->remove = 0;
	initial->parent=initial->current;
	initial->g = 0;
	//initial->h = numberOfMisplacedTiles(initial);
	initial->f=initial->g + initial->h;

	// for(int i=0;i<16;i++){
	// 	printf("%d ",initial->current.p[i]);
	// }
	//return initial;
}

void findBestInOpen(NODE *open, STATE* best){
	NODE *temp = (NODE*)malloc(sizeof(NODE*));
	STATE *bestTemp = (STATE*)malloc(sizeof(STATE*));
	temp = open;
	best->current = temp->list->current;
	best->f = temp->list->f;
	best->h = temp->list->h;
	best->g = temp->list->g;
	best->pos = temp->list->pos;
	open->list->remove = 1;
	//best->remove = 1;
	/*int bestF = 0;
	while(temp != NULL){
		if(temp->list->f >= bestF ){
			bestF = temp->list->f;
			for(int i=0;i<16;i++){
				best->current.p[i] = temp->list->current.p[i];
			}
			best->f = bestF;
			best->h = temp->list->h;
			best->g = temp->list->g;
			best->pos = temp->list->pos;
			temp = temp->next;
		}
	}*/
	
}

void remove_expanded(NODE **open){
	//printf("removing expanded\n");
	NODE *temp, *prev;
	temp = *open;
	if (temp != NULL && temp->list->remove == 1){
	//	printf("1removed = %d\n", temp->list->pos);
        *open = temp->next;   
        free(temp);            
        return;
    }
    while (temp != NULL && temp->list->remove != 1){
    	//printf("rr%d %d\n", temp->list->pos, temp->list->remove );
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
  //  printf("2removed = %d\n", temp->list->pos);
    free(temp); 
}

int checkInClosed(STATE* newState, NODE **closed){
	struct node *temp;
	temp = *closed;
	int check = 0;
	//printf("\nCHECKING IN CLOSED : \n\n ");
	while(temp!=NULL){
		int flag = 0;
		for(int i=0;i<16;i++){
			if(temp->list->current.p[i] != newState->current.p[i]){
				flag = 1;
				break;
			}
		}
		if(flag == 0){
			//printf("\nduplicate in closed\n");
			check = 1;
			if(newState->f < temp->list->f){
				// Remove from Closed and Add in Open
				temp->list->remove = 1;
				//printf("UPDATED CLOSED\n");
				//remove_expanded(closed);
				//open = push()
				return 1;
			}
			return 2;
		}
		//printf("\n");
		temp=temp->next;
	}
	return 0;
}

// 1 2 3 4 5 6 7 8 9 11 0 10 13 12 15 14'
/*
1 2 3 4
5 6 7 8
9 11 10 13
12 0 15 14

6 13 7 10 8 9 11 0 15 2 12 5 14 3 1 4
*/

int checkInOpen(STATE* newState, NODE **open){
	struct node *temp;
	temp = *open;
	int check = 0;
	//printf("\nCHECKING IN CLOSED : \n\n ");
	while(temp!=NULL){
		int flag = 0;
		for(int i=0;i<16;i++){
			if(temp->list->current.p[i] != newState->current.p[i]){
				flag = 1;
				break;
			}
		}
		if(flag == 0){
			printf("\nduplicate in open\n");
			check = 1;
			if(newState->f < temp->list->f){

		//		
				temp->list->remove = 1;
				// Remove from Closed and Add in Open
				// temp->list->remove = 1;
				// printf("UPDATED OPEN\n");
				//remove_expanded(closed);
				//open = push()
				return 1;
			}
			return 2;
		}
		//printf("\n");
		temp=temp->next;
	}
	return 0;
}

NODE *expand(STATE *best, NODE *open, NODE *closed){
	// Find all neighburs of best add in open
	//printf("\n%d %d %d\n", best->h, best->x, best->y);
	if(best->pos/4 > 0){
		STATE *newState = (STATE *) malloc(sizeof(STATE));
		int temp;
		newState->current = best->current;
		temp = newState->current.p[best->pos];
		newState->current.p[best->pos] = newState->current.p[best->pos-4];
		newState->current.p[best->pos-4] = temp;
 
		newState->h = numberOfMisplacedTiles(newState);
		newState->pos = positionOfBlank(newState);
		newState->g = best->g + 1;
		newState->f = newState->h + newState->g;
		newState->remove = 0;

		//checkInClosed(newState, &closed)
	//	printf("fufufufufuffufufufufufufufufufufufufufufufufufufufufu\n");
		int o = checkInOpen(newState, &open);
		int c = checkInClosed(newState, &closed);

		//printf("***********************************************\n");
		// for(int i=0;i<16;i++){
		// 	if(i%4 == 0) printf("\n");
		// 	printf("%d ", newState->current.p[i] );
		// }
		//printf("\n%d", newState->h );
		//printf("o and c and f%d %d %d\n****************************************",o, c, newState->f );
		if(!o && !c){
			numberOfNodesGenerated+=1;
			//printf("pushing normally\n");
			open = push(open, newState, newState->f);
		}
		else if(o == 1){
			numberOfNodesGenerated+=1;
			//printf("In open already still adding\n");
			remove_expanded(&open);
			open = push(open, newState, newState->f);
			//printf("O IS UPDATTED #####################################################################################");
		}
		else if(c == 1){
			//printf("In closed already still adding in open\n");
			remove_expanded(&closed);
			//printf("C IS UPDATTED #####################################################################################");
			open = push(open, newState, newState->f);
		}
	}
	if(best->pos/4 < 3){
		
		STATE *newState = (STATE *) malloc(sizeof(STATE));
		int temp;
		newState->current = best->current;
		temp = newState->current.p[best->pos];
		newState->current.p[best->pos] = newState->current.p[best->pos+4];
		newState->current.p[best->pos+4] = temp;
		
		newState->h = numberOfMisplacedTiles(newState);
		newState->pos = positionOfBlank(newState);
		newState->g = best->g + 1;
		newState->f = newState->h + newState->g;
		newState->remove = 0;

		int o = checkInOpen(newState, &open);
		int c = checkInClosed(newState, &closed);

		//printf("***********************************************\n");
		// for(int i=0;i<16;i++){
		// 	if(i%4 == 0) printf("\n");
		// 	printf("%d ", newState->current.p[i] );
		// }
		//printf("\n%d", newState->h );
		//printf("o and c and f%d %d %d\n****************************************",o, c, newState->f );
		if(!o && !c){
		//	printf("pushing normally\n");
			open = push(open, newState, newState->f);
		}
		else if(o == 1){
		//	printf("In open already still adding\n");
			remove_expanded(&open);
			open = push(open, newState, newState->f);
			//printf("O IS UPDATTED #####################################################################################");
		}
		else if(c == 1){
		//	printf("In closed already still adding in open\n");
			remove_expanded(&closed);
			//printf("C IS UPDATTED #####################################################################################");
			open = push(open, newState, newState->f);
		}
	}
	if(best->pos%4 != 0){
		
		STATE *newState = (STATE *) malloc(sizeof(STATE));
		int temp;
		newState->current = best->current;
		temp = newState->current.p[best->pos];
		newState->current.p[best->pos] = newState->current.p[best->pos-1];
		newState->current.p[best->pos-1] = temp;
		
		newState->h = numberOfMisplacedTiles(newState);
		newState->pos = positionOfBlank(newState);
		newState->g = best->g + 1;
		newState->f = newState->h + newState->g;
		newState->remove = 0;

		int o = checkInOpen(newState, &open);
		int c = checkInClosed(newState, &closed);

		//printf("***********************************************\n");
		// for(int i=0;i<16;i++){
		// 	if(i%4 == 0) printf("\n");
		// 	printf("%d ", newState->current.p[i] );
		// }
		//printf("\n%d", newState->h );
		//printf("o and c and f%d %d %d\n****************************************",o, c, newState->f );
		if(!o && !c){
		//	printf("pushing normally\n");
			open = push(open, newState, newState->f);
		}
		else if(o == 1){
		//	printf("In open already still adding\n");
			remove_expanded(&open);
			open = push(open, newState, newState->f);
			//printf("O IS UPDATTED #####################################################################################");
		}
		else if(c == 1){
		//	printf("In closed already still adding in open\n");
			remove_expanded(&closed);
			//printf("C IS UPDATTED #####################################################################################");
			open = push(open, newState, newState->f);
		}
	}
	if(best->pos%4 != 3){
		
		STATE *newState = (STATE *) malloc(sizeof(STATE));
		int temp;
		newState->current = best->current;
		temp = newState->current.p[best->pos];
		newState->current.p[best->pos] = newState->current.p[best->pos+1];
		newState->current.p[best->pos+1] = temp;
		
		newState->h = numberOfMisplacedTiles(newState);
		newState->pos = positionOfBlank(newState);
		newState->g = best->g + 1;
		newState->f = newState->h + newState->g;
		newState->remove = 0;

		int o = checkInOpen(newState, &open);
		int c = checkInClosed(newState, &closed);

		//printf("***********************************************\n");
		// for(int i=0;i<16;i++){
		// 	if(i%4 == 0) printf("\n");
		// 	printf("%d ", newState->current.p[i] );
		// }
		//printf("\n%d", newState->h );
	//	printf("o and c and f%d %d %d\n****************************************",o, c, newState->f );
		if(!o && !c){
	//		printf("pushing normally\n");
			open = push(open, newState, newState->f);
		}
		else if(o == 1){
	//		printf("In open already still adding\n");
			remove_expanded(&open);
			open = push(open, newState, newState->f);
			//printf("O IS UPDATTED #####################################################################################");
		}
		else if(c == 1){
	//		printf("In closed already still adding in open\n");
			remove_expanded(&closed);
			//printf("C IS UPDATTED #####################################################################################");
			open = push(open, newState, newState->f);
		}
	}
	//best->remove = 1;
	//remove_expanded(&open);
	
	//disp(open);
	//printf("\n%%%%%%%%%%%%%%%%\n");
	return open;
}

int main(){
	STATE *initial = (STATE *) malloc(sizeof(STATE));
	NODE *open;
	NODE *closed;
	open = NULL;
	closed = NULL;
	take_input(initial);
	


	MATRIX goal;
	for(int i=0;i<15;i++){
		goal.p[i]=i+1;
	}
	goal.p[15]=0;
	
	open = push(open, initial, initial->f);

	numberOfNodesGenerated+=1;

	int j=6;
	if(isSolvable(initial)){
		while(1){
			STATE *best = (STATE *) malloc(sizeof(STATE));
			if(open==NULL){
				printf("NO SOLUTION\n");
				break;
			}
			findBestInOpen(open, best);
			
			for(int i=0;i<16;i++){
				if(i%4 == 0) printf("\n");
				printf("%d ", best->current.p[i] );
			}

			printf("\nNUMBER OF MISPLACED TILES %d\n\n", numberOfMisplacedTiles(best) );
			numberOfNodesGenerated+=1;
			if(numberOfMisplacedTiles(best) == 0){
				printf("DONE\n");
				printf("Total Moves- %d\n",best->g );
				printf("Time(Total number of Nodes Generated) - %lld\n", numberOfNodesGenerated);
				printf("Space(Number of Nodes in Memory) - %d\n", best->g+1);
				break;
			}
			/*for(int i=0;i<16;i++){
				printf("%d ",best->current.p[i] );
			}*/
			open = expand(best, open, closed);
			append(best, &closed);
			remove_expanded(&open);
			//disp(open);

		}
	//	disp(closed);
		// if(numberOfMisplacedTiles(best) == 0){

		// }

		//disp(open);
	}
	else printf("NOT SOLVABLE\n");
}