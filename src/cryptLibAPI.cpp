#include "../h/cryptLibAPI.h"

#include <assert>



#define NOMBRE_CL32_DLL		"cl32.dll"

#define FN_INIT					"cryptInit"
#define FN_END					"cryptEnd"
#define FN_DEVICE_OPEN			"cryptDeviceOpen"
#define FN_DEVICE_CLOSE			"cryptDeviceClose"
#define FN_SET_ATTRIBUTE_STRING	"cryptSetAttributeString"
#define FN_GET_ATTRIBUTE_STRING	"cryptGetAttributeString"
#define FN_SET_ATTRIBUTE        "cryptSetAttribute"
#define FN_GET_PUBLIC_KEY		"cryptGetPublicKey"
#define FN_GET_PRIVATE_KEY		"cryptGetPrivateKey"
#define FN_DESTROY_CERT			"cryptDestroyCert"
#define FN_IMPORT_CERT			"cryptImportCert"
#define FN_EXPORT_CERT			"cryptExportCert"
#define FN_CREATE_CONTEXT		"cryptCreateContext"
#define FN_DESTROY_CONTEXT		"cryptDestroyContext"
#define FN_CREATE_SIGNATURE_EX	"cryptCreateSignatureEx"
#define FN_CHECK_SIGNATURE_EX	"cryptCheckSignatureEx"
#define FN_ENCRYPT				"cryptEncrypt"
#define FN_DECRYPT				"cryptDecrypt"
#define FN_CHECK_CERT			"cryptCheckCert"


#define lengthof(x)		(sizeof(x)/sizeof(x[0]))



cryptLib*  cryptLib::instancia = NULL;



CRYPT_DEVICE_TYPE ConvertirEnumerado(CL_DEVICE_TYPE value)
{
	CRYPT_DEVICE_TYPE ret;

	switch (value)
	{
		case CL_DEVICE_NONE:
			ret = CRYPT_DEVICE_NONE;
			break;
		case CL_DEVICE_FORTEZZA:
			ret = CRYPT_DEVICE_FORTEZZA;
			break;
		case CL_DEVICE_PKCS11:
			ret = CRYPT_DEVICE_PKCS11;
			break;
		case CL_DEVICE_CRYPTOAPI:
			ret = CRYPT_DEVICE_CRYPTOAPI;
			break;
		case CL_DEVICE_LAST:
			ret = CRYPT_DEVICE_LAST;
			break;
	}

	return (ret);
}


CRYPT_KEYID_TYPE ConvertirEnumerado(CL_KEYID_TYPE value)
{
	CRYPT_KEYID_TYPE ret;

	switch (value)
	{
		case CL_KEYID_NONE:
			ret = CRYPT_KEYID_NONE;
			break;
		case CL_KEYID_NAME:
			ret = CRYPT_KEYID_NAME;
			break;
		case CL_KEYID_EMAIL:
			ret = CRYPT_KEYID_EMAIL;
			break;
		case CL_KEYID_FIRST:
			ret = CRYPT_KEYID_FIRST;
			break;
		case CL_KEYID_LAST:
			ret = CRYPT_KEYID_LAST;
			break;
	}

	return (ret);
}


CRYPT_FORMAT_TYPE ConvertirEnumerado(CL_FORMAT_TYPE value)
{
	CRYPT_FORMAT_TYPE ret;

	switch (value)
	{
		case CL_FORMAT_NONE:
			ret = CRYPT_FORMAT_NONE;
			break;
		case CL_FORMAT_AUTO:
			ret = CRYPT_FORMAT_AUTO;
			break;
		case CL_FORMAT_CRYPTLIB:
			ret = CRYPT_FORMAT_CRYPTLIB;
			break;
		case CL_FORMAT_PKCS7:
			ret = CRYPT_FORMAT_PKCS7;
			break;
/*
		case CL_FORMAT_CMS:
			ret = CRYPT_FORMAT_CMS;
			break;
*/
		case CL_FORMAT_SMIME:
			ret = CRYPT_FORMAT_SMIME;
			break;
		case CL_FORMAT_PGP:
			ret = CRYPT_FORMAT_PGP;
			break;
		case CL_FORMAT_LAST:
			ret = CRYPT_FORMAT_LAST;
			break;
	}

	return (ret);
}


CRYPT_CERTFORMAT_TYPE ConvertirEnumerado(CL_CERTFORMAT_TYPE value)
{
	CRYPT_CERTFORMAT_TYPE ret;

	switch (value)
	{
		case CL_CERTFORMAT_NONE:
			ret = CRYPT_CERTFORMAT_NONE;
			break;
		case CL_CERTFORMAT_CERTIFICATE:
			ret = CRYPT_CERTFORMAT_CERTIFICATE;
			break;
		case CL_CERTFORMAT_CERTCHAIN:
			ret = CRYPT_CERTFORMAT_CERTCHAIN;
			break;
		case CL_CERTFORMAT_TEXT_CERTIFICATE:
			ret = CRYPT_CERTFORMAT_TEXT_CERTIFICATE;
			break;
		case CL_CERTFORMAT_TEXT_CERTCHAIN:
			ret = CRYPT_CERTFORMAT_TEXT_CERTCHAIN;
			break;
		case CL_CERTFORMAT_XML_CERTIFICATE:
			ret = CRYPT_CERTFORMAT_XML_CERTIFICATE;
			break;
		case CL_CERTFORMAT_XML_CERTCHAIN:
			ret = CRYPT_CERTFORMAT_XML_CERTCHAIN;
			break;
		case CL_CERTFORMAT_LAST:
			ret = CRYPT_CERTFORMAT_LAST;
			break;
	}

	return (ret);
}


CRYPT_ALGO_TYPE ConvertirEnumerado(CL_ALGO_TYPE value)
{
	CRYPT_ALGO_TYPE crypt[] = {
		CRYPT_ALGO_NONE,				/* No encryption */
		CRYPT_ALGO_DES,					/* DES */
		CRYPT_ALGO_3DES,				/* Triple DES */
		CRYPT_ALGO_IDEA,				/* IDEA */
		CRYPT_ALGO_CAST,				/* CAST-128 */
		CRYPT_ALGO_RC2,					/* RC2 */
		CRYPT_ALGO_RC4,					/* RC4 */
		CRYPT_ALGO_RC5,					/* RC5 */
		CRYPT_ALGO_AES,					/* AES */
		CRYPT_ALGO_BLOWFISH,			/* Blowfish */
		CRYPT_ALGO_SKIPJACK,			/* Skipjack */
		CRYPT_ALGO_DH,					/* Diffie-Hellman */
		CRYPT_ALGO_RSA,					/* RSA */
		CRYPT_ALGO_DSA,					/* DSA */
		CRYPT_ALGO_ELGAMAL,				/* ElGamal */
		CRYPT_ALGO_KEA,					/* KEA */
		CRYPT_ALGO_MD2,					/* MD2 */
		CRYPT_ALGO_MD4,					/* MD4 */
		CRYPT_ALGO_MD5,					/* MD5 */
		CRYPT_ALGO_SHA,					/* SHA/SHA1 */
		CRYPT_ALGO_RIPEMD160,			/* RIPE-MD 160 */
	#ifdef USE_SHA2
		CRYPT_ALGO_SHA2,				/* SHA2 placeholder */
	#endif /* USE_SHA2 */
		CRYPT_ALGO_HMAC_MD5,			/* HMAC-MD5 */
		CRYPT_ALGO_HMAC_SHA,			/* HMAC-SHA */
		CRYPT_ALGO_HMAC_RIPEMD160,		/* HMAC-RIPEMD-160 */
	#ifdef USE_VENDOR_ALGOS
		CRYPT_ALGO_VENDOR1, CRYPT_ALGO_VENDOR2, CRYPT_ALGO_VENDOR3,
	#endif
		CRYPT_ALGO_LAST,				/* Last possible crypt algo value */
		CRYPT_ALGO_FIRST_CONVENTIONAL,
		CRYPT_ALGO_LAST_CONVENTIONAL,
		CRYPT_ALGO_FIRST_PKC,
		CRYPT_ALGO_LAST_PKC,
		CRYPT_ALGO_FIRST_HASH,
		CRYPT_ALGO_LAST_HASH,
		CRYPT_ALGO_FIRST_MAC,
		CRYPT_ALGO_LAST_MAC	/* End of mac algo.range */
	};

	CL_ALGO_TYPE cl[] = {
		CL_ALGO_NONE,				/* No encryption */
		CL_ALGO_DES,				/* DES */
		CL_ALGO_3DES,				/* Triple DES */
		CL_ALGO_IDEA,				/* IDEA */
		CL_ALGO_CAST,				/* CAST-128 */
		CL_ALGO_RC2,				/* RC2 */
		CL_ALGO_RC4,				/* RC4 */
		CL_ALGO_RC5,				/* RC5 */
		CL_ALGO_AES,				/* AES */
		CL_ALGO_BLOWFISH,			/* Blowfish */
		CL_ALGO_SKIPJACK,			/* Skipjack */
		CL_ALGO_DH,					/* Diffie-Hellman */
		CL_ALGO_RSA,				/* RSA */
		CL_ALGO_DSA,				/* DSA */
		CL_ALGO_ELGAMAL,			/* ElGamal */
		CL_ALGO_KEA,				/* KEA */
		CL_ALGO_MD2,				/* MD2 */
		CL_ALGO_MD4,				/* MD4 */
		CL_ALGO_MD5,				/* MD5 */
		CL_ALGO_SHA,				/* SHA/SHA1 */
		CL_ALGO_RIPEMD160,			/* RIPE-MD 160 */
	#ifdef USE_SHA2
		CL_ALGO_SHA2,				/* SHA2 placeholder */
	#endif /* USE_SHA2 */
		CL_ALGO_HMAC_MD5,			/* HMAC-MD5 */
		CL_ALGO_HMAC_SHA,			/* HMAC-SHA */
		CL_ALGO_HMAC_RIPEMD160,		/* HMAC-RIPEMD-160 */
	#ifdef USE_VENDOR_ALGOS
		CL_ALGO_VENDOR1, CL_ALGO_VENDOR2, CL_ALGO_VENDOR3,
	#endif
		CL_ALGO_LAST,				/* Last possible crypt algo value */
		CL_ALGO_FIRST_CONVENTIONAL,
		CL_ALGO_LAST_CONVENTIONAL,
		CL_ALGO_FIRST_PKC,
		CL_ALGO_LAST_PKC,
		CL_ALGO_FIRST_HASH,
		CL_ALGO_LAST_HASH,
		CL_ALGO_FIRST_MAC,
		CL_ALGO_LAST_MAC	/* End of mac algo.range */
	};
	int i;
	bool encontrado;

	i = lengthof(crypt) - 1;
	encontrado = false;

	while (i >= 0 && !encontrado)
	{
		encontrado = (value == cl[i]);
		if (!encontrado)
			i--;
	}

	assert(encontrado);

	return (crypt[i]);
}


