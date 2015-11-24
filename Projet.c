#include <stdio.h>
#include <stdlib.h>

int affichageMenu()
{ 
     int choixMenu = 0;
     printf("---Sélectionner votre fonction de chiffrement---\n");
     printf("1.Chiffrement AES\n");
     printf("2.Chiffrement El-Gamal\n");
     printf("3.Signature El-Gamal\n\n\n");
     printf("\nVotre choix?\n\n");
     scanf("%d", &choixMenu);
     return choixMenu;
}

int main(int argc, char *argv[])
{
    switch(affichageMenu())
    {
                     case 1:
                          printf("Vous avez choisis le chiffrement AES!");
                          break;
                     case 2:
                          printf("Vous avez choisis le chiffrement El-Gamal!");
                          break;
                     case 3:
                          printf("Vous avez choisis la signature El-Gamal!");
                          break;    
                     default:
                             printf("Choix non valide!");
                             break;
    }
     
  	system("PAUSE");  
  	return 0;
}