#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <getopt.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <assert.h>
 
struct pcap_filehdr {
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t  thiszone;
    uint32_t sigfigs;
    uint32_t snaplen;
    uint32_t linktype;
};
 
struct pcap_timeval {
    int32_t tv_sec;
    int32_t tv_usec;
};
 
struct pcap_pkthdr {
    struct pcap_timeval ts;
    uint32_t caplen;
    uint32_t len;
};
 
static const char *short_options = "f:h";
static const struct option long_options[] = {
    {"file",            required_argument,  NULL, 'f'},
    {"help",            no_argument,        NULL, 'h'},
    {NULL,              0,                  NULL,  0}
};
 
static void help(void)
{
    printf("pcap-parse. an example of parsing .pcap file\n");
    puts("Usage: pcap-parse [options][pcapfile]\n"
         "Options:\n"
         "  -f|--file <filename>            Parse .pcap file\n"
         "  -h|--help                       Help information\n\n");
     
    exit(0);
}
 
int read_pcap_filehdr(const char *filename, struct pcap_filehdr *phdr);
int read_pcap_pkthdr(FILE *fp, struct pcap_pkthdr *phdr);
int dump_packets(const char *filename);
void print_hex(uint8_t *packet, int caplen);
 
int main(int argc, char **argv)
{
    char *filename = NULL;
    int c, optindex;
    int i = 0;
    struct pcap_filehdr pcap_hdr;
     
    if (argc < 2)
        help();
     
    while ((c = getopt_long(argc, argv, short_options, long_options, &optindex)) != -1)
    {
        printf("%c--->%s\n", c, optarg);
        switch (c)
        {
            case 'h':
                help();
                break;
                     
            case 'f':
                filename = strdup(optarg);
                printf("filename:%s\n", filename);
                break;
                 
            case '?':
                switch (optopt)
                {
                    case 'f':
                        printf("Option -%c requires an argument!\n", optopt);
                        break;
                         
                    defalut :
                        if (isprint(optopt))
                            printf("Unknown option character `0x%X\'!\n", optopt);
                        exit(0);    
                     
                }
            default:
                break;
        }
    }
     
    if (argc >= optind)
    {
        for (i = optind; i < argc; i++)
            printf("--->%s\n", argv[i]);     
    }
         
    if (read_pcap_filehdr(filename, &pcap_hdr) == -1)
    {
        return -1;
    }
     
    printf("magic: 0x%08x\n", pcap_hdr.magic);
    printf("version_major: 0x%04x\n", pcap_hdr.version_major);
    printf("version_minor: 0x%04x\n", pcap_hdr.version_minor);
    printf("thiszone: 0x%08x\n", pcap_hdr.thiszone);
    printf("sigfigs: 0x%08x\n", pcap_hdr.sigfigs);
    printf("snaplen: 0x%08x\n", pcap_hdr.snaplen);
    printf("linktype: 0x%08x\n", pcap_hdr.linktype);
     
     
    dump_packets(filename);
 
    return 0;
}
 
int read_pcap_filehdr(const char *filename, struct pcap_filehdr *phdr)
{
    FILE *fp = NULL;
     
    assert(filename != NULL);
    assert(phdr != NULL);
     
    fp = fopen(filename, "r");
    assert(fp != NULL);
     
    fread(phdr, sizeof(struct pcap_filehdr), 1, fp);
    if (ferror(fp))
    {
        printf("read file error\n");
        fclose(fp);
        return -1;
    }
     
    fclose(fp);
 
    return 0;
}
 
int read_pcap_pkthdr(FILE *fp, struct pcap_pkthdr *phdr)
{
    assert(fp != NULL);
    assert(phdr != NULL);
     
    fread(phdr, sizeof(struct pcap_pkthdr), 1, fp);
    if (ferror(fp))
    {
        printf("read file error\n");
        clearerr(fp);
        return -1;
    }
     
    return 0;
}
 
