#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define FILE_SIZE 30
#define SCANPATH_SIZE 100
#define PATTERN_SIZE 100


struct Node // this node holds for people's scanpaths
{
    char scanPath[SCANPATH_SIZE];
    struct Node *next;
};

struct ListRecord
{
    struct Node *head;
    struct  Node *tail;
    int size;
};

typedef struct ListRecord *List; // List is my linked list


/*This function takes the name of the file containing a number of
scanpaths which are represented as string sequences and creates a linked-list where each
node contains a string representation of a single scanpath. It will return the created list.
*/
List createScanpathList(char fileName[FILE_SIZE]);


void MakeEmptyList(List l);//this functions makes my list empty at the beginning


/*This function takes the pattern and the linked lists of people with and without
autism to check whether the pattern is available in the scanpaths of people with and without
autism by using the Rabin-Karp Algorithm. PWA= people with autism, PWOA = people without autism
*/
void searchPattern(char pattern[PATTERN_SIZE],List PWA,List PWOA);



/*
This algorithm helps me to detect the string patterns. It takes:
1) The scanpath of person
2) The pattern which entered by user
3) d, which represents number of digits(since there are 10 digits, d equals to 10)
4) q, which is given initially as 11
5) patternCount, which is the count of patterns when they found
6) indexArr, which is an array simply holds the index when pattern is found, for example in the sample run, indexes are 8 16 and 5
7) indexArrCounter,which is the index of my indexArr. Initially, it is 0, when I found an index, I increment it by one
8) frequentAOI, which is an array that holds the most frequent AOI before the pattern for each group of people.
9) flag, which indicates whether I found any pattern or not. It is initially 0, if it remains as 0, I claimed that there are no pattern for people's scanpath

*/

void rabinKarp(char T[SCANPATH_SIZE],char P[PATTERN_SIZE],int,int,int*,int*,int*,int*,int*);





int main()
{

char withAutismFile[FILE_SIZE];//these are the arrays of my files and the pattern
char withoutAutismFile[FILE_SIZE];
char pattern[PATTERN_SIZE];

printf("Enter the file name for people with autism:");
gets(withAutismFile);
printf("Enter the file name for people without autism:");
gets(withoutAutismFile);
printf("Enter the pattern:");
gets(pattern);


List peopleWithAutism;
List peopleWithoutAutism;

peopleWithAutism=createScanpathList(withAutismFile); //here, I created linked list for both peopleWithAutism and peopleWithoutAutism
peopleWithoutAutism=createScanpathList(withoutAutismFile);

searchPattern(pattern,peopleWithAutism,peopleWithoutAutism);//after creating lists, I called searchPattern function

free(peopleWithAutism);// when I am done, I free the lists
free(peopleWithoutAutism);


    return 0;
}





void MakeEmptyList(List l)//makes the list empty
{
    l->head=(struct Node*)malloc(sizeof(struct Node));

    l->head->next=NULL;
    l->tail=l->head;
    l->size=0;
}

List createScanpathList(char fileName[FILE_SIZE])//basically it takes the file name and creates list
{

    List l;

    l=(struct ListRecord*)malloc(sizeof(struct ListRecord));
    MakeEmptyList(l);

    FILE *outFile;
    outFile=fopen(fileName,"r");
    if(outFile==NULL)
        printf("File can not opened for read mode!");

    int i;
    char path[SCANPATH_SIZE];
    struct Node *tmp;// I created temporary node to get the data from data file and not to lose my original data

    for(i=0;((fscanf(outFile,"%s\n",path)!=EOF));i++)//this loop takes data until it reaches EOF//
	{
        tmp = (struct Node *) malloc(SCANPATH_SIZE*sizeof(struct Node));// this temporary node carries the information taken by file//

	  	strcpy(tmp->scanPath,path);

        tmp->next = NULL;

	  	l->tail->next=tmp;
        l->tail=tmp;
        l->size++;
    }

    return l;// after everything is done, I returned created list

}




