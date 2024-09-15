# NetworkingLibrary-RakNet - Client

Il sottoprogetto **Client** è responsabile della gestione della comunicazione di rete lato client. L’obiettivo principale è consentire al client di connettersi, inviare pacchetti di dati, elaborare le risposte e gestire eventuali errori o disconnessioni. Questo sottoprogetto si basa ed ereditate le classi ed interfacce esposte dalla libreria Network.

- [Componenti Principali](#componenti-principali)

<br></br>

## Componenti Principali

### 1. **`main`**
Il punto d'ingresso dell'applicazione client. Qui vengono inizializzate le risorse di rete attraverso l'istanza di `Net::CNetDevice`. Se l'inizializzazione ha successo, crea un'istanza della classe `Client`.
- **Loop principale**: Una volta avviato il client e stabilita la connessione al server, il loop principale consente di avviare il processo di elaborazione dei pacchetti in arrivo, chiamando `ProcessNet`.
- La chiusura della connessione avviene al termine dell'esecuzione, con la pulizia delle risorse di rete.




### 2. **Classe `Client`**
La classe `Client` eredita da due classi astratte, `Net::CAbstractEntityClient` e `Net::CAbstractPacketClientHandler<Client>`, implementando la logica di rete per un client.
- **Costruttore**: Inizializza il gestore dei pacchetti e imposta lo stato del client come disconnesso.
- **Eventi di connessione**:
  - `__OnInitFail`, `__OnInitSuccess`: Gestiscono il successo o il fallimento dell'inizializzazione.
  - `__OnConnectFail`, `__OnConnectSuccess`: Informano sullo stato del tentativo di connessione al server.
- **Processo di rete**: La funzione `ProcessNet()` gestisce la ricezione dei pacchetti e il loro processamento, controllando gli stati di connessione, disconnessione o perdita di connessione con il server.
- **Gestione pacchetti**:
  - `LoadPacketHeaders()`: Associa gli header dei pacchetti con le rispettive funzioni di gestione.
  - `ProcessPacketError()`: Gestisce eventuali errori di processamento dei pacchetti, come header mancanti o mismatch di dimensione.
  - Funzioni specifiche per l'invio e la ricezione di pacchetti, come `SendHandshake()`, `SendLogin()`, e `TestSend()`.


### 3. **`Packet.h`**
Definisce le strutture dati e gli header per i pacchetti che vengono scambiati tra il client e il server.
