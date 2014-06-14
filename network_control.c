#include <stdlib.h>
#include <stdio.h>
#include <string.h>     
#include <sys/ioctl.h>  /* ioctl()  */
#include <sys/socket.h> /* socket() */
#include <arpa/inet.h> 
#include <unistd.h>     /* close()  */
#include <linux/if.h>   /* struct ifreq */

struct IF_FLAG_INFO { int flag; char *tstr; char *fstr; } if_flag_info[] =
{
    { IFF_UP,           "Interface Up",     "Interface Down",   },
    { IFF_BROADCAST,    "Broadcast",        "No broadcast",     },
    { IFF_DEBUG,        "Debug enabled",    NULL,               },
    { IFF_LOOPBACK,     "Loopback",         NULL,               },
    { IFF_POINTOPOINT,  "Point to Point",   NULL,               },
    { IFF_POINTOPOINT,  "No trailers",      NULL,               },
    { IFF_RUNNING,      "Running",          NULL,               },
    { IFF_NOARP,        "No ARP",           NULL,               },
    { IFF_PROMISC,      "Promiscuous mode", NULL,               },
    { IFF_ALLMULTI,     "Rcv all multicast",NULL,               },
    { IFF_MASTER,       "Master",           NULL,               },
    { IFF_SLAVE,        "Slave",            NULL,               },
    { IFF_MULTICAST,    "Multicast",        "No multicast"      },
    { 0,                NULL,               NULL,               }
};

int main(int argc, char* argv[])
{
    int i;
    int fd;
    int rc;
    struct ifreq ethreq;
    char *ifname = "eth0";

    if (argc > 1)
        ifname = argv[1];

    /* Open a dummy socket so we have a descriptor for ioctl */
    fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0)
    {
        perror("socket");
        return(1);
    }

    memset(&ethreq, 0, sizeof(ethreq));

    /* set the name of the interface we wish to check */
    strncpy(ethreq.ifr_name, ifname, IFNAMSIZ);

    /* grab flags associated with this interface */
    rc = ioctl(fd, SIOCGIFFLAGS, &ethreq);
    if (rc < 0)
    {
        perror("ioctl");
        return(2);
    }

    printf("Flags for %s\n", ifname);
    for (i=0; if_flag_info[i].flag != 0; i++)
    {
        if (ethreq.ifr_flags & if_flag_info[i].flag && if_flag_info[i].tstr)
            printf("  %s\n", if_flag_info[i].tstr);
        else if (if_flag_info[i].fstr)
            printf("  %s\n", if_flag_info[i].fstr);
    }

    close(fd);

    return 0;
}