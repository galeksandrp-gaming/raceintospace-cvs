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

extern char AI[2];

void PadDraw(char plr,char pad)
{
  int i,j,k,l;
  
  FadeOut(2,pal,10,0,0);
  gxClearDisplay(0,0);
  ShBox(0,0,319,22); ShBox(0,24,319,198); InBox(3,3,30,19); IOBox(243,3,316,19);
  InBox(167,27,316,176);RectFill(168,28,315,175,0);
  IOBox(167,179,316,195);
  ShBox(4,28,162,43);InBox(6,30,160,41);
  ShBox(4,46,162,61);InBox(6,48,160,59);
  ShBox(4,68,162,97);InBox(6,70,160,95);
  ShBox(56,64,110,74);InBox(57,65,109,73);
  ShBox(4,180,162,195);InBox(6,182,160,193); //sched. duration
  InBox(6,99,160,178);
  grSetColor(9);PrintAt(11,190,"SCHEDULED DURATION: ");
  grSetColor(7);
  switch(Data->P[plr].Mission[pad].Duration)
   {
    case 1:PrintAt(0,0,"A");break;
    case 2:PrintAt(0,0,"B");break;
    case 3:PrintAt(0,0,"C");break;
    case 4:PrintAt(0,0,"D");break;
    case 5:PrintAt(0,0,"E");break;
    case 6:PrintAt(0,0,"F");break;
    default:PrintAt(0,0,"NONE");break;
   }   
  grSetColor(7);PrintAt(63,71,"MISSION");
  FlagSm(plr,4,4);
  if (Data->P[plr].LaunchFacility[pad]==1 && Data->P[plr].Mission[pad].MissionCode>0) PadPict(2+plr);
	else if (Data->P[plr].LaunchFacility[pad]==1 && Data->P[plr].Mission[pad].MissionCode==0) PadPict(4+plr);
	 else if (Data->P[plr].LaunchFacility[pad]>1) PadPict(0+plr);
  grSetColor(1);PrintAt(15,37,"STATUS: ");grSetColor(9);
  if (Data->P[plr].LaunchFacility[pad]==1)
	PrintAt(0,0,"OPERATIONAL"); else PrintAt(0,0,"DESTROYED");
  grSetColor(1);

  if (Data->P[plr].LaunchFacility[pad]>1)
	{
	 PrintAt(15,56,"REPAIR COST: ");grSetColor(9);
	 DispNum(0,0,Data->P[plr].LaunchFacility[pad]);
	 PrintAt(0,0,"MB");
    if (Data->P[plr].Cash<Data->P[plr].LaunchFacility[pad])
     InBox(169,181,314,193);
	}
   else
    {
     grSetColor(9);
  	  if (Data->P[plr].Mission[pad].MissionCode==0)
      {
       PrintAt(15,56,"NO LAUNCH SCHEDULED");
       InBox(169,181,314,193);
      }
      else PrintAt(15,56,"PRE-MISSION CHECK");
    }
  grSetColor(1);
  PrintAt(258,13,"CONTINUE");
  if (Data->P[plr].LaunchFacility[pad]==1)
   {
    grSetColor(9);PrintAt(210,189,"S");grSetColor(1);PrintAt(0,0,"CRUB MISSION");
   }
  else
   {
    grSetColor(9);PrintAt(205,189,"F");grSetColor(1);PrintAt(0,0,"IX LAUNCH PAD");
   }
  DispBig(35,5,"LAUNCH FACILITY",0,-1);
  switch(pad) {
	 case 0: DispBig(201,5,"A",0,-1);break;
	 case 1: DispBig(201,5,"B",0,-1);break;
	 case 2: DispBig(201,5,"C",0,-1);break;
  }
   grSetColor(6);MissionName(Data->P[plr].Mission[pad].MissionCode,11,81,20);
   grSetColor(1);
  // joint mission part
  if (Data->P[plr].Mission[pad].Joint==1) 
    {
     ShBox(38,91,131,101);InBox(39,92,130,100);
     grSetColor(1);
     if (Data->P[plr].Mission[pad].part==0) PrintAt(53,98,"PRIMARY PART");
       else PrintAt(44,98,"SECONDARY PART");
    }
  // Hardware to Use

  i=Data->P[plr].Mission[pad].Prog;
  j=Data->P[plr].Mission[pad].PCrew-1;
  l=Data->P[plr].Mission[pad].BCrew-1;

  // Crews
  grSetColor(7);PrintAt(13,107,"PRIMARY CREW");PrintAt(13,145,"BACKUP CREW");
  grSetColor(11);
  if (j>=0) {
    for (k=0;k<Data->P[plr].Gcnt[i][j];k++) {
	    PrintAt(25,115+7*k,&Data->P[plr].Pool[Data->P[plr].Crew[i][j][k]-1].Name[0]);
    }
    if (l==-1) PrintAt(25,174,"UNAVAILABLE");
  }
  if (l>=0) {
    for (k=0;k<Data->P[plr].Gcnt[i][l];k++) {
	    PrintAt(25,153+7*k,&Data->P[plr].Pool[Data->P[plr].Crew[i][l][k]-1].Name[0]);
    }
    if (j==-1) PrintAt(25,136,"UNAVAILABLE");
  }

	if(Data->P[plr].Mission[pad].Prog>0)
		PatchMe(plr,126,40,Data->P[plr].Mission[pad].Prog-1,
			Data->P[plr].Mission[pad].Patch,32);
  FadeIn(2,pal,10,0,0);
  
  return;
}


