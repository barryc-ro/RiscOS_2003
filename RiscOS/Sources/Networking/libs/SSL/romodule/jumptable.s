


first_symbol

; lib/ssl/ssl.h

SSL_accept		DCD	0
SSL_clear		DCD	0
SSL_connect		DCD	0
SSL_copy_session_id		DCD	0
SSL_debug		DCD	0
SSL_flush_connections		DCD	0
SSL_free		DCD	0
SSL_get_cipher		DCD	0
SSL_get_fd		DCD	0
SSL_get_pref_cipher		DCD	0
SSL_get_shared_ciphers		DCD	0
SSL_get_read_ahead		DCD	0
SSL_get_time		DCD	0
SSL_get_timeout		DCD	0
SSL_is_init_finished		DCD	0
SSL_new		DCD	0
SSL_pending		DCD	0
SSL_read		DCD	0
SSL_set_fd		DCD	0
SSL_set_pref_cipher		DCD	0
SSL_set_read_ahead		DCD	0
SSL_set_timeout		DCD	0
SSL_set_verify		DCD	0
SSL_use_RSAPrivateKey		DCD	0
SSL_use_RSAPrivateKey_DER		DCD	0
SSL_use_RSAPrivateKey_file		DCD	0
SSL_use_certificate		DCD	0
SSL_use_certificate_DER		DCD	0
SSL_use_certificate_file		DCD	0
SSL_write		DCD	0
ERR_load_SSL_strings		DCD	0
SSL_load_error_strings		DCD	0
SSL_get_peer_certificate		DCD	0

; lib/x509/x509.h

RSA_new		DCD	0
RSA_size		DCD	0
RSA_generate_key		DCD	0
RSA_public_encrypt		DCD	0
RSA_private_encrypt		DCD	0
RSA_public_decrypt		DCD	0
RSA_private_decrypt		DCD	0
RSA_mod_exp		DCD	0
RSA_bn_rand		DCD	0
RSA_free		DCD	0
RSA_sign		DCD	0
RSA_set_generate_prime_callback		DCD	0
RSA_generate_prime		DCD	0

; lib/error/err.h

ERR_put_error		DCD	0
ERR_get_error		DCD	0
ERR_peek_error		DCD	0
ERR_clear_error		DCD	0
ERR_error_string		DCD	0
ERR_lib_error_string		DCD	0
ERR_func_error_string		DCD	0
ERR_reason_error_string		DCD	0
ERR_print_errors		DCD	0
ERR_load_strings		DCD	0
ERR_load_ERR_strings		DCD	0
ERR_load_crypto_strings		DCD	0

; lib/pem/pem.h

PEM_bin2ascii		DCD	0
PEM_ascii2bin		DCD	0
PEM_do_header		DCD	0
PEM_set_getkey_callback		DCD	0
PEM_write_X509		DCD	0
PEM_read		DCD	0
PEM_read_RSA		DCD	0
PEM_read_X509		DCD	0
PEM_write		DCD	0
PEM_write_RSA		DCD	0
PEM_write_X509_REQ		DCD	0
PEM_write_X509_CRL		DCD	0
PEM_read_X509_REQ		DCD	0
PEM_read_X509_CRL		DCD	0
PEM_DigestInit		DCD	0
PEM_DigestUpdate		DCD	0
PEM_DigestFinal		DCD	0
PEM_SignInit		DCD	0
PEM_SignUpdate		DCD	0
PEM_SignFinal		DCD	0
PEM_VerifyInit		DCD	0
PEM_VerifyUpdate		DCD	0
PEM_VerifyFinal		DCD	0
PEM_EncryptInit		DCD	0
PEM_EncryptUpdate		DCD	0
PEM_EncryptFinal		DCD	0
PEM_EncryptFinished		DCD	0
PEM_DecryptInit		DCD	0
PEM_DecryptUpdate		DCD	0
PEM_DecryptFinal		DCD	0
PEM_DecryptFinished		DCD	0


last_symbol

table_size	*	(last_symbol - first_symbol) / 4
