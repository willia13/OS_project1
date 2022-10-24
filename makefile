make:
	gcc    	PrimaryLikesServer.c -o primary
	gcc 	ParentProcess.c -o parent


ParentProcess:
	./parent
PrimaryLikesServer:
	./primary
