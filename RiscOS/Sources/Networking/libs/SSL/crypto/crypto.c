/* crypto/crypto.c */
/* Copyright (C) 1995-1996 Eric Young (eay@mincom.oz.au)
 * All rights reserved.
 * 
 * This file is part of an SSL implementation written
 * by Eric Young (eay@mincom.oz.au).
 * The implementation was written so as to conform with Netscapes SSL
 * specification.  This library and applications are
 * FREE FOR COMMERCIAL AND NON-COMMERCIAL USE
 * as long as the following conditions are aheared to.
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.  If this code is used in a product,
 * Eric Young should be given attribution as the author of the parts used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Eric Young (eay@mincom.oz.au)
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

/* If you are happy to use the assmbler version of bn/bn_mulw.c, define
 * ASM */
#ifndef ASM
#undef ASM
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USE_SOCKETS
#include "../e_os.h"

#include "buffer.h"
#include "stack.h"
#include "lhash.h"

#include "err.h"

#include "md2.h"
#include "md5.h"
#include "sha.h"

#include "des.h"
#include "rc2.h"
#include "rc4.h"
#include "idea.h"

#include "bn.h"
#include "dh.h"
#include "rsa.h"
#include "dsa.h"

#include "rand.h"
#include "conf.h"
#include "txt_db.h"

#include "err.h"
#include "envelope.h"

#include "meth.h"
#include "x509.h"
#include "pkcs7.h"
#include "pem.h"
#include "asn1.h"
#include "objects.h"

#include "cryptlib.c"

