/*
    Copyright (C) 2005 Michael K. McCarty & Fritz Bronner

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
// Interplay's BUZZ ALDRIN's RACE into SPACE
//
// Formerly -=> LiftOff : Race to the Moon :: IBM version MCGA
// Copyright 1991 by Strategic Visions, Inc.
// Designed by Fritz Bronner
// Programmed by Michael K McCarty
//

#include "Buzz_inc.h"
#include "externs.h"

extern struct ManPool far *Men;
struct ManPool far *Sov;
char M_Us[106],M_Sv[106];
extern char Musics,tMusics,Sounds,Option;

void DrawPrefs(int where,char a1,char a2)
{
  int i,mode=0;
  FILE *fin;
  MouseOff();
  FadeOut(2,pal,10,0,0);
  strcpy(IDT,"i013");strcpy(IKEY,"K013");
  fin=sOpen("PREFS.BUT","rb",0);
  fread(&pal[0],768,1,fin);
  i=filelength(fileno(fin))-768;
  fread((char *)screen,i,1,fin);
  fclose(fin);
  RLED_img((char *)screen,vhptr.vptr,i,vhptr.w,vhptr.h);

  gxClearDisplay(0,0);
  ShBox(0,0,319,22);ShBox(0,24,89,199);
  ShBox(91,24,228,107);
  IOBox(98,28,137,63);IOBox(98,68,137,103);
  IOBox(144,28,221,63);IOBox(144,68,221,103);
  
  ShBox(91,109,228,199);InBox(95,113,224,195);RectFill(96,114,223,194,0);
  ShBox(230,24,319,199);

  if (where==2) where=mode=1; //modem klugge
   else if (where==3) where=mode=2; //play-by-mail

  if (where==0 || where==2) {
    PreLoadMusic(M_SOVTYP);
    IOBox(6,105,83,140);IOBox(6,158,83,193);
    IOBox(236,105,313,140);IOBox(236,158,313,193);
    InBox(6,52,83,87);InBox(236,52,313,87);
    RectFill(7,53,82,86,0);RectFill(237,53,312,86,0);
    OutBox(8,77,18,85);OutBox(238,77,248,85);
    //BinT(8,54,0);BinT(238,54,0);  // Old way with buttons
    BinT(8,54,1);BinT(238,54,1);    // No select Buttons
    RectFill(237,35,312,41,0);RectFill(7,35,82,41,0);
  } else {
    PreLoadMusic(M_DRUMSM);
    InBox(8,107,81,138);InBox(8,160,81,191);
    InBox(238,107,311,138);InBox(238,160,311,191);
	  InBox(8,77,18,85);InBox(238,77,248,85);
    BinT(8,54,1);BinT(238,54,1);
    RectFill(237,35,312,41,0);RectFill(7,35,82,41,0);
  };

  if (mode==0) DispBig(6,5,"PREFERENCES SELECTIONS",0,-1);
   else if (mode==2) DispBig(3,5,"PLAY BY MAIL SELECTIONS",0,-1);
    else DispBig(6,5,"MODEM GAME SELECTIONS",0,-1);

  IOBox(243,3,316,19);
  InBox(236,34,313,42);
  InBox(6,34,83,42);
  PLevels(0,Data->Def.Plr1);CLevels(0,a1);
  PLevels(1,Data->Def.Plr2);CLevels(1,a2);
  Levels(0,Data->Def.Lev1,1);
  Levels(0,Data->Def.Ast1,0);
  Levels(1,Data->Def.Lev2,1);
  Levels(1,Data->Def.Ast2,0);
  if (where==0 || where==2) grSetColor(9);
    else grSetColor(34);
  PrintAt(23,30,"PLAYER 1");
  grSetColor(34);PrintAt(253,30,"PLAYER 2");
  grSetColor(5);
  PrintAt(20,49,"COUNTRY");PrintAt(250,49,"COUNTRY");
  PrintAt(17,101,"GAME LEVEL");PrintAt(247,101,"GAME LEVEL");
  PrintAt(250,148,"ASTRONAUT");PrintAt(250,155,"SELECTION");
  PrintAt(20,148,"ASTRONAUT");PrintAt(20,155,"SELECTION");
  grSetColor(1);PrintAt(258,13,"CONTINUE");
  PrintAt(8,40,&Data->P[ Data->Def.Plr1 ].Name[0]);
  PrintAt(238,40,&Data->P[ Data->Def.Plr2 ].Name[0]);
  gxVirtualDisplay(&vhptr,153+34*(Data->Def.Music),0,101,31,134,60,0);
  gxVirtualDisplay(&vhptr,221+34*(Data->Def.Sound),0,101,71,134,100,0);

  gxVirtualDisplay(&vhptr,216,30,147,31,218,60,0);
  gxVirtualDisplay(&vhptr,72*(Data->Def.Anim),90,147,71,218,100,0);
  HModel(Data->Def.Input,1);

  MouseOn();
  if (where==0 || where==2) MouseOn();
  FadeIn(2,pal,10,0,0);
  PlayMusic(0);
  return;
}

void xRLED (void *src, void *dest, unsigned int src_size);

void HModel(char mode,char tx)
{
  unsigned int j;
  struct GO {
   ui16 size;
   long offset;
  } table;

  GXHEADER local;
  FILE *in;

  in=sOpen("PRFX.BUT","rb",0);
  fseek(in,(mode==0 || mode==1)*(sizeof table),SEEK_CUR);
  fread(&table,sizeof table,1,in);
  fseek(in,table.offset,SEEK_SET);
  GV(&local,127,80);
  fread(&pal[112*3],96*3,1,in);  // Individual Palette
  fread(buffer,table.size,1,in);  // Get Image
  fclose(in);
  RLED_img(buffer,local.vptr,table.size, local.w, local.h);
  for (j=0;j<gxVirtualSize(gxVGA_13,127,80);j++) local.vptr[j]+=112;
  MouseOff();
  RectFill(96,114,223,194,0);
  if (tx==0) gxSetDisplayPalette(pal);
  gxPutImage(&local,gxSET,97,115,0);
  DV(&local);
  grSetColor(11);
  if (mode==2 || mode==3) PrintAt(100,122,"HISTORICAL MODEL");
  else PrintAt(100,122,"BASIC MODEL");
  grSetColor(9);
  if (mode==0 || mode==2) PrintAt(100,128,"HISTORICAL ROSTER");
  else PrintAt(100,128,"CUSTOM ROSTER");
  MouseOn();
  return;
}


void Levels(char plr,char which,char x)
{
 unsigned char v[2][2]={9,239, 161,108};
 MouseOff();
 gxVirtualDisplay(&vhptr,0+which*72,30+x*30,v[0][plr],v[1][x],v[0][plr]+71,v[1][x]+29,0);
 MouseOn();
 return;
}

void BinT(int x,int y,char st)
{
  char sta[2][2]={2,4,4,2};
  MouseOff();
  grSetColor(sta[st][0]);
  grMoveTo(0+x,y+20);grLineTo(0+x,y+0);grLineTo(72+x,y+0);
  grMoveTo(12+x,y+21);grLineTo(12+x,y+30);
  grSetColor(sta[st][1]);
  grMoveTo(0+x,y+21);grLineTo(11+x,y+21);
  grMoveTo(12+x,y+31);grLineTo(73+x,y+31);grLineTo(73+x,y+0);
  MouseOn();
  return;
}

void PLevels(char side,char wh)
{
 MouseOff();
 if (side==0) {
   gxVirtualDisplay(&vhptr, 0+wh*72,     0,   9,  55,  20,  74,  0);
   gxVirtualDisplay(&vhptr, 0+wh*72+11,  0,  21,  55,  80,  84,  0);
 }
 else {
   gxVirtualDisplay(&vhptr, 0+wh*72,     0, 239,  55, 250,  74,  0);
   gxVirtualDisplay(&vhptr, 0+wh*72+11,  0, 250,  55, 310,  84,  0);
 }
 MouseOn();
 return;
}

void CLevels(char side,char wh)
{
 MouseOff();
 if (side==0)
   gxVirtualDisplay(&vhptr,144,wh*7,9,78,17,84,0);
 else gxVirtualDisplay(&vhptr,144,wh*7,239,78,247,84,0);
 MouseOn();
 return;
}


/* Pref Levels:
  Player Select : 0 = USA
		  1 = USSR
		  2 = USA AI
		  3 = USSR AI
		  4 = FOREIGN (maybe)
		  5 = FOREIGN AI (maybe)

       6 = USA (modem play)
       7 = USSR (modem play)


  Game Level:  0 = Easy
	       1 = Medium
	       2 = Hard
  Astro Level: 0 = Easy
	       1 = Meduim
	       2 = Hard
  Input:       0 = Mouse
	       1 = Keyboard
	       2 = Joystick (maybe)
  Anim Level:  0 = Full
	       1 = Partial
	       2 = Results Only
  Music Level: 0 = Full
	       1 = Partial
	       2 = None
  Sound FX:    0 = On
	       1 = Off
  */

