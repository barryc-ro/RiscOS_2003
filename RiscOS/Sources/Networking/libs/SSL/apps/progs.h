#ifndef NOPROTO
extern int verify_main(int argc,char *argv[]);
extern int asn1parse_main(int argc,char *argv[]);
extern int req_main(int argc,char *argv[]);
extern int dgst_main(int argc,char *argv[]);
extern int dh_main(int argc,char *argv[]);
extern int enc_main(int argc,char *argv[]);
extern int gendh_main(int argc,char *argv[]);
extern int errstr_main(int argc,char *argv[]);
extern int ca_main(int argc,char *argv[]);
extern int crl_main(int argc,char *argv[]);
extern int rsa_main(int argc,char *argv[]);
extern int x509_main(int argc,char *argv[]);
extern int genrsa_main(int argc,char *argv[]);
extern int s_server_main(int argc,char *argv[]);
extern int s_client_main(int argc,char *argv[]);
extern int s_filter_main(int argc,char *argv[]);
extern int speed_main(int argc,char *argv[]);
extern int hashdir_main(int argc,char *argv[]);
extern int version_main(int argc,char *argv[]);
extern int pkcs7_main(int argc,char *argv[]);
extern int crl2pkcs7_main(int argc,char *argv[]);
#else
extern int verify_main();
extern int asn1parse_main();
extern int req_main();
extern int dgst_main();
extern int dh_main();
extern int enc_main();
extern int gendh_main();
extern int errstr_main();
extern int ca_main();
extern int crl_main();
extern int rsa_main();
extern int x509_main();
extern int genrsa_main();
extern int s_server_main();
extern int s_client_main();
extern int s_filter_main();
extern int speed_main();
extern int hashdir_main();
extern int version_main();
extern int pkcs7_main();
extern int crl2pkcs7_main();
#endif

#define FUNC_TYPE_GENERAL	1
#define FUNC_TYPE_MD		2
#define FUNC_TYPE_CIPHER	3

typedef struct {
	int type;
	char *name;
	int (*func)();
	} FUNCTION;

FUNCTION functions[] = {
	{FUNC_TYPE_GENERAL,"verify",verify_main},
	{FUNC_TYPE_GENERAL,"asn1parse",asn1parse_main},
	{FUNC_TYPE_GENERAL,"req",req_main},
	{FUNC_TYPE_GENERAL,"dgst",dgst_main},
	{FUNC_TYPE_GENERAL,"dh",dh_main},
	{FUNC_TYPE_GENERAL,"enc",enc_main},
	{FUNC_TYPE_GENERAL,"gendh",gendh_main},
	{FUNC_TYPE_GENERAL,"errstr",errstr_main},
	{FUNC_TYPE_GENERAL,"ca",ca_main},
	{FUNC_TYPE_GENERAL,"crl",crl_main},
	{FUNC_TYPE_GENERAL,"rsa",rsa_main},
	{FUNC_TYPE_GENERAL,"x509",x509_main},
	{FUNC_TYPE_GENERAL,"genrsa",genrsa_main},
#ifndef NO_SOCK
	{FUNC_TYPE_GENERAL,"s_server",s_server_main},
#endif
#ifndef NO_SOCK
	{FUNC_TYPE_GENERAL,"s_client",s_client_main},
#endif
#ifndef NO_SOCK
	{FUNC_TYPE_GENERAL,"s_filter",s_filter_main},
#endif
	{FUNC_TYPE_GENERAL,"speed",speed_main},
	{FUNC_TYPE_GENERAL,"hashdir",hashdir_main},
	{FUNC_TYPE_GENERAL,"version",version_main},
	{FUNC_TYPE_GENERAL,"pkcs7",pkcs7_main},
	{FUNC_TYPE_GENERAL,"crl2pkcs7",crl2pkcs7_main},
	{FUNC_TYPE_MD,"md2",dgst_main},
	{FUNC_TYPE_MD,"md5",dgst_main},
	{FUNC_TYPE_MD,"sha",dgst_main},
	{FUNC_TYPE_MD,"sha1",dgst_main},
	{FUNC_TYPE_CIPHER,"base64",enc_main},
	{FUNC_TYPE_CIPHER,"des",enc_main},
	{FUNC_TYPE_CIPHER,"des3",enc_main},
#ifndef NO_IDEA
	{FUNC_TYPE_CIPHER,"idea",enc_main},
#endif
#ifndef NO_RC4
	{FUNC_TYPE_CIPHER,"rc4",enc_main},
#endif
#ifndef NO_RC2
	{FUNC_TYPE_CIPHER,"rc2",enc_main},
#endif
	{FUNC_TYPE_CIPHER,"des-ecb",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede3",enc_main},
	{FUNC_TYPE_CIPHER,"des-cbc",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede-cbc",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede3-cbc",enc_main},
	{FUNC_TYPE_CIPHER,"des-cfb",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede-cfb",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede3-cfb",enc_main},
	{FUNC_TYPE_CIPHER,"des-ofb",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede-ofb",enc_main},
	{FUNC_TYPE_CIPHER,"des-ede3-ofb",enc_main},
#ifndef NO_IDEA
	{FUNC_TYPE_CIPHER,"idea-cbc",enc_main},
#endif
#ifndef NO_IDEA
	{FUNC_TYPE_CIPHER,"idea-ecb",enc_main},
#endif
#ifndef NO_IDEA
	{FUNC_TYPE_CIPHER,"idea-cfb",enc_main},
#endif
#ifndef NO_IDEA
	{FUNC_TYPE_CIPHER,"idea-ofb",enc_main},
#endif
#ifndef NO_RC2
	{FUNC_TYPE_CIPHER,"rc2-cbc",enc_main},
#endif
#ifndef NO_RC2
	{FUNC_TYPE_CIPHER,"rc2-ecb",enc_main},
#endif
#ifndef NO_RC2
	{FUNC_TYPE_CIPHER,"rc2-cfb",enc_main},
#endif
#ifndef NO_RC2
	{FUNC_TYPE_CIPHER,"rc2-ofb",enc_main},
#endif
	{0,NULL,NULL}
	};
