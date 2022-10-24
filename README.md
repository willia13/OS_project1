Name: Project 1
Author: William Mitchell
Class: Operating Systems


# OS_project1
A mock server system that has 10 like counter servers which each send data to a main host server

LikeServer.c
-This file creates a process that acts like a server sending out Likes data to a host server 

ParentProcess.c
-This file creates 10 Different LikeServers 

PrimaryLikesServer.c
-This file receives the likes data as a host server and compiles it into one database.