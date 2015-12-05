#include <stdio.h>
#include <stdlib.h>
#include "gmp-6.0.0/gmp.h"
#include <string.h>
#include <math.h>
#include <sys/stat.h>

int affichageMenu()
{
    int choixMenu = 0;
    printf("--> Sélectionner votre fonction de chiffrement <--\n");
    printf("1.Chiffrement AES\n");
    printf("2.Chiffrement El-Gamal\n");
    printf("3.Signature El-Gamal\n");
    printf("\nVotre choix?\n");
    scanf("%d", &choixMenu);
    return choixMenu;
}

void addRoundKey(unsigned char **state, unsigned char *cle)
{
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            state[i][j] = state[i][j] ^ cle[i*4+j];
        }
    }
}

void subBytes(unsigned char **state)
{
    unsigned char sbytes[256] =
    {
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
     };
     for (int i = 0;i < 4; i++){
        for (int j = 0; j < 4; j++){
            state[i][j]=sbytes[state[i][j]];
        }
    }
}

void mixColumns(unsigned char **state)
{
    for (int i = 0; i < 4; ++i)
    {
        int a=0;
        int temp=0;
        temp = (2*state[0][i]) ^ (3*state[1][i]) ^ state[2][i] ^ state[3][i];
        if (temp>=1024) // (1024) dec = 100 0000 0000binaire
        {
            temp=temp ^ 108; //108dec = 110 1100bin
            temp=temp-1024;
        }
        if(temp>=512)
        {
            temp= temp ^ 54;//54dec = 11 0110
            temp=temp-512;
        }
        if (temp>=256)
        {
            temp=temp ^ 27;//27dec = 1 1011
            temp=temp-256;
        }
        state[0][i]=(char)temp;
        temp = state[0][i] ^ (2*state[1][i]) ^ (3*state[2][i]) ^ state[3][i];
        if (temp>=1024) // (1024) dec = 100 0000 0000binaire
        {
            temp=temp ^ 108; //108dec = 110 1100bin
            temp=temp-1024;
        }
        if(temp>=512)
        {
            temp= temp ^ 54;//54dec = 11 0110
            temp=temp-512;
        }
        if (temp>=256)
        {
            temp=temp ^ 27;//27dec = 1 1011
            temp=temp-256;
        }
        state[1][i]=(char)temp;
        temp = state[0][i] ^ state[1][i] ^ (2*state[2][i]) ^ (3*state[3][i]);
        if (temp>=1024) // (1024) dec = 100 0000 0000binaire
        {
            temp=temp ^ 108; //108dec = 110 1100bin
            temp=temp-1024;
        }
        if(temp>=512)
        {
            temp= temp ^ 54;//54dec = 11 0110
            temp=temp-512;
        }
        if (temp>=256)
        {
            temp=temp ^ 27;//27dec = 1 1011
            temp=temp-256;
        }
        state[2][i]=(char)temp;
        temp = (3*state[0][i]) ^ state[1][i] ^ state[2][i] ^ (2*state[3][i]);
        if (temp>=1024) // (1024) dec = 100 0000 0000binaire
        {
            temp=temp ^ 108; //108dec = 110 1100bin
            temp=temp-1024;
        }
        if(temp>=512)
        {
            temp= temp ^ 54;//54dec = 11 0110
            temp=temp-512;
        }
        if (temp>=256)
        {
            temp=temp ^ 27;//27dec = 1 1011
            temp=temp-256;
        }
        state[3][i]=(char)temp;
    }
}

void shiftRows(unsigned char **state)
{
    unsigned char temp='0';
    for (int i = 0; i < 4; ++i) //Pour chaque ligne
    {
        for (int j = i; j > 0; j--){ //On effectue un nombre de décalage vers la droite du tableau correspondant à la ligne
            temp=state[i][0];
            state[i][0]=state[i][1];
            state[i][1]=state[i][2];
            state[i][2]=state[i][3];
            state[i][3]=temp;
        }
    }
}

void keyExpansion(unsigned char *cle)
{
    unsigned char cleChar[16];
    unsigned char cle2D[4][4];
    for (int i = 0; i < 16 ; ++i)
    {
        cleChar[i] = cle[i];
    }
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            cle2D[i][j] = cleChar[i*4+j];
        }
    }
    strcpy(cle2D, subBytes(cle2D));
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            cle[i*4+j] = cle2D[i][j];
        }
    }
}