CRYPT_ATTRIBUTE_TYPE ConvertirEnumerado(CL_ATTRIBUTE_TYPE value)
{
	CRYPT_ATTRIBUTE_TYPE crypt[] = {
		CRYPT_ATTRIBUTE_NONE,			/* Non-value */
		CRYPT_PROPERTY_FIRST,
		CRYPT_PROPERTY_HIGHSECURITY,	/* Owned+non-forwardcount+locked */
		CRYPT_PROPERTY_OWNER,			/* Object owner */
		CRYPT_PROPERTY_FORWARDCOUNT,	/* No.of times object can be forwarded */
		CRYPT_PROPERTY_LOCKED,			/* Whether properties can be chged/read */
		CRYPT_PROPERTY_USAGECOUNT,		/* Usage count before object expires */
		CRYPT_PROPERTY_NONEXPORTABLE,	/* Whether key is nonexp.from context */
		CRYPT_PROPERTY_LAST, CRYPT_GENERIC_FIRST,
		CRYPT_ATTRIBUTE_ERRORTYPE,		/* Type of last error */
		CRYPT_ATTRIBUTE_ERRORLOCUS,		/* Locus of last error */
		CRYPT_ATTRIBUTE_INT_ERRORCODE,	/* Low-level software-specific */
		CRYPT_ATTRIBUTE_INT_ERRORMESSAGE, /*   error code and message */
		CRYPT_ATTRIBUTE_BUFFERSIZE,		/* Internal data buffer size */
		CRYPT_GENERIC_LAST,
		CRYPT_OPTION_FIRST,
		CRYPT_OPTION_INFO_DESCRIPTION,	/* Text description */
		CRYPT_OPTION_INFO_COPYRIGHT,	/* Copyright notice */
		CRYPT_OPTION_INFO_MAJORVERSION,	/* Major release version */
		CRYPT_OPTION_INFO_MINORVERSION,	/* Minor release version */
		CRYPT_OPTION_INFO_STEPPING,		/* Release stepping */
		CRYPT_OPTION_ENCR_ALGO,			/* Encryption algorithm */
		CRYPT_OPTION_ENCR_HASH,			/* Hash algorithm */
		CRYPT_OPTION_ENCR_MAC,			/* MAC algorithm */
		CRYPT_OPTION_PKC_ALGO,			/* Public-key encryption algorithm */
		CRYPT_OPTION_PKC_KEYSIZE,		/* Public-key encryption key size */
		CRYPT_OPTION_SIG_ALGO,			/* Signature algorithm */
		CRYPT_OPTION_SIG_KEYSIZE,		/* Signature keysize */
		CRYPT_OPTION_KEYING_ALGO,		/* Key processing algorithm */
		CRYPT_OPTION_KEYING_ITERATIONS,	/* Key processing iterations */
		CRYPT_OPTION_CERT_SIGNUNRECOGNISEDATTRIBUTES,	/* Whether to sign unrecog.attrs */
		CRYPT_OPTION_CERT_VALIDITY,		/* Certificate validity period */
		CRYPT_OPTION_CERT_UPDATEINTERVAL,	/* CRL update interval */
		CRYPT_OPTION_CERT_COMPLIANCELEVEL,	/* PKIX compliance level for cert chks.*/
		CRYPT_OPTION_CMS_DEFAULTATTRIBUTES,	/* Add default CMS attributes */
		CRYPT_OPTION_SMIME_DEFAULTATTRIBUTES,
		CRYPT_OPTION_KEYS_LDAP_OBJECTCLASS,	/* Object class */
		CRYPT_OPTION_KEYS_LDAP_OBJECTTYPE,	/* Object type to fetch */
		CRYPT_OPTION_KEYS_LDAP_FILTER,		/* Query filter */
		CRYPT_OPTION_KEYS_LDAP_CACERTNAME,	/* CA certificate attribute name */
		CRYPT_OPTION_KEYS_LDAP_CERTNAME,	/* Certificate attribute name */
		CRYPT_OPTION_KEYS_LDAP_CRLNAME,		/* CRL attribute name */
		CRYPT_OPTION_KEYS_LDAP_EMAILNAME,	/* Email attribute name */
		CRYPT_OPTION_DEVICE_PKCS11_DVR01,	/* Name of first PKCS #11 driver */
		CRYPT_OPTION_DEVICE_PKCS11_DVR02,	/* Name of second PKCS #11 driver */
		CRYPT_OPTION_DEVICE_PKCS11_DVR03,	/* Name of third PKCS #11 driver */
		CRYPT_OPTION_DEVICE_PKCS11_DVR04,	/* Name of fourth PKCS #11 driver */
		CRYPT_OPTION_DEVICE_PKCS11_DVR05,	/* Name of fifth PKCS #11 driver */
		CRYPT_OPTION_DEVICE_PKCS11_HARDWAREONLY,/* Use only hardware mechanisms */
		CRYPT_OPTION_NET_SOCKS_SERVER,		/* Socks server name */
		CRYPT_OPTION_NET_SOCKS_USERNAME,	/* Socks user name */
		CRYPT_OPTION_NET_HTTP_PROXY,		/* Web proxy server */
		CRYPT_OPTION_NET_CONNECTTIMEOUT,	/* Timeout for network connection setup */
		CRYPT_OPTION_NET_TIMEOUT,			/* Timeout for network read/write */
		CRYPT_OPTION_MISC_ASYNCINIT,	/* Whether to init cryptlib async'ly */
		CRYPT_OPTION_MISC_SIDECHANNELPROTECTION, /* Protect against side-channel attacks */
		CRYPT_OPTION_CONFIGCHANGED,		/* Whether in-mem.opts match on-disk ones */
		CRYPT_OPTION_SELFTESTOK,		/* Whether self-test was completed and OK */
		CRYPT_OPTION_LAST,
		CRYPT_CTXINFO_FIRST,
		CRYPT_CTXINFO_ALGO,				/* Algorithm */
		CRYPT_CTXINFO_MODE,				/* Mode */
		CRYPT_CTXINFO_NAME_ALGO,		/* Algorithm name */
		CRYPT_CTXINFO_NAME_MODE,		/* Mode name */
		CRYPT_CTXINFO_KEYSIZE,			/* Key size in bytes */
		CRYPT_CTXINFO_BLOCKSIZE,		/* Block size */
		CRYPT_CTXINFO_IVSIZE,			/* IV size */
		CRYPT_CTXINFO_KEYING_ALGO,		/* Key processing algorithm */
		CRYPT_CTXINFO_KEYING_ITERATIONS,/* Key processing iterations */
		CRYPT_CTXINFO_KEYING_SALT,		/* Key processing salt */
		CRYPT_CTXINFO_KEYING_VALUE,		/* Value used to derive key */
		CRYPT_CTXINFO_KEY,				/* Key */
		CRYPT_CTXINFO_KEY_COMPONENTS,	/* Public-key components */
		CRYPT_CTXINFO_IV,				/* IV */
		CRYPT_CTXINFO_HASHVALUE,		/* Hash value */
		CRYPT_CTXINFO_LABEL,			/* Label for private/secret key */
		CRYPT_CTXINFO_LAST,
		CRYPT_CERTINFO_FIRST,
		CRYPT_CERTINFO_SELFSIGNED,		/* Cert is self-signed */
		CRYPT_CERTINFO_IMMUTABLE,		/* Cert is signed and immutable */
		CRYPT_CERTINFO_XYZZY,			/* Cert is a magic just-works cert */
		CRYPT_CERTINFO_CERTTYPE,		/* Certificate object type */
		CRYPT_CERTINFO_FINGERPRINT,		/* Certificate fingerprints */
		CRYPT_CERTINFO_FINGERPRINT_MD5,
		CRYPT_CERTINFO_FINGERPRINT_SHA,
		CRYPT_CERTINFO_CURRENT_CERTIFICATE,/* Cursor mgt: Rel.pos in chain/CRL/OCSP */
		CRYPT_CERTINFO_CURRENT_EXTENSION,/* Cursor mgt: Rel.pos.or abs.extension */
		CRYPT_CERTINFO_CURRENT_FIELD,	/* Cursor mgt: Rel.pos.or abs.field in ext */
		CRYPT_CERTINFO_CURRENT_COMPONENT,/* Cursor mgt: Rel.pos in multival.field */
		CRYPT_CERTINFO_TRUSTED_USAGE,	/* Usage that cert is trusted for */
		CRYPT_CERTINFO_TRUSTED_IMPLICIT,/* Whether cert is implicitly trusted */
		CRYPT_CERTINFO_SIGNATURELEVEL,	/* Amount of detail to include in sigs.*/
		CRYPT_CERTINFO_VERSION,			/* Cert.format version */
		CRYPT_CERTINFO_SERIALNUMBER,	/* Serial number */
		CRYPT_CERTINFO_SUBJECTPUBLICKEYINFO,	/* Public key */
		CRYPT_CERTINFO_CERTIFICATE,		/* User certificate */
		CRYPT_CERTINFO_USERCERTIFICATE,
		CRYPT_CERTINFO_CACERTIFICATE,	/* CA certificate */
		CRYPT_CERTINFO_ISSUERNAME,		/* Issuer DN */
		CRYPT_CERTINFO_VALIDFROM,		/* Cert valid-from time */
		CRYPT_CERTINFO_VALIDTO,			/* Cert valid-to time */
		CRYPT_CERTINFO_SUBJECTNAME,		/* Subject DN */
		CRYPT_CERTINFO_ISSUERUNIQUEID,	/* Issuer unique ID */
		CRYPT_CERTINFO_SUBJECTUNIQUEID,	/* Subject unique ID */
		CRYPT_CERTINFO_CERTREQUEST,		/* Cert.request (DN + public key) */
		CRYPT_CERTINFO_THISUPDATE,		/* CRL/OCSP current-update time */
		CRYPT_CERTINFO_NEXTUPDATE,		/* CRL/OCSP next-update time */
		CRYPT_CERTINFO_REVOCATIONDATE,	/* CRL/OCSP cert-revocation time */
		CRYPT_CERTINFO_REVOCATIONSTATUS,/* OCSP revocation status */
		CRYPT_CERTINFO_CERTSTATUS,		/* RTCS certificate status */
		CRYPT_CERTINFO_DN,				/* Currently selected DN in string form */
		CRYPT_CERTINFO_PKIUSER_ID,		/* PKI user ID */
		CRYPT_CERTINFO_PKIUSER_ISSUEPASSWORD,	/* PKI user issue password */
		CRYPT_CERTINFO_PKIUSER_REVPASSWORD,		/* PKI user revocation password */
		CRYPT_CERTINFO_COUNTRYNAME,	/* countryName */
		CRYPT_CERTINFO_STATEORPROVINCENAME,	/* stateOrProvinceName */
		CRYPT_CERTINFO_LOCALITYNAME,		/* localityName */
		CRYPT_CERTINFO_ORGANIZATIONNAME,	/* organizationName */
		CRYPT_CERTINFO_ORGANISATIONNAME,
		CRYPT_CERTINFO_ORGANIZATIONALUNITNAME,	/* organizationalUnitName */
		CRYPT_CERTINFO_ORGANISATIONALUNITNAME,
		CRYPT_CERTINFO_COMMONNAME,		/* commonName */
		CRYPT_CERTINFO_OTHERNAME_TYPEID,		/* otherName.typeID */
		CRYPT_CERTINFO_OTHERNAME_VALUE,			/* otherName.value */
		CRYPT_CERTINFO_RFC822NAME,				/* rfc822Name */
		CRYPT_CERTINFO_EMAIL,
		CRYPT_CERTINFO_DNSNAME,					/* dNSName */
		CRYPT_CERTINFO_DIRECTORYNAME,			/* directoryName */
		CRYPT_CERTINFO_EDIPARTYNAME_NAMEASSIGNER,	/* ediPartyName.nameAssigner */
		CRYPT_CERTINFO_EDIPARTYNAME_PARTYNAME,	/* ediPartyName.partyName */
		CRYPT_CERTINFO_UNIFORMRESOURCEIDENTIFIER,	/* uniformResourceIdentifier */
		CRYPT_CERTINFO_IPADDRESS,				/* iPAddress */
		CRYPT_CERTINFO_REGISTEREDID,			/* registeredID */
		CRYPT_CERTINFO_KEYFEATURES,
		CRYPT_CERTINFO_AUTHORITYINFOACCESS,
		CRYPT_CERTINFO_AUTHORITYINFO_RTCS,		/* accessDescription.accessLocation */
		CRYPT_CERTINFO_AUTHORITYINFO_OCSP,		/* accessDescription.accessLocation */
		CRYPT_CERTINFO_AUTHORITYINFO_CAISSUERS,	/* accessDescription.accessLocation */
		CRYPT_CERTINFO_AUTHORITYINFO_TIMESTAMPING,/* accessDescription.accessLocation */
		CRYPT_CERTINFO_OCSP_NONCE,				/* nonce */
		CRYPT_CERTINFO_OCSP_RESPONSE,
		CRYPT_CERTINFO_OCSP_RESPONSE_OCSP,		/* OCSP standard response */
		CRYPT_CERTINFO_OCSP_NOCHECK,
		CRYPT_CERTINFO_OCSP_ARCHIVECUTOFF,
		CRYPT_CERTINFO_SUBJECTINFOACCESS,
		CRYPT_CERTINFO_SUBJECTINFO_CAREPOSITORY,/* accessDescription.accessLocation */
		CRYPT_CERTINFO_SUBJECTINFO_TIMESTAMPING,/* accessDescription.accessLocation */
		CRYPT_CERTINFO_SIGG_DATEOFCERTGEN,
		CRYPT_CERTINFO_SIGG_PROCURATION,
		CRYPT_CERTINFO_SIGG_PROCURE_COUNTRY,	/* country */
		CRYPT_CERTINFO_SIGG_PROCURE_TYPEOFSUBSTITUTION,	/* typeOfSubstitution */
		CRYPT_CERTINFO_SIGG_PROCURE_SIGNINGFOR,	/* signingFor.thirdPerson */
		CRYPT_CERTINFO_SIGG_MONETARYLIMIT,
		CRYPT_CERTINFO_SIGG_MONETARY_CURRENCY,	/* currency */
		CRYPT_CERTINFO_SIGG_MONETARY_AMOUNT,	/* amount */
		CRYPT_CERTINFO_SIGG_MONETARY_EXPONENT,	/* exponent */
		CRYPT_CERTINFO_SIGG_RESTRICTION,
		CRYPT_CERTINFO_STRONGEXTRANET,
		CRYPT_CERTINFO_STRONGEXTRANET_ZONE,		/* sxNetIDList.sxNetID.zone */
		CRYPT_CERTINFO_STRONGEXTRANET_ID,		/* sxNetIDList.sxNetID.id */
		CRYPT_CERTINFO_SUBJECTDIRECTORYATTRIBUTES,
		CRYPT_CERTINFO_SUBJECTDIR_TYPE,			/* attribute.type */
		CRYPT_CERTINFO_SUBJECTDIR_VALUES,		/* attribute.values */
		CRYPT_CERTINFO_SUBJECTKEYIDENTIFIER,
		CRYPT_CERTINFO_KEYUSAGE,
		CRYPT_CERTINFO_PRIVATEKEYUSAGEPERIOD,
		CRYPT_CERTINFO_PRIVATEKEY_NOTBEFORE,	/* notBefore */
		CRYPT_CERTINFO_PRIVATEKEY_NOTAFTER,		/* notAfter */
		CRYPT_CERTINFO_SUBJECTALTNAME,
		CRYPT_CERTINFO_ISSUERALTNAME,
		CRYPT_CERTINFO_BASICCONSTRAINTS,
		CRYPT_CERTINFO_CA,						/* cA */
		CRYPT_CERTINFO_AUTHORITY,
		CRYPT_CERTINFO_PATHLENCONSTRAINT,		/* pathLenConstraint */
		CRYPT_CERTINFO_CRLNUMBER,
		CRYPT_CERTINFO_CRLREASON,
		CRYPT_CERTINFO_HOLDINSTRUCTIONCODE,
		CRYPT_CERTINFO_INVALIDITYDATE,
		CRYPT_CERTINFO_DELTACRLINDICATOR,
		CRYPT_CERTINFO_ISSUINGDISTRIBUTIONPOINT,
		CRYPT_CERTINFO_ISSUINGDIST_FULLNAME,	/* distributionPointName.fullName */
		CRYPT_CERTINFO_ISSUINGDIST_USERCERTSONLY,	/* onlyContainsUserCerts */
		CRYPT_CERTINFO_ISSUINGDIST_CACERTSONLY,	/* onlyContainsCACerts */
		CRYPT_CERTINFO_ISSUINGDIST_SOMEREASONSONLY,	/* onlySomeReasons */
		CRYPT_CERTINFO_ISSUINGDIST_INDIRECTCRL,	/* indirectCRL */
		CRYPT_CERTINFO_CERTIFICATEISSUER,
		CRYPT_CERTINFO_NAMECONSTRAINTS,
		CRYPT_CERTINFO_PERMITTEDSUBTREES,		/* permittedSubtrees */
		CRYPT_CERTINFO_EXCLUDEDSUBTREES,		/* excludedSubtrees */
		CRYPT_CERTINFO_CRLDISTRIBUTIONPOINT,
		CRYPT_CERTINFO_CRLDIST_FULLNAME,		/* distributionPointName.fullName */
		CRYPT_CERTINFO_CRLDIST_REASONS,			/* reasons */
		CRYPT_CERTINFO_CRLDIST_CRLISSUER,		/* cRLIssuer */
		CRYPT_CERTINFO_CERTIFICATEPOLICIES,
		CRYPT_CERTINFO_CERTPOLICYID,		/* policyInformation.policyIdentifier */
		CRYPT_CERTINFO_CERTPOLICY_CPSURI,
		CRYPT_CERTINFO_CERTPOLICY_ORGANIZATION,
		CRYPT_CERTINFO_CERTPOLICY_NOTICENUMBERS,
		CRYPT_CERTINFO_CERTPOLICY_EXPLICITTEXT,
		CRYPT_CERTINFO_POLICYMAPPINGS,
		CRYPT_CERTINFO_ISSUERDOMAINPOLICY,	/* policyMappings.issuerDomainPolicy */
		CRYPT_CERTINFO_SUBJECTDOMAINPOLICY,	/* policyMappings.subjectDomainPolicy */
		CRYPT_CERTINFO_AUTHORITYKEYIDENTIFIER,
		CRYPT_CERTINFO_AUTHORITY_KEYIDENTIFIER,	/* keyIdentifier */
		CRYPT_CERTINFO_AUTHORITY_CERTISSUER,	/* authorityCertIssuer */
		CRYPT_CERTINFO_AUTHORITY_CERTSERIALNUMBER,	/* authorityCertSerialNumber */
		CRYPT_CERTINFO_POLICYCONSTRAINTS,
		CRYPT_CERTINFO_REQUIREEXPLICITPOLICY,	/* policyConstraints.requireExplicitPolicy */
		CRYPT_CERTINFO_INHIBITPOLICYMAPPING,	/* policyConstraints.inhibitPolicyMapping */
		CRYPT_CERTINFO_EXTKEYUSAGE,
		CRYPT_CERTINFO_EXTKEY_MS_INDIVIDUALCODESIGNING,	/* individualCodeSigning */
		CRYPT_CERTINFO_EXTKEY_MS_COMMERCIALCODESIGNING,	/* commercialCodeSigning */
		CRYPT_CERTINFO_EXTKEY_MS_CERTTRUSTLISTSIGNING,	/* certTrustListSigning */
		CRYPT_CERTINFO_EXTKEY_MS_TIMESTAMPSIGNING,	/* timeStampSigning */
		CRYPT_CERTINFO_EXTKEY_MS_SERVERGATEDCRYPTO,	/* serverGatedCrypto */
		CRYPT_CERTINFO_EXTKEY_MS_ENCRYPTEDFILESYSTEM,	/* encrypedFileSystem */
		CRYPT_CERTINFO_EXTKEY_SERVERAUTH,		/* serverAuth */
		CRYPT_CERTINFO_EXTKEY_CLIENTAUTH,		/* clientAuth */
		CRYPT_CERTINFO_EXTKEY_CODESIGNING,		/* codeSigning */
		CRYPT_CERTINFO_EXTKEY_EMAILPROTECTION,	/* emailProtection */
		CRYPT_CERTINFO_EXTKEY_IPSECENDSYSTEM,	/* ipsecEndSystem */
		CRYPT_CERTINFO_EXTKEY_IPSECTUNNEL,		/* ipsecTunnel */
		CRYPT_CERTINFO_EXTKEY_IPSECUSER,		/* ipsecUser */
		CRYPT_CERTINFO_EXTKEY_TIMESTAMPING,		/* timeStamping */
		CRYPT_CERTINFO_EXTKEY_OCSPSIGNING,		/* ocspSigning */
		CRYPT_CERTINFO_EXTKEY_DIRECTORYSERVICE,	/* directoryService */
		CRYPT_CERTINFO_EXTKEY_NS_SERVERGATEDCRYPTO,	/* serverGatedCrypto */
		CRYPT_CERTINFO_EXTKEY_VS_SERVERGATEDCRYPTO_CA,	/* serverGatedCrypto CA */
		CRYPT_CERTINFO_FRESHESTCRL,
		CRYPT_CERTINFO_FRESHESTCRL_FULLNAME,	/* distributionPointName.fullName */
		CRYPT_CERTINFO_FRESHESTCRL_REASONS,		/* reasons */
		CRYPT_CERTINFO_FRESHESTCRL_CRLISSUER,	/* cRLIssuer */
		CRYPT_CERTINFO_INHIBITANYPOLICY,
		CRYPT_CERTINFO_NS_CERTTYPE,				/* netscape-cert-type */
		CRYPT_CERTINFO_NS_BASEURL,				/* netscape-base-url */
		CRYPT_CERTINFO_NS_REVOCATIONURL,		/* netscape-revocation-url */
		CRYPT_CERTINFO_NS_CAREVOCATIONURL,		/* netscape-ca-revocation-url */
		CRYPT_CERTINFO_NS_CERTRENEWALURL,		/* netscape-cert-renewal-url */
		CRYPT_CERTINFO_NS_CAPOLICYURL,			/* netscape-ca-policy-url */
		CRYPT_CERTINFO_NS_SSLSERVERNAME,		/* netscape-ssl-server-name */
		CRYPT_CERTINFO_NS_COMMENT,				/* netscape-comment */
		CRYPT_CERTINFO_SET_HASHEDROOTKEY,
		CRYPT_CERTINFO_SET_ROOTKEYTHUMBPRINT,	/* rootKeyThumbPrint */
		CRYPT_CERTINFO_SET_CERTIFICATETYPE,
		CRYPT_CERTINFO_SET_MERCHANTDATA,
		CRYPT_CERTINFO_SET_MERID,				/* merID */
		CRYPT_CERTINFO_SET_MERACQUIRERBIN,		/* merAcquirerBIN */
		CRYPT_CERTINFO_SET_MERCHANTLANGUAGE,	/* merNames.language */
		CRYPT_CERTINFO_SET_MERCHANTNAME,		/* merNames.name */
		CRYPT_CERTINFO_SET_MERCHANTCITY,		/* merNames.city */
		CRYPT_CERTINFO_SET_MERCHANTSTATEPROVINCE,/* merNames.stateProvince */
		CRYPT_CERTINFO_SET_MERCHANTPOSTALCODE,	/* merNames.postalCode */
		CRYPT_CERTINFO_SET_MERCHANTCOUNTRYNAME,	/* merNames.countryName */
		CRYPT_CERTINFO_SET_MERCOUNTRY,			/* merCountry */
		CRYPT_CERTINFO_SET_MERAUTHFLAG,			/* merAuthFlag */
		CRYPT_CERTINFO_SET_CERTCARDREQUIRED,
		CRYPT_CERTINFO_SET_TUNNELING,
		CRYPT_CERTINFO_SET_TUNNELLING,
		CRYPT_CERTINFO_SET_TUNNELINGFLAG,		/* tunneling */
		CRYPT_CERTINFO_SET_TUNNELLINGFLAG,
		CRYPT_CERTINFO_SET_TUNNELINGALGID,		/* tunnelingAlgID */
		CRYPT_CERTINFO_SET_TUNNELLINGALGID,
		CRYPT_CERTINFO_CMS_CONTENTTYPE,
		CRYPT_CERTINFO_CMS_MESSAGEDIGEST,
		CRYPT_CERTINFO_CMS_SIGNINGTIME,
		CRYPT_CERTINFO_CMS_COUNTERSIGNATURE,	/* counterSignature */
		CRYPT_CERTINFO_CMS_SMIMECAPABILITIES,
		CRYPT_CERTINFO_CMS_SMIMECAP_3DES,		/* 3DES encryption */
		CRYPT_CERTINFO_CMS_SMIMECAP_AES,		/* AES encryption */
		CRYPT_CERTINFO_CMS_SMIMECAP_CAST128,	/* CAST-128 encryption */
		CRYPT_CERTINFO_CMS_SMIMECAP_IDEA,		/* IDEA encryption */
		CRYPT_CERTINFO_CMS_SMIMECAP_RC2,		/* RC2 encryption (w.128 key) */
		CRYPT_CERTINFO_CMS_SMIMECAP_RC5,		/* RC5 encryption (w.128 key) */
		CRYPT_CERTINFO_CMS_SMIMECAP_SKIPJACK,	/* Skipjack encryption */
		CRYPT_CERTINFO_CMS_SMIMECAP_DES,		/* DES encryption */
		CRYPT_CERTINFO_CMS_SMIMECAP_PREFERSIGNEDDATA,	/* preferSignedData */
		CRYPT_CERTINFO_CMS_SMIMECAP_CANNOTDECRYPTANY,	/* canNotDecryptAny */
		CRYPT_CERTINFO_CMS_RECEIPTREQUEST,
		CRYPT_CERTINFO_CMS_RECEIPT_CONTENTIDENTIFIER, /* contentIdentifier */
		CRYPT_CERTINFO_CMS_RECEIPT_FROM,		/* receiptsFrom */
		CRYPT_CERTINFO_CMS_RECEIPT_TO,			/* receiptsTo */
		CRYPT_CERTINFO_CMS_SECURITYLABEL,
		CRYPT_CERTINFO_CMS_SECLABEL_CLASSIFICATION, /* securityClassification */
		CRYPT_CERTINFO_CMS_SECLABEL_POLICY,		/* securityPolicyIdentifier */
		CRYPT_CERTINFO_CMS_SECLABEL_PRIVACYMARK,/* privacyMark */
		CRYPT_CERTINFO_CMS_SECLABEL_CATTYPE,	/* securityCategories.securityCategory.type */
		CRYPT_CERTINFO_CMS_SECLABEL_CATVALUE,	/* securityCategories.securityCategory.value */
		CRYPT_CERTINFO_CMS_MLEXPANSIONHISTORY,
		CRYPT_CERTINFO_CMS_MLEXP_ENTITYIDENTIFIER, /* mlData.mailListIdentifier.issuerAndSerialNumber */
		CRYPT_CERTINFO_CMS_MLEXP_TIME,			/* mlData.expansionTime */
		CRYPT_CERTINFO_CMS_MLEXP_NONE,			/* mlData.mlReceiptPolicy.none */
		CRYPT_CERTINFO_CMS_MLEXP_INSTEADOF,		/* mlData.mlReceiptPolicy.insteadOf.generalNames.generalName */
		CRYPT_CERTINFO_CMS_MLEXP_INADDITIONTO,	/* mlData.mlReceiptPolicy.inAdditionTo.generalNames.generalName */
		CRYPT_CERTINFO_CMS_CONTENTHINTS,
		CRYPT_CERTINFO_CMS_CONTENTHINT_DESCRIPTION,	/* contentDescription */
		CRYPT_CERTINFO_CMS_CONTENTHINT_TYPE,	/* contentType */
		CRYPT_CERTINFO_CMS_EQUIVALENTLABEL,
		CRYPT_CERTINFO_CMS_EQVLABEL_POLICY,		/* securityPolicyIdentifier */
		CRYPT_CERTINFO_CMS_EQVLABEL_CLASSIFICATION, /* securityClassification */
		CRYPT_CERTINFO_CMS_EQVLABEL_PRIVACYMARK,/* privacyMark */
		CRYPT_CERTINFO_CMS_EQVLABEL_CATTYPE,	/* securityCategories.securityCategory.type */
		CRYPT_CERTINFO_CMS_EQVLABEL_CATVALUE,	/* securityCategories.securityCategory.value */
		CRYPT_CERTINFO_CMS_SIGNINGCERTIFICATE,
		CRYPT_CERTINFO_CMS_SIGNINGCERT_ESSCERTID, /* certs.essCertID */
		CRYPT_CERTINFO_CMS_SIGNINGCERT_POLICIES,/* policies.policyInformation.policyIdentifier */
		CRYPT_CERTINFO_CMS_SIGTYPEIDENTIFIER,
		CRYPT_CERTINFO_CMS_SIGTYPEID_ORIGINATORSIG, /* originatorSig */
		CRYPT_CERTINFO_CMS_SIGTYPEID_DOMAINSIG,	/* domainSig */
		CRYPT_CERTINFO_CMS_SIGTYPEID_ADDITIONALATTRIBUTES, /* additionalAttributesSig */
		CRYPT_CERTINFO_CMS_SIGTYPEID_REVIEWSIG,	/* reviewSig */
		CRYPT_CERTINFO_CMS_NONCE,				/* randomNonce */
		CRYPT_CERTINFO_SCEP_MESSAGETYPE,		/* messageType */
		CRYPT_CERTINFO_SCEP_PKISTATUS,			/* pkiStatus */
		CRYPT_CERTINFO_SCEP_FAILINFO,			/* failInfo */
		CRYPT_CERTINFO_SCEP_SENDERNONCE,		/* senderNonce */
		CRYPT_CERTINFO_SCEP_RECIPIENTNONCE,		/* recipientNonce */
		CRYPT_CERTINFO_SCEP_TRANSACTIONID,		/* transID */
		CRYPT_CERTINFO_CMS_SPCAGENCYINFO,
		CRYPT_CERTINFO_CMS_SPCAGENCYURL,		/* spcAgencyInfo.url */
		CRYPT_CERTINFO_CMS_SPCSTATEMENTTYPE,
		CRYPT_CERTINFO_CMS_SPCSTMT_INDIVIDUALCODESIGNING,	/* individualCodeSigning */
		CRYPT_CERTINFO_CMS_SPCSTMT_COMMERCIALCODESIGNING,	/* commercialCodeSigning */
		CRYPT_CERTINFO_CMS_SPCOPUSINFO,
		CRYPT_CERTINFO_CMS_SPCOPUSINFO_NAME,	/* spcOpusInfo.name */
		CRYPT_CERTINFO_CMS_SPCOPUSINFO_URL,		/* spcOpusInfo.url */
		CRYPT_CERTINFO_LAST,
		CRYPT_KEYINFO_FIRST,
		CRYPT_KEYINFO_QUERY,			/* Keyset query */
		CRYPT_KEYINFO_QUERY_REQUESTS,	/* Query of requests in cert store */
		CRYPT_KEYINFO_LAST,
		CRYPT_DEVINFO_FIRST,
		CRYPT_DEVINFO_INITIALISE,	/* Initialise device for use */
		CRYPT_DEVINFO_INITIALIZE,
		CRYPT_DEVINFO_AUTHENT_USER,	/* Authenticate user to device */
		CRYPT_DEVINFO_AUTHENT_SUPERVISOR,	/* Authenticate supervisor to dev.*/
		CRYPT_DEVINFO_SET_AUTHENT_USER,	/* Set user authent.value */
		CRYPT_DEVINFO_SET_AUTHENT_SUPERVISOR,	/* Set supervisor auth.val.*/
		CRYPT_DEVINFO_ZEROISE,	/* Zeroise device */
		CRYPT_DEVINFO_ZEROIZE,
		CRYPT_DEVINFO_LOGGEDIN,		/* Whether user is logged in */
		CRYPT_DEVINFO_LABEL,		/* Device/token label */
		CRYPT_DEVINFO_LAST,
		CRYPT_ENVINFO_FIRST,
		CRYPT_ENVINFO_DATASIZE,			/* Data size information */
		CRYPT_ENVINFO_COMPRESSION,		/* Compression information */
		CRYPT_ENVINFO_CONTENTTYPE,		/* Inner CMS content type */
		CRYPT_ENVINFO_DETACHEDSIGNATURE,/* Generate CMS detached signature */
		CRYPT_ENVINFO_SIGNATURE_RESULT,	/* Signature check result */
		CRYPT_ENVINFO_MAC,				/* Use MAC instead of encrypting */
		CRYPT_ENVINFO_CURRENT_COMPONENT,/* Cursor management: Relative pos.*/
		CRYPT_ENVINFO_PASSWORD,			/* User password */
		CRYPT_ENVINFO_KEY,				/* Conventional encryption key */
		CRYPT_ENVINFO_SIGNATURE,		/* Signature/signature check key */
		CRYPT_ENVINFO_SIGNATURE_EXTRADATA,	/* Extra information added to CMS sigs */
		CRYPT_ENVINFO_RECIPIENT,		/* Recipient email address */
		CRYPT_ENVINFO_PUBLICKEY,		/* PKC encryption key */
		CRYPT_ENVINFO_PRIVATEKEY,		/* PKC decryption key */
		CRYPT_ENVINFO_PRIVATEKEY_LABEL,	/* Label of PKC decryption key */
		CRYPT_ENVINFO_ORIGINATOR,		/* Originator info/key */
		CRYPT_ENVINFO_SESSIONKEY,		/* Session key */
		CRYPT_ENVINFO_HASH,				/* Hash value */
		CRYPT_ENVINFO_TIMESTAMP_AUTHORITY,	/* Timestamp authority */
		CRYPT_ENVINFO_KEYSET_SIGCHECK,	/* Signature check keyset */
		CRYPT_ENVINFO_KEYSET_ENCRYPT,	/* PKC encryption keyset */
		CRYPT_ENVINFO_KEYSET_DECRYPT,	/* PKC decryption keyset */
		CRYPT_ENVINFO_LAST,
		CRYPT_SESSINFO_FIRST,
		CRYPT_SESSINFO_ACTIVE,			/* Whether session is active */
		CRYPT_SESSINFO_CONNECTIONACTIVE,/* Whether network connection is active */
		CRYPT_SESSINFO_USERNAME,		/* User name */
		CRYPT_SESSINFO_PASSWORD,		/* Password */
		CRYPT_SESSINFO_PRIVATEKEY,		/* Server/client private key */
		CRYPT_SESSINFO_KEYSET,			/* Certificate store */
		CRYPT_SESSINFO_SERVER_NAME,		/* Server name */
		CRYPT_SESSINFO_SERVER_PORT,		/* Server port number */
		CRYPT_SESSINFO_SERVER_FINGERPRINT,/* Server key fingerprint */
		CRYPT_SESSINFO_CLIENT_NAME,		/* Client name */
		CRYPT_SESSINFO_CLIENT_PORT,		/* Client port number */
		CRYPT_SESSINFO_SESSION,			/* Transport mechanism */
		CRYPT_SESSINFO_NETWORKSOCKET,	/* User-supplied network socket */
		CRYPT_SESSINFO_VERSION,			/* Protocol version */
		CRYPT_SESSINFO_REQUEST,			/* Cert.request object */
		CRYPT_SESSINFO_RESPONSE,		/* Cert.response object */
		CRYPT_SESSINFO_CACERTIFICATE,	/* Issuing CA certificate */
		CRYPT_SESSINFO_TSP_MSGIMPRINT,	/* TSP message imprint */
		CRYPT_SESSINFO_CMP_REQUESTTYPE,	/* Request type */
		CRYPT_SESSINFO_CMP_PKIBOOT,		/* Enable PKIBoot facility */
		CRYPT_SESSINFO_CMP_PRIVKEYSET,	/* Private-key keyset */
		CRYPT_SESSINFO_SSH_SUBSYSTEM,	/* SSH subsystem */
		CRYPT_SESSINFO_SSH_PORTFORWARD,	/* SSH port forwarding */
		CRYPT_SESSINFO_LAST,
		CRYPT_USERINFO_FIRST,
		CRYPT_USERINFO_PASSWORD,		/* Password */
		CRYPT_USERINFO_CAKEY_CERTSIGN,	/* CA cert signing key */
		CRYPT_USERINFO_CAKEY_CRLSIGN,	/* CA CRL signing key */
		CRYPT_USERINFO_CAKEY_RTCSSIGN,	/* CA RTCS signing key */
		CRYPT_USERINFO_CAKEY_OCSPSIGN,	/* CA OCSP signing key */
		CRYPT_USERINFO_LAST,
		CRYPT_ATTRIBUTE_LAST
	};

	CL_ATTRIBUTE_TYPE	cl[] = {
		CL_ATTRIBUTE_NONE,			/* Non-value */
		CL_PROPERTY_FIRST,
		CL_PROPERTY_HIGHSECURITY,	/* Owned+non-forwardcount+locked */
		CL_PROPERTY_OWNER,			/* Object owner */
		CL_PROPERTY_FORWARDCOUNT,	/* No.of times object can be forwarded */
		CL_PROPERTY_LOCKED,			/* Whether properties can be chged/read */
		CL_PROPERTY_USAGECOUNT,		/* Usage count before object expires */
		CL_PROPERTY_NONEXPORTABLE,	/* Whether key is nonexp.from context */
		CL_PROPERTY_LAST, CL_GENERIC_FIRST,
		CL_ATTRIBUTE_ERRORTYPE,		/* Type of last error */
		CL_ATTRIBUTE_ERRORLOCUS,		/* Locus of last error */
		CL_ATTRIBUTE_INT_ERRORCODE,	/* Low-level software-specific */
		CL_ATTRIBUTE_INT_ERRORMESSAGE, /*   error code and message */
		CL_ATTRIBUTE_BUFFERSIZE,		/* Internal data buffer size */
		CL_GENERIC_LAST,
		CL_OPTION_FIRST,
		CL_OPTION_INFO_DESCRIPTION,	/* Text description */
		CL_OPTION_INFO_COPYRIGHT,	/* Copyright notice */
		CL_OPTION_INFO_MAJORVERSION,	/* Major release version */
		CL_OPTION_INFO_MINORVERSION,	/* Minor release version */
		CL_OPTION_INFO_STEPPING,		/* Release stepping */
		CL_OPTION_ENCR_ALGO,			/* Encryption algorithm */
		CL_OPTION_ENCR_HASH,			/* Hash algorithm */
		CL_OPTION_ENCR_MAC,			/* MAC algorithm */
		CL_OPTION_PKC_ALGO,			/* Public-key encryption algorithm */
		CL_OPTION_PKC_KEYSIZE,		/* Public-key encryption key size */
		CL_OPTION_SIG_ALGO,			/* Signature algorithm */
		CL_OPTION_SIG_KEYSIZE,		/* Signature keysize */
		CL_OPTION_KEYING_ALGO,		/* Key processing algorithm */
		CL_OPTION_KEYING_ITERATIONS,	/* Key processing iterations */
		CL_OPTION_CERT_SIGNUNRECOGNISEDATTRIBUTES,	/* Whether to sign unrecog.attrs */
		CL_OPTION_CERT_VALIDITY,		/* Certificate validity period */
		CL_OPTION_CERT_UPDATEINTERVAL,	/* CRL update interval */
		CL_OPTION_CERT_COMPLIANCELEVEL,	/* PKIX compliance level for cert chks.*/
		CL_OPTION_CMS_DEFAULTATTRIBUTES,	/* Add default CMS attributes */
		CL_OPTION_SMIME_DEFAULTATTRIBUTES,
		CL_OPTION_KEYS_LDAP_OBJECTCLASS,	/* Object class */
		CL_OPTION_KEYS_LDAP_OBJECTTYPE,	/* Object type to fetch */
		CL_OPTION_KEYS_LDAP_FILTER,		/* Query filter */
		CL_OPTION_KEYS_LDAP_CACERTNAME,	/* CA certificate attribute name */
		CL_OPTION_KEYS_LDAP_CERTNAME,	/* Certificate attribute name */
		CL_OPTION_KEYS_LDAP_CRLNAME,		/* CRL attribute name */
		CL_OPTION_KEYS_LDAP_EMAILNAME,	/* Email attribute name */
		CL_OPTION_DEVICE_PKCS11_DVR01,	/* Name of first PKCS #11 driver */
		CL_OPTION_DEVICE_PKCS11_DVR02,	/* Name of second PKCS #11 driver */
		CL_OPTION_DEVICE_PKCS11_DVR03,	/* Name of third PKCS #11 driver */
		CL_OPTION_DEVICE_PKCS11_DVR04,	/* Name of fourth PKCS #11 driver */
		CL_OPTION_DEVICE_PKCS11_DVR05,	/* Name of fifth PKCS #11 driver */
		CL_OPTION_DEVICE_PKCS11_HARDWAREONLY,/* Use only hardware mechanisms */
		CL_OPTION_NET_SOCKS_SERVER,		/* Socks server name */
		CL_OPTION_NET_SOCKS_USERNAME,	/* Socks user name */
		CL_OPTION_NET_HTTP_PROXY,		/* Web proxy server */
		CL_OPTION_NET_CONNECTTIMEOUT,	/* Timeout for network connection setup */
		CL_OPTION_NET_TIMEOUT,			/* Timeout for network read/write */
		CL_OPTION_MISC_ASYNCINIT,	/* Whether to init cryptlib async'ly */
		CL_OPTION_MISC_SIDECHANNELPROTECTION, /* Protect against side-channel attacks */
		CL_OPTION_CONFIGCHANGED,		/* Whether in-mem.opts match on-disk ones */
		CL_OPTION_SELFTESTOK,		/* Whether self-test was completed and OK */
		CL_OPTION_LAST,
		CL_CTXINFO_FIRST,
		CL_CTXINFO_ALGO,				/* Algorithm */
		CL_CTXINFO_MODE,				/* Mode */
		CL_CTXINFO_NAME_ALGO,		/* Algorithm name */
		CL_CTXINFO_NAME_MODE,		/* Mode name */
		CL_CTXINFO_KEYSIZE,			/* Key size in bytes */
		CL_CTXINFO_BLOCKSIZE,		/* Block size */
		CL_CTXINFO_IVSIZE,			/* IV size */
		CL_CTXINFO_KEYING_ALGO,		/* Key processing algorithm */
		CL_CTXINFO_KEYING_ITERATIONS,/* Key processing iterations */
		CL_CTXINFO_KEYING_SALT,		/* Key processing salt */
		CL_CTXINFO_KEYING_VALUE,		/* Value used to derive key */
		CL_CTXINFO_KEY,				/* Key */
		CL_CTXINFO_KEY_COMPONENTS,	/* Public-key components */
		CL_CTXINFO_IV,				/* IV */
		CL_CTXINFO_HASHVALUE,		/* Hash value */
		CL_CTXINFO_LABEL,			/* Label for private/secret key */
		CL_CTXINFO_LAST,
		CL_CERTINFO_FIRST,
		CL_CERTINFO_SELFSIGNED,		/* Cert is self-signed */
		CL_CERTINFO_IMMUTABLE,		/* Cert is signed and immutable */
		CL_CERTINFO_XYZZY,			/* Cert is a magic just-works cert */
		CL_CERTINFO_CERTTYPE,		/* Certificate object type */
		CL_CERTINFO_FINGERPRINT,		/* Certificate fingerprints */
		CL_CERTINFO_FINGERPRINT_MD5,
		CL_CERTINFO_FINGERPRINT_SHA,
		CL_CERTINFO_CURRENT_CERTIFICATE,/* Cursor mgt: Rel.pos in chain/CRL/OCSP */
		CL_CERTINFO_CURRENT_EXTENSION,/* Cursor mgt: Rel.pos.or abs.extension */
		CL_CERTINFO_CURRENT_FIELD,	/* Cursor mgt: Rel.pos.or abs.field in ext */
		CL_CERTINFO_CURRENT_COMPONENT,/* Cursor mgt: Rel.pos in multival.field */
		CL_CERTINFO_TRUSTED_USAGE,	/* Usage that cert is trusted for */
		CL_CERTINFO_TRUSTED_IMPLICIT,/* Whether cert is implicitly trusted */
		CL_CERTINFO_SIGNATURELEVEL,	/* Amount of detail to include in sigs.*/
		CL_CERTINFO_VERSION,			/* Cert.format version */
		CL_CERTINFO_SERIALNUMBER,	/* Serial number */
		CL_CERTINFO_SUBJECTPUBLICKEYINFO,	/* Public key */
		CL_CERTINFO_CERTIFICATE,		/* User certificate */
		CL_CERTINFO_USERCERTIFICATE,
		CL_CERTINFO_CACERTIFICATE,	/* CA certificate */
		CL_CERTINFO_ISSUERNAME,		/* Issuer DN */
		CL_CERTINFO_VALIDFROM,		/* Cert valid-from time */
		CL_CERTINFO_VALIDTO,			/* Cert valid-to time */
		CL_CERTINFO_SUBJECTNAME,		/* Subject DN */
		CL_CERTINFO_ISSUERUNIQUEID,	/* Issuer unique ID */
		CL_CERTINFO_SUBJECTUNIQUEID,	/* Subject unique ID */
		CL_CERTINFO_CERTREQUEST,		/* Cert.request (DN + public key) */
		CL_CERTINFO_THISUPDATE,		/* CRL/OCSP current-update time */
		CL_CERTINFO_NEXTUPDATE,		/* CRL/OCSP next-update time */
		CL_CERTINFO_REVOCATIONDATE,	/* CRL/OCSP cert-revocation time */
		CL_CERTINFO_REVOCATIONSTATUS,/* OCSP revocation status */
		CL_CERTINFO_CERTSTATUS,		/* RTCS certificate status */
		CL_CERTINFO_DN,				/* Currently selected DN in string form */
		CL_CERTINFO_PKIUSER_ID,		/* PKI user ID */
		CL_CERTINFO_PKIUSER_ISSUEPASSWORD,	/* PKI user issue password */
		CL_CERTINFO_PKIUSER_REVPASSWORD,		/* PKI user revocation password */
		CL_CERTINFO_COUNTRYNAME,	/* countryName */
		CL_CERTINFO_STATEORPROVINCENAME,	/* stateOrProvinceName */
		CL_CERTINFO_LOCALITYNAME,		/* localityName */
		CL_CERTINFO_ORGANIZATIONNAME,	/* organizationName */
		CL_CERTINFO_ORGANISATIONNAME,
		CL_CERTINFO_ORGANIZATIONALUNITNAME,	/* organizationalUnitName */
		CL_CERTINFO_ORGANISATIONALUNITNAME,
		CL_CERTINFO_COMMONNAME,		/* commonName */
		CL_CERTINFO_OTHERNAME_TYPEID,		/* otherName.typeID */
		CL_CERTINFO_OTHERNAME_VALUE,			/* otherName.value */
		CL_CERTINFO_RFC822NAME,				/* rfc822Name */
		CL_CERTINFO_EMAIL,
		CL_CERTINFO_DNSNAME,					/* dNSName */
		CL_CERTINFO_DIRECTORYNAME,			/* directoryName */
		CL_CERTINFO_EDIPARTYNAME_NAMEASSIGNER,	/* ediPartyName.nameAssigner */
		CL_CERTINFO_EDIPARTYNAME_PARTYNAME,	/* ediPartyName.partyName */
		CL_CERTINFO_UNIFORMRESOURCEIDENTIFIER,	/* uniformResourceIdentifier */
		CL_CERTINFO_IPADDRESS,				/* iPAddress */
		CL_CERTINFO_REGISTEREDID,			/* registeredID */
		CL_CERTINFO_KEYFEATURES,
		CL_CERTINFO_AUTHORITYINFOACCESS,
		CL_CERTINFO_AUTHORITYINFO_RTCS,		/* accessDescription.accessLocation */
		CL_CERTINFO_AUTHORITYINFO_OCSP,		/* accessDescription.accessLocation */
		CL_CERTINFO_AUTHORITYINFO_CAISSUERS,	/* accessDescription.accessLocation */
		CL_CERTINFO_AUTHORITYINFO_TIMESTAMPING,/* accessDescription.accessLocation */
		CL_CERTINFO_OCSP_NONCE,				/* nonce */
		CL_CERTINFO_OCSP_RESPONSE,
		CL_CERTINFO_OCSP_RESPONSE_OCSP,		/* OCSP standard response */
		CL_CERTINFO_OCSP_NOCHECK,
		CL_CERTINFO_OCSP_ARCHIVECUTOFF,
		CL_CERTINFO_SUBJECTINFOACCESS,
		CL_CERTINFO_SUBJECTINFO_CAREPOSITORY,/* accessDescription.accessLocation */
		CL_CERTINFO_SUBJECTINFO_TIMESTAMPING,/* accessDescription.accessLocation */
		CL_CERTINFO_SIGG_DATEOFCERTGEN,
		CL_CERTINFO_SIGG_PROCURATION,
		CL_CERTINFO_SIGG_PROCURE_COUNTRY,	/* country */
		CL_CERTINFO_SIGG_PROCURE_TYPEOFSUBSTITUTION,	/* typeOfSubstitution */
		CL_CERTINFO_SIGG_PROCURE_SIGNINGFOR,	/* signingFor.thirdPerson */
		CL_CERTINFO_SIGG_MONETARYLIMIT,
		CL_CERTINFO_SIGG_MONETARY_CURRENCY,	/* currency */
		CL_CERTINFO_SIGG_MONETARY_AMOUNT,	/* amount */
		CL_CERTINFO_SIGG_MONETARY_EXPONENT,	/* exponent */
		CL_CERTINFO_SIGG_RESTRICTION,
		CL_CERTINFO_STRONGEXTRANET,
		CL_CERTINFO_STRONGEXTRANET_ZONE,		/* sxNetIDList.sxNetID.zone */
		CL_CERTINFO_STRONGEXTRANET_ID,		/* sxNetIDList.sxNetID.id */
		CL_CERTINFO_SUBJECTDIRECTORYATTRIBUTES,
		CL_CERTINFO_SUBJECTDIR_TYPE,			/* attribute.type */
		CL_CERTINFO_SUBJECTDIR_VALUES,		/* attribute.values */
		CL_CERTINFO_SUBJECTKEYIDENTIFIER,
		CL_CERTINFO_KEYUSAGE,
		CL_CERTINFO_PRIVATEKEYUSAGEPERIOD,
		CL_CERTINFO_PRIVATEKEY_NOTBEFORE,	/* notBefore */
		CL_CERTINFO_PRIVATEKEY_NOTAFTER,		/* notAfter */
		CL_CERTINFO_SUBJECTALTNAME,
		CL_CERTINFO_ISSUERALTNAME,
		CL_CERTINFO_BASICCONSTRAINTS,
		CL_CERTINFO_CA,						/* cA */
		CL_CERTINFO_AUTHORITY,
		CL_CERTINFO_PATHLENCONSTRAINT,		/* pathLenConstraint */
		CL_CERTINFO_CRLNUMBER,
		CL_CERTINFO_CRLREASON,
		CL_CERTINFO_HOLDINSTRUCTIONCODE,
		CL_CERTINFO_INVALIDITYDATE,
		CL_CERTINFO_DELTACRLINDICATOR,
		CL_CERTINFO_ISSUINGDISTRIBUTIONPOINT,
		CL_CERTINFO_ISSUINGDIST_FULLNAME,	/* distributionPointName.fullName */
		CL_CERTINFO_ISSUINGDIST_USERCERTSONLY,	/* onlyContainsUserCerts */
		CL_CERTINFO_ISSUINGDIST_CACERTSONLY,	/* onlyContainsCACerts */
		CL_CERTINFO_ISSUINGDIST_SOMEREASONSONLY,	/* onlySomeReasons */
		CL_CERTINFO_ISSUINGDIST_INDIRECTCRL,	/* indirectCRL */
		CL_CERTINFO_CERTIFICATEISSUER,
		CL_CERTINFO_NAMECONSTRAINTS,
		CL_CERTINFO_PERMITTEDSUBTREES,		/* permittedSubtrees */
		CL_CERTINFO_EXCLUDEDSUBTREES,		/* excludedSubtrees */
		CL_CERTINFO_CRLDISTRIBUTIONPOINT,
		CL_CERTINFO_CRLDIST_FULLNAME,		/* distributionPointName.fullName */
		CL_CERTINFO_CRLDIST_REASONS,			/* reasons */
		CL_CERTINFO_CRLDIST_CRLISSUER,		/* cRLIssuer */
		CL_CERTINFO_CERTIFICATEPOLICIES,
		CL_CERTINFO_CERTPOLICYID,		/* policyInformation.policyIdentifier */
		CL_CERTINFO_CERTPOLICY_CPSURI,
		CL_CERTINFO_CERTPOLICY_ORGANIZATION,
		CL_CERTINFO_CERTPOLICY_NOTICENUMBERS,
		CL_CERTINFO_CERTPOLICY_EXPLICITTEXT,
		CL_CERTINFO_POLICYMAPPINGS,
		CL_CERTINFO_ISSUERDOMAINPOLICY,	/* policyMappings.issuerDomainPolicy */
		CL_CERTINFO_SUBJECTDOMAINPOLICY,	/* policyMappings.subjectDomainPolicy */
		CL_CERTINFO_AUTHORITYKEYIDENTIFIER,
		CL_CERTINFO_AUTHORITY_KEYIDENTIFIER,	/* keyIdentifier */
		CL_CERTINFO_AUTHORITY_CERTISSUER,	/* authorityCertIssuer */
		CL_CERTINFO_AUTHORITY_CERTSERIALNUMBER,	/* authorityCertSerialNumber */
		CL_CERTINFO_POLICYCONSTRAINTS,
		CL_CERTINFO_REQUIREEXPLICITPOLICY,	/* policyConstraints.requireExplicitPolicy */
		CL_CERTINFO_INHIBITPOLICYMAPPING,	/* policyConstraints.inhibitPolicyMapping */
		CL_CERTINFO_EXTKEYUSAGE,
		CL_CERTINFO_EXTKEY_MS_INDIVIDUALCODESIGNING,	/* individualCodeSigning */
		CL_CERTINFO_EXTKEY_MS_COMMERCIALCODESIGNING,	/* commercialCodeSigning */
		CL_CERTINFO_EXTKEY_MS_CERTTRUSTLISTSIGNING,	/* certTrustListSigning */
		CL_CERTINFO_EXTKEY_MS_TIMESTAMPSIGNING,	/* timeStampSigning */
		CL_CERTINFO_EXTKEY_MS_SERVERGATEDCRYPTO,	/* serverGatedCrypto */
		CL_CERTINFO_EXTKEY_MS_ENCRYPTEDFILESYSTEM,	/* encrypedFileSystem */
		CL_CERTINFO_EXTKEY_SERVERAUTH,		/* serverAuth */
		CL_CERTINFO_EXTKEY_CLIENTAUTH,		/* clientAuth */
		CL_CERTINFO_EXTKEY_CODESIGNING,		/* codeSigning */
		CL_CERTINFO_EXTKEY_EMAILPROTECTION,	/* emailProtection */
		CL_CERTINFO_EXTKEY_IPSECENDSYSTEM,	/* ipsecEndSystem */
		CL_CERTINFO_EXTKEY_IPSECTUNNEL,		/* ipsecTunnel */
		CL_CERTINFO_EXTKEY_IPSECUSER,		/* ipsecUser */
		CL_CERTINFO_EXTKEY_TIMESTAMPING,		/* timeStamping */
		CL_CERTINFO_EXTKEY_OCSPSIGNING,		/* ocspSigning */
		CL_CERTINFO_EXTKEY_DIRECTORYSERVICE,	/* directoryService */
		CL_CERTINFO_EXTKEY_NS_SERVERGATEDCRYPTO,	/* serverGatedCrypto */
		CL_CERTINFO_EXTKEY_VS_SERVERGATEDCRYPTO_CA,	/* serverGatedCrypto CA */
		CL_CERTINFO_FRESHESTCRL,
		CL_CERTINFO_FRESHESTCRL_FULLNAME,	/* distributionPointName.fullName */
		CL_CERTINFO_FRESHESTCRL_REASONS,		/* reasons */
		CL_CERTINFO_FRESHESTCRL_CRLISSUER,	/* cRLIssuer */
		CL_CERTINFO_INHIBITANYPOLICY,
		CL_CERTINFO_NS_CERTTYPE,				/* netscape-cert-type */
		CL_CERTINFO_NS_BASEURL,				/* netscape-base-url */
		CL_CERTINFO_NS_REVOCATIONURL,		/* netscape-revocation-url */
		CL_CERTINFO_NS_CAREVOCATIONURL,		/* netscape-ca-revocation-url */
		CL_CERTINFO_NS_CERTRENEWALURL,		/* netscape-cert-renewal-url */
		CL_CERTINFO_NS_CAPOLICYURL,			/* netscape-ca-policy-url */
		CL_CERTINFO_NS_SSLSERVERNAME,		/* netscape-ssl-server-name */
		CL_CERTINFO_NS_COMMENT,				/* netscape-comment */
		CL_CERTINFO_SET_HASHEDROOTKEY,
		CL_CERTINFO_SET_ROOTKEYTHUMBPRINT,	/* rootKeyThumbPrint */
		CL_CERTINFO_SET_CERTIFICATETYPE,
		CL_CERTINFO_SET_MERCHANTDATA,
		CL_CERTINFO_SET_MERID,				/* merID */
		CL_CERTINFO_SET_MERACQUIRERBIN,		/* merAcquirerBIN */
		CL_CERTINFO_SET_MERCHANTLANGUAGE,	/* merNames.language */
		CL_CERTINFO_SET_MERCHANTNAME,		/* merNames.name */
		CL_CERTINFO_SET_MERCHANTCITY,		/* merNames.city */
		CL_CERTINFO_SET_MERCHANTSTATEPROVINCE,/* merNames.stateProvince */
		CL_CERTINFO_SET_MERCHANTPOSTALCODE,	/* merNames.postalCode */
		CL_CERTINFO_SET_MERCHANTCOUNTRYNAME,	/* merNames.countryName */
		CL_CERTINFO_SET_MERCOUNTRY,			/* merCountry */
		CL_CERTINFO_SET_MERAUTHFLAG,			/* merAuthFlag */
		CL_CERTINFO_SET_CERTCARDREQUIRED,
		CL_CERTINFO_SET_TUNNELING,
		CL_CERTINFO_SET_TUNNELLING,
		CL_CERTINFO_SET_TUNNELINGFLAG,		/* tunneling */
		CL_CERTINFO_SET_TUNNELLINGFLAG,
		CL_CERTINFO_SET_TUNNELINGALGID,		/* tunnelingAlgID */
		CL_CERTINFO_SET_TUNNELLINGALGID,
		CL_CERTINFO_CMS_CONTENTTYPE,
		CL_CERTINFO_CMS_MESSAGEDIGEST,
		CL_CERTINFO_CMS_SIGNINGTIME,
		CL_CERTINFO_CMS_COUNTERSIGNATURE,	/* counterSignature */
		CL_CERTINFO_CMS_SMIMECAPABILITIES,
		CL_CERTINFO_CMS_SMIMECAP_3DES,		/* 3DES encryption */
		CL_CERTINFO_CMS_SMIMECAP_AES,		/* AES encryption */
		CL_CERTINFO_CMS_SMIMECAP_CAST128,	/* CAST-128 encryption */
		CL_CERTINFO_CMS_SMIMECAP_IDEA,		/* IDEA encryption */
		CL_CERTINFO_CMS_SMIMECAP_RC2,		/* RC2 encryption (w.128 key) */
		CL_CERTINFO_CMS_SMIMECAP_RC5,		/* RC5 encryption (w.128 key) */
		CL_CERTINFO_CMS_SMIMECAP_SKIPJACK,	/* Skipjack encryption */
		CL_CERTINFO_CMS_SMIMECAP_DES,		/* DES encryption */
		CL_CERTINFO_CMS_SMIMECAP_PREFERSIGNEDDATA,	/* preferSignedData */
		CL_CERTINFO_CMS_SMIMECAP_CANNOTDECRYPTANY,	/* canNotDecryptAny */
		CL_CERTINFO_CMS_RECEIPTREQUEST,
		CL_CERTINFO_CMS_RECEIPT_CONTENTIDENTIFIER, /* contentIdentifier */
		CL_CERTINFO_CMS_RECEIPT_FROM,		/* receiptsFrom */
		CL_CERTINFO_CMS_RECEIPT_TO,			/* receiptsTo */
		CL_CERTINFO_CMS_SECURITYLABEL,
		CL_CERTINFO_CMS_SECLABEL_CLASSIFICATION, /* securityClassification */
		CL_CERTINFO_CMS_SECLABEL_POLICY,		/* securityPolicyIdentifier */
		CL_CERTINFO_CMS_SECLABEL_PRIVACYMARK,/* privacyMark */
		CL_CERTINFO_CMS_SECLABEL_CATTYPE,	/* securityCategories.securityCategory.type */
		CL_CERTINFO_CMS_SECLABEL_CATVALUE,	/* securityCategories.securityCategory.value */
		CL_CERTINFO_CMS_MLEXPANSIONHISTORY,
		CL_CERTINFO_CMS_MLEXP_ENTITYIDENTIFIER, /* mlData.mailListIdentifier.issuerAndSerialNumber */
		CL_CERTINFO_CMS_MLEXP_TIME,			/* mlData.expansionTime */
		CL_CERTINFO_CMS_MLEXP_NONE,			/* mlData.mlReceiptPolicy.none */
		CL_CERTINFO_CMS_MLEXP_INSTEADOF,		/* mlData.mlReceiptPolicy.insteadOf.generalNames.generalName */
		CL_CERTINFO_CMS_MLEXP_INADDITIONTO,	/* mlData.mlReceiptPolicy.inAdditionTo.generalNames.generalName */
		CL_CERTINFO_CMS_CONTENTHINTS,
		CL_CERTINFO_CMS_CONTENTHINT_DESCRIPTION,	/* contentDescription */
		CL_CERTINFO_CMS_CONTENTHINT_TYPE,	/* contentType */
		CL_CERTINFO_CMS_EQUIVALENTLABEL,
		CL_CERTINFO_CMS_EQVLABEL_POLICY,		/* securityPolicyIdentifier */
		CL_CERTINFO_CMS_EQVLABEL_CLASSIFICATION, /* securityClassification */
		CL_CERTINFO_CMS_EQVLABEL_PRIVACYMARK,/* privacyMark */
		CL_CERTINFO_CMS_EQVLABEL_CATTYPE,	/* securityCategories.securityCategory.type */
		CL_CERTINFO_CMS_EQVLABEL_CATVALUE,	/* securityCategories.securityCategory.value */
		CL_CERTINFO_CMS_SIGNINGCERTIFICATE,
		CL_CERTINFO_CMS_SIGNINGCERT_ESSCERTID, /* certs.essCertID */
		CL_CERTINFO_CMS_SIGNINGCERT_POLICIES,/* policies.policyInformation.policyIdentifier */
		CL_CERTINFO_CMS_SIGTYPEIDENTIFIER,
		CL_CERTINFO_CMS_SIGTYPEID_ORIGINATORSIG, /* originatorSig */
		CL_CERTINFO_CMS_SIGTYPEID_DOMAINSIG,	/* domainSig */
		CL_CERTINFO_CMS_SIGTYPEID_ADDITIONALATTRIBUTES, /* additionalAttributesSig */
		CL_CERTINFO_CMS_SIGTYPEID_REVIEWSIG,	/* reviewSig */
		CL_CERTINFO_CMS_NONCE,				/* randomNonce */
		CL_CERTINFO_SCEP_MESSAGETYPE,		/* messageType */
		CL_CERTINFO_SCEP_PKISTATUS,			/* pkiStatus */
		CL_CERTINFO_SCEP_FAILINFO,			/* failInfo */
		CL_CERTINFO_SCEP_SENDERNONCE,		/* senderNonce */
		CL_CERTINFO_SCEP_RECIPIENTNONCE,		/* recipientNonce */
		CL_CERTINFO_SCEP_TRANSACTIONID,		/* transID */
		CL_CERTINFO_CMS_SPCAGENCYINFO,
		CL_CERTINFO_CMS_SPCAGENCYURL,		/* spcAgencyInfo.url */
		CL_CERTINFO_CMS_SPCSTATEMENTTYPE,
		CL_CERTINFO_CMS_SPCSTMT_INDIVIDUALCODESIGNING,	/* individualCodeSigning */
		CL_CERTINFO_CMS_SPCSTMT_COMMERCIALCODESIGNING,	/* commercialCodeSigning */
		CL_CERTINFO_CMS_SPCOPUSINFO,
		CL_CERTINFO_CMS_SPCOPUSINFO_NAME,	/* spcOpusInfo.name */
		CL_CERTINFO_CMS_SPCOPUSINFO_URL,		/* spcOpusInfo.url */
		CL_CERTINFO_LAST,
		CL_KEYINFO_FIRST,
		CL_KEYINFO_QUERY,			/* Keyset query */
		CL_KEYINFO_QUERY_REQUESTS,	/* Query of requests in cert store */
		CL_KEYINFO_LAST,
		CL_DEVINFO_FIRST,
		CL_DEVINFO_INITIALISE,	/* Initialise device for use */
		CL_DEVINFO_INITIALIZE,
		CL_DEVINFO_AUTHENT_USER,	/* Authenticate user to device */
		CL_DEVINFO_AUTHENT_SUPERVISOR,	/* Authenticate supervisor to dev.*/
		CL_DEVINFO_SET_AUTHENT_USER,	/* Set user authent.value */
		CL_DEVINFO_SET_AUTHENT_SUPERVISOR,	/* Set supervisor auth.val.*/
		CL_DEVINFO_ZEROISE,	/* Zeroise device */
		CL_DEVINFO_ZEROIZE,
		CL_DEVINFO_LOGGEDIN,		/* Whether user is logged in */
		CL_DEVINFO_LABEL,		/* Device/token label */
		CL_DEVINFO_LAST,
		CL_ENVINFO_FIRST,
		CL_ENVINFO_DATASIZE,			/* Data size information */
		CL_ENVINFO_COMPRESSION,		/* Compression information */
		CL_ENVINFO_CONTENTTYPE,		/* Inner CMS content type */
		CL_ENVINFO_DETACHEDSIGNATURE,/* Generate CMS detached signature */
		CL_ENVINFO_SIGNATURE_RESULT,	/* Signature check result */
		CL_ENVINFO_MAC,				/* Use MAC instead of encrypting */
		CL_ENVINFO_CURRENT_COMPONENT,/* Cursor management: Relative pos.*/
		CL_ENVINFO_PASSWORD,			/* User password */
		CL_ENVINFO_KEY,				/* Conventional encryption key */
		CL_ENVINFO_SIGNATURE,		/* Signature/signature check key */
		CL_ENVINFO_SIGNATURE_EXTRADATA,	/* Extra information added to CMS sigs */
		CL_ENVINFO_RECIPIENT,		/* Recipient email address */
		CL_ENVINFO_PUBLICKEY,		/* PKC encryption key */
		CL_ENVINFO_PRIVATEKEY,		/* PKC decryption key */
		CL_ENVINFO_PRIVATEKEY_LABEL,	/* Label of PKC decryption key */
		CL_ENVINFO_ORIGINATOR,		/* Originator info/key */
		CL_ENVINFO_SESSIONKEY,		/* Session key */
		CL_ENVINFO_HASH,				/* Hash value */
		CL_ENVINFO_TIMESTAMP_AUTHORITY,	/* Timestamp authority */
		CL_ENVINFO_KEYSET_SIGCHECK,	/* Signature check keyset */
		CL_ENVINFO_KEYSET_ENCRYPT,	/* PKC encryption keyset */
		CL_ENVINFO_KEYSET_DECRYPT,	/* PKC decryption keyset */
		CL_ENVINFO_LAST,
		CL_SESSINFO_FIRST,
		CL_SESSINFO_ACTIVE,			/* Whether session is active */
		CL_SESSINFO_CONNECTIONACTIVE,/* Whether network connection is active */
		CL_SESSINFO_USERNAME,		/* User name */
		CL_SESSINFO_PASSWORD,		/* Password */
		CL_SESSINFO_PRIVATEKEY,		/* Server/client private key */
		CL_SESSINFO_KEYSET,			/* Certificate store */
		CL_SESSINFO_SERVER_NAME,		/* Server name */
		CL_SESSINFO_SERVER_PORT,		/* Server port number */
		CL_SESSINFO_SERVER_FINGERPRINT,/* Server key fingerprint */
		CL_SESSINFO_CLIENT_NAME,		/* Client name */
		CL_SESSINFO_CLIENT_PORT,		/* Client port number */
		CL_SESSINFO_SESSION,			/* Transport mechanism */
		CL_SESSINFO_NETWORKSOCKET,	/* User-supplied network socket */
		CL_SESSINFO_VERSION,			/* Protocol version */
		CL_SESSINFO_REQUEST,			/* Cert.request object */
		CL_SESSINFO_RESPONSE,		/* Cert.response object */
		CL_SESSINFO_CACERTIFICATE,	/* Issuing CA certificate */
		CL_SESSINFO_TSP_MSGIMPRINT,	/* TSP message imprint */
		CL_SESSINFO_CMP_REQUESTTYPE,	/* Request type */
		CL_SESSINFO_CMP_PKIBOOT,		/* Enable PKIBoot facility */
		CL_SESSINFO_CMP_PRIVKEYSET,	/* Private-key keyset */
		CL_SESSINFO_SSH_SUBSYSTEM,	/* SSH subsystem */
		CL_SESSINFO_SSH_PORTFORWARD,	/* SSH port forwarding */
		CL_SESSINFO_LAST,
		CL_USERINFO_FIRST,
		CL_USERINFO_PASSWORD,		/* Password */
		CL_USERINFO_CAKEY_CERTSIGN,	/* CA cert signing key */
		CL_USERINFO_CAKEY_CRLSIGN,	/* CA CRL signing key */
		CL_USERINFO_CAKEY_RTCSSIGN,	/* CA RTCS signing key */
		CL_USERINFO_CAKEY_OCSPSIGN,	/* CA OCSP signing key */
		CL_USERINFO_LAST,
		CL_ATTRIBUTE_LAST
	};
	int i;
	bool encontrado;


	i = lengthof(crypt) - 1;
	encontrado = false;

	while (i >= 0 && !encontrado)
	{
		encontrado = (value == cl[i]);
		if (!encontrado)
			i--;
	}

	assert(encontrado);

	return (crypt[i]);
}