void Prefs(int where)
{
int i,num,hum1=0,hum2=0;
FILE *fin;
char ch,Name[20],ksel=0;
GXHEADER local2;
long size;

 if (where!=3)
  {
   if (where==0)
    {
     plr[0]=0;plr[1]=1;
     Data->Def.Plr2=1;
     Data->Def.Plr1=0;
     hum1=0,hum2=1;
     Data->Def.Lev1=Data->Def.Ast1=Data->Def.Ast2=0;
     Data->Def.Lev2=2;  //start computer level 3
     Data->Def.Input=0;
     Data->Def.Sound=Data->Def.Music=1;
    }
   if (Data->Def.Plr1>1) {Data->Def.Plr1-=2;hum1=1;}
   if (Data->Def.Plr2>1) {Data->Def.Plr2-=2;hum2=1;}
  }

  Data->Def.Sound=Data->Def.Music=1;
  DrawPrefs(where,hum1,hum2);
  while(1)  { GetMouse();if (mousebuttons==0) break;}
  while (1)
  {
    key=0;GetMouse();
    if (mousebuttons > 0 || key>0)  /* Game Play */
    {
      if (((x>=245 && y>=5 && x<=314 && y<=17) || key==0x0d) && !(hum1==1 && hum2==1))
      {
	    MouseOff();InBox(245,5,314,17);MouseOn();
	    while(1)  { GetMouse();if (mousebuttons==0) break;}
       if (key>0) delay(150); 
	    MouseOff();OutBox(245,5,314,17);MouseOn();
	    if (Data->Def.Plr1!=Data->Def.Plr2)
         {
	       if (Data->Def.Plr1==1)
            {
	          strcpy(&Name[0],&Data->P[0].Name[0]);
	          strcpy(&Data->P[0].Name[0],&Data->P[1].Name[0]);
	          strcpy(&Data->P[1].Name[0],&Name[0]);
	          i=Data->Def.Lev1;Data->Def.Lev1=Data->Def.Lev2;Data->Def.Lev2=i;
	          i=Data->Def.Ast1;Data->Def.Ast1=Data->Def.Ast2;Data->Def.Ast2=i;
	         }
	       Data->Def.Plr1+=hum1*2;
	       Data->Def.Plr2+=hum2*2;
	       if (where==0 || where==3) FadeOut(2,pal,10,0,0);
          key=0;
          if ((where==0 || where==3) && (Data->Def.Input==2 || Data->Def.Input==3))
            {
	          fin=sOpen("HIST.DAT","rb",0);
	          fread(&Data->P[0].Probe[0],28*(sizeof (struct Equipment)),1,fin);
	          fread(&Data->P[1].Probe[0],28*(sizeof (struct Equipment)),1,fin);
	          fclose(fin);
	         }
	       if (Data->Def.Input==0 || Data->Def.Input==2)
           { // Hist Crews
	         fin=sOpen("CREW.DAT","rb",0);
            size=filelength(fileno(fin));
            fread(buffer,size,1,fin);
            fclose(fin);
	         fin=sOpen("MEN.DAT","wb",1);
            fwrite(buffer,size,1,fin);
            fclose(fin);
	        }
          else if (Data->Def.Input==1 || Data->Def.Input==3)
           { // User Crews
	         fin=sOpen("USER.DAT","rb",0); size=filelength(fileno(fin));
	         fread(buffer,size,1,fin);fclose(fin);
	         fin=sOpen("MEN.DAT","wb",1);fwrite(buffer,size,1,fin);fclose(fin);
	        }
         KillMusic();
         if (Data->Def.Music==0 && Musics!=0) 
          {i=tMusics;tMusics=Musics;Musics=i;}
         if (Data->Def.Music==1 && Musics==0) 
          {i=tMusics;tMusics=Musics;Musics=i;}
	       return;
	     };
      }
     else
      if (key=='P' && (where==0 || where==3))
        {
        RectFill(59,26,68,31,3);RectFill(290,26,298,31,3);
         if (ksel==0)
           {
            ksel=1;
            grSetColor(9);PrintAt(253,30,"PLAYER 2");
            grSetColor(34);PrintAt(23,30,"PLAYER 1");
            }
           else
            {
             ksel=0;
             grSetColor(34);PrintAt(253,30,"PLAYER 2");
             grSetColor(9);PrintAt(23,30,"PLAYER 1");
            }
        }
      else
      if ((x>=146 && y>=30 && x<=219 && y<=61 && mousebuttons>0) || key=='E')
       {
	     MouseOff();InBox(146,30,219,61);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
        if (key>0) delay(150);
	     MouseOff();OutBox(146,30,219,61);MouseOn();
	     GV(&local2,319,199);
	     MouseOff();
	     gxGetImage(&local2,0,0,319,199,0);
	     MouseOn();
        KillMusic();
        PreLoadMusic(M_FILLER);PlayMusic(0);
	     EditAst();
       strcpy(IDT,"i013");strcpy(IKEY,"K013");
        KillMusic();
        PreLoadMusic( (where==0 || where==3)? M_SOVTYP :M_DRUMSM);
        PlayMusic(0);
	     MouseOff();
	     gxPutImage(&local2,gxSET,0,0,0);
	     MouseOn();
	     DV(&local2);
	     FadeIn(2,pal,10,0,0); 
	     // Astronaut Changes
       }
      else
      if (((x>=96 && y>=114 && x<=223 && y<=194 && mousebuttons>0) || key==K_SPACE) && (where==3 || where==0))  // Hist
       {
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     Data->Def.Input++;
        if (Data->Def.Input>3) Data->Def.Input=0;
	     HModel(Data->Def.Input,0);
      }
      else
      if ((x>=146 && y>=70 && x<=219 && y<=101 && mousebuttons>0) || key=='A')
       {
	     MouseOff();InBox(146,70,219,101);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     Data->Def.Anim=abs(Data->Def.Anim-1);
	     MouseOff();gxVirtualDisplay(&vhptr,72*(Data->Def.Anim),90,147,71,218,100,0);
	     OutBox(146,70,219,101);MouseOn();
	     /* Anim Level */
       }
      else
      if ((x>=100 && y>=30 && x<=135 && y<=61 && mousebuttons>0) || key=='M')
       {
	     MouseOff();InBox(100,30,135,61);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
        Data->Def.Music=abs(Data->Def.Music-1);
        SetMusicVolume((Data->Def.Music==1)?100:0);
//        i=tMusics;tMusics=Musics;Musics=i;
        MouseOff();gxVirtualDisplay(&vhptr,153+34*(Data->Def.Music),0,101,31,134,60,0);
	     OutBox(100,30,135,61);MouseOn();
	     /* Music Level */
       }
      else
      if ((x>=100 && y>=70 && x<=135 && y<=101 && mousebuttons>0) || key=='S')
       {
	     MouseOff();InBox(100,70,135,101);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
        Data->Def.Sound=abs(Data->Def.Sound-1);
        MouseOff();gxVirtualDisplay(&vhptr,221+34*(Data->Def.Sound),0,101,71,134,100,0);
	     OutBox(100,70,135,101);MouseOn();
	     /* Sound Level */
       }

#if 0
      else
      if (((x>=8 && y>=54 && x<=19 && y<=75 && where==0) || (x>=20 && y>=54 && x<=81 && y<=85 && where==0)) 
           || (where==0 && ksel==0 && key=='C'))
       {
	     BinT(8,54,1);
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     Data->Def.Plr1++;
	     if (Data->Def.Plr1>1) Data->Def.Plr1=0;
	     PLevels(0,Data->Def.Plr1);BinT(8,54,0);
	     /* Player 1 Select */
       }
#endif

      else
      if ((x>=8 && y>=77 && x<=18 && y<=85 && where==0 && mousebuttons>0) ||
          (where==0 && ksel==0 && key=='H'))
       {
	     MouseOff();InBox(8,77,18,85);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     hum1++; if (hum1>1) hum1=0;
	     CLevels(0,hum1);
	     MouseOff();OutBox(8,77,18,85);MouseOn();
	     /* P1: Human/Computer */
       }
      else
      if ((x>=8 && y>=107 && x<=81 && y<=138 && (where==0 || where==3) && mousebuttons>0) ||
          ((where==3 || where==0) && ksel==0 && key=='G'))
       {
	     MouseOff();InBox(8,107,81,138);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     MouseOff();OutBox(8,107,81,138);MouseOn();
	     Data->Def.Lev1++;
	     if (Data->Def.Lev1>2) Data->Def.Lev1=0;
	     Levels(0,Data->Def.Lev1,1);
	     /* P1: Game Level */
       }
      else
      if ((x>=8 && y>=160 && x<=81 && y<=191 && ((where==0 || where==3) && mousebuttons>0) ||
          ((where==3 || where==0) && ksel==0 && key=='L')))
       {
	     MouseOff();InBox(8,160,81,191);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     MouseOff();OutBox(8,160,81,191);MouseOn();
	     Data->Def.Ast1++;
	     if (Data->Def.Ast1>2) Data->Def.Ast1=0;
	     Levels(0,Data->Def.Ast1,0);
	     /* P1: Astro Level */
       }
#if 0
      else
      if (((x>=238 && y>=54 && x<=249 && y<=75 && ((where==3 || where==0) && mousebuttons>0) || (x>=250 && y>=54 && x<=311 && y<=85 && where==0)) 
          || (where==0 && ksel==1 && key=='C'))
       {
	     BinT(238,54,1);
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     Data->Def.Plr2++;
	     if (Data->Def.Plr2>1) Data->Def.Plr2=0;
	     PLevels(1,Data->Def.Plr2);BinT(238,54,0);
	     /* Player 2 Select */
       }
#endif
      else
      if ((x>=238 && y>=77 && x<=248 && y<=85 && where==0 && mousebuttons>0) ||
          (where==0 && ksel==1 && key=='H'))
       {
	     MouseOff();InBox(238,77,248,85);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     hum2++; if (hum2>1) hum2=0;
	     CLevels(1,hum2);
	     MouseOff();OutBox(238,77,248,85);MouseOn();
	     /* P2:Human/Computer */
       }
      else
      if ((x>=238 && y>=107 && x<=311 && y<=138 && (where==0 || where==3) && mousebuttons>0) ||
          ((where==0 || where==3) && ksel==1 && key=='G'))
       {
	     MouseOff();InBox(238,107,311,138);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     MouseOff();OutBox(238,107,311,138);MouseOn();
	     Data->Def.Lev2++;
	     if (Data->Def.Lev2>2) Data->Def.Lev2=0;
	     Levels(1,Data->Def.Lev2,1);
	     /* P2: Game Level */
       }
      else
	  if ((x>=238 && y>=160 && x<=311 && y<=191 && (where==0 || where==3) && mousebuttons>0) ||
         ((where==0 || where==3) && ksel==1 && key=='L'))
      {
	    MouseOff();InBox(238,160,311,191);MouseOn();
	    while(1)  { GetMouse();if (mousebuttons==0) break; }
	    MouseOff();OutBox(238,160,311,191);MouseOn();
	    Data->Def.Ast2++;
	    if (Data->Def.Ast2>2) Data->Def.Ast2=0;
	    Levels(1,Data->Def.Ast2,0);
	    /* P2: Astro Level */
	  }
     else
     if ((x>=6 && y>=34 && x<=83 && y<=42 && (where==3 || where==0) && mousebuttons>0) ||
       ((where==3 || where==0) && ksel==0 && key=='N'))
      {
	    MouseOff();RectFill(7,35,82,41,0);
	    for (i=0;i<20;i++) Data->P[0].Name[i]=0x00;
	    num=0;ch=0;
	    grSetColor(1);grMoveTo(8,40);DispChr(0x14);
	    while(ch!=0x0d)
        {
	      ch=getch();
        if (ch!=(ch&0xff)) ch=0x00;
        if (ch>='a' && ch<='z') ch-=0x20;
	      if (ch==0x08 && num>0) Data->P[0].Name[--num]=0x00;
	      else if (num<12 && ( isupper(ch) || isdigit(ch) || ch==0x20))
           Data->P[0].Name[num++]=ch;
	      RectFill(7,35,82,41,0);
	      grSetColor(1);PrintAt(8,40,&Data->P[0].Name[0]);
	      DispChr(0x14);
	      av_sync ();
	   }
	   Data->P[0].Name[num]=0x00;
	   RectFill(7,35,82,41,0);
	   grSetColor(1);PrintAt(8,40,&Data->P[0].Name[0]);
	   MouseOn();
	   av_sync ();
	   /* P1: Director Name */
     }
     else
	  if ((x>=236 && y>=34 && x<=313 && y<=42 && (where==3 || where==0) && mousebuttons>0) ||
         ((where==3 || where==0) && ksel==1 && key=='N'))
      {
	    MouseOff();RectFill(237,35,312,41,0);
	    for (i=0;i<20;i++) Data->P[1].Name[i]=0x00;
	    num=0;ch=0;
	    grSetColor(1); grMoveTo(238,40);DispChr(0x14);
	    while(ch!=0x0D) {
        ch=getch();
        if (ch!=(ch&0xff)) ch=0x00;
        if (ch>='a' && ch<='z') ch-=0x20;
	      if (ch==0x08 && num>0) Data->P[1].Name[--num]=0x00;
	      else if (num<12 && ( isupper(ch) || isdigit(ch) || ch==0x20))
           Data->P[1].Name[num++]=ch;
	      RectFill(237,35,312,41,0);
	      grSetColor(1);PrintAt(238,40,&Data->P[1].Name[0]);
	      av_sync ();
	   }

	  Data->P[1].Name[num]=0x00;
	  RectFill(237,35,312,41,0);
	  grSetColor(1);PrintAt(238,40,&Data->P[1].Name[0]);
	  MouseOn();
	  av_sync ();
	  /* P2: Director Name */
    };
   }
  };
}

void EditAst(void)
{
 int now,now2,BarA,BarB,i;
 FILE *fin;
 char Cur,Cnt,temp,b;
 strcpy(IDT,"i040");strcpy(IKEY,"k040");
 FadeOut(2,pal,10,0,0);        
 MouseOff();
 gxClearDisplay(0,0);
 ShBox(0,0,319,22);;IOBox(243,3,316,19);
 grSetColor(1);PrintAt(258,13,"CONTINUE");
 DispBig(11,5,"NAME AND SKILL EDITOR",0,-1);
 ShBox(1,24,159,118);ShBox(161,24,319,118);
 InBox(40,28,149,42);InBox(200,28,309,42);
 InBox(6,27,33,43);OutBox(167,27,194,43);FlagSm(0,7,28);FlagSm(1,168,28);
 IOBox(25,47,155,115);RectFill(26,48,154,114,0);IOBox(5,47,21,115);
 IOBox(185,47,315,115);RectFill(186,48,314,114,0);IOBox(165,47,181,115);
 RectFill(6,48,20,81,0);RectFill(166,48,180,81,0);
 ShBox(7,49,19,80);ShBox(7,82,19,113);UPArrow(10,52);DNArrow(10,85);
 ShBox(167,49,179,80);ShBox(167,82,179,113);UPArrow(170,52);DNArrow(170,85);
 grSetColor(6);PrintAt(66,37,"ASTRONAUTS");grSetColor(9);PrintAt(227,37,"COSMONAUTS");
 ShBox(0,120,319,199);
 OutBox(60,125,197,195);RectFill(61,126,196,194,9);InBox(65,129,192,191);RectFill(66,130,191,190,3);
 OutBox(6,124,43,136);OutBox(6,139,43,151);OutBox(6,154,43,166);
 OutBox(6,169,43,181);OutBox(6,184,43,196);
 grSetColor(9);
 PrintAt(13,132,"CAP");PrintAt(13,147,"L.M.");         
 PrintAt(13,162,"EVA");PrintAt(13,177,"DOCK.");PrintAt(13,192,"ENDR.");             
 grSetColor(1);
 PrintAt(71,138,"NAME:");PrintAt(71,147,"SKILL:");
 PrintAt(94,155,"CAP");PrintAt(94,163,"L.M.");PrintAt(94,171,"EVA");PrintAt(94,179,"DOCK");   
 PrintAt(94,187,"ENDR.");
 ShBox(206,126,311,146);RectFill(207,127,310,145,6);InBox(209,129,308,143); 
 RectFill(210,130,307,142,3);
 now=now2=BarA=BarB=0;
 Cnt=Cur=0; // holds current change
 ShBox(27,49+BarA*8,153,57+BarA*8); ShBox(187,49+BarB*8,313,57+BarB*8);
 Men=(struct ManPool far *) buffer;
 Sov=(struct ManPool far *) buffer+10000;
 FadeIn(2,pal,10,0,0);  
 MouseOn();
 temp=Idiot("i105");
 MouseOff();
 if (temp==1) fin = sOpen("CREW.DAT","rb",0);   /* Open Astronaut Data File  */
   else fin=sOpen("USER.DAT","rb",0);
 fseek(fin,((sizeof (struct ManPool))*106)*0,SEEK_SET);
 fread(Men,(sizeof (struct ManPool))*106,1,fin);
 fseek(fin,((sizeof (struct ManPool))*106)*1,SEEK_SET);
 fread(Sov,(sizeof (struct ManPool))*106,1,fin);
 fclose(fin);

 First(now,BarA);DrawStats(now,0);
 for (i=0;i<106;i++) M_Us[i]=0;
 for (i=0;i<106;i++) M_Sv[i]=0;
 for (i=0;i<106;i++) M_Us[i]=Men[i].Cap+Men[i].LM+Men[i].EVA+Men[i].Docking+Men[i].Endurance;
 for (i=0;i<106;i++) M_Sv[i]=Sov[i].Cap+Sov[i].LM+Sov[i].EVA+Sov[i].Docking+Sov[i].Endurance; 
 //FadeIn(2,pal,10,0,0);
 MouseOn();
 while(1)  { GetMouse();if (mousebuttons==0) break;}
  while (1)
  {
	key=0;GetMouse();
	if (mousebuttons > 0 || key>0)  /* Game Play */
	  {
	   if ((x>=245 && y>=5 && x<=314 && y<=17) || key==0x0d)
	 {
	  MouseOff();InBox(245,5,314,17);MouseOn();
	  while(1)  { GetMouse();if (mousebuttons==0) break;}
	  temp=0;
	  temp=Idiot("i106");
	  if (temp==1)
      {
        fin=sOpen("USER.DAT","wb",1);
	     fwrite(Men,(sizeof (struct ManPool))*106,1,fin);   
	     fwrite(Sov,(sizeof (struct ManPool))*106,1,fin); 
	     fclose(fin);
	    }
	 // delay(10);  
	  MouseOff();OutBox(245,5,314,17);MouseOn();
	  FadeOut(2,pal,10,0,0);       
	  delay(10);
	  return;
	 }
	}
    if ((x>=6 && y>=27 && x<=33 && y<=43 && mousebuttons>0 && Cnt==1) || key=='U')
      {
	    MouseOff();InBox(6,27,33,43);MouseOn();
	    while(1)  { GetMouse();if (mousebuttons==0) break;}
	    delay(10);
	    MouseOff();RectFill(186,48,314,114,0);Cnt=0;DrawStats(now,Cnt);
	    ShBox(27,49+BarA*8,153,57+BarA*8);First(now,BarA);MouseOn();
	    MouseOff();OutBox(167,27,194,43);MouseOn();
      }
     if ((x>=167 && y>=27 && x<=194 && y<=43 && mousebuttons>0 && Cnt==0) || key=='S')
       {
	     MouseOff();InBox(167,27,194,43);MouseOn();
	     while(1)  { GetMouse();if (mousebuttons==0) break;}
	     delay(10);MouseOff();RectFill(26,48,154,114,0);Cnt=1;DrawStats(now2,Cnt);
	     ShBox(187,49+BarB*8,313,57+BarB*8);Second(now2,BarB);MouseOn();
	     MouseOff();OutBox(6,27,33,43);MouseOn();
       }
     if (((x>=27 && y>=49+BarA*8 && x<=153 && y<=57+BarA*8 && mousebuttons>0) || key==0x0020) && Cnt==0)
       {
	     MouseOff();
	     ChangeStat(now,0,Cnt);  
	     ShBox(27,49+BarA*8,153,57+BarA*8);     
	     First(now,BarA);  
	     MouseOn();
       }
     if (((x>=187 && y>=49+BarB*8 && x<=313 && y<=57+BarB*8 && mousebuttons>0) || key==0x0020) && Cnt==1)
       {
	     MouseOff();
	     ChangeStat(now2,0,Cnt);
	     ShBox(187,49+BarB*8,313,57+BarB*8);   
	     Second(now2,BarB);
	     MouseOn();
       }
     if ((x>=6 && y>=124 && x<=43 && y<=136 && mousebuttons>0) || key=='C')
	   {
	    MouseOff();InBox(6,124,43,136);
	    if (Cur>0) OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));Cur=1;
      grSetColor(11);PrintAt(13,132,"CAP");
      MouseOn();
	    if (Cnt==0) ChangeStat(now,Cur,Cnt);
	     else ChangeStat(now2,Cur,Cnt);
       if (Cnt==0) ShBox(27,49+BarA*8,153,57+BarA*8);   
	     else ShBox(187,49+BarB*8,313,57+BarB*8);      
	    if (Cnt==0) First(now,BarA);
	     else Second(now2,BarB);
       OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));
	    while(1)  { GetMouse();if (mousebuttons==0) break;}
	    MouseOn();
      }
     if ((x>=6 && y>=139 && x<=43 && y<=151 && mousebuttons>0) || key=='L')
	   {
	    MouseOff();InBox(6,139,43,151);
	    if (Cur>0) OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));Cur=2;
      grSetColor(11);PrintAt(13,147,"L.M.");
      MouseOn();
	    if (Cnt==0) ChangeStat(now,Cur,Cnt);
	     else ChangeStat(now2,Cur,Cnt);
       if (Cnt==0) ShBox(27,49+BarA*8,153,57+BarA*8);   
	     else ShBox(187,49+BarB*8,313,57+BarB*8);      
	    if (Cnt==0) First(now,BarA);
	     else Second(now2,BarB);
	    OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));
	    while(1)  { GetMouse();if (mousebuttons==0) break;}
	    MouseOn();
      }
     if ((x>=6 && y>=154 && x<=43 && y<=166 && mousebuttons>0) || key=='E')
	   {
	    MouseOff();InBox(6,154,43,166);
	    if (Cur>0) OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));Cur=3;
      grSetColor(11);PrintAt(13,162,"EVA");
      MouseOn();
       if (Cnt==0) ChangeStat(now,Cur,Cnt);
	     else ChangeStat(now2,Cur,Cnt);
       if (Cnt==0) ShBox(27,49+BarA*8,153,57+BarA*8);   
	     else ShBox(187,49+BarB*8,313,57+BarB*8);      
	    if (Cnt==0) First(now,BarA);
	     else Second(now2,BarB);
	    OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));
	    while(1)  { GetMouse();if (mousebuttons==0) break;}
	    MouseOn();
      }
     if ((x>=6 && y>=169 && x<=43 && y<=181 && mousebuttons>0) || key=='D')
	   {
	    MouseOff();InBox(6,169,43,181);
	    if (Cur>0) OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));Cur=4;
      grSetColor(11);PrintAt(13,177,"DOCK.");
      MouseOn();
	    if (Cnt==0) ChangeStat(now,Cur,Cnt);                           
	     else ChangeStat(now2,Cur,Cnt);
       if (Cnt==0) ShBox(27,49+BarA*8,153,57+BarA*8);   
	     else ShBox(187,49+BarB*8,313,57+BarB*8);      
	    if (Cnt==0) First(now,BarA);
	     else Second(now2,BarB);
	    OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));
	    while(1)  { GetMouse();if (mousebuttons==0) break;}
	    MouseOn();
      }
     if ((x>=6 && y>=184 && x<=43 && y<=196 && mousebuttons>0) || key=='N')
	   {
	    MouseOff();InBox(6,184,43,196);
	    if (Cur>0) OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));Cur=5;
      grSetColor(11);PrintAt(13,192,"ENDR.");
      MouseOn();
	    if (Cnt==0) ChangeStat(now,Cur,Cnt);
	     else ChangeStat(now2,Cur,Cnt);
       if (Cnt==0) ShBox(27,49+BarA*8,153,57+BarA*8);   
	     else ShBox(187,49+BarB*8,313,57+BarB*8);      
	    if (Cnt==0) First(now,BarA);
	     else Second(now2,BarB);
	    OutBox(6,124+((Cur-1)*15),43,136+((Cur-1)*15));
	    while(1)  { GetMouse();if (mousebuttons==0) break;}
	    MouseOn();
      }
      //US SIDE
      if (Cnt==0)
       {
        for(b=0;b<8;b++)
         {
          if (x>=27 && y>=(49+b*8) && x<=153 && y<=(57+b*8) && mousebuttons>0 && (now-BarA+b)<=105)
           {
            now-=BarA;now+=b;BarA=b;
            key=0;
            GetMouse();
	         delay(10);
            MouseOff();
	         if (BarA==0)
	          if (now>=0)
              {
	            RectFill(26,48,154,114,0);
	            ShBox(27,49+BarA*8,153,57+BarA*8);
	            First(now,BarA);
	           };
	         if (BarA>0)
             {
	           RectFill(26,48,154,114,0);
	           ShBox(27,49+BarA*8,153,57+BarA*8);
	           First(now,BarA);
	          };
            while(1)  { GetMouse();if (mousebuttons==0) break;}
	         DrawStats(now,0);
	         MouseOn();
           }
         }
       }
      else if (Cnt==1)
       {
        for(b=0;b<8;b++)
         {
          if (x>=187 && y>=(49+b*8) && x<=314 && y<=(57+b*8) && mousebuttons>0 && (now2-BarB+b)<=105)
           {
            now2-=BarB;now2+=b;BarB=b;
            key=0;
            GetMouse();
	         delay(10);
            MouseOff();
  	         if (BarB==0)
	         if (now2>=0)
             {
	           RectFill(186,48,314,114,0);
	           ShBox(187,49+BarB*8,313,57+BarB*8);
	           Second(now2,BarB);
	          };
	         if (BarB>0)
             {
	           RectFill(186,48,314,114,0);
	           ShBox(187,49+BarB*8,313,57+BarB*8);
	           Second(now2,BarB);
	         };
           while(1)  { GetMouse();if (mousebuttons==0) break;}
	        DrawStats(now2,1);
	        MouseOn();
          }
        }
       }

      if (((x>=7 && y>=49 && x<=19 && y<=80 && mousebuttons>0) || key==0x4800) && Cnt==0)
        { /* Lft Up */
	      MouseOff();InBox(7,49,19,80);MouseOn();
         for (i=0;i<50;i++)
          {
	        key=0;
	        GetMouse();
	        delay(10);
	        if (mousebuttons==0) {
            MouseOff();
	      if (BarA==0)
	       if (now>0)
           {
	         now--;
	         RectFill(26,48,154,114,0);
	         ShBox(27,49+BarA*8,153,57+BarA*8);
	         First(now,BarA);
	        };
	      if (BarA>0)
           {
	         RectFill(26,48,154,114,0);
	         BarA--;now--;
	         ShBox(27,49+BarA*8,153,57+BarA*8);
	         First(now,BarA);
	        };
	      DrawStats(now,0);
	      MouseOn();
         i=51;
  	    }
	   }
      while (mousebuttons==1 || key==UP_ARROW)
       {
        delay(100);
        MouseOff();
	      if (BarA==0)
	       if (now>0)
           {
	         now--;
	         RectFill(26,48,154,114,0);
	         ShBox(27,49+BarA*8,153,57+BarA*8);
	         First(now,BarA);
	        };
	      if (BarA>0)
           {
	         RectFill(26,48,154,114,0);
	         BarA--;now--;
	         ShBox(27,49+BarA*8,153,57+BarA*8);
	         First(now,BarA);
	        };
	      DrawStats(now,0);
	      MouseOn();
         key=0;
         GetMouse();
         }
	      //while(1)  { GetMouse();if (mousebuttons==0) break;}
	      MouseOff();OutBox(7,49,19,80);MouseOn();delay(10);
        }
      if (((x>=7 && y>=82 && x<=19 && y<=113 && mousebuttons>0) || key==0x5000) && Cnt==0)
        { /* Lft Dwn */
	      MouseOff();InBox(7,82,19,113);MouseOn();
         for (i=0;i<50;i++)
          {
	        key=0;
	        GetMouse();
	        delay(10);
	        if (mousebuttons==0) {
           MouseOff();
    	     if (BarA==7)
	         if (now < 105)
             {
	           now++;
	           RectFill(26,48,154,114,0);
	           ShBox(27,49+BarA*8,153,57+BarA*8);
	           First(now,BarA);
	         };
	        if (BarA<7)
	        if (now<105)
            {
	          RectFill(26,48,154,114,0);
	          BarA++;now++;
		       ShBox(27,49+BarA*8,153,57+BarA*8);
	          First(now,BarA);
	         };
	        DrawStats(now,0);
	      MouseOn();
        i=51;
	    }
	   }
      while (mousebuttons==1 || key==DN_ARROW)
       {
        delay(100);
        MouseOff();
	     if (BarA==7)
	      if (now < 105)
          {
	        now++;
	        RectFill(26,48,154,114,0);
	        ShBox(27,49+BarA*8,153,57+BarA*8);
	        First(now,BarA);
	       };
	     if (BarA<7)
	      if (now<105)
          {
	        RectFill(26,48,154,114,0);
	        BarA++;now++;
		     ShBox(27,49+BarA*8,153,57+BarA*8);
	        First(now,BarA);
	       };
	      DrawStats(now,0);
	      MouseOn();
         key=0;
         GetMouse();
         }
	      //while(1)  { GetMouse();if (mousebuttons==0) break;}
	      MouseOff();OutBox(7,82,19,113);MouseOn();delay(10);
        };
      if (((x>=167 && y>=49 && x<=179 && y<=80 && mousebuttons>0) || key==0x4800) && Cnt==1)
        { /* Rt Up */
	      MouseOff();InBox(167,49,179,80);MouseOn();
         for (i=0;i<50;i++)
          {
	        key=0;
	        GetMouse();
	        delay(10);
	        if (mousebuttons==0) {
            MouseOff();
  	         if (BarB==0)
	          if (now2 > 0)
              {
	            now2--;
	            RectFill(186,48,314,114,0);
	            ShBox(187,49+BarB*8,313,57+BarB*8);
	            Second(now2,BarB);
	           };
	         if (BarB>0)
             {
	           RectFill(186,48,314,114,0);
	           BarB--;now2--;
	           ShBox(187,49+BarB*8,313,57+BarB*8);
	           Second(now2,BarB);
	          };
	         DrawStats(now2,1);
	         MouseOn();
            i=51;
  	      }
	     }
      while (mousebuttons==1 || key==UP_ARROW)
       {
        delay(100);
        MouseOff();
	      if (BarB==0)
	      if (now2 > 0)
           {
	         now2--;
	         RectFill(186,48,314,114,0);
	         ShBox(187,49+BarB*8,313,57+BarB*8);
	         Second(now2,BarB);
	        };
	      if (BarB>0)
           {
	         RectFill(186,48,314,114,0);
	         BarB--;now2--;
	         ShBox(187,49+BarB*8,313,57+BarB*8);
	         Second(now2,BarB);
	        };
	      DrawStats(now2,1);
	      MouseOn();
         key=0;
         GetMouse();
        }
	     //while(1)  { GetMouse();if (mousebuttons==0) break;}
         MouseOff();OutBox(167,49,179,80);MouseOn();delay(10);
        };
      if (((x>=167 && y>=82 && x<=179 && y<=113 && mousebuttons>0) || key==0x5000) && Cnt==1)
        { /* Rt Dwn */
	      MouseOff();InBox(167,82,179,113);MouseOn();
         for (i=0;i<50;i++)
          {
	        key=0;
	        GetMouse();
	        delay(10);
	        if (mousebuttons==0) {
           MouseOff();
  	        if (BarB==7)
	         if (now2 < 105)
             {
		        now2++;
		        RectFill(186,48,314,114,0);
		        ShBox(187,49+BarB*8,313,57+BarB*8);
		        Second(now2,BarB);
	          };
	        if (BarB<7)
		      if (now2<105)
             {
	           RectFill(186,48,314,114,0);
	           BarB++;now2++;
	           ShBox(187,49+BarB*8,313,57+BarB*8);
	           Second(now2,BarB);
	          };
	        DrawStats(now2,1);
	        MouseOn();
           i=51;
	     }
	   }
      while (mousebuttons==1 || key==DN_ARROW)
       {
        delay(100);
        MouseOff();
	     if (BarB==7)
	      if (now2 < 105)
          {
		     now2++;
		     RectFill(186,48,314,114,0);
		     ShBox(187,49+BarB*8,313,57+BarB*8);
		     Second(now2,BarB);
	       };
	      if (BarB<7)
		   if (now2<105)
           {
	         RectFill(186,48,314,114,0);
	         BarB++;now2++;
	         ShBox(187,49+BarB*8,313,57+BarB*8);
	         Second(now2,BarB);
	        };
	      DrawStats(now2,1);
	      MouseOn();
        key=0;
        GetMouse();
       }
	    //while(1)  { GetMouse();if (mousebuttons==0) break;}
	    MouseOff();OutBox(167,82,179,113);MouseOn();delay(10);
	   };
  }
}

