//6521878 zy21878 Yuyang ZHOU 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>


// Whenever memory allocation failure occurs, the program will exit with integer 5 immediately.

// Defining a linked list data structure including three integers x,y,r to store the no-fly areas read in.
struct area{
	int x;
	int y;
	int r;
	struct area* next;
};
typedef struct area Area;

// Defining a linked list data structure including two integers x,y to store the flight plans read in.
struct waypoint{
	int x;
	int y;
	struct waypoint* next;
};
typedef struct waypoint Wpoint;

// A function to append new elements to Area after the existing ones.
void append_area(Area** start, int val_x, int val_y, int val_r)
{
	Area* n=(Area*)malloc(sizeof(Area));
	if(n==NULL)
	{
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(5);
	}
	n->x=val_x;
	n->y=val_y;
	n->r=val_r;
	n->next=NULL;
	
	if(*start==NULL)
	{
		*start=n;
	}
	else
	{
		Area* cur=*start;
		while(cur->next != NULL)
		{
			cur = cur->next;
		}
		cur->next=n;
	}
}

// A function to append new elements to Waypoint after the existing ones.
void append_wpoint(Wpoint** start, int val_x, int val_y)
{
	Wpoint* n=(Wpoint*)malloc(sizeof(Wpoint));
	if(n==NULL)
	{
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(5);
	}
	n->x=val_x;
	n->y=val_y;
	n->next=NULL;
	
	if(*start==NULL)
	{
		*start=n;
	}
	else
	{
		Wpoint* cur=*start;
		while(cur->next != NULL)
		{
			cur = cur->next;
		}
		cur->next=n;
	}
	
}

/* A function to validate the read in waypoints, 
** if there is less than 2 waypoints or there are two same consecutive waypoints, return failure.
*/
int check_wpoint(Wpoint* start)
{
	int i=0;
	Wpoint* cur=start;
	Wpoint* temp;
	while(cur!=NULL)
	{
		i++;
		temp=cur;
		cur=cur->next;
		if(cur!=NULL)
		{
			if(temp->x==cur->x&&temp->y==cur->y)
			{
				return 0;
			}
		}
		
	}
	if(i<2)
	{
		return 0;
	}
	return 1;
}

// A function to free all the memory used by list Area.
void free_area(Area** start)
{
	Area* cur=*start,*temp;
	while(cur!=NULL)
	{
		temp=cur;
		cur=cur->next;
		free(temp);
	}
	
}

// A function to free all the memory used by list Waypoint.
void free_wpoint(Wpoint** start)
{
	Wpoint* cur=*start,*temp;
	while(cur!=NULL)
	{
		temp=cur;
		cur=cur->next;
		free(temp);
	}
	
}

// Declaring prototypes for functions to read two files and to validate the plan.
int read_nofly(char*,Area**);
int read_flightplan(char*,Wpoint**);
int check(Area* , Wpoint*,Area**);

// Declaring prototype for a function to free any memory allocated by function malloc().
void before_exit(void);

// Declaring a global variable to store integers read from both files for validating.
char* new_val=NULL;
// Declaring two global variables to store the address of read in no-fly zones and flight plans.S
Area* no__fly__zone=NULL;
Wpoint* flight__plan=NULL;

/* The main function to call functions to read in two files and to validate the flight plan, 
** and print corresponding error messages when any error occurs then exit with proper exit number.
** If the flight plan is valid, it will exit with integer 0. 
*/
int main(int argc, char *argv[])
{

	Area* restricted_area_entered=NULL;
	
	atexit(before_exit);
	
	if(argc!=3)
	{
		fprintf(stderr,"Invalid command line arguments. Usage: <noflyzones> <flightplan>\n");
		exit(6);
	}
	
	if(read_nofly(argv[1],&no__fly__zone)!=1)
	{
		fprintf(stderr,"No-fly zone file invalid.\n");
		exit(2);
	}
	
	if(read_flightplan(argv[2],&flight__plan)!=1)
	{
		fprintf(stderr,"Flight plan file invalid.\n");
		exit(3);
	}
	
	if(check(no__fly__zone,flight__plan,&restricted_area_entered)==0)
	{
		fprintf(stdout,"Invalid flight plan.\n"
		"Enters restricted area around %d, %d.\n",restricted_area_entered->x,restricted_area_entered->y);
		exit(4);
	}

	fprintf(stdout,"Flight plan valid.\n");
	exit(0);
}