// función callback para llamar por atexit
void LiberarSingleton(void)
{
	cryptLib::freeInstancia(true);
}



cryptLib::cryptLib()
{
	hDLL = NULL;
	memset(&funciones, 0, sizeof(funciones));
}

cryptLib::~cryptLib()
{
	end();
}


cryptLib*  cryptLib::getInstancia()
{
	int status;
	
	if (instancia == NULL)
	{
		instancia = new cryptLib();

		if ( instancia->loadDLL(NOMBRE_CL32_DLL) )
		{
			// se inicia cryptLib
			status = instancia->init();
			if ( isOK(status) )
			{
				// Este singleton se liberará automaticamente cuando el proceso termine
				// Hay que hacerlo así porque el sistema cliente puede no ser consciente
				// de que está utilizando cryptLib, por lo que no hará una liberación
				// explícita.
				atexit(LiberarSingleton);
			}
		}
		else
			freeInstancia();
	}

	return (instancia);
}

bool cryptLib::freeInstancia(bool hacerEnd)
{
	int status;
	bool liberar;
	bool ret;

	if (instancia != NULL)
	{
		if (hacerEnd)
		{
			status = instancia->end();
			liberar = isOK(status);
		}
		else
			liberar = true;

		if (liberar)
		{
			instancia->freeDLL();
			
			delete instancia;
			instancia = NULL;
		}

		ret = liberar;
	}
	else
		ret = true;

	return (ret);
}


