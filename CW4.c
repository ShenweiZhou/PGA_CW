//6521878 zy21878 Yuyang ZHOU 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
		exit(2);
	}
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
		exit(3);
	}
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
	char* comment;
	comment=(char*)malloc(16*sizeof(char));
	if(comment==NULL)
	{
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(5);
	}
	
	char useless;
	
	do
	{
		char* word=NULL;
		int val[3];
		
		if(fscanf(nfz_text,"%s",comment)<1)
		{
			useless=getc(nfz_text);
			printf("%s\n",comment);
			continue;
		}
		
		printf("%s\n",comment);
		if(*comment==35)
		{
			useless=getc(nfz_text);
			while(useless!=10)
			{
				if(useless==EOF)
				{
					printf("file ends successfully!\n");
					break;
				}
				useless=getc(nfz_text);
			}
		}
		else
		{
			val[0]=(int)strtol(comment,&word,10);
			if(*word!=0)
			{
				fprintf(stderr,"No-fly zone file invalid.1\n");
				printf("%s\n",word);
				printf("%d\n",val[0]);
				free(comment);
				return -1;
			}
			if(fscanf(nfz_text,"%d %d",&val[1],&val[2])!=2)
			{
				fprintf(stderr,"No-fly zone file invalid.2\n");
				
				printf("%d %d\n",val[1],val[2]);
				free(comment);
				return -1;
			}
			printf("%d %d\n",val[1],val[2]);
			useless=getc(nfz_text);
			while(useless==32)
			{
				useless=getc(nfz_text);
			}
			if(useless==10||useless==EOF)
			{
				append_area(no_fly_zone,val[0],val[1],val[2]);
			}
			else
			{
				fprintf(stderr,"No-fly zone file invalid.3\n");
				printf("%c\n",useless);
				free(comment);
				return -1;
			}
		}
		
		
		
	}while(useless==10);
	

	
	
	free(comment);
	fclose(nfz_text);
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
	char* comment;
	comment=(char*)malloc(16*sizeof(char));
	if(comment==NULL)
	{
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(5);
	}
	char useless;
	
	do
	{
		char* word=NULL;
		int val[2];
		
		if(fscanf(flp_text,"%s",comment)<1)
		{
			useless=getc(flp_text);
			printf("%s\n",comment);
			continue;
		}
		
		printf("%s\n",comment);
		if(*comment==35)
		{
			useless=getc(flp_text);
			while(useless!=10)
			{
				if(useless==EOF)
				{
					printf("file ends successfully!\n");
					break;
				}
				useless=getc(flp_text);
			}
		}
		else
		{
			val[0]=(int)strtol(comment,&word,10);
			if(*word!=0)
			{
				fprintf(stderr,"Flight plan file invalid.1\n");
				printf("%s\n",word);
				printf("%d\n",val[0]);
				free(comment);
				return -1;
			}
			if(fscanf(flp_text,"%d",&val[1])<1)
			{
				fprintf(stderr,"Flight plan file invalid.2\n");
				
				printf("%d\n",val[1]);
				free(comment);
				return -1;
			}
			printf("%d\n",val[1]);
			useless=getc(flp_text);
			while(useless==32)
			{
				useless=getc(flp_text);
			}
			if(useless==10||useless==EOF)
			{
				append_wpoint(flight_plan,val[0],val[1]);
			}
			else
			{
				fprintf(stderr,"Flight plan file invalid.3\n");
				printf("%c\n",useless);
				free(comment);
				return -1;
			}
		}
		
		
		
	}while(useless==10);
	
	
	
	
	free(comment);
	fclose(flp_text);
	return 0;
}

int check(Area* start_nfz, Wpoint* start_flp,Area** restricted_area_entered)
{
	int xa,ya,xb,yb,xc,yc,r;
	long int X,Y,Z,D;
	double t1,t2;
	Wpoint* cur_flp=start_flp;
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
				*restricted_area_entered=cur_nfz;
				return 0;
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
				*restricted_area_entered=cur_nfz;
				return 0;
			}
			cur_nfz=cur_nfz->next;
		}
		
		cur_flp=cur_flp->next;
	}
	
	
	
	return 1;
	
	
}
