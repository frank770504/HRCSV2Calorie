#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vector>

#define STRING_SPEED		"Speed"
#define STRING_AGE		"Age"
#define STRING_WEIGHT		"Weight"
#define STRING_GENDER		"Gender"
#define STRING_FEMALE		"Female"
#define STRING_MALE		"Male"
#define STRING_HEART_RATE	"Heart rate"
#define STRING_ACC		"acc"

using namespace std;

enum _column_2
{
	Sp,
	Ag,
	We,
	Ge,
	He,
	ac
} column_2;

typedef struct _HeartRate
{
	long timestamp;
	int rate;
	int timediff;
} HeartRate;

typedef struct _Init
{
	int age;
	int weight;
	float location[2];
	char gender[10];
} InitData;

typedef struct _Speed
{
	long timestamp;
	float speed;
} Speed;

const char* getfield(char* line, int num, const char* knife)
{
	const char* tok;
	char sp[10];
	sprintf(sp, "%s\n", knife);
	for (tok = strtok(line, knife);
			tok && *tok;
			tok = strtok(NULL, sp))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}

int get_string_number(char* str)
{
	if (strcmp(str,STRING_SPEED) == 0)
		return Sp;
	else if(strcmp(str,STRING_AGE) == 0)
		return Ag;
	else if(strcmp(str,STRING_WEIGHT) == 0)
		return We;
	else if(strcmp(str,STRING_GENDER) == 0)
		return Ge;
	else if(strcmp(str,STRING_HEART_RATE) == 0)
		return He;
	else if(strcmp(str,STRING_ACC) == 0)
		return ac;
	else
		return -1;
}

InitData initdata;
vector <HeartRate> hr_cal;

float do_cal(vector <HeartRate> hr, InitData id)
{
	float time, cal_sum = 0;
	for (int i=0;i<(int)hr.size();i++)
	{
		time = ((float)hr[i].timediff)/60000;
		if (strcmp(id.gender, STRING_MALE)==0)
			cal_sum += ((-55.0969 +(0.6309*hr[i].rate)+(0.1988*id.weight)+(0.2017*id.age))/4.184)*time;
		else if (strcmp(id.gender, STRING_FEMALE)==0)
			cal_sum += ((-20.4022 +(0.4472*hr[i].rate)+(0.1263*id.weight)+(0.074*id.age))/4.184)*time;
		else
			return -1.414;//never here
	}
	return cal_sum;
}


int main()
{
	FILE *rdf;
	char input[80], filename[100];
	printf("please input csv file name\n >>");
	scanf("%s", &input);
	sprintf(filename, "./%s", input);
	rdf = fopen(filename, "r");
	char line[1024];
	HeartRate hr_tmp;
	initdata.location[0] = 0;
	initdata.location[1] = 0;
	while (fgets(line, 1024, rdf))
	{
		char* tmp2 = strdup(line);
		char* tmp3 = strdup(line);
		char* tmp5 = strdup(line);
		char* tmp6 = strdup(line);
		char tmp_2[15], tmp_3[15], tmp_4[20], tmp_5[15];
		sprintf(tmp_2, "%s", getfield(tmp2, 2, ";"));
		switch (get_string_number(tmp_2))
		{
			case Sp:
			//sprintf(tmp_3, "%s", getfield(tmp3, 3, ";"));
			//printf("sp %s: %s\n", tmp_2, tmp_3);
				break;
			case He:
			sprintf(tmp_3, "%s", getfield(tmp3, 3, ";"));
			sprintf(tmp_4, "%s", getfield(tmp5, 1, ";"));
			hr_tmp.timestamp = atol(tmp_4);
			hr_tmp.rate = atoi(tmp_3);
			if (hr_cal.size()==0)
				hr_tmp.timediff = 0;
			else
				hr_tmp.timediff = hr_tmp.timestamp - hr_cal.back().timestamp;
			hr_cal.push_back(hr_tmp);
			//printf("he %ld: %d; %d\n", hr_cal.back().timestamp, hr_cal.back().timediff, hr_cal.back().rate);
				break;
			default:
			sprintf(tmp_3, "%s", getfield(tmp_2, 1, ":"));
			switch (get_string_number(tmp_3))
			{
				case Ag:
				sprintf(tmp_3, "%s", getfield(tmp5, 2, " "));
				initdata.age = atoi(tmp_3);
				//printf("ag : %d\n", initdata.age);
					break;
				case We:
				sprintf(tmp_3, "%s", getfield(tmp5, 2, " "));
				initdata.weight = atoi(tmp_3);
				//printf("we : %d\n", initdata.weight);
					break;
				case Ge:
				sprintf(initdata.gender, "%s", getfield(tmp5, 2, " "));
				//printf("ge : %s\n", initdata.gender);
					break;
				case ac:
				sprintf(tmp_4, "%s", getfield(tmp5, 4, ";"));
				sprintf(tmp_5, "%s", getfield(tmp6, 5, ";"));
				if (initdata.location[0]==0 && initdata.location[1]==0)
				{
					initdata.location[0] = atof(tmp_4);
					initdata.location[1] = atof(tmp_5);
				}
				//printf("Location %f %f\n", initdata.location[0], initdata.location[1]);
					break;
			}
				break;
		}
		// NOTE strtok clobbers tmp
		free(tmp2);
		free(tmp3);
		free(tmp5);
		free(tmp6);
	}
	float cal = 0;
	cal = do_cal(hr_cal, initdata);
	printf("Location : %f %f\n", initdata.location[0], initdata.location[1]);
	printf("Weight   : %d\n", initdata.weight);
	printf("Gender   : %s\n", initdata.gender);
	printf("Age      : %d\n", initdata.age);
	printf("cal      : %f\n", cal);
	hr_cal.clear();
	fclose(rdf);
	return 0;
}
