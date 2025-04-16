#include"Data.h"
#include"DataIndividu.h"
#include"Attribut.h"
#include<map>
#include<set>
#include <sstream>
#include<string>
#include<vector>
#include<fstream>


Data::Data(string texte,string TVar[],int indiceVarY){
    indVarY=indiceVarY;
    //Ouverture du fichier
    ifstream in(texte);
    if (!in) cout << "pb ouverture fichier" << endl;

    //Lecture des noms de variables (premiere ligne)
    string premiereLigne;
    if (getline(in, premiereLigne)){// Lire la premiere ligne du fichier
        stringstream ligne(premiereLigne); // Convertir la ligne en un flux pour la diviser
        string nom;
        while (getline(ligne, nom, ',')){
            if (!nom.empty() && nom.front() == '"' && nom.back() == '"'){
                nom.erase(nom.begin());
                nom.erase(nom.end() - 1);
            }
            NomVar.push_back(nom);
        }
    }

    nbrVar = NomVar.size();

    //Initialisation des types de variables
    for(int i=0; i<nbrVar; i++){
        TypeVar.push_back(TVar[i]);
    }

    //Lecture des donnees individuelles
    string ligne;
    while (getline(in, ligne)){
        stringstream donnees(ligne);
        string cellule;
        DataIndividu I;
        for (int i = 0; i < nbrVar && getline(donnees,cellule, ','); ++i){
                if(TVar[i]=="quanti"){
                    float y = stof(cellule);
                    I.ajouteAt(new AtQuant(y));
                }
                else{
                    I.ajouteAt(new AtQual(cellule));
                }
        }
        V.push_back(I);
    }

    //Construction des categories uniques pour les variables qualitatives
    for (int i=0;i < nbrVar;++i) {
        if (TypeVar[i]=="quali") {
            set<string> uniqCat;
            for (auto ligne : V) {
                    uniqCat.insert(ligne.getAtQual(i));
            }
            cat[NomVar[i]].assign(uniqCat.begin(),uniqCat.end());
        }
    }
        in.close();
}


//Affichage de la bdd
ostream & operator<<(ostream & os,Data D)
    {
        for(auto & nonVar : D.NomVar){
            cout << nonVar <<" ";
        }
        cout<<endl;

        int i = 0;
        for(auto  im=D.V.begin() ; im!=D.V.end() ; im++){
            os << "(" << (*im) <<")";
            os <<"\n";
            i+=1;
            if (i == 10) break;
        }
        return os;
    }
