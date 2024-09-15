# NetworkingLibrary-RakNet - Server

Il sottoprogetto **Server** è responsabile della gestione della comunicazione di rete lato server. Si occupa della gestione delle connessioni, dell'autenticazione e della trasmissione dei pacchetti. Il server gestisce più client contemporaneamente, dove ciascun client è visto come un "peer", ossia un'entità di rete che invia e riceve dati. Ogni connessione segue un ciclo ben definito: **connessione**, **handshake**, **autenticazione**, e infine l'interazione durante la **fase di gioco** o operativa.



### Componenti Principali

#### 1. **`main`**
Il file `main.cpp` è il punto d'ingresso dell'applicazione server. Qui vengono inizializzate le risorse di rete richiamando il metodo `Create` di `Net::CNetDevice` e viene gestito il ciclo di vita della connessione del server.



#### 2. **`ServerMain`**
La classe `ServerMain` eredita da `Net::CAbstractEntityServer` e rappresenta la classe centrale del sottoprogetto server. Si occupa di gestire il ciclo di vita del server, l'accettazione delle connessioni, e la gestione dei peer.

- **Metodi principali**:
  - **`__OnInitFail(int errorCode)`**: viene chiamato in caso di fallimento dell'inizializzazione del server.
  - **`__OnInitSuccess()`**: crea il gestore dei peer (`CPeerManager`) e attiva il monitoraggio delle connessioni in entrata.
  - **`ProcessNet()`**: gestisce il ciclo principale del server, elaborando pacchetti, connessioni e disconnessioni.



#### 3. **`Peer`**
La classe `CPeer` rappresenta un client connesso al server (peer). Gestisce il ciclo di vita della connessione per un determinato peer e le varie fasi della comunicazione, come handshake e autenticazione. È responsabile della ricezione dei pacchetti e dell'invio delle rispettive risposte.

- **Metodi principali**:
  - **`CPeer::Setup()`**: imposta il peer con un identificatore unico (GUID) e avvia l'handshake.
  - **`SendHandshake()`**: invia il pacchetto di handshake al client.
  - **`ProcessRecv()`**: elabora i pacchetti ricevuti dal client e li smista in base alla fase di comunicazione (handshake, autenticazione, gioco).
  - **`SetPhase(int phase)`**: imposta la fase corrente del peer (ad esempio, `PHASE_HANDSHAKE`, `PHASE_AUTH` o `PHASE_GAME`).




#### 4. **`Packet.h`**
Contiene le definizioni dei pacchetti che il server e i client utilizzano per comunicare. Ogni pacchetto è contraddistinto da un "header" che ne determina il tipo e la funzione.




#### 5. **`PeerManager`**
La classe `CPeerManager` gestisce i peer connessi, monitorando il loro stato e coordinando le fasi di comunicazione. Consente al server di mantenere traccia di tutti i peer attivi.

- **Metodi principali**:
  - **`AcceptPeer()`**: accetta una nuova connessione client.
  - **`Destroy()`**: chiude tutte le connessioni peer.
  - **`DestroyClosed()`**: elimina i peer chiusi dalla lista.
  - **`GetPeer()`**: recupera un peer specifico tramite il suo GUID.



#### 6. **`ServerAuth`**
La classe `ServerAuth` gestisce la fase di autenticazione dei peer. Dopo il completamento dell'handshake, i client devono autenticarsi per poter accedere alle funzionalità del server.

- **Metodi principali**:
  - **`LoadPacketHeaders()`**: associa gli header dei pacchetti ai metodi di gestione (ad esempio, autenticazione).
