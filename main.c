/* COP 3502C Assignment 1 
This program is written by: Darian Torres*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

FILE *ifp;
FILE *ofp;

typedef struct item { 
    int itemID; 
    int numParts; 
} item; 
//This  stores  one  component  of  a  smoothie  recipe.  The  itemID  represents  the  ingredient  number  and  numParts  
// represents the number of parts of that ingredient. 

typedef struct recipe { 
    int numItems; 
    item* itemList; 
    int totalParts; 
} recipe; 

//This  stores  one  smoothie  recipe.  numItems  stores  the  number  of  different  ingredients,  itemList  will  be  a  
//dynamically  allocated  array  of  item,  where  each  slot  of  the  array  stores  one  ingredient  from  the  recipe,  and  
//totalParts will equal the sum of the numParts of each ingredient in the smoothie. Notice that this is not an array 
//of pointers but just an array of struct. The reason this design decision has been made is because (a) to give you 
//practice with how to deal with an array of struct, and (b) Once the data is read into this array, no changes will be 
//made to the array (i.e., no swapping of elements), thus, this will work equally well for our purposes as an array 
//of pointers. 


//double* amtOfEachItem; //he idea is that amtOfEachItem[i] will store a double 
//equaling the number of pounds for the order for ingredient i. 
//allocate memory for each ingredient before reading store's sales information, then freed after calculatoion

// Pre-condition: utilize variable for the number of ingredients
// Post-condition: Reads in numIngredients, allocates an array of 
//                 strings to store the input, sizes each 
//                 individual string dynamically, and  
//                 returns a pointer to the array. 
char** readIngredients(int *numIngredients){
  char** ingredients=(char **)malloc(sizeof(char*) * *(numIngredients));
  char trash[20];
  for(int i =0; i<*(numIngredients); i++){
    fscanf(ifp,"%s", trash);
    ingredients[i]=(char*)malloc(sizeof(char)* strlen(trash)+1);
    strcpy(ingredients[i],trash);
  }
  return ingredients;
}

// Pre-condition: None
// Post-condition: Reads in details of a recipe such as numItems, 
//                 dynamically allocates space for a single 
//                 recipe and an array of item, updates the 
//                 numItems field of the struct, fills the 
//                 array of items and returns a pointer to the struct 
//                 dynamically allocated. 
recipe* readRecipe(){
  int amOfIngredients;
  fscanf(ifp, "%d", &amOfIngredients);
  recipe* recipeI=(recipe *)malloc(sizeof(recipe));
  recipeI->itemList=(item*) malloc(sizeof(item)*amOfIngredients);
  recipeI->numItems=amOfIngredients;
  int temp=0;
  for(int i=0; i<amOfIngredients; i++){ fscanf(ifp, "%d %d", &recipeI->itemList[i].itemID, &recipeI->itemList[i].numParts);
    temp+=recipeI->itemList[i].numParts;
  }
  recipeI->totalParts=temp;
  return recipeI;
}

// Pre-condition: reference to a variable to store number of recipes. 
// Post-condition: Dynamically allocates an array of pointers
//                 to recipes of size numR, reads number
//                 of recipes from standard input, creates 
//                 structs to store each recipe and has the  
//                 pointers point to each struct.
recipe** readAllRecipes(int *numRecipes){
  recipe** recipes=(recipe**)malloc(sizeof(recipe*) * *(numRecipes));
  for(int i=0; i< *(numRecipes); i++){
    recipes[i]=readRecipe();
  }
  return recipes;
} 

// Pre-condition: Condition on num smoothies(0 < numSmoothies <= 100000), recipeList is  
//    pointing to the list of all smoothie recipes, and  
//    numIngredients equals the number of total ingredients.
// Post-condition: Reads in number of smoothie orders and dynamically  
//    allocates an array of numIngredients such  
//    that index i stores the # of pounds of ingredient i 
//    needed to fulfill all smoothie orders and returns a pointer  
//    to the array. 
double* calculateOrder(int ingredientCount, int numSmoothies, recipe** recipeList){
  double *Array=(double *)calloc(ingredientCount, sizeof(double));
  int numStores, numStoreRec, recNum, totalUnits;
  fscanf(ifp, "%d", &numStoreRec);
  for(int j=0; j<numStoreRec; j++){
    fscanf(ifp, "%d %d", &recNum, &totalUnits);
    for(int k=0; k<recipeList[recNum]->numItems; k++)
      Array[recipeList[recNum]->itemList[k].itemID]+=(recipeList[recNum]->itemList[k].numParts * totalUnits)/(recipeList[recNum]->totalParts * 1.0);
  }
  return Array;
}

// Pre-conditions: ingredientNames store the names of each 
//                 ingredient and orderList stores the amount 
//                 to order for each ingredient, and both arrays  
//                 are of size numIngredients. 
// Post-condition: Prints out a list, in ingredient order, of each 
//                 ingredient, a space and the amount of that 
//                 ingredient to order rounded to 6 decimal 
//                 places. One ingredient per line. 
void printOrder(char** ingredientNames, double* orderList, int numIngredients){
  for(int i=0; i<numIngredients; i++)
    if(orderList[i] !=0){
      printf("%s %.6lf\n", ingredientNames[i], orderList[i]);
      fprintf(ofp, "%s %.6lf\n", ingredientNames[i], orderList[i]);}
  printf("\n");
  fprintf(ofp,"\n");
}


// Pre-conditions: ingredientList is an array of char* of size  
//          numIngredients.
// Post-condition: all the memory pointed to by ingredientList is  
//                 freed. 
void freeIngredients(char** ingredientList, int numIngredients){
  for(int i=0; i<numIngredients; i++){
    free(ingredientList[i]);}
  free(ingredientList);
}
 
 
// Pre-conditions: allRecipes is an array of recipe* of size  
//                numRecipes with each recipe* dynamically allocated  
//              to point to a single recipe. 
// Post-condition: all the memory pointed to by allRecipes is  
//                 freed. 
void freeRecipes(recipe** allRecipes, int numRecipes){
  for(int i=0; i<numRecipes; i++){
    free(allRecipes[i]->itemList);
    free(allRecipes[i]);
    }
  free(allRecipes);
}


int main(){
  atexit(report_mem_leak);
  recipe** smoothieList;
  double* amtOfEachItem;
  int numIngredients, numR, numStores;
  ifp=fopen("in.txt","r");
  ofp=fopen("out.txt","w");
  fscanf(ifp, "%d",&numIngredients);
  char **ingredients = readIngredients(&numIngredients);
  fscanf(ifp, "%d", &numR);
  smoothieList=readAllRecipes(&numR);
  fscanf(ifp, "%d", &numStores);
  for(int i =0; i<numStores; i++){
    amtOfEachItem=calculateOrder(numIngredients,numR,smoothieList);
    printf("Store #%d:\n", i+1);
    fprintf(ofp, "Store #%d:\n", i+1);
    printOrder(ingredients, amtOfEachItem,numIngredients);
    free(amtOfEachItem);
  }
  freeIngredients(ingredients, numIngredients);
  freeRecipes(smoothieList, numR);
}
