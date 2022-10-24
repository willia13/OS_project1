/*
PrimaryLikesServer.c

This process will take input from the 10 Like Servers and compile the data together
*/
#include <sys/prctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 8080

//prints out the log file
void makeServerLog(char* message, int integer, int timestamp){
  char* log_file = "./tmp/PrimaryLikesLog";

  //edits the log file
  FILE* file = fopen(log_file,"a");
  fputs(message, file);

  if (integer != -1)
    fprintf(file, "%d", integer);

  if (timestamp == 1){//if we want the timestamp
    time_t actual_time;
    time (&actual_time);
    fputs( ctime(&actual_time), file);
  }
  fclose(file);

}


//collects data from the servers
int PrimaryLikeServer(){

  //names the process
  prctl(PR_SET_NAME, "PrimaryLikeServer");

  //initialilzes the log file
  
  FILE* file = fopen("./tmp/PrimaryLikesLog","w+");
  fclose(file);
 
  
  // this code is based off of tutorialspoint.dev socket programming
  
  //initilizing
  int server_socket, socket_1, value_read;
  struct sockaddr_in address;
  int opt = 1;
  int address_length = sizeof(address);
  char new_data[1024] = { 0 };
  char* success = "Primary Server Hears you";
  int total_likes = 0;
  char* char_total_likes = {"999999"};
  char* numbers = "0123456789";//"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
  //create socket
  if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    makeServerLog("Could not create socket :", -1, 1);
    printf("socket");
    return -1;
  }
  //sets socket's opt
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))){
    makeServerLog("could not set the socket opt : ",-1, 1);
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  
  //binds the socket
  if((bind(server_socket, (struct sockaddr*)&address, sizeof(address)))<0){
    makeServerLog("bind failed :",-1,1);
    printf("bind");
    return -1;
  }
   
  //starts to listen
  if (listen(server_socket, 3) < 0){
    makeServerLog("Listen failed : ",-1, 1);
    printf("listen");
    return -1;
  }
  

  while(1){//infinite loop
  //accept 
  if (( socket_1 = accept(server_socket, (struct sockaddr *) &address, (socklen_t *)& address_length))<0){
    makeServerLog("accept failed : ",-1, 1);
    printf("accept");
    return -1;
  }
  //message recieved is now in new_data
  value_read = read(socket_1, new_data, 1024);
  
  //send message
  send(socket_1, success, strlen(success), 0);
  close(socket_1);

  //if message is to end this process
  char* ending = "kill";
  int end = 1;
  for(int jj = 0; jj <4; jj++){
    if (new_data[jj] != ending[jj]){
      end = 0;
      break;
    }
  }
  if (end == 1){ // if the kill process has been called
    sleep(30);
    shutdown(server_socket, SHUT_RDWR);
      return 0;
  }

   //updates the log file
  //each message s in form "LikeServerX XX", where X is a number

  char server_name[11] = "LikeServer ";
  server_name[10] = new_data[10]+48;
  int new_likes = (((int) new_data[12])-48)*10 + (int) new_data[13] - 48;
  total_likes += new_likes;
   
  makeServerLog(new_data,-1, 0);

  //records total likes
  makeServerLog("\nTotal Likes ",total_likes, 0);
  makeServerLog("\n",-1,0);
  }
}

int main(){
  if(  PrimaryLikeServer() == -1)
    makeServerLog("Server failed and ended at : ", -1, 1);
  else
    makeServerLog("Server shutdown at : ", -1, 1);
}
