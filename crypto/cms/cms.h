/* crypto/cms/cms.h */
/* Written by Dr Stephen N Henson (steve@openssl.org) for the OpenSSL
 * project.
 */
/* ====================================================================
 * Copyright (c) 2008 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */


#ifndef HEADER_CMS_H
#define HEADER_CMS_H

#include <openssl/x509.h>

#ifdef OPENSSL_NO_CMS
#error CMS is disabled.
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct CMS_ContentInfo_st CMS_ContentInfo;
typedef struct CMS_SignerInfo_st CMS_SignerInfo;
typedef struct CMS_CertificateChoices CMS_CertificateChoices;
typedef struct CMS_RevocationInfoChoice_st CMS_RevocationInfoChoice;

DECLARE_STACK_OF(CMS_SignerInfo)
DECLARE_ASN1_FUNCTIONS(CMS_ContentInfo)
DECLARE_ASN1_PRINT_FUNCTION(CMS_ContentInfo)

#define CMS_SIGNERINFO_ISSUER_SERIAL	0
#define CMS_SIGNERINFO_KEYIDENTIFIER	1

/* S/MIME related flags */

#define CMS_TEXT			0x1
#define CMS_NOCERTS			0x2
#define CMS_NO_CONTENT_VERIFY		0x4
#define CMS_NO_ATTR_VERIFY		0x8
#define CMS_NOSIGS			\
			(CMS_NO_CONTENT_VERIFY|CMS_NO_ATTR_VERIFY)
#define CMS_NOINTERN			0x10
#define CMS_NO_SIGNER_CERT_VERIFY	0x20
#define CMS_NOVERIFY			0x20
#define CMS_DETACHED			0x40
#define CMS_BINARY			0x80
#define CMS_NOATTR			0x100
#define	CMS_NOSMIMECAP			0x200
#define CMS_NOOLDMIMETYPE		0x400
#define CMS_CRLFEOL			0x800
#define CMS_STREAM			0x1000
#define CMS_NOCRL			0x2000
#define CMS_PARTIAL			0x4000
#define CMS_REUSE_DIGEST		0x8000
#define CMS_USE_KEYID			0x10000

const ASN1_OBJECT *CMS_get0_type(CMS_ContentInfo *cms);
CMS_ContentInfo *CMS_Data_Create(void);

BIO *CMS_dataInit(CMS_ContentInfo *cms, BIO *icont);
int CMS_dataFinal(CMS_ContentInfo *cms, BIO *bio);

ASN1_OCTET_STRING **CMS_get0_content(CMS_ContentInfo *cms);
int CMS_is_detached(CMS_ContentInfo *cms);
int CMS_set_detached(CMS_ContentInfo *cms, int detached);

#ifdef HEADER_PEM_H
DECLARE_PEM_rw_const(CMS, CMS_ContentInfo)
#endif

int CMS_stream(unsigned char ***boundary, CMS_ContentInfo *cms);
CMS_ContentInfo *d2i_CMS_bio(BIO *bp, CMS_ContentInfo **cms);
int i2d_CMS_bio(BIO *bp, CMS_ContentInfo *cms);

BIO *BIO_new_CMS(BIO *out, CMS_ContentInfo *cms);
int i2d_CMS_bio_stream(BIO *out, CMS_ContentInfo *cms, BIO *in, int flags);
int PEM_write_bio_CMS_stream(BIO *out, CMS_ContentInfo *cms, BIO *in, int flags);
CMS_ContentInfo *SMIME_read_CMS(BIO *bio, BIO **bcont);
int SMIME_write_CMS(BIO *bio, CMS_ContentInfo *cms, BIO *data, int flags);

int CMS_final(CMS_ContentInfo *cms, BIO *data, int flags);

CMS_ContentInfo *CMS_sign(X509 *signcert, EVP_PKEY *pkey, STACK_OF(X509) *certs,
						BIO *data, unsigned int flags);

int CMS_data(CMS_ContentInfo *cms, BIO *out, unsigned int flags);
CMS_ContentInfo *CMS_data_create(BIO *in, unsigned int flags);

int CMS_digest_verify(CMS_ContentInfo *cms, BIO *dcont, BIO *out,
							unsigned int flags);
CMS_ContentInfo *CMS_digest_create(BIO *in, const EVP_MD *md,
							unsigned int flags);

int CMS_EncryptedData_decrypt(CMS_ContentInfo *cms,
				const unsigned char *key, size_t keylen,
				BIO *dcont, BIO *out, unsigned int flags);

