/* Generated by Together */

#ifndef VALIDACIONCERTIFICADO_H
#define VALIDACIONCERTIFICADO_H
#include "Certificado.h"

class ValidacionCertificado {


public:

    virtual void estaRevocado(Certificado&) =0;

    virtual void estaCaducado() =0;

    private :    

    /** @link dependency */
    /*# Certificado lnkCertificado; */
};
#endif //VALIDACIONCERTIFICADO_H