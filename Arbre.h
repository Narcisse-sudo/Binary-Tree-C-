#ifndef ARBRE_H_INCLUDED
#define ARBRE_H_INCLUDED
#include <fstream>
#include<vector>
#include<set>
#include"Data.h"
#include <iostream>

using namespace std;

class Noeud {
public:
    Noeud* Ngauche; //Pointeur vers le noeud gauche
    Noeud* Ndroite; //Pointeur vers le noeud droite
    int indVarOpti; //Indice de la variable optimale
    int typeVarOpti;  //0 = quantitatif, 1 = qualitatif, -1 = feuille
    vector<int> vectIndiceInd;//Indices des individus du noeud
    Noeud(vector<int>& ind,int var=-1,int typeVar=-1):vectIndiceInd(ind),Ngauche(nullptr),Ndroite(nullptr),indVarOpti(var),typeVarOpti(typeVar){}//Constructeur
    virtual ~Noeud(){delete Ngauche; delete Ndroite;}//Destructeur
};

class NoeudQuantitatif : public Noeud {
public:
    float seuilOpti;//Seuil optimal
    NoeudQuantitatif(vector<int>& ind,int var,float seuil):Noeud(ind,var,0),seuilOpti(seuil){}
};

class NoeudQualitatif : public Noeud {
public:
    set<string> meilleurGroupe;//Groupe de modalités optimal
    NoeudQualitatif(vector<int>& ind,int var,set<string>& groupe):Noeud(ind,var,1),meilleurGroupe(groupe){}
};

class NoeudFeuille : public Noeud {
public:
    string prediction; // Classe majoritaire ou moyenne
    NoeudFeuille(vector<int>& ind) : Noeud(ind), prediction("") {}
};




class Arbre
{
private :
    Noeud* racine;//Pointeur vers la racine de l'arbre
    void diviserNoeud(Noeud& P,Data& D,int meilleureVar,float meilleurSeuil,set<string>& meilleurGroupe,vector<int>& classeGauche,vector<int>& classeDroite);
    bool critereArret(Noeud& P,Data& D);
    float gainGini(vector<int> & classeDroite,vector<int> & classeGauche,Data & D,Noeud & P);
    float Gini(vector<int> & classe,Data & D);
    void creerNoeud(Noeud *& P,Data & D);
    float VI(vector<int> & classeMere, Data & D);
    float gainVI(vector<int> & classeDroite,vector<int> & classeGauche,Data &D,Noeud & P);
    void exporterGraphvizRecursif(Noeud* noeud, ofstream& fichier, int& id,Data & D);
    string predireRecursif(Noeud* noeud,DataIndividu& individu);
    void calculerPredictionsFeuilles(Noeud* noeud, Data& D);
    void exporterGraphviz(Noeud * N,Data &D,string nomFichier);
public  :
    ~Arbre(); // Déclaration du destructeur
    string predire(DataIndividu& individu);//Prédir la classe d'un individu
    Arbre() : racine(nullptr) {}//Constructeur
    void creer_Arbre(Data & D,vector<int> indice,string nomFichier);//Construit l'arbre
};




class BaggingArbre {
private:
    vector<Arbre*> arbres; // Ensemble des arbres
    int nbrArbres;         // Nombre d’arbres dans le bagging
    string typeVarY;       // Type de la variable cible
    int indVarY;           // Indice de la variable cible
public:
    BaggingArbre(int nbrA) : nbrArbres(nbrA) {}
    ~BaggingArbre();//Destructeur
    void entrainer(Data& D);
    string predire(Data& D,DataIndividu& individu);
    void executerBagging(Data& D, DataIndividu& individuTest);
private:
    vector<int> genererEchantillon(Data& D);
};



#endif // ARBRE_H_INCLUDED
