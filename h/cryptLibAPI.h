#if !defined(_CRYPT_LIB_API_H_)
#define _CRYPT_LIB_API_H_

#include <pshpack8.h>
	#include "../../cryptLib/cryptLib.h"
#include <poppack.h>

#include "../../LibUtils/h/ArsCadena.h"

#include <windows.h>


// Errores que pueden retornar las funciones de CL (además de los propios de "cryptLib.h")
#define CRYPT_ERROR_LOADDLL		-999


// Para abreviar la llamada al singleton
#define CL	cryptLib::getInstancia()


//
// Redefinición de los tipos que utiliza cryptLib
//
typedef int CL_DEVICE;
typedef int CL_USER;
typedef int CL_HANDLE;
typedef int CL_KEYSET;
typedef int CL_CONTEXT;
typedef int CL_CERTIFICATE;

typedef enum
{
	CL_DEVICE_NONE,				/* No crypto device */
	CL_DEVICE_FORTEZZA,			/* Fortezza card */
	CL_DEVICE_PKCS11,			/* PKCS #11 crypto token */
	CL_DEVICE_CRYPTOAPI,			/* Microsoft CryptoAPI */
	CL_DEVICE_LAST				/* Last possible crypto device type */
} CL_DEVICE_TYPE;



typedef enum
{
	CL_ATTRIBUTE_NONE,			/* Non-value */

	/* Used internally */
	CL_PROPERTY_FIRST,

	/*********************/
	/* Object attributes */
	/*********************/

	/* Object properties */
	CL_PROPERTY_HIGHSECURITY,	/* Owned+non-forwardcount+locked */
	CL_PROPERTY_OWNER,			/* Object owner */
	CL_PROPERTY_FORWARDCOUNT,	/* No.of times object can be forwarded */
	CL_PROPERTY_LOCKED,			/* Whether properties can be chged/read */
	CL_PROPERTY_USAGECOUNT,		/* Usage count before object expires */
	CL_PROPERTY_NONEXPORTABLE,	/* Whether key is nonexp.from context */

	/* Used internally */
	CL_PROPERTY_LAST, CL_GENERIC_FIRST,

	/* Extended error information */
	CL_ATTRIBUTE_ERRORTYPE,		/* Type of last error */
	CL_ATTRIBUTE_ERRORLOCUS,		/* Locus of last error */
	CL_ATTRIBUTE_INT_ERRORCODE,	/* Low-level software-specific */
	CL_ATTRIBUTE_INT_ERRORMESSAGE, /*   error code and message */

	/* Generic information */
	CL_ATTRIBUTE_BUFFERSIZE,		/* Internal data buffer size */

	/* User internally */
	CL_GENERIC_LAST, CL_OPTION_FIRST = 100,

	/****************************/
	/* Configuration attributes */
	/****************************/

	/* cryptlib information (read-only) */
	CL_OPTION_INFO_DESCRIPTION,	/* Text description */
	CL_OPTION_INFO_COPYRIGHT,	/* Copyright notice */
	CL_OPTION_INFO_MAJORVERSION,	/* Major release version */
	CL_OPTION_INFO_MINORVERSION,	/* Minor release version */
	CL_OPTION_INFO_STEPPING,		/* Release stepping */

	/* Encryption options */
	CL_OPTION_ENCR_ALGO,			/* Encryption algorithm */
	CL_OPTION_ENCR_HASH,			/* Hash algorithm */
	CL_OPTION_ENCR_MAC,			/* MAC algorithm */

	/* PKC options */
	CL_OPTION_PKC_ALGO,			/* Public-key encryption algorithm */
	CL_OPTION_PKC_KEYSIZE,		/* Public-key encryption key size */

	/* Signature options */
	CL_OPTION_SIG_ALGO,			/* Signature algorithm */
	CL_OPTION_SIG_KEYSIZE,		/* Signature keysize */

	/* Keying options */
	CL_OPTION_KEYING_ALGO,		/* Key processing algorithm */
	CL_OPTION_KEYING_ITERATIONS,	/* Key processing iterations */

	/* Certificate options */
	CL_OPTION_CERT_SIGNUNRECOGNISEDATTRIBUTES,	/* Whether to sign unrecog.attrs */
	CL_OPTION_CERT_VALIDITY,		/* Certificate validity period */
	CL_OPTION_CERT_UPDATEINTERVAL,	/* CRL update interval */
	CL_OPTION_CERT_COMPLIANCELEVEL,	/* PKIX compliance level for cert chks.*/

	/* CMS/SMIME options */
	CL_OPTION_CMS_DEFAULTATTRIBUTES,	/* Add default CMS attributes */
		CL_OPTION_SMIME_DEFAULTATTRIBUTES = CL_OPTION_CMS_DEFAULTATTRIBUTES,

	/* LDAP keyset options */
	CL_OPTION_KEYS_LDAP_OBJECTCLASS,	/* Object class */
	CL_OPTION_KEYS_LDAP_OBJECTTYPE,	/* Object type to fetch */
	CL_OPTION_KEYS_LDAP_FILTER,		/* Query filter */
	CL_OPTION_KEYS_LDAP_CACERTNAME,	/* CA certificate attribute name */
	CL_OPTION_KEYS_LDAP_CERTNAME,	/* Certificate attribute name */
	CL_OPTION_KEYS_LDAP_CRLNAME,		/* CRL attribute name */
	CL_OPTION_KEYS_LDAP_EMAILNAME,	/* Email attribute name */

	/* Crypto device options */
	CL_OPTION_DEVICE_PKCS11_DVR01,	/* Name of first PKCS #11 driver */
	CL_OPTION_DEVICE_PKCS11_DVR02,	/* Name of second PKCS #11 driver */
	CL_OPTION_DEVICE_PKCS11_DVR03,	/* Name of third PKCS #11 driver */
	CL_OPTION_DEVICE_PKCS11_DVR04,	/* Name of fourth PKCS #11 driver */
	CL_OPTION_DEVICE_PKCS11_DVR05,	/* Name of fifth PKCS #11 driver */
	CL_OPTION_DEVICE_PKCS11_HARDWAREONLY,/* Use only hardware mechanisms */

	/* Network access options */
	CL_OPTION_NET_SOCKS_SERVER,		/* Socks server name */
	CL_OPTION_NET_SOCKS_USERNAME,	/* Socks user name */
	CL_OPTION_NET_HTTP_PROXY,		/* Web proxy server */
	CL_OPTION_NET_CONNECTTIMEOUT,	/* Timeout for network connection setup */
	CL_OPTION_NET_TIMEOUT,			/* Timeout for network read/write */

	/* Miscellaneous options */
	CL_OPTION_MISC_ASYNCINIT,	/* Whether to init cryptlib async'ly */
	CL_OPTION_MISC_SIDECHANNELPROTECTION, /* Protect against side-channel attacks */

	/* cryptlib state information */
	CL_OPTION_CONFIGCHANGED,		/* Whether in-mem.opts match on-disk ones */
	CL_OPTION_SELFTESTOK,		/* Whether self-test was completed and OK */

	/* Used internally */
	CL_OPTION_LAST, CL_CTXINFO_FIRST = 1000,

	/**********************/
	/* Context attributes */
	/**********************/

	/* Algorithm and mode information */
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

	/* State information */
	CL_CTXINFO_KEY,				/* Key */
	CL_CTXINFO_KEY_COMPONENTS,	/* Public-key components */
	CL_CTXINFO_IV,				/* IV */
	CL_CTXINFO_HASHVALUE,		/* Hash value */

	/* Misc.information */
	CL_CTXINFO_LABEL,			/* Label for private/secret key */

	/* Used internally */
	CL_CTXINFO_LAST, CL_CERTINFO_FIRST = 2000,

	/**************************/
	/* Certificate attributes */
	/**************************/

	/* Because there are so many cert attributes, we break them down into 
	   blocks to minimise the number of values that change if a new one is 
	   added halfway through */

	/* Pseudo-information on a cert object or meta-information which is used
	   to control the way that a cert object is processed */
	CL_CERTINFO_SELFSIGNED,		/* Cert is self-signed */
	CL_CERTINFO_IMMUTABLE,		/* Cert is signed and immutable */
	CL_CERTINFO_XYZZY,			/* Cert is a magic just-works cert */
	CL_CERTINFO_CERTTYPE,		/* Certificate object type */
	CL_CERTINFO_FINGERPRINT,		/* Certificate fingerprints */
		CL_CERTINFO_FINGERPRINT_MD5 = CL_CERTINFO_FINGERPRINT,
	CL_CERTINFO_FINGERPRINT_SHA,
	CL_CERTINFO_CURRENT_CERTIFICATE,/* Cursor mgt: Rel.pos in chain/CRL/OCSP */
	CL_CERTINFO_CURRENT_EXTENSION,/* Cursor mgt: Rel.pos.or abs.extension */
	CL_CERTINFO_CURRENT_FIELD,	/* Cursor mgt: Rel.pos.or abs.field in ext */
	CL_CERTINFO_CURRENT_COMPONENT,/* Cursor mgt: Rel.pos in multival.field */
	CL_CERTINFO_TRUSTED_USAGE,	/* Usage that cert is trusted for */
	CL_CERTINFO_TRUSTED_IMPLICIT,/* Whether cert is implicitly trusted */
	CL_CERTINFO_SIGNATURELEVEL,	/* Amount of detail to include in sigs.*/

	/* General certificate object information */
	CL_CERTINFO_VERSION,			/* Cert.format version */
	CL_CERTINFO_SERIALNUMBER,	/* Serial number */
	CL_CERTINFO_SUBJECTPUBLICKEYINFO,	/* Public key */
	CL_CERTINFO_CERTIFICATE,		/* User certificate */
		CL_CERTINFO_USERCERTIFICATE = CL_CERTINFO_CERTIFICATE,
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

	/* X.520 Distinguished Name components.  This is a composite field, the
	   DN to be manipulated is selected through the addition of a
	   pseudocomponent, and then one of the following is used to access the
	   DN components directly */
	CL_CERTINFO_COUNTRYNAME = CL_CERTINFO_FIRST + 100,	/* countryName */
	CL_CERTINFO_STATEORPROVINCENAME,	/* stateOrProvinceName */
	CL_CERTINFO_LOCALITYNAME,		/* localityName */
	CL_CERTINFO_ORGANIZATIONNAME,	/* organizationName */
		CL_CERTINFO_ORGANISATIONNAME = CL_CERTINFO_ORGANIZATIONNAME,
	CL_CERTINFO_ORGANIZATIONALUNITNAME,	/* organizationalUnitName */
		CL_CERTINFO_ORGANISATIONALUNITNAME = CL_CERTINFO_ORGANIZATIONALUNITNAME,
	CL_CERTINFO_COMMONNAME,		/* commonName */

	/* X.509 General Name components.  These are handled in the same way as
	   the DN composite field, with the current GeneralName being selected by
	   a pseudo-component after which the individual components can be
	   modified through one of the following */
	CL_CERTINFO_OTHERNAME_TYPEID,		/* otherName.typeID */
	CL_CERTINFO_OTHERNAME_VALUE,			/* otherName.value */
	CL_CERTINFO_RFC822NAME,				/* rfc822Name */
		CL_CERTINFO_EMAIL = CL_CERTINFO_RFC822NAME,
	CL_CERTINFO_DNSNAME,					/* dNSName */
#if 0	/* Not supported yet, these are never used in practice and have an
		   insane internal structure */
	CL_CERTINFO_X400ADDRESS,				/* x400Address */
#endif /* 0 */
	CL_CERTINFO_DIRECTORYNAME,			/* directoryName */
	CL_CERTINFO_EDIPARTYNAME_NAMEASSIGNER,	/* ediPartyName.nameAssigner */
	CL_CERTINFO_EDIPARTYNAME_PARTYNAME,	/* ediPartyName.partyName */
	CL_CERTINFO_UNIFORMRESOURCEIDENTIFIER,	/* uniformResourceIdentifier */
	CL_CERTINFO_IPADDRESS,				/* iPAddress */
	CL_CERTINFO_REGISTEREDID,			/* registeredID */

	/* X.509 certificate extensions.  Although it would be nicer to use names
	   that match the extensions more closely (e.g.
	   CL_CERTINFO_BASICCONSTRAINTS_PATHLENCONSTRAINT), these exceed the
	   32-character ANSI minimum length for unique names, and get really
	   hairy once you get into the weird policy constraints extensions whose
	   names wrap around the screen about three times.

	   The following values are defined in OID order, this isn't absolutely
	   necessary but saves an extra layer of processing when encoding them */

	/* 1 2 840 113549 1 9 7 challengePassword.  This is here even though it's
	   a CMS attribute because SCEP stuffs it into PKCS #10 requests */
	CL_CERTINFO_CHALLENGEPASSWORD = CL_CERTINFO_FIRST + 200,

	/* 1 3 6 1 4 1 3029 3 1 4 cRLExtReason */
	CL_CERTINFO_CRLEXTREASON,

	/* 1 3 6 1 4 1 3029 3 1 5 keyFeatures */
	CL_CERTINFO_KEYFEATURES,

	/* 1 3 6 1 5 5 7 1 1 authorityInfoAccess */
	CL_CERTINFO_AUTHORITYINFOACCESS,
	CL_CERTINFO_AUTHORITYINFO_RTCS,		/* accessDescription.accessLocation */
	CL_CERTINFO_AUTHORITYINFO_OCSP,		/* accessDescription.accessLocation */
	CL_CERTINFO_AUTHORITYINFO_CAISSUERS,	/* accessDescription.accessLocation */
	CL_CERTINFO_AUTHORITYINFO_TIMESTAMPING,/* accessDescription.accessLocation */

	/* 1 3 6 1 5 5 7 48 1 2 ocspNonce */
	CL_CERTINFO_OCSP_NONCE,				/* nonce */

	/* 1 3 6 1 5 5 7 48 1 4 ocspAcceptableResponses */
	CL_CERTINFO_OCSP_RESPONSE,
	CL_CERTINFO_OCSP_RESPONSE_OCSP,		/* OCSP standard response */

	/* 1 3 6 1 5 5 7 48 1 5 ocspNoCheck */
	CL_CERTINFO_OCSP_NOCHECK,

	/* 1 3 6 1 5 5 7 48 1 6 ocspArchiveCutoff */
	CL_CERTINFO_OCSP_ARCHIVECUTOFF,

	/* 1 3 6 1 5 5 7 48 1 11 subjectInfoAccess */
	CL_CERTINFO_SUBJECTINFOACCESS,
	CL_CERTINFO_SUBJECTINFO_CAREPOSITORY,/* accessDescription.accessLocation */
	CL_CERTINFO_SUBJECTINFO_TIMESTAMPING,/* accessDescription.accessLocation */

	/* 1 3 36 8 3 1 siggDateOfCertGen */
	CL_CERTINFO_SIGG_DATEOFCERTGEN,

	/* 1 3 36 8 3 2 siggProcuration */
	CL_CERTINFO_SIGG_PROCURATION,
	CL_CERTINFO_SIGG_PROCURE_COUNTRY,	/* country */
	CL_CERTINFO_SIGG_PROCURE_TYPEOFSUBSTITUTION,	/* typeOfSubstitution */
	CL_CERTINFO_SIGG_PROCURE_SIGNINGFOR,	/* signingFor.thirdPerson */

	/* 1 3 36 8 3 4 siggMonetaryLimit */
	CL_CERTINFO_SIGG_MONETARYLIMIT,
	CL_CERTINFO_SIGG_MONETARY_CURRENCY,	/* currency */
	CL_CERTINFO_SIGG_MONETARY_AMOUNT,	/* amount */
	CL_CERTINFO_SIGG_MONETARY_EXPONENT,	/* exponent */

	/* 1 3 36 8 3 8 siggRestriction */
	CL_CERTINFO_SIGG_RESTRICTION,

	/* 1 3 101 1 4 1 strongExtranet */
	CL_CERTINFO_STRONGEXTRANET,
	CL_CERTINFO_STRONGEXTRANET_ZONE,		/* sxNetIDList.sxNetID.zone */
	CL_CERTINFO_STRONGEXTRANET_ID,		/* sxNetIDList.sxNetID.id */

	/* 2 5 29 9 subjectDirectoryAttributes */
	CL_CERTINFO_SUBJECTDIRECTORYATTRIBUTES,
	CL_CERTINFO_SUBJECTDIR_TYPE,			/* attribute.type */
	CL_CERTINFO_SUBJECTDIR_VALUES,		/* attribute.values */

	/* 2 5 29 14 subjectKeyIdentifier */
	CL_CERTINFO_SUBJECTKEYIDENTIFIER,

	/* 2 5 29 15 keyUsage */
	CL_CERTINFO_KEYUSAGE,

	/* 2 5 29 16 privateKeyUsagePeriod */
	CL_CERTINFO_PRIVATEKEYUSAGEPERIOD,
	CL_CERTINFO_PRIVATEKEY_NOTBEFORE,	/* notBefore */
	CL_CERTINFO_PRIVATEKEY_NOTAFTER,		/* notAfter */

	/* 2 5 29 17 subjectAltName */
	CL_CERTINFO_SUBJECTALTNAME,

	/* 2 5 29 18 issuerAltName */
	CL_CERTINFO_ISSUERALTNAME,

	/* 2 5 29 19 basicConstraints */
	CL_CERTINFO_BASICCONSTRAINTS,
	CL_CERTINFO_CA,						/* cA */
		CL_CERTINFO_AUTHORITY = CL_CERTINFO_CA,
	CL_CERTINFO_PATHLENCONSTRAINT,		/* pathLenConstraint */

	/* 2 5 29 20 cRLNumber */
	CL_CERTINFO_CRLNUMBER,

	/* 2 5 29 21 cRLReason */
	CL_CERTINFO_CRLREASON,

	/* 2 5 29 23 holdInstructionCode */
	CL_CERTINFO_HOLDINSTRUCTIONCODE,

	/* 2 5 29 24 invalidityDate */
	CL_CERTINFO_INVALIDITYDATE,

	/* 2 5 29 27 deltaCRLIndicator */
	CL_CERTINFO_DELTACRLINDICATOR,

	/* 2 5 29 28 issuingDistributionPoint */
	CL_CERTINFO_ISSUINGDISTRIBUTIONPOINT,
	CL_CERTINFO_ISSUINGDIST_FULLNAME,	/* distributionPointName.fullName */
	CL_CERTINFO_ISSUINGDIST_USERCERTSONLY,	/* onlyContainsUserCerts */
	CL_CERTINFO_ISSUINGDIST_CACERTSONLY,	/* onlyContainsCACerts */
	CL_CERTINFO_ISSUINGDIST_SOMEREASONSONLY,	/* onlySomeReasons */
	CL_CERTINFO_ISSUINGDIST_INDIRECTCRL,	/* indirectCRL */

	/* 2 5 29 29 certificateIssuer */
	CL_CERTINFO_CERTIFICATEISSUER,

	/* 2 5 29 30 nameConstraints */
	CL_CERTINFO_NAMECONSTRAINTS,
	CL_CERTINFO_PERMITTEDSUBTREES,		/* permittedSubtrees */
	CL_CERTINFO_EXCLUDEDSUBTREES,		/* excludedSubtrees */

	/* 2 5 29 31 cRLDistributionPoint */
	CL_CERTINFO_CRLDISTRIBUTIONPOINT,
	CL_CERTINFO_CRLDIST_FULLNAME,		/* distributionPointName.fullName */
	CL_CERTINFO_CRLDIST_REASONS,			/* reasons */
	CL_CERTINFO_CRLDIST_CRLISSUER,		/* cRLIssuer */

	/* 2 5 29 32 certificatePolicies */
	CL_CERTINFO_CERTIFICATEPOLICIES,
	CL_CERTINFO_CERTPOLICYID,		/* policyInformation.policyIdentifier */
	CL_CERTINFO_CERTPOLICY_CPSURI,
		/* policyInformation.policyQualifiers.qualifier.cPSuri */
	CL_CERTINFO_CERTPOLICY_ORGANIZATION,
		/* policyInformation.policyQualifiers.qualifier.userNotice.noticeRef.organization */
	CL_CERTINFO_CERTPOLICY_NOTICENUMBERS,
		/* policyInformation.policyQualifiers.qualifier.userNotice.noticeRef.noticeNumbers */
	CL_CERTINFO_CERTPOLICY_EXPLICITTEXT,
		/* policyInformation.policyQualifiers.qualifier.userNotice.explicitText */

	/* 2 5 29 33 policyMappings */
	CL_CERTINFO_POLICYMAPPINGS,
	CL_CERTINFO_ISSUERDOMAINPOLICY,	/* policyMappings.issuerDomainPolicy */
	CL_CERTINFO_SUBJECTDOMAINPOLICY,	/* policyMappings.subjectDomainPolicy */

	/* 2 5 29 35 authorityKeyIdentifier */
	CL_CERTINFO_AUTHORITYKEYIDENTIFIER,
	CL_CERTINFO_AUTHORITY_KEYIDENTIFIER,	/* keyIdentifier */
	CL_CERTINFO_AUTHORITY_CERTISSUER,	/* authorityCertIssuer */
	CL_CERTINFO_AUTHORITY_CERTSERIALNUMBER,	/* authorityCertSerialNumber */

	/* 2 5 29 36 policyConstraints */
	CL_CERTINFO_POLICYCONSTRAINTS,
	CL_CERTINFO_REQUIREEXPLICITPOLICY,	/* policyConstraints.requireExplicitPolicy */
	CL_CERTINFO_INHIBITPOLICYMAPPING,	/* policyConstraints.inhibitPolicyMapping */

	/* 2 5 29 37 extKeyUsage */
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

	/* 2 5 29 46 freshestCRL */
	CL_CERTINFO_FRESHESTCRL,
	CL_CERTINFO_FRESHESTCRL_FULLNAME,	/* distributionPointName.fullName */
	CL_CERTINFO_FRESHESTCRL_REASONS,		/* reasons */
	CL_CERTINFO_FRESHESTCRL_CRLISSUER,	/* cRLIssuer */

	/* 2 5 29 54 inhibitAnyPolicy */
	CL_CERTINFO_INHIBITANYPOLICY,

	/* 2 16 840 1 113730 1 x Netscape extensions */
	CL_CERTINFO_NS_CERTTYPE,				/* netscape-cert-type */
	CL_CERTINFO_NS_BASEURL,				/* netscape-base-url */
	CL_CERTINFO_NS_REVOCATIONURL,		/* netscape-revocation-url */
	CL_CERTINFO_NS_CAREVOCATIONURL,		/* netscape-ca-revocation-url */
	CL_CERTINFO_NS_CERTRENEWALURL,		/* netscape-cert-renewal-url */
	CL_CERTINFO_NS_CAPOLICYURL,			/* netscape-ca-policy-url */
	CL_CERTINFO_NS_SSLSERVERNAME,		/* netscape-ssl-server-name */
	CL_CERTINFO_NS_COMMENT,				/* netscape-comment */

	/* 2 23 42 7 0 SET hashedRootKey */
	CL_CERTINFO_SET_HASHEDROOTKEY,
	CL_CERTINFO_SET_ROOTKEYTHUMBPRINT,	/* rootKeyThumbPrint */

	/* 2 23 42 7 1 SET certificateType */
	CL_CERTINFO_SET_CERTIFICATETYPE,

	/* 2 23 42 7 2 SET merchantData */
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

	/* 2 23 42 7 3 SET certCardRequired */
	CL_CERTINFO_SET_CERTCARDREQUIRED,

	/* 2 23 42 7 4 SET tunneling */
	CL_CERTINFO_SET_TUNNELING,
		CL_CERTINFO_SET_TUNNELLING = CL_CERTINFO_SET_TUNNELING,
	CL_CERTINFO_SET_TUNNELINGFLAG,		/* tunneling */
		CL_CERTINFO_SET_TUNNELLINGFLAG = CL_CERTINFO_SET_TUNNELINGFLAG,
	CL_CERTINFO_SET_TUNNELINGALGID,		/* tunnelingAlgID */
		CL_CERTINFO_SET_TUNNELLINGALGID = CL_CERTINFO_SET_TUNNELINGALGID,

	/* S/MIME attributes */

	/* 1 2 840 113549 1 9 3 contentType */
	CL_CERTINFO_CMS_CONTENTTYPE = CL_CERTINFO_FIRST + 500,

	/* 1 2 840 113549 1 9 4 messageDigest */
	CL_CERTINFO_CMS_MESSAGEDIGEST,

	/* 1 2 840 113549 1 9 5 signingTime */
	CL_CERTINFO_CMS_SIGNINGTIME,

	/* 1 2 840 113549 1 9 6 counterSignature */
	CL_CERTINFO_CMS_COUNTERSIGNATURE,	/* counterSignature */

	/* 1 2 840 113549 1 9 15 sMIMECapabilities */
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

	/* 1 2 840 113549 1 9 16 2 1 receiptRequest */
	CL_CERTINFO_CMS_RECEIPTREQUEST,
	CL_CERTINFO_CMS_RECEIPT_CONTENTIDENTIFIER, /* contentIdentifier */
	CL_CERTINFO_CMS_RECEIPT_FROM,		/* receiptsFrom */
	CL_CERTINFO_CMS_RECEIPT_TO,			/* receiptsTo */

	/* 1 2 840 113549 1 9 16 2 2 essSecurityLabel */
	CL_CERTINFO_CMS_SECURITYLABEL,
	CL_CERTINFO_CMS_SECLABEL_CLASSIFICATION, /* securityClassification */
	CL_CERTINFO_CMS_SECLABEL_POLICY,		/* securityPolicyIdentifier */
	CL_CERTINFO_CMS_SECLABEL_PRIVACYMARK,/* privacyMark */
	CL_CERTINFO_CMS_SECLABEL_CATTYPE,	/* securityCategories.securityCategory.type */
	CL_CERTINFO_CMS_SECLABEL_CATVALUE,	/* securityCategories.securityCategory.value */

	/* 1 2 840 113549 1 9 16 2 3 mlExpansionHistory */
	CL_CERTINFO_CMS_MLEXPANSIONHISTORY,
	CL_CERTINFO_CMS_MLEXP_ENTITYIDENTIFIER, /* mlData.mailListIdentifier.issuerAndSerialNumber */
	CL_CERTINFO_CMS_MLEXP_TIME,			/* mlData.expansionTime */
	CL_CERTINFO_CMS_MLEXP_NONE,			/* mlData.mlReceiptPolicy.none */
	CL_CERTINFO_CMS_MLEXP_INSTEADOF,		/* mlData.mlReceiptPolicy.insteadOf.generalNames.generalName */
	CL_CERTINFO_CMS_MLEXP_INADDITIONTO,	/* mlData.mlReceiptPolicy.inAdditionTo.generalNames.generalName */

	/* 1 2 840 113549 1 9 16 2 4 contentHints */
	CL_CERTINFO_CMS_CONTENTHINTS,
	CL_CERTINFO_CMS_CONTENTHINT_DESCRIPTION,	/* contentDescription */
	CL_CERTINFO_CMS_CONTENTHINT_TYPE,	/* contentType */

	/* 1 2 840 113549 1 9 16 2 9 equivalentLabels */
	CL_CERTINFO_CMS_EQUIVALENTLABEL,
	CL_CERTINFO_CMS_EQVLABEL_POLICY,		/* securityPolicyIdentifier */
	CL_CERTINFO_CMS_EQVLABEL_CLASSIFICATION, /* securityClassification */
	CL_CERTINFO_CMS_EQVLABEL_PRIVACYMARK,/* privacyMark */
	CL_CERTINFO_CMS_EQVLABEL_CATTYPE,	/* securityCategories.securityCategory.type */
	CL_CERTINFO_CMS_EQVLABEL_CATVALUE,	/* securityCategories.securityCategory.value */

	/* 1 2 840 113549 1 9 16 2 12 signingCertificate */
	CL_CERTINFO_CMS_SIGNINGCERTIFICATE,
	CL_CERTINFO_CMS_SIGNINGCERT_ESSCERTID, /* certs.essCertID */
	CL_CERTINFO_CMS_SIGNINGCERT_POLICIES,/* policies.policyInformation.policyIdentifier */

	/* 1 2 840 113549 1 9 16 9 signatureTypeIdentifier */
	CL_CERTINFO_CMS_SIGTYPEIDENTIFIER,
	CL_CERTINFO_CMS_SIGTYPEID_ORIGINATORSIG, /* originatorSig */
	CL_CERTINFO_CMS_SIGTYPEID_DOMAINSIG,	/* domainSig */
	CL_CERTINFO_CMS_SIGTYPEID_ADDITIONALATTRIBUTES, /* additionalAttributesSig */
	CL_CERTINFO_CMS_SIGTYPEID_REVIEWSIG,	/* reviewSig */

	/* 1 2 840 113549 1 9 25 3 randomNonce */
	CL_CERTINFO_CMS_NONCE,				/* randomNonce */

	/* SCEP attributes:
	   2 16 840 1 113733 1 9 2 messageType 
	   2 16 840 1 113733 1 9 3 pkiStatus
	   2 16 840 1 113733 1 9 4 failInfo
	   2 16 840 1 113733 1 9 5 senderNonce
	   2 16 840 1 113733 1 9 6 recipientNonce
	   2 16 840 1 113733 1 9 7 transID */
	CL_CERTINFO_SCEP_MESSAGETYPE,		/* messageType */
	CL_CERTINFO_SCEP_PKISTATUS,			/* pkiStatus */
	CL_CERTINFO_SCEP_FAILINFO,			/* failInfo */
	CL_CERTINFO_SCEP_SENDERNONCE,		/* senderNonce */
	CL_CERTINFO_SCEP_RECIPIENTNONCE,		/* recipientNonce */
	CL_CERTINFO_SCEP_TRANSACTIONID,		/* transID */

	/* 1 3 6 1 4 1 311 2 1 10 spcAgencyInfo */
	CL_CERTINFO_CMS_SPCAGENCYINFO,
	CL_CERTINFO_CMS_SPCAGENCYURL,		/* spcAgencyInfo.url */

	/* 1 3 6 1 4 1 311 2 1 11 spcStatementType */
	CL_CERTINFO_CMS_SPCSTATEMENTTYPE,
	CL_CERTINFO_CMS_SPCSTMT_INDIVIDUALCODESIGNING,	/* individualCodeSigning */
	CL_CERTINFO_CMS_SPCSTMT_COMMERCIALCODESIGNING,	/* commercialCodeSigning */

	/* 1 3 6 1 4 1 311 2 1 12 spcOpusInfo */
	CL_CERTINFO_CMS_SPCOPUSINFO,
	CL_CERTINFO_CMS_SPCOPUSINFO_NAME,	/* spcOpusInfo.name */
	CL_CERTINFO_CMS_SPCOPUSINFO_URL,		/* spcOpusInfo.url */

	/* Used internally */
	CL_CERTINFO_LAST, CL_KEYINFO_FIRST = 3000,

	/*********************/
	/* Keyset attributes */
	/*********************/

	CL_KEYINFO_QUERY,			/* Keyset query */
	CL_KEYINFO_QUERY_REQUESTS,	/* Query of requests in cert store */

	/* Used internally */
	CL_KEYINFO_LAST, CL_DEVINFO_FIRST = 4000,

	/*********************/
	/* Device attributes */
	/*********************/

	CL_DEVINFO_INITIALISE,	/* Initialise device for use */
		CL_DEVINFO_INITIALIZE = CL_DEVINFO_INITIALISE,
	CL_DEVINFO_AUTHENT_USER,	/* Authenticate user to device */
	CL_DEVINFO_AUTHENT_SUPERVISOR,	/* Authenticate supervisor to dev.*/
	CL_DEVINFO_SET_AUTHENT_USER,	/* Set user authent.value */
	CL_DEVINFO_SET_AUTHENT_SUPERVISOR,	/* Set supervisor auth.val.*/
	CL_DEVINFO_ZEROISE,	/* Zeroise device */
		CL_DEVINFO_ZEROIZE = CL_DEVINFO_ZEROISE,
	CL_DEVINFO_LOGGEDIN,		/* Whether user is logged in */
	CL_DEVINFO_LABEL,		/* Device/token label */

	/* Used internally */
	CL_DEVINFO_LAST, CL_ENVINFO_FIRST = 5000,

	/***********************/
	/* Envelope attributes */
	/***********************/

	/* Pseudo-information on an envelope or meta-information which is used to
	   control the way that data in an envelope is processed */
	CL_ENVINFO_DATASIZE,			/* Data size information */
	CL_ENVINFO_COMPRESSION,		/* Compression information */
	CL_ENVINFO_CONTENTTYPE,		/* Inner CMS content type */
	CL_ENVINFO_DETACHEDSIGNATURE,/* Generate CMS detached signature */
	CL_ENVINFO_SIGNATURE_RESULT,	/* Signature check result */
	CL_ENVINFO_MAC,				/* Use MAC instead of encrypting */
	CL_ENVINFO_CURRENT_COMPONENT,/* Cursor management: Relative pos.*/

	/* Resources required for enveloping/deenveloping */
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

	/* Keysets used to retrieve keys needed for enveloping/deenveloping */
	CL_ENVINFO_KEYSET_SIGCHECK,	/* Signature check keyset */
	CL_ENVINFO_KEYSET_ENCRYPT,	/* PKC encryption keyset */
	CL_ENVINFO_KEYSET_DECRYPT,	/* PKC decryption keyset */

	/* Used internally */
	CL_ENVINFO_LAST, CL_SESSINFO_FIRST = 6000,

	/**********************/
	/* Session attributes */
	/**********************/

	/* Pseudo-information on a session or meta-information which is used to
	   control the way that a session is managed */

	/* Pseudo-information about the session */
	CL_SESSINFO_ACTIVE,			/* Whether session is active */
	CL_SESSINFO_CONNECTIONACTIVE,/* Whether network connection is active */

	/* Security-related information */
	CL_SESSINFO_USERNAME,		/* User name */
	CL_SESSINFO_PASSWORD,		/* Password */
	CL_SESSINFO_PRIVATEKEY,		/* Server/client private key */
	CL_SESSINFO_KEYSET,			/* Certificate store */

	/* Client/server information */
	CL_SESSINFO_SERVER_NAME,		/* Server name */
	CL_SESSINFO_SERVER_PORT,		/* Server port number */
	CL_SESSINFO_SERVER_FINGERPRINT,/* Server key fingerprint */
	CL_SESSINFO_CLIENT_NAME,		/* Client name */
	CL_SESSINFO_CLIENT_PORT,		/* Client port number */
	CL_SESSINFO_SESSION,			/* Transport mechanism */
	CL_SESSINFO_NETWORKSOCKET,	/* User-supplied network socket */

	/* Generic protocol-related information */
	CL_SESSINFO_VERSION,			/* Protocol version */
	CL_SESSINFO_REQUEST,			/* Cert.request object */
	CL_SESSINFO_RESPONSE,		/* Cert.response object */
	CL_SESSINFO_CACERTIFICATE,	/* Issuing CA certificate */

	/* Protocol-specific information */
	CL_SESSINFO_TSP_MSGIMPRINT,	/* TSP message imprint */
	CL_SESSINFO_CMP_REQUESTTYPE,	/* Request type */
	CL_SESSINFO_CMP_PKIBOOT,		/* Enable PKIBoot facility */
	CL_SESSINFO_CMP_PRIVKEYSET,	/* Private-key keyset */
	CL_SESSINFO_SSH_SUBSYSTEM,	/* SSH subsystem */
	CL_SESSINFO_SSH_PORTFORWARD,	/* SSH port forwarding */

	/* Used internally */
	CL_SESSINFO_LAST, CL_USERINFO_FIRST = 7000,

	/**********************/
	/* User attributes */
	/**********************/

	/* Security-related information */
	CL_USERINFO_PASSWORD,		/* Password */

	/* User role-related information */
	CL_USERINFO_CAKEY_CERTSIGN,	/* CA cert signing key */
	CL_USERINFO_CAKEY_CRLSIGN,	/* CA CRL signing key */
	CL_USERINFO_CAKEY_RTCSSIGN,	/* CA RTCS signing key */
	CL_USERINFO_CAKEY_OCSPSIGN,	/* CA OCSP signing key */

	/* Used internally for range checking */
	CL_USERINFO_LAST, CL_ATTRIBUTE_LAST = CL_USERINFO_LAST

} CL_ATTRIBUTE_TYPE;


