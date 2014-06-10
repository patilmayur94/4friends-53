//PCAP by '4friends'
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pcap.h>

#define SIZE_ETHERNET 18
#define ETHERNET_ADDR_LEN 6

int main(int argc,char *argv[])
{
  char *filename=argv[1];
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *handle=pcap_open_offline(filename,errbuf);

  struct pcap_pkthdr *header;
  const u_char *packet;
   
  int i=0;	
  int pc=0;
  int j;
  FILE *fp=fopen("f1.txt","w");

  while(pcap_next_ex(handle,&header,&packet) >= 0) 
  {
    printf("packet %i \n",++pc);
    fprintf(fp,"Packet %i \n",pc);
          
    printf("packet size %d \n",header->len);
    fprintf(fp,"%d \n",header->len);

    printf("Source MAC Address is  ");

    for(j=0;j<6;j++,i++)
      printf("%02X:",(unsigned int)packet[i]);
    printf("\b");
    printf("\n");

    printf("Destination MAC Address is  ");

    for(j=0;j<6;j++,i++)
      printf("%02X:",(unsigned int)packet[i]);
    printf("\n");

    i=17;
    printf("\nTotal Length: ");
    j=packet[i];
    i--;	
    printf("%d\n",j+(packet[i]*256));

    i=22;
    printf("Time to live is :- %d\n",(int )packet[i]);
      

/*i=i+1;
      printf("Protocol :- %02x\n",(int )packet[i]);
      i=i+1;
      printf("Header checksum is :- %02x",(int) (packet[i]));
      i=i+1;
      printf("%02x\n",(int) packet[i]);
      i=i+1;*/
i=26;
 
    printf("Source IP address is  ");

    for(j=0;j<4;j++,i++)	
    	printf("%02d.",packet[i]);
    printf("\n");
    
    printf("Destination IP address is  ");

    for(j=0;j<4;j++,i++)	
      printf("%02d.",packet[i]);
    printf("\n");

    i++;
    printf("\nSource Port: ");
    j=packet[i];
    i--;	
    printf("%d",j+(packet[i]*256));

    i+=3;
    printf("\nDestination Port: ");
    j=packet[i];
    i--;	
    printf("%d",j+(packet[i]*256));

    printf("\n");
    fprintf(fp,"\n\n");
  }
  fclose(fp);
  return 0;
}
 

