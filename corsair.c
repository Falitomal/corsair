/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corsair.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jledesma <jledesma@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:30:52 by jledesma          #+#    #+#             */
/*   Updated: 2023/05/06 13:43:13 by jledesma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "openssl/ssl.h"
#include "openssl/bn.h"
#include "openssl/bio.h"
#include "openssl/evp.h"
#include "openssl/pem.h"
#include "openssl/x509.h"

#define RSA_KEYLEN 1024

/* Cargar Certificado RSA desde un fichero y return CLAVE RSA*/
RSA *loadRSAKey(char *fichero)
{
    // Definir las variables
    X509        *cert;                 // Certificado
    EVP_PKEY    *pkey;             // Clave pública
    RSA         *rsa;                   // Clave RSA
    BIO         *bio;                   // Buffer de entrada
    int          fd;               // Indicador de lectura correcta

    // Inicializar las variables
    bio = BIO_new(BIO_s_file());                    // Crear el buffer de entrada
    fd = BIO_read_filename(bio, fichero);     // Abrir el archivo

    // Comprobar que el fichero se leyó correctamente
    if (fd != 1) {
        printf("Error al leer el fichero '%s'.\n", fichero);
        exit(1);
    }

    // Leer el certificado
    cert = PEM_read_bio_X509(bio, NULL, 0, NULL);   // Leer el certificado
    pkey = X509_get_pubkey(cert);                   // Obtener la clave pública
    rsa = EVP_PKEY_get1_RSA(pkey);                  // Obtener la clave RSA
    
    if (!cert || !pkey || !rsa) {
        printf("Error al leer el certificado '%s'.\n", fichero);
        exit(1);
    }
    // Liberar la memoria
    X509_free(cert);        // Liberar el certificado
    EVP_PKEY_free(pkey);    // Liberar la clave pública
    BIO_free(bio);          // Liberar el buffer de entrada

    return rsa;
}