typedef enum
{
	CL_KEYID_NONE,				/* No key ID type */
	CL_KEYID_NAME,				/* Key owner name */
	CL_KEYID_EMAIL,				/* Key owner email address */
	CL_KEYID_FIRST,				/* JM: Recuperar el primer certificado */
	CL_KEYID_LAST				/* Last possible key ID type */
} CL_KEYID_TYPE;


typedef enum
{
	CL_CERTFORMAT_NONE,			/* No certificate format */
	CL_CERTFORMAT_CERTIFICATE,	/* DER-encoded certificate */
	CL_CERTFORMAT_CERTCHAIN,		/* PKCS #7 certificate chain */
	CL_CERTFORMAT_TEXT_CERTIFICATE,	/* base-64 wrapped cert */
	CL_CERTFORMAT_TEXT_CERTCHAIN,	/* base-64 wrapped cert chain */
	CL_CERTFORMAT_XML_CERTIFICATE,	/* XML wrapped cert */
	CL_CERTFORMAT_XML_CERTCHAIN,	/* XML wrapped cert chain */
	CL_CERTFORMAT_LAST			/* Last possible cert.format type */
} CL_CERTFORMAT_TYPE;



typedef enum
{						/* Algorithms */
	/* No encryption */
	CL_ALGO_NONE,				/* No encryption */

	/* Conventional encryption */
	CL_ALGO_DES,					/* DES */
	CL_ALGO_3DES,				/* Triple DES */
	CL_ALGO_IDEA,				/* IDEA */
	CL_ALGO_CAST,				/* CAST-128 */
	CL_ALGO_RC2,					/* RC2 */
	CL_ALGO_RC4,					/* RC4 */
	CL_ALGO_RC5,					/* RC5 */
	CL_ALGO_AES,					/* AES */
	CL_ALGO_BLOWFISH,			/* Blowfish */
	CL_ALGO_SKIPJACK,			/* Skipjack */

	/* Public-key encryption */
	CL_ALGO_DH = 100,			/* Diffie-Hellman */
	CL_ALGO_RSA,					/* RSA */
	CL_ALGO_DSA,					/* DSA */
	CL_ALGO_ELGAMAL,				/* ElGamal */
	CL_ALGO_KEA,					/* KEA */

	/* Hash algorithms */
	CL_ALGO_MD2 = 200,			/* MD2 */
	CL_ALGO_MD4,					/* MD4 */
	CL_ALGO_MD5,					/* MD5 */
	CL_ALGO_SHA,					/* SHA/SHA1 */
	CL_ALGO_RIPEMD160,			/* RIPE-MD 160 */
#ifdef USE_SHA2
	CL_ALGO_SHA2,				/* SHA2 placeholder */
#endif /* USE_SHA2 */

	/* MAC's */
	CL_ALGO_HMAC_MD5 = 300,		/* HMAC-MD5 */
	CL_ALGO_HMAC_SHA,			/* HMAC-SHA */
	CL_ALGO_HMAC_RIPEMD160,		/* HMAC-RIPEMD-160 */

	/* Vendors may want to use their own algorithms that aren't part of the
	   general cryptlib suite.  The following values are for vendor-defined
	   algorithms, and can be used just like the named algorithm types (it's
	   up to the vendor to keep track of what _VENDOR1 actually corresponds
	   to) */
#ifdef USE_VENDOR_ALGOS
	CL_ALGO_VENDOR1 = 10000, CL_ALGO_VENDOR2, CL_ALGO_VENDOR3,
#endif /* USE_VENDOR_ALGOS */

	CL_ALGO_LAST,				/* Last possible crypt algo value */

	/* In order that we can scan through a range of algorithms with
	   cryptQueryCapability(), we define the following boundary points for
	   each algorithm class */
	CL_ALGO_FIRST_CONVENTIONAL = CL_ALGO_DES,
	CL_ALGO_LAST_CONVENTIONAL = CL_ALGO_DH - 1,
	CL_ALGO_FIRST_PKC = CL_ALGO_DH,
	CL_ALGO_LAST_PKC = CL_ALGO_MD2 - 1,
	CL_ALGO_FIRST_HASH = CL_ALGO_MD2,
	CL_ALGO_LAST_HASH = CL_ALGO_HMAC_MD5 - 1,
	CL_ALGO_FIRST_MAC = CL_ALGO_HMAC_MD5,
	CL_ALGO_LAST_MAC = CL_ALGO_HMAC_MD5 + 99	/* End of mac algo.range */
} CL_ALGO_TYPE;


