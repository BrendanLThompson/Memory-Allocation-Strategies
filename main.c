#include <stdio.h>
#include <stdlib.h>

struct node{
	int id;
	int start;
	int end;
	struct node *link;
} *block_list = NULL;

typedef struct node block_type;

int pm_size;
int hole_algo;
int remaining;
int best_so_far;

void option1() {
	printf("Enter size of physical memory: ");
	scanf("%d", &pm_size);
	printf("%d", pm_size);
	remaining = pm_size;

	printf("Enter hole-fitting algorithm (0=first fit, 1=best_fit): ");
	scanf("%d", &hole_algo);
	printf("%d", hole_algo);
	return;

}


void printProcedure() {
	block_type *current_block = block_list;
	// declare local variables 
	if(current_block != NULL){
       printf("\n\n%d\t%d\t%d\n", current_block->id, current_block->start, current_block->end);
       current_block = current_block->link;
    }
	while (current_block != NULL){
		// print table containing block id, starting address, ending address
		printf("%d\t%d\t%d\n", current_block->id, current_block->start, current_block->end);
		current_block = current_block->link;
	}
	return;
}

void option2() {
	// declare local variables
	int block_id;
	int block_size;
	block_type *new_block, *current_block, *best_block;
	int hole_start, hole_end;
	int best_start, best_end;
	int large_hole = 0;
	best_so_far = pm_size;
	// prompt for block id & block size
	printf("Enter Block ID: ");
	scanf("%d", &block_id);
	printf("%d", block_id);
	printf("Enter Block Size: ");
	scanf("%d", &block_size);
	printf("%d", block_size);
	// check if size of block is larger than remaining unallocated space, if not, return	
	if (block_size > remaining){
		printf("Cannot fit block, not enough memory");
		return;
	}

	// allocate space for new block and set id
	new_block = (block_type *)malloc(sizeof(block_type));
	new_block ->id = block_id;
	
	// if no current blocks, insert block at front of linked list, set fields, return
	if (block_list == NULL){
		new_block->start = 0;
		new_block->end = block_size;
		new_block-> link = NULL;
		block_list = new_block;
		printProcedure();
		return;
	}
	// else traverse list until either appropriate hole is found or the end of the list is reached
	current_block = block_list;

	while (current_block != NULL){
		// if id already exists, reject request and return
		if (block_id == current_block -> id){
			printf("Duplicate ID \n");
			return;
		}

		// set values for start and end of currently found hole
		hole_start = current_block ->end;
		if (current_block -> link != NULL){
			hole_end = current_block -> link -> start;
		}
		else{
			hole_end = pm_size;
		}
		// if hole is large enough for block
		if (block_size <= (hole_end - hole_start)){
			large_hole = 1;
			// if first-fit algorithm
			if (hole_algo == 0){
				// set start & end fields of new block & add block into linked list
				new_block ->start = hole_start;
				new_block ->end = hole_start + block_size;
				remaining -= block_size;
				printProcedure();
				return;	
			}
				// else--best-fit algorithm
			else{
				// if hole is smaller than best so far
				if((hole_end - hole_start) < best_so_far){
					best_so_far = hole_end - hole_start;
					best_start = hole_start;
					best_end = hole_end;
					best_block = current_block;
					// update best block & advance next block 
				}
			}	
		}
		current_block = current_block -> link;
	}	  
	if(large_hole == 0){
		printf("Cannot fot block -- no hole large enough\n");
		return;
	} 
	// set start & end fields of new block & add block into linked list 
	new_block -> start = best_start;
	new_block -> end =  best_start + block_size;
	new_block -> link = best_block -> link;	  
	best_block -> link =  new_block;
	// reduce remaining available memory and return
	remaining -= block_size;
	printProcedure();
	return;	
}

void option3() {
	// declare local variables
	int block_id;
	block_type *current_block;
	block_type *prev_block;

	current_block = block_list;
	prev_block = current_block;
	if(current_block == NULL){
		printf("Memory is empty");
		return;
	}
	// prompt for block id
	printf("Enter block ID: ");
	scanf("%d", &block_id );
	printf("%d", block_id);

	// until block id is found or end of linked list is reached
	while ((current_block != NULL) && (block_id != current_block -> id)){
		prev_block = current_block;
		current_block = current_block -> link;
	}
	
	// if end of linked list reached, print block id not found
	if (current_block == NULL){
		printf("Block ID not found.\n");
	}
	else {
    prev_block->link = current_block->link;
    remaining += current_block->end - current_block->start;
		free(current_block);
	}
	printProcedure();
	return;
}

void option4() {
	// declare local variables 	 
	block_type *current_block;
	current_block = block_list;
	int current_size;
	int prev_end = 0;
	// until end of list is reached
	while(current_block != NULL){
		// calculate current hole size
		current_size = current_block -> end - current_block ->start;
		// adjust start & end fields of current block to eliminate hole
		current_block ->start = prev_end;
		current_block ->end = current_block ->start + current_size;
		prev_end = current_block ->end;
		current_block = current_block -> link;		
	}		
	printProcedure();
	return;
}

int main()
{
	int choice = 0;
	do
	{
		printf("\n\nMemory allocation strategies\n"
			   "1) Enter Parameters\n"
			   "2) Allocate memory for block\n"
			   "3) Deallocate memory for block\n"
				 "4) Defragment memory\n"
				 "5) Quit Program\n"
			   "Enter Selection:\n");
		scanf("%d", &choice);
		printf("%d", choice);
		switch (choice)
		{
		case 1:
			option1();
			break;
		case 2:
			option2();
			break;
		case 3:
			option3();
			break;
		case 4:
			option4();
			break;
		default:
			printf("Exiting\n");
		}
	} while (choice != 5);
	return 1;
}
