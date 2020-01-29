#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <windows.h>
#include "MMSystem.h"
using namespace std;

#define ESC    	27
#define space   32              

struct map			//Aqui se faz o struct dos espaços do mapa
{
	int x1;
	int y1;
	int x2;
	int y2;
	int who;
	int whoNum;
	bool someone;
};

struct character	//Aqui se faz o struct para cada personagem 
{
	int px;
	int py;
	int mapx;
	int mapy;
	int frame;
	bool player;
	
	
	bool hitted;
	int bloodFrame;
	int life;
	bool canAtc;
	bool canMove;
	bool atc;
	int moviment;
	int range;
	int damage;
};


void GravaArq(char *NomeArq, unsigned char *V, int Tam) {
  FILE *Arq;
  int i;
  Arq = fopen(NomeArq, "w");
  for(i = 0; i < Tam; i++)
    if (V[i] != 10)
      fprintf(Arq, "(%4d) %d = '%c'\n", i, V[i], V[i]);
    else
      fprintf(Arq, "(%4d) %d = ''\n", i, V[i]);
  fclose(Arq);
}

void AlteraCorImg(unsigned char *P, int Tam, int Cor) {
  int i;
  for(i=0; i < Tam; i++)
    if (i > 23 and i % 4 < 3)
      P[i] = Cor;
}

void PreparaImg(int Tam, unsigned char *Img, unsigned char *Msk) {
// Tam é o tamanho dos ponteiros da imagem e da máscara
// Img é o ponteiro que aponta para o vetor que contém a imagem capturada com getimage(..)
// Msk é o ponteiro que será transformado em máscara
  int i;
  unsigned char B, G, R;
  B = Img[24];
  G = Img[25];
  R = Img[26];
  for(i=24; i < Tam; i+=4) {
    if (Img[i]==B and Img[i+1]==G and Img[i+2]==R) {
      Img[i] = 0;
      Img[i+1] = 0;
      Img[i+2] = 0;
      Msk[i] = 255;
      Msk[i+1] = 255;
      Msk[i+2] = 255;
    }
    else {
      Msk[i] = 0;
      Msk[i+1] = 0;
      Msk[i+2] = 0;
    }
  }
}




