/**Name:Mohammad Belbesi**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define BUFLEN 7000
int flag =1;
/*
int port_num_flag = 0;
*/
int url_flag=0;
int post_flag=0;
int get_flag=0;
int zero_arg_flag=0;
int args_length=0;
int http_flag=0;
int httpIndx=0;

int main(int argc, char* argv[]) {
    int port_num = 80;
    char port_num_string[5]={0};

    int n_txt_num=0;
    int n_arg_num;

    char n_num_string[5]={0};
    char n_num_string_p[5]={0};//for -p
    char arg_str[1000]={0};
    char* path=NULL;
    char* http_url=NULL;
    /* char path[1000];
     char http_url[1000];*/
    char* txt=NULL;

/*
    printf("the 1st el is %c\n",path[0]);
*/

/*
    if(path==NULL){
        perror("the allocated memory is failed!");
        EXIT_FAILURE;
    }
    path[0]='/';
    if(http_url==NULL){
        perror("the allocated memory is failed!");
        EXIT_FAILURE;
    }
*/

    int i,j,k,x;
    if(argc<2) {//if the user didn't write anything in the command line
        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
        exit(0);
    }
    for (i = 0; i < argc; i++) {
        if(strstr(argv[i],"http://")!=NULL){
            if(argv[i][0]=='h'){
                http_flag=1;
                httpIndx=i;
            }
        }
    }

    for (i = 0; i < argc; i++) {//for loop to run on the argv arguments that the user wrote in the command line
        /*if(strstr(argv[i],"http://")==NULL && (strstr(argv[i],"www.")!=NULL ||strstr(argv[i],".co")!=NULL)){//to check if the url have http://
            printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
            exit(0);
        }*/
        if(http_flag==0){
            printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
            exit(0);
        }
        if(strstr(argv[i],"http://")!=NULL){//if the loop is on url
            url_flag=1;
            k=0;
            http_url= (char*)malloc((strlen(argv[i])+1)*sizeof(char));
            memset(http_url,0,(strlen(argv[i])+1)*sizeof(char));

            if(http_url==NULL){
                perror("the allocated memory is failed!");
                exit(EXIT_FAILURE);
            }
            for (j = 7; j < strlen(argv[i]) && argv[i][j]!=':' && argv[i][j]!='/'; j++,k++) {//save the http_url
                http_url[k]=argv[i][j];
            }
            http_url[k]='\0';

            k=0;
            for (j = 7; j < strlen(argv[i]); j++) {//check the port num and save it
                if(argv[i][j]==':' && argv[i][j+1]!='-'){//if there are a port num
                    x=0;
                    j++;

/*
                    port_num_flag=1;
*/

                    while(flag==1){//while loop to check the num and to convert it to an int number
                        if(isdigit(argv[i][j])!=0 && x<5){//is digit return a number or zero if it's not a num
                            port_num_string[x]=argv[i][j];
                            j++;
                            k++;
                            x++;
                        }
                        else{
                            if((isdigit(argv[i][j])!=0 && x==5)){//if the num is longer than 5 digits that's mean it's for sure bigger than 2^16
                                printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                                free(http_url);
                                exit(0);
                            }

                            flag=0;//to break the inf loop
                            port_num_string[k]='\0';
                        }
                    }
                    port_num= atoi(port_num_string);//convert the num from string to int number
                    int max_port_num = (int)pow(2,16);
                    if(port_num<0 || port_num >= max_port_num){//check if it's bigger than 2^16
                        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                        free(http_url);
                        exit(0);
                    }
                }

                if(argv[i][j]==':' && argv[i][j+1]=='-'){//if the port_num is negative print the usage and exit
                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                    free(http_url);
                    exit(0);
                }

            }
/*
            printf("the num is: %d\n",port_num);
*/
            k=0;
            path=(char*)malloc((strlen(argv[i])+1)*sizeof(char));
            memset(path,0,(strlen(argv[i])+1)*sizeof(char));
            if(path==NULL){
                free(http_url);
                perror("the allocated memory is failed!");
                exit(EXIT_FAILURE);
            }
            path[0]='/';

            for (j = 7; j < strlen(argv[i]); j++) {// save the path in path string
                if(argv[i][j]=='/'){
                    while (k< strlen(argv[i])){
                        path[k]=argv[i][j];
                        k++;
                        j++;
                    }
                }
            }
            if(k!=0){
                path[k]='\0';
            }

/*
            printf("the 1st el is %c\n",path[0]);
*/


        }

    }
    int s,d=0;
    for(s =1; s < argc; s++){//for loop to check -r and -p

        // for (d = 0; d < strlen(argv[s]); d++) {
        if(argv[s][d] == '-' && argv[s][d + 1] != 'p' && argv[s][d + 1] != 'r' && s!=httpIndx){
            printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
            free(http_url);
            free(path);
            exit(0);
        }
        d=0;
        if(argv[s][d] == '-' && argv[s][d + 1] == 'p' && url_flag == 1){//if we have -p
            if(s==argc-1){
                printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                free(http_url);
                free(path);
                exit(0);
            }
            if(isdigit(argv[s + 1][d]) != 0){//check the num
                k=0;
                x=0;
                while(x< strlen(argv[s + 1])){
                    n_num_string_p[k]=argv[s + 1][d];
                    d++;
                    k++;
                    x++;
                }
                n_txt_num= atoi(n_num_string_p);
                if(n_txt_num<=0){//if the num is zero or less print usage message and exit
                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                    free(http_url);
                    free(path);
                    exit(0);
                }
                txt=(char*) malloc((n_txt_num+1) * sizeof (char));
                memset(txt,0,(n_txt_num+1) * sizeof (char));
                if(txt==NULL){
                    free(http_url);
                    free(path);
                    perror("the allocated memory is failed!");
                    exit(EXIT_FAILURE);
                }

                if(strlen(argv[s + 2]) == n_txt_num){//if the txt length is the same of the n number
                    strcpy(txt,argv[s + 2]);
                    post_flag=1;
                    txt[n_txt_num]='\0';
                    /*printf("txt num is: %d\n",n_txt_num);
                    printf("txt length %lu\n", strlen(txt));
                    printf("the text is%s",txt);*/
                }
                else{//if the length of the txt is greater or smaller than n print the usage message and exit
                    free(http_url);
                    free(path);
                    free(txt);
                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                    exit(0);
                }

            }
            else{
                free(http_url);
                free(path);
                printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                exit(0);
            }
        }

        d=0;
        if(argv[s][d] == '-' && argv[s][d + 1] == 'r' && url_flag == 1){//if we have -r
            if(s==argc-1){
                printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                free(http_url);
                free(path);
                if(post_flag==1){
                    free(txt);
                }
                exit(0);
            }
            if(isdigit(argv[s + 1][d]) != 0){//check the num
                k=0;
                x=0;
                memset(n_num_string,0,sizeof (n_num_string));
                while(x< strlen(argv[s + 1])){
                    n_num_string[k]=argv[s + 1][d];
                    d++;
                    k++;
                    x++;
                }
                n_arg_num= atoi(n_num_string);

                if(n_arg_num==0){//check if the num n is zero we should have zero args
                    zero_arg_flag=1;
                    if(argv[s + 1][d] != '-' && (strstr(argv[s + 1], "http://") == NULL)){
                        free(http_url);
                        free(path);
                        if(post_flag==1){
                            free(txt);
                        }
                        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                        exit(0);
                    }
                }
                else if(n_arg_num>0){//check if the args are proper
                    arg_str[0]='?';
                    args_length++;
                    int arg_counter=0;
                    if(argv[s + 1][0] != '-' && (strstr(argv[s + 1], "http://") == NULL)){
                        s++;
                        while(s < argc && arg_counter < n_arg_num){
                            if(argv[s][0] != '-' && strstr(argv[s], "http://") == NULL ){
//                                printf("argv[s + 1]= %s\n" , argv[s + 1]);
                                if(strstr(argv[s + 1], "=") == NULL){//if one of the args don't have '=' print the usage message and exit
                                    free(http_url);
                                    free(path);
                                    if(post_flag==1){
                                        free(txt);
                                    }
                                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                                    exit(0);
                                }
                                s++;
                                strcat(arg_str, argv[s]);
                                args_length+=(int) strlen(argv[s]);
                                arg_counter++;
                                if(arg_counter!=n_arg_num){
                                    strcat(arg_str,"&");
                                    args_length++;
                                }
                                else{
                                    //strcat(arg_str," ");
                                    args_length++;
                                    arg_str[args_length]='\0';
                                    get_flag=1;
                                }
                            }

                        }
/*
                        printf("%s",arg_str);
*/
                    }

                    else{//if the args num is smaller than n print the usage message and exit
                        free(http_url);
                        free(path);
                        if(post_flag==1){
                            free(txt);
                        }
                        printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                        exit(0);
                    }

                }
                else{//if the args num is negative print the usage message and exit
                    free(http_url);
                    free(path);
                    if(post_flag==1){
                        free(txt);
                    }
                    printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n");
                    exit(0);
                }

            }
        }

    }

    /************  my server *********************/

    /************  my server *********************/

    char* request;
    unsigned long req_length=0;
    if(post_flag==1||get_flag==1){
        req_length= strlen(http_url)+ strlen(path)+strlen(" HTTP/1.0/r/nHost: ")+strlen("/r/n/r/n");
        if(post_flag==1){
            req_length+= strlen("POST ")+ strlen("/r/nContent-length:")+ strlen(txt)+ strlen(n_num_string_p);
            if(get_flag==1){
                if(zero_arg_flag!=1){
                    req_length+= strlen(arg_str);

                }
            }
        }
        if(post_flag==0 && get_flag==1){
            req_length+= strlen("GET ")+ strlen(arg_str);
        }
    }

    else{
        req_length+= strlen("GET ")+ strlen(path)+ strlen(" HTTP/1.0/r/nHost: ")+ strlen(http_url)+ strlen("/r/n/r/n");
    }
    request=(char*) malloc((req_length+1)*sizeof (char));
    memset(request,0,(req_length+1)*sizeof (char));
    /*******************************/
    if(post_flag==1){
        strcat(request,"POST ");
        strcat(request,path);
        if(get_flag==1){
            if(zero_arg_flag!=1){
                strcat(request,arg_str);
            }
        }
        strcat(request," HTTP/1.0\r\nHost: ");
        strcat(request,http_url);
        strcat(request,"\r\nContent-length:");
        char str_txt_num[10];
        sprintf(str_txt_num, "%d",(int) n_txt_num);
        strcat(request,str_txt_num);
/*
        strcat(request,n_num_string_p);
*/
        strcat(request,"\r\n\r\n");
        strcat(request,txt);
    }
    else if(post_flag==0 && get_flag==1){
        strcat(request,"GET ");
        strcat(request,path);
        if(zero_arg_flag!=1){
            strcat(request,arg_str);
        }
        strcat(request," HTTP/1.0\r\nHost: ");
        strcat(request,http_url);
        strcat(request,"\r\n\r\n");
    }
    else{
        strcat(request,"GET ");
        strcat(request,path);
        strcat(request," HTTP/1.0\r\nHost: ");
        strcat(request,http_url);
        strcat(request,"\r\n\r\n");
    }
    printf("HTTP request =\n%s\nLEN = %lu\n", request, strlen(request));
    /***********************/
    struct hostent *hp; //ptr to host info for remote
    struct sockaddr_in peeraddr;
    peeraddr.sin_family = AF_INET;
    hp = gethostbyname(http_url);
    if(hp==NULL){
        free(http_url);
        free(path);
        if(post_flag==1){
            free(txt);
        }
        free(request);
        herror("gethostbyname");
        exit(EXIT_FAILURE);
    }
    peeraddr.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;

    int fd;				/* socket descriptor */
    //struct sockaddr_in srv;		/* used by connect() */

/* create the socket */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        free(http_url);
        free(path);
        if(post_flag==1){
            free(txt);
        }
        free(request);
        perror("socket");
        exit(1);
    }

