//6521878 zy21878 Yuyang ZHOU 
//radius should be greater than 0; no two consecutive way-points should have the same coordinates.
// Each integer will be greater than or equal to zero and less than 10,000.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>


struct area{
	int x;
	int y;
	int r;
	struct area* next;
};
typedef struct area Area;

struct waypoint{
	int x;
	int y;
	struct waypoint* next;
};
typedef struct waypoint Wpoint;

int read_nofly(char*,Area**);
int read_flightplan(char*,Wpoint**);
int check(Area* , Wpoint*,Area**);

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

//Declare a global variable to store texts read from both files.
char* comment;

int main(int argc, char *argv[])
{
	Area* no_fly_zone=NULL;
	Wpoint* flight_plan=NULL;
	Area* restricted_area_entered=NULL;
	
	
	
	if(argc!=3)
	{
		fprintf(stderr,"Invalid command line arguments. Usage: <noflyzones> <flightplan>\n");
		return -1;
	}
	
	if(read_nofly(argv[1],&no_fly_zone)==-1)
	{
		//fprintf(stderr,"No-fly zone file invalid.\n");
		if(no_fly_zone!=NULL)
		{
			free_area(&no_fly_zone);
		}
		free(comment);
		exit(2);
	}
	printf("comment%p\n%s\n", comment,comment);
	
	if(read_flightplan(argv[2],&flight_plan)==-1)
	{
		//fprintf(stderr,"Flight plan file invalid.1\n");
		if(no_fly_zone!=NULL)
		{
			free_area(&no_fly_zone);
		}
		if(flight_plan!=NULL)
		{
			free_wpoint(&flight_plan);
		}
		free(comment);
		exit(3);
	}

	printf("comment%p\n%s\n", comment,comment);
	free(comment);
	printf("after free comment%p\n%s\n",comment,comment);

	print_area(no_fly_zone);
	print_wpoint(flight_plan);
	
	if(check(no_fly_zone,flight_plan,&restricted_area_entered)==0)
	{
		fprintf(stdout,"Invalid flight plan.\n"
		"Enters restricted area around %d, %d.\n",restricted_area_entered->x,restricted_area_entered->y);
		free_area(&no_fly_zone);
		free_wpoint(&flight_plan);
		exit(4);
	}
	free_area(&no_fly_zone);
	free_wpoint(&flight_plan);
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
	
	comment=(char*)malloc(6*sizeof(char));
	if(comment==NULL)
	{
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(5);
	}
	
	char useless;
	
	do
	{
		char* word=NULL;
		useless=getc(nfz_text);
		printf("%c\n",useless);
		if(useless==35||useless==10||useless==EOF)
		{
			
			while(useless!=10)
			{
				if(useless==EOF)
				{
					break;
				}
				useless=getc(nfz_text);
			}
		}
		else if(useless>47&&useless<58)
		{
			int val[3];
			for(int i=0;i<3;i++)
			{
				
				int j=0;
				while(i>0&&useless!=10&&isspace(useless))
				{
					useless=getc(nfz_text);
				}
				
				if(useless=='\n')
				{
					fprintf(stderr,"No-fly zone file invalid.\n""No enough parameters!\n");
					printf("%c\nj:%d\ni:%d\n",useless,j,i);
					return -1;
				}
				
				while(isspace(useless)==0&&j<15)
				{
					*(comment+j)=useless;
					useless=getc(nfz_text);
					j++;
				}
				*(comment+j)=0;
				
				
				val[i]=(int)strtol(comment,&word,10);
				printf("%d\n",val[i]);
				if(*word!=0)
				{
					fprintf(stderr,"No-fly zone file invalid.\n""Inproper input type!\n");
					printf("%s\n",word);
					printf("%d\n",val[i]);
					return -1;
				}
			}
			if(useless==10||useless==EOF)
			{
				if(val[0]>=0&&val[0]<10000&&val[1]>=0&&val[1]<10000&&val[2]>0&&val[2]<10000)
				{
					append_area(no_fly_zone,val[0],val[1],val[2]);
				}
				else
				{
					fprintf(stderr,"No-fly zone file invalid.\n""Inproper values!\n");
					printf("%c\n",useless);
					return -1;
				}
			}
			else
			{
				fprintf(stderr,"No-fly zone file invalid.\n""Inproper ending!\n");
				printf("%c\n",useless);
				
				return -1;
			}
			
			
		}
		else
		{
				fprintf(stderr,"No-fly zone file invalid.\n""Inproper starting!\n");
				printf("%c\n",useless);
				
				return -1;
		}
		
		
		
	}while(useless==10);
	
	
	if(fclose(nfz_text)!=0)
	{
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
		perror("Cannot open flight plan file.");
		exit(1);
	}
	
	* flight_plan=NULL;
	
	if(comment==NULL)
	{
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(5);
	}
	char useless;
	
	do
	{
		char* word=NULL;
		
		
		useless=getc(flp_text);
		printf("%c\n",useless);
		if(useless==35||useless==10||useless==EOF)
		{
			
			while(useless!=10)
			{
				if(useless==EOF)
				{
					break;
				}
				useless=getc(flp_text);
			}
		}
		else if(useless>47&&useless<58)
		{
			int val[2];
			for(int i=0;i<2;i++)
			{
				
				int j=0;
				while(i>0&&isspace(useless)&&useless!=10)
				{
					useless=getc(flp_text);
				}
				
				if(useless=='\n')
				{
					fprintf(stderr,"Flight plan file invalid.\n""No enough value!\n");
					printf("%c\nj:%d\ni:%d\n",useless,j,i);
					
					return -1;
				}
				
				while(!isspace(useless)&&j<15)
				{
					*(comment+j)=useless;
					useless=getc(flp_text);
					j++;
				}
				
				*(comment+j)=0;
				
				val[i]=(int)strtol(comment,&word,10);
				printf("%d\n",val[i]);
				if(*word!=0)
				{
					fprintf(stderr,"Flight plan file invalid.\n""Wrong type!\n");
					printf("%s\n",word);
					printf("%d\n",val[i]);
					
					return -1;
				}
			}
			if(useless==10||useless==EOF)
			{
				if(val[0]>=0&&val[0]<10000&&val[1]>=0&&val[1]<10000)
				{
					append_wpoint(flight_plan,val[0],val[1]);
				}
				else
				{
					fprintf(stderr,"Flight plan file invalid.\n""Inproper values!\n");
					printf("%c\n",useless);
					return -1;
				}
			}
			else
			{
				fprintf(stderr,"Flight plan file invalid.\n""Inproper ending!\n");
				printf("%c\n",useless);
				
				return -1;
			}
		}
		else
		{
				fprintf(stderr,"Flight plan file invalid.\n""Inproper starting!\n");
				printf("%c\n",useless);
				
				return -1;
		}
		
		
		
	}while(useless==10);
	
	
	if(fclose(flp_text)!=0)
	{
		fprintf(stderr,"Failed to close flight plan file.\n");
		exit(6);
	}
	if(check_wpoint(*flight_plan)<0)
	{
		fprintf(stderr,"Flight plan file invalid.\n""Waypoints errors!\n");
		return -1;
	}
	
	return 0;
}

