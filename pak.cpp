#include<cstdio>
#include<cstdlib>
#include<cstring>
#define ll long long
using namespace std;
FILE *fp,*fpout;
struct pt{
	int index;
	ll w;
}point[511],heap[2048];
struct ind{
	int L,R;
}ind0[255];
struct path{
	bool LR[260];
	int len;
}ph[256],tph;
int top,t=256;
ll w;
char FileName[256];
unsigned char cache=0;
int cachep=0;
void up(const int &st){
	int i=st,j=i/2;
	heap[0]=heap[i];
	while(j>0){
		if(heap[0].w>=heap[j].w)break;
		heap[i]=heap[j];
		i=j;j=i/2;
	}
	heap[i]=heap[0];
}
void down(const int &st){
	int i=st,j=i*2;
	heap[0]=heap[i];
	while(j<=top){
		if(j<top&&heap[j].w>heap[j+1].w)j++;
		if(heap[0].w<=heap[j].w)break;
		heap[i]=heap[j];
		i=j;j=i*2;
	}
	heap[i]=heap[0];
}
void ins(const pt &a){
	top++;
	heap[top]=a;
	up(top);
}
void pop(){
	heap[1]=heap[top];
	top--;
	down(1);
}
void DFS(int depth,path tph,int point){
	if(point<256){
		ph[point]=tph;return;
	}
	tph.len++;
	tph.LR[depth]=0;
	DFS(depth+1,tph,ind0[point-256].L);
	tph.LR[depth]=1;
	DFS(depth+1,tph,ind0[point-256].R);
}
void binput(ll x,int len){
	for(int i=1;i<=len;i++){
		cache=(cache<<1)+((x>>(len-i))&1);
		cachep++;
		if(cachep==8){
			cachep=0;
			fputc(cache,fpout);
		}
	}
}
void cleanput(){
	if(cachep==0)return;
	cache=cache<<(8-cachep);
	fputc(cache,fpout);
}
void getFileName(char* Dest,char* Source){
	int len=strlen(Source);
	int i=len;
	strcpy(Dest,"");
	while(i>0&&Source[i-1]!='/'&&Source[i-1]!='\\')i--;
	strncpy(Dest,Source+i,len-i);
	*(Dest+len-i)='\0';
}
int main(int argc, char *argv[]){
	if(argc<2){
		printf("No file\n");
		system("pause");
		return 0;
	}
	if(!(fpout=fopen("PACK.pak","wb"))){
		printf("Failed to open file : PACK.pak\n");
		fclose(fp);
		fclose(fpout);
		system("pause");
		return 0;
	}
	for(int i=0;i<511;i++)point[i].index=i;
	printf("Counting Files Bytes (%d Files)\n",argc-1);
	unsigned char ch;
	for(int Fi=1;Fi<argc;Fi++){
		getFileName(FileName,argv[Fi]);
		if((fp=fopen(argv[Fi],"rb"))){
			ch=fgetc(fp);
			while(!feof(fp)){
				w++;
				point[ch].w++;
				ch=fgetc(fp);
			}
		}else{
			printf("Failed to open file : %s\n",FileName);
			fclose(fp);
			fclose(fpout);
			system("pause");
			return 0;
		}
		fclose(fp);
	}
	for(int i=0;i<256;i++)ins(point[i]);
	while(t<511){
		pt a=heap[1];pop();
		ind0[t-256].L=a.index;
		point[t].w=a.w;
		a=heap[1];pop();
		ind0[t-256].R=a.index;
		point[t].w+=a.w;
		ins(point[t]);
		t++;
	}
	tph.len=0;
	DFS(0,tph,510);
	for(int i=0;i<255;i++){
		binput(ind0[i].L,9);
		binput(ind0[i].R,9);
	}
	binput(argc-1,32);
	for(int Fi=1;Fi<argc;Fi++){
		getFileName(FileName,argv[Fi]);
		if((fp=fopen(argv[Fi],"rb"))){
			ll wi=0;
			double pct=0;
			printf("Packing File %s (%d/%d)\n",FileName,Fi,argc-1);
			printf("Counting");
			ch=fgetc(fp);
			while(!feof(fp)){
				wi++;
				ch=fgetc(fp);
			}
			int len=strlen(FileName);
			binput(len,8);
			for(int i=0;i<len;i++)binput(FileName[i],8);
			binput(wi,64);
			clearerr(fp);
			fseek(fp,0,SEEK_SET);
			for(ll p=0;p<wi;p++){
				if(int(pct*100)!=int(double(p+1)/double(wi)*100)){
					printf("\rPacking:%3.0f%%",double(p+1)/double(wi)*100);
					pct=double(p+1)/double(wi);
				}
				ch=fgetc(fp);
				for(int i=0;i<ph[ch].len;i++){
					binput(ph[ch].LR[i]?1:0,1);
				}
			}
			fclose(fp);
			printf("\rDone!            \n");
		}else{
			printf("Failed to open file : %s\n",FileName);
			fclose(fp);
			fclose(fpout);
			system("pause");
			return 0;
		}
		fclose(fp);
	}
	cleanput();
	fclose(fpout);
	system("pause");
	return 0;
}
