# Comandos

## Ejecutar
```sh
rm main ; clear ; make
```

## Compilar
```sql
g++ main.cpp Client.cpp EdgeServer.cpp Node.cpp Transport.cpp Message.cpp DNS.cpp ../../src/libcppsim.a -I../../src  -o main
```

## Tests
- [ ] `./main 100 5 5000`

## Mostrar Grafo Simulador
*__Importante__: Se debe hacer después de ejecutar el programa, ya que se crea al final de cada ejecución*
```sql
# Formato "dot"
dot ./grafo.dot -o ./grafodot && xdot ./grafodot
# Formato "neato" (para poder posicionar los elementos mas distribuidamente)
dot ./grafo.dot -o ./grafodot && xdot -f "neato" ./grafodot
```

# Otros

## Git
- Agregar modificaciones (includyendo archivos eliminados): `git add . -A`
- Revert changes made to your working copy:
  `git checkout .`
Revert changes made to the index (i.e., that you have added):
	`git reset`
* Revert a change that you have committed:
	`git revert ...`

## Crear grafo de dependencias
```
 	perl grafo.pl --quotetypes=quote >> grafo/grafo.dot
 	dot grafo/grafo.dot -o grafo/grafo
 	xdot grafo/grafo
 	#Todo en un comando:
 	perl grafo.pl --quotetypes=quote >> ./grafo/grafo.dot && dot ./grafo/grafo.dot -o ./grafo/grafo && xdot ./grafo/grafo
```