/* Main return 0 si es correcto */
int main(int argc, char *argv[])
{   
    if (argc != 4) {
        printf("Uso: %s <archivo_certificado1> <archivo_certificado2> <archivo_a_descifrar>\n", argv[0]);
        return (1);
    }
    //Variables del programa
    //BUFFERS
    unsigned char *buffer_entra;      //Buffer de lectura
    unsigned char *buffer_result;     //Buffer de escritura
    BN_CTX *ctx;                    //Contexto
    RSA *rsa_privada;               //Clave RSA privada
    BIO *bioprint;                  //Buffer de escritura
    BIGNUM *numero_one;                 //Numero  1
    
    //Inicializar variables certificado 1
    RSA *rsa_publica1;              //Clave RSA publica 1
    BIGNUM *numero_cert1;                //Numero Certificado 1
    BIGNUM *qmero_cert1;                //Qmero 2  
    //Inicializar variables certificado 2
    RSA *rsa_publica2;              //Clave RSA publica 2
    BIGNUM *numero_cert2;                //Numero Certificado2
    BIGNUM *qmero_cert2;                //Qmero 2

    //Primo en comun
    BIGNUM *primo_comun;                //Primo en comun

    //Variables comunes
    BIGNUM *total;                      //Total
    BIGNUM *e;                          //Exponente Clave publica
    BIGNUM *d;                          //Exponente Clave privada

    BIGNUM *fi1;                    //Numero de factores primos de N1
    BIGNUM *fi2;                    //Numero de factores primos de N2
    
    int fd;
    int len;
    
            //Iniciar variables de buffer
    buffer_entra = malloc(sizeof(unsigned char) * RSA_KEYLEN);
    buffer_result = malloc(sizeof(unsigned char) * RSA_KEYLEN);
    //Inicializar variables BigNum
    ctx = BN_CTX_new();             //Inicializar contexto
    bioprint = BIO_new_fp(stdout, BIO_NOCLOSE); //Inicializar buffer de escritura
            //Cargar RSA
    rsa_publica1 = loadRSAKey(argv[1]);
    rsa_publica2 = loadRSAKey(argv[2]);
    
    numero_one = BN_new();          //Inicializar numero 1
    qmero_cert1 = BN_new();         //Inicializar qmero certificado 1
    qmero_cert2 = BN_new();         //Inicializar qmero certificado 2
    primo_comun = BN_new();         //Inicializar primo en comun que obtendremos con gcd
    total = BN_new();               //Inicializar total
                 //Inicializar exponente clave publica
    d = BN_new();                   //Inicializar exponente clave privada
    fi1 = BN_new();                 //Inicializar numero de factores primos de N1
    fi2 = BN_new();                 //Inicializar numero de factores primos de N2
    
            
            
            //Calculos para obtener los primos
    numero_cert1 = (BIGNUM*) RSA_get0_n(rsa_publica1);
    /*
    Esta función obtiene el valor n (producto de dos números primos) de la clave RSA pública 1.
    rsa_publica1 es un puntero a la estructura RSA, y el resultado se guarda en numero_cert1.
    */      
    numero_cert2 = (BIGNUM*) RSA_get0_n(rsa_publica2);      
    /*
    Esta función obtiene el valor n (producto de dos números primos) de la clave RSA pública 2.
    rsa_publica2 es un puntero a la estructura RSA, y el resultado se guarda en numero_cert1.
    */      
    e = (BIGNUM*) RSA_get0_e(rsa_publica1);                 // Obtener "e" de la clave publica
    /*
    Esta función obtiene el exponente público "e" de la clave RSA pública 1.
    */
    rsa_privada = RSA_new();
    /*
    Esta función crea una nueva estructura RSA vacía y devuelve un puntero a ella.
    Esta estructura se utilizará posteriormente para almacenar la clave RSA privada que se calculará.
    */
    BN_gcd(primo_comun, numero_cert1, numero_cert2, ctx);    
    /*
    Esta función calcula el máximo común divisor (GCD) de numero_cert1 y numero_cert2,
    que son los valores n de las dos claves RSA públicas.
    El resultado se guarda en primo_comun, que es un puntero a la estructura BIGNUM. ctx es un puntero aun
    BN_CTX que se utiliza para la gestión eficiente de la memoria durante las operaciones de aritmética de números grandes
    */
    BN_div(qmero_cert1, NULL, numero_cert1, primo_comun, ctx);
    /*
    Esta función divide numero_cert1 por primo_comun y guarda el resultado en qmero_cert1,
    que es un puntero a la estructura BIGNUM. El segundo argumento NULL indica que no se necesita calcular el resto de la división.
    ctx es un puntero a un contexto BN_CTX que se utiliza para la gestión eficiente de la memoria
    durante las operaciones de aritmética de números grandes
    */
    BN_div(qmero_cert2, NULL, numero_cert2, primo_comun, ctx);  //Obtener el numero de q primos de N2
    /*
    Esta función divide numero_cert2 por primo_comun y guarda el resultado en qmero_cert2,
    que es un puntero a la estructura BIGNUM. El segundo argumento NULL indica que no se necesita calcular el resto de la división.
    ctx es un puntero a un contexto BN_CTX que se utiliza para la gestión eficiente de la memoria
    durante las operaciones de aritmética de números grandes
    */
    
    BN_dec2bn(&numero_one, "1");
    /*
    Esta función convierte la cadena de caracteres "1" en un número decimal
    y lo guarda en la estructura BIGNUM apuntada por numero_one.
    Esto es para obtener una representación de BIGNUM del número 1, que se utilizará en cálculos posteriores.
    */
    BN_sub(fi1, qmero_cert1, numero_one);
    /*
    Esta función resta numero_one (que es 1) de qmero_cert1
    y guarda el resultado en fi1. Aquí se calcula f1 = q1 - 1 utilizando la fórmula de Euler.
    */
    BN_sub(fi2, primo_comun, numero_one);
    /*
    Esta función resta numero_one (que es 1) de primo_comun y guarda el resultado en fi2.
    Aquí se calcula f2 = p - 1 utilizando la fórmula de Euler.
    */
    BN_mul(total, fi1, fi2, ctx); 
    /*
    Esta función multiplica fi1 y fi2 y guarda el resultado en total.
    Aquí se calcula total = f1 * f2. ctx es un puntero a un contexto BN_CTX
    que se utiliza para la gestión eficiente de la memoria durante las operaciones de aritmética de números grandes.
    */
    BN_mod_inverse(d, e, total, ctx);
    /*
    Esta función calcula el inverso modular de e con respecto a total y guarda el resultado en d.
    Aquí se calcula d = e^-1 mod total, donde d es el exponente de la clave RSA privada. ctx es un puntero a un 
    contexto BN_CTX que se utiliza para la gestión eficiente de la memoria durante las operaciones de aritmética de números grandes.
    */
                
                 //Cargar clave privada
    RSA_set0_key(rsa_privada, numero_cert1, e, d);
    /*Esta función establece los valores n, e, y d de la clave RSA privada.
    rsa_privada es un puntero a la estructura RSA, numero_cert1 es el valor de n (producto de dos números primos),
    e es el exponente público y d es el exponente privado.*/
                
                //Asociar los numeros primos en cada RSA
    RSA_set0_factors(rsa_publica1, primo_comun, qmero_cert1);
    /*
    Esta función establece los factores primos p y q para la clave RSA pública 1.
    rsa_publica1 es un puntero a la estructura RSA,
    primo_comun es el valor del factor primo p,
    y qmero_cert1 es el valor del factor primo q.
    */
    RSA_set0_factors(rsa_publica2, primo_comun, qmero_cert2);
    /*
    Esta función establece los factores primos p y q para la clave RSA pública 2.
    rsa_publica2 es un puntero a la estructura RSA, 
    primo_comun es el valor del factor primo p, y qmero_cert2 es el valor del factor primo q.
    */
    
                //Mostar por pantalla los numeros primos de cada certificado
    printf("Certificado 1:\n");
    RSA_print(bioprint, rsa_publica1, 0);
    /*
    Imprime la información de la clave RSA pública 2 en el objeto BIO llamado bioprint.
    */
    RSA_print(bioprint, rsa_privada, 0);
    /*
    Imprime la información de la clave RSA privada en el objeto BIO llamado bioprint.
    */
    printf("Certificado 2:\n");
    RSA_print(bioprint, rsa_publica2, 0);
    /*
    Imprime la información de la clave RSA pública 2 en el objeto BIO llamado bioprint.
    */
    RSA_print(bioprint, rsa_privada, 0);
    /*Imprime la información de la clave RSA privada en el objeto BIO llamado bioprint.
    La salida se muestra en la pantalla la misma clave privada
    utilizada para descifrar mensajes cifrados con ambas claves públicas.
    */
                //Leer el archivo de entada y descifrar su contenido
    fd = open(argv[3], O_RDONLY);
    if (fd < 0) {
        printf("Error al abrir el archivo de entrada");
        exit(1);
    }
    len = read(fd, buffer_entra, RSA_KEYLEN);
    if (len <= 0) {
        printf("Error al leer el archivo de entrada");
        close(fd);
        exit(1);
    } 
    RSA_private_decrypt(len, buffer_entra, buffer_result, rsa_privada, RSA_PKCS1_PADDING);
    /*
    Esta funcion de la biblioteca OpenSSL para descifrar datos cifrado con claves RSA pública usando la clave privada correspondiente
    Usando len que es la longitud de buffer_entrada, esta que contiene los datos cifrados que se van a descifrar
    Buffer_resultado es un puntero donde se almacenara los datos descifrados en texto planoÇ
    rsa_privada es la clave privada, y RSA_PKCS1_PADDING es un tipo de relleno que se utiliza en cifrado RSA.
    Dicho de otra manera esta funcion toma los datos cifrados de buffer_entra
    y lo descifra utilizando la clave privada almacenandolo en buffer_result
    */

                //Mostar por pantalla el mensaje cifrado y descifrado
    printf("Mensaje cifrado:\n");
    printf("%s\n", buffer_entra);
    printf("Mensaje descifrado:\n");
    printf("%s\n", buffer_result);
    
                //Liberacion de todas las variables 
    BN_CTX_free(ctx);
    BIO_free(bioprint);
                
    
    BN_free(numero_one);
    BN_free(numero_cert1);
    BN_free(qmero_cert1);
    BN_free(numero_cert2);
    BN_free(qmero_cert2);
    
    BN_free(primo_comun);
    BN_free(e);
    BN_free(d); 
    
    BN_free(total);
    BN_free(fi1);
    BN_free(fi2);
    
/*                 //Liberar memoria */
    free(buffer_entra);
    free(buffer_result);              //Cerrar archivo
    close(fd);
    //comprobar leaks de memoria
    system("leaks -q corsair");
    return 0;
    
}

    //system("leaks -q corsair");
