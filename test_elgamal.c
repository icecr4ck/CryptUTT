#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gmp.h"

#define COMPOSITE        0
#define PROBABLE_PRIME   1

int miller_rabin_pass(mpz_t a, mpz_t n) {
    int i, s, result;
    mpz_t a_to_power, d, n_minus_one;

    mpz_init(n_minus_one);
    mpz_sub_ui(n_minus_one, n, 1);

    s = 0;
	mpz_init_set(d, n_minus_one);
	while (mpz_even_p(d)) {
    	mpz_fdiv_q_2exp(d, d, 1);
    	s++;
	}

    mpz_init(a_to_power);
    mpz_powm(a_to_power, a, d, n);
    if (mpz_cmp_ui(a_to_power, 1) == 0)  {
        result=PROBABLE_PRIME; goto exit;
    }
    for(i=0; i < s-1; i++) {
        if (mpz_cmp(a_to_power, n_minus_one) == 0) {
            result=PROBABLE_PRIME; goto exit;
        }
        mpz_powm_ui (a_to_power, a_to_power, 2, n);
    }
    if (mpz_cmp(a_to_power, n_minus_one) == 0) {
        result=PROBABLE_PRIME; goto exit;
    }
    result = COMPOSITE;

exit:
    mpz_clear(a_to_power);
    mpz_clear(d);
    mpz_clear(n_minus_one);
    return result;
}

int miller_rabin(mpz_t n, gmp_randstate_t rand_state) {
    mpz_t a;
    int repeat;
    mpz_init(a);
    for(repeat=0; repeat<20; repeat++) {
        do {
            mpz_urandomm(a, rand_state, n);
        } while (mpz_sgn(a) == 0);
        if (miller_rabin_pass(a, n) == COMPOSITE) {
            return COMPOSITE;
        }
    }
    return PROBABLE_PRIME;
}

int creation_nbPremier (size_t max, mpz_t p) {  //On rentre en parametre la borne superieur que l'entier premier ne doit pas depasser
    mpz_init(p);
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    gmp_randseed_ui (rand_state, time(NULL));
    max=max-1;
    printf( "size = %u\n", max );

    
    do {
        mpz_init(p);
        while (max != mpz_sizeinbase(p,2))
        {
            mpz_urandomb(p,rand_state, max);
            mpz_out_str(stdout, 10, p);
        puts("");
        }
        if (mpz_even_p(p)) continue;
		if (mpz_fdiv_ui(p, 3) == 0) continue;
		if (mpz_fdiv_ui(p, 5) == 0) continue;
		if (mpz_fdiv_ui(p, 7) == 0) continue;
    } while (miller_rabin(p, rand_state) == COMPOSITE);
    mpz_out_str(stdout, 10, p);
    puts("");
    mpz_out_str(stdout, 2, p);
    puts("");
return 0;
}

int test_nbPremier (mpz_t nbATester) //On envoie le nombre a tester en parametre
{
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    gmp_randseed_ui (rand_state, time(NULL));
    puts(miller_rabin(nbATester, rand_state) == PROBABLE_PRIME ? "PRIME" : "COMPOSITE");
}

int test_generateur(mpz_t testGen, mpz_t taillegroupe)
{
    mpz_t resultat, puissance, temp;
    mpz_init(resultat); mpz_init(temp);
    mpz_init(puissance);  //permet de suivre l'iteration et la puissance à laquelle on est allé
    mpz_set(resultat,testGen);//resultat=testGen permet de garder la valeur de testGen
    mpz_add_ui(puissance,puissance,1);

    while (mpz_cmp_ui(resultat,1) !=0 )//|| mpz_cmp_ui(resultat,0) != 0)
    {
        mpz_mul(resultat, resultat, testGen); //resultat = resultat *nb
        mpz_mod(resultat, resultat, taillegroupe); //resultat = resultat mod taillegroupe
        mpz_add_ui(puissance,puissance,1); // on incrementer 
    }
    mpz_out_str(stdout, 10, puissance);
    puts(" puissance");
    mpz_out_str(stdout, 10, taillegroupe);
    puts(" taillegroupe");
    mpz_sub_ui(temp,taillegroupe,1);
    if (mpz_cmp(temp,puissance)==0)
    {
        printf("//////////////////////////////// générateur\n");
        return 0;
    }
    else
    {
        return 1;
    }
}