CMS_ContentInfo *CMS_EncryptedData_encrypt(BIO *in, const EVP_CIPHER *cipher,
					const unsigned char *key, size_t keylen,
					unsigned int flags);

int CMS_EncryptedData_set1_key(CMS_ContentInfo *cms, const EVP_CIPHER *ciph,
				const unsigned char *key, size_t keylen);

int CMS_verify(CMS_ContentInfo *cms, STACK_OF(X509) *certs,
		 X509_STORE *store, BIO *dcont, BIO *out, unsigned int flags);

STACK_OF(X509) *CMS_get0_signers(CMS_ContentInfo *cms);

CMS_ContentInfo *CMS_encrypt(STACK_OF(X509) *certs, BIO *in,
				const EVP_CIPHER *cipher, unsigned int flags);

int CMS_decrypt(CMS_ContentInfo *cms, EVP_PKEY *pkey, X509 *cert, BIO *data,
				unsigned int flags);

int CMS_uncompress(CMS_ContentInfo *cms, BIO *dcont, BIO *out,
							unsigned int flags);
CMS_ContentInfo *CMS_compress(BIO *in, int comp_nid, unsigned int flags);

int CMS_set1_eContentType(CMS_ContentInfo *cms, const ASN1_OBJECT *oid);
const ASN1_OBJECT *CMS_get0_eContentType(CMS_ContentInfo *cms);

CMS_CertificateChoices *CMS_add0_CertificateChoices(CMS_ContentInfo *cms);
int CMS_add0_cert(CMS_ContentInfo *cms, X509 *cert);
int CMS_add1_cert(CMS_ContentInfo *cms, X509 *cert);
STACK_OF(X509) *CMS_get1_certs(CMS_ContentInfo *cms);

CMS_RevocationInfoChoice *CMS_add0_RevocationInfoChoice(CMS_ContentInfo *cms);
int CMS_add0_crl(CMS_ContentInfo *cms, X509_CRL *crl);
STACK_OF(X509_CRL) *CMS_get1_crls(CMS_ContentInfo *cms);

int CMS_SignedData_init(CMS_ContentInfo *cms);
CMS_SignerInfo *CMS_add1_signer(CMS_ContentInfo *cms,
			X509 *signer, EVP_PKEY *pk, const EVP_MD *md,
			unsigned int flags);
STACK_OF(CMS_SignerInfo) *CMS_get0_SignerInfos(CMS_ContentInfo *cms);

void CMS_SignerInfo_set1_signer_cert(CMS_SignerInfo *si, X509 *signer);
int CMS_SignerInfo_get0_signer_id(CMS_SignerInfo *si,
					ASN1_OCTET_STRING **keyid,
					X509_NAME **issuer, ASN1_INTEGER **sno);
int CMS_SignerInfo_cert_cmp(CMS_SignerInfo *si, X509 *cert);
int CMS_set1_signers_certs(CMS_ContentInfo *cms, STACK_OF(X509) *certs,
					unsigned int flags);
void CMS_SignerInfo_get0_algs(CMS_SignerInfo *si, EVP_PKEY **pk, X509 **signer,
					X509_ALGOR **pdig, X509_ALGOR **psig);
int CMS_SignerInfo_sign(CMS_SignerInfo *si);
int CMS_SignerInfo_verify(CMS_SignerInfo *si);
int CMS_SignerInfo_verify_content(CMS_SignerInfo *si, BIO *chain);

int CMS_add_smimecap(CMS_SignerInfo *si, STACK_OF(X509_ALGOR) *algs);
int CMS_add_simple_smimecap(STACK_OF(X509_ALGOR) **algs,
				int algnid, int keysize);
int CMS_add_standard_smimecap(STACK_OF(X509_ALGOR) **smcap);

int CMS_signed_get_attr_count(const CMS_SignerInfo *si);
int CMS_signed_get_attr_by_NID(const CMS_SignerInfo *si, int nid,
			  int lastpos);
int CMS_signed_get_attr_by_OBJ(const CMS_SignerInfo *si, ASN1_OBJECT *obj,
			  int lastpos);
X509_ATTRIBUTE *CMS_signed_get_attr(const CMS_SignerInfo *si, int loc);
X509_ATTRIBUTE *CMS_signed_delete_attr(CMS_SignerInfo *si, int loc);
int CMS_signed_add1_attr(CMS_SignerInfo *si, X509_ATTRIBUTE *attr);
int CMS_signed_add1_attr_by_OBJ(CMS_SignerInfo *si,
			const ASN1_OBJECT *obj, int type,
			const void *bytes, int len);
int CMS_signed_add1_attr_by_NID(CMS_SignerInfo *si,
			int nid, int type,
			const void *bytes, int len);
