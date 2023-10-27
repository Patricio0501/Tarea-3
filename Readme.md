git clone <https://github.com/Patricio0501>
cd biblioteca
gcc main.c funciones_biblioteca.c -o biblioteca
./biblioteca

Como compilar y ejecutar la tarea:

Para ejecutar este programa debemos tener en cuenta 3 factores importantes:
1.	Se trabaja con archivos de texto.
2.	Solo se pueden ingresar números en el menú desde el 1 hasta el 8, por lo tanto, si no se ingresa un número dentro de ese rango provocará un error.
3.	Hay que tener en claro que, al iniciar un número del menú, pedirá los archivos que están dentro de los archivos de texto, para los cuales hay que respetar mayúsculas, espacios y números.
Para que el programa comience, se tendrá que escribir un número del 1 al 8, cada uno de estos números tiene una función dentro de la cual se encargará de proporcionar lo que dice el enunciado del número:
1.	Cargar documentos: El usuario coloca los nombres de una lista de archivos txt separados por espacio, donde estos son cargados por la aplicación. Para que funcione perfectamente hay que tener cuidado en respetar las mayúsculas, minúsculas y números.
2.	Mostrar documentos ordenados: La aplicación muestra los ID y títulos de los libros ordenados por título, además se muestra por la cantidad de palabras y caracteres. 
3.	Buscar un libro por título: El usuario coloca algunas palabras separadas por espacios y la aplicación muestra los títulos de libros que contienen todas las palabras.
4.	Palabras con mayor frecuencia: El usuario ingresa el ID de un libro y la aplicación muestras las 10 palabras que se repiten con mayor frecuencia.
5.	Palabras más relevantes: El usuario ingresa el título de un libro y la aplicación muestras las 10 palabras más relevantes.
6.	Buscar por palabra: El usuario ingresa una palabra y la aplicación muestra los libros que tienen la palabra en su contenido.
7.	Mostrar palabra en su contexto dentro del libro: El usuario ingresa el título de un libro y el de una palabra a buscar. La aplicación muestra las distintas apariciones de la palabra en el contexto del documento, es decir, para cada aparición, se muestran algunas palabras hacia atrás y hacia adelante de la palabra buscada.

NOMBRES DE INTEGRANTES DEL GRUPO:
-	Belén Fernández.
-	Patricio Cisternas 
-	Ignacio Figueroa.

Funcionando correctamente: 
1. Cargar documentos.
2. Mostrar documentos ordenados.
3. Buscar un libro por título.
4. Palabras con mayor frecuencia.
6. Buscar por palabra.

Funcionando incorrectamente:
5. Palabras más relevantes.
7. Mostrar palabra en su contexto dentro del libro.

Organización del grupo:
Todos los integrantes del grupo mostraron mucha participación e interés, cuando teníamos errores en las funciones contribuíamos en resolverlos y ayudarnos mutuamente a entender las funciones creadas por el otro, cada uno explicó como realizó la función que se le proporcionaba, por lo cual de esta forma todos aprendían como se hacía cada una de las funciones, en las cuales se aceptaban críticas y contribuciones positivas para el crecimiento personal al realizar el código.
Específicamente lo que hizo cada uno es lo siguiente:
-	Patricio Cisternas: Realizó las funciones de cargar documentos, mostrar documentos ordenados y buscar libro por título, además también ordeno el código para que se viera de la mejor manera posible.
-	Belén Fernández: Realizo las funciones de buscar por palabra y mostrar palabra, además realizó la mayor parte del README y aportó en la realización de los comentarios.
-	Ignacio Figueroa: Realizó las funciones de palabras con mayor frecuencia y palabras más relevantes, además comento el código y aportó en la realización del README.

Calificación de aporte al trabajo:
-	Patricio Cisternas: Aporte excelente = 3
-	Belén Fernández: Aporte bueno = 2
-	Ignacio Figueroa: Aporte bueno = 2