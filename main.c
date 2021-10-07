#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

typedef struct tarih{
	int gun;
	int ay;
	int yil;
	int saat;
	int dakika;
	int saniye;
}Tarih;

typedef struct belge{
	int dosyaNo;
	int belgeNo;
	char dosyaAdi[50];
	char belgeAdi[70];
	char dosyaTuru[20];
	char belgeTuru[20];
	int dosyaSayisi;
	int belgeSayisi;
	int dosyaBoyut;
	int belgeBoyut;
	Tarih dosyaKayitTarihi;
	Tarih belgeKayitTarihi;
}Belge;
Belge belgeler[500];
void tarihYazdir(Tarih tarih){
	printf("%d/%d/%d %d:%d:%d",tarih.gun,tarih.ay,tarih.yil,tarih.saat,tarih.dakika,tarih.saniye);
}
void tumunulistele(){
	int i=0;
	printf("%-10s %-50s %-10s %-20s %-20s %-20s\n","Belge No","Belge Adi","Turu","Boyut (bytes)","Dosya Sayisi","Tarih");
	while(1){
		if(belgeler[i].belgeNo!=-1){
			printf("%-10d %-50s %-10s %-20d %-20d",belgeler[i].belgeNo,belgeler[i].belgeAdi,belgeler[i].belgeTuru,belgeler[i].belgeBoyut,0);	
			tarihYazdir(belgeler[i].belgeKayitTarihi);
		}
		else{
			printf("%-10d %-50s %-10s %-20d %-20d",belgeler[i].dosyaNo,belgeler[i].dosyaAdi,belgeler[i].dosyaTuru,belgeler[i].dosyaBoyut,belgeler[i].dosyaSayisi);	
			tarihYazdir(belgeler[i].belgeKayitTarihi);
		}
		printf("\n");
	    if(belgeler[i].belgeNo==-2||belgeler[i].dosyaNo==-2){
			break;
		}
		i++;
	}
}
char* konumsec(){
	char lokasyon[50];
	int secim;
	while(1){
		DIR *d;
	    struct dirent *dir;
	    int i=1;
	    d = opendir(".");
	    if (d)
	    {
	    	long int toplam=0;
	    	int adet=0;
	        while ((dir = readdir(d)) != NULL)
	        {
	            FILE* isFile = fopen(dir->d_name,"r");
	            if(isFile){
	            	fseek(isFile, 0L, SEEK_END);
	    			long int boyut = ftell(isFile);
					belgeler[i-1].dosyaNo=-1;
					belgeler[i-1].belgeNo=i;
					strcpy(belgeler[i-1].belgeAdi,dir->d_name);
					belgeler[i-1].belgeBoyut = boyut;
					strcpy(belgeler[i-1].belgeTuru,"Belge");
					fclose(isFile);
    				struct stat stats;
					stat(dir->d_name,&stats);
   					struct tm dt;
   					dt = *(gmtime(&stats.st_ctime));
   					belgeler[i-1].belgeKayitTarihi.gun = dt.tm_mday;
   					belgeler[i-1].belgeKayitTarihi.ay = dt.tm_mon;
   					belgeler[i-1].belgeKayitTarihi.yil = dt.tm_year+1900;
   					belgeler[i-1].belgeKayitTarihi.saat = dt.tm_hour;
   					belgeler[i-1].belgeKayitTarihi.dakika = dt.tm_min;
   					belgeler[i-1].belgeKayitTarihi.saniye = dt.tm_sec;
				}
				else{
					toplam=0;
					DIR* d2;
	            	struct dirent *dir2;
	            	d2 = opendir(dir->d_name);
	            	while((dir2=readdir(d2))!=NULL){
			        	char tamAd2[250];
			        	strcpy(tamAd2,dir->d_name);
			        	strcat(tamAd2,"\\");
			        	strcat(tamAd2,dir2->d_name);
	            		FILE* isFile2 = fopen(tamAd2,"r");
	            		if(isFile2){
			            	fseek(isFile2, 0L, SEEK_END);
			    			long int boyut = ftell(isFile2);	
			    			toplam+=boyut;
			    			adet++;
			    			fclose(isFile2);
						}
					}
					closedir(d2);
					belgeler[i-1].belgeNo=-1;
					belgeler[i-1].dosyaNo=i;
					strcpy(belgeler[i-1].dosyaAdi,dir->d_name);
					belgeler[i-1].dosyaBoyut = toplam;
					strcpy(belgeler[i-1].dosyaTuru,"Klasor");
					belgeler[i-1].dosyaSayisi=adet;
				}
				i++;
			}
	        closedir(d);
		}
		belgeler[i-1].belgeNo=-2;
		belgeler[i-1].dosyaNo=-2;
		tumunulistele();
		printf("1-Ust Dizine Git\n2-Secilen Dizine Git\n3-Dizin Sec\nSecim:");
		scanf("%d",&secim);
		if(secim==1){
			chdir("..");
		}
		else if(secim==2){
			printf("Dizin adi giriniz: ");
			scanf(" %[^\n]%*c",lokasyon);
			int ch = chdir(lokasyon);
			if(!ch){
				printf("Dizin bulunamadi.\n");
			}
		}
		else if(secim==3){
			int ch = chdir(lokasyon);
			char *cur = (char*)calloc(200,sizeof(char));
			getcwd(cur,200);
			strcat(cur,"\\");
			if(!ch){
				printf("Dizin bulunamadi.\n");
			}
			else{
				printf("Secilen dizin: %s\n",cur);
				return cur;
			}
		}
		else{
			printf("Hatali secim\n");
		}
	}
}
void dosyaolustur(){
	char dosyaAdi[50];
	char tamisim[250];
	printf("Klasor Adi giriniz (orn: klasor1):");
	scanf(" %[^\n]%*c",dosyaAdi);
	printf("Eklenecek klasor icin konum seciniz\n");
	char *lokasyon = konumsec();
	strcpy(tamisim,lokasyon);
	strcat(tamisim,dosyaAdi);
	DIR* dir = opendir(lokasyon);
	if(dir){
		int kontrol = mkdir(tamisim);
		if(kontrol){
			printf("Klasor olusturulamadi.\n");
		}
		else{
			printf("Klasor %s konumunda olusturuldu.\n",tamisim);
		}
		closedir(dir);
	}
	else{
		printf("Konum bulunamadi.\n");
	}
}
void belgeekle(){
	char belgeismi[50];
	char turu[10];
	char tamisim[100];
	printf("Belge Adi giriniz (orn: 'belge1'): ");
	scanf(" %[^\n]%*c",belgeismi);
	printf("Belge turu giriniz (orn: '.txt'): ");
	scanf(" %[^\n]%*c",turu);
	printf("Belgenin kaydedilecegi konumu seciniz:\n");
	char* klasoradi = konumsec();
	strcpy(tamisim,klasoradi);
	strcat(tamisim,belgeismi);
	strcat(tamisim,turu);
	DIR* dir = opendir(klasoradi);
	if(dir){
		FILE* f = fopen(tamisim,"w");
		if(f){
			printf("Belge %s konumunda olusturuldu.\n",tamisim);
			fclose(f);
		}
		else{
			printf("Belge olusturulamadi.\n");
		}
		closedir(dir);
	}
	else{
		printf("Konum bulunamadi.\n");
	}
}
void dosyaekle(){
	char temp[4096];
	char klasorismi[50];
	printf("Olusturulacak klasorun ismini giriniz (orn klasor1):");
	scanf("%[^\n]%*c",klasorismi);
	printf("Klasorun kaydedilecegi konumu seciniz:\n");
	char* hedef = konumsec();
	printf("Hangi dizindeki dosya ve belgeleri kopyalayacaksiniz? Seciniz:\n");
	char* konum = konumsec();
	char tamisim[250];
	strcpy(tamisim,hedef);
	strcat(tamisim,klasorismi);
	DIR* dir = opendir(hedef);
	if(dir){
		int kontrol = mkdir(tamisim);
		if(kontrol){
			printf("Klasor olusturulamadi.\n");
			return;
		}
		else{
			printf("Klasor %s konumunda olusturuldu.\n",tamisim);
	    	DIR *d;
		    struct dirent *dir;
		    d = opendir(konum);
		    if (d)
		    {
		        while ((dir = readdir(d)) != NULL)
		        {
		            //printf("%s\n", dir->d_name);
		            char kopya_dosya[300];
		            char hedef_dosya[300];
		            strcpy(kopya_dosya,konum);
		            strcat(kopya_dosya,dir->d_name);
		            strcpy(hedef_dosya,tamisim);
		            strcat(hedef_dosya,"\\");
		            strcat(hedef_dosya,dir->d_name);
		            FILE* isFile = fopen(kopya_dosya,"r");
		            if(isFile){
						fclose(isFile);
						int src_fd = open(kopya_dosya, O_RDONLY);
					    int dst_fd = open(hedef_dosya, O_CREAT | O_WRONLY);
			            int err = read(src_fd, temp, 4096);
				        if (err == -1) {
				            printf("Dosya okuma hatasi: %s.\n",kopya_dosya);
				        }
				        int n = err;
				
				        if (n == 0) break;
				
				        err = write(dst_fd, temp, n);
				        if (err == -1) {
				            printf("Dosya yazma hatasi : %s.\n",hedef_dosya);
				        }
					    close(src_fd);
					    close(dst_fd);
					}
					else{
						mkdir(hedef_dosya);
					}
		        }
		        closedir(d);
			    }
		    printf("Dosya ve klasorler kopyalandi.\n");
		}
		closedir(dir);
	}
	else{
		printf("Konum bulunamadi.\n");
	}
}
void belgelistele(){
	printf("Belgelerin listelenecegi konumu seciniz:\n");
	char* konum = konumsec();
	chdir(konum);
	DIR *d;
    struct dirent *dir;
    int i=1;
    d = opendir(konum);
    if (d)
    {
    	printf("%-10s %-50s %-20s\n","Belge No","Belge Adi&Turu","Boyut (bytes)");
        while ((dir = readdir(d)) != NULL)
        {
        	char tamAd[250];
        	strcpy(tamAd,konum);
        	strcat(tamAd,dir->d_name);
            FILE* isFile = fopen(tamAd,"r");
            if(isFile){
            	fseek(isFile, 0L, SEEK_END);
    			long int boyut = ftell(isFile);
    			printf("%-10d %-50s %-20d\n",i++,dir->d_name,boyut);
				fclose(isFile);
			}
        }
        closedir(d);
	}
}
void dosyalistele(){
	printf("Klasorlerin listelenecegi konumu seciniz:\n");
	char* konum = konumsec();
	chdir(konum);
	DIR *d;
    struct dirent *dir;
    int i=1;
    d = opendir(konum);
    if (d)
    {
    	printf("%-10s %-50s %-20s\n","Belge No","Belge Adi&Turu","Boyut (bytes)");
        while ((dir = readdir(d)) != NULL)
        {
        	long int toplam = 0;
        	char tamAd[250];
        	strcpy(tamAd,konum);
        	strcat(tamAd,dir->d_name);
            FILE* isFile = fopen(tamAd,"r");
            if(!isFile){
            	DIR* d2;
            	struct dirent *dir2;
            	d2 = opendir(tamAd);
            	while((dir2=readdir(d2))!=NULL){
		        	char tamAd2[250];
		        	strcpy(tamAd2,tamAd);
		        	strcat(tamAd2,"\\");
		        	strcat(tamAd2,dir2->d_name);
            		FILE* isFile2 = fopen(tamAd2,"r");
            		if(isFile2){
		            	fseek(isFile2, 0L, SEEK_END);
		    			long int boyut = ftell(isFile2);	
		    			toplam+=boyut;
		    			fclose(isFile2);
					}
				}
				closedir(d2);
    			printf("%-10d %-50s %-20d\n",i++,dir->d_name,toplam);
			}
			else fclose(isFile);
        }
        closedir(d);
	    }
}
void ara(){
	printf("Belgelerin ve dosyalarin aranacagi konumu seciniz:\n");
	char* konum = konumsec();
	chdir(konum);
	printf("Aranacak kelimeyi giriniz:");
	char kelime[50];
	scanf(" %[^\n]%*c",kelime);
	DIR *d;
    struct dirent *dir;
    int i=1;
    d = opendir(konum);
    if (d)
    {
    	printf("%-10s %-50s %-10s %-20s\n","Belge No","Belge Adi","Turu","Boyut (bytes)");
        while ((dir = readdir(d)) != NULL)
        {
        	if(strstr(dir->d_name,kelime)!=NULL){
        		char tamAd[250];
	        	strcpy(tamAd,konum);
	        	strcat(tamAd,dir->d_name);
	            FILE* isFile = fopen(tamAd,"r");
	            if(isFile){
	            	fseek(isFile, 0L, SEEK_END);
	    			long int boyut = ftell(isFile);
	    			printf("%-10d %-50s %-10s %-20d\n",i++,dir->d_name,"Belge",boyut);
					fclose(isFile);
				}
				else{
	    			printf("%-10d %-50s %-10s %-20d\n",i++,dir->d_name,"Klasor",0);
				}
			}
        	
        }
        closedir(d);
	}
}