bool cryptLib::loadDLL(const ArsCadena &nombreDLL)
{
	const char* NOMBRES[] = {
		FN_INIT,
		FN_END,
		FN_DEVICE_OPEN,
		FN_DEVICE_CLOSE,
		FN_SET_ATTRIBUTE_STRING,
		FN_GET_ATTRIBUTE_STRING,
		FN_SET_ATTRIBUTE,
		FN_GET_PUBLIC_KEY,
		FN_GET_PRIVATE_KEY,
		FN_DESTROY_CERT,
		FN_IMPORT_CERT,
		FN_EXPORT_CERT,
		FN_CREATE_CONTEXT,
		FN_DESTROY_CONTEXT,
		FN_CREATE_SIGNATURE_EX,
		FN_CHECK_SIGNATURE_EX,
		FN_ENCRYPT,
		FN_DECRYPT,
		FN_CHECK_CERT
	};
	
	bool ret;
	FARPROC* funcs;
	int num;

	// precondiciones
	assert( sizeof(funciones)/sizeof(FARPROC) == lengthof(NOMBRES) );
	assert( nombreDLL != "" );

	if (hDLL != NULL)
		freeDLL();

	hDLL = ::LoadLibrary(nombreDLL.cadena());
	ret = (hDLL != NULL);
	if (ret)
	{

		funcs = new FARPROC[lengthof(NOMBRES)];

		for (int i = 0; ret && i < lengthof(NOMBRES); i++)
		{
			funcs[i] = ::GetProcAddress(hDLL, NOMBRES[i]);
			ret = (funcs[i] != NULL);
		}

		if (ret)
		{
			funciones.init		 		 = (pfnInit) 				funcs[0];
			funciones.end		 		 = (pfnEnd)		 			funcs[1];
			funciones.deviceOpen 		 = (pfnDeviceOpen) 			funcs[2];
			funciones.deviceClose 		 = (pfnDeviceClose) 		funcs[3];
			funciones.setAttributeString = (pfnSetAttributeString) 	funcs[4];
			funciones.getAttributeString = (pfnGetAttributeString) 	funcs[5];
			funciones.setAttribute 		 = (pfnSetAttribute) 		funcs[6];
			funciones.getPublicKey 		 = (pfnGetPublicKey) 		funcs[7];
			funciones.getPrivateKey 	 = (pfnGetPrivateKey) 		funcs[8];
			funciones.destroyCert 		 = (pfnDestroyCert) 		funcs[9];
			funciones.importCert 		 = (pfnImportCert) 			funcs[10];
			funciones.exportCert 		 = (pfnExportCert) 			funcs[11];
			funciones.createContext 	 = (pfnCreateContext) 		funcs[12];
			funciones.destroyContext 	 = (pfnDestroyContext) 		funcs[13];
			funciones.createSignatureEx  = (pfnCreateSignatureEx) 	funcs[14];
			funciones.checkSignatureEx 	 = (pfnCheckSignatureEx) 	funcs[15];
			funciones.encrypt 			 = (pfnEncrypt) 			funcs[16];
			funciones.decrypt 			 = (pfnDecrypt) 			funcs[17];
			funciones.checkCert 		 = (pfnCheckCert) 			funcs[18];
		}

		delete [] funcs;
	}

	return (ret);
}


