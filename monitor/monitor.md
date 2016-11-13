# Monitor

## Monitor e tipi di dato astratto
#### Nascono dall'adattamento del concetto di regione critica al concetto di tipo di dato astratto. Un tipo di dato astratto T racchiude in una unità logicamente coesa:
* la rappresentazione di un dato di tipo T
* tutte le operazioni lecite sui dati di tipo T

#### Possono esistere diverse istanze dello stesso monitor, come della medesima risorsa.
#### L'idea è che la rappresentazione del tipo di dato astratta costituisce la risorsa comune condivisa alla quale vengono allegate le operazioni che può subire quando si trova in sezione critica.
#### Una possibile sintassi per i monitor: 
```
type <nome_monitor> = monitor
	<dichiarazioni di tipi e costanti globali>;
var <variabili condivise>;
entry procedure OP1 (<lista_parametri>) begin
	<dichiarazioni locali>;
	<corpo di OP1>;
end
<eventuali procedure locali al monitor>;
<procedura di inizializzazione delle variabili condivise>;
end monitor
```
#### Il monitor esporta le OP che costituiscono le uniche operazioni permesse sui tipi di dato <nome_monitor>, le operazioni vengono richiamate su un'istanza del tipo <nome istanza>.OPi(<parametri attuali>), inoltre serve un costruttore, per inizializzare le variabili condivise, le variabili del monitor, rappresentano lo stato della risorsa comune, sono condivise ed accessibili unicamente attraverso le OP.
#### Le esecuzioni delle OP avviene in modo mutuamente esclusivo con quella di possibili chiamate concorrenti ad operazioni sulla stessa istanza di monitor.

## Le primitive wait e signal di Hoare
#### I monitor rispetto alle regioni critiche:
* risolvono il problema della scarsa coesione
* consentono di risolvere problemi di competizione
* non consentono di risolvere facilmente i problemi di cooperazione per la mancanza di meccanismi espliciti per la sincronizzazione tra f.d.e. 

#### Per superare questi limiti bisogna introdurre nei monitor le primitive signal e wait per la sincronizzazione tra f.d.e.
#### L'idea è di associare una variabile condizione agli eventi o condizione di interesse su cui sincronizzarsi:
* un modo per assegnare un nome esplicito a degli eventi interesse.
* condizioni su una risorsa condivisa per la quale competono diversi f.d.e.
* tali f.d.e. durante le loro sezioni critiche modificano lo stato della risorsa e finiscono per alterare la valutazione delle condizioni
* i f.d.e. che attendono l'evento associato possono esplicitare il proprio interesse emettendo una wait sulla variabile condizione
* i f.d.e. che sono nella posizione di conoscere quando tali eventi si verificano notificano f.d.e. interessati emettendo una signal sulla variabile condizione associata.

#### Alla variabile condizione è associata una coda di attesa, qunado un f.d.e. esegue una <variabile-condizione>.wait esso viene bloccato ed inserito nella coda di attesa e rilascia il monitor, quando invece esegue <variabile-condizione>.signal, se nessun altro f.d.e. è in attesa sulla variabile condizione prosegue il proprio avanzamento, altrimenti viene bloccato e rilascia il monitor ed il primo f.d.e. in attesa sulla variabile condizione viene risvegliato per rientrare in competizione sul monitor.
#### Per utilizzarle basta associare una variabile condizione Xcond per ogni condizione necessaria all'avanzamento dell'esecuzione in una procedura del monitor ed inserire:
```
while(not <cond>) do
	Xcond.wait;
end
```
#### È poi compito del programmatore individuare i punti in cui <cond> può diventare vera e forzare la segnalazione agli altri f.d.e. tramite:
```
Xcond.signal;
```

### Ci sono diverse semantiche di wait e signal:
* signal_and_continue: il flusso che esegue la signal continua indisturbato mantenendo l'uso esclusivo del monitor sino alla naturale terminazione
* signal_and_wait: il flusso che esegue la signal viene sospeso e rilascia il monitor per dare la possibilità ai flussi interessati di reagire immediatamente alla segnalazione
* signal_and_return: le due semantiche di sopra collassano in quanto si richiede esplicitamente che una signal sia sempre collocata come ultima istruzione eseguita in ogni entry-procedure.