int CMS_signed_add1_attr_by_txt(CMS_SignerInfo *si,
			const char *attrname, int type,
			const void *bytes, int len);
void *CMS_signed_get0_data_by_OBJ(CMS_SignerInfo *si, ASN1_OBJECT *oid,
					int lastpos, int type);

int CMS_unsigned_get_attr_count(const CMS_SignerInfo *si);
int CMS_unsigned_get_attr_by_NID(const CMS_SignerInfo *si, int nid,
			  int lastpos);
int CMS_unsigned_get_attr_by_OBJ(const CMS_SignerInfo *si, ASN1_OBJECT *obj,
			  int lastpos);
X509_ATTRIBUTE *CMS_unsigned_get_attr(const CMS_SignerInfo *si, int loc);
X509_ATTRIBUTE *CMS_unsigned_delete_attr(CMS_SignerInfo *si, int loc);
int CMS_unsigned_add1_attr(CMS_SignerInfo *si, X509_ATTRIBUTE *attr);
int CMS_unsigned_add1_attr_by_OBJ(CMS_SignerInfo *si,
			const ASN1_OBJECT *obj, int type,
			const void *bytes, int len);
int CMS_unsigned_add1_attr_by_NID(CMS_SignerInfo *si,
			int nid, int type,
			const void *bytes, int len);
int CMS_unsigned_add1_attr_by_txt(CMS_SignerInfo *si,
			const char *attrname, int type,
			const void *bytes, int len);
void *CMS_unsigned_get0_data_by_OBJ(CMS_SignerInfo *si, ASN1_OBJECT *oid,
					int lastpos, int type);

/* BEGIN ERROR CODES */
/* The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */
void ERR_load_CMS_strings(void);

/* Error codes for the CMS functions. */

/* Function codes. */
#define CMS_F_CMS_ADD1_RECIPIENT_CERT			 99
#define CMS_F_CMS_ADD1_SIGNER				 100
#define CMS_F_CMS_ADD1_SIGNINGTIME			 101
#define CMS_F_CMS_BIO_TO_ENCRYPTEDCONTENT		 137
#define CMS_F_CMS_BIO_TO_ENCRYPTEDCONTENT_BIO		 142
#define CMS_F_CMS_COMPRESS				 102
#define CMS_F_CMS_COMPRESSEDDATA_CREATE			 103
#define CMS_F_CMS_COMPRESSEDDATA_INIT_BIO		 104
#define CMS_F_CMS_COPY_CONTENT				 105
#define CMS_F_CMS_COPY_MESSAGEDIGEST			 106
#define CMS_F_CMS_DATA					 107
#define CMS_F_CMS_DATAFINAL				 108
#define CMS_F_CMS_DATAINIT				 109
#define CMS_F_CMS_DECRYPTEDCONTENT_DECRYPT_BIO		 145
#define CMS_F_CMS_DECRYPTEDCONTENT_ENCRYPT_BIO		 143
#define CMS_F_CMS_DIGESTALGORITHM_FIND_CTX		 110
#define CMS_F_CMS_DIGESTALGORITHM_INIT_BIO		 111
#define CMS_F_CMS_DIGESTEDDATA_DO_FINAL			 112
#define CMS_F_CMS_DIGEST_VERIFY				 113
#define CMS_F_CMS_ENCRYPTEDCONTENT_DECRYPT_BIO		 146
#define CMS_F_CMS_ENCRYPTEDCONTENT_ENCRYPT_BIO		 144
#define CMS_F_CMS_ENCRYPTEDCONTENT_INIT_BIO		 148
#define CMS_F_CMS_ENCRYPTEDCONTENT_TO_BIO		 138
#define CMS_F_CMS_ENCRYPTEDDATA_DECRYPT			 140
#define CMS_F_CMS_ENCRYPTEDDATA_INIT_BIO		 147
#define CMS_F_CMS_ENCRYPTEDDATA_SET1_KEY		 141
#define CMS_F_CMS_ENCRYPTED_DATA_DECRYPT		 139
#define CMS_F_CMS_ENVELOPED_DATA_INIT			 114
#define CMS_F_CMS_FINAL					 115
#define CMS_F_CMS_GET0_CERTIFICATE_CHOICES		 116
#define CMS_F_CMS_GET0_CONTENT				 117
#define CMS_F_CMS_GET0_ECONTENT_TYPE			 118
#define CMS_F_CMS_GET0_ENVELOPED			 119
#define CMS_F_CMS_GET0_REVOCATION_CHOICES		 120
#define CMS_F_CMS_GET0_SIGNED				 121
#define CMS_F_CMS_RECIPIENTINFO_KTRI_CERT_CMP		 122
#define CMS_F_CMS_RECIPIENTINFO_KTRI_GET0_ALGS		 123
#define CMS_F_CMS_RECIPIENTINFO_KTRI_GET0_SIGNER_ID	 124
#define CMS_F_CMS_SET1_SIGNERIDENTIFIER			 125
#define CMS_F_CMS_SET_DETACHED				 126
#define CMS_F_CMS_SIGN					 127
#define CMS_F_CMS_SIGNED_DATA_INIT			 128
#define CMS_F_CMS_SIGNERINFO_CONTENT_SIGN		 129
#define CMS_F_CMS_SIGNERINFO_SIGN			 130
#define CMS_F_CMS_SIGNERINFO_VERIFY			 131
#define CMS_F_CMS_SIGNERINFO_VERIFY_CERT		 132
#define CMS_F_CMS_SIGNERINFO_VERIFY_CONTENT		 133
#define CMS_F_CMS_STREAM				 134
#define CMS_F_CMS_UNCOMPRESS				 135
#define CMS_F_CMS_VERIFY				 136