bool cryptLib::freeDLL()
{
	bool ret;

	if (hDLL != NULL)
	{
		ret = (bool) ::FreeLibrary(hDLL);
		if (ret)
		{
			hDLL = NULL;
			memset(&funciones, 0, sizeof(funciones));
		}
	}
	else
		ret = true;

	return (ret);
}


ArsCadena cryptLib::getErrorString(int status)
{
	#define RETORNAR_ERROR(msg)		\
					{				\
						ret = msg;	\
						break;		\
					}

	ArsCadena	ret;

	switch (status)
	{
		case CRYPT_ERROR_LOADDLL:		RETORNAR_ERROR("La DLL no ha podido ser cargada.");

		case CRYPT_ERROR_PARAM1:		RETORNAR_ERROR("Error en el parámetro 1.");
		case CRYPT_ERROR_PARAM2:		RETORNAR_ERROR("Error en el parámetro 2.");
		case CRYPT_ERROR_PARAM3:		RETORNAR_ERROR("Error en el parámetro 3.");
		case CRYPT_ERROR_PARAM4:		RETORNAR_ERROR("Error en el parámetro 4.");
		case CRYPT_ERROR_PARAM5:		RETORNAR_ERROR("Error en el parámetro 5.");
		case CRYPT_ERROR_PARAM6:		RETORNAR_ERROR("Error en el parámetro 6.");
		case CRYPT_ERROR_PARAM7:		RETORNAR_ERROR("Error en el parámetro 7.");
		case CRYPT_ERROR_MEMORY:		RETORNAR_ERROR("No hay memoria suficiente para completar la operación.");
		case CRYPT_ERROR_NOTINITED:		RETORNAR_ERROR("Los datos no se han inicializado.");
		case CRYPT_ERROR_INITED:		RETORNAR_ERROR("Los datos ya se han inicializado.");
		case CRYPT_ERROR_NOSECURE:		RETORNAR_ERROR("La operación no está permitida porque puede comprometer la seguridad.");
		case CRYPT_ERROR_RANDOM:		RETORNAR_ERROR("No hay datos aleatorios.");
		case CRYPT_ERROR_FAILED:		RETORNAR_ERROR("La operación falló.");
		case CRYPT_ERROR_NOTAVAIL:		RETORNAR_ERROR("Opción no disponible.");
		case CRYPT_ERROR_PERMISSION:	RETORNAR_ERROR("No tiene permisos para realizar esta operación.");
		case CRYPT_ERROR_WRONGKEY:		RETORNAR_ERROR("Clave incorrecta.");
		case CRYPT_ERROR_INCOMPLETE:	RETORNAR_ERROR("Operación incompleta o en progreso.");
		case CRYPT_ERROR_COMPLETE:		RETORNAR_ERROR("Operación terminada o imposible de continuar.");
		case CRYPT_ERROR_TIMEOUT:		RETORNAR_ERROR("Se ha agotado el tiempo de espera antes de terminar la operación.");
		case CRYPT_ERROR_INVALID:		RETORNAR_ERROR("Información incorrecta o inconsistente.");
		case CRYPT_ERROR_SIGNALLED:		RETORNAR_ERROR("El recurso ha sido liberado por un evento externo.");
		case CRYPT_ERROR_OVERFLOW:		RETORNAR_ERROR("Espacio de recursos agotado.");
		case CRYPT_ERROR_UNDERFLOW:		RETORNAR_ERROR("No hay sufientes datos.");
		case CRYPT_ERROR_BADDATA:		RETORNAR_ERROR("Datos incorrecto o irreconocibles.");
		case CRYPT_ERROR_SIGNATURE:		RETORNAR_ERROR("Firma incorrecta en la verificación.");
		case CRYPT_ERROR_OPEN:			RETORNAR_ERROR("No se puede abrir el objeto.");
		case CRYPT_ERROR_READ:			RETORNAR_ERROR("No se puede leer un elemento del objeto.");
		case CRYPT_ERROR_WRITE:			RETORNAR_ERROR("No se puede escribir un elemento en el objeto.");
		case CRYPT_ERROR_NOTFOUND:		RETORNAR_ERROR("No se encuentra en el objeto el elemento solicitado.");
		case CRYPT_ERROR_DUPLICATE:		RETORNAR_ERROR("El elmento ya se encuentra en el objeto.");
		case CRYPT_ENVELOPE_RESOURCE:	RETORNAR_ERROR("Se necesita un recurso para continuar.");
		default:						RETORNAR_ERROR("Error desconocido.");
	}

	return (ret);
}