### Implementazione dei semafori con wait e signal
#### Si supponga di voler implementare le primitive P(S) e V(S) facendo uso dei monitor con le primitive wait e signal, la condizione su cui sincronizzarsi è S>0, gli associamo una variabile condizione S_POSITIVO, e la coda associata alla variabile condizione coinciderà in questo caso con quella del semaforo. Basterà invocare la P e la V, rispettivamente con S.P e S.V:
```
type semaforo = monitor
	var S:0..last;
	var S_POSITIVO: condition; //variabile condizione per S>0
	entry procedure P begin
		while(S=0) do S_POSITIVO.wait; end
		S <- S-1;
	end
	entry procedure V begin 
		S<-S+1;
		S_POSITIVO.signal;
	end
	begin 
		S<-0
	end
end monitor
```
#### Per gestire i sistemi di cooperazione, bisogna rivisitare il concetto di sezione critica:
* in prcedenza: una sequenza di istruzioni eseguita da f.d.e. per accedere ad una risorsa R (seriale) con la garanzia di possederla esclusivamente e senza perderne il possesso durante tutta l'esecuzione.
* D'ora in poi: durante la sezione critica è possibile che il f.d.e. perda temporaneamente l'uso della risorsa purché il flusso stesso rimanga sospeso durante il rilascio (altrimenti verrebbe meno la mutua esclusione).

#### Ogni problema di cooperazione nasconde un sottostante problema di cooperazione: per sincronizzarsi su un evento inerente una risorsa condivisa è necessario possederla, sia per segnalare l'evento (bisogna valutare una condizione sulla risorsa per fare la segnalazione), sia per reagire alla segnalazione (bisogna valutare una condizione sulla risorsa per accertare che l'evento notificato sia realmente accaduto). 

## Mailbox (Produttori/Consumatori con i monitor)
#### Le operazioni da implementare sono:
* DEPOSITA(M:Messaggio) per i produttori
* CONSUMA(ref R: messaggio) per i consumatori

#### Gli eventi su cui sincronizzarsi sono:
* buffer non pieno (variabile condizione NON_PIENO)
* buffer non vuoto (varaibile condizione NON_VUOTO)

#### Rispetto alla soluzione con i semafori l'accesso esclusivo agli indici risulta inutile perché l'uso esclusivo del buffer è garantito dalla semantica dei monitor.
```
type mailbox(messaggio) = monitor
	sia N:...;
	type indice = 0...N-1;
	var BUFFER: array[indice] of messaggio
	var T,D: indice;
	var NON_PIENO, NON_VUOTO: condition;
	K:integer; //numero messaggi presenti
	
	entry procedure DEPOSITA(M:messaggio) begin
		while(K=N) NON_PIENO.wait;
		BUFFER[D] <- M;
		D <- (D+1) mod N;
		K <- K+1;
		NON_VUOTO.signal;
	end
	
	entry procedure PRELEVA(ref M1:messaggio) BEGIN
		WHILE(K=0) NON_VUOTO.wait;
		M1 <- BUFFER[T];
		T <- (T+1) mod N;
		K <- K-1;
		NON_PIENO.signal;
	end
	
	begin
		K <- 0; T <- 0; D <- 0;
	end
end monitor
```
#### Rispetto alla soluzione con i semafori, la soluzione con i monitor è più semplice ed elegante, tuttavia impone limitazioni nel parallelismo:
* un produttore ed un consumatore non possono operare concorrentemente neanche se il buffer non è ne vuoto nè pieno.
* Due produttori non possono depositare contemporaneamente in due posizioni diverse nel buffer
* Due consumatori non possono prelevare contemporaneamente da due posizioni diverse del buffer.
* vengono emesse signal dai produttori senza che ci siano consumatori in attesa di consumare
* viceversa tra consumatori e produttori.

## Implementazione dei monitor tramite semafori
#### Si associa un semaforo binario MUTEX ad ogni istanza di monitor, il corpo di ciascuna procedura viene racchiusa tra P(MUTEX) e V(MUTEX). Ad ogni variabile condizione si associa:
* un contatore Ccond inizializzato a 0 che conta il numero di f.d.e. bloccati sulla <cond> (per evitare segnalazioni inutili)
* un semaforo binario Scond inizializzato a 0 (per la sincronizzazione sulla variabile condizione).

#### wait e signal possono essere quindi implementate come segue:
```
Xcond.wait: begin
	Ccond <- Ccond + 1;
	V(MUTEX); //libera il monitor
	P(Scond); //attende una Xcond.signal
	Ccond <- Ccond-1;
end;

Xcond.signal: begin
	if Ccond > 0 then V(Scond)
	else V(MUTEX)
end
```

## Monitor: Svantaggi e vantaggi
### Vantaggi:
* Il vantaggio principale è il maggior livello di astrazione che deriva dal loro utilizzo

### Svantaggi:
* limitazioni sul parallelismo, il meccanismo automatico di mutua esclusione talvolta risulta troppo conservativo.
* problema delle chiamate annidate, l'implementazione tramite semafori non permette la nidificazione delle chiamate sulla medesima istanza di monitor
* presuppongono la disponibilità di memoria comune.

## Monitor e POO
#### I monitor hanno trovato piena applicazione con i linguaggi che utilizzano il paradigma di programmazione orientata agli oggetti, la tipologia di risorsa comune su cui regolare gli accessi concorrenti è modellata da una classe. La risorsa è un oggetto di tale classe, le operazioni su tale risorsa corrispondono ai metodi pubblici della classe.
