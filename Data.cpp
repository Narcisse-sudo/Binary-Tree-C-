#include"Data.h"
#include"DataIndividu.h"
#include"Attribut.h"
#include<map>
#include<set>
#include <sstream>
#include<string>
#include<vector>
#include<fstream>
#include <algorithm>


Data::Data(string texte, int indiceVarY){
    indVarY = indiceVarY;
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

    // Lecture de la deuxième ligne pour recuperer le type des variables
    string Ligne_2;
    if (getline(in, Ligne_2)){
        stringstream ligne(Ligne_2);
        string value_Var;
        while(getline(ligne, value_Var, ',')){
            try{
                stof(value_Var);
                TypeVar.push_back("quanti");
            }
            catch(...){ TypeVar.push_back("quali"); }
        }
    }


    //Lecture des donnees individuelles
    string ligne;
    while (getline(in, ligne)){
        stringstream donnees(ligne);
        string cellule;
        DataIndividu I;
        for (int i = 0; i < nbrVar && getline(donnees,cellule, ','); ++i){
                if(TypeVar[i]=="quanti"){
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

//------------------------------------------------------------------------------------------------------------------

//Affichage des données
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

    //------------------------------------------------------------------------------------------------------------------

    // Affichage du type des variables
    void Data :: getTypeVar(){
        for ( int i = 0; i<nbrVar; i++){
        cout <<  NomVar[i] <<" : "<< TypeVar[i] << endl;
        }
    }
 

//------------------------------------------------------------------------------------------------------------------

// Définir la méthode pour diviser les données
pair<Data, Data> Data::Train_Test_Split(float pourcentageTrain) const {
    int n = V.size();
    int n_train = n * pourcentageTrain;

    vector<DataIndividu> shuffled = V;
    srand(time(0));
    random_shuffle(shuffled.begin(), shuffled.end());

    vector<DataIndividu> train_V(shuffled.begin(), shuffled.begin() + n_train);
    vector<DataIndividu> test_V(shuffled.begin() + n_train, shuffled.end());

    Data trainData("", indVarY);
    Data testData("", indVarY);

    trainData.V = train_V;
    testData.V = test_V;

    trainData.NomVar = NomVar;
    trainData.TypeVar = TypeVar;
    trainData.cat = cat;
    trainData.nbrVar = nbrVar;
    trainData.indVarY = indVarY;

    testData.NomVar = NomVar;
    testData.TypeVar = TypeVar;
    testData.cat = cat;
    testData.nbrVar = nbrVar;
    testData.indVarY = indVarY;

    return {trainData, testData};
}
