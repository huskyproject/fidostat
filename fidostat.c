/*****************************************************************************
*                                                                            
* Fidostat V 0.01                                   
*                                                                            
* Copyright (C) 1999                                                    
* Gabriel Plutzar                                                             
*                                                                            
* Fido:     2:31/1
* Internet: gabriel@hit.priv.at           
*                                                                            
* FIDOSTAT is free software; you can redistribute it and/or modify it        
* under the terms of the GNU General Public License as published by the      
* Free Software Foundation; either version 2, or (at your option) any        
* later version.                                                             
* FIDOSTAT is distributed in the hope that it will be useful, but        
* WITHOUT ANY WARRANTY; without even the implied warranty of                  
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU           
* General Public License for more details.                                    
*                                                                             
* You should have received a copy of the GNU General Public License           
* along with FIDOSTAT; see the file COPYING.  If not, write to the Free      
* Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.                
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MSDOS          
#include "fidoconfig.h"
#else                  
#include "fidoconf.h"  
#endif                 

#include "common.h"  

struct sessioncounttype {
       char fidoaka[50];
       char fromname[100];
       int count;
       } *sessioncount;

static char mounthstr[12][4]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug",
"Sep","Oct","Nov","Dec"};

int sessionsort(const void *a, const void *b)
{
    s_addr addra,addrb;

    string2addr(a,&addra);
    string2addr(b,&addrb);

    if (addra.point==0 && addrb.point>0)
       return(-1);

    if (addrb.point==0 && addra.point>0)
       return(1);

    if (addra.zone!=addrb.zone)
       {
       if (addra.zone>addrb.zone)
          return(1);
         else
          return(-1);
       }        
      else
       if (addra.net!=addrb.net)
          {
          if (addra.net>addrb.net)
             return(1);
            else
             return(-1);
          }
         else
          if (addra.node!=addrb.node)
             {
             if (addra.node>addrb.node)
                return(1);
               else
                return(-1);
             }
            else
             if (addra.point!=addrb.point)
                {
                if (addra.point>addrb.point)
                   return(1);
                  else
                   return(-1);
                }

    return(0);
}

void main(int argc,char **argv)
{
   char hlp[200],*hlp1;
   char session_with[200],firstaddr[200];
   char todaydate[20];
   int getaddr=0;
   FILE *binkdlog;
   struct tm *date;      
   time_t currentTime;
   int anzsession=0,sessionfound;
   int i;
   s_fidoconfig *config;

   if (argc<2 || (stricmp(argv[1],"binkdstat")!=0 &&
                  stricmp(argv[1],"binkdall")!=0) )
      {
      printf(
"FidoStat V 0.01                    Statisticgenerator by Gabriel Plutzar\n"
"\n"
"Syntax: fidostat <command>\n"
"\n"
"commands:  binkdall   Print all Binkd Polls\n"
"           binkdstat  Print a summary of alle Binkd Polls\n"
"\n"
"You may pipe the output into a file, and post that file via hpt post\n"
);
      exit(3);
      }

   config = readConfig();
   if (config == NULL) exit(3);  

   currentTime = time(NULL);       
   date = localtime(&currentTime); 
   sprintf(todaydate,"%02u %s",date->tm_mday,mounthstr[date->tm_mon]);

   printf(
"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n"
"   FidoStat V 0.01    Binkd Poll Statistic\n"
"\n"
"   Date: %s"
"   System: %u:%u/%u.%u, %s\n"
"\n"
"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n"
"\n",
ctime(&currentTime),
config->addr[0].zone,config->addr[0].net, config->addr[0].node,
config->addr[0].point,config->sysop);

   sessioncount=malloc(500*sizeof(*sessioncount));

   strcpy(hlp,config->logFileDir);
   strcat(hlp,"/binkd.log");
   binkdlog=fopen(hlp,"r");

   if (binkdlog==NULL)
      {
      printf("Cannot open %s !\n",hlp);
      exit(3);
      }

   while (!feof(binkdlog))
         {
         fgets(hlp,200,binkdlog);
         hlp[strlen(hlp)-1]=0;

         if (strncmp(hlp+2,todaydate,6)!=0)
            continue;

         hlp1=strstr(hlp,"session with ");
         if (hlp1 != NULL)
            {
            /* Got New Session with LogFile Part - let's get the session
               and the first Fido Adress */
            getaddr=1;
            strcpy(session_with,(char *)(hlp1+13));
            continue;
            }


         hlp1=strstr(hlp,"addr: ");
         if (hlp1 != NULL && getaddr==1)
            {
            getaddr=0;
            strcpy(firstaddr,(char *)(hlp1+6));

            if (strcasecmp(argv[1],"binkdall")==0)
               printf("%s, %s\n",session_with,firstaddr);
            
            sessionfound=0;
            for (i=0;i<anzsession && i<499;i++)
                if (strcmp(firstaddr,sessioncount[i].fidoaka)==0)
                   {
                   sessionfound=1;
                   sessioncount[i].count++;
                   break;
                   }

            if (sessionfound==0)
               {
               strcpy(sessioncount[anzsession].fidoaka,firstaddr);
               strcpy(sessioncount[anzsession].fromname,session_with);
               sessioncount[anzsession].count=1;
               anzsession++;
               }

            }
         }

  if (strcasecmp(argv[1],"binkdstat")==0)
     {
     qsort(sessioncount,anzsession,
           sizeof(*sessioncount),sessionsort);

     for (i=0;i<anzsession;i++)
         printf("%3u Sessions with %s, %s\n",sessioncount[i].count,
                sessioncount[i].fidoaka,sessioncount[i].fromname);
     }
  fclose(binkdlog);

  disposeConfig(config);
}


