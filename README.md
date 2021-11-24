# Documentazione
 Repository riguardante la prima parte dell'esonero(protocollo TCP) di reti.<br>
 Il software realizzato riguarda un'applicazione Client/Server, dove il server è una calcolatrice capace di effettuare operazioni di somma, sottrazione, moltiplicazione e divisione tra numeri interi; mentre il Client si occupa di inviare i dati dell'operazione al Server.<br>
 
 ## Spiegazione di alcune scelte
Visto che la calcolatrice effettua operazione fra numeri interi, si è deciso di gestire il risultati delle operazioni attraverso numeri reali _(float)_. <br>
Le operazioni sono definite in questo modo:
- +: Intero x Intero -> Reale
- x: Intero x Intero -> Reale
- /: Intero x Intero -> Reale
- -: Intero x Intero -> Reale <br>

La _divisione per zero_ visto che è un'operazione che non può essere svolta, si è deciso che il server restuisca il massimo numero reale che può essere rappresentato in C, in modo tale che il Client una volta che riceve questo numero può stampare un messaggio di errore. <br>

_All' avvio_ sia al Client che al Server possono essere passati i parametri da _Comand Line_ che identificano _l'indirizzo ip_ e il _numero di porta_, nel caso in cui non dovessero essere passati parametri da _Comand Line_ verranno usati _indirizzo ip_ e _numero di porta_ di default corrispondenti a:
- _127.0.0.1 per l'indirizzo ip_ 
- _27015 per il numero di porta_ <br>

Per il controllo della stringa inserita dall'utente si è deciso che la stringa verrà ritenuta corretta solamente nel momento in cui si trovi nel seguente formato:<br> 
<p align = "center"><code>[Operazione][Spazio][Primo operando][Spazio][Secondo operando]</code> </p> 
(DA CAMBIARE IN BASE ALLA RISPOSTA DEL PROF).
<br>

## Autori
L'esonero è stato realizzato da:
- Lorenzo Cassano (matricola 718331)
- Jacopo D'Abramo (matricola 716484)