bool cryptLib::isOK(int status)
{
	return cryptStatusOK(status);
}

//
// Llamadas a cryptLib
//
int cryptLib::init()
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.init != NULL)
		ret = funciones.init();

	return (ret);
}


int cryptLib::end()
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.end != NULL)
		ret = funciones.end();

	return (ret);
}


int cryptLib::deviceOpen(CL_DEVICE* device,
				CL_USER cryptUser,
				CL_DEVICE_TYPE deviceType,
				char* name)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.deviceOpen != NULL)
	{
		CRYPT_DEVICE *_device;
		CRYPT_DEVICE _deviceValue;
		CRYPT_USER _cryptUser;
		CRYPT_DEVICE_TYPE _deviceType;

		if (device != NULL)
		{
			_deviceValue = *device;
			_device = &_deviceValue;
		}
		else
			_device = NULL;

		_cryptUser = cryptUser;
		_deviceType = ConvertirEnumerado(deviceType);

		ret = funciones.deviceOpen(_device, _cryptUser, _deviceType, name);

		if (device != NULL)
		{
			*device = _deviceValue;
		}
	}

	return (ret);
}


int cryptLib::deviceClose(CL_DEVICE device)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.deviceClose != NULL)
	{
		CRYPT_DEVICE _device;

		_device = device;

		ret = funciones.deviceClose(_device);
	}

	return (ret);
}


