/* CAMPO MINATO
    Realizzare il gioco campo minato, indicando con 0 le celle senza mine, con -1 le celle con le mine ed attorno alle mine posizionare, come nel vero campo minato, il numero di mine adiacenti.

    Cliccando su una cella senza mine, questa si scopre e mostra il numero di mine adiacenti. Se si clicca su una cella con una mina, la partita termina. Si possono utilizzare delle bandierine (indicatori) per segnalare le celle che si pensa contengano delle mine. Si ha un limite di bandierine pari al numero di mine presenti nel campo.

    Si vince la partita quando si scoprono tutte le celle senza mine.
    Si possono scegliere tre livelli di difficoltà:
    - Principiante: 9x9 celle con 10 mine
    - Intermedio: 16x16 celle con 40 mine
    - Esperto: 16x30 celle con 99 mine
    Si può scegliere di giocare in modalità test, in cui le posizioni delle mine sono note.

    In alto viene contato il tempo trascorso dall'inizio della partita e il numero di bandierine rimanenti.
    Utilizzare i codici ANSI per colorare le celle in base al loro stato (coperte, scoperte, con bandierina) e al numero di mine adiacenti.
    Utilizzare i codici ANSI per salire di una riga (\033[A) e per eliminare l’intera riga (\033[2K).

    Sviluppatore: Alessandro Cundari
    Matricola: 275922
    Data ultima versione: 16/12/2025

    TO DO:
    - [X] Definire i pacchetti e le dimensioni del campo
    - [X] Inizializzare il campo di gioco
    - [X] Inserire le mine nel campo di gioco
    - [X] Calcolare i numeri adiacenti alle mine
    
    FASE 1 - STRUTTURE DATI:
    - [X] Creare array per tracciare lo STATO delle celle (coperta/scoperta/bandiera)
    - [X] Definire costanti per gli stati: COPERTA=0, SCOPERTA=1, BANDIERA=2
    
    FASE 2 - VISUALIZZAZIONE:
    - [X] Creare funzione mostraCampoGioco() che:
          * Mostra timer in formato MM:SS in alto
          * Mostra numero bandiere rimanenti
          * Mostra griglia con numeri riga/colonna
          * Mostra celle coperte come ■
          * Mostra celle scoperte con numero o vuote
          * Mostra bandiere con simbolo B
    
    FASE 3 - TIMER:
    - [X] Salvare time_t all'inizio partita con time(NULL)
    - [X] Calcolare tempo trascorso con difftime(time(NULL), tempoInizio)
    - [X] Convertire in minuti e secondi per visualizzazione
    
    FASE 4 - INPUT UTENTE:
    - [X] Creare loop principale del gioco
    - [X] Leggere comando utente (s=scopri, b=bandiera, q=esci)
    - [X] Leggere coordinate riga e colonna
    - [X] Validare che coordinate siano dentro i limiti
    
    FASE 5 - LOGICA SCOPRI CELLA:
    - [X] Se cella ha bandiera → bloccare azione
    - [X] Se cella è mina (-1) → GAME OVER
    - [X] Se cella ha numero > 0 → scoprire solo quella cella
    - [X] Se cella è vuota (0) → scoprire ricorsivamente celle adiacenti vuote
    
    FASE 6 - LOGICA BANDIERA:
    - [X] Se cella già scoperta → bloccare azione
    - [X] Se cella ha bandiera → rimuoverla e incrementare contatore
    - [X] Se cella coperta e bandiere disponibili → piazzare bandiera
    - [X] Decrementare contatore bandiere quando piazzata
    
        FASE 7 - CONDIZIONI VITTORIA/SCONFITTA:
        - [X] Creare funzione controllaVittoria() che verifica se tutte le celle
            non-mina sono scoperte
        - [X] Se scopri mina → mostrare tutto il campo e messaggio SCONFITTA
        - [X] Se scopri ultima cella sicura → messaggio VITTORIA con tempo finale
    
    FASE 8 - REFRESH SCHERMO:
    - [X] Pulire schermo ad ogni turno (uso di \033[A e \033[2K)
    - [X] Ridisegnare campo aggiornato
    - [X] Mostrare comandi disponibili sotto il campo
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Dimensione Principiante
#define RIGHE_PRINCIPIANTE 9
#define COLONNE_PRINCIPIANTE 9
#define MINE_PRINCIPIANTE 10

// Dimensione Intermedio
#define RIGHE_INTERMEDIO 16
#define COLONNE_INTERMEDIO 16
#define MINE_INTERMEDIO 40

// Dimensione Esperto
#define RIGHE_ESPERTO 16
#define COLONNE_ESPERTO 30
#define MINE_ESPERTO 99

// Codici ANSI per formattazione
#define ROSSO_GRASSETTO "\033[1;31m" // Rosso
#define BG_GRIGIO_CHIARO "\033[47m" // Sfondo grigio chiaro
#define BG_GRIGIO_SCURO "\033[100m" // Sfondo grigio scuro
#define RESET "\033[0m" // Reset formattazione del colore

// Stato delle celle
#define COPERTA 0
#define SCOPERTA 1
#define BANDIERA 2

// Funzioni di utilità per pulire lo schermo e il menù
void pulisciMenu(){
    for(int i = 0; i < 12; i++){
        printf("\033[A"); // Salta su di una riga
        printf("\033[2K"); // Pulisce l'intera riga
    }
}

void pulisciSchermo(){
    for(int i = 0; i < 50; i++){
        printf("\033[A"); // Salta su di una riga
        printf("\033[2K"); // Pulisce l'intera riga
    } // Pulisce tutto lo schermo e porta il cursore in alto a sinistra
}

// Menù
void menu(){
    printf("\n\n================= CAMPO MINATO ================\n");
    printf("Scegli il livello di difficoltà:\n\n");

    printf("1. Principiante (9x9, 10 mine)\n");
    printf("2. Intermedio (16x16, 40 mine)\n");
    printf("3. Esperto (16x30, 99 mine)\n");
    printf("4. Modalità Test (On/Off)\n");
    printf("5. Tutorial\n");
    printf("0. Esci\n\n");
}
// TUTORIAL:
void tutorial(){
    pulisciSchermo();
    printf("================= TUTORIAL ================\n\n");
    printf("Obiettivo:\n");
    printf("- Scopri tutte le celle sicure senza trovare una mina.\n\n");
    printf("Comandi:\n");
    printf("- s: scopri una cella (se 0, si espandono le celle vuote adiacenti)\n");
    printf("- b: piazza/rimuovi una bandiera (massimo = numero di mine)\n");
    printf("- q: esci dalla partita\n\n");
    printf("Indicazioni:\n");
    printf("- Timer in alto in formato MM:SS\n");
    printf("- Bandiere rimanenti mostrate accanto al timer\n");
    printf("- Modalità Test mostra le mine durante il gioco\n\n");

    // Legenda
    printf("Legenda: ");
    printf("%s■%s = Coperta, ", BG_GRIGIO_CHIARO, RESET);
    printf("%sB%s = Bandiera, ", BG_GRIGIO_CHIARO, RESET);
    printf("%s*%s = Mina, ", ROSSO_GRASSETTO, RESET);
    printf("%s_%s = Vuota, ", BG_GRIGIO_CHIARO, RESET);
    printf("numero = mine adiacenti\n\n");

    // Piccolo esempio 5x5
    const int R = 5, C = 5;
    int esempioCampo[5][5] = {
        {0, 1, 1, 1, 0},
        {0, 1,-1, 2, 1},
        {0, 1, 1, 2,-1},
        {0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0}
    };
    int esempioStato[5][5];
    for(int i=0;i<R;i++) for(int j=0;j<C;j++) esempioStato[i][j] = COPERTA;
    // Scoperte
    esempioStato[0][0] = SCOPERTA;
    esempioStato[0][1] = SCOPERTA;
    esempioStato[1][1] = SCOPERTA;
    esempioStato[3][0] = SCOPERTA;
    esempioStato[3][1] = SCOPERTA;
    esempioStato[3][2] = SCOPERTA;
    esempioStato[4][2] = SCOPERTA;
    esempioStato[4][3] = SCOPERTA;
    esempioStato[4][4] = SCOPERTA;
    // Bandiere su possibili mine
    esempioStato[1][2] = BANDIERA; // sospetta mina
    esempioStato[2][4] = BANDIERA; // sospetta mina

    // Esempio senza mostrare le mine (gioco normale)
    printf("Esempio (gioco normale):\n");
    printf("   ");
    for(int j=0;j<C;j++) printf("%2d ", j);
    printf("\n");
    for(int i=0;i<R;i++){
        printf("%2d ", i);
        for(int j=0;j<C;j++){
            const char* bg = ((i+j)%2==0) ? BG_GRIGIO_CHIARO : BG_GRIGIO_SCURO;
            if(esempioStato[i][j] == COPERTA){
                printf("%s ■ %s", bg, RESET);
            } else if(esempioStato[i][j] == BANDIERA){
                printf("%s B %s", bg, RESET);
            } else { // SCOPERTA
                if(esempioCampo[i][j] == 0){
                    printf("%s _ %s", bg, RESET);
                } else {
                    printf("%s %d %s", bg, esempioCampo[i][j], RESET);
                }
            }
        }
        printf("\n");
    }
    printf("\n");

    // Esempio Modalità Test (mine visibili)
    printf("Esempio (Modalità Test - mine visibili):\n");
    printf("   ");
    for(int j=0;j<C;j++) printf("%2d ", j);
    printf("\n");
    for(int i=0;i<R;i++){
        printf("%2d ", i);
        for(int j=0;j<C;j++){
            const char* bg = ((i+j)%2==0) ? BG_GRIGIO_CHIARO : BG_GRIGIO_SCURO;
            if(esempioCampo[i][j] == -1){
                // Mostra sempre le mine
                printf("%s * %s", ROSSO_GRASSETTO, RESET);
            } else if(esempioStato[i][j] == COPERTA){
                printf("%s ■ %s", bg, RESET);
            } else if(esempioStato[i][j] == BANDIERA){
                printf("%s B %s", bg, RESET);
            } else { // SCOPERTA
                if(esempioCampo[i][j] == 0){
                    printf("%s _ %s", bg, RESET);
                } else {
                    printf("%s %d %s", bg, esempioCampo[i][j], RESET);
                }
            }
        }
        printf("\n");
    }

    printf("\nCondizioni:\n");
    printf("- Se scegli una mina: GAME OVER\n");
    printf("- Se scopri tutte le celle sicure: VITTORIA\n\n");
    printf("Premi Invio per tornare al menu...");
    fflush(stdout); // Serve per assicurarsi che tutto venga stampato prima di attendere l'input
    int c;
    while((c = getchar()) != '\n' && c != EOF) {}
    getchar();
    pulisciSchermo();
}

// =============================================

// Inizializzazione campo
void inizializzaCampo(size_t righe, size_t colonne, int campo[righe][colonne]){
    for(size_t i = 0; i<righe; i++){
        for(size_t j = 0; j<colonne; j++){
            campo[i][j] = 0;
        }
    }
}

// Mine
void inserisciMine(size_t righe, size_t colonne, int campo[righe][colonne], int mine){
    int count = 0;
    while (count < mine){
        size_t raw = rand() % righe; // Riga casuale
        size_t col = rand() % colonne; // Colonna casuale

        if(campo[raw][col] != -1){ // Se non c'è già una mina
            campo[raw][col] = -1; // Inserisci mina
            count++;
        }
    }
}

// Numeri adiacenti alle mine
void calcolaNumeri(size_t righe, size_t colonne, int campo[righe][colonne]){
    for(size_t i = 0; i<righe; i++){
        for(size_t j = 0; j<colonne; j++){
            if(campo[i][j] == -1){

                // Scorri le celle adiacenti
                for (int k = -1; k <= 1; k++) {
                    for (int w = -1; w <= 1; w++) {
                        size_t riga_adiacente = i + k;
                        size_t colonna_adiacente = j + w;

                        // Controlla i limiti del campo
                        if (riga_adiacente >= 0 && riga_adiacente < righe && colonna_adiacente >= 0 && colonna_adiacente < colonne) {
                            if (campo[riga_adiacente][colonna_adiacente] != -1) {
                                campo[riga_adiacente][colonna_adiacente]++; // Incrementa il contatore delle mine adiacenti
                            }
                        }
                    }
                }
            }
        }
    }
}

// Stampa campo (per modalità test)
void stampaCampoTest(size_t righe, size_t colonne, int campo[righe][colonne]){
    pulisciSchermo();
    printf("CAMPO DI GIOCO (Modalità Test):\n\n");
    for(size_t i = 0; i<righe; i++){
        for(size_t j = 0; j<colonne; j++){
            // Colore alternato per effetto scacchiera
            const char* bg_color;
            if((i + j) % 2 == 0){
                bg_color = BG_GRIGIO_CHIARO;
            } else {
                bg_color = BG_GRIGIO_SCURO;
            }
            
            if(campo[i][j] == -1){
                printf("%s * %s", ROSSO_GRASSETTO, RESET); // Mine in rosso grassetto
            } else if(campo[i][j] == 0){
                printf("%s _ %s", bg_color, RESET); // Celle libere con sfondo alternato
            } else {
                printf("%s %d %s", bg_color, campo[i][j], RESET);
            }
        }
        printf("\n");
    }
}



void mostraCampoGioco(size_t righe, size_t colonne, int campo[righe][colonne], int stato[righe][colonne], time_t tempoInizio, int numero_mine, int bandiere_piazzate){
    time_t tempoTrascorso = difftime(time(NULL), tempoInizio);
    int minuti = tempoTrascorso / 60;
    int secondi = tempoTrascorso % 60;

    int bandiere_rimanenti = numero_mine - bandiere_piazzate;

    printf("TIMER: %02d:%02d | Bandiere rimanenti: %s%d%s\n\n", minuti, secondi, ROSSO_GRASSETTO, bandiere_rimanenti, RESET);

    // Intestazione colonne
    printf("   ");
    for(int j = 0; j < colonne; j++){
        printf("%2d ", j); // 2 spazi per ogni colonna
    }
    printf("\n");

    // Stampa il campo
    for(int i = 0; i < righe; i++){
        printf("%2d ", i); // Intestazione righe
        for(int j = 0; j < colonne; j++){
            // Colore alternato per effetto scacchiera
            const char* bg_color; // const perchè non cambia, char perchè è una stringa
            if((i + j) % 2 == 0){
                bg_color = BG_GRIGIO_CHIARO;
            } else {
                bg_color = BG_GRIGIO_SCURO;
            }
            if(stato[i][j] == COPERTA){
                printf("%s ■ %s", bg_color, RESET); // Cella coperta
            } else if(stato[i][j] == BANDIERA){
                printf("%s B %s", bg_color, RESET); // Bandiera
            } else {
                if(campo[i][j] == -1){
                    printf("%s * %s", ROSSO_GRASSETTO, RESET); // Mine in rosso grassetto
                } else if(campo[i][j] == 0){
                    printf("%s _ %s", bg_color, RESET); // Celle libere con sfondo alternato
                } else {
                    printf("%s %d %s", bg_color, campo[i][j], RESET);
                }
            }
        }
        printf("\n");
    }
}

// Scoprire tutte le celle vuote adiacenti (funzione ricorsiva)
void scopriCelleVuote(size_t righe, size_t colonne, int campo[righe][colonne], int stato[righe][colonne], size_t riga, size_t colonna){
    // Controlla i limiti del campo
    if (riga >= righe || colonna >= colonne || stato[riga][colonna] == SCOPERTA || stato[riga][colonna] == BANDIERA) {
        return;
    }
    
    stato[riga][colonna] = SCOPERTA; // Scopri la cella
    if (campo[riga][colonna] == 0) {
        // Scorri le celle adiacenti
        for (int k = -1; k <= 1; k++) { // da -1 a 1 per includere diagonali
            for (int w = -1; w <= 1; w++) { 
                size_t riga_adiacente = riga + k;
                size_t colonna_adiacente = colonna + w;

                // Evita di chiamare ricorsivamente la cella stessa
                if (k == 0 && w == 0) {
                    continue;
                }

                // Controlla i limiti del campo
                if (riga_adiacente < righe && colonna_adiacente < colonne) {
                    scopriCelleVuote(righe, colonne, campo, stato, riga_adiacente, colonna_adiacente);
                }
            }
        }
    }
}

bool verificaVittoria(size_t righe, size_t colonne, int campo[righe][colonne], int stato[righe][colonne]){
    for(size_t i = 0; i<righe; i++){
        for(size_t j = 0; j<colonne; j++){
            if(campo[i][j] != -1 && stato[i][j] != SCOPERTA){
                return false; // C'è ancora una cella non scoperta che non è una mina
            }
        }
    }
    return true; // Tutte le celle sicure sono state scoperte
}



void gioco(size_t righe, size_t colonne, int campo[righe][colonne], int mine){
    printf("Inizio del gioco!\n");

    int stato[righe][colonne];
    //Inizializza camop
    for(size_t i = 0; i<righe; i++){
        for(size_t j = 0; j<colonne; j++){
            stato[i][j] = COPERTA; // Inizializza tutte le celle come coperte
        }
    }

    time_t tempoInizio = time(NULL);
    int bandiere_piazzate = 0;
    bool gioco_attivo = true;

    while(gioco_attivo == true){

        mostraCampoGioco(righe, colonne, campo, stato, tempoInizio, mine, bandiere_piazzate);

        char comando;
        printf("\nCOMANDI DISPONIBILI:\n");
        printf("s - Scopri una cella\n");
        printf("b - Piazza/Rimuovi una bandiera\n");
        printf("q - Esci dal gioco\n");
        printf("Inserisci comando (s/b/q): ");
        scanf(" %c", &comando);
        if(comando == 'q'){
            printf("Uscita dal gioco.\n");
            break;
        }

        int riga, colonna;
        printf("Inserisci la riga: ");
        scanf("%d", &riga);
        printf("Inserisci la colonna: ");
        scanf("%d", &colonna);
        
        pulisciSchermo();
        
        // Controlla i limiti
        if(riga < 0 || riga >= righe || colonna < 0 || colonna >= colonne){
            printf("Coordinate fuori dai limiti. Riprova.\n");
            continue;
        }

        if(comando == 's'){
            if(stato[riga][colonna] == BANDIERA){
                printf("Cella con bandiera. Rimuovi la bandiera prima di scoprire.\n");
                continue;
            }
            if(campo[riga][colonna] == -1){
                // Scoperta mina - GAME OVER
                pulisciSchermo();
                printf("\n\n");
                printf("================= GAME OVER ================\n");
                printf("Hai scoperto una mina!\n");
                printf("=============================================\n\n");
                // Mostra tutto il campo
                for(size_t i = 0; i<righe; i++){
                    for(size_t j = 0; j<colonne; j++){
                        stato[i][j] = SCOPERTA;
                    }
                }
                mostraCampoGioco(righe, colonne, campo, stato, tempoInizio, mine, bandiere_piazzate);
                gioco_attivo = false;
                break;

            } else if(campo[riga][colonna] == 0){
                // Scopri celle vuote adiacenti
                scopriCelleVuote(righe, colonne, campo, stato, riga, colonna);
            } else {
                stato[riga][colonna] = SCOPERTA; // Scopri solo quella cella
            }
            // Verifica vittoria
            if(verificaVittoria(righe, colonne, campo, stato)){
                pulisciSchermo();
                mostraCampoGioco(righe, colonne, campo, stato, tempoInizio, mine, bandiere_piazzate);
                printf("\n\n================= VITTORIA ================\n");
                printf("Congratulazioni! Hai vinto la partita!\n");
                printf("=============================================\n");
                gioco_attivo = false;
                break;
            }
        }

        // BANDIERA
        else if(comando == 'b'){
            if(stato[riga][colonna] == SCOPERTA){
                pulisciSchermo();
                printf("Non puoi mettere bandiera su cella scoperta!\n\n");
                continue;
            }
            
            // Rimuovi bandiera
            if(stato[riga][colonna] == BANDIERA){
                stato[riga][colonna] = COPERTA;
                bandiere_piazzate--;
            }
            // Piazza bandiera
            else if(bandiere_piazzate < mine){
                stato[riga][colonna] = BANDIERA;
                bandiere_piazzate++;
            } else {
                pulisciSchermo();
                printf("Nessuna bandiera disponibile!\n\n");
            }
        }
        else {
            pulisciSchermo();

            printf("Comando non valido!\n\n");
        } 
    }
}


int main() {
    bool modalitaTest = false;

    // Inizializza rand
    srand(time(NULL));

    int scelta;
    do{
        menu();
        printf("Inserisci la tua scelta: ");
        scanf("%d", &scelta);

        size_t righe, colonne;
        int mine;

        switch(scelta){
            case 1:
                pulisciMenu();

                righe = RIGHE_PRINCIPIANTE;
                colonne = COLONNE_PRINCIPIANTE;
                mine = MINE_PRINCIPIANTE;
                break;
            case 2:
                pulisciMenu();

                righe = RIGHE_INTERMEDIO;
                colonne = COLONNE_INTERMEDIO;
                mine = MINE_INTERMEDIO;
                break;
            case 3:
                pulisciMenu();

                righe = RIGHE_ESPERTO;
                colonne = COLONNE_ESPERTO;
                mine = MINE_ESPERTO;
                break;
            case 4:
                pulisciMenu();
                modalitaTest = !modalitaTest; // Alterna modalità test
                printf("Modalità Test: %s\n\n", modalitaTest ? "ATTIVATA" : "DISATTIVATA");
                continue;
            case 5:
                pulisciMenu();
                tutorial();
                continue;
            case 0:
                printf("Uscita dal gioco.\n");
                break;
            default:
                pulisciSchermo();
                printf("Scelta non valida. Riprova.\n\n");
                continue;
            }
        if(scelta >= 1 && scelta <= 3){
            int campo[righe][colonne];
            inizializzaCampo(righe, colonne, campo);
            inserisciMine(righe, colonne, campo, mine);
            calcolaNumeri(righe, colonne, campo);
            
            if(modalitaTest){
                printf("\nModalità Test ATTIVA\n\n");
                stampaCampoTest(righe, colonne, campo);
            } else {
                printf("\n\n");
                gioco(righe, colonne, campo, mine);
                
            }
            printf("\n\n");
        }

    } while(scelta != 0);
    return 0;
}

// Alessandro Cundari