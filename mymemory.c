/* mymemory.c
 * 
 * provides interface to memory management
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "shell.c"


// our memory
Byte        mymemory [MAXMEM] ;
Segment_t * segmenttable = NULL;


void initialize ()
{
   printf ( "initialize> start\n");
   // set memory to 0

   // create segment table
   // contains one segment description that declares the whole memory
   // as one free segment
   Segment_t segment = {
     .allocated = FALSE,
     .start = &mymemory,
     .size = MAXMEM};
   /*segmenttable->allocated = FALSE;
   segmenttable->start = &mymemory;
   segmenttable->size = MAXMEM;*/
   printsegmentdescriptor(&segment);
   segmenttable = &segment; 
   printsegmentdescriptor(segmenttable);
   printf("Segmentable and segmenttable next initialize: %p %p \n", segmenttable, segmenttable->next);
   
   //
   // create a single segment descriptor
   
   // initialise the segment

   printf ( "initialize> end\n");
}



void * mymalloc ( size_t asize )
{
   printf ( "mymalloc> start\n");
   printf("Segmentable and next mymalloc: %p %p \n", segmenttable, segmenttable->next );
  // implement the mymalloc functionality
  Segment_t * place;
  Segment_t NewSegment;
  place = findFree(segmenttable,asize);
  printsegmentdescriptor(place);
  if (place == NULL) {printf("Not enough space");}
  else{
    place->size = place->size - asize;
    NewSegment.allocated = TRUE;
    NewSegment.size = asize;
    NewSegment.start = place->start;
    printf("%ld %x %p \n", NewSegment.size, NewSegment.allocated, NewSegment.start);
    printf("%x ", segmenttable->allocated);
  
    printsegmentdescriptor(&NewSegment);
    printsegmentdescriptor(segmenttable);
    
    printf("MyMalloc place pointer: %p \n", place);
    printf("MyMalloc segmentable pointer: %p \n", segmenttable);
    printf("MyMalloc NewSegment pointer: %p \n", &NewSegment);
    insertAfter(place, &NewSegment);
  };
  
  printf ( "mymalloc> end\n");
}

void myfree ( void * ptr )
{
   printf ( "myfree> start\n");

}

void mydefrag ( void ** ptrlist)
{
   printf ( "mydefrag> start\n");

}


// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t searchsize )
{
   printf ( "findFree> start\n");
  
   Segment_t * ptr = list;
   printsegmentdescriptor(ptr);
   printsegmentdescriptor(list);
   while (TRUE) {
      printf("myfree Segmentable: %p \n",list);
      printf("myfree Segmentable.next: %p \n",ptr->next);
      if(ptr->allocated == FALSE){
        
        if(ptr->size >= searchsize){
          printf("%x ", ptr->allocated);
          printsegmentdescriptor(ptr);
          return ptr;
        };
           
      }; 

    if (ptr->next->allocated == 0 || ptr->next->allocated == 1) {ptr = ptr->next;}
    else {break;};
    
  };
  

  printf ( "findFree> end\n");
}

void insertAfter ( Segment_t * oldSegment, Segment_t * newSegment )
{
  printf ( "insertAfter> start\n");
  Segment_t * ptr = oldSegment;
  printf("ptr: %p  newSegment: %p Segmentable: %p \n",ptr,newSegment,segmenttable);
  if (oldSegment->next->allocated == 0 || oldSegment->next->allocated == 1) {
    Segment_t * Temp = oldSegment->next;
    oldSegment->next = newSegment;
    newSegment->next = Temp;

    
  }
  else {
    ptr->next = newSegment;
  };
  printf("ptr: %p  newSegment: %p Segmentable: %p \n",ptr,newSegment,segmenttable);
  printf ( "insertAfter> end\n");
}

Segment_t * findSegment ( Segment_t * list, void * ptr )
{
}

int isPrintable ( int c )
{
   if ( c >= 0x20 && c <= 0x7e ) return c ;

   return 0 ;
}

void printmemory ()
{
    Byte * ptr;
    ptr = segmenttable->start; //Copy of pointer
    
    Byte ListaByte[10]; //List for later printing
    int z = 0;
     for (int i = 0; i<MAXMEM; i++ ){ //Iterate throw all the values in memory
       ListaByte[i%10] = *ptr; //Using modulo for index
         
       if (i % 10 == 0 && i != 0) //10 values
         {
         
           printf("[%4d] ", z); //Table index
         
           for (int j= 0; j<10; j++){ //Printing Hex
         
             printf("%02x ", ListaByte[j]);
                   
           };
           
           printf("| "); 
         
           for (int j= 0; j<10; j++){ //Printing Char
             if (isPrintable(ListaByte[j])==0) 
               { printf(".");}
             else 
               { printf("%c ", ListaByte[j]);};
           
          };
       z += 10;
       printf("\n");
       };
       ptr++; //Moving copy of pointer
     };
    if (MAXMEM % 10 != 0 ){ //Printing any non multiple of 10
           
        printf("[%4d] ", z);
         
        for (int j= 0; j<(MAXMEM%10); j++){
         
             printf("%02x ", ListaByte[j]);
                   
           };
              for (int j= 0; j<(10 - MAXMEM%10); j++){
         
             printf("__ ");
                   
           };
           
        printf("| ");
         
        for (int j= 0; j<(MAXMEM%10); j++){
             if (isPrintable(ListaByte[j])==0) 
               { printf(".");}
             else 
               { printf("%c ", ListaByte[j]);};
           
      
      
        };

    };
  printf("\n");
}

void printsegmenttable()
{
  Segment_t * ptr = segmenttable;
  printf("semgentable next and ptr next printsegmentable: %p %p \n", segmenttable->next, ptr->next);
  while (TRUE) {
    printsegmentdescriptor(ptr);
    printf("Allocated = %d \n", ptr->allocated );
    printf("start = %p  ", ptr->start );
    printf("next = %p" , ptr->next );
    printf("size = %lu \n", ptr->size );
    if (ptr->next->allocated == 0 || ptr->next->allocated == 1) {ptr = ptr->next;}
    else {break;};
    break;
  };

}

void printsegmentdescriptor ( Segment_t * descriptor )
{
      printf ( "\tallocated = %s\n" , (descriptor->allocated == FALSE ? "FALSE" : "TRUE" ) ) ;
      printf ( "\tstart     = %p\n" , descriptor->start ) ;
      printf ( "\tsize      = %ld\n", descriptor->size  ) ;
      printf ( "\twhere     = %p \n", descriptor);
}

