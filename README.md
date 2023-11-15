# Proyecto IA

| Nombre | Rol | 
|----------|----------|
| Ricardo Olalquiaga    | 202073091-1   | 

## Uso

Se debe ejecutar `make` en la carpeta raíz, donde se encuentra `main.cpp`. Se deja un comando `make example` que ejecutará la instrucción `./GVRP instances/AB101.dat 1000`.

```
make
make example
```

Su uso luego de la compilación es de la forma

```
./GVRP [ruta relativa de archivo] [n° de iteraciones]
```
## Pruebas

Dentro de la aplicación existen algunas funciones implementadas que permite el debugueo y comprobar el funcionamiento. Por el momento, se deben escribir dentro de `main.cpp`:

* `print_matriz(matriz_costos, n)`: Printea la matriz de costos con n nodos.
* `print_poblacion(poblacion, k, n)`: Printea la población con k cromosomas de largo n.

## Consideraciones

* El programa considerará el orden de los nodos entregados en los `.dat` como 1 depósito, *m* estaciones de servicio y *n* clientes. Si se altera el orden, el programa no podrá ejecutarse correctamente. 