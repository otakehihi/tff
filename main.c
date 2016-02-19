/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Jo
 *
 * Created on 19 février 2016, 09:23
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

typedef struct dirent dirent;

/*
Liste dynamique pour un répertoire
fichiers : pointeur vers un tableau d'entités de repertoire
chemin : string du chemin du repertoire
capacité : nombre d'entités contenu dans le repertoire
*/


struct listeFichiers {
    dirent** fichiers;
    char* chemin;
    int capacite;
    int curseur;
};
typedef struct listeFichiers listeFichiers;



listeFichiers* newListeFichiers();
void ajouteFichier(listeFichiers*, dirent*);
void listerFichiers(listeFichiers*,char* chemin);
void affListeFichiers(listeFichiers*);
void affListeFichiersRecursif(listeFichiers*);
void freeListe(listeFichiers*);
char* concat(char*,char*);
char* concatDoss(char*,char*);
char* concatChar(char*,char);
void findChaineFic(char*,char*);
void findChaineRep(char*,char*);
void findChaineRepRecursif(char*, char*);
void findChaineFicname(char*, char*);
void findChaineFicnameRecursif(char*, char*);


int main(int argc, char** argv) {
    char* chemin;
    chemin="C:/Users/Jo/Desktop/";
    /*listeFichiers* liste = newListeFichiers();
    listerFichiers(liste,chemin);
    affListeFichiers(liste);*/

    findChaineFicnameRecursif("C:/Users/Jo/Desktop/",".txt");

    //freeListe(liste);
    return 0;
}


/*
Demande une liste vide et le chemin d'un répertoire
Remplis la liste avec les entités trouvés dans le chemin
*/
void listerFichiers(listeFichiers* liste,char* chemin) {
    liste->chemin=malloc(sizeof(char)*strlen(chemin));
    liste->chemin=chemin;

    DIR* rep = NULL;


    rep = opendir(liste->chemin);
    seekdir(rep,2);
    dirent* fichierLu=readdir(rep);

    while (fichierLu != NULL)
    {
        ajouteFichier(liste,fichierLu);
        fichierLu=readdir(rep);
    }
}

/*
Crée une liste vide
*/
listeFichiers* newListeFichiers() {
    listeFichiers* liste = malloc(sizeof(listeFichiers));
    liste->capacite=0;
    liste->curseur=0;
    liste->fichiers=malloc(sizeof(dirent*));

    return liste;
}

/*
Demande une liste et une entité
Ajoute l'entité à la liste
*/
void ajouteFichier(listeFichiers* liste, dirent* fichier) {
    liste->capacite=liste->capacite+1;
    liste->fichiers=realloc(liste->fichiers,sizeof(dirent*)*(liste->capacite));
    liste->fichiers[liste->curseur]=malloc(sizeof(dirent));


    *liste->fichiers[liste->curseur]=*fichier;
    liste->curseur=liste->curseur+1;
}

/*
Demande une liste
Affiche la liste
*/
void affListeFichiers(listeFichiers* liste) {
    int i;
    int taille=liste->capacite;
    for(i=0;i<taille;i++) {
        if(liste->fichiers[i]->d_type==0) {
            char* chemin = liste->chemin;
            char* nom = liste->fichiers[i]->d_name;
            char* conc = concat(chemin,nom);
            printf("Fichier : %s\n",conc);
            free(conc);
        }
        if(liste->fichiers[i]->d_type==16) {
            char* chemin = liste->chemin;
            char* nom = liste->fichiers[i]->d_name;
            char* conc = concatDoss(chemin,nom);
            printf("Dossier : %s\n",conc);
            free(conc);
        }
    }
}

/*
Libere la liste
*/
void freeListe(listeFichiers* liste) {
    int i;
    for(i=0;i<liste->capacite;i++) {
        free(liste->fichiers[i]);
    }
    free(liste->fichiers);
    free(liste);
}

/*
Demande une liste
Affiche toutes les entités recursivement
*/
void affListeFichiersRecursif(listeFichiers* liste) {
    int i;
    int taille=liste->capacite;
    for(i=0;i<taille;i++) {

        if(liste->fichiers[i]->d_type==0) {
            char* chemin = liste->chemin;
            char* nom = liste->fichiers[i]->d_name;
            char* conc = concat(chemin,nom);
            printf("Fichier : %s\n",conc);
            free(conc);
        }
        if(liste->fichiers[i]->d_type==16) {
            char* chemin = liste->chemin;
            char* nom = liste->fichiers[i]->d_name;
            char* conc = concatDoss(chemin,nom);
            printf("Dossier : %s\n",conc);
            listeFichiers* liste2 = newListeFichiers();
            listerFichiers(liste2,conc);
            affListeFichiersRecursif(liste2);
            freeListe(liste2);
            free(conc);
        }
    }
}