/* connect: use the Internet address family */
    peeraddr.sin_family = AF_INET;

/* connect: socket ‘fd’ to port */
    peeraddr.sin_port = htons(port_num);

/* connect: connect to IP Address hp */
    peeraddr.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;

    if(connect(fd, (struct sockaddr*) &peeraddr, sizeof(peeraddr)) < 0) {
        free(http_url);
        free(path);
        if(post_flag==1){
            free(txt);
        }
        free(request);
        perror("connect");
        exit(1);
    }

    int nbytes;				/* used by write() */

/* 1) create the socket */
/* 2) connect() to the server */
/*    char *eeeeeeee="Get / HTTP/1.0\r\nHost: www.google.com\r\n\r\n";
    char *eeeeeee="GET /55fd0dab-7a20-4681-87ca-69832a2a8081 HTTP/1.0\r\nHost:webhook.site\r\n\r\n";*/

/* Example: A client could “write” a request to a server */
    nbytes = (int )write(fd, request, strlen(request));
    if(nbytes < 0) {
        free(http_url);
        free(path);
        if(post_flag==1){
            free(txt);
        }
        free(request);
        perror("write");
        exit(EXIT_FAILURE);
    }

    unsigned char* buf = (unsigned char *) malloc((BUFLEN+1)*sizeof (unsigned char));
    memset(buf,0,(BUFLEN+1)*sizeof (unsigned char));
    nbytes=0;
    while (1){
        /*  nbytes = (int )write(fd, request, sizeof (request));
          if(nbytes < 0) {
              perror("write");
              exit(EXIT_FAILURE);
          }*/
        bzero(buf,BUFLEN);
        int nRead=(int)read(fd,buf,BUFLEN);
        nbytes+=nRead;
        if(nRead==0){
            break;
        }
        /* if(strstr(buf,"\r\n\r\n")){
             break;
         }*/

        buf[nRead] ='\0';
        //printf("%s",(char*)buf);
        write(STDOUT_FILENO,buf,nRead);

    }
    printf("\n Total received response bytes: %d\n",nbytes);
    close(fd);
    free(http_url);
    free(path);
    if(post_flag==1){
        free(txt);
    }
    free(request);
    free(buf);

    return 0;
}