typedef enum
{
	CL_FORMAT_NONE,				/* No format type */
	CL_FORMAT_AUTO,				/* Deenv, auto-determine type */
	CL_FORMAT_CRYPTLIB,			/* cryptlib native format */
	CL_FORMAT_CMS,				/* PKCS #7 / CMS / S/MIME fmt.*/
		CL_FORMAT_PKCS7 = CL_FORMAT_CMS,
	CL_FORMAT_SMIME,				/* As CMS with MSG-style behaviour */
	CL_FORMAT_PGP,				/* PGP format */
	CL_FORMAT_LAST				/* Last possible format type */
} CL_FORMAT_TYPE;



//
// Definición de los tipos de punteros a método
//
#define CRYPT_LIB_API	__stdcall


typedef int CRYPT_LIB_API (*pfnInit)(void);

typedef int CRYPT_LIB_API (*pfnEnd)(void);

typedef int CRYPT_LIB_API (*pfnDeviceOpen)(
								CRYPT_DEVICE *device,
								CRYPT_USER cryptUser,
								int /*CRYPT_DEVICE_TYPE*/ deviceType,
								char *name);

typedef int CRYPT_LIB_API (*pfnDeviceClose)(CRYPT_DEVICE device);

typedef int CRYPT_LIB_API (*pfnSetAttributeString)(
								CRYPT_HANDLE cryptHandle,
								int /*CRYPT_ATTRIBUTE_TYPE*/ attributeType,
								void* value, int valueLength);

