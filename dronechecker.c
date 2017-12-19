//6521878 zy21878 Yuyang ZHOU 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>




//define a linked list data structure including three integers x,y,r to store the no-fly areas read in
struct area{
	int x;
	int y;
	int r;
	struct area* next;
};
typedef struct area Area;

//define a linked list data structure including two integers x,y to store the flight plans read in
struct waypoint{
	int x;
	int y;
	struct waypoint* next;
};
typedef struct waypoint Wpoint;

//a function to append new elements to Area after the existing ones
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

//a function to append new elements to Waypoint after the existing ones
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

void print_area(Area* start)
{
	Area* cur=start;
	while(cur!=NULL)
	{
		printf("The no-fly zone is:\nX: %d; Y: %d; R: %d\n",cur->x,cur->y,cur->r);
		cur=cur->next;
	}
}

void print_wpoint(Wpoint* start)
{
	Wpoint* cur=start;
	while(cur!=NULL)
	{
		printf("The flight plan is:\nX: %d; Y: %d\n",cur->x,cur->y);
		cur=cur->next;
	}
}

//a function to validate the read in waypoints, if there is less than 2 waypoints or there are two same consecutive waypoints, return failure
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
				return -1;
			}
		}
		
	}
	if(i<2)
	{
		return -1;
	}
	return 1;
}

//a function to free all the space used by list Area
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

//a function to free all the space used by list Waypoint
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

//prototypes for functions to read two files and to validate the plan
int read_nofly(char*,Area**);
int read_flightplan(char*,Wpoint**);
int check(Area* , Wpoint*,Area**);

void before_exit(void);

//declaring a global variable to store integers read from both files for validating.
char* new_val=NULL;
//declaring two global variables to store the address of read in no-fly zones and flight plans.
Area* no__fly__zone=NULL;
Wpoint* flight__plan=NULL;

int main(int argc, char *argv[])
{

	Area* restricted_area_entered=NULL;
	
	atexit(before_exit);
	
	if(argc!=3)
	{
		fprintf(stderr,"Invalid command line arguments. Usage: <noflyzones> <flightplan>\n");
		exit(6);
	}
	
	if(read_nofly(argv[1],&no__fly__zone)==-1)
	{
		fprintf(stderr,"No-fly zone file invalid.\n");
		exit(2);
	}
	
	if(read_flightplan(argv[2],&flight__plan)==-1)
	{
		fprintf(stderr,"Flight plan file invalid.\n");
		exit(3);
	}

	//print_area(no__fly__zone);
	//print_wpoint(flight__plan);
	
	if(check(no__fly__zone,flight__plan,&restricted_area_entered)==0)
	{
		fprintf(stdout,"Invalid flight plan.\n"
		"Enters restricted area around %d, %d.\n",restricted_area_entered->x,restricted_area_entered->y);
		exit(4);
	}

	fprintf(stdout,"Flight plan valid.\n");
	exit(0);
}



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
					//fprintf(stderr,"No-fly zone file invalid.\n"/*"No enough parameters!\n"*/);
					return -1;
				}
				
				while(isspace(text)==0&&j<15)
				{
					*(new_val+j)=text;
					text=getc(nfz_text);
					j++;
				}
				*(new_val+j)=0;
				
				
				val[i]=(int)strtol(new_val,&word,10);
				if(*word!=0)
				{
					//fprintf(stderr,"No-fly zone file invalid.\n"/*"Inproper input type!\n"*/);
					return -1;
				}
			}
			if(text==10||text==EOF)
			{
				if(val[0]>=0&&val[0]<10000&&val[1]>=0&&val[1]<10000&&val[2]>0&&val[2]<10000)
				{
					append_area(no_fly_zone,val[0],val[1],val[2]);
				}
				else
				{
					//fprintf(stderr,"No-fly zone file invalid.\n"/*"Inproper values!\n"*/);
					return -1;
				}
			}
			else
			{
				//fprintf(stderr,"No-fly zone file invalid.\n"/*"Inproper ending!\n"*/);
				return -1;
			}
			
			
		}
		else
		{
				//fprintf(stderr,"No-fly zone file invalid.\n"/*"Inproper starting!\n"*/);
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
	return 0;
}

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
					//fprintf(stderr,"Flight plan file invalid.\n"/*"No enough values!\n"*/);
					return -1;
				}
				
				while(!isspace(text)&&j<15)
				{
					*(new_val+j)=text;
					text=getc(flp_text);
					j++;
				}
				
				*(new_val+j)=0;
				
				val[i]=(int)strtol(new_val,&word,10);
				if(*word!=0)
				{
					//fprintf(stderr,"Flight plan file invalid.\n"/*"Wrong type!\n"*/);
					/*printf("%s\n",word);
					printf("%d\n",val[i]);
					*/
					return -1;
				}
			}
			if(text==10||text==EOF)
			{
				if(val[0]>=0&&val[0]<10000&&val[1]>=0&&val[1]<10000)
				{
					append_wpoint(flight_plan,val[0],val[1]);
				}
				else
				{
					//fprintf(stderr,"Flight plan file invalid.\n"/*"Inproper values!\n"*/);
					return -1;
				}
			}
			else
			{
				//fprintf(stderr,"Flight plan file invalid.\n"/*"Inproper ending!\n"*/);
				return -1;
			}
		}
		else
		{
				//fprintf(stderr,"Flight plan file invalid.\n"/*"Inproper starting!\n"*/);
				return -1;
		}
		
		
		
	}while(text==10);
	
	
	if(fclose(flp_text)!=0)
	{
		fprintf(stderr,"Failed to close flight plan file.\n");
		exit(6);
	}
	if(check_wpoint(*flight_plan)<0)
	{
		//fprintf(stderr,"Flight plan file invalid.\n"/*"Waypoints errors!\n*/");
		return -1;
	}
	
	return 0;
}

int check(Area* start_nfz, Wpoint* start_flp,Area** restricted_area_entered)
{
	int xa,ya,xb,yb,xc,yc,r;
	long int X,Y,Z,D;
	double t1,t2,distance,px,py;
	Wpoint* cur_flp=start_flp;
	Area* error;
	error=NULL;
	Area* cur_nfz=start_nfz;
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
					//printf("distance: %f\n",distance);
				}
				else
				{
					if(distance>sqrt((px-xa)*(px-xa)+(py-ya)*(py-ya)))
					{
						error=cur_nfz;
						distance=sqrt((px-xa)*(px-xa)+(py-ya)*(py-ya));
						//printf("distance: %f\n",distance);
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
	//printf("exit program now!\n");
}