void searchPattern(char pattern[PATTERN_SIZE],List PWA,List PWOA)//it takes the pattern and lists that are created in createScanpathList.It searches for patterns in scanpaths
{

    int d=10;
    int q=11;
    int simpleCounter=0;//this is the counter of patterns.
    //In sampleRun, they are 1. and 2.  => (1. 65473827[123]84758[123]  2. 35477[123]64687)

    int patternCount=0;// this is the count of number of patterns
    int i,n,m;// n holds for length of the scanpath, m holds for length of the pattern
    int indexArr[30];//this is the array that holds the index values when a pattern is found
    int flag;//this is the flag that indicates I found a pattern for scanpath or not

    int frequentAOI[10];//this array holds the value of most frequentAOI before the pattern. Every index of this array represents the value of it
    //so I initialize the array indexes with 0, and hold the array based on the most frequentAOI before the pattern
    for(i=0;i<10;i++)
        frequentAOI[i]=0;






    struct Node *tmpPWA;//this is a temporary node for peoplewithautism list
    tmpPWA=PWA->head->next;

    printf("Detected patterns for people with autism:\n");
    while(tmpPWA != NULL)//when it is not null, I do the following operations
    {
        int indexArrCounter=0;// this is the counter of my indexArray, I do print operations based on this index
        flag=0;

        n=strlen(tmpPWA->scanPath);// I took the length of scanpath and pattern to do print operations
        m=strlen(pattern);
        rabinKarp(tmpPWA->scanPath,pattern,d,q,&patternCount,indexArr,&indexArrCounter,frequentAOI,&flag);// I called rabinKarp algorithm to search for pattern

        indexArrCounter=0;// after calling it, I assign the indexArr counter as 0 to traverse the index array from 0
        if(flag!=0)
        {
            simpleCounter++;// when flag is not zero, this means I found a pattern, so I increment the simple counter
            printf("%d. ",simpleCounter);
            for(i=0;i<n;i++)// in this for loop, I printed the scanpath
            {

                if(indexArr[indexArrCounter]==i)//when I reach the index of foundPattern, I print "["
                    printf("[");
                else if(indexArr[indexArrCounter]+m==i)//when I reach the end of the pattern, I print "]"
                {
                    printf("]");
                    indexArrCounter++;// after printing "]", this means I need to go with new index, so I incremented indexArrCounter
                }


                printf("%c",tmpPWA->scanPath[i]);
            }


            if(indexArr[indexArrCounter]+m==n)//this is the case if the pattern exist at the end.
            {
                printf("]");
            }
            printf("\n");
        }


        tmpPWA=tmpPWA->next;// I move to next node

    }
    if(patternCount!=0)
    {
        printf("%d patterns detected in %d scanpaths\n",patternCount,simpleCounter);

        int max=0;//this is the value of most frequent AOI before the pattern
        int maxArray[10];//this array represents the most frequent AOI before the pattern

        for(i=0;i<10;i++)//in this loop, I determined which array element has the  most frequent AOI before the pattern
        {
            //for example, if the most frequent AOI before the pattern is 7 and the value of the existence of it is 3, frequentAOI[7]=3
            if(frequentAOI[i]>max)
                max=frequentAOI[i];

        }
        int maxCount=0;

        for(i=0;i<10;i++)
        {
            if(frequentAOI[i]==max)//after getting the maximum array value, I compare which indexes has equal to max
            {
                maxArray[maxCount]=i;
                maxCount++;
            }
        }

        printf("The most frequent AOI before the pattern is/are");
        for(i=0;i<maxCount;i++)//then I traverse the array and print the  most frequent AOI before the pattern
            printf("% d",maxArray[i]);
    }
    else
        printf("None");


    printf("\n\n");

    struct Node *tmpPWOA;//same operations with the people with autism case
    tmpPWOA=PWOA->head->next;

    patternCount=0;
    simpleCounter=0;

    printf("Detected patterns for people without autism:\n");

    while(tmpPWOA != NULL)
    {
        int indexArrCounter=0;
        flag=0;

        n=strlen(tmpPWOA->scanPath);
        m=strlen(pattern);
        rabinKarp(tmpPWOA->scanPath,pattern,d,q,&patternCount,indexArr,&indexArrCounter,frequentAOI,&flag);

        indexArrCounter=0;
        if(flag!=0)
        {
            simpleCounter++;
            printf("%d. ",simpleCounter);
            for(i=0;i<n;i++)
            {

                if(indexArr[indexArrCounter]==i)
                    printf("[");
                else if(indexArr[indexArrCounter]+m==i)
                {
                    printf("]");
                    indexArrCounter++;
                }


                printf("%c",tmpPWOA->scanPath[i]);
            }


            if(indexArr[indexArrCounter]+m==n)
            {
                printf("]");
            }
            printf("\n");
        }


        tmpPWOA=tmpPWOA->next;

    }
    if(patternCount!=0)
    {
        printf("%d patterns detected in %d scanpaths\n",patternCount,simpleCounter);

        int max=0;
        int maxArray[10];

        for(i=0;i<10;i++)
        {
            if(frequentAOI[i]>max)
                max=frequentAOI[i];

        }
        int maxCount=0;

        for(i=0;i<10;i++)
        {
            if(frequentAOI[i]==max)
            {
                maxArray[maxCount]=i;
                maxCount++;
            }
        }

        printf("The most frequent AOI before the pattern is/are");
        for(i=0;i<maxCount;i++)
            printf("% d",maxArray[i]);
    }
    else
        printf("None");



}









void rabinKarp(char T[],char P[],int d, int q,int *patternCount,int indexArr[],int *indexArrCounter,int frequentAOI[],int *flag)
{
    //this is the rabin karp algorithm that helps me to search for patterns

    int i,j;
    int n = strlen(T);
    int m = strlen(P);
    int p = 0;
    int tZero = 0;
    int h=1;

    for(i=0;i<m-1;i++)
    {
        h=(h*d)%q;
    }

    for(i=0;i<m;i++)
    {
        p=(d*p+P[i])%q;
        tZero=(d*tZero+T[i])%q;

    }

    for(i=0;i<=n-m;i++)
    {
        if(p==tZero)
        {
            for(j=0;j<m;j++)
            {
                if(T[i+j]!=P[j])
                    break;
            }
            if(j==m)
            {

                *patternCount=*patternCount+1;// in this part, I found a pattern, so I increment the patterncount by one

                indexArr[*indexArrCounter]=i;//this is the part where I stored the information of pattern indexes
                *indexArrCounter=*indexArrCounter+1;

                int frequentAOIindex;//this is the part where I stored the information of the  most frequent AOI before the pattern
                frequentAOIindex=T[i-1]-48;// as the look for before the pattern, I take the value of T[i-1], -48 for the integer version of it
                frequentAOI[frequentAOIindex]++;
                *flag=1;
            }
        }

           if(i<n-m)
            {
                tZero=(d*(tZero-T[i]*h)+T[i+m])%q;

                if (tZero < 0)
                    tZero = (tZero + q);
            }
    }

}
