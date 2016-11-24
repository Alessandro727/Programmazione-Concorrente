# Guida per installare ed usare CUnit su MAC con Xcode.

#### Per prima cosa scaricare CUnit con brew, attraverso questo comando: 
```
brew install cunit
```

#### Se non avete installato brew, potete installarlo seguendo le indicazioni al seguente [link](http://brew.sh/index_it.html)

#### Una volta completata l'installazione di cunit dovreste verificare che avete questo file in questa directory:
```
/usr/local/opt/cunit
```

#### Una volta verificato aprite il vostro progetto su Xcode, e cliccate il file xcodeproj (il file in cima al vostro progetto sulla navigation laterale).
#### All'interno di quel file, cliccate su 'All' in alto e andate alla voce 'linking', nella sottovoce 'Other linker Flags' mettete '-lncurses' e '-lcunit' sia per debug che per release.
#### Poi andate alla voce 'Search Path', e nella sottovoce 'Library Search Paths' inserite:
* '/usr/local/opt/cunit'
* '/usr/local/opt/cunit/lib'
* '/usr/local/opt/cunit/include'

#### Anche qui sia per debug che per release, infine quando includete le librerie di CUnit al posto di:
```
#include CUnit/Basic.h 
```
#### Inserite 
```
#include "/usr/local/opt/cunit/include/CUnit/Basic.h"
```