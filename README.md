# Proyecto IA

| Nombre | Rol | 
|----------|----------|
| Ricardo Olalquiaga    | 202073091-1   | 

## Uso

Se debe ejecutar `make` en la carpeta raíz, donde se encuentra `main.cpp`. Se deja un comando `make example` que ejecutará la instrucción `./GVRP instances/AB101.dat 100000`.

```
make
make example
```

Su uso luego de la compilación es de la forma

```
./GVRP [ruta relativa de archivo] [n° de iteraciones]
```

## Consideraciones

* El programa considerará el orden de los nodos entregados en los `.dat` como 1 depósito, *m* estaciones de servicio y *n* clientes. Si se altera el orden, el programa no podrá ejecutarse correctamente. 
* Las iteraciones a testear deben mínimo en el orden de las 100.000, ya que así el código puede converger a un mínimo local. Iteraciones menores darán resultados malos y posiblemente infactibles.