#!/bin/sh
#
# CA - wrapper around ca to make it easier to use ... basically ca requires
#      some setup stuff to be done before you can use it and this makes
#      things easier between now and when Eric is convinced to fix it :-)
#
# CA -newca ... will setup the right stuff
# CA -newreq ... will generate a certificate request 
# CA -sign ... will sign the generated request and output 
#
# At the end of that grab newreq.pem and newcert.pem (one has the key 
# and the other the certificate) and cat them together and that is what
# you want/need ... I'll make even this a little cleaner later.
#
#
# 11-Jun-96 eay    Fixed a few filename missmatches.
# 03-May-96 eay    Modified to use 'ssleay cmd' instead of 'cmd'.
# 18-Apr-96 tjh    Original hacking
#
# Tim Hudson
# tjh@mincom.com
#

# default ssleay.conf file has setup as per the following
# demoCA ... where everything is stored

REQ='ssleay req'
CA='ssleay ca'

CATOP=./demoCA

for i
do
case $i in
-\?|-h|-help)
    echo "usage: CA -newcert|-newreq|-newca|-sign|-verify" >&2
    exit 0
    ;;
-newcert) 
    # create a certificate
    $REQ -new -x509 -keyout newreq.pem -out newreq.pem
    ;;
-newreq) 
    # create a certificate request
    $REQ -new -keyout newreq.pem -out newreq.pem
    ;;
-newca)     
    # if explictly asked for or it doesn't exist then setup the directory
    # structure that Eric likes to manage things 
    NEW="1"
    if [ "$NEW" -o ! -f ${CATOP}/serial ]; then
	# create the directory hierarchy
	mkdir ${CATOP} 
	mkdir ${CATOP}/certs 
	mkdir ${CATOP}/crl 
	mkdir ${CATOP}/new_certs
	mkdir ${CATOP}/private
	echo "01" > ${CATOP}/serial
	touch ${CATOP}/index.txt
    fi
    if [ ! -f ${CATOP}/private/CAkey.pem ]; then
	echo "CA certificate filename (or enter to create)"
	read FILE

	# ask user for existing CA certificate
	if [ "$FILE" ]; then
	    cp $CERTIFICATE ${CATOP}/private/CAkey.pem
	else
	    echo "Making CA certificate ..."
	    $REQ -new -x509 -keyout ${CATOP}/private/CAkey.pem \
			   -out ${CATOP}/CAcert.pem
	fi
    fi
    ;;
-xsign)
    $CA -policy policy_anything -infiles newreq.pem 
    ;;
-sign|-signreq) 
    $CA -policy policy_anything -out newcert.pem -infiles newreq.pem 
    cat newcert.pem
    ;;
-signcert) 
    echo "Not yet implemented"
    ;;
-verify) 
    shift
    if [ -z "$1" ]; then
	    verify -CAfile demoCA/CAcert.pem newcert.pem
    else
	for j
	do
	    verify -CAfile demoCA/CAcert.pem $j
	done
    fi
    exit 0
    ;;
*)
    echo "Unknown arg $i";
    exit 1
    ;;
esac
done

