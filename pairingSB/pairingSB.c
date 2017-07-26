#include "pairingSB.h"
#include "SecurityModel.h"
#include "videoSB.h"

char *sdip;
//char *publicip;
//char *publicport;
char pairing[5];
char pairingkey[5];
json_object *jobj1,*jobj2,*jobj3,*jobj4,*jobj5,*jobj6,*jobj7;
char myip[INET_ADDRSTRLEN]; //myip is this settop box's ip
char addressBuffer[INET_ADDRSTRLEN];	
char *Key;
char *UUID;


char *genKey(char *UUID){
	uniaddr.sin_family = AF_INET;
    uniaddr.sin_port = htons(SENDPORT);
    inet_pton( AF_INET,sdip, &( uniaddr.sin_addr.s_addr)  );

	Key = getKey(UUID);
	
	jobj7 = json_object_new_object();
	json_object *jstring7 = json_object_new_string(Key);	
	json_object_object_add(jobj7,"Key",jstring7);
	printf("[gen] Key  %s\n",Key);

    if( sendto(sockfd,json_object_to_json_string(jobj7), strlen( json_object_to_json_string(jobj7) ) ,
            0,  ( struct sockaddr * )&uniaddr, sizeof( uniaddr ) ) != -1) {             	
    }//if send Key
	else{
		printf("Send Key fail!\n");
		perror("ERROR MESSAGE");
	}//else
 return Key;
}//genKey

char *sendKey(char *checkUUID){
	char data[150];
	char *nowKey = malloc(sizeof(char)*100);
	FILE *fp_r = fopen("savedKey.txt", "r");	 
	if(fp_r == NULL)
	 	return;
	
	printf("Read File\n");
	while(!feof(fp_r))
	{
		char *ip;
		char *key;
		fscanf(fp_r,"%s", data);
		ip = strtok(data, "@");
		key = strtok(NULL, "@");
		if(strcmp(checkUUID, ip)== 0)
		{
			printf("Scucces Check IP:%s\n", ip);
			printf("Scucces Check KEY:%s\n", key);
			if(key != NULL)
			{
				strcpy(nowKey,key);
				printf("Scucces KEY:%s\n", nowKey);
			}
		}
	}
	
	return nowKey;

/*
    char *Key = malloc(sizeof(char)*50);
    char *checkip = malloc(sizeof(char)*17);
    char *temp = malloc(sizeof(char)*70);
    int i,longstr,j,k;
    char change = '0'; 
    FILE *fp;
    char StrLine[70];
    if((fp = fopen("savedKey.txt","r")) == NULL) { //判断文件是否存在及可读
	printf("error!");
	return -1;
    } // if
    while (1 ) {
	if (feof(fp)) break;
	fgets(StrLine,70,fp);  //读取一行
	//printf("%s\n", StrLine); //输出
	longstr = strlen(StrLine) ;
	for ( i=0,j=0,k=0 ; j<44; i++ ) {
		if (StrLine[i] == '@'){
			change = '1' ;
			i++;
		} // if
		if (change == '1'){
			//printf("11111Strline:%c\n", StrLine[i]);
			Key[j] = StrLine[i];		
			j++;		
		} // if
		else {
			//printf("0000Strline:%c\n", StrLine[i]);
			checkip[k] = StrLine[i];
			k++;
		} // else
		
		
	} // for
	if(strcmp(checkip,url)==0){
		printf( "checkip:%s\n", checkip );
		printf( "Key:%s\n", Key );
		return Key;	
		}
	change = '0';
    } // while
    printf("The ip is not found!\n");
    fclose(fp);
    return 0 ;
*/
}//char sendKey


bool saveKey(char *Key){
	char *save = malloc(sizeof(char)*70);
	memset(save,'\0',sizeof(save));
	FILE *f = fopen("savedKey.txt","a+");
	printf("save sdip : %s\n",UUID);
	strcpy(save,UUID);
	strncat(save,"@",1);
	strncat(save,Key,strlen(Key));
	strncat(save,"\n",1);
	
	if(fwrite(save,1,strlen(save),f) != 0){
		fclose(f);
		return TRUE;
	}
	else {
		fclose(f);
		return FALSE;
	}	
}//saveKey

