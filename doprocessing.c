#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <time.h>
#include <string.h>

void doprocessing (int sock) {
    int n;
    char client_buffer[256],my_buffer[256],buffer[100];

    if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
	int read_size;
	int i , p , num ;
	int no[10] ;
    int guess[4] , tmp , counterA , counterB , j , k ,count=1;
 
     while(count>0)
	{
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
	    bzero(my_buffer,256);
		bzero(client_buffer,256);	
	    sprintf(my_buffer,"Game%d:Please enter your guess number : ",count);
	    if((n=write(sock , my_buffer , strlen(my_buffer)))<0)
	    {
	      perror("ERROR writing to socket");
	      exit(1);		
		} 
	    int count2=0;
	    while( (read_size = recv(sock , client_buffer , 256 , 0)) > 0)
	    {
	        // 把輸入的數字分解成 千位, 百位, 十位, 個位, 並把結果放入一個陣>列中
	        int i,pointer;
	        for(i=0,pointer=3;client_buffer[i]!=0&&pointer>=0;i++)
	        {
	            if(client_buffer[i]>='0'&&client_buffer[i]<='9')
	               guess[pointer--]=client_buffer[i]-'0';
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
	
	        if( counterA == 4 )
	        {
               sprintf(my_buffer,"Right number!!");
			    if((n=write(sock , my_buffer , strlen(my_buffer)))<0)
			    {
			      perror("ERROR writing to socket");
			      exit(1);		
				} 
               count++;
               break;
	        } 
			else if(count2>4)
			{
               sprintf(my_buffer,"Game over!!ANS is:%d%d%d%d",no[3],no[2],no[1],no[0]);
			    if((n=write(sock , my_buffer , strlen(my_buffer)))<0)
			    {
			      perror("ERROR writing to socket");
			      exit(1);		
				} 
			   count++;
               break;				
			}
			else {
	                sprintf(my_buffer,"Hint%d:%dA%dB",++count2,counterA,counterB);
				    if((n=write(sock , my_buffer , strlen(my_buffer)))<0)
				    {
				      perror("ERROR writing to socket");
				      exit(1);		
					} 			
	            }
	            //Send the message back to client
		        bzero(my_buffer,256);
			    bzero(client_buffer,256);	
			    bzero(buffer,100);
	            sprintf(buffer,"\nGame%d:Please enter your guess number : ",count);
	            strcat(my_buffer,buffer);
			    if((n=write(sock , my_buffer , strlen(my_buffer)))<0)
			    {
			      perror("ERROR writing to socket");
			      exit(1);		
				} 
		    }
	}   
}
