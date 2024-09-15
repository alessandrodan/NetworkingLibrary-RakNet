# NetworkingLibrary-RakNet - Network

Questa libreria offre un'implementazione delle funzionalità di rete basata su RakNet. Oltre ad implementare le classiche operazioni di rete (connessione, disconnessione, ecc), si occupa anche della gestione dei pacchetti e della logica di comunicazione.

- [Componenti Principali](#componenti-principali)

<br></br>


### 1. **Entità di rete (`CAbstractEntity` e le sue classi derivate)**:
   - **`CAbstractEntity`**: Classe base astratta che rappresenta un'entità di rete (server e client). Include metodi come `Start` e `ProcessNet`.
   - **`CAbstractEntityClient`**: Derivata da `CAbstractEntity`, questa classe gestisce l'inizializzazione specifica del client, la connessione a un server e la gestione del successo o del fallimento della connessione.
   - **`CAbstractEntityServer`**: Derivata da `CAbstractEntity`, si occupa dell'inizializzazione e delle configurazioni specifiche del server.



### 2. **`CAbstractPeer`**:
   Questa classe rappresenta un peer nella rete, ovvero un client connesso a un server. Definisce metodi di base per la gestione dei pacchetti e delle fasi di comunicazione.
   - **Metodi principali:**
     - `void ProcessRecv(NetPacket* packet)`: Elabora i pacchetti ricevuti.
     - `void Packet(const void* c_pvData, int iSize)`: Invia un pacchetto di dati.
     - `void SetPhase(int phase)`: Imposta la fase corrente del peer.



### 3. **Gestione dei pacchetti**:
   - **`NetPacket`**: Struttura che rappresenta un pacchetto di rete, e contiene informazioni come indirizzo di sistema, GUID, intestazione, lunghezza e dati.
   - **Elaborazione dei pacchetti (`PacketManagerBase`, `PacketManagerClient`, e `PacketManagerServer`)**:
     - Questi template gestiscono il mapping dei pacchetti e garantiscono che questi corrispondano alle dimensioni e intestazioni previste.



### 4. **`CPacketIO`**:
   - Classe di utily che implementa metodi statici per la lettura e l'invio di pacchetti nella rete.



### 5. **Gestori di pacchetti**:
   - **`CAbstractPacketHandler`**: Classe base astratta per la gestione dei pacchetti. Viene ereditata da:
     - **`CAbstractPacketClientHandler`**: Gestisce l'elaborazione dei pacchetti sul lato client.
     - **`CAbstractPacketServerHandler`**: Gestisce l'elaborazione dei pacchetti sul lato server.
   - Entrambi utilizzano il `PacketManager` per elaborare i pacchetti in base all'header.



### 6. **NetDevice**:
   - **Descrizione:** Classe che gestisce le operazioni di rete a basso livello, come la connessione, la ricezione e l'invio di pacchetti, interfacciandosi con `RakPeerInterface`.
   - **Metodi principali:**
     - `void Destroy()`: Termina e distrugge la sessione di rete.
     - `bool Create()`: Crea un'istanza del peer di rete.
     - `ConnectionAttemptResult Connect(const char* host, unsigned short remotePort)`: Stabilisce una connessione a un host specifico.
     - `SLNet::Packet* Receive()`: Riceve un pacchetto dalla rete.
    
     
