#include<cstdio>
#include<cstdlib>
#include<cstring>
#define ll long long
using namespace std;
FILE *fp,*fpout;
struct ind{
	int L,R;
}ind0[255];
ll w;
int fc;
unsigned char cache=0;
int cachep=0;
int point;
char FileName[256];
ll binin(int len){
	ll x=0;
	if(cachep==0){
		cache=fgetc(fp);
		cachep=8;
	}
	for(int i=1;i<=len;i++){
		x=(x<<1)+((cache>>7)&1);
		cachep--;
		cache<<=1;
		if(cachep==0){
			cache=fgetc(fp);
			cachep=8;
		}
	}
	return x;
}
int main(int argc, char *argv[]){
	if(argc<2){
		printf("No file\n");
		system("pause");
		return 0;
	}
	if(!(fp=fopen(argv[1],"rb"))){
		printf("Failed to open Pak file\n");
		fclose(fp);
		system("pause");
		return 0;
	}
	for(int i=0;i<255;i++){
		ind0[i].L=binin(9);
		ind0[i].R=binin(9);
	}
	fc=binin(32);
	for(int Fi=0;Fi<fc;Fi++){
		int len=binin(8);
		for(int i=0;i<len;i++)*(FileName+i)=binin(8);
		*(FileName+len)='\0';
		if((fpout=fopen(FileName,"wb"))){
			double pct=0;
			w=binin(64);
			printf("Unpacking File %s (%d/%d)\n",FileName,Fi+1,fc);
			point=510;
			for(ll p=0;p<w;p++){
				if(int(pct*100)!=int(double(p+1)/double(w)*100)){
					printf("\r%3.0f%%",double(p+1)/double(w)*100);
					pct=double(p+1)/double(w);
				}
				while(point>=256)if(binin(1)==0)point=ind0[point-256].L;else point=ind0[point-256].R;
				fputc((unsigned char)point,fpout);
				point=510;
			}
			fclose(fpout);
			printf("\rDone!    \n");
		}else{
			printf("Failed to open file : %s\n",FileName);
			fclose(fp);
			fclose(fpout);
			system("pause");
			return 0;
		}
		fclose(fpout);
	}
	
	fclose(fp);
	system("pause");
	return 0;
}
