#ifndef ARBRE_H_INCLUDED
#define ARBRE_H_INCLUDED
#include <fstream>
#include <set>
#include "Data.h"
#include <iostream>
#include <vector>

using namespace std;

//--------------------------------------------------------------------------------------------------------------------

class Noeud {
public:
    Noeud* Ngauche;   //Pointeur vers le noeud gauche
    Noeud* Ndroite;   //Pointeur vers le noeud droite
    int indVarOpti;   //Indice de la variable optimale
    int typeVarOpti;  // 0 = quantitatif, 1 = qualitatif, -1 = feuille
    vector<int> vectIndiceInd;//Indices des individus du noeud
    Noeud(vector<int>& ind, int var=-1, int typeVar=-1) : vectIndiceInd(ind), Ngauche(nullptr), Ndroite(nullptr), indVarOpti(var), typeVarOpti(typeVar){} //Constructeur
    virtual ~Noeud(){ delete Ngauche; delete Ndroite; }//Destructeur
};

//--------------------------------------------------------------------------------------------------------------------

class NoeudQuantitatif : public Noeud {
public:
    float seuilOpti;  //Seuil optimal
    NoeudQuantitatif(vector<int>& ind, int var, float seuil) : Noeud(ind, var,0), seuilOpti(seuil){}
};

//--------------------------------------------------------------------------------------------------------------------

class NoeudQualitatif : public Noeud {
public:
    set<string> meilleurGroupe;//Groupe de modalites optimal
    NoeudQualitatif(vector<int>& ind, int var, set<string>& groupe) : Noeud(ind, var, 1), meilleurGroupe(groupe){}
};

//--------------------------------------------------------------------------------------------------------------------

class NoeudFeuille : public Noeud {
public:
    string prediction; // Classe majoritaire ou moyenne
    NoeudFeuille(vector<int>& ind) : Noeud(ind), prediction("") {}
};

//--------------------------------------------------------------------------------------------------------------------

class Arbre
{
private :
    Noeud* racine; //Pointeur vers la racine de l'arbre

    // Methodes Creation d'un noeud
    void creerNoeud(Noeud *& P,Data& D);
    void diviserNoeud(Noeud& P,Data& D,int meilleureVar,float meilleurSeuil,set<string>& meilleurGroupe,vector<int>& classeGauche,vector<int>& classeDroite);
    bool critereArret(Noeud& P,Data& D);

    //   Gini pour les variables qualitatives
    float gainGini(vector<int> & classeDroite,vector<int> & classeGauche,Data & D,Noeud & P);
    float Gini(vector<int> & classe,Data & D);

     // Variance pour les variables quantitatives
    float Variance(vector<int> & classeMere, Data & D);
    float gainVariance(vector<int> & classeDroite,vector<int> & classeGauche,Data &D,Noeud & P);

    // Methodes de Prediction
    string predireRecursif(Noeud* noeud,DataIndividu& individu);
    void calculerPredictionsFeuilles(Noeud* noeud, Data& D);


    // Methode de creation Graphviz
    void exporterGraphviz(Noeud * N,Data &D,string nomFichier);
    void exporterGraphvizRecursif(Noeud* noeud, ofstream& fichier, int& id,Data & D);
public  :
    ~Arbre(); 
    string predire(DataIndividu& individu);                             //Predir  un individu
    Arbre() : racine(nullptr) {}                                         //Constructeur
    void creer_Arbre(Data & D, vector<int> indice, string nomFichier);   //Construit l'arbre
    pair<vector<string>, float>  evaluerTestSet(Data& test_set);          // prediction et calcule du score
};

//--------------------------------------------------------------------------------------------------------------------

class BaggingArbre {
private:
    vector<Arbre*> arbres; // Ensemble des arbres
    int nbrArbres;         // Nombre d'arbres dans le bagging
    string typeVarY;       // Type de la variable cible
    int indVarY;           // Indice de la variable cible
    Data D;
    vector<int> genererEchantillon();
public:
    BaggingArbre(int nbrA,Data& E) : nbrArbres(nbrA),D(E) {}
    ~BaggingArbre();//Destructeur
    void entrainer();
    string predire(DataIndividu& individu);
    void executerBagging();
    pair<vector<string>, float>  evaluerTestSet(Data& test_set);  
};
#endif // ARBRE_H_INCLUDED
