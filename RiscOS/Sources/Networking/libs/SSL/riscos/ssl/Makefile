
.SUFFIXES: .c .oo

OBJECTS= ssl_lib.oo ssl_err2.oo ssl_trc.oo ssl_cert.oo ssl_sess.oo \
	ssl_srvr.oo ssl_clnt.oo ssl_pkt.oo  ssl_auth.oo ssl_stat.oo \
	ssl_asn1.oo ssl_txt.oo \
	ssl_rsa.oo ssl_null.oo ssl_des.oo ssl_idea.oo ssl_rc4.oo \
	ssl_rc2.oo bio_ssl.oo \
	ssl_md5.oo ssl_sha.oo ssl_err.oo

VPATH = ../../ssl:.

CFLAGS = -Wp -DRISCOS -Ospace -J../extra -I../../crypto -I../../include

.c.oo:;	armcc $(CFLAGS) -c -o $@ $<

../lib: $(OBJECTS)
	armlib -c -o ../lib $(OBJECTS)

clean:
	rm -f *.oo
