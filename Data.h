
#include <iostream>
using namespace std;


#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED
#include<map>
#include<string>
#include<vector>
#include"DataIndividu.h"

class Data{
public :
    map<string, vector<string>> cat;//Modalites uniques
    int nbrVar;//nombre total de variable
    int indVarY;//l'indice de la colonne de la variable Y
    vector<string> TypeVar;//vecteur contenant le type de variable (qualitative/quantitative) de chaques variables.
    vector<string> NomVar;//vecteur contenant le nom de chaque variable
    vector<DataIndividu> V;//vecteur contenant les valeurs de chaques individus
    Data(string texte,string TVar[],int indiceVarY);//Charge les donnees depuis un fichier CSV
    friend ostream & operator<<(ostream & os,Data D);//Affiche les donnees
};
#endif // DATA_H_INCLUDED