// ClrMiss NOTES
// pad is +3 for compatability with News so that it will go ahead and
// scrub the mission automatically

void ClrMiss(char plr,char pad)
{
 char prime,back,men,i,prg,temp=0,padd;
 padd=pad%3;

 //pad joint first part/second part/single 
 if (Data->P[plr].Mission[padd].Joint==0)
  {
   if (!AI[plr] && pad<3) temp=Idiot("i111");
  }
  else 
   switch(padd)
    {
     case 0:if (!AI[plr] && pad<3) temp=Idiot("i110");break;
     case 1:if (Data->P[plr].Mission[1].Joint==1)
             {
              if (Data->P[plr].Mission[0].Joint==1)
               {
                if (!AI[plr] && pad<3) temp=Idiot("i112");
                padd=0;
               }
              else
               {
                if (!AI[plr] && pad<3) temp=Idiot("i110");
               }
             }
            break;
     case 2:padd=1;if (!AI[plr] && pad<3) temp=Idiot("i112");break;
     default:temp=0;break;
    }
 if (AI[plr]) temp=1;
 if (temp==-1) return;

 prg=Data->P[plr].Mission[padd].Prog;
 Data->P[plr].Mission[padd].Hard[Mission_PrimaryBooster] = 0;
 if (Data->P[plr].Mission[padd].PCrew!=0)
   prime=Data->P[plr].Mission[padd].PCrew-1; else prime=-1;
   if (Data->P[plr].Mission[padd].BCrew!=0)
	 back=Data->P[plr].Mission[padd].BCrew-1; else back=-1;
	  men= Data->P[plr].Mission[padd].Men;
	  if (prime!=-1)
	  for (i=0;i<men;i++)
		Data->P[plr].Pool[Data->P[plr].Crew[prg][prime][i]-1].Prime=0;
	  if (back!=-1)
	  for (i=0;i<men;i++)
		Data->P[plr].Pool[Data->P[plr].Crew[prg][back][i]-1].Prime=0;
	  if (Data->P[plr].Mission[padd].Joint==1)
		{
      prg=Data->P[plr].Mission[padd+1].Prog;
		 if (Data->P[plr].Mission[padd+1].PCrew!=0)
		   prime=Data->P[plr].Mission[padd+1].PCrew-1; else prime=-1;
		 if (Data->P[plr].Mission[padd+1].BCrew!=0)
		   back=Data->P[plr].Mission[padd+1].BCrew-1; else back=-1;
		 men=Data->P[plr].Mission[padd+1].Men;
		 if (prime!=-1)
		   for (i=0;i<men;i++)
		  Data->P[plr].Pool[Data->P[plr].Crew[prg][prime][i]-1].Prime=0;
		 if (back!=-1)
		   for (i=0;i<men;i++)
		Data->P[plr].Pool[Data->P[plr].Crew[prg][back][i]-1].Prime=0;
		 Data->P[plr].Mission[padd+1].part=0;
		 Data->P[plr].Mission[padd+1].Prog=0;
		 Data->P[plr].Mission[padd+1].PCrew=0;
		 Data->P[plr].Mission[padd+1].BCrew=0;
		 Data->P[plr].Mission[padd+1].Joint=0;
		 Data->P[plr].Mission[padd+1].Men=0;
		 Data->P[plr].Mission[padd+1].MissionCode=0;
	  }
	  Data->P[plr].Mission[padd].Prog=0;
	  Data->P[plr].Mission[padd].PCrew=0;
	  Data->P[plr].Mission[padd].BCrew=0;
	  Data->P[plr].Mission[padd].Men=0;
     Data->P[plr].Mission[padd].Joint=0;
	  Data->P[plr].Mission[padd].MissionCode=0;
	  if (Data->P[plr].Mission[padd].Joint==1 && Data->P[plr].Mission[padd].part==0)
   	  memset(&Data->P[plr].Mission[padd+1],0x00,sizeof (struct MissionType));
	  if (Data->P[plr].Mission[padd+1].Joint==1 && Data->P[plr].Mission[padd+1].part==1)
	    memset(&Data->P[plr].Mission[padd+1],0x00,sizeof (struct MissionType));
 return;
}


