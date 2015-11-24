#include <stdio.h>
#include <stdlib.h>
#include "gmp-6.0.0/gmp.h"

int affichageMenu()
{ 
    int choixMenu = 0;
    printf("---Sélectionner votre fonction de chiffrement---\n");
    printf("1.Chiffrement AES\n");
    printf("2.Chiffrement El-Gamal\n");
    printf("3.Signature El-Gamal\n");
    printf("\nVotre choix?\n");
    scanf("%d", &choixMenu);
    return choixMenu;
}

void chiffrementAES()
{
    printf("Vous avez choisi le chiffrement AES!\n\n");
}

void chiffrementElGamal()
{
    printf("Vous avez choisi le chiffrement El-Gamal!\n\n");
}

void signatureElGamal()
{
    printf("Vous avez choisi la signature El-Gamal!\n\n");
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
            printf("Choix non valide!\n\n");
            break;
    }
    printf("test\n");
  	return 0;
}