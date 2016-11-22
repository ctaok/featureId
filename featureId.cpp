#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>                                                                                                                                                                      
#include <string.h>
#include <string>
#include <unistd.h>
#include <math.h>
#include <map>
#include <hash_map>
#include <unordered_map>
using namespace std;
using namespace __gnu_cxx;

//#define MAP map<string, int>
#define MAP unordered_map<string, int>

int _featIdCnt = 0;
const int _strLength = 4096000;
MAP hm_feat;
char *str_feat = new char[_strLength];

int loadMap(char *filePath)
{
	FILE *fp = fopen(filePath, "rt");
	if (fp == NULL) {
		printf("Cannot open file to read!\n");
		return -1;
	}
	printf("%s\n", filePath);
	while (!feof(fp)) {
		char tmpStr[1024] = {'\0'};
		fgets(tmpStr, 1024, fp);
		for (int j=0; j<1024; j++) {
			if (tmpStr[j] == '\n' || tmpStr[j] == '\0') {
				tmpStr[j] = '\0';
				break;
			}
		}
		char *leftStr_c = strtok(tmpStr, "\t");
		if (leftStr_c == NULL)
			continue;
		char *rightStr_c = strtok(NULL, "\t");
		if (rightStr_c == NULL)
			continue;
		string leftStr_s = leftStr_c;
		int rightStr_i = atoi(rightStr_c);
		hm_feat[leftStr_s] = rightStr_i;
		_featIdCnt++;
	}
	fclose(fp);
	return 0;
}

int saveMap(char *filePath)
{
	ofstream outfile;
	outfile.open(filePath, ios::app);
	MAP::iterator k;
	for (k=hm_feat.begin(); k!=hm_feat.end(); k++) {
		outfile<<(*k).first<<"\t"<<(*k).second<<endl;
	}
	outfile.close();
	return 0;
}

int getFeatId(char *feat_s)
{
	string str_mem = feat_s;
	MAP::iterator k;
	k = hm_feat.find(str_mem);
	if (k != hm_feat.end()) {
		return (*k).second;
	} else {
		_featIdCnt++;
		hm_feat[str_mem] = _featIdCnt;
		return _featIdCnt;
	}
}

int analyseSent(char *filepath, char *featdir)
{
	int sentnum = 0;
	char featpath[1024] = {'\0'};
	char *filename = strrchr(filepath, '/') + 1;
	sprintf(featpath, "%s/%s_0", featdir, filename);
	FILE *fp = fopen(filepath, "rt");
	if (fp == NULL) {
		printf("Cannot open file to read!\n");
	}
	ofstream outfile;
	outfile.open(featpath, ios::app);
	while (!feof(fp)) {
		sentnum++;
		memset(str_feat, '\0', _strLength);
		fgets(str_feat, _strLength, fp);
		for (int j=0; j<_strLength; j++) {
			if (str_feat[j] == '\n') {
				str_feat[j] = '\0';
				break;
			}
			if (str_feat[j] == '\0')
				break;
		}

		char *str = strtok(str_feat, "\t");
		if (str==NULL)
			continue;
		str = strtok(NULL, "\t");
		if (str==NULL)
			continue;
		str = strtok(NULL, "\t");
		if (str==NULL)
			continue;
		//outfile<<str[0];
		outfile<<str;
		str = strtok(NULL, "\t");
		if (str==NULL)
			continue;

		char *tmpStr = NULL;
		while (1) {
			if (tmpStr == NULL)
				tmpStr = strtok(str, " ");
			else
				tmpStr = strtok(NULL, " ");
			if (tmpStr==NULL)
				break;
			int id = getFeatId(tmpStr);
			outfile<<" "<<id<<":1";
		}
		outfile<<endl;
	}
	outfile.close();
	fclose(fp);
	return sentnum;
}

int main(int argc, char** argv)
{
	if (argc != 5 && argc != 4) {
		printf("Usage: datalist idpath (mapsrc) mapdst!\n");
		return -1;
	}
	if (argc == 5) {
		if (loadMap(argv[3]) == -1) {
			return -1;
		}
	}
	int lineCnt = 1;
	char *dataList = argv[1];
	char *idName = argv[2];
	char *mapName = argv[argc-1];
	FILE *fp = fopen(dataList, "rt");
	if (fp == NULL) {
		printf("Cannot open list to read!\n");
	}
	while (!feof(fp)) {
		char featName[1024] = {'\0'};
		fgets(featName, 1024, fp);
		for (int j=0; j<1024; j++) {
			if (featName[j] == '\n' || featName[j] == '\0') {
				featName[j] = '\0';
				break;
			}
		}
		if (featName[0] == '\0')
			continue;
		int sentNum = 0;
		sentNum = analyseSent(featName, idName);
	}
	saveMap(mapName);
	fclose(fp);
	delete str_feat;
	return 0;
}