int check(Area* start_nfz, Wpoint* start_flp,Area** restricted_area_entered)
{
	int xa,ya,xb,yb,xc,yc,r;
	long int X,Y,Z,D;
	double t1,t2;
	Wpoint* cur_flp=start_flp;
	Wpoint* error;
	error=NULL;
	
	while(cur_flp->next!=NULL)
	{
		Area* cur_nfz=start_nfz;
		while(cur_nfz!=NULL)
		{
			xa=cur_flp->x;
			ya=cur_flp->y;
			xb=cur_flp->next->x;
			yb=cur_flp->next->y;
			xc=cur_nfz->x;
			yc=cur_nfz->y;
			r=cur_nfz->r;
			if((xa-xc)*(xa-xc)+(ya-yc)*(ya-yc)<r*r||(xb-xc)*(xb-xc)+(yb-yc)*(yb-yc)<r*r)
			{
				if(error==NULL)
				{
					error=cur_nfz;
					
				}
				else
				{
					;
				}
				
			}
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
				if(error==NULL)
				{
					error=cur_nfz;
					
				}
				else
				{
					;
				}
			}
			cur_nfz=cur_nfz->next;
			if(error!=NULL)
			{
				*restricted_area_entered=error;
				return 0;
			}
		}
		
		cur_flp=cur_flp->next;
	}
	
	
	
	return 1;
	
	
}