void ChangeStat(char mum,char Cur,char Cnt) // Cur holds current setting
{
 char tot,srt,max,i,mxc,Save_Name[14];
 memset(Save_Name,0x00,sizeof Save_Name);
 RectFill(210,130,307,142,3);
 RectFill(205,151,316,196,3);
 grSetColor(6);
 if (Cur==0)
  {
   PrintAt(231,138,"NAME CHANGE");InBox(209,155,308,169);
   RectFill(210,156,307,168,0);
   if (Cnt==0)
    {
     strcpy(Save_Name,Men[mum].Name);
     memset(Men[mum].Name,0x00,sizeof Men[mum].Name);
    }
    else
     if (Cnt==1)
      {
       strcpy(Save_Name,Sov[mum].Name);
       memset(Sov[mum].Name,0x00,sizeof Sov[mum].Name);
      }
   grSetColor(1);grMoveTo(215,165);
   key=0;i=0;
   MouseOff();
   while(!(key==0x0d || key==0x1b))
	 {
	  key = bioskey (0);
	  if (key >= 'a' && key <= 'z')
		  key = toupper (key);
	  if (key&0x00ff)
		{
		 if ((i<13) && ((key==' ') || ((key>='A' && key<='Z')) ||
		   (key>='0' && key<='9')))
			{  // valid key
			 if (Cnt==0) 
			   {
			    Men[mum].Name[i++]=key;
			    grSetColor(1);PrintAt(215,165,&Men[mum].Name[0]);
			   }
			  else 
			   {
			    Sov[mum].Name[i++]=key;
			    grSetColor(1);PrintAt(215,165,&Sov[mum].Name[0]);
			   }
			 key=0;
			}
		if (i>0 && key==0x08)
		   {
			if (Cnt==0) Men[mum].Name[--i]=0x00;
			  else Sov[mum].Name[--i]=0x00;
			RectFill(210,156,307,168,0);
			grSetColor(1);
			if (Cnt==0) PrintAt(215,165,&Men[mum].Name[0]);
			  else PrintAt(215,165,&Sov[mum].Name[0]);
			key=0;
		   }
      if (key==K_ESCAPE)
        {
         if (Cnt==0) memset(Men[mum].Name,0x00,sizeof Men[mum].Name);
          else memset(Sov[mum].Name,0x00,sizeof Sov[mum].Name);
         i=0;
         RectFill(210,156,307,168,0);
			grSetColor(1);
			if (Cnt==0) PrintAt(215,165,&Men[mum].Name[0]);
			  else PrintAt(215,165,&Sov[mum].Name[0]);
         key=0;
        }

		}
	 } 
   if (i==0) 
    {
     if (Cnt==0) strcpy(Men[mum].Name,Save_Name);
      else strcpy(Sov[mum].Name,Save_Name);
    }
	DrawStats(mum,Cnt);RectFill(210,156,307,168,0);    
   RectFill(210,130,307,142,3);RectFill(205,151,316,196,3);    
   MouseOn();
   return;
  }
 else if (Cur>=1 && Cur<=5)
   {
    grSetColor(9);PrintAt(228,138,"SKILL CHANGE");
    tot=0;
    if (Cur!=1) 
      if (Cnt==0) tot+=Men[mum].Cap; else tot+=Sov[mum].Cap;
    if (Cur!=2) 
      if (Cnt==0) tot+=Men[mum].LM; else tot+=Sov[mum].LM;
    if (Cur!=3) 
      if (Cnt==0) tot+=Men[mum].EVA; else tot+=Sov[mum].EVA;
    if (Cur!=4) 
      if (Cnt==0) tot+=Men[mum].Docking; else tot+=Sov[mum].Docking;
    if (Cur!=5) 
      if (Cnt==0) tot+=Men[mum].Endurance; else tot+=Sov[mum].Endurance;
    switch(Cur)
     {
      case 1:if (Cnt==0) srt=Men[mum].Cap;else srt=Sov[mum].Cap;break;
      case 2:if (Cnt==0) srt=Men[mum].LM;else srt=Sov[mum].LM;break;
      case 3:if (Cnt==0) srt=Men[mum].EVA;else srt=Sov[mum].EVA;break;
      case 4:if (Cnt==0) srt=Men[mum].Docking;else srt=Sov[mum].Docking;break;
      case 5:if (Cnt==0) srt=Men[mum].Endurance;else srt=Sov[mum].Endurance;break;
      default:break;
     }
    mxc=0;
    if (Cnt==0) mxc=M_Us[mum];
      else mxc=M_Sv[mum];
    tot=mxc-tot;
    InBox(209,151,241,161);InBox(247,151,270,161);InBox(276,151,308,161);
    RectFill(248,152,269,160,0);grSetColor(9);
	PrintAt(214,158,"MIN 0");PrintAt(281,158,"MAX");
	if (strcmp(Men[97].Name,"VON BRAUN")==0) tot=4;
	if (tot>=4) tot=4;
	DispNum(302,158,tot);
	max=tot;
	grSetColor(1);DispNum(256,158,srt);
	// plus minus goes here
	IOBox(210,166,253,178);IOBox(265,166,308,178);IOBox(247,181,278,193);
	grSetColor(1);
	PrintAt(222,174,"PLUS");PrintAt(273,174,"MINUS");PrintAt(252,189,"DONE");
    while(1)  { GetMouse();if (mousebuttons==0) break;}
    while (1)
      {
	    key=0;GetMouse();
     	 if ((x>=212 && y>=168 && x<=251 && y<=176 && srt<max && mousebuttons>0) || (key=='+' && srt<max))  
	      {
	       MouseOff();InBox(212,168,251,176);MouseOn();
	       ++srt;RectFill(248,152,269,160,0);
	       grSetColor(1);DispNum(256,158,srt);  
	       delay(10);
	       while(1)  { GetMouse();if (mousebuttons==0) break;}     
	       MouseOff();OutBox(212,168,251,176);MouseOn();
	      }
	    if ((x>=267 && y>=168 && x<=306 && y<=176 && srt>=1 && mousebuttons>0) || (key=='-' && srt>=1))
	      {
	       MouseOff();InBox(267,168,306,176);MouseOn();
	       --srt;RectFill(248,152,269,160,0);
	       grSetColor(1);DispNum(256,158,srt);  
	       delay(10);
	       while(1)  { GetMouse();if (mousebuttons==0) break;}   
	       MouseOff();OutBox(267,168,306,176);MouseOn();
	      }
	    if ((x>=249 && y>=183 && x<=276 && y<=191 && mousebuttons>0) || key==0x0d)
	      {
	       MouseOff();InBox(249,183,276,191);MouseOn();
	       MouseOff();
	       switch(Cur)
	         {
	          case 1:if (Cnt==0) Men[mum].Cap=srt;else Sov[mum].Cap=srt;break;
	          case 2:if (Cnt==0) Men[mum].LM=srt;else Sov[mum].LM=srt;break;
	          case 3:if (Cnt==0) Men[mum].EVA=srt;else Sov[mum].EVA=srt;break;
	          case 4:if (Cnt==0) Men[mum].Docking=srt;else Sov[mum].Docking=srt;break;
	          case 5:if (Cnt==0) Men[mum].Endurance=srt;else Sov[mum].Endurance=srt;break;
	          default:break;
	         }
	       DrawStats(mum,Cnt);
	       MouseOn();
	       while(1)  { GetMouse();if (mousebuttons==0) break;}    
	       MouseOff();OutBox(249,183,276,191);
	       RectFill(210,156,307,168,0);    
	       RectFill(210,130,307,142,3);       
	       RectFill(205,151,316,196,3);
          grSetColor(9);
          PrintAt(13,132,"CAP");PrintAt(13,147,"L.M.");         
          PrintAt(13,162,"EVA");PrintAt(13,177,"DOCK.");PrintAt(13,192,"ENDR.");             
	       MouseOn();
	       return;
	      }
       } 
   } // end else-if
  return;
}

