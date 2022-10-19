/* 
ParentProcess.c
This file creates 10 child processes
*/
void parentProcess(){
  int times = 0;

  cleanLogs();
  makeLog("Parent Process began at: ", 0, 0){

  
  //creates 10 processes
  for(int ii = 0; ii<9; ii++){
    if (fork() == 0){
      ii = 10; //ends loop
    }
    else
      times++;      
  }
  makeLog("Child Process

  if (times == 0){ // if the first parent
    //wait();
    
  }
  return;
}

int main(){
  parentProcess();
}
