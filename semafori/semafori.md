# Semafori di Dijkstra
## Il problema della mutua esclusione
#### Dati due f.d.e. P e Q e una risorsa R, garantire che:
* In ogni istante, R è libera oppure risulta assegnata ad uno solo tra P e Q (mutua esclusione)
* Ciascuno dei due flussi di esecuzione deve sempre poter ottenere l'uso della risorsa (fairness)

#### La prima condizione impone la serializzazione nell'uso della risorsa, la seconda che nessun f.d.e. rimanga indefinitivamente in attesa della risorsa.
#### Ogni soluzione prevede un protocollo di utilizzo della risorsa R articolato in tre fasi:
* Richiesta di R
* Utilizzo di R 
* Rilascio di R

#### Il frammento di programma che utilizza la risorsa si dice "Sezione critica" o "regione critica" rispetto alla risorsa R. I f.d.e. in sezione critica vogliono accedere alla risorsa con la garanzia di esserne l'unico utilizzatore. Praticamente quando la sezione critica è occupata da un f.d.e., se un f.d.e. tenta di entrare viene sospeso, finisce il primo f.d.e. e poi gli viene consentito l'accesso alla sezione critica.
#### Un esempio di risoluzione del problema di mutua esclusione, si ha una variabile X associata alla risorsa R, se X=0, allora uno ed un solo f.d.e. è nella sua sezione critica, se X=1, allora nessun f.d.e. è nella sua sezione critica. In realtà è sufficiente che X sia un bit di memoria, detto anche semaforo di basso livello, X=0 significa rosso, X=1 significa verde.

## Spin Lock
#### Il protocollo di utilizzo assume questa forma:
```
<richiesta>: LOCK(X):
				begin
					loop begin // ciclo di attesa attività
						if X=1 exit; //aspetta che sia verde
					end
					X<-0 // poni il semaforo a rosso
				end
<rilascio>: UNLOCK(X):
				begin 
					X<-1 //poni il semaforo a verde
				end
```
#### Questa proposta presenta gravi problemi:
* Attesa attività
* Non garantisce l'accesso seriale, P esegue la LOCK(X) e trova il semaforo verde, ma prima di riuscire a farlo diventare rosso, perde l'utilizzo del processore fisico, allorché anche Q esegue la LOCK(X) e trova il semaforo ancora verde, entrambi proseguono impostando il semaforo a rosso.
* Bisogna garantire l'indivisibilità delle due primitive.

```
<richiesta>: LOCK(X):
				begin
					loop begin // ciclo di attesa attività
						<disabilita interruzioni>
						if X=1 exit; //aspetta che sia verde
						<abilita interruzioni>
						<ritardo>
					end
					X<-0 // poni il semaforo a rosso
					<abilita interruzioni>
				end
<rilascio>: UNLOCK(X):
				begin 
					<disabilita interruzioni>
					X<-1 //poni il semaforo a verde
					<abilita interruzioni>
				end
```

#### La soluzione presentata garantisce l'indivisibilità delle primitive LOCK e UNLOCK, tuttavia ha anch'essa degli inconvenienti:
* Valida solo su sitemi uniprocessor, poiché è possibile che P esegue la LOCK, e trova il semaforo verde, ma prima di riuscire a farlo diventare rosso, interviene un f.d.e. Q in esecuzione su un altro processore, così si trovano i due f.d.e. in sezione critica
* alla sequenza di interleaving ipotizzata prima, si è sostituita una sequenza di esecuzione analoga.
* Richiede continuamente di disabilitare le interruzioni

#### Questi inconvenienti si risolvono con un aiuto da parte dell'hardware, l'unico punto in cui la LOCK può essere interrotta è nell'intervallo di tempo che intercorre tra il test del valore di X e la sua assegnazione a 0, pertanto si prevede un'istruzione macchina TestAndSet che in maniera indivisibile controlla il valore di un bit e lo azzera.

```
<richiesta>: LOCK(X):
				begin
					loop begin
						TestAndSet(X); //istruzione macchina indivisibile
							if X=1 exit; //aspetta che sia verde
						<ritardo>
					end
					//qui il semaforo è già rosso
				end
<rilascio>: UNLOCK(X):
				begin 
					Set(X); //poni il semaforo a verde
				end
```
#### Ci sono ancora inconvenienti in quanto è presente un attesa attiva (il processore viene impegnato attivamente nel controllare il valore di un bit, per tutto il periodo in cui il f.d.e. attende che la risorsa si liberi), non è garantita la proprietà di Fairness (non esiste garanzia esplicita contro l'evenienza che un f.d.e. attenda indefinitivamente un semaforo che diventa verde infinite volte), quindi si escludono gli spin-lock come soluzione fair al problema della mutua esclusione (ma sarà utilizzata per costruire soluzioni migliori).