int make_socket_non_blocking ( int sockfd ){
    int flags, s;   
    flags = fcntl (sockfd, F_GETFL, 0);     
    if (flags == -1){
        perror ("fcntl");
        return -1;
    }// if flag == -1
                           
    flags |= O_NONBLOCK;
    s = fcntl (sockfd, F_SETFL, flags);
     
    if (s == -1){
        perror ("fcntl");
        return -1;
    }//if s==-1                                                    
    return 0;
}// int make_socket_non_blocking

char *pGetSB_IP(void){
	struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;
	getifaddrs(&ifAddrStruct);
    int ipnum=0;
	char *address = malloc(sizeof(char)*17);		
    while (ifAddrStruct!=NULL) {     
    if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr,address, INET_ADDRSTRLEN);            
            if(ipnum==1) {                  
            	return address;	
            	break;	            
            }// if ipnum==1                   
        ipnum++; 
    }//if ifAddr  		    
        ifAddrStruct=ifAddrStruct->ifa_next;
    }//while ifAddrStruct!=NULL
//-------------------------------
 }//pGetSB_IP 

void pSendBcast(void *ptr){
	char *sbip = malloc(sizeof(char)*17);
	sbip = (char *)ptr;
	printf("IP : %s\n",sbip);
    printf("SBNAME : SBTEST\n");
   	 jobj1 = json_object_new_object();
  	 jobj2 = json_object_new_object();
	 json_object *jstring1 = json_object_new_string( sbip );
     json_object *jstring2 = json_object_new_string( "SBTEST" );
     json_object_object_add(jobj1,"ip", jstring1);
     json_object_object_add(jobj2,"sbname", jstring2);
	printf("[debug] sbip : %s\n",sbip);
	char *dot=".";
    char *pch,*aft;    
    int a = 0;   
    char *broip = malloc(sizeof(char)*17);   

    pch = strtok(sbip,dot);    
    while (pch != NULL){       	
        if(a == 3){      	
        	strcat(broip,"255");         	
        break;  
        }//if a == 3                               
        else{      	
        	strcat(broip,pch);     	                        
        	strcat(broip,".");     	    
        	pch = strtok (NULL,dot);      	
        	a++;
        }//else
    }//while pch != NULL 
    printf("[debug] broip : %s\n",broip);

	dstaddr.sin_family = AF_INET;
    dstaddr.sin_port = htons(SENDPORT);
    inet_pton( AF_INET, broip, &( dstaddr.sin_addr.s_addr ) );
//-------------------------------
while(1){
        if( sendto(sockfd,json_object_to_json_string(jobj1), strlen( json_object_to_json_string(jobj1) ) ,
            0,  ( struct sockaddr * )&dstaddr, sizeof( dstaddr ) ) != -1) {             	
        }//if send ip 
    	else{    		    		 
            printf("Send a brocast ip fail!\n");          
            perror("ERROR MESSAGE");
            //exit(0);
        }//else 
		if( sendto(sockfd,json_object_to_json_string(jobj2), strlen( json_object_to_json_string(jobj2) ) ,
            0,  ( struct sockaddr * )&dstaddr, sizeof( dstaddr ) ) != -1) {
        }//if send sbname
        else{
            printf("Send a brocast sbname fail!\n");          
            perror("ERROR MESSAGE");
            //exit(0);
        }//else 
        sleep(6);     
}
}//pSendBcast

void pShowPW(void){
	char rndnum[11]={"0123456789"};	     
	int i;
        char p[5]="";
		printf("The smart device asks to pairing\n");				 		
		srand((unsigned) time(NULL));		
		for(i=0;i<4;i++) strncat(p,&rndnum[rand()%10],1);
        printf( "The pairing key is %s\n", p ); 
	strcpy(pairing , p);
	strcpy(p,"");
}//pShowPW