void sil(){
	printf("Belgelerin veya dosyalarin silinecegi konumu seciniz:\n");
	char* konum = konumsec();
	chdir(konum);
	printf("Aranacak kelimeyi giriniz:");
	char kelime[50];
	scanf(" %[^\n]%*c",kelime);
	DIR *d;
    struct dirent *dir;
    int i=1;
    d = opendir(konum);
    if (d)
    {
    	printf("%-10s %-50s %-10s %-20s\n","Belge No","Belge Adi","Turu","Boyut (bytes)");
        while ((dir = readdir(d)) != NULL)
        {
        	if(strstr(dir->d_name,kelime)!=NULL){
        		char tamAd[250];
				char cevap;
	        	strcpy(tamAd,konum);
	        	strcat(tamAd,dir->d_name);
	            FILE* isFile = fopen(tamAd,"r");
	            if(isFile){
	            	fseek(isFile, 0L, SEEK_END);
	    			long int boyut = ftell(isFile);
	    			printf("%-10d %-50s %-10s %-20d\nBu klasoru silmek istediginize emin misiniz? y/n\n",i++,dir->d_name,"Belge",boyut);
					fclose(isFile);
					scanf(" %c",&cevap);
					if(cevap=='y'||cevap=='Y'){
						remove(tamAd);
						printf("Belge silindi\n");
					}
				}
				else{
	    			printf("%-10d %-50s %-10s %-20d\nBu klasoru silmek istediginize emin misiniz? y/n\n",i++,dir->d_name,"Klasor",0);
					scanf(" %c",&cevap);
					if(cevap=='y'||cevap=='Y'){	
						int res = rmdir(tamAd);
						if(res==-1){
							printf("Klasor bos degil, silinemedi.\n");
						}
						else{
							printf("Klasor silindi\n");
						}
					}
				}
				
			}
        	
        }
        closedir(d);
	}
}
void rapor(){
	printf("Rapor olusturulacak konumu seciniz:\n");
	char* konum = konumsec();
	FILE* rapordosya = fopen("rapor.txt","w");
	chdir(konum);
	DIR *d;
    struct dirent *dir;
    int i=1;
    d = opendir(konum);
    if (d)
    {
    	fprintf(rapordosya,"%-10s %-50s %-10s %-20s\n","Belge No","Belge Adi","Turu","Boyut (bytes)");
        while ((dir = readdir(d)) != NULL)
        {
        	long int toplam = 0;
        	char tamAd[250];
        	strcpy(tamAd,konum);
        	strcat(tamAd,dir->d_name);
            FILE* isFile = fopen(tamAd,"r");
            if(!isFile){
            	DIR* d2;
            	struct dirent *dir2;
            	d2 = opendir(tamAd);
            	while((dir2=readdir(d2))!=NULL){
		        	char tamAd2[250];
		        	strcpy(tamAd2,tamAd);
		        	strcat(tamAd2,"\\");
		        	strcat(tamAd2,dir2->d_name);
            		FILE* isFile2 = fopen(tamAd2,"r");
            		if(isFile2){
		            	fseek(isFile2, 0L, SEEK_END);
		    			long int boyut = ftell(isFile2);	
		    			toplam+=boyut;
		    			fclose(isFile2);
					}
				}
				closedir(d2);
    			fprintf(rapordosya,"%-10d %-50s %-10s %-20d\n",i++,dir->d_name,"Belge",toplam);
			}
			else {
				fseek(isFile, 0L, SEEK_END);
    			long int boyut = ftell(isFile);
    			fprintf(rapordosya,"%-10d %-50s %-10s %-20d\n",i++,dir->d_name,"Klasor",boyut);
				fclose(isFile);
			}
        }
        closedir(d);
	    }
	fclose(rapordosya);
	printf("Rapor.txt, ana dizinde olusturuldu ve kaydedildi.\n");
	
}
int main(int argc, char *argv[]) {
	while(1){
		int secim;
		printf("1-Belge Islemler\n2-Klasor Islemleri\n3-Diger Islemler\n4-Cikis\nSecim:");
		scanf("%d",&secim);
		if(secim==1){
			printf("1-Belge Ekle\n2-Belge Listele\nSecim:");
			scanf("%d",&secim);
			if(secim==1){
				belgeekle();
			}
			else if(secim==2){
				belgelistele();
			}
		}
		else if(secim==2){
			printf("1-Klasor Ekle\n2-Klasor Listele\n3-Klasor Kopyala\nSecim:");
			scanf("%d",&secim);
			if(secim==1){
				dosyaolustur();
			}
			else if(secim==2){
				dosyalistele();
			}
			else if(secim==3){
				dosyaekle();
			}
		}
		else if(secim==3){
			printf("1-Klasor/Dosya Ara\n2-Klasor/Dosya Sil\n3-Rapor Al\nSecim:");
			scanf("%d",&secim);
			if(secim==1){
				ara();
			}
			else if(secim==2){
				sil();
			}
			else if(secim==3){
				rapor();
			}
		}
		else if(secim==4){
			break;
		}
		else{
			printf("Hatali giris.\n");
		}
	}
	return 0;
}
