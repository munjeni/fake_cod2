/*
	Make fake players on Call Of Duty 2
	by munjeni @ 2014
 */

#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <unistd.h> //close
#ifdef WIN32
	#include <winsock.h>
	#include <io.h>
	#include "winerror.h"

	#define close closesocket
	#define sleep Sleep
	#define ONESEC 1000
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h> //fd_set
#define ONESEC 1
#endif

#include "md5.h"
#include "pb_md5.h"

#define BUFLEN 1400
#define MAX_PLAYERS 25

#define ENABLE_DEBUG 0

#if ENABLE_DEBUG
#define LOG printf
#else
#define LOG(...)
#endif

#ifdef WIN32
typedef uint32_t u32;
typedef int socklen_t;

u32 resolv(char *host) {
	struct hostent *hp;
	u32 host_ip;

	host_ip = inet_addr(host);
	if(host_ip == INADDR_NONE) {
		hp = gethostbyname(host);
		if(!hp) {
			printf("\nError: Unable to resolv hostname (%s)\n\n", host);
			exit(1);
		} else host_ip = *(u32 *)(hp->h_addr);
	}
	return(host_ip);
}

int inet_aton(const char *cp, struct in_addr *addr) {
	addr->s_addr = inet_addr(cp);
	return (addr->s_addr == INADDR_NONE) ? 0 : 1;
}
#endif

static void die(char *s) {
	perror(s);
	exit(1);
}

static int waitTimeOut(int sock, unsigned int time) {

	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(sock, &readSet);
	struct timeval timeVal;
	timeVal.tv_sec = time;
	timeVal.tv_usec = 0;
	if(select(sock+1, &readSet, NULL, NULL, &timeVal) > 0)
	{
		return 1;
	}
	return 0;
}

static int getRandomNumber(int a, int b) {

	int nombre = a + (int)((double)rand() / ((double)RAND_MAX + 1) * b);
	return nombre;
}

static char *calc_md5(const char *str, int length) {
	int n;
	MD5_CTX c;
	unsigned char digest[16];
	static char out[33];
	static const char hex[16] = "0123456789abcdef";

	MD5_Init(&c);

	while (length > 0) {
		if (length > 512)
			MD5_Update(&c, str, 512);
		else
			MD5_Update(&c, str, length);
		length -= 512;
		str += 512;
    	}

	MD5_Final(digest, &c);
/*
	for (n = 0; n < 16; ++n)
		snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
*/
	for(n=0; n<16; n++) {
		out[n<<1] = hex[digest[n] >> 4];
		out[(n<<1)+1] = hex[digest[n] & 15];
	}
	out[n<<1] = 0;

	return out;
}

static char *do_md5(uint8_t *key, uint32_t seed) {

    PB_MD5_CTX ctx;
    int i;
    static char hash[33];
    static const char hex[16] = "0123456789abcdef";

    MD5Init(&ctx, seed);
    MD5Update(&ctx, key, strlen((char *)key));
    MD5Final(&ctx);

    for(i=0; i<16; i++) {
        hash[i<<1] = hex[ctx.digest[i] >> 4];
        hash[(i<<1)+1] = hex[ctx.digest[i] & 15];
    }
    hash[i<<1] = 0;

    return hash;
}