/* Reason codes. */
#define CMS_R_ADD_SIGNER_ERROR				 99
#define CMS_R_CERTIFICATE_VERIFY_ERROR			 100
#define CMS_R_CIPHER_INITIALISATION_ERROR		 138
#define CMS_R_CIPHER_PARAMETER_INITIALISATION_ERROR	 139
#define CMS_R_CMS_DATAFINAL_ERROR			 101
#define CMS_R_CONTENT_NOT_FOUND				 102
#define CMS_R_CONTENT_TYPE_NOT_COMPRESSED_DATA		 103
#define CMS_R_CONTENT_TYPE_NOT_ENVELOPED_DATA		 104
#define CMS_R_CONTENT_TYPE_NOT_SIGNED_DATA		 105
#define CMS_R_CONTENT_VERIFY_ERROR			 106
#define CMS_R_CTRL_ERROR				 107
#define CMS_R_CTRL_FAILURE				 108
#define CMS_R_ERROR_GETTING_PUBLIC_KEY			 109
#define CMS_R_ERROR_READING_MESSAGEDIGEST_ATTRIBUTE	 110
#define CMS_R_INVALID_KEY_LENGTH			 140
#define CMS_R_MD_BIO_INIT_ERROR				 111
#define CMS_R_MESSAGEDIGEST_ATTRIBUTE_WRONG_LENGTH	 112
#define CMS_R_MESSAGEDIGEST_WRONG_LENGTH		 113
#define CMS_R_NOT_ENCRYPTED_DATA			 143
#define CMS_R_NOT_KEY_TRANSPORT				 114
#define CMS_R_NOT_SUPPORTED_FOR_THIS_KEY_TYPE		 115
#define CMS_R_NO_CONTENT				 116
#define CMS_R_NO_DEFAULT_DIGEST				 117
#define CMS_R_NO_DIGEST_SET				 118
#define CMS_R_NO_MATCHING_DIGEST			 119
#define CMS_R_NO_PRIVATE_KEY				 120
#define CMS_R_NO_PUBLIC_KEY				 121
#define CMS_R_NO_SIGNERS				 122
#define CMS_R_PRIVATE_KEY_DOES_NOT_MATCH_CERTIFICATE	 123
#define CMS_R_SIGNER_CERTIFICATE_NOT_FOUND		 124
#define CMS_R_SIGNFINAL_ERROR				 125
#define CMS_R_SMIME_TEXT_ERROR				 126
#define CMS_R_STORE_INIT_ERROR				 127
#define CMS_R_TYPE_NOT_COMPRESSED_DATA			 128
#define CMS_R_TYPE_NOT_DATA				 129
#define CMS_R_TYPE_NOT_DIGESTED_DATA			 130
#define CMS_R_TYPE_NOT_ENCRYPTED_DATA			 142
#define CMS_R_UNABLE_TO_FINALIZE_CONTEXT		 131
#define CMS_R_UNKNOWN_CIPHER				 141
#define CMS_R_UNKNOWN_DIGEST_ALGORIHM			 132
#define CMS_R_UNKNOWN_ID				 133
#define CMS_R_UNSUPPORTED_COMPRESSION_ALGORITHM		 134
#define CMS_R_UNSUPPORTED_CONTENT_TYPE			 135
#define CMS_R_UNSUPPORTED_TYPE				 136
#define CMS_R_VERIFICATION_FAILURE			 137

#ifdef  __cplusplus
}
#endif
#endif