void PadPict(char poff)
{
  GXHEADER local,local2;
  struct FF {
	ui16 size;
	long offset;
   } table;
  FILE *in;
  in=sOpen("LFACIL.BUT","rb",0);
  fread(&table,sizeof table,1,in);
  fseek(in,6*(sizeof table),SEEK_SET);
  fread(&pal[0],768,1,in);
  fseek(in,table.offset,SEEK_SET);
  fread(buffer,table.size,1,in);
  GV(&local,148,148); GV(&local2,148,148);
  RLED_img(buffer,local.vptr,table.size,local.w,local.h);
  fseek(in,(poff)*(sizeof table),SEEK_SET);
  fread(&table,sizeof table,1,in);
  fseek(in,table.offset,SEEK_SET);
  fread(buffer,table.size,1,in);
  RLED_img(buffer,local2.vptr,table.size,local2.w,local2.h);
  
  gxPutImage(&local2,gxSET,168,28,0);
  
  fclose(in);
  DV(&local); DV(&local2);
  return;
}


void ShowPad(char plr,char pad)
{
  char temp;
  PreLoadMusic((plr==1)?M_USMIL:M_USSRMIL);
  PadDraw(plr,pad);
  PlayMusic(0);
  temp=CheckCrewOK(plr,pad);
   if (temp==1) //found mission no crews
   {

    ClrMiss(plr,pad+3);
    return;
   }
  strcpy(IDT,"i028\0");strcpy(IKEY,"k028\0");
  while(1)  { GetMouse();if (mousebuttons==0) break;}
  while (1)
  {
	key=0;GetMouse();
	if ((Data->P[plr].LaunchFacility[pad]==1 && x>=169 && y>=181 && x<=314 && y<=193 && mousebuttons>0 && Data->P[plr].Mission[pad].MissionCode>0)
      || (Data->P[plr].LaunchFacility[pad]==1 && Data->P[plr].Mission[pad].MissionCode>0 && key=='S')) 
	  {  // Scrub Mission
	   InBox(169,181,314,193);
	   key=0;
	   while(1)  { GetMouse();if (mousebuttons==0) break;}
     ClrMiss(plr,pad);
	   OutBox(169,181,314,193);
	   key=0; 
	   if (Data->P[plr].Mission[pad].MissionCode==0) {KillMusic();return;};
	  }
   else
	if ((Data->P[plr].LaunchFacility[pad]<=Data->P[plr].Cash && Data->P[plr].LaunchFacility[pad]>1 && x>=169 && y>=181 && x<=314 && y<=193 && mousebuttons>0)
    || (key=='F' && Data->P[plr].LaunchFacility[pad]>1 && Data->P[plr].LaunchFacility[pad]<=Data->P[plr].Cash)) 
	  {  // Scrub Mission
	   InBox(169,181,314,193);
	   key=0;
	   while(1)  { GetMouse();if (mousebuttons==0) break;}
	   if (Data->P[plr].Cash>=Data->P[plr].LaunchFacility[pad]) 
	     { 
		   temp=0;
		   temp=Idiot("i115");
		   if (temp==1)
          {
           Data->P[plr].Cash=Data->P[plr].Cash-Data->P[plr].LaunchFacility[pad];
           Data->P[plr].LaunchFacility[pad]=1;
          }
	     }
	   else 
	    {
	     temp=0;
	     temp=Idiot("i116");
        temp=0;
	    }
	   OutBox(169,181,314,193);
	   key=0;
      if (temp==1) {KillMusic();return;};
	  }
   else
	if ((x>=245 && y>=5 && x<=314 && y<=17 && mousebuttons>0) || key==0x0d)
	{
	   InBox(245,5,314,17);key=0;
	   while(1)  { GetMouse();if (mousebuttons==0) break;}
	   OutBox(245,5,314,17);
      if (key>0) delay(150);
      KillMusic();
	   key=0; return;  /* Done */
	};
  }
}

// EOF