int read_pcap_pkt(FILE *fp, uint8_t *packet, int caplen)
{
    assert(fp != NULL);
    assert(packet != NULL);
     
    fread(packet, sizeof(uint8_t), caplen, fp);
    if (ferror(fp))
    {
        printf("read file error\n");
        clearerr(fp);
        return -1;
    }
     
    return 0;
}
 
 
int dump_packets(const char *filename)
{
    FILE *fp = NULL;
    struct pcap_pkthdr pcap_pkthdr;
    uint8_t pkt_buff[2000] = {0};
     
    assert(filename != NULL);
     
    fp = fopen(filename, "r");
    assert(fp != NULL);
     
    if(fseek(fp, sizeof(struct pcap_filehdr), SEEK_SET) == -1)
    {
        fclose(fp);
        return -1;
    }
     
    printf("curr file offset: %d\n", ftell(fp));
     
    do
    {
        if (read_pcap_pkthdr(fp, &pcap_pkthdr) == -1)
        {
            fclose(fp);
            return -1;
        }
         
        printf("Timestamp: %d.%d\n", pcap_pkthdr.ts.tv_sec, pcap_pkthdr.ts.tv_usec);
        printf("Cap length: %d\n", pcap_pkthdr.caplen);
        printf("Length: %d\n", pcap_pkthdr.len);
         
        memset(pkt_buff, 0, 2000);
        if (read_pcap_pkt(fp, pkt_buff, pcap_pkthdr.caplen)  == -1)
        {
            fclose(fp);
            return -1;
        }
         
        print_hex(pkt_buff, pcap_pkthdr.caplen);
         
    } while (!feof(fp));
     
    fclose(fp);
    return 0;
}
 
void print_hex(uint8_t *packet, int caplen)
{
    int i = 0;
     
    for (i = 0; i < caplen; i++)
    {
        if (i % 16 == 0)
            printf("\n");
         
        printf("%02x ", packet[i]);
    }
    printf("\n");
}
 
运行结果:
[j@localhost pcap-parse]$ ./pcap_parse -f dhcp.pcap 
f--->dhcp.pcap
filename:dhcp.pcap
magic: 0xa1b2c3d4
version_major: 0x0002
version_minor: 0x0004
thiszone: 0x00000000
sigfigs: 0x00000000
snaplen: 0x0000ffff
linktype: 0x00000001
curr file offset: 24
Timestamp: 1102274184.317453
Cap length: 314
Length: 314
 
ff ff ff ff ff ff 00 0b 82 01 fc 42 08 00 45 00 
01 2c a8 36 00 00 fa 11 17 8b 00 00 00 00 ff ff 
ff ff 00 44 00 43 01 18 59 1f 01 01 06 00 00 00 
3d 1d 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 0b 82 01 fc 42 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 63 82 53 63 35 01 01 3d 07 01 
00 0b 82 01 fc 42 32 04 00 00 00 00 37 04 01 03 
06 2a ff 00 00 00 00 00 00 00 
Timestamp: 1102274184.317748
Cap length: 342
Length: 342
 
00 0b 82 01 fc 42 00 08 74 ad f1 9b 08 00 45 00 
01 48 04 45 00 00 80 11 00 00 c0 a8 00 01 c0 a8 
00 0a 00 43 00 44 01 34 22 33 02 01 06 00 00 00 
3d 1d 00 00 00 00 00 00 00 00 c0 a8 00 0a c0 a8 
00 01 00 00 00 00 00 0b 82 01 fc 42 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 63 82 53 63 35 01 02 01 04 ff 
ff ff 00 3a 04 00 00 07 08 3b 04 00 00 0c 4e 33 
04 00 00 0e 10 36 04 c0 a8 00 01 ff 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 
Timestamp: 1102274184.387484
Cap length: 314
Length: 314
 
ff ff ff ff ff ff 00 0b 82 01 fc 42 08 00 45 00 
01 2c a8 37 00 00 fa 11 17 8a 00 00 00 00 ff ff 
ff ff 00 44 00 43 01 18 9f bd 01 01 06 00 00 00 
3d 1e 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 0b 82 01 fc 42 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 63 82 53 63 35 01 03 3d 07 01 
00 0b 82 01 fc 42 32 04 c0 a8 00 0a 36 04 c0 a8 
00 01 37 04 01 03 06 2a ff 00 
Timestamp: 1102274184.387798
Cap length: 342
Length: 342
 
00 0b 82 01 fc 42 00 08 74 ad f1 9b 08 00 45 00 
01 48 04 46 00 00 80 11 00 00 c0 a8 00 01 c0 a8 
00 0a 00 43 00 44 01 34 df db 02 01 06 00 00 00 
3d 1e 00 00 00 00 00 00 00 00 c0 a8 00 0a 00 00 
00 00 00 00 00 00 00 0b 82 01 fc 42 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 63 82 53 63 35 01 05 3a 04 00 
00 07 08 3b 04 00 00 0c 4e 33 04 00 00 0e 10 36 
04 c0 a8 00 01 01 04 ff ff ff 00 ff 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 
Timestamp: 1102274184.387798
Cap length: 342
Length: 342
 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00