unsigned char * creationCle(int tailleCle)
{
    unsigned long mot_passe;
    printf(" \n --> Entrez votre clé (numérique) pour chiffrer <-- ");
    scanf("%lu", &mot_passe);
    srand(mot_passe);
    tailleCle=sqrt(tailleCle/8);
    unsigned char cle[tailleCle][tailleCle];
    for (int i = 0; i < tailleCle ; i++ )
        {
        for (int j = 0; j < tailleCle ; ++j)
        {
            cle[i][j] = rand()%2;
            cle[i][j] = (char)cle[i][j];
        }
    }
    return cle;
}

void chiffrementAES128(unsigned char *cle)
{
     FILE* fichier = NULL;
     FILE* cipher = NULL;
     fichier = fopen ("test.txt", "r");
     cipher = fopen ("cipher.txt", "w");
     unsigned char temp = '0';
     unsigned char state[4][4];
     int Nr = 10;
     if (fichier == NULL){
        printf ("Erreur dans l'ouverture du fichier !\n");
     }
     else{
        //On récupère le tableau qu'on va mettre dans la round
        while (temp != EOF)
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    state[i][j] = 0;
                }
            }
            int k = 0;
            int l = 0;
            while (temp != EOF && k<4)
            {
                while (temp != EOF && l<4)
                {
                    temp = fgetc(fichier);
                    if (temp != EOF)
                    {
                        state[k][l] = temp;
                    }
                    l++;
                }
                k++;
            }
            addRoundKey(state, cle);
            for (int i=0; i<Nr; i++ )
            {
                subBytes(state);
                shiftRows(state);
                mixColumns(state);
                keyExpansion(cle);
                addRoundKey(state, cle);
            }
            subBytes(state);
            shiftRows(state);
            keyExpansion(cle);
            addRoundKey(state, cle);
            if (cipher == NULL){
                 printf ("Erreur dans l'ouverture du fichier cipher !\n");
            }
            else{
                for (int i=0; i<4; i++)
                {
                     for (int j=0; j<4; j++)
                     {
                         fputc(state[i][j], cipher);
                     }
                }
            }
        }
        fclose (cipher);
        fclose (fichier);
     }
}

void chiffrementAES196(int *cle)
{

}

void chiffrementAES256(int *cle)
{

}

void chiffrementAES()
{
    int choixMenu;
    int *cle;
    printf("Vous avez choisi le chiffrement AES !\n\n");
    printf("--> Sélectionner la taille de clé <--\n");
    printf("1.128 bits\n");
    printf("2.196 bits\n");
    printf("3.256 bits\n");
    scanf("%d", &choixMenu);
    switch (choixMenu)
    {
    case 1:
        cle = creationCle(128);
        chiffrementAES128(cle);
        break;
    case 2:
        cle = creationCle(196);
        chiffrementAES196(cle);
        break;
    case 3:
        cle = creationCle(256);
        chiffrementAES256(cle);
        break;
    default:
        printf("Erreur, taille de clé incorrecte !\n\n");
        exit(1);
        break;
    }

}

void chiffrementElGamal()
{
    printf("Vous avez choisi le chiffrement El-Gamal !\n\n");
}

void signatureElGamal()
{
    printf("Vous avez choisi la signature El-Gamal !\n\n");
}

int main(int argc, char *argv[])
{
    switch(affichageMenu())
    {
        case 1:
            chiffrementAES();
            break;
        case 2:
            chiffrementElGamal();
            break;
        case 3:
            signatureElGamal();
            break;
        default:
            printf("Erreur, choix non valide !\n\n");
            break;
    }
  	return 0;
}

void test_rabinMiller(mpz_t nbATester){
    int rep;
    mpz_t resultat, a, b;
    mpz_init(resultat);
    mpz_init(a);
    mpz_init(b);
    mpz_init_set_str(a, "2", 10);
    mpz_init_set_str(b, "1", 10);
    mpz_mod(resultat, nbATester,  a); //resultat=1 si nbatester impair sinon resultat=0
    rep= mpz_cmp(b,resultat);//si b=resultat, rep=0. Si b>resultat, rep=1.
    if (rep==1)
    {
        gmp_printf ("Nombre pair %Zd\n", "here", nbATester);
    }
    mpz_clear(resultat);
    mpz_clear(a);
    mpz_clear(b);
}