typedef int CRYPT_LIB_API (*pfnGetAttributeString)(
								CRYPT_HANDLE cryptHandle,
								int /*CRYPT_ATTRIBUTE_TYPE*/ attributeType,
								void* value,
								int*  valueLength);

typedef int CRYPT_LIB_API (*pfnSetAttribute)(
								CRYPT_HANDLE cryptHandle,
								int /*CRYPT_ATTRIBUTE_TYPE*/ attributeType,
								int value);


typedef int CRYPT_LIB_API (*pfnGetPublicKey)(
								CRYPT_KEYSET keyset,
								CRYPT_CONTEXT* cryptContext,
								int /*CRYPT_KEYID_TYPE*/ keyIDtype,
								char* keyID);

typedef int CRYPT_LIB_API (*pfnGetPrivateKey)(
								CRYPT_KEYSET keyset,
								CRYPT_CONTEXT* cryptContext,
								int /*CRYPT_KEYID_TYPE*/ keyIDtype,
								char* keyID,
								char* password = NULL);

typedef int CRYPT_LIB_API (*pfnDestroyCert)(
								CRYPT_CERTIFICATE certificate);

typedef int CRYPT_LIB_API (*pfnImportCert)(
								void* certObject,
								int certObjectLength,
								CRYPT_USER cryptUser,
								CRYPT_CERTIFICATE* certificate);

