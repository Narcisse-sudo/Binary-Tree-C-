/*
 * Fichier : main.cpp
 * Auteur : Benjamin SWIETEK & Narcisse DALKO
 * Date : 2024/2025
 */

#include"Data.h"
#include"Arbre.h"
#include <iostream>
using namespace std;

int main()
{
//Configuration des types de variables correxpondant aux colonnes du fichier
string QualQuant[10]={"quali","quali","quanti","quanti","quanti","quanti","quanti","quali","quanti","quanti"};
Data P("C:/Users/benji/OneDrive/Bureau/mon_fichier.txt",QualQuant,1);//Colonne 1 = variable cible
//cout << P;
cout << endl; 

    //Preparation des indices des individus
    vector<int> indice;
    for(int i=0; i<P.V.size(); i++){
        indice.push_back(i);
    }

    //Construction de l'arbre de decision
    Arbre A;
    A.creer_Arbre(P,indice,"PremierArbre");

    //Prediction pour le premier individu
    string prediction = A.predire(P.V[0]);
    cout << "Prediction pour l'individu 1 : " << prediction << endl;

    // Executer le bagging
    BaggingArbre bagging(5); // 5 arbres
    bagging.executerBagging(P,P.V[0]);
}