#### Oggi esistono primitive di alto livello, chiamati semaforo di alto livello o di Dijkstra. Le due primitive ricevono come parametro un intero non negativo, S detto semaforo. I f.d.e. eseguono attese attive perché verificano autonomamente gli eventi sui quali sincronizzarsi, per un f.d.e. che attende un evento, esiste sempre un altro f.d.e. nella posizione per indicargli attivamente quando si verifica. Le attese attive possono essere trasformate in passive, ovvero i f.d.e. consumatori di eventi ad esplicitare il proprio interesse ed attendere passivamente che si verificano, mentre i f.d.e. produttori di eventi ad informare attivamente i corrispondenti f.d.e. consumatori.

## Le primitive P & V di Dijkstra
```
P(S):
	if S=0
		then <<poni il f.d.e. in stato di attesa passiva di una V(S)>>
	else S <- S-1;
V(S):
	if <<esiste un f.d.e. in attesa di una V(S)>>
		then <<risveglia uno di questi f.d.e.>>
	else S <- S+1	
```
#### Ad ogni semaforo S è associato una coda Qs di f.d.e. in attesa di una V(S), un f.d.e. che esegue una P(S) può avanzare solo se trova S>0, altrimenti deve accodarsi in Qs per attendere passivamente una V(S), un f.d.e. che esegue una V(S), se non ci sono f.d.e. in attesa in Qs ha come effetto quello di incrementare S, altrimenti risveglia uno dei f.d.e. in Qs, in qualsiasi caso il f.d.e. che ha eseguito la V(S) può continuare indisturbato. La disciplina di estrazione e inserimento in Qs deve garantire la proprietà di fairness, in genere si utilizza una disciplina FIFO.

#### Implementazione della primitiva P(S):
```
P(S): begin 
			<disabilita le interruzioni>
			LOCK(SX)
			if S=0 then <<sospendi il f.d.e. ed inserisci il suo 
							descrittore nella coda Qs>>
			else S <- S-1;
			UNLOCK(SX)
			<abilita le interruzioni>
		end
```
#### Implementazione della primitiva V(S):
```
V(S): begin
			<disabilita le interruzioni>
			LOCK(SX)
			if Qs non è vuota
				then <<estrai un descrittore da Qs e poni il 
						corrispondente f.d.e. in stato di pronto>>
			else S <- S+1
			UNLOCK(SX)
			<abilita le interruzioni>
```
#### SX è uno spin-lock utilizzato per garantire l'indivisibilità delle due primitive ed è necessario solamente nei sistemi multiprocessore
#### Lo spin-lock garantisce l'indivisibilità (ma non la fairness) delle primitive di f.d.e. che avanzano su processori diversi, la disabilitazione delle interruzioni garantisce l'indivisibilità delle primitive che avanzano sullo stesso processore

## Espressività dei semafori

#### I semafori sono sufficienti a risolvere qualsiasi problema di sincronizzazione, è infatti possibile implementare la primitiva join, con i semafori.
#### Due utilizzi tipici dei semafori:
* per sincronizzare due f.d.e. su un evento
* per risolvere problemi di competizione nell'accesso ad una risorsa di molteplicità finita

#### Per i due utilizzi si usano rispettivamente:
* Semafori in stile cooperativo: assumono valore 0 o 1, vengono inizializzati a 0, il f.d.e. che effettua una V è diverso da quello che ha effettuato la P.
* Semafori in stile competitivo: assumono valore da 0 a M, se M è la molteplicità della risorsa, vengono inizializzati al valore della molteplicità, il f.d.e. che effettua una V è lo stesso che ha effettuato la P di apertura della sezione critica sulla risorsa.

#### Costo della sincronizzazione
* Blocking: attesa passiva e context switch, quindi si ha costo delle attese attive sullo spin-lock di basso livello e il costo del context-switch
* Non-Blocking: Attesa attiva, si ha il costo delle attese attive sullo spin-lock di alto livello.

#### Per macchine multiprocessore, deve esistere un livello di competizione, sotto il quale se le sezioni critiche sono di durata sufficientemente bassa, può risultare meno oneroso, una attesa attiva di breve durata che due context-switch.