typedef int CRYPT_LIB_API (*pfnExportCert)(
								void* certObject,
								int* certObjectLength,
								int /*CRYPT_CERTFORMAT_TYPE*/ certFormatType,
								CRYPT_CERTIFICATE certificate);

typedef	int CRYPT_LIB_API (*pfnCreateContext)(
								CRYPT_CONTEXT *cryptContext,
								CRYPT_USER cryptUser,
								int /*CRYPT_ALGO_TYPE*/ cryptAlgo);

typedef int CRYPT_LIB_API (*pfnDestroyContext)(
								CRYPT_CONTEXT cryptContext);


typedef int CRYPT_LIB_API (*pfnCreateSignatureEx)(
								void* signature,
								int* signatureLength,
								int /*CRYPT_FORMAT_TYPE*/ formatType,
								CRYPT_CONTEXT signContext,
								CRYPT_CONTEXT hashContext,
								CRYPT_CERTIFICATE extraData);

typedef int CRYPT_LIB_API (*pfnCheckSignatureEx)(
								void* signature,
								CRYPT_HANDLE sigCheckKey,
								CRYPT_CONTEXT hashContext,
								CRYPT_HANDLE* extraData = NULL);

typedef int CRYPT_LIB_API (*pfnEncrypt)(
								CRYPT_CONTEXT cryptContext,
								void* buffer,
								int length);

