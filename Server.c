#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <time.h>
int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char message[1000],client_message[2000];
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
            printf("Could not create socket");
    }
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
            puts("bind failed");
            return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (new_socket<0)
    {
            perror("accept failed");
            return 1;
    }
            puts("Connection accepted");

//Receive a message from client
    int read_size;
    int i , p , num ;
    int no[10] ;

    int guess[4] , tmp , counterA , counterB , j , k ,count=1;
    while(count>0)
	{
		memset(client_message,0,sizeof(client_message));
  		memset(message,0,sizeof(message));
	    for ( i = 0 ; i < 10 ; ++i )
	            no[i] = i;
	
	    srand( time(NULL) ) ;
	    // 由電腦隨機產生一組四個不同的數字
	    for ( i = 0 ; i < 4 ; ++i ) {
	            p = rand() % 10 ;
	            if ( p == i ) continue ;
	            tmp = no[i] ;
	            no[i] = no[p] ;
	            no[p] = tmp ;
	    }
	    printf("ANS:");
	    for(i=3;i>=0;i--)
	            printf("%d",no[i]);
	    printf("\n");
	    sprintf(message,"Game%d:Please enter your guess number : ",count);
	    write(new_socket , message , strlen(message));
	    int count2=0;
	    while( (read_size = recv(new_socket , client_message , 2000 , 0)) > 0)
	    {
	        // 把輸入的數字分解成 千位, 百位, 十位, 個位, 並把結果放入一個陣>列中
	        int i,pointer;
	        for(i=0,pointer=3;client_message[i]!=0&&pointer>=0;i++)
	        {
	            if(client_message[i]>='0'&&client_message[i]<='9')
	               guess[pointer--]=client_message[i]-'0';
	        }
	        // 判斷輸入數字與答案的符合程度
	        counterA = counterB = 0;
	        for( j = 0 ; j < 4 ; ++j )
	                for( k = 0 ; k < 4 ; ++k )
	                        if( no[j] == guess[k] )
	                                if( j == k )
	                                        counterA++;
	                                else
	                                        counterB++;
	
	            memset(message,0,sizeof(message));
	
	        if( counterA == 4 )
	        {
	               sprintf(message,"Right number!!");
	               write(new_socket , message , strlen(message));
	               count++;
	               break;
	        } 
			else if(count2>4)
			{
	               sprintf(message,"Game over!!ANS is:%d%d%d%d",no[3],no[2],no[1],no[0]);
	               write(new_socket , message , strlen(message));
	               count++;
	               break;				
			}
			else {
	                sprintf(message,"Hint%d:%dA%dB",++count2,counterA,counterB);
			
	            }
	            //Send the message back to client
	            write(new_socket , message , strlen(message));
	            memset(client_message,0,sizeof(client_message));
	            memset(message,0,sizeof(message));
	            sprintf(message,"Game%d:Please enter your guess number : ",count);
	            write(new_socket , message , strlen(message));
	
	    }
	}

    if(read_size == 0)
    {
            puts("Client disconnected");
            fflush(stdout);
    }
            else if(read_size == -1)
    {
            perror("recv failed");
	}
    return 0;
}
