
.SUFFIXES: .c .oo

OBJECTS_ASN1= a_object.oo a_bitstr.oo a_utctm.oo a_int.oo a_octet.oo a_print.oo \
	a_type.oo a_set.oo a_dup.oo a_d2i_fp.oo a_i2d_fp.oo \
	a_sign.oo a_verify.oo \
	x_algor.oo x_val.oo x_pubkey.oo x_sig.oo x_req.oo x_attrib.oo \
	x_name.oo x_cinf.oo x_x509.oo x_crl.oo x_info.oo x_spki.oo \
	d2i_r_pr.oo i2d_r_pr.oo d2i_r_pu.oo i2d_r_pu.oo \
	d2i_s_pr.oo d2i_s_pu.oo \
	d2i_pu.oo d2i_pr.oo i2d_pu.oo i2d_pr.oo \
	t_req.oo t_x509.oo t_pkey.oo \
	p7_i_s.oo p7_signi.oo p7_signd.oo p7_recip.oo p7_enc_c.oo p7_evp.oo \
	p7_dgst.oo p7_s_e.oo p7_enc.oo p7_lib.oo \
	f_int.oo i2d_dh.oo d2i_dh.oo n_pkey.oo \
	a_hdr.oo x_pkey.oo \
	asn1_par.oo asn1_lib.oo asn1_err.oo

OBJECTS_BN = bn_add.oo bn_div.oo bn_exp.oo bn_lib.oo bn_mod.oo bn_mul.oo \
	bn_print.oo bn_rand.oo bn_shift.oo bn_sub.oo bn_word.oo \
	bn_gcd.oo bn_prime.oo bn_err.oo bn_sqr.oo bn_mulw.oo

OBJECTS_BUF = buffer.oo bio_lib.oo bio_cb.oo buf_err.oo \
	bss_mem.oo bss_null.oo bss_fd.oo \
	bss_file.oo bss_sock.oo \
	bf_buff.oo

OBJECTS_CONF = conf_err.oo conf.oo

OBJECTS_DES = set_key.oo  ecb_enc.oo  ede_enc.oo  cbc_enc.oo  cbc3_enc.oo \
	ecb3_enc.oo cfb64enc.oo cfb64ede.oo cfb_enc.oo  ofb64ede.oo \
	enc_read.oo enc_writ.oo fcrypt.oo   ncbc_enc.oo ofb64enc.oo \
	ofb_enc.oo  str2key.oo  pcbc_enc.oo qud_cksm.oo rand_key.oo \
	read_pwd.oo rpc_enc.oo  cbc_cksm.oo supp.oo

OBJECTS_DH = dh_gen.oo dh_key.oo dh_lib.oo dh_check.oo dh_err.oo

OBJECTS_DSA = dsa_lib.oo dsa_vrf.oo dsa_sign.oo dsa_err.oo

OBJECTS_ERR=err.oo err_all.oo

OBJECTS_EVP=	encode.oo digest.oo evp_enc.oo evp_key.oo \
	e_ecb_d.oo e_cbc_d.oo e_cfb_d.oo e_ofb_d.oo \
	e_ecb_i.oo e_cbc_i.oo e_cfb_i.oo e_ofb_i.oo \
	e_ecb_3d.oo e_cbc_3d.oo e_rc4.oo e_names.oo \
	e_cfb_3d.oo e_ofb_3d.oo \
	e_ecb_r2.oo e_cbc_r2.oo e_cfb_r2.oo e_ofb_r2.oo \
	m_md2.oo m_md5.oo m_sha.oo m_sha1.oo m_dss.oo m_dss1.oo m_names.oo \
	p_open.oo p_seal.oo p_sign.oo p_verify.oo p_lib.oo \
	bio_md.oo evp_err.oo

OBJECTS_LHASH = lh_stats.oo lhash.oo

OBJECTS_MD = md2_dgst.oo md2_one.oo md5_dgst.oo md5_one.oo

OBJECTS_METH = by_dir.oo by_file.oo meth_err.oo meth_lib.oo x509meth.oo

OBJECTS_OBJECTS = obj_dat.oo obj_err.oo obj_lib.oo

OBJECTS_PEM= pem_sign.oo pem_seal.oo pem_info.oo pem_lib.oo pem_err.oo

OBJECTS_RAND=md5_rand.oo randfile.oo

OBJECTS_RC2=rc2_ecb.oo rc2_skey.oo rc2_cbc.oo rc2cfb64.oo rc2ofb64.oo

OBJECTS_RC4=rc4_enc.oo

OBJECTS_RSA=rsa_enc.oo rsa_gen.oo rsa_lib.oo rsa_sign.oo rsa_err.oo

OBJECTS_SHA=sha_dgst.oo sha1dgst.oo sha_one.oo sha1_one.oo

OBJECTS_STACK=stack.oo

OBJECTS_TXT_DB=txt_db.oo

OBJECTS_X509=x509_def.oo x509_r2x.oo x509_cmp.oo \
            x509_obj.oo x509_req.oo x509_vrf.oo \
	    x509_crt.oo x509_ath.oo x509_err.oo

OBJECTS= $(OBJECTS_ASN1) $(OBJECTS_BN) $(OBJECTS_BUF) $(OBJECTS_CONF) \
         $(OBJECTS_DES) $(OBJECTS_DH) $(OBJECTS_DSA) $(OBJECTS_ERR) \
         $(OBJECTS_EVP) $(OBJECTS_LHASH) $(OBJECTS_MD) $(OBJECTS_METH) \
         $(OBJECTS_OBJECTS) $(OBJECTS_PEM) $(OBJECTS_RAND) $(OBJECTS_RC2) \
         $(OBJECTS_RC4) $(OBJECTS_RSA) $(OBJECTS_SHA) $(OBJECTS_STACK) \
         $(OBJECTS_TXT_DB) $(OBJECTS_X509) \
         robits.oo cryptlib.oo

VPATH = ../../crypto/asn1:../../crypto/bn:../../crypto/buffer:../../crypto/conf:../../crypto/des:../../crypto/dh:../../crypto/dsa:../../crypto/error:../../crypto/evp:../../crypto/lhash:../../crypto/md:../../crypto/meth:../../crypto/objects:../../crypto/pem:../../crypto/rand:../../crypto/rc2:../../crypto/rc4:../../crypto/rsa:../../crypto/sha:../../crypto/stack:../../crypto/txt_db:../../crypto/x509:../../crypto:../extra:.

CFLAGS = -Wp -DRISCOS -DNO_IDEA -Ospace -J../extra -I../../crypto -I../../include

.c.oo:;	armcc $(CFLAGS) -c -o $@ $<

../cryptlib: $(OBJECTS)
	armlib -c -o ../cryptlib $(OBJECTS)

clean:
	del -f *.oo