typedef int CRYPT_LIB_API (*pfnDecrypt)(
								CRYPT_CONTEXT cryptContext,
								void* buffer,
								int length);

typedef int CRYPT_LIB_API (*pfnCheckCert)(
								CRYPT_CERTIFICATE certificate,
								CRYPT_HANDLE sigCheckKey);

typedef struct
{
	pfnInit					init;
	pfnEnd					end;
	pfnDeviceOpen			deviceOpen;
	pfnDeviceClose 			deviceClose;
	pfnSetAttributeString 	setAttributeString;
	pfnGetAttributeString 	getAttributeString;
	pfnSetAttribute 		setAttribute;
	pfnGetPublicKey 		getPublicKey;
	pfnGetPrivateKey 		getPrivateKey;
	pfnDestroyCert 			destroyCert;
	pfnImportCert 			importCert;
	pfnExportCert 			exportCert;
	pfnCreateContext 		createContext;
	pfnDestroyContext 		destroyContext;
	pfnCreateSignatureEx 	createSignatureEx;
	pfnCheckSignatureEx 	checkSignatureEx;
	pfnEncrypt				encrypt;
	pfnDecrypt				decrypt;
	pfnCheckCert 			checkCert;
} FuncionesCL32;


class cryptLib
{
public:
	virtual ~cryptLib();

