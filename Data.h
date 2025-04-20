
#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>
#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED
#include<map>
#include<string>
#include<vector>
#include"DataIndividu.h"
#include <utility>

class Data{
public :
    map<string, vector<string>> cat;//Modalites uniques

    int nbrVar;//nombre total de variable
    int indVarY;//l'indice de la colonne de la variable Y

    vector<string> TypeVar;//vecteur contenant le type de variable (qualitative/quantitative) de chaques variables.
    vector<string> NomVar;//vecteur contenant le nom de chaque variable
    vector<DataIndividu> V;//vecteur contenant les valeurs de chaques individus

    Data(string texte, int indiceVarY);//Charge les donnees depuis un fichier CSV

    friend ostream & operator<<(ostream & os,Data D);//Affiche les donnees

    void getTypeVar() ;
     
    // Renvoie une paire de Data (train, test)
    pair<Data, Data> Train_Test_Split(float pourcentageTrain = 0.8) const;
};
#endif // DATA_H_INCLUDED
