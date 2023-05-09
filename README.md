<h1 align="center">
📖 co-RSA-ir - 42 Proyect
</h1>

<p align="center">
	<b><i>RSA Projects</i></b><br>
</p>

<p align="center">
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Falitomal/corsair?color=lightblue" />
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/Falitomal/corsair?color=yellow" />
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/Falitomal/corsair?color=blue" />
	<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/Falitomal/corsair?color=green" />
</p>



## ✏️ Sinopsis 
```

Este proyecto aborda conceptos específicos relacionados con la solidez del **algoritmo RSA** y sus posibles vulnerabilidades.

Aunque el algoritmo se considera lo suficientemente robusto frente a la capacidad computacional de los dispositivos actuales.

 **el uso incorrecto del mismo puede generar serios problemas de seguridad**.

```
## 💡 Concepto del proyecto
```

Desarrollar una herramienta en lenguaje C que permita:
- [x] Extraer la clave pública de dos certificados, obteniendo el módulo y el exponente de cada uno de ellos.
- [x] Reconstruir la clave privada a partir de dos números primos y su producto.
- [x] Reconstruir la clave simétrica cifrada utilizando la clave privada.

```
## 🛠️ Usage

Debemos instalar las librerias de OPENSSL en el equipo y cambiar la ruta en el Makefile de donde estén:

## 💡Generar Certificados
```

Para generar los dos certificados usar en shell el siguiente comando:

  make generate
```

## 💡Usage Corsair
```
  
make 

./corsair certificado1 certificado2 archivo_a_descifrar

```
## ✏️ Limpieza
```

make fclean

```

En el archivo .c se encuentra comentada una línea para verificar las fugas de memoria del programa. Además, todas las funciones están debidamente comentadas.
