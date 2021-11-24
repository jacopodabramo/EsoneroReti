# Documentazione
Repository riguardante la prima parte dell'esonero (protocollo TCP) dell'esame di reti di calcolatori.<br>
Il software realizzato è un'applicazione Client/Server, in cui il server è una calcolatrice capace di effettuare operazioni di somma, sottrazione, moltiplicazione e divisione tra numeri interi; mentre il Client si occupa di inviare la stringa contenente l'operazione e gli operandi, nella giusta sintassi, al Server.<br>
 
 ## Spiegazione di alcune scelte
Visto che la calcolatrice effettua operazione fra numeri interi, si è deciso di gestire il risultati delle operazioni attraverso numeri reali _(float)_. <br>
Le operazioni sono definite in questo modo:
- +: Intero x Intero -> Reale
- x: Intero x Intero -> Reale
- /: Intero x Intero -> Reale
- -: Intero x Intero -> Reale <br>

La _divisione per zero_ visto che è un'operazione che non può essere svolta, si è deciso che il server restuisca il massimo numero reale che può essere rappresentato in C, in modo tale che il Client una volta che riceve questo numero può stampare un messaggio di errore. <br>

_All' avvio_, sia al Client che al Server possono essere passati dei parametri da _Comand Line_ che identificano rispettivamente _l'indirizzo ip_ e il _numero di porta_, che, nel caso del client, specificano i parametri del server a cui si deve connettere, nel caso del server, invece, permette di assegnare indirizzo ip e numero di porta diversi da quelli di default. Se non dovessero essere passati parametri da _Comand Line_ verranno usati _indirizzo ip_ e _numero di porta_ di default corrispondenti a:
- _127.0.0.1 per l'indirizzo ip_ 
- _27015 per il numero di porta_ <br>

Per il controllo della stringa inserita dall'utente, si è deciso, come da requisiti, che la stringa verrà ritenuta corretta esclusivamente nel seguente formato:<br> 
<p align = "center"><code>[Operazione][Spazio][Primo operando][Spazio][Secondo operando]</code> </p> 
<br>
_Esempi di stringhe accettate:_ 
- <code>+ 10 20</code>
- <code>x -10 20</code>
- <code>- -10 -20</code><br>
_Esempi di stringhe non accettate:_ 
- <code>++ 10 20</code>
- <code>+ 10 20aaa</code>
- <code>- --10 20</code> <br>

## Autori
L'esonero è stato realizzato da:
- Lorenzo Cassano (matricola 718331)
- Jacopo D'Abramo (matricola 716484)