int gen_generateur (mpz_t taille_groupe, mpz_t elementGen) //la taille du groupe doit être un nombre premier
{
    int trouver=1,finwhile;
    mpz_init(elementGen); mpz_set_ui(elementGen,2);
    finwhile=mpz_cmp(taille_groupe,elementGen);//retourne 1 si taillegroupe >i, si taillegroupe=i on retourne 0
    while (finwhile && trouver) //mpz_cmp
    {
        printf("finwhile %d  trouver %d\n",finwhile, trouver );
        //-------
        printf("/!\\  test nouveau générateur /!\\\n");
        mpz_out_str(stdout, 10, elementGen);
        puts("");
        trouver=test_generateur(elementGen,taille_groupe);
        mpz_add_ui(elementGen,elementGen,1);//ajoute 1 à i
        finwhile=mpz_cmp(taille_groupe,elementGen); //compare i et taillegroupe
    }
    printf("Nombre générateur : ");
    mpz_sub_ui(elementGen,elementGen,1); //dans le while on a rajouté 1 a l'élement générateur trouvé. On rééquilibre ici en enlevant 1.
    mpz_out_str(stdout, 10, elementGen);
    puts("");
}

void generation_nbAlatoire (mpz_t borne, mpz_t resultat) //On rentre la borne de l'ensemble, souvent c'est p
{
    mpz_init(resultat); 
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    gmp_randseed_ui (rand_state, time(NULL));
    mpz_urandomm(resultat,rand_state, borne);
    mpz_out_str(stdout, 10, resultat);
    puts("");

}

void puissance_mpz(mpz_t reponse, mpz_t nb, mpz_t puissance)
{
	mpz_sub_ui(puissance,puissance,1);
	mpz_set(reponse,nb);
	while (mpz_cmp_ui(puissance,0) !=0 )
	{
		mpz_mul(reponse, reponse, nb);
		mpz_sub_ui(puissance,puissance,1);
	}
}

void generation_clefPrivee ()
{
    mpz_t p,borne,generateur,x,A;
    mpz_init(p); mpz_init(borne); mpz_init(generateur); mpz_init(x); mpz_init(A);
    size_t b=15;
    
    creation_nbPremier(b,p);
    gen_generateur(p, generateur) ;
    generation_nbAlatoire(p,x);
    mpz_powm (A,generateur,x,p);

    //On écrit dans un fichier
    FILE* cleSecrete = NULL ;
    cleSecrete = fopen("cleP.txt", "w");
    if (cleSecrete != NULL)
    {
    	fputs("p : ",cleSecrete);
    	mpz_out_str(cleSecrete, 10, p);
    	fputs("\ngenerateur : ",cleSecrete);
    	mpz_out_str(cleSecrete, 10, generateur);
    	fputs("\nA : ",cleSecrete);
    	mpz_out_str(cleSecrete, 10, A);
    	fputs("\nx : ",cleSecrete);
    	mpz_out_str(cleSecrete, 10, x);
    	fputs("\n",cleSecrete);
		printf("Clé secrète enregistrée\n");
		fclose(cleSecrete);
	}
    else
    {
        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier cleP.txt\n");
    }
}

void chiffrementElgamal()
{
	//Ici on récupere h, p et generateur
	generation_nbAlatoire(p,k); //on genere un k
	mpz_powm (C1,generateur,k,p); //On calcul C1 = generateur^k mod p
	mpz_powm (y,h,k,p);//On calcul le secret partagé y=g^k mod p
	//Ici on chiffre en multipliant les messages par y
	//On inscrit dans le fichier chiffré : C1 et à la ligne m*y
}

void dechiffrementElgamal ()
{
	mpz_t quotient, C2, C1;
	mpz_init(quotient); mpz_init(C2); mpz_init(C1);
	//Ici on récuere x et p
	//Ici on lit le fichier crypté et on récupérere C1 et C2
	mpz_powm (C1,C1,x,p)
	mpz_cdiv_q(quotient, C2, C1);
}

int main(int argc, char *argv[])
{
    // mpz_t testPrimalite, borne, z ,gen;
    // size_t b=15;
    // mpz_init(testPrimalite); mpz_init(borne), mpz_init(z); mpz_init(gen);
    // mpz_set_str(testPrimalite,"7311862289987099202981511384312299669757192756891208283874145736575923",10);
    // mpz_set_str(borne,"97",10);
    // test_nbPremier (testPrimalite);
    // printf("------ Création nb premier -------\n");
    // creation_nbPremier(b,z);
    // printf("------ Génération générateur -------\n");
    // gen_generateur(borne, gen);
    // printf("------ Génération nb alatoire -------\n");
    // generation_nbAlatoire(borne,z);
	generation_clefPrivee();
	return 0;

}

//  Source test primalité et generation de nb premier :
//  http://en.literateprograms.org/Miller-Rabin_primality_test_%28C%29

//https://openclassrooms.com/courses/la-cryptographie-asymetrique-rsa/comment-utiliser-gmp