int cryptLib::setAttributeString(CL_HANDLE cryptHandle,
							CL_ATTRIBUTE_TYPE attributeType,
							void* value, int valueLength)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.setAttributeString != NULL)
	{
		CRYPT_HANDLE _cryptHandle;
		CRYPT_ATTRIBUTE_TYPE _attributeType;

		_cryptHandle = cryptHandle;
		_attributeType = ConvertirEnumerado(attributeType);

		ret = funciones.setAttributeString(_cryptHandle, _attributeType, value, valueLength);
	}

	return (ret);
}


int cryptLib::getAttributeString(CL_HANDLE cryptHandle,
								CL_ATTRIBUTE_TYPE attributeType,
								void* value,
								int*  valueLength)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.getAttributeString != NULL)
	{
		CRYPT_HANDLE	_cryptHandle;
		CRYPT_ATTRIBUTE_TYPE _attributeType;

		_cryptHandle = cryptHandle;
		_attributeType = ConvertirEnumerado(attributeType);

		ret = funciones.getAttributeString(_cryptHandle, _attributeType, value, valueLength);
	}

	return (ret);
}


int cryptLib::setAttribute(CL_HANDLE cryptHandle,
						CL_ATTRIBUTE_TYPE attributeType,
						int value)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.setAttribute != NULL)
	{
		CRYPT_HANDLE	_cryptHandle;
		CRYPT_ATTRIBUTE_TYPE _attributeType;

		_cryptHandle = cryptHandle;
		_attributeType = ConvertirEnumerado(attributeType);

		ret = funciones.setAttribute(_cryptHandle, _attributeType, value);
	}

	return (ret);
}