bool pVerifyPW(char *pairing , char *pringingkey , char *publicip , char *publicport){
    uniaddr.sin_family = AF_INET;
    uniaddr.sin_port = htons(SENDPORT);
    inet_pton( AF_INET,sdip, &( uniaddr.sin_addr.s_addr)  );

	char result[2]="";
	jobj3 = json_object_new_object();
	jobj4 = json_object_new_object();
	jobj5 = json_object_new_object();	
	if(strcmp(pairing , pairingkey) == 0){
		strcpy(result,"1");
	}//if strcmp (pairing , pairingkey)
	else {
		strcpy(result,"0");		
		return FALSE;
	}//else 
json_object *jstring3 = json_object_new_string(result);	
json_object *jstring4 = json_object_new_string(publicip);	
json_object *jstring5 = json_object_new_string(publicport);	
json_object_object_add(jobj3,"result",jstring3);	
json_object_object_add(jobj4,"publicip",jstring4);	
json_object_object_add(jobj5,"publicport",jstring5);	
	if( sendto(sockfd,json_object_to_json_string(jobj3), strlen( json_object_to_json_string(jobj3) ) ,
        	    0,  ( struct sockaddr * )&uniaddr,sizeof( uniaddr ) ) != -1) {		
				printf("result %s\n",result);
	}//if 

	if(strcmp(result,"1")==0){
			if( sendto(sockfd,json_object_to_json_string(jobj4), strlen( json_object_to_json_string(jobj4) ) ,
            	0,  ( struct sockaddr * )&uniaddr,sizeof( uniaddr ) ) != -1) {		
    		}//if -- send public ip
			if( sendto(sockfd,json_object_to_json_string(jobj5), strlen( json_object_to_json_string(jobj5) ) ,
        	   	0,  ( struct sockaddr * )&uniaddr,sizeof( uniaddr ) ) != -1) {		
    		}//if -- send piblic port									
			else{
		       perror("sendto");	   
		       printf("Sendto fail!\n");					
			}//else
			return TRUE;
	}//if strcmp(result,1)
}


bool execCmd(char cmd[MAX_SIZE]){
    uniaddr.sin_family = AF_INET;
    uniaddr.sin_port = htons(SENDPORT);
    inet_pton( AF_INET,sdip, &( uniaddr.sin_addr.s_addr)  );
printf("exec sdip :%s\n",sdip);
	json_object *jobj6;
	jobj6 = json_object_new_object();
	json_object *jstring6 = json_object_new_string( cmd );
	json_object_object_add(jobj6,"msg", jstring6);
	if( sendto(sockfd,json_object_to_json_string(jobj6), strlen( json_object_to_json_string(jobj6) ) ,
            0,  ( struct sockaddr * )&uniaddr, sizeof( uniaddr ) ) != -1) {     
            return TRUE;      
    }//if send execCmd successful 
    else {
    	perror ("Send video command fail!\n");
    	return FALSE;
    }
}//execCmd

void json_parse(json_object * jobj , char *publicip , char *publicport){
	int ask = -1;
	json_object_object_foreach(jobj, key, val) {
		if (strcmp(key,"ask") == 0){
			ask = atoi(json_object_get_string(val));
		}//if strcmp(key,ask)

		else if (strcmp(key,"ip") == 0){
			sdip = json_object_get_string(val);			
		}//else if strcmp(key,sdip)

		else if (strcmp(key,"pairingkey") == 0){
			strncpy(pairingkey , json_object_get_string(val),4);
			pairingkey[4]='\0';
		}//eise if strcmp(key,pairnigkey)			

		else if (strcmp(key,"Key") == 0){
			Key = json_object_get_string(val);
		}//else if strcmp(key,Key)

		else if (strcmp(key,"msg") == 0){
				strcpy(filename,"");
				vDecodeCmd(json_object_get_string(val));
				vExecCmd();
		}//else if strcmp(key,msg)

		else if (strcmp(key,"uuid") == 0){				
				char *Key = malloc(sizeof(char)*70);
				UUID = json_object_get_string(val);				
				printf("[debug] UUID : %s\n",UUID);
				Key = genKey(UUID);				
				saveKey(Key);
		}//else if strcmp(key,uuid)
		
	}//json_object_object_foreach
	switch (ask){
		int sdresult;
		case 0 :
			pShowPW();
		break;

		case 1:	
			sdresult = pVerifyPW(pairing , pairingkey , publicip , publicport );
			printf("[debug] pairing %d\n",sdresult);
			
		break;

		case 2:
		break;
	}//switch ask

}//json_parse