void DrawStats(char mum,char chce)
{
 RectFill(100,133,186,141,3);RectFill(122,151,186,190,3); 
 grSetColor(6);
 if (chce==0) 
   {
    PrintAt(100,138,&Men[mum].Name[0]);
    grSetColor(11);
    DispNum(124,155,Men[mum].Cap);
    DispNum(124,163,Men[mum].LM);  
    DispNum(124,171,Men[mum].EVA);  
    DispNum(124,179,Men[mum].Docking);  
    DispNum(124,187,Men[mum].Endurance);
   }
   else 
     {
      PrintAt(100,138,&Sov[mum].Name[0]);
      grSetColor(11);
      DispNum(124,155,Sov[mum].Cap);
      DispNum(124,163,Sov[mum].LM);  
      DispNum(124,171,Sov[mum].EVA);  
      DispNum(124,179,Sov[mum].Docking);  
      DispNum(124,187,Sov[mum].Endurance);
     }
 return;
}

void First(char now,char loc)
{
  int i,start;
  start=now-loc;
  for (i=start;i<start+8;i++) {
	grSetColor(1);
	DispNum(32,55+(i-start)*8,i);
   if (Men[i].Sex==0) grSetColor(1);
    else grSetColor(8);
	PrintAt(52,55+(i-start)*8,&Men[i].Name[0]);
  };
  return;
}

void Second(int nw,int lc)
{
  int i,start;
  start=nw-lc;
  for (i=start;i<start+8;i++) {
	grSetColor(1);
	DispNum(190,55+(i-start)*8,i);
   if (Men[i].Sex==0) grSetColor(1);
    else grSetColor(8);
	PrintAt(210,55+(i-start)*8,&Sov[i].Name[0]);
  };
  return;
}

