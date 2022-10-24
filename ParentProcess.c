/* 
ParentProcess.c
This file creates 10 child "LikeServer" processes.
Each "LikeServer" process will send the amount of likes to the PrimaryLikesServer.

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#define PORT 8080

//prints out the log file
void makeLog(char* message, int which_file, int like_server_num, int timestamp){
  char* files[] = {"./tmp/ParentProcessStatus","./tmp/PrimaryLikesLog","./tmp/LikeServer0","./tmp/LikeServer1","./tmp/LikeServer2","./tmp/LikeServer3","./tmp/LikeServer4","./tmp/LikeServer5","./tmp/LikeServer6","./tmp/LikeServer7","./tmp/LikeServer8","./tmp/LikeServer9"};

  char* log_file = files[which_file + like_server_num];

  //edits the log file
  FILE* file = fopen(log_file,"a");
  fputs(message, file);
  if (timestamp == 1){//if we want the timestamp
    time_t actual_time;
    time (&actual_time);
    fputs( ctime(&actual_time), file);
  }
  fclose(file);

}

int contactPrimaryServer(int server_num, int likes){
  int like_socket = 0, value_read, client_number;
  struct sockaddr_in server_address;
  char message[] = "LikeServerX 00";
  char buffer[1024] = { 0 };
  if ((like_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    makeLog("Error occured in the creation of the Socket at: ",2, server_num, 1);
    return 1;
  }
  
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary
  // form
  if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
    makeLog("Error with the address at :", 2, server_num,  1);
    return 1;
  }
  
  if ((client_number  = connect(like_socket, (struct sockaddr*)&server_address, sizeof(server_address))) < 0) {
    makeLog("The connection failed at : ", 2, server_num, 1);
    return 1;
  }

  //creates message
  //if to close the server
  if(server_num == -1 && likes == -1){
    char* kill_message = "kill";
    send(like_socket, kill_message, strlen(kill_message), 0);
  }
  
  else{ // if a regular message
    message[10] = ((char) server_num) + 48;
    message[12] = ((char) likes/10) + 48;
    message[13] = ((char) likes%10) + 48;  
    send(like_socket, message, strlen(message), 0);
  }
  makeLog("Message sent at: ", 2, server_num, 1);
  value_read = read(like_socket, buffer, 1024);
     
  // closing the connected socket
  close(client_number);
  return 0;
}



int LikeServer(int server_number){
  const int five_minutes = 60*5;
  int total_time = 0;
  int random_time;
  int likes = 0;
  char* numbers[] = {"0","1","2","3","4","5","6","7","8","9"};
  //creates log file
  clock_t start_time = clock();
  
  //initilizes the random number generator
  srand(server_number + time(0)); 

  time_t start, end;
  time(&start);
  time(&end);
  //checks to see if the 5 minutes has passed
  while (difftime(end,start) < (double)five_minutes){
    //gets next waiting period between 1 and 5 seconds
    random_time = rand()%5 + 1;
    //gets random amount of likes between 0 and 42
    likes = rand()%43;
    
    total_time = total_time + random_time;

    /*    
    //to prevent the system from going over time
    if(total_time > five_minutes){
      int old_random = random_time;
      random_time = total_time - five_minutes;
      total_time = total_time - old_random + random_time; 
      
    }
    */
    
    sleep(random_time);
    //sends message to primary server
    makeLog("Sent message to PrimaryLikeServer",2,server_number,1);
    int exit_code = contactPrimaryServer(server_number, likes);
    //logs that this event ends
    time(&end);
    if (exit_code != 0)
      return exit_code;
  }
  printf("%i", server_number);
  return 0;
}

//cleans up the log files and creates them if nessisary
void cleanLogs(){
//creates blank log files
  char* files[] = {"./tmp/ParentProcessStatus","./tmp/LikeServer0","./tmp/LikeServer1","./tmp/LikeServer2","./tmp/LikeServer3","./tmp/LikeServer4","./tmp/LikeServer5","./tmp/LikeServer6","./tmp/LikeServer7","./tmp/LikeServer8","./tmp/LikeServer9"};

  FILE* log_file;
  for (int jj = 0; jj < 11; jj ++){
    log_file = fopen(files[jj],"w+");
    fclose(log_file);
  }
}

//this is the parent process that starts up the 10 files
void ParentProcess(){
  int times = 0;
  //naming the process
  prctl(PR_SET_NAME, "ParentProcess");
  cleanLogs();
  makeLog("Parent Process began at: ", 0, 0, 1);

  char* server_nums[] = {"0","1","2","3","4","5","6","7","8","9"};
  char process_name[] = "LikeServerX";
  //creates 10 processes
  for(int ii = 0; ii<10; ii++){
    if (fork() == 0){//if child
      ii = 11; //ends loop
    }
    else //if parent
      times++;  
    sleep(1);
  }

  
  if(times != 10){//if not the parent
    process_name[10] = (char)times + 48;
    //name the process
    prctl(PR_SET_NAME, process_name);
    
    makeLog("LikeServer", 0,0,0);
    makeLog(server_nums[times], 0,0,0);
    makeLog(" process created: ", 0,0, 1);

    int stuff = LikeServer(times);
    if (stuff == 0){
    //  if (LikeServer(times) == 0){ //if child was successful
      makeLog("LikeServer", 0,0,0);
      makeLog(server_nums[times], 0,0,0);
      makeLog(" process finished: ", 0,0, 1);
    }
    else{ //if child failed
      printf("%i", stuff);
      makeLog("LikeServer", 0,0,0);
      makeLog(server_nums[times], 0,0,0);
      makeLog(" process failed: ", 0,0, 1);
    }
  }
  else{//parent
    wait(NULL);
    makeLog("All child processes have ended\n",0,0,0);

    //informs the server to end
    sleep(15);
    contactPrimaryServer(-1,-1);
  }
  
  return;
}

int main(){
  ParentProcess();



}
