# NetworkingLibrary-RakNet

## Panoramica del Progetto

NetworkingLibrary-RakNet è una libreria progettata per la gestione di connessioni client-server, basata su [SLikeNet](https://github.com/SLikeSoft/SLikeNet.git), un fork di RakNet. L’obiettivo principale è permettere la comunicazione bidirezionale tra client server, scambiando pacchetti di dati in modo efficiente. Il progetto si suddivide in tre componenti fondamentali: la libreria di rete, il client e il server.


### Descrizione del Progetto

1. **Libreria di rete**: Fornisce le funzionalità per gestire le connessioni di rete. Si interfaccia con le API di SLikeNet e implementa la logica di comunicazione tra client e server, occupandosi anche della gestione dei pacchetti e delle diverse fasi di comunicazione.

2. **Client**: Il client eredita e personalizza le classi fornite dalla libreria di rete. È responsabile della connessione al server, della gestione della comunicazione tramite pacchetti strutturati e seguire il flusso di comunicazione stabilito (es. handshake e poi autenticazione).

3. **Server**: Il server eredita e personalizza le classi fornite dalla libreria di rete. Gestisce le operazioni lato server e il flusso di dati tra più connessioni simultanee. Implementa le diverse fasi di comunicazione e include funzionalità per monitorare e amministrare le connessioni attive.




## Caratteristiche Principali

- **Comunicazione TCP**
- **Programmazione orientata agli oggetti**
- **Fasi di comunicazione**: Include fasi predefinite come handshake e autenticazione.
- **Gestione pacchetti estendibile**: Permette la creazione e gestione di pacchetti personalizzati.
- **Architettura modulare ed estensibile**: Facilita l'integrazione di nuove funzionalità senza alterare la struttura esistente.
- **Connessioni multi-client**: Supporta più client simultanei, con opzioni per configurare timeout e numero massimo di connessioni.
- **Basato su un engine stabile (RakNet/SLikeNet)**

## Struttura del Progetto

Il progetto è strutturato come segue:

```
NetworkingLibrary-RakNet/
├── Client/                 # Implementazione del client
├── Network/                # Libreria di rete
├── Server/                 # Implementazione del server
└── NetworkingLibrary-RakNet.sln   # File di soluzione per la compilazione del progetto
```



## Documentazione

Per maggiori informazioni su architettura e funzionalità di ciascun componente, fai riferimento ai README specifici nei sottoprogetti:

- [Libreria di Rete](./Network/README.md)
- [Client](./Client/README.md)
- [Server](./Server/README.md)




## Requisiti di Sistema

- **Sistema Operativo**: Windows
- **Ambiente di Sviluppo**: Visual Studio 2019 (compilatore MSVC)




## Installazione

1. Apri il file `NetworkingLibrary-RakNet.sln` nella root del progetto utilizzando Visual Studio 2019.
2. Compila il progetto con il compilatore MSVC.




## Utilizzi e Applicazioni

La libreria può essere utilizzata in diversi scenari, tra cui:

- **Giochi Multiplayer**: Consente lo scambio di pacchetti per aggiornamenti in tempo reale tra giocatori.
- **Sistemi di Chat**: Invio e ricezione di messaggi tra utenti.
- **Applicazioni Distribuite**: Scambio continuo di dati tra nodi in una rete.



## Note Aggiuntive

Per ulteriori dettagli e informazioni tecniche, consulta i README specifici dei sottoprogetti.
