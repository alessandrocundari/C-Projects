#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Tutte le tessere del gioco
    int tessere[28][2];
    int manoGiocatore[7][2];
    int manoComputer[7][2];
    int tavolo[100][2];
    int numTessereTavolo = 0;
    
    srand(time(NULL));
    
    // Crea tutte le tessere da [0|0] a [6|6]
    int indice = 0;
    for (int a = 0; a <= 6; a++) {
        for (int b = a; b <= 6; b++) {
            tessere[indice][0] = a;
            tessere[indice][1] = b;
            indice++;
        }
    }
    
    // Mescola le tessere (scambia ogni tessera con una casuale)
    for (int i = 0; i < 28; i++) {
        int j = rand() % 28;
        int valoretemporaneo0 = tessere[i][0];
        int valoretemporaneo1 = tessere[i][1];
        tessere[i][0] = tessere[j][0];
        tessere[i][1] = tessere[j][1];
        tessere[j][0] = valoretemporaneo0;
        tessere[j][1] = valoretemporaneo1;
    }
    
    // Distribuisci 7 tessere al giocatore
    for (int i = 0; i < 7; i++) {
        manoGiocatore[i][0] = tessere[i][0];
        manoGiocatore[i][1] = tessere[i][1];
    }
    
    // Distribuisci 7 tessere al computer
    for (int i = 0; i < 7; i++) {
        manoComputer[i][0] = tessere[7 + i][0];
        manoComputer[i][1] = tessere[7 + i][1];
    }
    
    // La prima tessera sul tavolo
    tavolo[0][0] = tessere[14][0];
    tavolo[0][1] = tessere[14][1];
    numTessereTavolo = 1;
    
    int turnoGiocatore = 1; // 1 = giocatore, 0 = computer
    int passaGiocatore = 0;
    int passaComputer = 0;
    
    printf("\nBenvenuto al gioco del Domino!\n");

    // Gioco principale
    while (1) {
        int sinistro = tavolo[0][0];
        int destro = tavolo[numTessereTavolo - 1][1];
        
        // Controlla se qualcuno ha finito le tessere
        int tesseraGiocatore = 0;
        int tesseraComputer = 0;
        for (int i = 0; i < 7; i++) {
            if (manoGiocatore[i][0] != -1) tesseraGiocatore++;
            if (manoComputer[i][0] != -1) tesseraComputer++;
        }
        
        if (tesseraGiocatore == 0) {
            printf("\n*** HAI VINTO! Hai finito tutte le tessere! ***\n");
            break;
        }
        if (tesseraComputer == 0) {
            printf("\n*** HA VINTO IL COMPUTER! ***\n");
            break;
        }
        
        // Se entrambi hanno passato, gioco bloccato
        if (passaGiocatore && passaComputer) {
            printf("\n*** GIOCO BLOCCATO! ***\n");
            break;
        }
        
        // TURNO DEL GIOCATORE
        if (turnoGiocatore) {
            printf("\n========================================\n");
            printf("TAVOLO: ");
            for (int i = 0; i < numTessereTavolo; i++) {
                printf("[%d|%d] ", tavolo[i][0], tavolo[i][1]);
            }
            printf("\n(Estremi: SINISTRA = %d, DESTRA = %d)\n", sinistro, destro);
            
            printf("\nLA TUA MANO:\n");
            for (int i = 0; i < 7; i++) {
                if (manoGiocatore[i][0] != -1) {
                    printf("  %d: [%d|%d]\n", i, manoGiocatore[i][0], manoGiocatore[i][1]);
                }
            }
            
            printf("\nScegli una tessera (0-6) o -1 per passare: ");
            int scelta;
            scanf("%d", &scelta);
            
            // Passa il turno
            if (scelta == -1) {
                printf("Hai passato il turno.\n");
                passaGiocatore = 1;
                turnoGiocatore = 0;
                continue;
            }
            
            // Controlla se la scelta è valida
            if (scelta < 0 || scelta >= 7 || manoGiocatore[scelta][0] == -1) {
                printf("Scelta non valida! Riprova.\n");
                continue;
            }
            
            int val0 = manoGiocatore[scelta][0];
            int val1 = manoGiocatore[scelta][1];
            int giocata = 0;
            
            // Prova a giocare a destra
            if (val0 == destro) {
                tavolo[numTessereTavolo][0] = val0;
                tavolo[numTessereTavolo][1] = val1;
                numTessereTavolo++;
                giocata = 1;
            } else if (val1 == destro) {
                tavolo[numTessereTavolo][0] = val1;
                tavolo[numTessereTavolo][1] = val0;
                numTessereTavolo++;
                giocata = 1;
            }
            // Prova a giocare a sinistra
            else if (val1 == sinistro) {
                // Sposta tutto a destra
                for (int i = numTessereTavolo; i > 0; i--) {
                    tavolo[i][0] = tavolo[i-1][0];
                    tavolo[i][1] = tavolo[i-1][1];
                }
                tavolo[0][0] = val0;
                tavolo[0][1] = val1;
                numTessereTavolo++;
                giocata = 1;
            } else if (val0 == sinistro) {
                // Sposta tutto a destra
                for (int i = numTessereTavolo; i > 0; i--) {
                    tavolo[i][0] = tavolo[i-1][0];
                    tavolo[i][1] = tavolo[i-1][1];
                }
                tavolo[0][0] = val1;
                tavolo[0][1] = val0;
                numTessereTavolo++;
                giocata = 1;
            }
            
            if (giocata) {
                manoGiocatore[scelta][0] = -1; // Rimuovi la tessera
                passaGiocatore = 0; // Reset del passa
                printf("Hai giocato [%d|%d]!\n", val0, val1);
            } else {
                printf("Non puoi giocare questa tessera! Riprova.\n");
                continue;
            }
            
            turnoGiocatore = 0;
        }
        // TURNO DEL COMPUTER
        else {
            printf("\nTurno del computer...\n");
            int mossaTrovata = 0;
            
            for (int i = 0; i < 7; i++) {
                if (manoComputer[i][0] == -1) continue;
                
                int val0 = manoComputer[i][0];
                int val1 = manoComputer[i][1];
                
                // Prova a destra
                if (val0 == destro) {
                    tavolo[numTessereTavolo][0] = val0;
                    tavolo[numTessereTavolo][1] = val1;
                    numTessereTavolo++;
                    manoComputer[i][0] = -1;
                    printf("Il computer ha giocato [%d|%d]\n", val0, val1);
                    mossaTrovata = 1;
                    break;
                } else if (val1 == destro) {
                    tavolo[numTessereTavolo][0] = val1;
                    tavolo[numTessereTavolo][1] = val0;
                    numTessereTavolo++;
                    manoComputer[i][0] = -1;
                    printf("Il computer ha giocato [%d|%d]\n", val0, val1);
                    mossaTrovata = 1;
                    break;
                }
                // Prova a sinistra
                else if (val1 == sinistro) {
                    for (int j = numTessereTavolo; j > 0; j--) {
                        tavolo[j][0] = tavolo[j-1][0];
                        tavolo[j][1] = tavolo[j-1][1];
                    }
                    tavolo[0][0] = val0;
                    tavolo[0][1] = val1;
                    numTessereTavolo++;
                    manoComputer[i][0] = -1;
                    printf("Il computer ha giocato [%d|%d]\n", val0, val1);
                    mossaTrovata = 1;
                    break;
                } else if (val0 == sinistro) {
                    for (int j = numTessereTavolo; j > 0; j--) {
                        tavolo[j][0] = tavolo[j-1][0];
                        tavolo[j][1] = tavolo[j-1][1];
                    }
                    tavolo[0][0] = val1;
                    tavolo[0][1] = val0;
                    numTessereTavolo++;
                    manoComputer[i][0] = -1;
                    printf("Il computer ha giocato [%d|%d]\n", val0, val1);
                    mossaTrovata = 1;
                    break;
                }
            }
            
            if (mossaTrovata) {
                passaComputer = 0;
            } else {
                printf("Il computer passa il turno.\n");
                passaComputer = 1;
            }
            
            turnoGiocatore = 1;
        }
    }
    
    printf("\nGrazie per aver giocato!\n");

    char risposta;
    
    printf("Vuoi rigiocare? (s/n): ");
    scanf(" %c", &risposta);
    if (risposta == 's' || risposta == 'S') {
        main(); // Riavvia il gioco
    }
    else {
        printf("Ciao!\n");
    }
    return 0;
}