/* A function to read the no-fly zone file and construct a list to store the values read from the file.
** If fail to open file, exit with integer 1;
** if the file is invalid, return failure to the main function, then main function print error message and exit with integer 2;
** if fail to close the file, it will exit with integer 6.
** if the function runs with no error, it will return true to the main function;
** in addition, it will change the pointer passed by main function to point to the list containing all the valid values read from the no-fly zone file.
*/
int read_nofly(char* no_fly_path,Area** no_fly_zone)
{
	FILE * nfz_text=fopen(no_fly_path,"r");
	if(nfz_text==NULL)
	{
		perror("Cannot open no-fly zone file.");
		exit(1);
	}
	
	*no_fly_zone=NULL;
	
	new_val=(char*)malloc(16*sizeof(char));
	if(new_val==NULL)
	{
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(5);
	}
	
	char text;
	long int tmp;
	
	/* This do while loop is for validating the file and construct the list to store the values read from the no-fly zone file.
	** If a line is started with '#', it will skip the whole line;
	** if a line is started with neither '#' nor a number, it will return -1 for failure;
	** if a line is started with a number, but ending with a character which is not '\n', it will return -1 for failure;
	** if the number of values in the line started with a number is not 3, it will return -1 for failure;
	** if the values in the line are not in the required range, which is 0<=val<10000 and the third value should be greater than 0, it will return -1 for failure;
	** if the values in the line are combined with other charactres in the form as "132dc12", it will reutrn -1 for failure;
	** otherwise, it will construct a list containing all the valid values read from the file, 
	** it will also change the pointer passed by main function to point to the list constructed during the process for further validation.
	*/
	do
	{
		char* word=NULL;
		text=getc(nfz_text);
		if(text==35||text==10||text==EOF)
		{
			
			while(text!=10)
			{
				if(text==EOF)
				{
					break;
				}
				text=getc(nfz_text);
			}
		}
		else if(text>47&&text<58)
		{
			int val[3];
			for(int i=0;i<3;i++)
			{
				int j=0;
				while(i>0&&text!=10&&isspace(text))
				{
					text=getc(nfz_text);
				}
				
				if(text=='\n')
				{
					return -1;
				}
				
				while(isspace(text)==0&&j<15)
				{
					*(new_val+j)=text;
					text=getc(nfz_text);
					j++;
				}
				*(new_val+j)=0;
				
				tmp=strtol(new_val,&word,10);
				
				if(*word!=0||tmp<0||tmp>=10000||(i==2&&tmp==0))
				{
					return -1;
				}
				else
				{
					val[i]=(int)tmp;
				}
			}
			
			if(text==10||text==EOF)
			{
				append_area(no_fly_zone,val[0],val[1],val[2]);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}while(text==10);
	
	
	if(fclose(nfz_text)!=0)
	{
		if(*no_fly_zone!=NULL)
		{
			free_area(no_fly_zone);
		}
		fprintf(stderr,"Failed to close no-fly zone file.\n");
		exit(6);
	}
	return 1;
}




/* A function to read the flight plan file and construct a list to store the values read from the file.
** If fail to open file, exit with integer 1;
** if the file is invalid, return failure to the main function, then main function print error message and exit with integer 2;
** if fail to close the file, it will exit with integer 6.
** if the function runs with no error, it will return true to the main function;
** in addition, it will change the pointer passed by main function to point to the list containing all the valid values read from the flight plan file.
*/
int read_flightplan(char* flight_plan_path,Wpoint** flight_plan)
{
	
	FILE *flp_text=fopen(flight_plan_path,"r");
	if(flp_text==NULL)
	{
		if(no__fly__zone!=NULL)
		{
			free_area(&no__fly__zone);
		}
		perror("Cannot open flight plan file.");
		exit(1);
	}
	
	* flight_plan=NULL;
	char text;
	long int tmp;

	/* This do while loop is for validating the flight plan file and construct the list to store the values read from it.
	** If a line is started with '#', it will skip the whole line;
	** if a line is started with neither '#' nor a number, it will return -1 for failure;
	** if a line is started with a number, but ending with a character which is not '\n', it will return -1 for failure;
	** if the number of values in the line started with a number is not 2, it will return -1 for failure;
	** if the values in the line are not in the required range, which is 0<=val<10000, it will return -1 for failure;
	** if the values in the line are combined with other charactres in the form as "132dc12", it will reutrn -1 for failure;
	** otherwise, it will construct a list containing all the valid values read from the file, 
	** it will also change the pointer passed by main function to point to the list constructed during the process for further validation.
	*/	
	do
	{
		char* word=NULL;
		
		
		text=getc(flp_text);
		if(text==35||text==10||text==EOF)
		{
			
			while(text!=10)
			{
				if(text==EOF)
				{
					break;
				}
				text=getc(flp_text);
			}
		}
		else if(text>47&&text<58)
		{
			int val[2];
			for(int i=0;i<2;i++)
			{
				
				int j=0;
				while(i>0&&isspace(text)&&text!=10)
				{
					text=getc(flp_text);
				}
				
				if(text=='\n')
				{
					return -1;
				}
				
				while(!isspace(text)&&j<15)
				{
					*(new_val+j)=text;
					text=getc(flp_text);
					j++;
				}
				
				*(new_val+j)=0;
				
				tmp=strtol(new_val,&word,10);
				
				if(*word!=0||tmp<0||tmp>=10000)
				{
					return -1;
				}
				else
				{
					val[i]=(int)tmp;
				}
			}
			if(text==10||text==EOF)
			{
				append_wpoint(flight_plan,val[0],val[1]);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
		
		
		
	}while(text==10);
	
	
	if(fclose(flp_text)!=0)
	{
		fprintf(stderr,"Failed to close flight plan file.\n");
		exit(6);
	}
	
	/* To validate the data values stored in the list constructed above.
	** If the number of waypoints is less than 2, it will return -1 for failure;
	** if there are two consecutive waypoints which are totally same, it will return -1 for failure;
	** otherwise, it will let the function go on.
	*/
	if(check_wpoint(*flight_plan)==0)
	{
		return -1;
	}
	
	return 1;
}



/* A function to validate the flight plan.
** If a path enters any no-fly zone, or the waypoint is in any no-fly zone, it will set that no-fly zone as the restricted area entered.
** If the drone enters more than one no-fly zone on one path, it will set the no-fly zone the drone enters first as the restricted area entered. 
** After a path has tested through all no-fly zones, it the restricted area is set, 
** the function will return 0 for failure and change the pointer passed by the main function to the restricted area entered;
** If the drone enters none no-fly zone, the function will return 1 for success.
*/
int check(Area* start_nfz, Wpoint* start_flp,Area** restricted_area_entered)
{
	int xa,ya,xb,yb,xc,yc,r;
	long int X,Y,Z,D;
	double t1,t2,distance,px,py;
	Wpoint* cur_flp=start_flp;
	Area* error;
	error=NULL;
	Area* cur_nfz=start_nfz;
	
	// This while loop check whether the starting waypoint is in any no-fly zone.
	while(cur_nfz!=NULL)
		{
			xa=cur_flp->x;
			ya=cur_flp->y;
			xc=cur_nfz->x;
			yc=cur_nfz->y;
			r=cur_nfz->r;
			if((xa-xc)*(xa-xc)+(ya-yc)*(ya-yc)<r*r)
			{
				*restricted_area_entered=cur_nfz;
			}
			cur_nfz=cur_nfz->next;
		}
		
		
	/* This while loop check whether the path between two waypoints enters any no-fly zone;
	** if a path enters more than one no-fly zones, it will only set the one entered first as the restricted area entered.
	*/
	while(cur_flp->next!=NULL)
	{
		cur_nfz=start_nfz;
		xa=cur_flp->x;
		ya=cur_flp->y;
		xb=cur_flp->next->x;
		yb=cur_flp->next->y;
		
		while(cur_nfz!=NULL)
		{
			xc=cur_nfz->x;
			yc=cur_nfz->y;
			r=cur_nfz->r;
			X=xa*xa-2*xa*xb+xb*xb+ya*ya-2*ya*yb+yb*yb;
			Y=2*xc*xb-2*xc*xa-2*yc*ya+2*yc*yb+2*xa*xb+2*ya*yb-2*xb*xb-2*yb*yb;
			Z=xc*xc-2*xc*xb+yc*yc-2*yc*yb-r*r+xb*xb+yb*yb;
			D=Y*Y-4*X*Z;
			if(D<0)
			{
				cur_nfz=cur_nfz->next;
				continue;
			}
			t1=(-Y-sqrt(D))/(2*X);
			t2=(-Y+sqrt(D))/(2*X);
			if((t1>0&&t1<1)||(t2>0&&t2<1))
			{
				px=xb+t1*(xa-xb);
				py=yb+t1*(ya-yb);
				if(error==NULL)
				{
					error=cur_nfz;
					distance=sqrt((px-xa)*(px-xa)+(py-ya)*(py-ya));
				}
				else
				{
					if(distance>sqrt((px-xa)*(px-xa)+(py-ya)*(py-ya)))
					{
						error=cur_nfz;
						distance=sqrt((px-xa)*(px-xa)+(py-ya)*(py-ya));
					}
				}
			}
			cur_nfz=cur_nfz->next;
			
		}
		if(error!=NULL)
		{
			*restricted_area_entered=error;
			return 0;
		}
		cur_flp=cur_flp->next;
	}
	
	return 1;
}


//This function is to free any memory allocated by malloc before exiting the program.
void before_exit(void)
{
	if(no__fly__zone!=NULL)
	{
		free_area(&no__fly__zone);
	}
	
	if(flight__plan!=NULL)
	{
		free_wpoint(&flight__plan);
	}
	
	if(new_val!=NULL)
	{
		free(new_val);
	}
	
}



