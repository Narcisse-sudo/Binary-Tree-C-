#ifndef ATTRIBUT_H_INCLUDED
#define ATTRIBUT_H_INCLUDED
#include <iostream>
using namespace std;

class Attribut{
public:
    // Methodes virtuelles pures
    virtual float getAtQuant()=0;//Retourne la valeur quantitative
    virtual string getAtQual()=0;//Retourne la valeur qualitative
    virtual void affiche(ostream &os)=0;//Affichage
};


class AtQual : public Attribut{
string rep;//Valeur qualitative stockee
public :
    //Constructeur
    AtQual(string r){
        rep=r;
    }

    //Implementation des methodes virtuelles
    string getAtQual(){ return rep; }
    float getAtQuant(){ return 0; }
    void affiche(ostream &os){ os << rep ; }
};

class AtQuant : public Attribut{
    float rep;//Valeur quantitative stockee
public :
    //Constructeur
    AtQuant(double r){ rep=r; }

    //Implementation des methodes virtuelles
    float getAtQuant(){ return rep; }
    string getAtQual(){ return ""; }
    void affiche(ostream &os){ os << rep; }
};
#endif // ATTRIBUT_H_INCLUDED