	static cryptLib* getInstancia();
	static bool freeInstancia(bool hacerEnd = true);

	static ArsCadena getErrorString(int status);
	static bool isOK(int status);


	//
	// interfaz público de cryptLib
	//

	// inicio/fin del api
	int init();
	int end();

	// apertura/cierre del dispositivo
	int deviceOpen(	CL_DEVICE* device,
					CL_USER cryptUser,
					CL_DEVICE_TYPE deviceType,
					char* name);

	int deviceClose(CL_DEVICE device);


	// gestión de atributos
	int setAttributeString(
					CL_HANDLE cryptHandle,
					CL_ATTRIBUTE_TYPE attributeType,
					void* value, int valueLength);

	int getAttributeString(
					CL_HANDLE cryptHandle,
					CL_ATTRIBUTE_TYPE attributeType,
					void* value,
					int*  valueLength);

	int setAttribute(CL_HANDLE cryptHandle,
					CL_ATTRIBUTE_TYPE attributeType,
					int value);


	// obtención de claves
	int getPublicKey(CL_KEYSET keyset,
					CL_CONTEXT* cryptContext,
					CL_KEYID_TYPE keyIDtype,
					char* keyID);

	int getPrivateKey(CL_KEYSET keyset,
					CL_CONTEXT* cryptContext,
					CL_KEYID_TYPE keyIDtype,
					char* keyID,
					char* password = NULL);