int cryptLib::getPublicKey(CL_KEYSET keyset,
						CL_CONTEXT* cryptContext,
						CL_KEYID_TYPE keyIDtype,
						char* keyID)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.getPublicKey != NULL)
	{
		CRYPT_KEYSET	_keyset;
		CRYPT_CONTEXT	*_cryptContext;
		CRYPT_CONTEXT	_cryptContextValue;
		CRYPT_KEYID_TYPE	_keyIDtype;

		if (cryptContext != NULL)
		{
			_cryptContextValue = *cryptContext;
			_cryptContext = &_cryptContextValue;
		}
		else
			_cryptContext = NULL;

		_keyset = keyset;
		_keyIDtype = ConvertirEnumerado(keyIDtype);

		ret = funciones.getPublicKey(_keyset, _cryptContext, _keyIDtype, keyID);

		if (cryptContext != NULL)
		{
			*cryptContext = _cryptContextValue;
		}
	}

	return (ret);
}


int cryptLib::getPrivateKey(CL_KEYSET keyset,
				CL_CONTEXT* cryptContext,
				CL_KEYID_TYPE keyIDtype,
				char* keyID,
				char* password)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.getPrivateKey != NULL)
	{
		CRYPT_KEYSET	_keyset;
		CRYPT_CONTEXT	*_cryptContext;
		CRYPT_CONTEXT	_cryptContextValue;
		CRYPT_KEYID_TYPE	_keyIDtype;

		if (cryptContext != NULL)
		{
			_cryptContextValue = *cryptContext;
			_cryptContext = &_cryptContextValue;
		}
		else
			_cryptContext = NULL;

		_keyset = keyset;
		_keyIDtype = ConvertirEnumerado(keyIDtype);

		ret = funciones.getPrivateKey(_keyset, _cryptContext, _keyIDtype, keyID, password);

		if (cryptContext != NULL)
			*cryptContext = _cryptContextValue;
	}

	return (ret);
}


int cryptLib::destroyCert(CL_CERTIFICATE certificate)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.destroyCert != NULL)
	{
		CRYPT_CERTIFICATE _certificate;

		_certificate = certificate;
		
		ret = funciones.destroyCert(_certificate);
	}

	return (ret);
}


int cryptLib::importCert(void* certObject,
						int certObjectLength,
						CL_USER cryptUser,
						CL_CERTIFICATE* certificate)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.importCert != NULL)
	{
		CRYPT_USER _cryptUser;
		CRYPT_CERTIFICATE *_certificate;
		CRYPT_CERTIFICATE _certificateValue;

		_cryptUser = cryptUser;
		if (certificate != NULL)
		{
			_certificateValue = *certificate;
			_certificate = &_certificateValue;
		}
		else
			_certificate = NULL;

		ret = funciones.importCert(certObject, certObjectLength, _cryptUser, _certificate);

		if (certificate != NULL)
		{
			*certificate = _certificateValue;
		}
	}

	return (ret);
}


int cryptLib::exportCert(void* certObject,
						int* certObjectLength,
						CL_CERTFORMAT_TYPE certFormatType,
						CL_CERTIFICATE certificate)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.exportCert != NULL)
	{
		CRYPT_CERTFORMAT_TYPE _certFormatType;
		CRYPT_CERTIFICATE	_certificate;

		_certFormatType = ConvertirEnumerado(certFormatType);
		_certificate = certificate;

		ret = funciones.exportCert(certObject, certObjectLength, _certFormatType, _certificate);
	}

	return (ret);
}


int cryptLib::createContext(CL_CONTEXT *cryptContext,
							CL_USER cryptUser,
							CL_ALGO_TYPE cryptAlgo)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.createContext != NULL)
	{
		CRYPT_CONTEXT	*_cryptContext;
		CRYPT_CONTEXT	_cryptContextValue;
		CRYPT_USER		_cryptUser;
		CRYPT_ALGO_TYPE	_cryptAlgo;

		if (cryptContext != NULL)
		{
			_cryptContextValue = *cryptContext;
			_cryptContext = &_cryptContextValue;
		}
		else
			_cryptContext = NULL;

		_cryptUser = cryptUser;
		_cryptAlgo = ConvertirEnumerado(cryptAlgo);

		ret = funciones.createContext(_cryptContext, _cryptUser, _cryptAlgo);

		if (cryptContext != NULL)
		{
			*cryptContext = _cryptContextValue;
		}
	}

	return (ret);
}


int cryptLib::destroyContext(CL_CONTEXT cryptContext)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.destroyContext != NULL)
	{
		CRYPT_CONTEXT _cryptContext;

		_cryptContext = cryptContext;

		ret = funciones.destroyContext(_cryptContext);
	}

	return (ret);
}


int cryptLib::createSignatureEx(void* signature,
								int* signatureLength,
								CL_FORMAT_TYPE formatType,
								CL_CONTEXT signContext,
								CL_CONTEXT hashContext,
								CL_CERTIFICATE extraData)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.createSignatureEx != NULL)
	{
		CRYPT_FORMAT_TYPE _formatType;
		CRYPT_CONTEXT _signContext;
		CRYPT_CONTEXT _hashContext;
		CRYPT_CERTIFICATE _extraData;

		_formatType = ConvertirEnumerado(formatType);
		_signContext = signContext;
		_hashContext = hashContext;
		_extraData = extraData;

		ret = funciones.createSignatureEx(signature, signatureLength, _formatType, _signContext, _hashContext, _extraData);
	}

	return (ret);
}


int cryptLib::checkSignatureEx(	void* signature,
								CL_HANDLE sigCheckKey,
								CL_CONTEXT hashContext,
								CL_HANDLE* extraData)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.checkSignatureEx != NULL)
	{
		CRYPT_HANDLE _sigCheckKey;
		CRYPT_CONTEXT _hashContext;
		CRYPT_HANDLE *_extraData;
		CRYPT_HANDLE _extraDataValue;

		if (extraData != NULL)
		{
			_extraDataValue = *extraData;
			_extraData = &_extraDataValue;
		}
		else
			_extraData = NULL;

		_sigCheckKey = sigCheckKey;
		_hashContext = hashContext;

		ret = funciones.checkSignatureEx(signature, _sigCheckKey, _hashContext, _extraData);

		if (extraData != NULL)
		{
			*extraData = _extraDataValue;
		}
	}

	return (ret);
}


int cryptLib::encrypt(CL_CONTEXT cryptContext, void* buffer, int length)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.encrypt != NULL)
	{
		CRYPT_CONTEXT _cryptContext;

		_cryptContext = cryptContext;

		ret = funciones.encrypt(_cryptContext, buffer, length);
	}

	return (ret);
}


int cryptLib::decrypt(CL_CONTEXT cryptContext, void* buffer, int length)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.decrypt != NULL)
	{
		CRYPT_CONTEXT _cryptContext;

		_cryptContext = cryptContext;

		ret = funciones.decrypt(_cryptContext, buffer, length);
	}

	return (ret);
}


int cryptLib::checkCert(CL_CERTIFICATE certificate, CL_HANDLE sigCheckKey)
{
	int ret = CRYPT_ERROR_LOADDLL;

	if (funciones.checkCert != NULL)
	{
		CRYPT_CERTIFICATE _certificate;
		CRYPT_HANDLE _sigCheckKey;

		_certificate = certificate;
		_sigCheckKey = sigCheckKey;

		ret = funciones.checkCert(_certificate, _sigCheckKey);
	}

	return (ret);
}

