# OS_project1
A mock server system that has 10 like counter servers which each send data to a main host server

To use this program, run make on the makefile to compile the files.
Then run the executables in order:  ./PrimaryLikesServer   ./ParentProcess
The PrimaryLikesServer contains the PrimaryLikesServer, while the ParentProcess contains both the ParentProcess and the LikeServer.
The files will leave their temp files in the /tmp/ directory.

Also after 10 seconds after the ParentProcess ends, the PrimaryLikesServer will automatically end as well.