static char *getFakeName(void) {

	static char nomFinal[40];

	char *noms[] = {
			"fly",
			"ópium",
			"[11]glyhigh'",
			"55th|supah",
			"pandemic prank`",
			"esuba' Lucker",
			"skill>l33t",
			":P'Owner:",
			"BB|goliot",
			"aG.L4stknight",
			"revol<3",
			"[vg]Frst",
			"[11]glyhigh'",
			"cr33p inct'v",
			"=TFF=MinuteMan",
			"shox",
			"Bodyguard#",
			"phnyyyyyyyyyyyy",
			"Local",
			"(LZ) Tr3n",
			"eZ0o_",
			"Interc3pt0r",
			"[K9] EnV0ltA",
			"LoL.mp",
			"eMuda.Mandar1ni",
			"Jo!Ntzz",
			"Death Magnet1c",
			"St1flersMum",
			"Virtue.Drush",
			"Kqp",
			"<-Low",
			"h4ns",
			"Pown-D.D&B",
			"Kr0ketje1.",
			"scrappycoco",
			"-=SnF=-STIN",
			"Accce",
			"element0",
			"|IF|Kebe",
			"|IF|SadiC",
			"#MHB::Sho0ter",
			"#MHB::Urwan",
			"(-GPS-)Own3d",
			"[C.SF.]boreas",
			"SOLO",
			"Berta incazz0so",
			"cabutssss",
			"daro",
			"[SdF] GhOsT!",
			"MA3EH_HEPE",
			"Moron",
			"McLovin",
			"=OSM=Amantis",
			"BrotherS|KAM!L3",
			"*R!P* ^6ZDRAWEX",
			"^6???",
			"WdC^4|^7.Vig",
			"^5z^30^9h4n",
			"mordshase",
			"m0u",
			"Wac Emigration",
			"^6BrotherS^7|^3NuK3",
			"^9Doggy^7Fashion",
			"[^3SWL^7]MadToni",
			"[KUR!] ^1W^7alkuere ^1!",
			"Lisa ^5:)",
			"^4]^7PBC^4[^7 Pvt.Paula",
			"[k^90^7|^9L^7eckSieKon]",
			"^^00[DFF]D4rk",
			"WdC|^8.Myros",
			"Alam^5...",
			"<cH0p^3-Ch0P>",
			"FG' SoNNy",
			"Lucker.Pro <3",
			"Where",
			"=TCFF=MinuteMan",
			"GL^1-^7M^14^7FIA",
			"ubojica",
			"thoMPson",
			"NAPUSENA KAMILA",
			"|AFK|bidzy",
			"d2T.TurboHIGH",
			"{silk}AxE",
			"Ruzno pace",
			"exor",
			"[1oolac] haTa*",
			"b4rr4",
			"=^9TCFF^7=^"
	};

	snprintf(nomFinal, sizeof(nomFinal), "%s", noms[getRandomNumber(0,86)]);

	return nomFinal;
}

static int get_server_status(char *server, char *port, int get_players_count) {

	struct sockaddr_in client;
	int s;
	int slen;
	char buf[BUFLEN];
	char get_status[] = "\xFF\xFF\xFF\xFFgetstatus";

	char *line;
	int line_num = 0;

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");

	memset((char *) &client, 0, sizeof(client));

	client.sin_family = AF_INET;
	client.sin_port = htons(atoi(port));

	if (inet_aton(server, &client.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	slen = sizeof(client);

	if (sendto(s, get_status, strlen(get_status), 0, (struct sockaddr *)&client, slen) == -1)
		die("[!] get_status");

	if(!waitTimeOut(s, 3)) {
		printf("Error : Time out 1.\n");
		close(s);
		return 1;
	}

	memset(buf,'\0', BUFLEN);

	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&client, (socklen_t *)&slen) == -1)
		die("[!] can not receive server info");

	if (get_players_count) {
		line = strtok(strdup(buf), "\n");

		while(line) {
			if (strstr(line, "0 999") != NULL) {
				line_num++;
				LOG("%s\n", line);
			}
			line  = strtok(NULL, "\n");
		}
		close(s);
		return line_num;
	} else {
		puts(buf);
		close(s);
		return 0;
	}
}

