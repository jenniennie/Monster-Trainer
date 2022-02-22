/*
Jennifer Nguyen
January 26, 2021
Assignment 1: Monster Trainer
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"

#define MAXCHAR 50

typedef struct monster 
{ 
    char *name; 
    char *element; 
    int population;
} monster;

typedef struct region 
{
    char *name; 
    int nmonsters;
    int total_population;
    monster **monsters;
} region;

typedef struct itinerary 
{ 
    int nregions;
    region **regions; 
    int captures;
} itinerary;

typedef struct trainer 
{ 
    char *name; 
    itinerary *visits;
} trainer;

// Creates memory for one monster
monster* createMonster(char *name, char *element, int population)
{
    monster *mon = NULL;
    
    mon = (struct monster*) malloc(sizeof(struct monster*));
    
    mon->name = malloc(MAXCHAR * sizeof(char));
    strcpy(mon->name, name);
    
    mon->element = malloc(MAXCHAR * sizeof(char));
    strcpy(mon->element, element);
    
    mon->population = population;

    return mon;
}

// Creates spaces in memory for all monster information
monster** readMonsters(FILE* infile, int *monsterCount)
{
    monster **monsterList = NULL;
    int i, j, tempPop;
    char tempName[MAXCHAR], tempElement[MAXCHAR];
    
    monsterList = (struct monster**) malloc(*monsterCount * (sizeof(struct monster)));
    
    for (i = 0; i < *monsterCount; i++)
    {
        
        fscanf(infile, "%s", tempName);
        fscanf(infile, "%s", tempElement);
        fscanf(infile, "%d", &tempPop);
        
        monsterList[i] = createMonster(tempName, tempElement, tempPop);
    }
    
    return monsterList;
}


// Create regions and read in information
region** readRegions(FILE* infile, int *countRegions, monster** monsterList, int monsterCount)
{
    region **regionList = NULL;
    monster **monsters = NULL;
    
    int num_ofRegions, i, j , nmonsters, monCmp, total_population;
    char name[MAXCHAR], regionMon[MAXCHAR];
     
    
     
    //Region directory
    regionList = (struct region**) malloc(*countRegions * sizeof(struct region));
    
     // Directory of monsters
   
    for (num_ofRegions = 0; num_ofRegions < *countRegions; num_ofRegions++)
    {
        // 3 regions in region directory
        regionList[num_ofRegions] = (struct region*) malloc(sizeof(struct region));
        
        // Name
        regionList[num_ofRegions]->name = malloc(MAXCHAR * sizeof(char));
        fscanf(infile, "%s", name);
        strcpy(regionList[num_ofRegions]->name, name);
        
        // monster types in each region
        fscanf(infile, "%d %*s", &nmonsters);
        regionList[num_ofRegions]->nmonsters = nmonsters;
        
        //printf("%d n Monster", nmonsters);
        
        regionList[num_ofRegions]->monsters = (struct monster**)malloc(nmonsters * sizeof(struct monster));
        
            
        
        // Compare regional monsters to list of all monsters
        for ( i = 0; i < nmonsters; i++)
        {
            regionList[num_ofRegions]->monsters[i] = malloc(sizeof(struct monster));
            fscanf(infile, "%s", regionMon);
            
            // List of all monsters
            for (j = 0; j < monsterCount; j++)
            {
                monCmp = strcmp(regionMon, monsterList[j]-> name);
                
                // If matches
                if (monCmp == 0)
                {


                    regionList[num_ofRegions]->monsters[i] = monsterList[j];
                    
                    break;
                   
                }
               
            }
            
            regionList[num_ofRegions]->total_population += regionList[num_ofRegions]->monsters[i]->population;
        }
        
    }

    return regionList;
}

 // Returns list of trainers and their information
trainer* readTrainers(FILE* infile, int *trainerCount, region** regionList, int countRegions)
{
    trainer *trainerList = NULL;
    itinerary *visit = NULL;
    region **regions = NULL;
    
    int i, j, k, m, regCmp, captures, nregions, U;
    char name[MAXCHAR], regName[MAXCHAR];
    
    
    
    trainerList = (struct trainer*) malloc(*trainerCount * (sizeof(struct trainer)));
    
    int count, ctmon;
    
    for (i = 0; i < *trainerCount; i++)
    {

        visit = (struct itinerary*) malloc(sizeof(struct itinerary));

        fscanf(infile, "%s", name);
        trainerList[i].name = malloc(MAXCHAR * sizeof(char));
        strcpy(trainerList[i].name, name);
     

        fscanf(infile, "%d %*s", &captures);
        visit->captures = captures;

        
        
        fscanf(infile, "%d %*s", &nregions);
        visit->nregions = nregions;
        
        regions = (struct region**) malloc(nregions * sizeof(struct region*));

    
        for (k = 0; k < nregions; k++)
        {
 
            fscanf(infile, "%s", regName);
            
            // List of all regions
            for (j = 0; j < countRegions; j++)
            {
                regCmp = strcmp(regName, regionList[j]-> name);
 
                // If matches
                if (regCmp == 0)
                {
                    regions[k] = regionList[j];
                   
                    break;

                }
                
            }
            visit->regions = regions;
            
        }
        
        
        trainerList[i].visits = visit;
       
    }

    return trainerList;
}

void process_inputs(monster** monsterList, int monsterCount, region** regionList, int regionCount, trainer* trainerList, int trainerCount)
{
    FILE *infile, *outfile;
    infile = fopen("in.txt", "r");
    outfile = fopen("out.txt", "w");
    
    int i, j, N, count, NMon;
    float M, R, C, F;
    
    // Output
    for (i = 0; i < trainerCount; i++)
    {
        printf("%s\n", trainerList[i].name);
        fprintf(outfile, "%s\n", trainerList[i].name);
        
        for (j = 0; j < trainerList[i].visits->nregions; j++)
        {
        
            printf("%s\n", trainerList[i].visits->regions[j]->name);
            fprintf(outfile, "%s\n", trainerList[i].visits->regions[j]->name);
            
            for ( count = 0; count < trainerList[i].visits->regions[j]->nmonsters; count++)
            {

                M = trainerList[i].visits->regions[j]->total_population;
                R = trainerList[i].visits->captures;
                C = (trainerList[i].visits->regions[j]->monsters[count]->population / M) * R;
                C = round(C);
                
                if (C != 0)
                {
                    printf("%.0f ", C);
                    fprintf(outfile, "%.0f ", C);
                 
                    
                    printf("%s\n", trainerList[i].visits->regions[j]->monsters[count]->name);
                    fprintf(outfile, "%s\n", trainerList[i].visits->regions[j]->monsters[count]->name);
                }
            }
        }
        if(i + 1 != trainerCount)
        {
            printf("\n");
            fprintf(outfile,"\n");
        }
    }
    return;
}

void release_memory(monster** monsterList, int monsterCount, region** regionList, int regionCount, trainer* trainerList, int trainerCount )
{
  int i, j, k, m, p, monsternum;
  
  for (k = 0; k < monsterCount; k++)
    {
        free(monsterList[k]->name);
        free(monsterList[k]->element);
        free(monsterList[k]);
    }
    free(monsterList);
    
    
   for (j = 0; j < regionCount; j++)
    {
        monsternum = regionList[j]->nmonsters;
        for (m = 0; m < monsternum; m++)
        {
            // I attempted to free the memory leak on line 128 but this caused an Aboted (core dumped), I'm not sure why
            //free(regionList[j]->monsters[m]);
           
        }
        
        free(regionList[j]->name);
        free(regionList[j]->monsters);
        free(regionList[j]);
    }
    free(regionList);
    
    
    for (i = 0; i < trainerCount; i++)
    {
        free(trainerList[i].name);
        free(trainerList[i].visits->regions);
        free(trainerList[i].visits);
    }
    
    free(trainerList);
    
    return;
}



int main()
{
    atexit(report_mem_leak);
    
    FILE *infile, *outfile;
    infile = fopen("in.txt", "r");
    outfile = fopen("out.txt", "w");

    struct monster **monsterList;
    struct region **regionList;
    struct trainer *trainerList;

    
    int population, monsterCount, regionCount, trainerCount, j, i;
    char name[MAXCHAR];
    
    fscanf(infile, "%d %*s", &monsterCount);
    monsterList = readMonsters(infile, &monsterCount); 
    
    fscanf(infile, "%d %*s", &regionCount);
    regionList = readRegions(infile, &regionCount, monsterList, monsterCount);
    
    fscanf(infile, "%d %*s", &trainerCount);
    trainerList = readTrainers(infile, &trainerCount, regionList, regionCount);
    
    process_inputs(monsterList, monsterCount, regionList, regionCount, trainerList, trainerCount);
    
    release_memory(monsterList, monsterCount, regionList, regionCount, trainerList, trainerCount);
    
    fclose(infile);
    fclose(outfile);
   
    return 0;
}