int main()
{
	
  /////INICIO DE VARIÁVEIS/////
  
  int xscreen, yscreen;
  int screen_type = 2;						  // Screentype 1 = 800 X 600
  int bk_size, size, pg = 1;	     		 // Screentype 2 = 1280 X 720
  char tecla = 0;	 		  				// Screentype 3 = 1600 X 900
  int i, j, h;   		   					// Screentype 4 = Fullscreen
  int mx = 0, my = 0, mxR = 0, myR = 0;
  POINT mousePos;
  int choice = 0;
  HWND screenHandle;
  
  unsigned long long start, end, average;				//Variaveis utilizadas para medir o FPS
  
  char pointsChar [30];
  int points[2];
  points[0] = 100;
  points[1] = 100;
  
	  int sSizeW = 170, sSizeH = 121;				//Tamanho da imagem do Swordsmen X, Y
	  int bSizeW = 132, bSizeH = 123;				//Tamanho da imagem do Bowmen X, Y
	  int shSizeW =132, shSizeH =122;				//Tamanho da imagem do Schield's men X, Y
	  int spSizeW =233, spSizeH =124;				//Tamanho da imagem do Spearsmen X, Y
	  
	  int sSizeWp = 150, sSizeHp = 124;				//Tamanho da imagem do Swordsmen X, Y
	  int bSizeWp = 132, bSizeHp = 117;				//Tamanho da imagem do Bowmen X, Y
	  int hSizeWp =128, hSizeHp =119;				//Tamanho da imagem do Schield's men X, Y
	  int spSizeWp =197, spSizeHp =120;				//Tamanho da imagem do Spearsmen X, Y
	  
	  int pSizeW = 279, pSizeH = 105;
	  int indSizeW=115, indSizeH =43;
	  int hcSizeW =466, hcSizeH = 89;
	  
	  int squareW = 115, squareH = 89;
	  
	  int upY = 20;									//Quanto maior o "upY" menos o personagem sai da borda de cima
	  int animation = 8;						//Quanto mais alto O número de "animation" mais devagar fica a animação
	  										   //Atenção:Use apenas números pares em "animation"
	  	
		/*									   
		  	AVISO 1:Para a melhor otimização do jogo,		  ||	AVISO 2: o tamanho sempre é 1 menor q o original,
		  	se possível, use o memso tamanho de imagem para	  ||	por exemplo, se a imagem for 100 por 200 você deve
		  	os Swordsmen, Bowmen e Schield's men.			  ||	colocar SizeW = 99, SizeH =199
	  	*/
	  										   
	  //
	  ///
	  //////////////////// CAIQUE ////////////////////
  
  int printNumber = 0;
  int printY,printX;
  
  bool mainMenu = 1;
  bool gameplay = 0;
  bool cutscene = 0;
  int endGame = 0;
  int swordsMenNumber = 0, schieldsMenNumber = 0;	//Números que contam a quantidades 
  int bowMenNumber = 0, spearMenNumber = 0;			//de personagens de cada tipo
  int horseManNumber = 0;
  
  
  int sound=0;							//Variaáveis de som
  int canSound=0;
  bool language=0;
  bool options=0;
  map optionsRec[5];
  int volChoice = 1;
  
  bool viewLife = 0;							//Variaáveis de som
  bool viewLL = 1;
  int iLife = 0, jLife = 0;
  int whoLife;
  
  bool moveNow = 0;
  
  void *logo;
  void *mMImage;
  void *hpBar[3];
  void *square[2];
  void *victory[2];
  void *credits;
  void *credits2;
  void *optionsPaper;
  unsigned char *paperStart[2];
  unsigned char *MpaperStart[2];
  unsigned char *paperOptions[2];
  unsigned char *MpaperOptions[2];
  unsigned char *paperExit[2];
  unsigned char *MpaperExit[2];
  
  void *scene[7];              // NICK
  
  void *bk;											//Vetores que receberam imagens
  unsigned char *spriteSwordsMenS[2];
  unsigned char *spriteSwordsMenAS[3];
  unsigned char *MspriteSwordsMenS[2];
  unsigned char *MspriteSwordsMenAS[3];
  
  unsigned char *spriteSpearMenS[2];
  unsigned char *spriteSpearMenAS[3];
  unsigned char *MspriteSpearMenS[2]; 
  unsigned char *MspriteSpearMenAS[3]; 
   
  unsigned char *spriteShieldsMenS[2];
  unsigned char *MspriteShieldsMenS[2];
     
  unsigned char *spriteBowMenS[2];
  unsigned char *spriteBowMenAS[4];
  unsigned char *MspriteBowMenS[2];
  unsigned char *MspriteBowMenAS[4];
  
  unsigned char *spriteSwordsMenP[2];
  unsigned char *spriteSwordsMenAP[3];
  unsigned char *MspriteSwordsMenP[2];
  unsigned char *MspriteSwordsMenAP[3];
  
  unsigned char *spriteSpearMenP[2];
  unsigned char *spriteSpearMenAP[3];
  unsigned char *MspriteSpearMenP[2]; 
  unsigned char *MspriteSpearMenAP[3]; 
   
  unsigned char *spriteHorseManP[2];
  unsigned char *spriteHorseManAP[3];
  unsigned char *MspriteHorseManP[2];
  unsigned char *MspriteHorseManAP[3];
     
  unsigned char *spriteBowMenP[2];
  unsigned char *spriteBowMenAP[4];
  unsigned char *MspriteBowMenP[2];
  unsigned char *MspriteBowMenAP[4];
     
  unsigned char *spriteLeonidas[2];
  unsigned char *spriteLeonidasA[3];
  unsigned char *MspriteLeonidas[2];
  unsigned char *MspriteLeonidasA[3];
  
  unsigned char *bloodL[3];
  unsigned char *MbloodL[3];
  unsigned char *bloodR[3];
  unsigned char *MbloodR[3];
   
  unsigned char *hudCharacter[2]; 
  unsigned char *MhudCharacter[2];  
  unsigned char *ind[2];
  unsigned char *Mind[2];
  unsigned char *shiftBut[2];
  unsigned char *face[2];
  unsigned char *Mface[2];
  
  character *swordsMen = NULL;					//Structs que receberam os personagens
  character *schieldsMen = NULL;
  character *horseMan = NULL;
  character *bowMen = NULL;
  character *spearMen = NULL;
  character leonidas;
  
  bool leonidasAtc = 0;
  bool atcNow = 0;								//Variáveis de ataque
  int atcI=100;
  int atcJ=100;
  bool whoAtc;
  int atcWho;
  int atcRange;
  int atcDamage;
  
  int smHP = 5;
  int bmHP = 4;
  int shHP = 10;
  int hmHP = 4;
  int spHP = 8;
  
  int smPrice=15;
  int bmPrice=35;
  int sh_hmPrice=25;
  int spPrice=35;
  
  map paper[3];
  map leonidasMap;									//Structs que geraram a tabela
  map table[8][5];
  map character_choice[4];
  map shift;                            //Botão de passar o turno
  int bout = 0;							//Contador de turno
  bool canShift;						//Checa se pode segurou o click
  map point_player;       
  
  

  
  /////FIM DO INICIO DE VARIÁVEIS/////
  
  /////INICIO DE TELA E LOAD DE IMAGENS/////
  if (screen_type == 1){ xscreen = 800; yscreen = 600;}
  else if (screen_type == 2){xscreen = 1280; yscreen = 720;}					    	      // De acordo com o Screentype se
  else if (screen_type == 3) { xscreen = 1600; yscreen = 900;}								 //estabelece o tamanho da tela
  else { xscreen = GetSystemMetrics(SM_CXSCREEN); yscreen = GetSystemMetrics(SM_CYSCREEN);}
  
  initwindow(xscreen, yscreen, "Ancestor's Fight'");
  setbkcolor(RGB(163,33,24)); 
  settextstyle(0, 0, 3);
  
  
  screenHandle = GetForegroundWindow();
  
  bk_size = imagesize(0, 0, xscreen -1, yscreen - 1);
  logo = malloc(bk_size);
  readimagefile("logo.bmp", 0, 0, xscreen -1, yscreen - 1);
  getimage(0, 0, xscreen -1, yscreen - 1, logo);
  putimage(0, 0, logo, COPY_PUT);
  setactivepage(pg);
  
  if (screen_type == 2)			//Aqui vão carregar todas as imagens compátiveis com a resolução 2
  {
	  	for(i=0; i < 2; i++)	//Reserva no struct espaço pra imagem correspondente
	  {
	  	  size = imagesize(0, 0, sSizeW, sSizeH);					//Sparta
		  spriteSwordsMenS[i] = (unsigned char *) malloc(size);
		  MspriteSwordsMenS[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, shSizeW, shSizeH);		  
		  spriteShieldsMenS[i] = (unsigned char *) malloc(size);
		  MspriteShieldsMenS[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, bSizeW, bSizeH);		   
		  spriteBowMenS[i] = (unsigned char *) malloc(size);
		  MspriteBowMenS[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, spSizeW, spSizeH);  
		  spriteSpearMenS[i] = (unsigned char *) malloc(size);
		  MspriteSpearMenS[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, 82, 111);  
		  spriteLeonidas[i] = (unsigned char *) malloc(size);
		  MspriteLeonidas[i] = (unsigned char *) malloc(size);
		  
		  
		  size = imagesize(0, 0, sSizeWp, sSizeHp);					//Persia
		  spriteSwordsMenP[i] = (unsigned char *) malloc(size);
		  MspriteSwordsMenP[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, hSizeWp, hSizeHp);		  
		  spriteHorseManP[i] = (unsigned char *) malloc(size);
		  MspriteHorseManP[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, bSizeWp, bSizeHp);		   
		  spriteBowMenP[i] = (unsigned char *) malloc(size);
		  MspriteBowMenP[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, spSizeWp, spSizeHp);  
		  spriteSpearMenP[i] = (unsigned char *) malloc(size);
		  MspriteSpearMenP[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, pSizeW, pSizeH);  				//HUD
		  paperStart[i] = (unsigned char *) malloc(size);
		  MpaperStart[i] = (unsigned char *) malloc(size);
		  paperOptions[i] = (unsigned char *) malloc(size);
		  MpaperOptions[i] = (unsigned char *) malloc(size);
		  paperExit[i] = (unsigned char *) malloc(size);
		  MpaperExit[i] = (unsigned char *) malloc(size);
		  
		  
		  size = imagesize(0, 0, hcSizeW, hcSizeH);
		  hudCharacter[i] = (unsigned char *) malloc(size);
		  MhudCharacter[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, indSizeW, indSizeH);  
		  ind[i] = (unsigned char *) malloc(size);
		  Mind[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, 79, 85);  
		  face[i] = (unsigned char *) malloc(size);
		  Mface[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, 115, 69);  
		  shiftBut[i] = (unsigned char *) malloc(size);
		  
		  size = imagesize(0, 0, squareW, squareH);
		  square[i] = malloc(size);
		  
		  size = imagesize(0, 0, xscreen -1, yscreen - 1);
	  	  victory[i] = malloc(size);
	  
	  }
  	  
  	  							///SPARTA///
	  readimagefile("sprites/smS1.bmp", 0, 0, sSizeW, sSizeH);	//Linka a stand by do SwodsMen
	  getimage(0, 0, sSizeW, sSizeH, spriteSwordsMenS[0]);
	  getimage(0, 0, sSizeW, sSizeH, MspriteSwordsMenS[0]);
	  readimagefile("sprites/smS2.bmp", 0, 0, sSizeW, sSizeH);
	  getimage(0, 0, sSizeW, sSizeH, spriteSwordsMenS[1]);
	  getimage(0, 0, sSizeW, sSizeH, MspriteSwordsMenS[1]);		//Termina o link do SwodsMen
	  
	  readimagefile("sprites/bmS1.bmp", 0, 0, bSizeW, bSizeH);	//Linka a stand by do BowMen
	  getimage(0, 0, bSizeW, bSizeH, spriteBowMenS[0]);
	  getimage(0, 0, bSizeW, bSizeH, MspriteBowMenS[0]);
	  readimagefile("sprites/bmS2.bmp", 0, 0, bSizeW, bSizeH);
	  getimage(0, 0, bSizeW, bSizeH, spriteBowMenS[1]);
	  getimage(0, 0, bSizeW, bSizeH, MspriteBowMenS[1]);		//Termina o link do BowMen
	  
	  readimagefile("sprites/shmS1.bmp", 0, 0, shSizeW, shSizeH);   //Linka a stand by do ShieldsMen
	  getimage(0, 0, shSizeW, shSizeH, spriteShieldsMenS[0]);
	  getimage(0, 0, shSizeW, shSizeH, MspriteShieldsMenS[0]);
      readimagefile("sprites/shmS2.bmp", 0, 0, shSizeW, shSizeH);
	  getimage(0, 0, shSizeW, shSizeH, spriteShieldsMenS[1]);
	  getimage(0, 0, shSizeW, shSizeH, MspriteShieldsMenS[1]);	   //Termina o link do ShieldsMen
	  
	  readimagefile("sprites/spmS1.bmp", 0, 0, spSizeW, spSizeH);  //Linka a stand by do SpearsMen
	  getimage(0, 0, spSizeW, spSizeH, spriteSpearMenS[0]);	  
	  getimage(0, 0, spSizeW, spSizeH, MspriteSpearMenS[0]);	
	  readimagefile("sprites/spmS2.bmp", 0, 0, spSizeW, spSizeH);
	  getimage(0, 0, spSizeW, spSizeH, spriteSpearMenS[1]);
	  getimage(0, 0, spSizeW, spSizeH, MspriteSpearMenS[1]);	  //Termina o link do SpearsMen
	  
	  readimagefile("sprites/leonidas1.bmp", 0, 0, 82, 111);          //Linka a stand by do Leonidas
	  getimage(0, 0, 82, 111, spriteLeonidas[0]);	  
	  getimage(0, 0, 82, 111, MspriteLeonidas[0]); 
	  readimagefile("sprites/leonidas2.bmp", 0, 0, 82, 111);
	  getimage(0, 0, 82, 111, spriteLeonidas[1]);
	  getimage(0, 0, 82, 111, MspriteLeonidas[1]);	             //Termina o link do Leonidas
	  
	  							///PERSIA///
	  readimagefile("sprites/smP1.bmp", 0, 0, sSizeWp, sSizeHp);	//Linka a stand by do SwodsMen
	  getimage(0, 0, sSizeWp, sSizeHp, spriteSwordsMenP[0]);
	  getimage(0, 0, sSizeWp, sSizeHp, MspriteSwordsMenP[0]);
	  readimagefile("sprites/smP2.bmp", 0, 0, sSizeWp, sSizeHp);
	  getimage(0, 0, sSizeWp, sSizeHp, spriteSwordsMenP[1]);
	  getimage(0, 0, sSizeWp, sSizeHp, MspriteSwordsMenP[1]);		//Termina o link do SwodsMen
	  
	  readimagefile("sprites/bmP1.bmp", 0, 0, bSizeWp, bSizeHp);	//Linka a stand by do BowMen
	  getimage(0, 0, bSizeWp, bSizeHp, spriteBowMenP[0]);
	  getimage(0, 0, bSizeWp, bSizeHp, MspriteBowMenP[0]);
	  readimagefile("sprites/bmP2.bmp", 0, 0, bSizeWp, bSizeHp);
	  getimage(0, 0, bSizeWp, bSizeHp, spriteBowMenP[1]);
	  getimage(0, 0, bSizeWp, bSizeHp, MspriteBowMenP[1]);		//Termina o link do BowMen
	  
	  readimagefile("sprites/hmP1.bmp", 0, 0, hSizeWp, hSizeHp);   //Linka a stand by do Horseman
	  getimage(0, 0, hSizeWp, hSizeHp, spriteHorseManP[0]);
	  getimage(0, 0, hSizeWp, hSizeHp, MspriteHorseManP[0]);
	  readimagefile("sprites/hmP2.bmp", 0, 0, hSizeWp, hSizeHp);
	  getimage(0, 0, hSizeWp, hSizeHp, spriteHorseManP[1]);
	  getimage(0, 0, hSizeWp, hSizeHp, MspriteHorseManP[1]);	   //Termina o link do Horseman
	  
	  readimagefile("sprites/spmP1.bmp", 0, 0, spSizeWp, spSizeHp);  //Linka a stand by do SpearsMen
	  getimage(0, 0, spSizeWp, spSizeHp, spriteSpearMenP[0]);
	  getimage(0, 0, spSizeWp, spSizeHp, MspriteSpearMenP[0]);	  
	  readimagefile("sprites/spmP2.bmp", 0, 0, spSizeWp, spSizeHp);
	  getimage(0, 0, spSizeWp, spSizeHp, spriteSpearMenP[1]); 
	  getimage(0, 0, spSizeWp, spSizeHp, MspriteSpearMenP[1]);	  //Termina o link do SpearsMen
	  
  	  							///HUD///	  
	  readimagefile("menu/play1.bmp", 0, 0, pSizeW, pSizeH);  //Linka a stand by do 
	  getimage(0, 0, pSizeW, pSizeH, paperStart[0]);
	  getimage(0, 0, pSizeW, pSizeH, MpaperStart[0]);
	  readimagefile("menu/play2.bmp", 0, 0, pSizeW, pSizeH);
	  getimage(0, 0, pSizeW, pSizeH, paperStart[1]);
	  getimage(0, 0, pSizeW, pSizeH, MpaperStart[1]);


	  readimagefile("menu/op1.bmp", 0, 0, pSizeW, pSizeH);
	  getimage(0, 0, pSizeW, pSizeH, paperOptions[0]);
	  getimage(0, 0, pSizeW, pSizeH, MpaperOptions[0]);	 
	  readimagefile("menu/op2.bmp", 0, 0, pSizeW, pSizeH);
	  getimage(0, 0, pSizeW, pSizeH, paperOptions[1]);
	  getimage(0, 0, pSizeW, pSizeH, MpaperOptions[1]);	 

	  readimagefile("menu/exit1.bmp", 0, 0, pSizeW, pSizeH);
	  getimage(0, 0, pSizeW, pSizeH, paperExit[0]);
	  getimage(0, 0, pSizeW, pSizeH, MpaperExit[0]);
	  readimagefile("menu/exit2.bmp", 0, 0, pSizeW, pSizeH);
	  getimage(0, 0, pSizeW, pSizeH, paperExit[1]);
	  getimage(0, 0, pSizeW, pSizeH, MpaperExit[1]);
				 	//Termina o link do 
	  
	  readimagefile("sprites/hudCharacterS.bmp", 0, 0, hcSizeW, hcSizeH);  //Linka a stand by do 
	  getimage(0, 0, hcSizeW, hcSizeH, hudCharacter[0]);
	  getimage(0, 0, hcSizeW, hcSizeH, MhudCharacter[0]);
	  readimagefile("sprites/hudCharacterP.bmp", 0, 0, hcSizeW, hcSizeH);
	  getimage(0, 0, hcSizeW, hcSizeH, hudCharacter[1]);
	  getimage(0, 0, hcSizeW, hcSizeH, MhudCharacter[1]);				 	//Termina o link do 	  
	  
	  readimagefile("sprites/ind1.bmp", 0, 0, indSizeW, indSizeH);  //Linka a stand by do 
	  getimage(0, 0, indSizeW, indSizeH, ind[0]);
	  getimage(0, 0, indSizeW, indSizeH, Mind[0]);	  
	  readimagefile("sprites/ind2.bmp", 0, 0, indSizeW, indSizeH);
	  getimage(0, 0, indSizeW, indSizeH, ind[1]);
	  getimage(0, 0, indSizeW, indSizeH, Mind[1]);	  //Termina o link do 
	  
	  readimagefile("sprites/sb1.bmp", 0, 0, 115, 69);  //Linka o Shift Button
	  getimage(0, 0, 115, 69, shiftBut[0]);	  
	  readimagefile("sprites/sb2.bmp", 0, 0, 115, 69);
	  getimage(0, 0, 115, 69, shiftBut[1]);
	  
	  readimagefile("sprites/is.bmp", 0, 0, 79, 85);  //Linka as faces dos personagens
	  getimage(0, 0, 79, 85, face[0]);
	  getimage(0, 0, 79, 85, Mface[0]);	  
	  readimagefile("sprites/ip.bmp", 0, 0, 79, 85);
	  getimage(0, 0, 79, 85, face[1]);
	  getimage(0, 0, 79, 85, Mface[1]);
	  
	  
      	//		             Prepara as imagens 
  	  							///SPARTA///
	  size = imagesize(0, 0, sSizeW, sSizeH);
	  PreparaImg(size, spriteSwordsMenS[0], MspriteSwordsMenS[0]);
	  PreparaImg(size, spriteSwordsMenS[1], MspriteSwordsMenS[1]);
	  
	  size = imagesize(0, 0, shSizeW, shSizeH);	
	  PreparaImg(size, spriteShieldsMenS[0], MspriteShieldsMenS[0]);
	  PreparaImg(size, spriteShieldsMenS[1], MspriteShieldsMenS[1]);
	  
	  size = imagesize(0, 0, bSizeW, bSizeH);
	  PreparaImg(size, spriteBowMenS[0], MspriteBowMenS[0]);
	  PreparaImg(size, spriteBowMenS[1], MspriteBowMenS[1]);
	  
	  size = imagesize(0, 0, spSizeW, spSizeH); 
	  PreparaImg(size, spriteSpearMenS[0], MspriteSpearMenS[0]);
	  PreparaImg(size, spriteSpearMenS[1], MspriteSpearMenS[1]);
	  
	  size = imagesize(0, 0, 82, 111); 
	  PreparaImg(size, spriteLeonidas[0], MspriteLeonidas[0]);
	  PreparaImg(size, spriteLeonidas[1], MspriteLeonidas[1]);
	  
	  							///PERSIA///	  
	  size = imagesize(0, 0, sSizeWp, sSizeHp);
	  PreparaImg(size, spriteSwordsMenP[0], MspriteSwordsMenP[0]);
	  PreparaImg(size, spriteSwordsMenP[1], MspriteSwordsMenP[1]);
	  
	  size = imagesize(0, 0, hSizeWp, hSizeHp);	
	  PreparaImg(size, spriteHorseManP[0], MspriteHorseManP[0]);
	  PreparaImg(size, spriteHorseManP[1], MspriteHorseManP[1]);
	  
	  size = imagesize(0, 0, bSizeWp, bSizeHp);
	  PreparaImg(size, spriteBowMenP[0], MspriteBowMenP[0]);
	  PreparaImg(size, spriteBowMenP[1], MspriteBowMenP[1]);
	  
	  size = imagesize(0, 0, spSizeWp, spSizeHp); 
	  PreparaImg(size, spriteSpearMenP[0], MspriteSpearMenP[0]);
	  PreparaImg(size, spriteSpearMenP[1], MspriteSpearMenP[1]);
	  
	  							///HUD///	  
	  size = imagesize(0, 0, hcSizeW, hcSizeH);
	  PreparaImg(size, hudCharacter[0], MhudCharacter[0]);
	  PreparaImg(size, hudCharacter[1], MhudCharacter[1]);
	  
	  size = imagesize(0, 0, indSizeW, indSizeH); 
	  PreparaImg(size, ind[0], Mind[0]);
	  PreparaImg(size, ind[1], Mind[1]);
	  
	  size = imagesize(0, 0, 79, 85); 
	  PreparaImg(size, face[0], Mface[0]);
	  PreparaImg(size, face[1], Mface[1]);
	  
	  size = imagesize(0, 0, pSizeW, pSizeH); 
	  PreparaImg(size, paperStart[0], MpaperStart[0]);
	  PreparaImg(size, paperStart[1], MpaperStart[1]); 
	  PreparaImg(size, paperOptions[0], MpaperOptions[0]);
	  PreparaImg(size, paperOptions[1], MpaperOptions[1]);
	  PreparaImg(size, paperExit[0], MpaperExit[0]);
	  PreparaImg(size, paperExit[1], MpaperExit[1]);
	  
  	///* Aqui loga as imagens de ataque
  
  for(i=0; i < 3; i++)
  {
  	  size = imagesize(0, 0, sSizeW, sSizeH);					//Sparta
	  spriteSwordsMenAS[i] = (unsigned char *) malloc(size);
	  MspriteSwordsMenAS[i] = (unsigned char *) malloc(size);
		  
	  size = imagesize(0, 0, spSizeW, spSizeH);  
	  spriteSpearMenAS[i] = (unsigned char *) malloc(size);
	  MspriteSpearMenAS[i] = (unsigned char *) malloc(size);
		  
	  size = imagesize(0, 0, 82, 111);  
	  spriteLeonidasA[i] = (unsigned char *) malloc(size);
	  MspriteLeonidasA[i] = (unsigned char *) malloc(size);
		  
		  
	  size = imagesize(0, 0, sSizeWp, sSizeHp);					//Persia
	  spriteSwordsMenAP[i] = (unsigned char *) malloc(size);
	  MspriteSwordsMenAP[i] = (unsigned char *) malloc(size);
		  
	  size = imagesize(0, 0, hSizeWp, hSizeHp);		  
	  spriteHorseManAP[i] = (unsigned char *) malloc(size);
	  MspriteHorseManAP[i] = (unsigned char *) malloc(size);
		  
	  size = imagesize(0, 0, spSizeWp, spSizeHp);  
	  spriteSpearMenAP[i] = (unsigned char *) malloc(size);
	  MspriteSpearMenAP[i] = (unsigned char *) malloc(size);;
	  
	  size = imagesize(0, 0, 168, 174); 
	  bloodL[i] = (unsigned char *) malloc(size);
	  MbloodL[i] = (unsigned char *) malloc(size);
	  bloodR[i] = (unsigned char *) malloc(size);
	  MbloodR[i] = (unsigned char *) malloc(size);
	  
	  size = imagesize(0, 0, xscreen -1, yscreen - 1);				//Scene
	  scene[i] = malloc(size);
	  
	  size = imagesize(0, 0, squareW, 17);
	  hpBar[i] = malloc(size);
  }
  
  for(i=0; i < 4; i++)
  {
	size = imagesize(0, 0, bSizeW, bSizeH);		   				//Sparta
	spriteBowMenAS[i] = (unsigned char *) malloc(size);
	MspriteBowMenAS[i] = (unsigned char *) malloc(size);
	
	size = imagesize(0, 0, bSizeWp, bSizeHp);		   				//Persia
	spriteBowMenAP[i] = (unsigned char *) malloc(size);
	MspriteBowMenAP[i] = (unsigned char *) malloc(size);
	
	size = imagesize(0, 0, xscreen -1, yscreen - 1);				//Tutorial
	scene[i + 3] = malloc(size);
  } 
	  														
	  readimagefile("sprites/smaS1.bmp", 0, 0, sSizeW, sSizeH);	 //Sparta
	  getimage(0, 0, sSizeW, sSizeH, spriteSwordsMenAS[0]);	
	  getimage(0, 0, sSizeW, sSizeH, MspriteSwordsMenAS[0]);	  
	  readimagefile("sprites/smaS2.bmp", 0, 0, sSizeW, sSizeH);
	  getimage(0, 0, sSizeW, sSizeH, spriteSwordsMenAS[1]);	 
	  getimage(0, 0, sSizeW, sSizeH, MspriteSwordsMenAS[1]); 
	  readimagefile("sprites/smaS3.bmp", 0, 0, sSizeW, sSizeH);
	  getimage(0, 0, sSizeW, sSizeH, spriteSwordsMenAS[2]);
	  getimage(0, 0, sSizeW, sSizeH, MspriteSwordsMenAS[2]);
	  
	  readimagefile("sprites/bmaS1.bmp", 0, 0, bSizeW, bSizeH);
	  getimage(0, 0, bSizeW, bSizeH, spriteBowMenAS[0]);
	  getimage(0, 0, bSizeW, bSizeH, MspriteBowMenAS[0]);	  
	  readimagefile("sprites/bmaS2.bmp", 0, 0, bSizeW, bSizeH);
	  getimage(0, 0, bSizeW, bSizeH, spriteBowMenAS[1]);
	  getimage(0, 0, bSizeW, bSizeH, MspriteBowMenAS[1]);	  
	  readimagefile("sprites/bmaS3.bmp", 0, 0, bSizeW, bSizeH);
	  getimage(0, 0, bSizeW, bSizeH, spriteBowMenAS[2]);
	  getimage(0, 0, bSizeW, bSizeH, MspriteBowMenAS[2]);	  
	  readimagefile("sprites/bmaS4.bmp", 0, 0, bSizeW, bSizeH);
	  getimage(0, 0, bSizeW, bSizeH, spriteBowMenAS[3]);
	  getimage(0, 0, bSizeW, bSizeH, MspriteBowMenAS[3]);
	  	  
	  readimagefile("sprites/spmaS1.bmp", 0, 0, spSizeW, spSizeH);
	  getimage(0, 0, spSizeW, spSizeH, spriteSpearMenAS[0]);
	  getimage(0, 0, spSizeW, spSizeH, MspriteSpearMenAS[0]);	  
	  readimagefile("sprites/spmaS2.bmp", 0, 0, spSizeW, spSizeH);
	  getimage(0, 0, spSizeW, spSizeH, spriteSpearMenAS[1]);	  
	  getimage(0, 0, spSizeW, spSizeH, MspriteSpearMenAS[1]);	
	  readimagefile("sprites/spmaS3.bmp", 0, 0, spSizeW, spSizeH);
	  getimage(0, 0, spSizeW, spSizeH, spriteSpearMenAS[2]);
	  getimage(0, 0, spSizeW, spSizeH, MspriteSpearMenAS[2]);
	  
	  readimagefile("sprites/leonidasmaS1.bmp", 0, 0, 82, 111);
	  getimage(0, 0, 82, 111, spriteLeonidasA[0]);
	  getimage(0, 0, 82, 111, MspriteLeonidasA[0]);	  
	  readimagefile("sprites/leonidasmaS2.bmp", 0, 0, 82, 111);
	  getimage(0, 0, 82, 111, spriteLeonidasA[1]);	  
	  getimage(0, 0, 82, 111, MspriteLeonidasA[1]);	
	  readimagefile("sprites/leonidasmaS3.bmp", 0, 0, 82, 111);
	  getimage(0, 0, 82, 111, spriteLeonidasA[2]);
	  getimage(0, 0, 82, 111, MspriteLeonidasA[2]);
	  
	  readimagefile("sprites/smaP1.bmp", 0, 0, sSizeWp, sSizeHp);	 //Persia
	  getimage(0, 0, sSizeWp, sSizeHp, spriteSwordsMenAP[0]);	
	  getimage(0, 0, sSizeWp, sSizeHp, MspriteSwordsMenAP[0]);	  
	  readimagefile("sprites/smaP2.bmp", 0, 0, sSizeWp, sSizeHp);
	  getimage(0, 0, sSizeWp, sSizeHp, spriteSwordsMenAP[1]);	 
	  getimage(0, 0, sSizeWp, sSizeHp, MspriteSwordsMenAP[1]); 
	  readimagefile("sprites/smaP3.bmp", 0, 0, sSizeWp, sSizeHp);
	  getimage(0, 0, sSizeWp, sSizeHp, spriteSwordsMenAP[2]);
	  getimage(0, 0, sSizeWp, sSizeHp, MspriteSwordsMenAP[2]);
	  
	  readimagefile("sprites/hmaP1.bmp", 0, 0, hSizeWp, hSizeHp);
	  getimage(0, 0, hSizeWp, hSizeHp, spriteHorseManAP[0]);
	  getimage(0, 0, hSizeWp, hSizeHp, MspriteHorseManAP[0]);	  
	  readimagefile("sprites/hmaP2.bmp", 0, 0, hSizeWp, hSizeHp);
	  getimage(0, 0, hSizeWp, hSizeHp, spriteHorseManAP[1]);
	  getimage(0, 0, hSizeWp, hSizeHp, MspriteHorseManAP[1]);	  
	  readimagefile("sprites/hmaP3.bmp", 0, 0, hSizeWp, hSizeHp);
	  getimage(0, 0, hSizeWp, hSizeHp, spriteHorseManAP[2]);
	  getimage(0, 0, hSizeWp, hSizeHp, MspriteHorseManAP[2]);
	  
	  readimagefile("sprites/bmaP1.bmp", 0, 0, bSizeWp, bSizeHp);
	  getimage(0, 0, bSizeWp, bSizeHp, spriteBowMenAP[0]);
	  getimage(0, 0, bSizeWp, bSizeHp, MspriteBowMenAP[0]);	  
	  readimagefile("sprites/bmaP2.bmp", 0, 0, bSizeWp, bSizeHp);
	  getimage(0, 0, bSizeWp, bSizeHp, spriteBowMenAP[1]);
	  getimage(0, 0, bSizeWp, bSizeHp, MspriteBowMenAP[1]);	  
	  readimagefile("sprites/bmaP3.bmp", 0, 0, bSizeWp, bSizeHp);
	  getimage(0, 0, bSizeWp, bSizeHp, spriteBowMenAP[2]);
	  getimage(0, 0, bSizeWp, bSizeHp, MspriteBowMenAP[2]);
	  readimagefile("sprites/bmaP4.bmp", 0, 0, bSizeWp, bSizeHp);
	  getimage(0, 0, bSizeWp, bSizeHp, spriteBowMenAP[3]);
	  getimage(0, 0, bSizeWp, bSizeHp, MspriteBowMenAP[3]);
	  	  
	  readimagefile("sprites/spmaP1.bmp", 0, 0, spSizeWp, spSizeHp);
	  getimage(0, 0, spSizeWp, spSizeHp, spriteSpearMenAP[0]);
	  getimage(0, 0, spSizeWp, spSizeHp, MspriteSpearMenAP[0]);	  
	  readimagefile("sprites/spmaP3.bmp", 0, 0, spSizeWp, spSizeHp);
	  getimage(0, 0, spSizeWp, spSizeHp, spriteSpearMenAP[1]);	  
	  getimage(0, 0, spSizeWp, spSizeHp, MspriteSpearMenAP[1]);	
	  readimagefile("sprites/spmaP3.bmp", 0, 0, spSizeWp, spSizeHp);
	  getimage(0, 0, spSizeWp, spSizeHp, spriteSpearMenAP[2]);
	  getimage(0, 0, spSizeWp, spSizeHp, MspriteSpearMenAP[2]);
	  
	  readimagefile("sprites/bloodL1.bmp", 0, 0, 168, 174); //Dano
	  getimage(0, 0, 168, 174, bloodL[0]);
	  getimage(0, 0, 168, 174, MbloodL[0]);	  
	  readimagefile("sprites/bloodL2.bmp", 0, 0, 168, 174);
	  getimage(0, 0, 168, 174, bloodL[1]);	  
	  getimage(0, 0, 168, 174, MbloodL[1]);	
	  readimagefile("sprites/bloodL3.bmp", 0, 0, 168, 174);
	  getimage(0, 0, 168, 174, bloodL[2]);
	  getimage(0, 0, 168, 174, MbloodL[2]);
	  readimagefile("sprites/bloodR1.bmp", 0, 0, 168, 174);
	  getimage(0, 0, 168, 174, bloodR[0]);
	  getimage(0, 0, 168, 174, MbloodR[0]);	  
	  readimagefile("sprites/bloodR2.bmp", 0, 0, 168, 174);
	  getimage(0, 0, 168, 174, bloodR[1]);	  
	  getimage(0, 0, 168, 174, MbloodR[1]);	
	  readimagefile("sprites/bloodR3.bmp", 0, 0, 168, 174);
	  getimage(0, 0, 168, 174, bloodR[2]);
	  getimage(0, 0, 168, 174, MbloodR[2]);
	  
	  size = imagesize(0, 0, sSizeW, sSizeH); 
	  PreparaImg(size, spriteSwordsMenAS[0], MspriteSwordsMenAS[0]);
	  PreparaImg(size, spriteSwordsMenAS[1], MspriteSwordsMenAS[1]); 
	  PreparaImg(size, spriteSwordsMenAS[2], MspriteSwordsMenAS[2]); 
	  
	  size = imagesize(0, 0, bSizeW, bSizeH);
	  PreparaImg(size, spriteBowMenAS[0], MspriteBowMenAS[0]);
	  PreparaImg(size, spriteBowMenAS[1], MspriteBowMenAS[1]);
	  PreparaImg(size, spriteBowMenAS[2], MspriteBowMenAS[2]);
	  PreparaImg(size, spriteBowMenAS[3], MspriteBowMenAS[3]);
	  
	  size = imagesize(0, 0, spSizeW, spSizeH);
	  PreparaImg(size, spriteSpearMenAS[0], MspriteSpearMenAS[0]);
	  PreparaImg(size, spriteSpearMenAS[1], MspriteSpearMenAS[1]);
	  PreparaImg(size, spriteSpearMenAS[2], MspriteSpearMenAS[2]);
	  
	  size = imagesize(0, 0, 82, 111); 
	  PreparaImg(size, spriteLeonidasA[0], MspriteLeonidasA[0]);
	  PreparaImg(size, spriteLeonidasA[1], MspriteLeonidasA[1]);
	  PreparaImg(size, spriteLeonidasA[2], MspriteLeonidasA[2]);
	  
	  size = imagesize(0, 0, sSizeWp, sSizeHp); 
	  PreparaImg(size, spriteSwordsMenAP[0], MspriteSwordsMenAP[0]);
	  PreparaImg(size, spriteSwordsMenAP[1], MspriteSwordsMenAP[1]);
	  PreparaImg(size, spriteSwordsMenAP[2], MspriteSwordsMenAP[2]);
	  
	  size = imagesize(0, 0, bSizeWp, bSizeHp);
	  PreparaImg(size, spriteBowMenAP[0], MspriteBowMenAP[0]);
	  PreparaImg(size, spriteBowMenAP[1], MspriteBowMenAP[1]);
	  PreparaImg(size, spriteBowMenAP[2], MspriteBowMenAP[2]);
	  PreparaImg(size, spriteBowMenAP[3], MspriteBowMenAP[3]);
	  
	  size = imagesize(0, 0, spSizeW, spSizeH);
	  PreparaImg(size, spriteSpearMenAP[0], MspriteSpearMenAP[0]);
	  PreparaImg(size, spriteSpearMenAP[1], MspriteSpearMenAP[1]);
	  PreparaImg(size, spriteSpearMenAP[2], MspriteSpearMenAP[2]);
	  
	  size = imagesize(0, 0, hSizeWp, hSizeHp);
	  PreparaImg(size, spriteHorseManAP[0], MspriteHorseManAP[0]);
	  PreparaImg(size, spriteHorseManAP[1], MspriteHorseManAP[1]);
	  PreparaImg(size, spriteHorseManAP[2], MspriteHorseManAP[2]);
	  
	  size = imagesize(0, 0, 168, 174);
	  PreparaImg(size, bloodL[0], MbloodL[0]);
	  PreparaImg(size, bloodL[1], MbloodL[1]);
	  PreparaImg(size, bloodL[2], MbloodL[2]);
	  PreparaImg(size, bloodR[0], MbloodR[0]);
	  PreparaImg(size, bloodR[1], MbloodR[1]);
	  PreparaImg(size, bloodR[2], MbloodR[2]);
	  
	  readimagefile("sprites/HPBar.bmp", 0, 0, squareW, 17);
	  getimage(0, 0, squareW, 17, hpBar[0]);
	  readimagefile("sprites/Mbordahp.bmp", 0, 0, squareW, 17);
	  getimage(0, 0, squareW, 17, hpBar[1]);
	  readimagefile("sprites/bordahp.bmp", 0, 0, squareW, 17);
	  getimage(0, 0, squareW, 17, hpBar[2]);
	  
	  readimagefile("sprites/atk.bmp", 0, 0, squareW, squareH);
	  getimage(0, 0, squareW, squareH, square[0]);
	  readimagefile("sprites/walk.bmp", 0, 0, squareW, squareH);
	  getimage(0, 0, squareW, squareH, square[1]);
	  
	  bk_size = imagesize(0, 0, xscreen -1, yscreen - 1);	//Aqui pega a imagem de BACKGROUND
	  bk = malloc(bk_size);
	  mMImage = malloc(bk_size); 
	  //bk_size = imagesize(0, 0, xscreen -1, (yscreen*2) - 1);
	  credits = malloc(bk_size);
	  credits2 = malloc(bk_size);
	  						
	  								
	  readimagefile("bk.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, bk);
	  readimagefile("menu/mainMenu.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, mMImage);
	  
	  size = imagesize(0, 0, xscreen/ 2, yscreen/ 2 + (yscreen/ 4));
	  optionsPaper = malloc(size);
	  readimagefile("menu/optionsPaper.bmp", 0, 0, xscreen/ 2, yscreen/ 2 + (yscreen/ 4));
	  getimage(0, 0, xscreen/ 2, yscreen/ 2 + (yscreen/ 4), optionsPaper);
	  
	  readimagefile("scene/cutscene.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, scene[0]);
	  readimagefile("scene/cutscene2.bmp", 0, 0, xscreen -1, yscreen - 1); 
	  getimage(0, 0, xscreen -1, yscreen - 1, scene[1]);
	  readimagefile("scene/cutscene3.bmp", 0, 0, xscreen -1, yscreen - 1); 
	  getimage(0, 0, xscreen -1, yscreen - 1, scene[2]);
	  readimagefile("scene/cutscene4.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, scene[3]);
	  readimagefile("scene/cutscene5.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, scene[4]);
	  readimagefile("scene/cutscene6.bmp", 0, 0, xscreen -1, yscreen - 1); 
	  getimage(0, 0, xscreen -1, yscreen - 1, scene[5]);
	  readimagefile("scene/cutscene7.bmp", 0, 0, xscreen -1, yscreen - 1); 
	  getimage(0, 0, xscreen -1, yscreen - 1, scene[6]);
	  
	  readimagefile("scene/vs.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, victory[0]);
	  readimagefile("scene/vp.bmp", 0, 0, xscreen -1, yscreen - 1); 
	  getimage(0, 0, xscreen -1, yscreen - 1, victory[1]);
	  readimagefile("scene/credits.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, credits);
	  readimagefile("scene/credits2.bmp", 0, 0, xscreen -1, yscreen - 1);
	  getimage(0, 0, xscreen -1, yscreen - 1, credits2);
	  
	 
  }
  
  
  /////FIM DO INICIO DE TELA E LOAD DE IMAGENS/////
  
  
  /////GERAÇÃO DOS RETANGULOS DO MAPA/////
  for (i=0; i < 8; i += 1) //Faz o quadrado do mapa na posição desejada com uma margem de meio quadrado
  {
  	 for (j=0; j < 5; j += 1)
     {
	  	    table[i][j].x1 = ((xscreen /11) * i + ((xscreen/22) * 3)); // o número quebrado na divisão faz precisar somar o "+i", pois ele é cortado no arredondamento
	  	    table[i][j].x2 = ((xscreen /11) * (i+1) + ((xscreen/22) * 3));
	  	    table[i][j].y1 = (yscreen/8)*(j+1) + (yscreen/16 * 3);
	  	    table[i][j].y2 = (yscreen/8)*(j+2) + (yscreen/16 * 3);
	  	    table[i][j].someone = 0;
	  	    table[i][j].who = 0;
	  	    table[i][j].whoNum = 0;
	  	    //table[i][j].atcRange = 0;
     }
  }
  
  printf("Mapa: X1=%d Y1=%d X2=%d Y2=%d\n", table[0][0].x1, table[0][0].y1, table[7][4].x2, table[7][4].y2);
  	// Printa as bordas da tabela como um todo para checar a proporção
  
  for (i=0; i < 4; i += 1) //Faz o quadrado da escolha de personagem na posição desejada com uma margem de meio quadrado
  {
	  	character_choice[i].x1 = ((xscreen /10.4) * i + ((xscreen/22) * 3)) + i;
	  	character_choice[i].x2 = ((xscreen /10.4) * (i+1) + ((xscreen/22) * 3)) + i;
	  	character_choice[i].y1 = yscreen/16;
	  	character_choice[i].y2 = (yscreen/8) + (yscreen/16);
  }
  
  printf("Persoganens: X1=%d Y1=%d X2=%d Y2=%d\n\n", character_choice[0].x1, character_choice[0].y1, character_choice[3].x2, character_choice[3].y2);
  	// Printa as bordas da tabela como um todo para checar a proporção
  	
  {// Cria a posição do Shift
	  shift.x1 = ((xscreen /11) * 7 + ((xscreen/22) * 3)) + 7;
	  shift.x2 = ((xscreen /11) * (7+1) + ((xscreen/22) * 3)) + 7;    
	  shift.y1 = yscreen/11;
	  shift.y2 = character_choice[3].y2;
	  shift.someone = 0;											
	  printf("Botao: X1=%d Y1=%d X2=%d Y2=%d\n\n", shift.x1, shift.y1, shift.x2, shift.y2);
  }// Termina de criar a posição do Shift
  
  for (i=0; i < 3; i += 1) // Cria a posição dos Paper
  {
	  	paper[i].x1 = (xscreen /11);
	  	paper[i].x2 = (xscreen /11) + (xscreen/5);
	  	paper[i].y1 = (yscreen/8)*(i*1.5) + (yscreen/8 * 3) + i;
	  	paper[i].y2 = paper[i].y1 + (yscreen/8);
  }// Termina de criar a posição do Paper
  

  optionsRec[0].x1 = (xscreen /11) ;
  optionsRec[0].x2 = optionsRec[0].x1 + (xscreen/20);
  optionsRec[0].y1 = (yscreen/8 * 3) + (yscreen / 20);
  optionsRec[0].y2 = optionsRec[0].y1 + (yscreen/8);
  
  optionsRec[1].x1 = (xscreen /11) + (xscreen/5);
  optionsRec[1].x2 = optionsRec[1].x1 + (xscreen/20);
  optionsRec[1].y1 = (yscreen/8 * 3) + (yscreen / 20);
  optionsRec[1].y2 = optionsRec[0].y1 + (yscreen/8);
  
  optionsRec[2].x1 = optionsRec[0].x1;
  optionsRec[2].x2 = optionsRec[0].x2;
  optionsRec[2].y1 = (yscreen/8 * 6) + 2;
  optionsRec[2].y2 = optionsRec[2].y1 + (yscreen/8);
  
  optionsRec[3].x1 = optionsRec[1].x1;
  optionsRec[3].x2 = optionsRec[1].x2;
  optionsRec[3].y1 = optionsRec[2].y1;
  optionsRec[3].y2 = optionsRec[2].y2;

  optionsRec[4].x1 = optionsRec[3].x1 + (xscreen/10);
  optionsRec[4].x2 = optionsRec[3].x1 + (xscreen/7);
  optionsRec[4].y1 = optionsRec[3].y1 + (yscreen / 30);
  optionsRec[4].y2 = optionsRec[3].y2 + (yscreen / 30);
  // Termina de criar a posição do Options
  
  
  
  
  
  leonidasMap.someone = 1;							//Preenche a posição
  leonidasMap.x1 = table[0][2].x1 - (xscreen /11); //	do Leonidas      
  leonidasMap.x2 = table[0][2].x1;
  leonidasMap.y1 = table[0][2].y1;
  leonidasMap.y2 = table[0][2].y2;
  
  leonidas.px = leonidasMap.x1;
  leonidas.py = leonidasMap.y1;
  leonidas.frame = 0;
  leonidas.player= 0;
			  	
  leonidas.atc = 0;
  leonidas.canAtc = 0;
  leonidas.bloodFrame = 0;
  leonidas.life= 12;
  leonidas.moviment= 0;
  leonidas.range= 2;
  leonidas.damage= 4;
  /////FIM DA GERAÇÃO DOS RETANGULOS DO MAPA/////
  
  waveOutSetVolume(0, 0x11111111); //Seta o volume em Hexadecimal 0xFFFFFFFF ///0x88888888 ///0x11111111
  
  //////////////////////// ***** GAME LOOP ***** /////////////////////////
  while(tecla != ESC && (mainMenu == 1 || gameplay == 1 || cutscene == 1 || endGame > 0)) {
  	
  	while (mainMenu == 1)
	  {
	  	mciSendString("open .\\audio\\trilhamenu.mp3 type MPEGVideo alias menu", NULL, 0, 0);
      	mciSendString("play menu notify repeat", NULL, 0, 0);
	  	start = clock();				//Pega o Clock inicial do programa no início do código
  	
	    // Alterna a página de desenho.
	    if (pg == 1) pg = 2; else pg=1;
	      setactivepage(pg);
	    
	    // Limpa a área de desenho
	    cleardevice();
	    
	    putimage(0, 0, mMImage, COPY_PUT); //COLOCA O FUNDO
	    
	    /////CHECA A COLISÃO/////
	    if (options == 0)
		    for (i=0; i < 3; i += 1) //Checa a colisão da escolha de personagem na posição desejada
		    {
			    if ((paper[i].x1 <= mx && mx <= paper[i].x2) && (paper[i].y1 <= my && my <= paper[i].y2))//Checa a colisão do botão Shift
			      //Se colidiu, faz um retangulo vermelho aonde foi clicado
			      {
				  	mx=0;
				    my=0;
				    if (i== 0)
					  {
					  	putimage(paper[i].x1, paper[i].y1, MpaperStart[1], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperStart[1], OR_PUT);
					  	mainMenu = 0;
					  	gameplay = 0;
					  	cutscene = 1;
					  	sound=0;
					  	canSound=0;
					  	mciSendString("stop menu", NULL, 0, 0);
					  }
					else if (i== 1)
					  {
					  	putimage(paper[i].x1, paper[i].y1, MpaperOptions[1], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperOptions[1], OR_PUT);
		    			sound=0;
					  	canSound=0;
		    			options = 1;
					  }
					else
					  {
					  	putimage(paper[i].x1, paper[i].y1, MpaperExit[1], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperExit[1], OR_PUT);
					  	mainMenu = 0;
					  	gameplay = 0;
					  }
				  }
				  
			    else if ((paper[i].x1 <= mousePos.x && mousePos.x <= paper[i].x2) && (paper[i].y1 <= mousePos.y && mousePos.y <= paper[i].y2))
			      // Se o mouse está por cima faz um retangulo branco
				  { 
				    //mciSendString("open .\\audio\\select.mp3 type MPEGVideo alias select", NULL, 0, 0);
	      			//mciSendString("play select", NULL, 0, 0); |||| + SND_LOOP
	      			
				  	if (i== 0)
					  {
					  	if (canSound != 1){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 1;}
					  	putimage(paper[i].x1, paper[i].y1, MpaperStart[1], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperStart[1], OR_PUT);
					  }
					else if (i== 1)
					  {
					  	if (canSound != 2){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 2;}
					  	putimage(paper[i].x1, paper[i].y1, MpaperOptions[1], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperOptions[1], OR_PUT);
					  }
					else
					  {
					  	if (canSound != 3){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 3;}
					  	putimage(paper[i].x1, paper[i].y1, MpaperExit[1], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperExit[1], OR_PUT);
					  }
				  }
				else 
				{
					
					if (i== 0)
					  {
					  	sound++;
					  	putimage(paper[i].x1, paper[i].y1, MpaperStart[0], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperStart[0], OR_PUT);
					  }
					else if (i== 1)
					  {
					  	sound++;
					  	putimage(paper[i].x1, paper[i].y1, MpaperOptions[0], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperOptions[0], OR_PUT);
					  }
					else
					  {
					  	sound++;
					  	putimage(paper[i].x1, paper[i].y1, MpaperExit[0], AND_PUT);
		    			putimage(paper[i].x1, paper[i].y1, paperExit[0], OR_PUT);
					  }
				}
			}
		
		else//Caso esteja selecionado Options
		{
			putimage(optionsRec[0].x1 - (xscreen/15), optionsRec[0].y1 - (yscreen/4), optionsPaper, OR_PUT);
			for(i=0; i < 5; i += 1)
			{
				if ((optionsRec[i].x1 <= mx && mx <= optionsRec[i].x2) && (optionsRec[i].y1 <= my && my <= optionsRec[i].y2))
				{
					if(i == 0)
					{
						bar(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
						
						 if(volChoice > -1) volChoice--;
						 if (volChoice == 0) waveOutSetVolume(0, 0x00000000);
						 if (volChoice == 1) waveOutSetVolume(0, 0x11111111);
						 if (volChoice == 2) waveOutSetVolume(0, 0x88888888);
						 if (volChoice == 3) waveOutSetVolume(0, 0xFFFFFFFF);
					}
					else if(i == 1)
					{
						bar(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
						
						if(volChoice < 4)volChoice++;
						if (volChoice == 0) waveOutSetVolume(0, 0x00000000);
						if (volChoice == 1) waveOutSetVolume(0, 0x11111111);
						if (volChoice == 2) waveOutSetVolume(0, 0x88888888);
						if (volChoice == 3) waveOutSetVolume(0, 0xFFFFFFFF);
					}
					else if(i == 2)
					{
						bar(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
						if(language == 0) language = 1; else language = 0;
						printf(" %d ", language);
					}
					else if(i == 3)
					{
						bar(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
						if(language == 0) language = 1; else language = 0;
						printf(" %d ", language);
					}
					else if(i == 4)
					{
						bar(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
						options = 0;
					}
					mx = 0;
					my = 0;
				}
				else if ((optionsRec[i].x1 <= mousePos.x && mousePos.x <= optionsRec[i].x2) && (optionsRec[i].y1 <= mousePos.y && mousePos.y <= optionsRec[i].y2))
				{
					if(i == 0)
					{
						if (canSound != 1){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 1;}
						rectangle(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
					}
					else if(i == 1)
					{
						if (canSound != 2){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 2;}
						rectangle(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
					}
					else if(i == 2)
					{
						if (canSound != 3){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 3;}
						rectangle(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
					}
					else if(i == 3)
					{
						if (canSound != 4){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 4;}
						rectangle(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
					}
					else if(i == 4)
					{
						if (canSound != 5){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = 5;}
						rectangle(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
					}
				}
				else
				{
					sound++;
					rectangle(optionsRec[i].x1 ,optionsRec[i].y1, optionsRec[i].x2,optionsRec[i].y2);
				}
			}
		}
		  
	    /////FIM DA CHECAGEM DE COLISÃO/////
	    
	    setvisualpage(pg);//Atualiza a pagina de visão
	    if (sound == 3 && options == 0) canSound = 4;
	    else if (sound == 5 && options == 1) canSound = 6;
	    sound=0;
	    
	    /////CHECA A ENTRADA DE COMANDOS/////
	    GetCursorPos(&mousePos);
	    ScreenToClient(screenHandle, &mousePos);	//Pega a posição do Mouse
	    
	    
	    //Se apertou com o botão esquerdo seleciona
	    if(GetKeyState(VK_LBUTTON) & 0X100)
		{
			if(canShift == 1)
			{
				canShift = 0;
				mx = mousePos.x;
				my = mousePos.y;
			}
		}
		else canShift = 1;
		  
	    
	    fflush(stdin);
	    if (kbhit())
		{
			tecla = getch();
			if (tecla == ESC) mainMenu = 0;
		}
	    /////FIM DA CHECAGEM DA ENTRADA DE COMANDOS/////
	    
	    end = clock();						//Pega o Clock inicial do programa no início do código
	    average = end - start;  			//Faz a média dos Frames
	    //printf("FPS puro:%d\n", int(1000/average));
	    if(average<17)						//Se rodou o programa em mais de X milesegundos,
	    delay(17-average);					//se da um Delay de X - milesec.
	    									//Para 60 FPS --->17
	    									//Para 30 FPS --->33
	    									//Para 24 FPS --->42
	  }
	  
	  i= 0;
	  while (cutscene == 1)     // cutscene                     
	  {	
		  	
			mciSendString("open .\\audio\\trilha.mp3 type MPEGVideo alias trilha", NULL, 0, 0);//Loga a trilha
	      	mciSendString("play trilha notify repeat", NULL, 0, 0);//Da play na trilha
	      	
		    putimage(0, 0, scene[i], COPY_PUT);
		  
	        fflush(stdin);
		    if (kbhit()){
				tecla = getch();
				
				if (tecla == space && canShift == 1) 
				{
					i++;
					canShift = 0;
				}
				if (tecla == ESC) cutscene = 0;
				if(i== 7 && tecla == space)
				{
					cutscene = 0;
					gameplay =1;
				}
			}
			else canShift = 1;
	  }	  
	  
	/////////////////////////////////////WHILE DO GAMEPLAY/////////////////////////////////////
	while (gameplay == 1)
	  {
	  	start = clock();				//Pega o Clock inicial do programa no início do código
  	
	    // Alterna a página de desenho.
	    if (pg == 1) pg = 2; else pg=1;
	      setactivepage(pg);
	    
	    // Limpa a área de desenho
	    cleardevice();
	    
	    putimage(0, 0, bk, COPY_PUT); //COLOCA O FUNDO
	    
	    /////CHECA A COLISÃO/////
	    
	    for (i=0; i < 8; i += 1) //Checa a colisão do mapa na posição desejada
	    {
		  
	  	  for (j=0; j < 5; j += 1)
	      {    	
	        if (choice != 0 &&
		     (table[i][j].x1 <= mx && mx <= table[i][j].x2) && (table[i][j].y1 <= my && my <= table[i][j].y2) &&
		     table[i][j].someone == 0)
		     //Se escolheu alguem E clicou em um quadrado E neste quadrado não tem ninguém
		   {
		   		sndPlaySound(".\\audio\\spawn.wav", SND_ASYNC);
		   	    if(choice==1 && points[shift.someone]>= smPrice && ((shift.someone == 0 && i<4) ||(shift.someone == 1 && i>6)))			//Se escolheu Swordsmen
				{
					table[i][j].someone = 1;
					points[shift.someone] -= smPrice;
		  			table[i][j].who = 1;
		  			table[i][j].whoNum = swordsMenNumber;
		  			
					swordsMenNumber++;
		  			//printf("VAI %d", table[i][j].whoNum);
				  	swordsMen = (struct character *)realloc(swordsMen, sizeof(struct character) * swordsMenNumber);
				  	
				  	if (shift.someone == 0) swordsMen[swordsMenNumber - 1].px = table[i][j].x1;
				  	else swordsMen[swordsMenNumber - 1].px = table[i][j].x1 - (sSizeWp-(xscreen /10));
				  	swordsMen[swordsMenNumber - 1].py = table[i][j].y1 - (yscreen/upY);
				  	swordsMen[swordsMenNumber - 1].mapx = i;
				  	swordsMen[swordsMenNumber - 1].mapy = j;
				  	swordsMen[swordsMenNumber - 1].frame = 0;
				  	swordsMen[swordsMenNumber - 1].player= shift.someone;
				  	
				  	swordsMen[swordsMenNumber - 1].hitted = 0;
				  	swordsMen[swordsMenNumber - 1].bloodFrame = 0;
				  	swordsMen[swordsMenNumber - 1].atc = 0;
					swordsMen[swordsMenNumber - 1].life= smHP;
					swordsMen[swordsMenNumber - 1].canAtc = 1;
					swordsMen[swordsMenNumber - 1].canMove = 1;
				  	swordsMen[swordsMenNumber - 1].moviment= 1;
				  	swordsMen[swordsMenNumber - 1].range= 1;
					swordsMen[swordsMenNumber - 1].damage= 2;
					
					//table[i][j].range = swordsMen[swordsMenNumber - 1].range;
				  	  
				  	  //printf("\n %d %d\n",swordsMen[swordsMenNumber -1].px, swordsMen[swordsMenNumber -1].py);
				}
				else if (choice==2 && points[shift.someone]>= bmPrice && ((shift.someone == 0 && i<4) ||(shift.someone == 1 && i>6)))		//Se escolheu Bowmen
				{   
				    table[i][j].someone = 1;
					points[shift.someone] -=bmPrice;
		  			table[i][j].who = 2;
		  			table[i][j].whoNum = bowMenNumber;
		  			
					bowMenNumber++;
				  	bowMen = (struct character *)realloc(bowMen, sizeof(struct character) * bowMenNumber);
				  	
				  	bowMen[bowMenNumber - 1].px = table[i][j].x1;
				  	bowMen[bowMenNumber - 1].py = table[i][j].y1 - (yscreen/upY);
				  	bowMen[bowMenNumber - 1].mapx = i;
				  	bowMen[bowMenNumber - 1].mapy = j;
				  	bowMen[bowMenNumber - 1].frame = 0;
				  	bowMen[bowMenNumber - 1].player= shift.someone;
				  	
				  	bowMen[bowMenNumber - 1].hitted = 0;
				  	bowMen[bowMenNumber - 1].bloodFrame = 0;
				  	bowMen[bowMenNumber - 1].atc = 0;
					bowMen[bowMenNumber - 1].life= bmHP;
					bowMen[bowMenNumber - 1].canAtc = 1;
					bowMen[bowMenNumber - 1].canMove = 1;
				  	bowMen[bowMenNumber - 1].moviment= 1;
				  	bowMen[bowMenNumber - 1].range= 3;
					bowMen[bowMenNumber - 1].damage= 2;
					
					//table[i][j].range = bowMen[bowMenNumber - 1].range;
				}
				else if (choice==3 && points[shift.someone]>= sh_hmPrice && ((shift.someone == 0 && i<4) ||(shift.someone == 1 && i>6)))		//Se escolheu Shield's men
				{
					if (shift.someone == 0 )
					{   
					    table[i][j].someone = 1;
					    points[shift.someone] -= sh_hmPrice;
						table[i][j].who = 3;
		  				table[i][j].whoNum = schieldsMenNumber;
		  				
						schieldsMenNumber++;
					  	schieldsMen = (struct character *)realloc(schieldsMen, sizeof(struct character) * schieldsMenNumber);
					  	
					  	schieldsMen[schieldsMenNumber - 1].px = table[i][j].x1;
					  	schieldsMen[schieldsMenNumber - 1].py = table[i][j].y1 - (yscreen/upY);
				  		schieldsMen[schieldsMenNumber - 1].mapx = i;
				  		schieldsMen[schieldsMenNumber - 1].mapy = j;
					  	schieldsMen[schieldsMenNumber - 1].frame = 0;
					  	schieldsMen[schieldsMenNumber - 1].player= 0;
					  	
					  	schieldsMen[schieldsMenNumber - 1].hitted = 0;
					  	schieldsMen[schieldsMenNumber - 1].bloodFrame = 0;
					  	schieldsMen[schieldsMenNumber - 1].atc = 0;
						schieldsMen[schieldsMenNumber - 1].life= shHP;
						schieldsMen[schieldsMenNumber - 1].canAtc = 1;
						schieldsMen[schieldsMenNumber - 1].canMove= 1;
					  	schieldsMen[schieldsMenNumber - 1].moviment= 1;
					  	schieldsMen[schieldsMenNumber - 1].range= 1;
						schieldsMen[schieldsMenNumber - 1].damage= 0;
						
						//table[i][j].range = schieldsMen[schieldsMenNumber - 1].range;
					}
					else
					{   
					    table[i][j].someone = 1;
						points[shift.someone] -= sh_hmPrice;
		  				table[i][j].who = 5;
		  				table[i][j].whoNum = horseManNumber;
		  				
						horseManNumber++;
					  	horseMan = (struct character *)realloc(horseMan, sizeof(struct character) * horseManNumber);
					  	
					  	horseMan[horseManNumber - 1].px = table[i][j].x1;
					  	horseMan[horseManNumber - 1].py = table[i][j].y1 - (yscreen/upY);
				  		horseMan[horseManNumber - 1].mapx = i;
				  		horseMan[horseManNumber - 1].mapy = j;
					  	horseMan[horseManNumber - 1].frame = 0;
					  	horseMan[horseManNumber - 1].player= 1;
					  	
					  	horseMan[horseManNumber - 1].hitted = 0;
					  	horseMan[horseManNumber - 1].bloodFrame = 0;
					  	horseMan[horseManNumber - 1].atc = 0;
						horseMan[horseManNumber - 1].life= hmHP;
						horseMan[horseManNumber - 1].canAtc = 1;
						horseMan[horseManNumber - 1].canMove = 1;
					  	horseMan[horseManNumber - 1].moviment= 2;
					  	horseMan[horseManNumber - 1].range= 1;
						horseMan[horseManNumber - 1].damage= 4;
						
						//table[i][j].range = horseMan[horseManNumber - 1].range;
					}
					
				}
				else if (choice==4 && points[shift.someone]>= spPrice && ((shift.someone == 0 && i<4) ||(shift.someone == 1 && i>6)))		//Se escolheu Spearmen
				{   
				    table[i][j].someone = 1;
					points[shift.someone]-= spPrice;
		  			table[i][j].who = 4;
					table[i][j].whoNum = spearMenNumber;
		  			
					spearMenNumber++;
				  	spearMen = (struct character *)realloc(spearMen, sizeof(struct character) * spearMenNumber);
				  	
				  	if (shift.someone == 0) spearMen[spearMenNumber - 1].px = table[i][j].x1;
					else spearMen[spearMenNumber - 1].px = table[i][j].x1 - (spSizeWp-(xscreen /9));
				  	spearMen[spearMenNumber - 1].py = table[i][j].y1 - (yscreen/upY);
				  	spearMen[spearMenNumber - 1].mapx = i;
				  	spearMen[spearMenNumber - 1].mapy = j;
				  	spearMen[spearMenNumber - 1].frame = 0;
				  	spearMen[spearMenNumber - 1].player= shift.someone;
				  	
				  	spearMen[spearMenNumber - 1].hitted = 0;
				  	spearMen[spearMenNumber - 1].bloodFrame = 0;
				  	spearMen[spearMenNumber - 1].atc = 0;
					spearMen[spearMenNumber - 1].life= spHP;
					spearMen[spearMenNumber - 1].canAtc = 1;
					spearMen[spearMenNumber - 1].canMove = 1;
				  	spearMen[spearMenNumber - 1].moviment= 1;
				  	spearMen[spearMenNumber - 1].range= 2;
					spearMen[spearMenNumber - 1].damage= 4;
					
				  	//table[i][j].range = spearMen[spearMenNumber - 1].range;
				}
				 
		  		
		  	  	choice = 0;
		  	  	mx=0;
		  	  	my=0;
		  	  	atcNow=0;
		   }
		   
		   
		   ///-------------Detecção de ataque
		  else if(atcNow == 0 &&
		  (table[i][j].x1 <= mx && mx <= table[i][j].x2) && (table[i][j].y1 <= my && my <= table[i][j].y2) &&
	      table[i][j].someone == 1)
		  {
		      atcI=i;
		      atcJ=j;
		      if (table[i][j].who == 1 && shift.someone == swordsMen[table[i][j].whoNum].player
			  && swordsMen[table[atcI][atcJ].whoNum].canAtc == 1)
	      	  {
		      	atcWho = 1;
		      	atcNow = 1;
		      	atcDamage = swordsMen[table[i][j].whoNum].damage;
		      	atcRange = swordsMen[table[i][j].whoNum].range;
			  }
			  else if (table[i][j].who == 2 && shift.someone == bowMen[table[i][j].whoNum].player 
			  && bowMen[table[atcI][atcJ].whoNum].canAtc == 1)
	      	  {
		      	atcWho = 2;
		      	atcNow = 1;
		      	atcDamage = bowMen[table[i][j].whoNum].damage;
		      	atcRange = bowMen[table[i][j].whoNum].range;
			  }
			  else if (table[i][j].who == 4 && shift.someone == spearMen[table[i][j].whoNum].player
			  && spearMen[table[atcI][atcJ].whoNum].canAtc == 1)
	      	  {
		      	atcWho = 4;
		      	atcNow = 1;
		      	atcDamage = spearMen[table[i][j].whoNum].damage;
		      	atcRange = spearMen[table[i][j].whoNum].range;
			  }
			  else if (table[i][j].who == 5 && shift.someone == horseMan[table[i][j].whoNum].player
			  && horseMan[table[atcI][atcJ].whoNum].canAtc == 1)
	      	  {
		      	atcWho = 5;
		      	atcNow = 1;
		      	atcDamage = horseMan[table[i][j].whoNum].damage;
		      	atcRange = horseMan[table[i][j].whoNum].range;
			  }
		      
	 	  }
	     
	      else if (atcNow == 1 && table[i][j].someone == 1 &&
	      (table[i][j].x1 <= mx && mx <= table[i][j].x2) && (table[i][j].y1 <= my && my <= table[i][j].y2) &&
	      ((i <= atcI + atcRange && i >= atcI - atcRange && j == atcJ) || 
	      (i== atcI && j <= atcJ + (atcRange/2) && j >= atcJ - (atcRange/2) )))
	      {
	      	if((shift.someone == 0 && i >= atcI) || (shift.someone == 1 && i <= atcI))//N está no if de cima pois dava BUG ¯\_(?)_/¯
	      	{
	      		if (table[i][j].who == 1 && shift.someone != swordsMen[table[i][j].whoNum].player)
		      	{
		      		sndPlaySound(".\\audio\\battle.wav", SND_ASYNC);
					swordsMen[table[i][j].whoNum].life-= atcDamage;
					swordsMen[table[i][j].whoNum].hitted = 1;
					atcNow = 0;
				}
				else if (table[i][j].who == 2 && shift.someone != bowMen[table[i][j].whoNum].player)
		      	{
		      		sndPlaySound(".\\audio\\battle.wav", SND_ASYNC);
					bowMen[table[i][j].whoNum].life -= atcDamage;
					bowMen[table[i][j].whoNum].hitted = 1;
					atcNow = 0;
				}
				else if (table[i][j].who == 3 && shift.someone != schieldsMen[table[i][j].whoNum].player)
		      	{
		      		sndPlaySound(".\\audio\\battle.wav", SND_ASYNC);
		      		schieldsMen[table[i][j].whoNum].life-= atcDamage;
		      		schieldsMen[table[i][j].whoNum].hitted = 1;
		      		atcNow = 0;
				}
				else if (table[i][j].who == 4  && shift.someone != spearMen[table[i][j].whoNum].player)
		      	{
		      		sndPlaySound(".\\audio\\battle.wav", SND_ASYNC);
		      		spearMen[table[i][j].whoNum].life-= atcDamage;
		      		spearMen[table[i][j].whoNum].hitted = 1;
		      		atcNow = 0;
				}
				else if (table[i][j].who == 5 && shift.someone != horseMan[table[i][j].whoNum].player)
		      	{
		      		sndPlaySound(".\\audio\\battle.wav", SND_ASYNC);
		      		horseMan[table[i][j].whoNum].life-= atcDamage;
		      		horseMan[table[i][j].whoNum].hitted = 1;
		      		atcNow = 0;
				}
				if(atcNow == 0 && atcWho == 1){swordsMen[table[atcI][atcJ].whoNum].frame = 0; swordsMen[table[atcI][atcJ].whoNum].atc = 1; swordsMen[table[atcI][atcJ].whoNum].canAtc= 0;}
				else if(atcNow == 0 && atcWho == 2){bowMen[table[atcI][atcJ].whoNum].frame = 0; bowMen[table[atcI][atcJ].whoNum].atc = 1; bowMen[table[atcI][atcJ].whoNum].canAtc= 0;}
				else if(atcNow == 0 && atcWho == 4){spearMen[table[atcI][atcJ].whoNum].frame = 0; spearMen[table[atcI][atcJ].whoNum].atc = 1; spearMen[table[atcI][atcJ].whoNum].canAtc= 0;}
				else if(atcNow == 0 && atcWho == 5){horseMan[table[atcI][atcJ].whoNum].frame = 0; horseMan[table[atcI][atcJ].whoNum].atc = 1; horseMan[table[atcI][atcJ].whoNum].canAtc= 0;}
				else if(atcNow == 0 && atcWho == 6){leonidas.frame = 0; leonidas.atc = 1; leonidas.canAtc= 0;}
			  	mx = 0;
			  	my = 0;
			}
	      	
	      }
	      
	      
		   /*
			   	  ^													 ^
				  |	Se iniciou a condição deu inicio ele acrescenta  |
				  |um na contagem, faz um realloc no vetor		     |
				  |e prencheenche o struct com posição,			     |
				  |frame da animação, player pertencente,    		 |
				  |se está atacando ou não, vida, movimento, 		 |
				  |range de ataque e dano							 |
		   																	*/
		   
		   
		    /*				//Printa os quadrados de HitBox do Tabuleiro
	      	else rectangle (table[i][j].x1, table[i][j].y1, table[i][j].x2, table[i][j].y2);
	      	if (table[i][j].someone == 0) rectangle(table[i][j].x1, table[i][j].y1, table[i][j].x2, table[i][j].y2);		
	      	*/
	      	
	      	////////////////////////MOVIMENTAÇÃO////////////////////////
	      else if ((table[i][j].x1 <= mxR && mxR <= table[i][j].x2) && (table[i][j].y1 <= myR && myR <= table[i][j].y2) &&
		  table[i][j].someone == 1 && shift.someone == 1)
		  //Primeiro Click
		  {
		     
		     	if (table[i][j].who == 1 && shift.someone == swordsMen[table[i][j].whoNum].player)
		      	{
		      	    atcI = swordsMen[table[i][j].whoNum].mapx;                  
					atcJ = swordsMen[table[i][j].whoNum].mapy;
					moveNow = 1;
				}
				else if (table[i][j].who == 2 && shift.someone == bowMen[table[i][j].whoNum].player)
		      	{
		      	    atcI = bowMen[table[i][j].whoNum].mapx;                  
					atcJ = bowMen[table[i][j].whoNum].mapy;
					moveNow = 1;
				}
					else if (table[i][j].who == 5 && shift.someone == horseMan[table[i][j].whoNum].player)
		      	{
		      	    atcI = horseMan[table[i][j].whoNum].mapx;                  
					atcJ = horseMan[table[i][j].whoNum].mapy;
					moveNow = 1;
				}
					else if (table[i][j].who == 4 && shift.someone == spearMen[table[i][j].whoNum].player)
		      	{
		      	    atcI = spearMen[table[i][j].whoNum].mapx;                  
					atcJ = spearMen[table[i][j].whoNum].mapy;
					moveNow = 1;
				}
				 mxR = 0;
			  	 myR = 0;
		  }
		     
		  else if ((table[i][j].x1 <= mxR && mxR <= table[i][j].x2) && (table[i][j].y1 <= myR && myR <= table[i][j].y2) &&
		  table[i][j].someone == 0 && moveNow == 1 && atcI > i && atcJ == j)
		  //Segundo Click
		  {
		     	if (table[atcI][atcJ].who == 1 && atcI <= i + swordsMen[table[i][j].whoNum].moviment
				&& swordsMen[table[atcI][atcJ].whoNum].canMove == 1)
		      	{
		      		sndPlaySound(".\\audio\\march.wav", SND_ASYNC);
					swordsMen[table[atcI][atcJ].whoNum].canMove = 0;
		      	    table[i][j].someone = 1;
		      	    table[i][j].who = 1;
		      	    table[i][j].whoNum = table[atcI][atcJ].whoNum;
		      	    table[atcI][atcJ].someone = 0;
		      	    
		      	    swordsMen[table[i][j].whoNum].px = table[i][j].x1 - (sSizeWp-(xscreen /10));     //O px é feito a conta igual a q é feita no Spawn             
					swordsMen[table[i][j].whoNum].mapx = i;	
							
					moveNow = 0;
				}
				
				else if (table[atcI][atcJ].who == 2 && atcI <= i + bowMen[table[i][j].whoNum].moviment 
				&& bowMen[table[atcI][atcJ].whoNum].canMove == 1)
		      	{
		      		sndPlaySound(".\\audio\\march.wav", SND_ASYNC);
		      	    bowMen[table[atcI][atcJ].whoNum].canMove = 0;
		      	    table[i][j].someone = 1;
		      	    table[i][j].who = 2;
		      	    table[i][j].whoNum = table[atcI][atcJ].whoNum;
		      	    table[atcI][atcJ].someone = 0;
		      	    
		      	    bowMen[table[i][j].whoNum].px = table[i][j].x1;              
					bowMen[table[i][j].whoNum].mapx = i;		
					
					moveNow = 0;
				}
				else if (table[atcI][atcJ].who == 5 && atcI <= i + horseMan[table[i][j].whoNum].moviment
				&& horseMan[table[atcI][atcJ].whoNum].canMove == 1)
		      	{  
		      	    sndPlaySound(".\\audio\\march.wav", SND_ASYNC);
		      	    horseMan[table[atcI][atcJ].whoNum].canMove = 0;
		      	    table[i][j].someone = 1;
		      	    table[i][j].who = 5;
		      	    table[i][j].whoNum = table[atcI][atcJ].whoNum;
		      	    table[atcI][atcJ].someone = 0;
		      	    
		      	    horseMan[table[i][j].whoNum].px = table[i][j].x1;              
					horseMan[table[i][j].whoNum].mapx = i;		
					
					moveNow = 0;
				}
				else if (table[atcI][atcJ].who == 4 && atcI <= i + spearMen[table[i][j].whoNum].moviment 
				&& spearMen[table[atcI][atcJ].whoNum].canMove == 1)
		      	{  
		      	    sndPlaySound(".\\audio\\march.wav", SND_ASYNC);
		      	    spearMen[table[atcI][atcJ].whoNum].canMove = 0;
		      	    table[i][j].someone = 1;
		      	    table[i][j].who = 4;
		      	    table[i][j].whoNum = table[atcI][atcJ].whoNum;
		      	    table[atcI][atcJ].someone = 0;
		      	    
		      	    spearMen[table[i][j].whoNum].px = table[i][j].x1 - (spSizeWp-(xscreen /9));              
					spearMen[table[i][j].whoNum].mapx = i;		
					
					moveNow = 0;
				}
				 mxR = 0;
			  	 myR = 0;
		  }
		  
		  
		  if (table[i][j].someone == 1 &&										//Se tiver alguem e passar o mouse por cima mostra o HP
		  (table[i][j].x1 <= mousePos.x && mousePos.x <= table[i][j].x2) && 
		  (table[i][j].y1 <= mousePos.y && mousePos.y <= table[i][j].y2))
	      {
	      	viewLife = 1;
	      	whoLife = table[i][j].who;
  			iLife = i;
			jLife = j;
		  }
		  else if (table[i][j].someone == 0 && 
		  (table[iLife][jLife].x1 > mousePos.x || mousePos.x > table[iLife][jLife].x2) || 
		  (table[iLife][jLife].y1 > mousePos.y || mousePos.y > table[iLife][jLife].y2))
		  {
		  	viewLife = 0;
		  }
	      	
	      	
	      }
	      
	    }
	    
	    if (shift.someone ==1)//Print da escolha de personagem
		{
			putimage(character_choice[0].x1, character_choice[0].y1, MhudCharacter[1], AND_PUT);
	    	putimage(character_choice[0].x1, character_choice[0].y1, hudCharacter[1], OR_PUT);
		}
		else
		{
			putimage(character_choice[0].x1, character_choice[0].y1, MhudCharacter[0], AND_PUT);
	    	putimage(character_choice[0].x1, character_choice[0].y1, hudCharacter[0], OR_PUT);
		}
		
		// Obs a escolha de personagem é printada antes da seta de seleção para que não haja sobreposição
		
	    for (i=0; i < 4; i += 1) //Checa a colisão da escolha de personagem na posição desejada
	    {
	      if ((character_choice[i].x1 <= mx && mx <= character_choice[i].x2) && (character_choice[i].y1 <= my && my <= character_choice[i].y2)) 
	      //Se colidiu, faz um retangulo vermelho aonde foi clicado
	      {
	      	choice = i + 1;
		  	if (canSound != i+2){sndPlaySound(".\\audio\\click.wav", SND_ASYNC); canSound = i+2;}
		  	putimage(character_choice[i].x1, character_choice[i].y1 - (yscreen/45), Mind[1], AND_PUT);
	    	putimage(character_choice[i].x1, character_choice[i].y1 - (yscreen/45), ind[1], OR_PUT);
		  	//setfillstyle(1,RED);
		  }
		  
	      else if ((character_choice[i].x1 <= mousePos.x && mousePos.x <= character_choice[i].x2) && (character_choice[i].y1 <= mousePos.y && mousePos.y <= character_choice[i].y2))
	      // Se o mouse está por cima faz um retangulo branco
		  {
	      	if (canSound != i+1){sndPlaySound(".\\audio\\select.wav", SND_ASYNC); canSound = i+1;}
	    	putimage(character_choice[i].x1, character_choice[i].y1 - (yscreen/40), Mind[0], AND_PUT);
	    	putimage(character_choice[i].x1, character_choice[i].y1 - (yscreen/40), ind[0], OR_PUT);
		  	//setfillstyle(1,WHITE);
		  }
		  						 
		  else sound++;
		    /*
		  	//Printa os quadrados de HitBox da escolha de personagens
	  	    rectangle(character_choice[i].x1, character_choice[i].y1, character_choice[i].x2, character_choice[i].y2);
	  	    */
	    }
	    
	    if (sound == 4) canSound=0;//Zera a checagem de som
	    sound=0;
	    
	    ///////////Leonidas
		if (shift.someone == 0 && leonidas.canAtc == 1 && atcNow == 0 &&
		(leonidasMap.x1 <= mx && mx <= leonidasMap.x2) && (leonidasMap.y1 <= my && my <= leonidasMap.y2))
		{
			atcWho = 6;
		    atcNow = 1;
			leonidasAtc = 1;
		    atcDamage = leonidas.damage;
		    atcRange = leonidas.range;
			atcI = 0;
			atcJ = 2;
		}
		else if (shift.someone == 1 && atcNow == 1 && 
		((-1 <= atcI + atcRange && -1 >= atcI - atcRange && 2 == atcJ) || 
	    (0== atcI && 2 <= atcJ + (atcRange/2) && 2 >= atcJ - (atcRange/2) )) &&
		(leonidasMap.x1 <= mx && mx <= leonidasMap.x2) && (leonidasMap.y1 <= my && my <= leonidasMap.y2))
		{
			sndPlaySound(".\\audio\\battle.wav", SND_ASYNC);
			if (leonidas.life < atcDamage) leonidas.life = 0;
	      	else leonidas.life -= atcDamage;
	      	leonidas.hitted = 1;
	      	atcNow = 0;
		}
		else if (shift.someone == 1 && leonidas.canAtc == 1 && moveNow == 1 &&
		atcI == 0 && (atcJ == 4 || atcJ == 0) &&
		(leonidasMap.x1 <= mxR && mxR <= leonidasMap.x2) && (leonidasMap.y1 <= myR && myR <= leonidasMap.y2))
		{
	      	endGame = 2;
		}
		else if((leonidasMap.x1 <= mousePos.x && mousePos.x <= leonidasMap.x2) && 
		(leonidasMap.y1 <= mousePos.y && mousePos.y <= leonidasMap.y2))
		{
			viewLife = 1;
			viewLL = 1;
			whoLife = 0;
		}
		else {viewLL = 0;}
		
	    
	    if ((shift.x1 <= mx && mx <= shift.x2) && (shift.y1 <= my && my <= shift.y2))//Checa a colisão do botão Shift
	      {
		  	putimage(shift.x1, shift.y1, shiftBut[1], COPY_PUT);
		  	mx=0;
		    my=0;
		    mxR=0;
		    myR=0;
		    atcNow = 0;
		    moveNow = 0;
		    if (shift.someone== 0)
			{
				 shift.someone= 1; 
				 points[shift.someone] += 80;
			 }
		
			else
			{
				 shift.someone= 0; 
				 points[shift.someone] += 50;
			};
		  
		    bout++;
		    
		    leonidas.canAtc= 1;
		    for(i=0; i < swordsMenNumber; i++)
		    {
		    	swordsMen[i].canMove = 1;
		    	swordsMen[i].canAtc = 1;
			}
			for(i=0; i < bowMenNumber; i++)
		    {
		    	bowMen[i].canMove = 1;
		    	bowMen[i].canAtc = 1;
			}
			for(i=0; i < spearMenNumber; i++)
		    {
		    	spearMen[i].canMove = 1;
		    	spearMen[i].canAtc = 1;
			}
			for(i=0; i < horseManNumber; i++)
		    {
		    	horseMan[i].canMove = 1;
		    	horseMan[i].canAtc = 1;
			}
		    
		  }
	    else if ((shift.x1 <= mousePos.x && mousePos.x <= shift.x2) && (shift.y1 <= mousePos.y && mousePos.y <= shift.y2))
	      // Se o mouse está por cima faz um retangulo branco
		  {
		  	mx=0;
		    my=0;
		  	putimage(shift.x1, shift.y1, shiftBut[1], COPY_PUT);
		  }
		else {putimage(shift.x1, shift.y1, shiftBut[0], COPY_PUT);}
		  
	    ////////////////////////////FIM DA CHECAGEM DE COLISÃO////////////////////////////
	    
	    
	    
	    
		////////////////////////////AQUI PRINTA TODOS OS PERSONAGENS E HUD////////////////////////////
		
		
		
		putimage(character_choice[3].x2 +  (xscreen /13), shift.y1 - (yscreen /18), Mface[shift.someone], AND_PUT);
	    putimage(character_choice[3].x2 +  (xscreen /13), shift.y1 - (yscreen /18), face[shift.someone], OR_PUT);
	    
	    sprintf(pointsChar, "%d", points[shift.someone]);
	   
		outtextxy(character_choice[3].x2 +  (xscreen /14),character_choice[3].y2 -  (yscreen /42), pointsChar);
	    
	    sprintf(pointsChar, "%d", bout);
		outtextxy(shift.x1 +  (xscreen /21), shift.y1 - (yscreen /24), pointsChar);
		
		if(atcNow ==1 && leonidasAtc == 0 && table[atcI][atcJ].who == 1){
	
		 if(shift.someone==1){
		   putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[0], COPY_PUT);
		   }
		 else{
		   putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI+1][atcJ].x1, table[atcI+1][atcJ].y1, square[0], COPY_PUT);
		   }
		}
		else if(atcNow ==1 && leonidasAtc == 0 && table[atcI][atcJ].who == 2){
	
		 if(shift.someone==1){
		   putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI-2][atcJ].x1, table[atcI-2][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI-3][atcJ].x1, table[atcI-3][atcJ].y1, square[0], COPY_PUT);
		   if(atcJ<4)
		   putimage(table[atcI][atcJ+1].x1, table[atcI][atcJ+1].y1, square[0], COPY_PUT);
		   else if(atcJ>0)
		   putimage(table[atcI][atcJ-1].x1, table[atcI][atcJ-1].y1, square[0], COPY_PUT);
		   }
		 else{
		   putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI+1][atcJ].x1, table[atcI+1][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI+2][atcJ].x1, table[atcI+2][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI+3][atcJ].x1, table[atcI+3][atcJ].y1, square[0], COPY_PUT);
		   if(atcJ<4)
		   putimage(table[atcI][atcJ+1].x1, table[atcI][atcJ+1].y1, square[0], COPY_PUT);
		   else if(atcJ>0)
		   putimage(table[atcI][atcJ-1].x1, table[atcI][atcJ-1].y1, square[0], COPY_PUT);
		   }
		}
		
		else if(atcNow ==1 && leonidasAtc == 0 && table[atcI][atcJ].who == 4){
	
		 if(shift.someone==1){
		   putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI-2][atcJ].x1, table[atcI-2][atcJ].y1, square[0], COPY_PUT);
		   if(atcJ<4)
		   putimage(table[atcI][atcJ+1].x1, table[atcI][atcJ+1].y1, square[0], COPY_PUT);
		   else if(atcJ>0)
		   putimage(table[atcI][atcJ-1].x1, table[atcI][atcJ-1].y1, square[0], COPY_PUT);
		   
		   }
		 else{
		   putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI+1][atcJ].x1, table[atcI+1][atcJ].y1, square[0], COPY_PUT);
		   putimage(table[atcI+2][atcJ].x1, table[atcI+2][atcJ].y1, square[0], COPY_PUT);
		   if(atcJ<4)
		   putimage(table[atcI][atcJ+1].x1, table[atcI][atcJ+1].y1, square[0], COPY_PUT);
		   else if(atcJ>0)
		   putimage(table[atcI][atcJ-1].x1, table[atcI][atcJ-1].y1, square[0], COPY_PUT);
		   
		   }
		}
		
		else if(atcNow ==1 && leonidasAtc == 0 && table[atcI][atcJ].who == 5){
	
	     putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[0], COPY_PUT);
		 putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[0], COPY_PUT);
		
		}
		
		if(moveNow ==1 && table[atcI][atcJ].who == 1){
		putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[1], COPY_PUT);
		putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[1], COPY_PUT);
	    }
		else if(moveNow ==1 && table[atcI][atcJ].who == 2){		
		putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[1], COPY_PUT);
		putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[1], COPY_PUT);	
		}

		else if(moveNow ==1 && table[atcI][atcJ].who == 4){		
		putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[1], COPY_PUT);
		putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[1], COPY_PUT);
		}
		
		else if (moveNow ==1 && table[atcI][atcJ].who == 5){
		putimage(table[atcI][atcJ].x1, table[atcI][atcJ].y1, square[1], COPY_PUT);
		putimage(table[atcI-1][atcJ].x1, table[atcI-1][atcJ].y1, square[1], COPY_PUT);
		putimage(table[atcI-2][atcJ].x1, table[atcI-2][atcJ].y1, square[1], COPY_PUT);
		}
		
	    
	    
		//Printa os Personagens, mas aproveita o for usado para checar se eles morreram
		printNumber = swordsMenNumber + bowMenNumber+ spearMenNumber + schieldsMenNumber + horseManNumber;
		
		for (printY = 0; printY < 5; printY++)
		{
			for (printX = 0; printX < 8; printX++)
			{
				
				if(table[printX][printY].someone == 1)//Se houver alguem naquele lugar procura o personagem para ser printado
				{
							//////SWORDSMEN//////
				    for(i=0; i < swordsMenNumber; i++)//For que roda todos os Swordsman
				    {
				    	if(swordsMen[i].mapx == printX && swordsMen[i].mapy == printY)
				    	{
					    	if (swordsMen[i].life >= 1 || swordsMen[i].hitted != 0)
							{
						    	if (swordsMen[i].player == 0 )//Se for o player zero printa os spartanos, se for um printa os persas;
						    	{
						    		if (swordsMen[i].atc == 0)//Um if pra checar o ataque, pois há outra animação para o ataque
						    		{
							    		putimage(swordsMen[i].px, swordsMen[i].py, MspriteSwordsMenS[swordsMen[i].frame/animation], AND_PUT);
							    		putimage(swordsMen[i].px, swordsMen[i].py, spriteSwordsMenS[swordsMen[i].frame/animation], OR_PUT);
									}
									else
									{
										putimage(swordsMen[i].px, swordsMen[i].py, MspriteSwordsMenAS[swordsMen[i].frame/animation], AND_PUT);
							    		putimage(swordsMen[i].px, swordsMen[i].py, spriteSwordsMenAS[swordsMen[i].frame/animation], OR_PUT);
									}
									
									if (swordsMen[i].hitted == 1)
									{
										putimage(swordsMen[i].px, swordsMen[i].py, MbloodR[swordsMen[i].bloodFrame/animation], AND_PUT);
							    		putimage(swordsMen[i].px, swordsMen[i].py, bloodR[swordsMen[i].bloodFrame/animation], OR_PUT);
							    		if (swordsMen[i].bloodFrame >= (animation*3) -1){swordsMen[i].hitted = 0; swordsMen[i].bloodFrame = -1;}
							    		swordsMen[i].bloodFrame++;
									}
								}
						    	else
						    	{
						    		if (swordsMen[i].atc == 0)
						    		{
							    		putimage(swordsMen[i].px, swordsMen[i].py, MspriteSwordsMenP[swordsMen[i].frame/animation], AND_PUT);
							    		putimage(swordsMen[i].px, swordsMen[i].py, spriteSwordsMenP[swordsMen[i].frame/animation], OR_PUT);
									}
									else
									{
										putimage(swordsMen[i].px, swordsMen[i].py, MspriteSwordsMenAP[swordsMen[i].frame/animation], AND_PUT);
							    		putimage(swordsMen[i].px, swordsMen[i].py, spriteSwordsMenAP[swordsMen[i].frame/animation], OR_PUT);
									}
									if (swordsMen[i].hitted == 1)
									{
										putimage(swordsMen[i].px, swordsMen[i].py, MbloodR[swordsMen[i].bloodFrame/animation], AND_PUT);
							    		putimage(swordsMen[i].px, swordsMen[i].py, bloodR[swordsMen[i].bloodFrame/animation], OR_PUT);
							    		if (swordsMen[i].bloodFrame >= (animation*3) -1){swordsMen[i].hitted = 0; swordsMen[i].bloodFrame = -1;}
							    		swordsMen[i].bloodFrame++;
									}
								}
								if (swordsMen[i].atc == 0)//Atualiza o frame de animação
						        	if (swordsMen[i].frame < (animation*2) -1) swordsMen[i].frame++; else swordsMen[i].frame = 0;
								else
									if (swordsMen[i].frame < (animation*3) -1) swordsMen[i].frame++; else {swordsMen[i].frame = 0; swordsMen[i].atc = 0;}
							}
							else			//Caso o life seja igual a zero
							{
								
								table[swordsMen[i].mapx][swordsMen[i].mapy].someone = 0;//Zera o someone para que outro personagem possa sobrescreve-lo
								table[swordsMen[i].mapx][swordsMen[i].mapy].who = 0;
								
								//Atualiza o WhoNum
								table[swordsMen[swordsMenNumber - 1].mapx][swordsMen[swordsMenNumber - 1].mapy].whoNum = table[swordsMen[i].mapx][swordsMen[i].mapy].whoNum;
								
								//Joga todas as várias do último do vetor em cima do que será deletado
								swordsMen[i].px = swordsMen[swordsMenNumber - 1].px;
								swordsMen[i].py = swordsMen[swordsMenNumber - 1].py;
								swordsMen[i].mapx = swordsMen[swordsMenNumber - 1].mapx;
								swordsMen[i].mapy = swordsMen[swordsMenNumber - 1].mapy;
								swordsMen[i].frame = swordsMen[swordsMenNumber - 1].frame;
								swordsMen[i].player = swordsMen[swordsMenNumber - 1].player;
								
								swordsMen[i].hitted = swordsMen[swordsMenNumber - 1].hitted;
								swordsMen[i].bloodFrame = swordsMen[swordsMenNumber - 1].bloodFrame;
								
								swordsMen[i].canAtc = swordsMen[swordsMenNumber - 1].canAtc;
								swordsMen[i].canMove = swordsMen[swordsMenNumber - 1].canMove;
								
								swordsMen[i].atc = swordsMen[swordsMenNumber - 1].atc;
								swordsMen[i].life = swordsMen[swordsMenNumber - 1].life;
								swordsMen[i].moviment = swordsMen[swordsMenNumber - 1].moviment;
								swordsMen[i].range = swordsMen[swordsMenNumber - 1].range;
								swordsMen[i].damage = swordsMen[swordsMenNumber - 1].damage;
								
								//Roda o realloc para o delet
								swordsMenNumber--;
								swordsMen = (struct character *)realloc(swordsMen, sizeof(struct character) * swordsMenNumber);
								
							}	
							break;
						}
					}
					
										//////BOWMEN//////
					for(i=0; i < bowMenNumber; i++)	
				    {
				    	if(bowMen[i].mapx == printX && bowMen[i].mapy == printY)
						{
							
					    	if (bowMen[i].life >= 1  || bowMen[i].hitted != 0)
							{
						    	if (bowMen[i].player == 0)
						    	{
						    		if (bowMen[i].atc == 0)
						    		{
						    			putimage(bowMen[i].px, bowMen[i].py, MspriteBowMenS[bowMen[i].frame/animation], AND_PUT);
							    		putimage(bowMen[i].px, bowMen[i].py, spriteBowMenS[bowMen[i].frame/animation], OR_PUT);
									}
									else
						    		{
						    			putimage(bowMen[i].px, bowMen[i].py, MspriteBowMenAS[bowMen[i].frame/animation], AND_PUT);
							    		putimage(bowMen[i].px, bowMen[i].py, spriteBowMenAS[bowMen[i].frame/animation], OR_PUT);
									}
									if (bowMen[i].hitted == 1)
									{
										putimage(bowMen[i].px, bowMen[i].py, MbloodR[bowMen[i].bloodFrame/animation], AND_PUT);
							    		putimage(bowMen[i].px, bowMen[i].py, bloodR[bowMen[i].bloodFrame/animation], OR_PUT);
							    		if (bowMen[i].bloodFrame >= (animation*3) -1){bowMen[i].hitted = 0; bowMen[i].bloodFrame = -1;}
							    		bowMen[i].bloodFrame++;
									}
							    	
						    	}
						    	else
						    	{
						    		if (bowMen[i].atc == 0)
						    		{
						    			putimage(bowMen[i].px, bowMen[i].py, MspriteBowMenP[bowMen[i].frame/animation], AND_PUT);
							    		putimage(bowMen[i].px, bowMen[i].py, spriteBowMenP[bowMen[i].frame/animation], OR_PUT);
									}
							    	else
						    		{
						    			putimage(bowMen[i].px, bowMen[i].py, MspriteBowMenAP[bowMen[i].frame/animation], AND_PUT);
							    		putimage(bowMen[i].px, bowMen[i].py, spriteBowMenAP[bowMen[i].frame/animation], OR_PUT);
									}
									
									if (bowMen[i].hitted == 1)
									{
										putimage(bowMen[i].px, bowMen[i].py, MbloodR[bowMen[i].bloodFrame/animation], AND_PUT);
							    		putimage(bowMen[i].px, bowMen[i].py, bloodR[bowMen[i].bloodFrame/animation], OR_PUT);
							    		if (bowMen[i].bloodFrame >= (animation*3) -1){bowMen[i].hitted = 0; bowMen[i].bloodFrame = -1;}
							    		bowMen[i].bloodFrame++;
									}
								}
								if (bowMen[i].atc == 0)				//Atualiza o frame de animação
						        	if (bowMen[i].frame < (animation*2) -1) bowMen[i].frame++; else bowMen[i].frame = 0;
								else
									if (bowMen[i].frame < (animation*4) -1) bowMen[i].frame++; else {bowMen[i].frame = 0; bowMen[i].atc = 0;}
							}
							else
							{
								table[bowMen[i].mapx][bowMen[i].mapy].someone = 0;
						  		table[bowMen[i].mapx][bowMen[i].mapy].who = 0;
						  		
								table[bowMen[bowMenNumber - 1].mapx][bowMen[bowMenNumber - 1].mapy].whoNum = table[bowMen[i].mapx][bowMen[i].mapy].whoNum;
								
								bowMen[i].px = bowMen[bowMenNumber - 1].px;
							    bowMen[i].py = bowMen[bowMenNumber - 1].py;
							    bowMen[i].mapx = bowMen[bowMenNumber - 1].mapx;
							    bowMen[i].mapy = bowMen[bowMenNumber - 1].mapy;
								bowMen[i].frame = bowMen[bowMenNumber - 1].frame;
								bowMen[i].player = bowMen[bowMenNumber - 1].player;
								
								bowMen[i].hitted = bowMen[bowMenNumber - 1].hitted;
								bowMen[i].bloodFrame = bowMen[bowMenNumber - 1].bloodFrame;
								
								bowMen[i].canAtc = bowMen[bowMenNumber - 1].canAtc;
								bowMen[i].canMove = bowMen[bowMenNumber - 1].canMove;
								
								bowMen[i].atc = bowMen[bowMenNumber - 1].atc;
								bowMen[i].life = bowMen[bowMenNumber - 1].life;
								bowMen[i].moviment = bowMen[bowMenNumber - 1].moviment;
								bowMen[i].range = bowMen[bowMenNumber - 1].range;
								bowMen[i].damage = bowMen[bowMenNumber - 1].damage;
								
								
								bowMenNumber--;
								bowMen = (struct character *)realloc(bowMen, sizeof(struct character) * bowMenNumber);
							}
							break;
						}
					}
					
										//////SPEARMEN	//////
				    for(i=0; i < spearMenNumber; i++)
				    {
				    	if(spearMen[i].mapx == printX && spearMen[i].mapy == printY)
						{
							
					    	if (spearMen[i].life >= 1 || spearMen[i].hitted != 0)
							{
						    	if (spearMen[i].player == 0)
						    	{
							    	if (spearMen[i].atc == 0)
							    	{
							    		putimage(spearMen[i].px, spearMen[i].py, MspriteSpearMenS[spearMen[i].frame/animation], AND_PUT);
							    		putimage(spearMen[i].px, spearMen[i].py, spriteSpearMenS[spearMen[i].frame/animation], OR_PUT);
									}
									else
									{
										putimage(spearMen[i].px, spearMen[i].py, MspriteSpearMenAS[spearMen[i].frame/animation], AND_PUT);
							    		putimage(spearMen[i].px, spearMen[i].py, spriteSpearMenAS[spearMen[i].frame/animation], OR_PUT);
									}
									
									if (spearMen[i].hitted == 1)
									{
										putimage(spearMen[i].px, spearMen[i].py, MbloodR[spearMen[i].bloodFrame/animation], AND_PUT);
								    	putimage(spearMen[i].px, spearMen[i].py, bloodR[spearMen[i].bloodFrame/animation], OR_PUT);
								    	if (spearMen[i].bloodFrame >= (animation*3) -1){spearMen[i].hitted = 0; spearMen[i].bloodFrame = -1;}
								    	spearMen[i].bloodFrame++;
									}
								}
						    	else
						    	{
						    		if (spearMen[i].atc == 0)
							    	{
							    		putimage(spearMen[i].px, spearMen[i].py, MspriteSpearMenP[spearMen[i].frame/animation], AND_PUT);
							    		putimage(spearMen[i].px, spearMen[i].py, spriteSpearMenP[spearMen[i].frame/animation], OR_PUT);
									}
									else
							    	{
							    		putimage(spearMen[i].px, spearMen[i].py, MspriteSpearMenAP[spearMen[i].frame/animation], AND_PUT);
							    		putimage(spearMen[i].px, spearMen[i].py, spriteSpearMenAP[spearMen[i].frame/animation], OR_PUT);
									}	
									if (spearMen[i].hitted == 1)
									{
										putimage(spearMen[i].px + (xscreen/20), spearMen[i].py, MbloodR[spearMen[i].bloodFrame/animation], AND_PUT);
								    	putimage(spearMen[i].px + (xscreen/20), spearMen[i].py, bloodR[spearMen[i].bloodFrame/animation], OR_PUT);
								    	if (spearMen[i].bloodFrame >= (animation*3) -1){spearMen[i].hitted = 0; spearMen[i].bloodFrame = -1;}
								    	spearMen[i].bloodFrame++;
									}
								}
								
						        if (spearMen[i].atc == 0)			//Atualiza o frame de animação
						        	if (spearMen[i].frame < (animation*2) -1) spearMen[i].frame++; else spearMen[i].frame = 0;
								else
									if (spearMen[i].frame < (animation*3) -1) spearMen[i].frame++; else {spearMen[i].frame = 0; spearMen[i].atc = 0;}
							}
							else
							{
								table[spearMen[i].mapx][spearMen[i].mapy].someone = 0;
						  		table[spearMen[i].mapx][spearMen[i].mapy].who = 0;
						  		
								table[spearMen[spearMenNumber - 1].mapx][spearMen[spearMenNumber - 1].mapy].whoNum = table[spearMen[i].mapx][spearMen[i].mapy].whoNum;
								
								spearMen[i].px = spearMen[spearMenNumber - 1].px;
							    spearMen[i].py = spearMen[spearMenNumber - 1].py;
							    spearMen[i].mapx = spearMen[spearMenNumber - 1].mapx;
							    spearMen[i].mapy = spearMen[spearMenNumber - 1].mapy;
								spearMen[i].frame = spearMen[spearMenNumber - 1].frame;
								spearMen[i].player = spearMen[spearMenNumber - 1].player;
								
								spearMen[i].hitted = spearMen[spearMenNumber - 1].hitted;
								spearMen[i].bloodFrame = spearMen[spearMenNumber - 1].bloodFrame;
								
								spearMen[i].canAtc = spearMen[spearMenNumber - 1].canAtc;
								spearMen[i].canMove = spearMen[spearMenNumber - 1].canMove;
								
								spearMen[i].atc = spearMen[spearMenNumber - 1].atc;
								spearMen[i].life = spearMen[spearMenNumber - 1].life;
								spearMen[i].moviment = spearMen[spearMenNumber - 1].moviment;
								spearMen[i].range = spearMen[spearMenNumber - 1].range;
								spearMen[i].damage = spearMen[spearMenNumber - 1].damage;
								
								
								spearMenNumber--;
								spearMen = (struct character *)realloc(spearMen, sizeof(struct character) * spearMenNumber);
							}
						break;
							
						}
					}
					
										//////SCHIEDSMEN//////
					for(i=0; i < schieldsMenNumber; i++)	//Printa os SchildsMen
				    {
				    	if(schieldsMen[i].mapx == printX && schieldsMen[i].mapy == printY)
						{
							
					    	if (schieldsMen[i].life >= 1 || schieldsMen[i].hitted == 1)
							{
						    	putimage(schieldsMen[i].px, schieldsMen[i].py, MspriteShieldsMenS[schieldsMen[i].frame/animation], AND_PUT);
						    	putimage(schieldsMen[i].px, schieldsMen[i].py, spriteShieldsMenS[schieldsMen[i].frame/animation], OR_PUT);
						        if (schieldsMen[i].hitted == 1)
								{
									putimage(schieldsMen[i].px, schieldsMen[i].py, MbloodR[schieldsMen[i].bloodFrame/animation], AND_PUT);
								    putimage(schieldsMen[i].px, schieldsMen[i].py, bloodR[schieldsMen[i].bloodFrame/animation], OR_PUT);
								    if (schieldsMen[i].bloodFrame >= (animation*3) -1){schieldsMen[i].hitted = 0; schieldsMen[i].bloodFrame = -1;}
								    schieldsMen[i].bloodFrame++;
								}
						        if (schieldsMen[i].frame < (animation*2)-1) schieldsMen[i].frame++; else schieldsMen[i].frame = 0;	//Atualiza o frame de animação
					    	}
					    	else
							{
								table[schieldsMen[i].mapx][schieldsMen[i].mapy].someone = 0;
						  		table[schieldsMen[i].mapx][schieldsMen[i].mapy].who = 0;
						  		
								table[schieldsMen[schieldsMenNumber - 1].mapx][schieldsMen[schieldsMenNumber - 1].mapy].whoNum = table[schieldsMen[i].mapx][schieldsMen[i].mapy].whoNum;
								
								schieldsMen[i].px = schieldsMen[schieldsMenNumber - 1].px;
							    schieldsMen[i].py = schieldsMen[schieldsMenNumber - 1].py;
							    schieldsMen[i].mapx = schieldsMen[schieldsMenNumber - 1].mapx;
							    schieldsMen[i].mapy = schieldsMen[schieldsMenNumber - 1].mapy;
								schieldsMen[i].frame = schieldsMen[schieldsMenNumber - 1].frame;
								schieldsMen[i].player = schieldsMen[schieldsMenNumber - 1].player;
								
								schieldsMen[i].hitted = schieldsMen[schieldsMenNumber - 1].hitted;
								schieldsMen[i].bloodFrame = schieldsMen[schieldsMenNumber - 1].bloodFrame;
								
								schieldsMen[i].canAtc = schieldsMen[schieldsMenNumber - 1].canAtc;
								schieldsMen[i].canMove = schieldsMen[schieldsMenNumber - 1].canMove;
								
								schieldsMen[i].atc = schieldsMen[schieldsMenNumber - 1].atc;
								schieldsMen[i].life = schieldsMen[schieldsMenNumber - 1].life;
								schieldsMen[i].moviment = schieldsMen[schieldsMenNumber - 1].moviment;
								schieldsMen[i].range = schieldsMen[schieldsMenNumber - 1].range;
								schieldsMen[i].damage = schieldsMen[schieldsMenNumber - 1].damage;
								
								
								schieldsMenNumber--;
								schieldsMen = (struct character *)realloc(schieldsMen, sizeof(struct character) * schieldsMenNumber);
							}
						break;
						}
					}
					
										//////HORSEMAN//////
					for(i=0; i < horseManNumber; i++)
				    {
				    	if(horseMan[i].mapx == printX && horseMan[i].mapy == printY)
				    	{
				    		
					    	if (horseMan[i].life >= 1 || horseMan[i].hitted != 0)
							{
						    	if(horseMan[i].atc == 0)
						    	{
						    		putimage(horseMan[i].px, horseMan[i].py, MspriteHorseManP[horseMan[i].frame/animation], AND_PUT);
						    		putimage(horseMan[i].px, horseMan[i].py, spriteHorseManP[horseMan[i].frame/animation], OR_PUT);
								}
						    	else
						    	{
						    		putimage(horseMan[i].px, horseMan[i].py, MspriteHorseManAP[horseMan[i].frame/animation], AND_PUT);
						    		putimage(horseMan[i].px, horseMan[i].py, spriteHorseManAP[horseMan[i].frame/animation], OR_PUT);
								}
								if (horseMan[i].hitted == 1)
								{
									putimage(horseMan[i].px, horseMan[i].py, MbloodR[horseMan[i].bloodFrame/animation], AND_PUT);
								    putimage(horseMan[i].px, horseMan[i].py, bloodR[horseMan[i].bloodFrame/animation], OR_PUT);
								    if (horseMan[i].bloodFrame >= (animation*3) -1){horseMan[i].hitted = 0; horseMan[i].bloodFrame = -1;}
								    horseMan[i].bloodFrame++;
								}
								
						        if (horseMan[i].atc == 0)			//Atualiza o frame de animação
						        	if (horseMan[i].frame < (animation*2) -1) horseMan[i].frame++; else horseMan[i].frame = 0;
								else
									if (horseMan[i].frame < (animation*3) -1) horseMan[i].frame++; else {horseMan[i].frame = 0; horseMan[i].atc = 0;}
							}
							else
							{
								table[horseMan[i].mapx][horseMan[i].mapy].someone = 0;
						  		table[horseMan[i].mapx][horseMan[i].mapy].who = 0;
						  		
								table[horseMan[horseManNumber - 1].mapx][horseMan[horseManNumber - 1].mapy].whoNum = table[horseMan[i].mapx][horseMan[i].mapy].whoNum;
								
								horseMan[i].px = horseMan[horseManNumber - 1].px;
							    horseMan[i].py = horseMan[horseManNumber - 1].py;
							    horseMan[i].mapx = horseMan[horseManNumber - 1].mapx;
							    horseMan[i].mapy = horseMan[horseManNumber - 1].mapy;
								horseMan[i].frame = horseMan[horseManNumber - 1].frame;
								horseMan[i].player = horseMan[horseManNumber - 1].player;
								
								horseMan[i].hitted = horseMan[horseManNumber - 1].hitted;
								horseMan[i].bloodFrame = horseMan[horseManNumber - 1].bloodFrame;
								
								horseMan[i].canAtc = horseMan[horseManNumber - 1].canAtc;
								horseMan[i].canMove = horseMan[horseManNumber - 1].canMove;
								
								horseMan[i].atc = horseMan[horseManNumber - 1].atc;
								horseMan[i].life = horseMan[horseManNumber - 1].life;
								horseMan[i].moviment = horseMan[horseManNumber - 1].moviment;
								horseMan[i].range = horseMan[horseManNumber - 1].range;
								horseMan[i].damage = horseMan[horseManNumber - 1].damage;
								
								horseManNumber--;
								horseMan = (struct character *)realloc(horseMan, sizeof(struct character) * horseManNumber);
							}
							
						break;
						}
					}
					
					
				}
					
			}
		}
		
				
		
		
		
	    
		
		
					
	    
	    
	    if (leonidas.atc == 0)//Um if pra checar o ataque, pois há outra animação para o ataque
		{
		   putimage(leonidas.px + (xscreen/70), leonidas.py - (yscreen/40), MspriteLeonidas[leonidas.frame/animation], AND_PUT);
		   putimage(leonidas.px + (xscreen/70), leonidas.py - (yscreen/40), spriteLeonidas[leonidas.frame/animation], OR_PUT);
		}
		else
		{
		    putimage(leonidas.px + (xscreen/70),  leonidas.py - (yscreen/40), MspriteLeonidasA[leonidas.frame/animation], AND_PUT);
		    putimage(leonidas.px + (xscreen/70), leonidas.py - (yscreen/40), spriteLeonidasA[leonidas.frame/animation], OR_PUT);
		}
		if (leonidas.hitted == 1)
		{
			putimage(leonidas.px, leonidas.py, MbloodR[leonidas.bloodFrame/animation], AND_PUT);
			putimage(leonidas.px, leonidas.py, bloodR[leonidas.bloodFrame/animation], OR_PUT);
			if (leonidas.bloodFrame >= (animation*3) -1){leonidas.hitted = 0; leonidas.bloodFrame = -1;}
			leonidas.bloodFrame++;
		}
	    if (leonidas.atc == 0)			//Atualiza o frame de animação
		    if (leonidas.frame < (animation*2) -1) leonidas.frame++; else leonidas.frame = 0;
		else
			if (leonidas.frame < (animation*3) -1) leonidas.frame++; else {leonidas.frame = 0; leonidas.atc = 0; leonidasAtc = 0;}
	    
	    setfillstyle(1,GREEN);
	    if(viewLife == 1)
	    {
	    	
			if (viewLL == 1)
		      	{
		      		putimage(leonidasMap.x1, leonidasMap.y2 - (yscreen/40), hpBar[0], COPY_PUT);
		      		
		      		bar(leonidasMap.x1, leonidasMap.y2 - (yscreen/40), 
					   (leonidasMap.x2 - leonidasMap.x1)*
					   ((leonidas.life * 1.0)/12) + leonidasMap.x1,
					    leonidasMap.y2);
		      		
		      		putimage(leonidasMap.x1, leonidasMap.y2 - (yscreen/40), hpBar[1], AND_PUT);
			    	putimage(leonidasMap.x1, leonidasMap.y2 - (yscreen/40), hpBar[2], OR_PUT);
		      	}
	    	else if (table[iLife][jLife].who == 1)
		      	{
		      		putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[0], COPY_PUT);
			    	
		      	    bar(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), 
					   (table[iLife][jLife].x2 - table[iLife][jLife].x1)*
					   ((swordsMen[table[iLife][jLife].whoNum].life * 1.0)/smHP) + table[iLife][jLife].x1,
					    table[iLife][jLife].y2);
					    
				    putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[1], AND_PUT);
			    	putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[2], OR_PUT);
				}
			else if (table[iLife][jLife].who == 2)
		      	{
		      		putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[0], COPY_PUT);
		      		
		      	    bar(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), 
					   (table[iLife][jLife].x2 - table[iLife][jLife].x1)*
					   ((bowMen[table[iLife][jLife].whoNum].life * 1.0)/bmHP) + table[iLife][jLife].x1,
					    table[iLife][jLife].y2);
					
				    putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[1], AND_PUT);
			    	putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[2], OR_PUT);
				}
			else if (table[iLife][jLife].who == 3)
		      	{
		      		putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[0], COPY_PUT);
		      		
		      		bar(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), 
					   (table[iLife][jLife].x2 - table[iLife][jLife].x1)*
					   ((schieldsMen[table[iLife][jLife].whoNum].life * 1.0)/shHP) + table[iLife][jLife].x1,
					    table[iLife][jLife].y2);
					
				    putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[1], AND_PUT);
			    	putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[2], OR_PUT);
				}
			else if (table[iLife][jLife].who == 4)
		      	{
		      		putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[0], COPY_PUT);
		      		
		      		bar(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), 
					   (table[iLife][jLife].x2 - table[iLife][jLife].x1)*
					   ((spearMen[table[iLife][jLife].whoNum].life * 1.0)/spHP) + table[iLife][jLife].x1,
					    table[iLife][jLife].y2);
					
					
				    putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[1], AND_PUT);
			    	putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[2], OR_PUT);
				}
			else if (table[iLife][jLife].who == 5)
		      	{
		      		putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[0], COPY_PUT);
		      		
		      		bar(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), 
					   (table[iLife][jLife].x2 - table[iLife][jLife].x1)*
					   ((horseMan[table[iLife][jLife].whoNum].life * 1.0)/hmHP) + table[iLife][jLife].x1,
					    table[iLife][jLife].y2);
					
					
				    putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[1], AND_PUT);
			    	putimage(table[iLife][jLife].x1, table[iLife][jLife].y2 - (yscreen/40), hpBar[2], OR_PUT);
				}
  			
		}
		
	    ///FIM DOS PRINTIS DE PERSONAGENS///
	    
	    //bar(mousePos.x-3, mousePos.y-3, mousePos.x+3, mousePos.y+3);//Checa printando um quadrado na posição do mouse
	    //rectangle(mxR-5, myR-5, mxR+5, myR+5);//Checa printando um quadrado na posição do mouse
	    
	    setvisualpage(pg);//Atualiza a pagina de visão
	    
	    
	    //CHECA SE O JOGO ACABOU//
	    if (endGame > 0)
	    {
	    	gameplay = 0;
		}
		else if (bout > 31)
		{
			gameplay = 0;
			endGame = 1;
		}
		else if (leonidas.life <= 0)
		{
			gameplay = 0;
			endGame = 2;
		}
	    
	    /////CHECA A ENTRADA DE COMANDOS/////
	    GetCursorPos(&mousePos);	//Pega a posição do Mouse  
	    ScreenToClient(screenHandle, &mousePos);  
	    
	    
	    //Se apertou com o botão esquerdo seleciona, direito apaga
	    if(GetKeyState(VK_LBUTTON) & 0X100)
	    {
	    	if(canShift == 1)
			{
		      canShift = 0;
		      mx = mousePos.x;
		      my = mousePos.y;
		      mxR = 0;
			  myR = 0;
			  moveNow = 0;
			}	
		}
		else if(GetKeyState(VK_RBUTTON) & 0X100){
			if(canShift == 1)
			{
			  canShift = 0;
			  mx = 0;
		      my = 0;
		      mxR = mousePos.x;
			  myR = mousePos.y;
		      atcNow = 0;
			  choice = 0;
			  leonidasAtc = 0;
			}
		}
		else canShift = 1;
		
		if(GetKeyState(0x43) & 0X80){gameplay = 0; endGame = 3;}
		  
	    
	    fflush(stdin);
	    if (kbhit())
		{
			tecla = getch();
			if (tecla == ESC) gameplay = 0;
		}
	    /////FIM DA CHECAGEM DA ENTRADA DE COMANDOS/////
	    
	    //printf("Turno:%d\n", int(shift.someone));
	    end = clock();						//Pega o Clock inicial do programa no início do código
	    average = end - start;  			//Faz a média dos Frames
	    //printf("FPS puro:%d\n", int(1000/average));
	    if(average<17)						//Se rodou o programa em mais de X milesegundos,
	    delay(17-average);					//se da um Delay de X - milesec.
	    									//Para 60 FPS --->17
	    									//Para 30 FPS --->33
	    									//Para 24 FPS --->42
	  }
	  
	  //Creditos
	  i=0;
	  while(endGame > 0)
	  {
	  		setbkcolor(RGB(0,0,0));
		  	start = clock();				//Pega o Clock inicial do programa no início do código
	  	
		    // Alterna a página de desenho.
		    if (pg == 1) pg = 2; else pg=1;
		      setactivepage(pg);
		    
		    // Limpa a área de desenho
		    cleardevice();
	    
	  	    setvisualpage(pg);
		  	if (endGame < 3) putimage(0, 0, victory[endGame - 1], COPY_PUT);
		    else if(i > -720) {putimage(0, i, credits, COPY_PUT);putimage(0, i + 720, credits2, COPY_PUT); i-=10;}
		    else { putimage(0, i + 720, credits2, COPY_PUT); }
		    
	        fflush(stdin);
		    if (kbhit())
			{
				tecla = getch();
				if (tecla == space) 
				{
					if (endGame < 3)endGame = 3;
					else endGame = 0;
				}
				if (tecla == ESC)
				{
					if (endGame < 3)endGame = 3;
					else endGame = 0;
				}
			}
			
			setvisualpage(pg);//Atualiza a pagina de visão
		    end = clock();						//Pega o Clock inicial do programa no início do código
		    average = end - start;  			//Faz a média dos Frames
		    //printf("FPS puro:%d\n", int(1000/average));
		    if(average<33)						//Se rodou o programa em mais de X milesegundos,
		    delay(33-average);					//se da um Delay de X - milesec.
		    									//Para 60 FPS --->17
		    									//Para 30 FPS --->33
		    									//Para 24 FPS --->42
	  }
  	
  }
  
  
  /////FECHA A TABELA GRÁFICA E LIBERA MEMÓRIA/////
  closegraph();
  
  free(logo); 
  free(bk); 
  free(mMImage);
  free(credits); 
  free(credits2); 
  free(swordsMen);
  free(bowMen);
  free(schieldsMen);
  free(spearMen);
  free(horseMan);
  free(optionsPaper);
  
  for(i=0; i < 2; i++){
  free(spriteSwordsMenS[i]);
  free(MspriteSwordsMenS[i]);
  free(victory[i]);
  free(paperStart[i]);
  free(MpaperStart[i]);
  free(paperOptions[i]);
  free(MpaperOptions[i]);
  free(paperExit[i]);
  free(MpaperExit[i]);
  free(square[i]);
  free(spriteSpearMenS[i]);
  free(MspriteSpearMenS[i]);
  free(spriteShieldsMenS[i]);
  free(MspriteShieldsMenS[i]);
  free(spriteBowMenS[i]);
  free(MspriteBowMenS[i]);
  free(spriteSwordsMenP[i]);
  free(MspriteSwordsMenP[i]);
  free(spriteSpearMenP[i]);
  free(MspriteSpearMenP[i]);
  free(spriteHorseManP[i]);
  free(MspriteHorseManP[i]);
  free(spriteBowMenP[i]);
  free(MspriteBowMenP[i]);
  free(spriteLeonidas[i]);
  free(MspriteLeonidas[i]);
  free(hudCharacter[i]);
  free(MhudCharacter[i]);
  free(ind[i]);
  free(Mind[i]);
  free(shiftBut[i]);
  free(face[i]);
  free(Mface[i]);
  }
  
  for(i=0; i < 3; i++){
  
  free(MspriteSwordsMenAS[i]);
  free(spriteSwordsMenAS[i]);
  free(hpBar[i]);
  free(spriteSpearMenAS[i]);
  free(MspriteSpearMenAS[i]);
  free(spriteSwordsMenAP[i]);
  free(MspriteSwordsMenAP[i]);
  free(spriteSpearMenAP[i]);
  free(MspriteSpearMenAP[i]);
  free(spriteHorseManAP[i]);
  free(MspriteHorseManAP[i]);
  free(spriteLeonidasA[i]);
  free(MspriteLeonidasA[i]);
  free(bloodL[i]);
  free(MbloodL[i]);
  free(bloodR[i]);
  free(MbloodR[i]);
  }
  
  for(i=0; i < 4; i++){
  free(spriteBowMenAS[i]);
  free(MspriteBowMenAS[i]);
  free(spriteBowMenAP[i]);
  free(MspriteBowMenAP[i]);
  }
  
  for(i=0; i < 7; i++){
    free(scene[i]);
  }	
  
  
  return(0);
}
