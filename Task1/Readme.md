# L1-A: Actividad de Programación

> Cierre: jueves, 8 de mayo de 2025, 23:59

## Calculadora Sencilla

Se desea un programa que reciba cada dígito introducido por el usuario, y muestre el número resultante alineado a la derecha, y su resultado

> Ejemplo: el usuario presiona `1,7,3,+,5,4,\n`

Impresión esperada:

```txt
  173
+  54
------
  237
```

Debe tener soporte para suma '+', resta '-', multipliación '*' y división entera '/'

## Sugerencias

- Para alinear a la derecha, use ancho fijo
  - Por ejemplo, para alinear al noveno caracter, use printf("%9d", x)
- Para borrar un caracter ya escrito, use printf("\b") (donde \b significa "backspace")
  - Imprima tantos backspace como sea necesario
- Use atoi(…) para convertir de char a int
- Si no desea leer los caracters uno a la vez, utilice scanf("%d", &x)

Encontrará más información de estas funciones en los primeros capítulos del libro K&R y en su apéndice.