#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include "DataIndividu.h"

using namespace std;

class Data {
public:
    map<string, vector<string>> cat; // Modalités uniques pour les variables qualitatives
    int nbrVar;                      // Nombre total de variables
    int indVarY;//// Indice de la colonne de la variable cible Y

    vector<string> TypeVar;         // Type de chaque variable : "qualitative" ou "quantitative"
    vector<string> NomVar;          // Nom de chaque variable
    vector<DataIndividu> V;         // Ensemble des individus (données)

    // Constructeur : charge les données à partir d'un fichier texte (CSV-like)
    Data(string texte, string TVar[], int indiceVarY);

    // Surcharge de l'opérateur << pour afficher l'objet Data
    friend ostream& operator<<(ostream& os, Data D);

    // Détecte automatiquement le type des variables (quantitative/qualitative)
    void getTypeVar();

    // Sépare les données en un ensemble d'entraînement et un ensemble de test
    pair<Data, Data> Train_Test_Split(float pourcentageTrain = 0.8) ;
};

#endif // DATA_H_INCLUDED