	// gestión de certificados
	int destroyCert(CL_CERTIFICATE certificate);

	int importCert(	void* certObject,
					int certObjectLength,
					CL_USER cryptUser,
					CL_CERTIFICATE* certificate);

	int exportCert(	void* certObject,
					int* certObjectLength,
					CL_CERTFORMAT_TYPE certFormatType,
					CL_CERTIFICATE certificate);

	// gestión de contextos
	int createContext(
					CL_CONTEXT *cryptContext,
					CL_USER cryptUser,
					CL_ALGO_TYPE cryptAlgo);

	int destroyContext(CL_CONTEXT cryptContext);


	// generación/verificación de firmas
	int createSignatureEx(
					void* signature,
					int* signatureLength,
					CL_FORMAT_TYPE formatType,
					CL_CONTEXT signContext,
					CL_CONTEXT hashContext,
					CL_CERTIFICATE extraData);

	int checkSignatureEx(
					void* signature,
					CL_HANDLE sigCheckKey,
					CL_CONTEXT hashContext,
					CL_HANDLE* extraData = NULL);

	// cifrado/descrifrado
	int encrypt(CL_CONTEXT cryptContext, void* buffer, int length);
	int decrypt(CL_CONTEXT cryptContext, void* buffer, int length);

	// comprobación de certificado contra CRL
	int checkCert(CL_CERTIFICATE certificate, CL_HANDLE sigCheckKey);

protected:
	cryptLib();

	bool loadDLL(const ArsCadena &nombreDLL);
	bool freeDLL();

private:
	static cryptLib*  instancia;

	FuncionesCL32	funciones;
	HINSTANCE		hDLL;
};


#endif
