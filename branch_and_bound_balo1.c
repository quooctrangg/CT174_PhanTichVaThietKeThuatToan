#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char TenDV[20];
	float TL, GT, DG;
	int PA;
} DoVat;

DoVat *ReadFromFile(float *W, int *n){
	FILE *f;
	f = fopen("Branch_and_Bound_balo1.txt", "r");
	fscanf(f, "%f", W); 
	DoVat *dsdv;
	dsdv = (DoVat*)malloc( sizeof(DoVat));
	int i = 0;
	while(!feof(f)){
		fscanf(f, "%f%f%[^\n]",&dsdv[i].TL, &dsdv[i].GT, &dsdv[i].TenDV);
		dsdv[i].DG = dsdv[i].GT / dsdv[i].TL;
		dsdv[i].PA = 0;
		i++;
		dsdv = (DoVat*)realloc(dsdv, sizeof(DoVat) * (i + 1));  
	 }
	*n = i;
    fclose(f);
    return dsdv;
}

void swap(DoVat *x, DoVat *y){
	DoVat Temp;
	Temp = *x;
	*x = *y;
	*y = Temp;
}

void BubbleSort(DoVat *dsdv, int n){
	int i, j;
	for(i = 0; i <= n - 2; i++)
	   	for (j = n - 1; j >= i + 1; j--){
	   	 	if (dsdv[j].DG > dsdv[j - 1].DG) 
				swap(&dsdv[j], &dsdv[j - 1]);
	  	}   
}

void InDSDV(DoVat *dsdv ,int n, float W){
	int i;
	float TongTL = 0.0, TongGT = 0.0;
	printf("\nPhuong an Cai Ba lo 1 dung thuat toan NHANH CAN nhu sau:\n");
	printf("Trong luong cua ba lo = %-9.2f\n",W);
	printf("|---|--------------------|---------|---------|---------|-----------|\n");
	printf("|STT|     Ten Do Vat     |T. Luong | Gia Tri | Don gia | Phuong an |\n");
	printf("|---|--------------------|---------|---------|---------|-----------|\n");
	for(i = 0; i < n; i++){
		printf("|%2d |%-20s|%9.2f|%9.2f|%9.2f|%6d     |\n", i + 1, dsdv[i].TenDV, dsdv[i].TL, dsdv[i].GT, dsdv[i].DG, dsdv[i].PA);
		TongTL = TongTL + dsdv[i].PA * dsdv[i].TL;
		TongGT = TongGT + dsdv[i].PA * dsdv[i].GT;
	}	
	printf("|---|--------------------|---------|---------|---------|-----------|\n");	
	printf("Phuong an (theo thu tu DG giam dan): X(");
	for(i = 0; i < n - 1; i++){
		printf(" %d,", dsdv[i].PA);
	}	
	printf(" %d)\n", dsdv[n - 1].PA);
	printf("Tong trong luong = %-9.2f\n", TongTL);
	printf("Tong gia tri = %-9.2f\n", TongGT);
}

void Tao_Nut_Goc(float W, float *V, float *CT, float *GLNTT, float *TGT, float DG_Max){
	*TGT = 0.0;
	*V = W;
	*CT = *V * DG_Max; 
	*GLNTT = 0.0;
} 

void Cap_Nhat_GLNTT(float TGT, float *GLNTT, int x[], DoVat *dsdv, int n){
	int i;
	if(*GLNTT < TGT){
		 *GLNTT = TGT;
		 for(i = 0; i < n; i++)
		 	dsdv[i].PA = x[i];
	}
}

void Nhanh_Can(int i, float *TGT, float *CT, float *V, float *GLNTT, int x[], DoVat *dsdv, int n){
	int j; 
	int yk = *V / dsdv[i].TL;	
	for(j = yk; j >= 0; j--){
		*TGT = *TGT + j*dsdv[i].GT;
		*V = *V - j * dsdv[i].TL;
		*CT = *TGT + *V * dsdv[i + 1].DG;
		if(*CT > *GLNTT){ 					
			x[i] = j;
		 	if((i == n-1) || (*V == 0)) 
		 		Cap_Nhat_GLNTT(*TGT, GLNTT, x, dsdv, n);
		 	else
		 		Nhanh_Can(i + 1, TGT, CT, V, GLNTT, x, dsdv, n);
	 	}
		x[i] = 0;
		*TGT = *TGT - j * dsdv[i].GT;	
		*V = *V + j * dsdv[i].TL;	
	}
}
	
int main(){
	DoVat * dsdv;
	int n; 		
	float W; 		
	
	float CT; 	
	float TGT; 	
	float V; 		
	float GLNTT; 	
	
	dsdv = ReadFromFile(&W, &n);
	
	int x[n];		
	
	BubbleSort(dsdv,n);
	Tao_Nut_Goc(W, &V, &CT, &GLNTT, &TGT, dsdv[0].DG);
	Nhanh_Can(0, &TGT, &CT, &V, &GLNTT, x, dsdv, n);
	InDSDV(dsdv, n, W);
	
	free(dsdv);
	return 0;
}
