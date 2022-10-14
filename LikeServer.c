/*
LikeServer.c
This process generates random amounts of likes and sends the count to the Primary Likes Server
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int LikeServer(){
  const int five_minutes = 60*5;
  int total_time = 0;
  int random_time;
  int likes = 0;
  //initilizes the random number generator
  srand(time(0)); 
  
  //checks to see if the 5 minutes has passed
  while ( total_time < five_minutes){
    //gets next waiting period between 1 and 5 seconds
    random_time = rand()%5 + 1;
    //gets random amount of likes between 0 and 42
    likes = rand()%43;

    total_time = total_time + random_time;

    sleep(random_time);
    printf("%l %l", random_time, likes);
    //sends message to primary server
  }

}

int main(){
  printf("hello");
  LikeServer();
  return 0;
}
