#ifndef DATAINDIVIDU_H_INCLUDED
#define DATAINDIVIDU_H_INCLUDED
#include"Attribut.h"
#include<vector>

class DataIndividu{

vector<Attribut*> vect; //Liste des attributs (qualitatifs/quantitatifs)

public :
    //Ajouter un attribut a l'individu
    void ajouteAt(Attribut* r){  vect.push_back(r); }

    //Recuperer la valeur numerique d'un attribut
    double getAtQuant(int i){  return vect[i]->getAtQuant(); }

    //Recupere la valeur texte d'un attribu
    string getAtQual(int i){ return vect[i]->getAtQual(); }


    //Afficher un individu
    friend ostream & operator<<(ostream & os,DataIndividu& ind){
        for(int i=0;i<ind.vect.size();i++){
            ind.vect[i]->affiche(os);
            if(i!=ind.vect.size()-1) {  os << ","; }
        }
        return os;
    }

    /* // Libère la mémoire allouée pour chaque attribut
    ~DataIndividu() {
        for (auto a : vect) {
            delete a;
        }
    }*/

    

};
#endif // DATAINDIVIDU_H_INCLUDED
