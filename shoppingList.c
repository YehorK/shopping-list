//The program asks user for the amount of money. IN the input.txt file the informatin about desired products,it's amount and price per object is stated. 
// Program calculates how much of what you can buy (according to the priority) and outputs those products to output.txt
//The products that you cannot afford it outputs to the screen.
//"The priority" means that those products that are on the top in the input.txt file will be favored

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char *productName;
	int amount;
	float productPrice; 
}data;

data * mydata;

int check(FILE*); //determines how many different things we can buy in total
float userInput(); //it allows user to enter the initial sum
void outputFunction(FILE*, float, int, float); //output function
float moneyCalc(float, int, int); //calculates how much money we will be left with
void inputFunction(FILE*); //reads from the text file
int main(void){
	
	char fileR[12] = "input.txt", fileW[12] = "output.txt"; //R and W stand for reading and writing
	FILE *fi,*fo;
	fi = fopen(fileR, "r");
	fo = fopen(fileW, "w");
	
	int i = 1;
	float S;
	
	S = userInput();
	
	inputFunction(fi);
	
	fclose(fi);
	fopen(fileR, "r");
	int elements = check(fi);
	
	float moneyLeft;
	
	moneyLeft = moneyCalc(S, i, elements);

	outputFunction(fo, S, elements, moneyLeft);
	
	fclose(fi);
	fclose(fo);
	
	free(mydata);
	
	return 0;
}

////////////////////////////////////////////////////////////////////////

//The function is responsible for reading the input file and allocating dynamic memory 
void inputFunction(FILE* f){
	int size = 1;
	char item1[15];
	int item2;  
	float item3;
	
	while (fscanf(f, "%s %d %f", item1, &item2, &item3) == 3) {
	size++;
	mydata = (data*)realloc(mydata, size * sizeof(data)); //allocating dynamic memory for my structure
	
	mydata[size-1].productName = (char*)calloc(strlen(item1) + 1, sizeof(char)); //handling the case of ProductName, which is a string
	strcpy(mydata[size-1].productName, item1);
	mydata[size-1].amount = item2;
	mydata[size-1].productPrice = item3;
	}

}
////////////////////////////////////////////////////////////////////////

int check(FILE* f){
	int counter=0;
	char item1[15];
	int item2;
	float item3;
	
	while (fscanf(f, "%s %d %f", item1, &item2, &item3) == 3) {
		counter++;
	}
	
	return counter;
}

////////////////////////////////////////////////////////////////////////

float userInput(){
	float num;
	do{
		printf("Please enter the sum: ");
		scanf("%f", &num);
		printf("\n\n");
	}while(num < 0); //I add this condition because the negative amount of money doesn't make any sense
	return num; 
}

////////////////////////////////////////////////////////////////////////

float moneyCalc(float sum, int i, int el){
	
	int number, j;
	float price; 
	
	number = mydata[i].amount;
	price = mydata[i].productPrice;
	
	for(j = 0; j < number; j++){
		sum = sum - price;
		if (sum < 0){
			sum = sum + price;
			return sum;
		}
		else if(sum == 0){
			return sum;
		}
		else{
			continue;}
	}
	
	if(i < el){
		return moneyCalc(sum, i+1, el);
	} else{
		return sum;}
		
}

////////////////////////////////////////////////////////////////////////

void outputFunction(FILE* f, float sum, int el, float sumLeft){
	int i;
	for(i = 1; i <= el; i++){ //this for-loop is made to change the products
		int j;
		int counter1 = 0;
		int counter2 = 0;
		for(j = 0; j < mydata[i].amount; j++){ //this for-loop is made for checking how many of the same products we actually can buy
			sum = sum - mydata[i].productPrice;
			if (sum >= sumLeft){
				counter1++;
			}
			else{
				counter2++;
			}
		}
		
		if(counter1 != 0){
			//This line outputs into the text file what we can buy
			fprintf(f, "Product Name: %s\nAmount: %d\nProduct Price: %f\n\n", mydata[i].productName, counter1, mydata[i].productPrice);	 
		}
		if(counter2 != 0){
			//This line prints out the products that we cannot buy
			printf("Product Name: %s\nAmount: %d\nProduct Price: %f\n\n", mydata[i].productName, counter2, mydata[i].productPrice);
		}
		
		//in case when we reach the end of our array of products
		if(i == el){
			printf("Finally, you are left with this amount of money: %f\n", sumLeft);
		}
	}
}
