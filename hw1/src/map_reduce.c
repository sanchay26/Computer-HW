//**DO NOT** CHANGE THE PROTOTYPES FOR THE FUNCTIONS GIVEN TO YOU. WE TEST EACH
//FUNCTION INDEPENDENTLY WITH OUR OWN MAIN PROGRAM.
#include "../include/map_reduce.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "../include/const.h"
//Implement map_reduce.h functions here.
// Help Menu Print Funtion 
int help(){
printf("%s\n", "Usage: ./mapreduce [h|v] FUNC DIR");
printf("%s\n","FUNC");
printf("%s\n","Which operation you would like to run on the data:");
printf("%s\n","ana - Analysis of various text files in a directory.");
printf("%s\n","stats - Calculates stats on files which contain only numbers.");
printf("%s\n","DIR");
printf("%s\n","The directory in which the files are located.");
printf("%s\n","Options:");
printf("%s\n","-h");
printf("%s\n","Prints this help menu.");
printf("%s\n","-v");
printf("%s\n","Prints the map function’s results, stating the file it’s from.");
return 0;
}
// Validate Agrument Function Defination 
int validateargs(int argc, char** argv){
	
	// Invalidate Arguments 0 or 1 Argument 
	if (argc < 2){
	printf("%s\n","Invalid Arguments");
	help();
	return -1;
	}
	
	else if(argc == 2){
		//validating first argument 
		if (strcmp("./map_reduce",*argv) == 0){
			
			argv++;

			if(strcmp(*argv,"-h") == 0){
				// print menu here 
				help();
				return EXIT_SUCCESS;
			}
			else {
				//print menu here 
				help();
				return -1;
				}
		}
		else{
			printf("%s\n","Invalid Arguments");
			help();
			return -1;
			//if first argument is not a ./mapreduce 
		}

	}
	else if (argc == 3){

		if(strcmp(*argv,"./map_reduce")==0){
			
			argv++;
			
			if(strcmp(*argv,"-h")==0){
				// print menu here 
				help();
				return EXIT_SUCCESS;
			}

			else if (strcmp(*argv,"ana")==0){
				printf("%s\n",*argv );
				argv++;
				printf("%s\n%s\n",*argv,"this" );
				DIR* dir = opendir(*argv);
				if (dir)
				{
    				/* Directory exists. */
    				closedir(dir);
    				return 1;
				}
				else 
				{
					printf("%s\n","Invalid Directory");
					return -1;
    			/* Directory does not exist. */
				}
			}

			else if (strcmp(*argv,"stats")){
				argv++;
				DIR* dir = opendir(*argv);
				if (dir)
				{
    				/* Directory exists. */
    				closedir(dir);
    				return 2;
				}
				else 
				{
					printf("%s\n","Invalid Directory");
					return -1;
    			/* Directory does not exist. */
				}
			}
			else {
				printf("%s\n","Invalid Arguments");
				return -1;
			}
		}
	}
	else if( argc==4){

	}
	return -1;
}

// counting number of files 
int nfiles(char* dir){
	int number_of_files = 0;
	DIR * directory;
	struct dirent * entrance;

	directory = opendir(dir); 
	while ((entrance = readdir(directory)) != NULL) {
    	// Checking if it is a file 
    	if (entrance->d_type == DT_REG) { 
         number_of_files++;
        }
	}
	closedir(directory);
	return number_of_files;
}

int map(char* dir, void* results, size_t size, int (*act)(FILE* f, void* res, char* fn)){

	DIR * directory;
	struct dirent * entrance;

	directory = opendir(dir); 
	memset(results,'\0',NFILES*size);
	void *res;
    res=results;
	while ((entrance = readdir(directory)) != NULL) {
    	// Checking if it is a file 
    	if (entrance->d_type == DT_REG) { 
         char relativepath[strlen(dir)+strlen(entrance->d_name)+1];
		 strcpy(relativepath,dir);
         strcat(relativepath,"/"); 
		 strcat(relativepath,entrance->d_name);
         printf("Sanchay%s\n",relativepath);
         char *filepath = relativepath;
         //Check if it is correct 
         char *duplicatefilepath = strdup(entrance->d_name);
         printf("%s\n",duplicatefilepath);
         printf("%s\n",filepath);
         
         FILE * fp;
		 fp = fopen (filepath, "r");
		 int g=act(fp,res, duplicatefilepath);
		 printf("%d\n",g);
		 fclose(fp);
		 res = res + size;
		 //results = results + size;
    	}
	}
	closedir(directory);
return 0;
}


//Analysis Function

int analysis(FILE* f, void* res, char* filename){
	char c;
	int n=0;
	int line_number = 1;
    int longest_line_length = 0;
    int longest_line_number = 0;
    int total_bytes = 0;
    int ascii_code=0;
    while((c = fgetc(f)) != EOF) {
    	ascii_code = (int)c;
    	printf("%d\n",ascii_code );
    	total_bytes = total_bytes + sizeof(c);
        if(c =='\n'){
        	if(longest_line_length < n){
        		longest_line_length = n;
        		longest_line_number = line_number;
        		}
        		n=0;
        		line_number++;    		
        }
        else if (c!='\n')
    	{
    		n++;
    	}
    }
    printf("File: %s\n", filename);
    printf("Longest line lenght: %d\n",longest_line_length);
    printf("longest line number: %d\n",longest_line_number);
    printf("total bytes in file %d\n", total_bytes);
    struct Analysis *pointer=(struct Analysis*)res;
    pointer->lnlen= longest_line_length;
    pointer->lnno= longest_line_number;
  	pointer->filename=filename; 
    //printf("Saved filename %s\n",pointer->filename );
	return total_bytes;
}

float mean(int total, int count){
	float m =((float)total)/((float)count);
	return m ; 
}
int sort(FILE* f, int count){
	//printf("sort called\n" );
	int c;
    int n = 0;
    int array[count];
    while((fscanf(f,"%d",&c)) != EOF){
        array[n]=c;
        n++;
    }
    for(int i=0;i<n;i++){
    	printf("Array%d\n",array[i]);
    }
    printf("\n");
    return n;
return 0;
}

int stats(FILE* f, void* res, char* filename){
	int c;
	int count=0;
	int total = 0;
	int min=0;
	int max=0;
	float average;
	int array[NVAL];
	
	//intializing array to zero
	for(int i=0;i<NVAL;i++){
		array[i]=0;
	}

	while((fscanf(f,"%d",&c)) != EOF){
		//printf("%d\n",c );
		array[c]++;
		count=count+1;
		total=total+c;
		if(c>max)
           max=c;
        if(c<min)
           min=c;
	}
	average = mean(total,count);
	printf("Mean %f\n", average);
	//printf("minimum:%d\n",min );
	//printf("maximum:%d\n",max );
	struct Stats *pointer=(struct Stats*)res;
	pointer->sum=total;
	pointer->n=count;
	pointer->filename=filename;
	for(int i=0;i<NVAL;i++){
		pointer->histogram[i]=array[i];
	}
	printf("%d\n",pointer->sum);
	printf("%d\n",pointer->n);
	printf("%s\n",pointer->filename);
	for(int i=0;i<NVAL;i++){
		printf("%d\n",pointer->histogram[i] );
	}
	
	return 0;;
}



