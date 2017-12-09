//6521878 zy21878 Yuyang ZHOU 

#include <stdio.h>
#include <stdlib.h>

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

Area* read_nofly(char*);
Wpoint* read_flightplan(char*);
int check();

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


int main(int argc, char *argv[])
{
	Area* no_fly_zone=NULL;
	Wpoint* flight_plan=NULL;
	
	
	
	if(argc!=3)
	{
		fprintf(stderr,"Invalid command line arguments. Usage: <noflyzones> <flightplan>\n");
		exit(0);
	}
	no_fly_zone=read_nofly(argv[1]);
	flight_plan=read_flightplan(argv[2]);
	
	
}

Area* read_nofly(char* no_fly_path)
{
	FILE * nfz_text=fopen(no_fly_path,"r");
	if(nfz_text==NULL)
	{
		perror("Cannot open no-fly zone file.\n");
		exit(1);
	}
	
	Area* nfz_start=NULL;
	char* comment,*word=NULL;
	int val[3];
	char useless;
	do
	{
		while(fscanf(nfz_text,"%s",comment)==1)
		{
			if(*comment==35)
			{
				while(useless=getc(nfz_text)!=10)
				{
					if(useless==EOF)
					{
						break;
					}
				}
			}
			else
			{
				for(int i=0;i<3;i++)
				{
					val[i]=(int)strtol(comment,&word,10);
					if(word!=NULL||(i==3&&fscanf(nfz_text,"%s",comment)==1))
					{
						fprintf(stderr,"No-fly zone file invalid.\n");
						exit(2);
					}
				}
				append_area(&nfz_start,val[0],val[1],val[2]);
			}
		
		}
	}while(getc(nfz_text)!=EOF&&useless!=EOF);
	
	
	
	
	
	
	fclose(nfz_text);
	return nfz_start;
}

Wpoint* read_flightplan(char* flight_plan_path)
{
	
	FILE *flp_text=fopen(flight_plan_path,"r");
	if(flp_text==NULL)
	{
		perror("Cannot open flight plan file.\n");
		exit(1);
	}
	Wpoint* flp_start=NULL;
	
	
	fclose(flp_text);
	return flp_start;
}