int main(int argc, char *argv[]) {

	struct sockaddr_in client, master_server;
	int s, m, num_p=0;
	int slen, master_slen;
	char buf[BUFLEN];
	int version = 0;

	char master[80];	// = "\xff\xff\xff\xffgetKeyAuthorize 0 HEEXU3XAG6EZ7JPX PB 069ca5c3f5c175c9c47d6fb6e9bc5259";
	char get_chalenge[80];	// = "\xff\xff\xff\xffgetchallenge 0 \"069ca5c3f5c175c9c47d6fb6e9bc5259\"";
	char game_key[] = "HEEXU3XAG6EZ7JPXFDFD";
	char key_part[17];	//part of the game key since no whole key is used for getting chalenge

	char *chalenge_response, *chresp;
	char connect[4096];
	char fake_name[40];
	char *guid;

#ifdef WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(1,0), &wsadata);
#endif

	if (argc != 3 && argc != 4) {
		printf("use: fake_cod2 IP PORT\n"
			"or use: fake_cod2 IP PORT status\n"
			"or use: fake_cod2 IP PORT GAME_KEY\n"
			"\n"
			"for example: fake_cod2 213.92.164.227 28960\n"
			"or: fake_cod2 213.92.164.227 28960 status\n"
			"or: fake_cod2 213.92.164.227 28960 %s\n", game_key);
		return 0;
	}

	if (argc == 4 && strstr(argv[3], "status")) {
		get_server_status(argv[1], argv[2], 0);
		return 0;
	}

	if (argc == 4 && strlen(argv[3]) == 20) {
		strncpy(key_part, argv[3], 16);
		key_part[16] = '\0';
		guid = do_md5((unsigned char *)argv[3], 0x00b684a3);
		snprintf(master, sizeof(master), "\xff\xff\xff\xffgetKeyAuthorize 0 %s PB %s", key_part, guid);
		snprintf(get_chalenge, sizeof(get_chalenge), "\xff\xff\xff\xffgetchallenge 0 \"%s\"", guid);
		printf("GAME CD KEY: %s\nGENERATED GUID: %s\n\n", argv[3], guid);
	} else {
		strncpy(key_part, game_key, 16);
		key_part[16] = '\0';
		guid = do_md5((unsigned char *)game_key, 0x00b684a3);
		snprintf(master, sizeof(master), "\xff\xff\xff\xffgetKeyAuthorize 0 %s PB %s", key_part, guid);
		snprintf(get_chalenge, sizeof(get_chalenge), "\xff\xff\xff\xffgetchallenge 0 \"%s\"", guid);
		printf("GAME CD KEY: %s\nGENERATED GUID: %s\n\n", game_key, guid);
	}


	//---------------------- MASTER SERVER AKA cod2master.activision.com -------------------------

	if ((m = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("m_socket");
	memset((char *) &master_server, 0, sizeof(master_server));
	master_server.sin_family = AF_INET;
	master_server.sin_port = htons(20700);
	if (inet_aton("63.146.124.40", &master_server.sin_addr) == 0) {
		fprintf(stderr, "m_inet_aton() failed\n");
		return 1;
	}
	master_slen = sizeof(master_server);

	if (sendto(m, master, strlen(master), 0, (struct sockaddr *)&master_server, master_slen) == -1)
		die("[!] send master server");

	//------------------------- Chalenge -------------------------------------------------------


	while(num_p < MAX_PLAYERS) {

		sleep(ONESEC);
		num_p = get_server_status(argv[1], argv[2], 1);

		if (num_p >= MAX_PLAYERS) {
			printf("Number of players (%d) reached maximum limit of (%d)\n\n", num_p, MAX_PLAYERS);
			close(m);
			return 1;
		}

		if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
			die("socket");
		memset((char *) &client, 0, sizeof(client));
		client.sin_family = AF_INET;
		client.sin_port = htons(atoi(argv[2]));
		if (inet_aton(argv[1], &client.sin_addr) == 0) {
			fprintf(stderr, "inet_aton() failed\n");
			return 1;
		}
		slen = sizeof(client);

		if (sendto(s, get_chalenge, strlen(get_chalenge), 0, (struct sockaddr *)&client, slen) == -1)
			die("[!] get_chalenge");

		if(!waitTimeOut(s, 1)) {
			printf("Error : Time out 2.\n");
			close(s);
			close(m);
			return 1;
		}

		memset(buf,'\0', BUFLEN);

		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&client, (socklen_t *)&slen) == -1)
			die("[!] chalenge response");
		else {
			if (strstr(buf, "\xff\xff\xff\xff" "challengeResponse") == NULL) {
				if (strstr(calc_md5(buf, strlen(buf)), "d41d8cd98f00b204e9800998ecf8427e") != NULL) {
					printf("?\n");
					continue;
				}

				if (strstr(buf, "EXE_ERR_CDKEY_IN_USE") != NULL) {
					printf("I\n");
					continue;
				}

				if (strstr(buf, "needcdkey") != NULL) {
					if (sendto(m, master, strlen(master), 0, (struct sockaddr *)&master_server, master_slen) == -1)
						die("[!] send master server");
					printf("N\n");
					continue;
				}

				if (strstr(buf, "You are permanently banned from this server") != NULL) {
					printf("%s\n", buf);
					close(s);
					close(m);
					return 1;
				}

				printf("\nUnhandled reply after requesting challenge: %s\n", buf);
				continue;
			} 
		}

		//---------------------------------------------------- Connect ----------------------------------------------------------------------

		chalenge_response = strdup(buf);
		chresp = strsep(&chalenge_response, " ");
		chresp = strsep(&chalenge_response, " ");
		LOG("chalenge response: %s\n\n", chresp);

		snprintf(fake_name, sizeof(fake_name), "%s", getFakeName());
		LOG("%s\"\n", fake_name);

		if (!version) {
			/* for version 1.3 */
			snprintf(connect, sizeof(connect), "\xff\xff\xff\xff" "connect \"\\cg_predictItems\\1\\cl_anonymous\\0\\cl_punkbuster\\1\\cl_voice\\1\\cl_wwwDownload\\1\\rate\\25000\\snaps\\20\\name\\%s\\protocol\\118\\challenge\\%s\\qport\\%d\"", fake_name, chresp, getRandomNumber(10000, 65534));
		} else {
			/* for version 1.0  correct this... I do not know how its looking on 1.0 */
			snprintf(connect, sizeof(connect), "\xff\xff\xff\xff" "connect \"\\cg_predictItems\\1\\cl_anonymous\\0\\cl_punkbuster\\1\\cl_voice\\1\\cl_wwwDownload\\1\\rate\\25000\\snaps\\20\\name\\%s\\protocol\\118\\challenge\\%s\\qport\\%d\"", fake_name, chresp, getRandomNumber(10000, 65534));
			LOG("%s\n", connect);
		}

		if (sendto(s, connect, strlen(connect), 0, (struct sockaddr *)&client, slen) == -1)
			die("[!] connect string");

		if(!waitTimeOut(s, 1)) {
			printf("Error : Time out 3.\n");
			close(s);
			close(m);
			return 1;
		}

		memset(buf,'\0', BUFLEN);

		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&client, (socklen_t *)&slen) == -1)
			die("recvfrom()");

		if (strstr(buf, "\xff\xff\xff\xff" "connectResponse") == NULL) {
			if (strstr(buf, "EXE_SERVER_IS_DIFFERENT_VER1.0") != NULL) {
				printf("VER 1.0\n");
				version = 1;
				continue;
			}

			if (strstr(buf, "EXE_SERVERISFULL") != NULL) {
				printf("F\n");
				continue;
			}

			if (strstr(buf, "EXE_BAD_CHALLENGE") != NULL) {
				printf("C\n");
				continue;
			}

			if (strstr(calc_md5(buf, strlen(buf)), "d41d8cd98f00b204e9800998ecf8427e") != NULL) {
				printf("?\n");
				continue;
			}

			printf("\nUnhandled reply after sending the connect string: %s\n", buf);
			continue;
		}

		printf("player %d : ...............connected using name: %s ...............\n", num_p, fake_name);
		close(s);
	}

	close(m);
	return 0;
}
