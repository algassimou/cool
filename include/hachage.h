#ifndef __HACHAGE__
#define __HACHAGE__

#include <stdint.h>

#include "dliste.h"
#include "liste.h"
#include "couple.h"

/**
 * Definition du type de la table de hachage 
 * celle ci est un tableau de liste (car les collisions sont gérer en chainant 
 * les différents couple qui sont entrés en collision
 */

struct _HashTable {
  DListe **table ;
  Liste *liste_listes_collision ;
  int size ; // la taille du tableau>
  int count ;// le nombre de couple contenu dans le tableau 
  /* 
   * fonction de comapraison des clés
   * renvoi :
   * 0 si les deux clés sont égales
   * 1 sinon 
   */
  int (*compare) (const void *, const void *); 
  uint32_t (*hash_function) (const void *);
} ;

#define HASH_TABLE_SIZE(h) ((h)->size)
#define HASH_TABLE_COUNT(h) ((h)->count)
#define HASH_TABLE_SIZE_MAX 1024

typedef struct _HashTable HashTable ;

extern HashTable *hash_table_create () ;

/**
 * initialise une table de hachage
 * \param h la table de hachage 
 * \param hash la donction de hachage utilisée
 * \param cmp fonction de comparaison
 */
int hash_table_init (HashTable *h, 
		    uint32_t (*hash) (const void *),
		    int (*cmp) (const void *, const void *));

/**
 * ajoute une valeur dans la table de hachage 
 * \param h la table de hachage 
 * \param cle la cle de la valeur 
 * \param val la valeur

 * remarque : si une valeur est déjà associé à la clé alors on aura
 * deux entré pour une même clé et la valeur renvoie par hashtable_search est la 
 * première entré
 */
int hash_table_add (HashTable *h, void *cle, void *val);

/**
 * ajoute une valeur dans la table de hachage 
 * \param h la table de hachage 
 * \param cle la cle de la valeur 
 * \param val la valeur
 * \param l'ancienne valeur associé à la clé
 * \result 0 on ajouter une val 1 on a changer une valeur déjà presente -1 erreur
 * remarque : si une valeur est déjà associé à la clé alors celle ci est 
 * renplacé par la nouvelle et elle est sauvergardé dans old_val 
 * (si old_val est different de null)
 */
int hash_table_put (HashTable *h, void *cle, void *val, void **old_val);


/**
 * recherche la valeur d'une cle dans une table de hachage
 * \param h la table de hachage 
 * \param cle la cle
 * \result renvoie le couple formé par la cle et la valeur si la cle est 
 * presente dans la table sinon renvoie NULL
 */
Couple *hash_table_search (HashTable *h, void *cle) ;

/**
 * supprime une valeur de la table
 * \param h la table de hachage
 * \param cle la cle de la valeur
 * \param data la donnée associé à la clé
 * \result renvoie  si tous c'est bien passé 
 */
int hash_table_rm (HashTable *h, void *cle, void **data) ;

/**
 * detruit la table de hachage ainsi toutes les clés et valeur quelle contient
 * les fonctions destroy_cle et destroy_val sont utilisées pour liberer 
 * l'espace mémoire occupé par les clés et les valeurs
 * \param h la table de hachage
 * \param destroy_cle fonction pour liberer l'espace mémoire occupé par une clé
 * \param destroy_val fonction pour liberer l'espace mémoire occupé par une valeur
 */
int hash_table_destroy(HashTable *h, 
		      void (*destroy_cle) (void*), 
		      void (*destroy_val) (void*)
		     );

#endif 