/*
Demande deux chaines
Renvoie une chaine concaténée des deux
*/
char* concat(char* a,char* b) {
    char* res=malloc(sizeof(char)*(strlen(a)+strlen(b)+1));
    int i=0;
    int j=0;
    for(i=0;i<strlen(a);i++) {
        res[i]=a[i];
    }
    for(j=0;j<strlen(b)+1;j++) {
        res[i]=b[j];
        i++;
    }
    res[strlen(a)+strlen(b)]='\0';
    return res;
}

/*
Demande une chaine et un caractère
Concatène le caractère à la chaine et renvoie la nouvelle chaine
*/
char* concatChar(char* a,char b) {
    char* res = malloc(sizeof(char)*(strlen(a)+2));
    int i;
    for(i=0;i<strlen(a);i++) {
        res[i]=a[i];
    }
    res[strlen(a)]=b;
    res[strlen(a)+1]='\0';
    return res;
}

/*
Demande deux chaines
Concatène les deux chaines et ajoute un / à la fin
*/
char* concatDoss(char* a,char* b) {
    char* res=malloc(sizeof(char)*(strlen(a)+strlen(b)+2));
    int i=0;
    int j=0;
    for(i=0;i<strlen(a);i++) {
        res[i]=a[i];
    }
    for(j=0;j<strlen(b)+1;j++) {
        res[i]=b[j];
        i++;
    }
    res[strlen(a)+strlen(b)]='/';
    res[strlen(a)+strlen(b)+1]='\0';
    return res;
}

/*
Demande un chemin vers un fichier et une chaine
Cherche la chaine dans le fichier et affiche toutes les lignes contenant la chaine
*/
void findChaineFic(char* fic, char* chaine) {
    FILE* f = NULL;
    f=fopen(fic,"r");
    if(f != NULL) {
        printf("\n----------- %s\n",fic);
        char ch[256];
        while(fgets(ch,256,f)!=NULL){
            if (strstr(ch,chaine)!=NULL) {
                printf("%s",ch);
            }
        }
        printf("\n");
    }
    else {
        printf("Fichier non valide");
    }
    fclose(f);
}

/*
Demande un chemin vers un repertoire et une chaine
Cherche parmis tous les fichiers de ce repertoire et affiche les lignes contenant la chaine
*/
void findChaineRep(char* chemin, char* chaine) {
    listeFichiers* liste = newListeFichiers();
    listerFichiers(liste,chemin);
    int i;
    char* chemin2;
    char* nom;
    char* conc;
    for(i=0;i<liste->capacite;i++) {
        if(liste->fichiers[i]->d_type==0) {
            chemin2 = liste->chemin;
            nom = liste->fichiers[i]->d_name;
            conc = concat(chemin,nom);
            //printf("%s\n",conc);
            findChaineFic(conc,chaine);
            free(conc);
        }
    }
    freeListe(liste);
}

/*
Demande un chemin vers un repertoire et une chaine
Cherche parmis tous les fichiers de ce repertoire et affiche récursivement les lignes contenant la chaine
*/
void findChaineRepRecursif(char* chemin, char* chaine) {
    listeFichiers* liste = newListeFichiers();
    listerFichiers(liste,chemin);
    int i;
    char* chemin2;
    char* nom;
    char* conc;
    for(i=0;i<liste->capacite;i++) {
        if(liste->fichiers[i]->d_type==0) {
            chemin2 = liste->chemin;
            nom = liste->fichiers[i]->d_name;
            conc = concat(chemin,nom);
            //printf("%s\n",conc);
            findChaineFic(conc,chaine);
            free(conc);
        }
        else if(liste->fichiers[i]->d_type==16){
            findChaineRepRecursif(concatDoss(liste->chemin,liste->fichiers[i]->d_name),chaine);
        }
    }
    freeListe(liste);
}

void findChaineFicname(char* chemin, char* chaine) {
    listeFichiers* liste = newListeFichiers();
    listerFichiers(liste,chemin);
    int i;
    char* chemin2;
    char* nom;
    char* conc;
    for(i=0;i<liste->capacite;i++) {
        if(liste->fichiers[i]->d_type==0) {
            if (strstr(liste->fichiers[i]->d_name,chaine)!=NULL) {
                printf("%s\n",concat(liste->chemin,liste->fichiers[i]->d_name));
            }
        }
    }
    freeListe(liste);
}

void findChaineFicnameRecursif(char* chemin, char* chaine) {
    listeFichiers* liste = newListeFichiers();
    listerFichiers(liste,chemin);
    int i;
    char* chemin2;
    char* nom;
    char* conc;
    for(i=0;i<liste->capacite;i++) {
        if(liste->fichiers[i]->d_type==0) {
            if (strstr(liste->fichiers[i]->d_name,chaine)!=NULL) {
                printf("%s\n",concat(liste->chemin,liste->fichiers[i]->d_name));
            }
        }
        else if (liste->fichiers[i]->d_type==16) {
            char* res = concatDoss(liste->chemin,liste->fichiers[i]->d_name);
            findChaineFicnameRecursif(res,chaine);
            free(res);
        }
    }
    freeListe(liste);
}