#include "asn1/a_bitstr.c"
#include "asn1/a_d2i_fp.c"
#include "asn1/a_dup.c"
#include "asn1/a_hdr.c"
#include "asn1/a_i2d_fp.c"
#include "asn1/a_int.c"
#include "asn1/a_object.c"
#include "asn1/a_octet.c"
#include "asn1/a_print.c"
#include "asn1/a_set.c"
#include "asn1/a_sign.c"
#include "asn1/a_type.c"
#include "asn1/a_utctm.c"
#include "asn1/a_verify.c"
#include "asn1/asn1_err.c"
#include "asn1/asn1_lib.c"
#include "asn1/asn1_par.c"
#include "asn1/d2i_dh.c"
#include "asn1/d2i_pr.c"
#include "asn1/d2i_pu.c"
#include "asn1/d2i_r_pr.c"
#include "asn1/d2i_r_pu.c"
#include "asn1/d2i_s_pr.c"
#include "asn1/d2i_s_pu.c"
#include "asn1/f_int.c"
#include "asn1/i2d_dh.c"
#include "asn1/i2d_pr.c"
#include "asn1/i2d_pu.c"
#include "asn1/i2d_r_pr.c"
#include "asn1/i2d_r_pu.c"
#include "asn1/n_pkey.c"
#include "asn1/p7_dgst.c"
#include "asn1/p7_enc.c"
#include "asn1/p7_enc_c.c"
#include "asn1/p7_evp.c"
#include "asn1/p7_i_s.c"
#include "asn1/p7_lib.c"
#include "asn1/p7_recip.c"
#include "asn1/p7_s_e.c"
#include "asn1/p7_signd.c"
#include "asn1/p7_signi.c"
#include "asn1/t_pkey.c"
#include "asn1/t_req.c"
#include "asn1/t_x509.c"
#include "asn1/x_algor.c"
#include "asn1/x_attrib.c"
#include "asn1/x_cinf.c"
#include "asn1/x_crl.c"
#include "asn1/x_info.c"
#include "asn1/x_name.c"
#include "asn1/x_pkey.c"
#include "asn1/x_pubkey.c"
#include "asn1/x_req.c"
#include "asn1/x_sig.c"
#include "asn1/x_spki.c"
#include "asn1/x_val.c"
#include "asn1/x_x509.c"
#include "bn/bn_add.c"
#include "bn/bn_div.c"
#include "bn/bn_err.c"
#include "bn/bn_exp.c"
#include "bn/bn_gcd.c"
#include "bn/bn_lib.c"
#include "bn/bn_mod.c"
#include "bn/bn_mul.c"
#ifndef ASM
#include "bn/bn_mulw.c"
#endif
#include "bn/bn_prime.c"
#include "bn/bn_print.c"
#include "bn/bn_rand.c"
#include "bn/bn_shift.c"
#include "bn/bn_sqr.c"
#include "bn/bn_sub.c"
#include "bn/bn_word.c"
#include "buffer/bf_buff.c"
#include "buffer/bss_fd.c"
#include "buffer/bss_file.c"
#include "buffer/bio_lib.c"
#include "buffer/bio_cb.c"
#include "buffer/bss_mem.c"
#include "buffer/bss_null.c"
#include "buffer/bss_sock.c"
#include "buffer/buf_err.c"
#include "buffer/buffer.c"
#include "conf/conf.c"
#include "conf/conf_err.c"
#include "des/cbc3_enc.c"
#include "des/cbc_cksm.c"
#include "des/cbc_enc.c"
#include "des/cfb64ede.c"
#include "des/cfb64enc.c"
#include "des/cfb_enc.c"
#include "des/ecb3_enc.c"
#include "des/ecb_enc.c"
#include "des/ede_enc.c"
#include "des/enc_read.c"
#include "des/enc_writ.c"
#include "des/fcrypt.c"
#include "des/ncbc_enc.c"
#include "des/ofb64ede.c"
#include "des/ofb64enc.c"
#include "des/ofb_enc.c"
#include "des/pcbc_enc.c"
#include "des/qud_cksm.c"
#include "des/rand_key.c"
#include "des/read_pwd.c"
#include "des/rpc_enc.c"
#include "des/set_key.c"
#include "des/str2key.c"
#include "des/supp.c"
#include "dh/dh_check.c"
#include "dh/dh_err.c"
#include "dh/dh_gen.c"
#include "dh/dh_key.c"
#include "dh/dh_lib.c"
#include "dsa/dsa_err.c"
#include "dsa/dsa_lib.c"
#include "dsa/dsa_sign.c"
#include "dsa/dsa_vrf.c"
#include "error/err.c"
#include "error/err_all.c"
#include "evp/bio_md.c"
#include "evp/digest.c"
#include "evp/e_cbc_3d.c"
#include "evp/e_cbc_d.c"
#include "evp/e_cbc_i.c"
#include "evp/e_cbc_r2.c"
#include "evp/e_cfb_3d.c"
#include "evp/e_cfb_d.c"
#include "evp/e_cfb_i.c"
#include "evp/e_cfb_r2.c"
#include "evp/e_ecb_3d.c"
#include "evp/e_ecb_d.c"
#include "evp/e_ecb_i.c"
#include "evp/e_ecb_r2.c"
#include "evp/e_names.c"
#include "evp/e_ofb_3d.c"
#include "evp/e_ofb_d.c"
#include "evp/e_ofb_i.c"
#include "evp/e_ofb_r2.c"
#include "evp/e_rc4.c"
#include "evp/encode.c"
#include "evp/evp_enc.c"
#include "evp/evp_err.c"
#include "evp/evp_key.c"
#include "evp/m_dss.c"
#include "evp/m_dss1.c"
#include "evp/m_md2.c"
#include "evp/m_md5.c"
#include "evp/m_names.c"
#include "evp/m_sha.c"
#include "evp/m_sha1.c"
#include "evp/p_lib.c"
#include "evp/p_open.c"
#include "evp/p_seal.c"
#include "evp/p_sign.c"
#include "evp/p_verify.c"
#include "idea/i_cbc.c"
#include "idea/i_cfb64.c"
#include "idea/i_ecb.c"
#include "idea/i_ofb64.c"
#include "idea/i_skey.c"
#include "lhash/lh_stats.c"
#include "lhash/lhash.c"
#include "md/md2_dgst.c"
#include "md/md2_one.c"
#include "md/md5_dgst.c"
#include "md/md5_one.c"
#include "meth/by_dir.c"
#include "meth/by_file.c"
#include "meth/meth_err.c"
#include "meth/meth_lib.c"
#include "meth/x509meth.c"
#include "objects/obj_dat.c"
#include "objects/obj_err.c"
#include "objects/obj_lib.c"
#include "pem/pem_err.c"
#include "pem/pem_info.c"
#include "pem/pem_lib.c"
#include "pem/pem_seal.c"
#include "pem/pem_sign.c"
#include "rand/md5_rand.c"
#include "rand/randfile.c"
#include "rc2/rc2_cbc.c"
#include "rc2/rc2_ecb.c"
#include "rc2/rc2_skey.c"
#include "rc2/rc2cfb64.c"
#include "rc2/rc2ofb64.c"
#include "rc4/rc4_enc.c"
#include "rsa/rsa_enc.c"
#include "rsa/rsa_err.c"
#include "rsa/rsa_gen.c"
#include "rsa/rsa_lib.c"
#include "rsa/rsa_sign.c"
#include "sha/sha1_one.c"
#include "sha/sha1dgst.c"
#include "sha/sha_dgst.c"
#include "sha/sha_one.c"
#include "stack/stack.c"
#include "txt_db/txt_db.c"
#include "x509/x509_ath.c"
#include "x509/x509_cmp.c"
#include "x509/x509_crt.c"
#include "x509/x509_def.c"
#include "x509/x509_err.c"
#include "x509/x509_obj.c"
#include "x509/x509_r2x.c"
#include "x509/x509_req.c"
#include "x509/x509_vrf.c"
