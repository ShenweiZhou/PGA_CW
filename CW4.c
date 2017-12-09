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

void print_area(Area* start)
{
	Area* cur=start;
	while(cur!=NULL)
	{
		printf("The no-fly zone is:\nX: %d; Y: %d; R: %d\n",cur->x,cur->y,cur->r);
		cur=cur->next;
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
	print_area(no_fly_zone);
	
}

Area* read_nofly(char* no_fly_path)
{
	FILE * nfz_text=fopen(no_fly_path,"r");
	if(nfz_text==NULL)
	{
		perror("Cannot open no-fly zone file.");
		exit(1);
	}
	
	Area* nfz_start=NULL;
	char* comment;
	comment=(char*)malloc(sizeof(char));
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
				exit(2);
			}
			if(fscanf(nfz_text,"%d %d",&val[1],&val[2])!=2)
			{
				fprintf(stderr,"No-fly zone file invalid.2\n");
				
				printf("%d %d\n",val[1],val[2]);
				exit(2);
			}
			printf("%d %d\n",val[1],val[2]);
			useless=getc(nfz_text);
			while(useless==32)
			{
				useless=getc(nfz_text);
			}
			if(useless==10||useless==EOF)
			{
				append_area(&nfz_start,val[0],val[1],val[2]);
			}
			else
			{
				fprintf(stderr,"No-fly zone file invalid.3\n");
				printf("%c\n",useless);
				exit(2);
			}
		}
		
		
		
	}while(useless==10);
	

	
	
	
	fclose(nfz_text);
	return nfz_start;
}

Wpoint* read_flightplan(char* flight_plan_path)
{
	
	FILE *flp_text=fopen(flight_plan_path,"r");
	if(flp_text==NULL)
	{
		perror("Cannot open flight plan file.");
		exit(1);
	}
	Wpoint* flp_start=NULL;
	
	
	fclose(flp_text);
	return flp_start;
}


