#include"Data.h"
#include"Arbre.h"
#include <iostream>
using namespace std;

int main()
{
//Configuration des types de variables correxpondant aux colonnes du fichier
//string QualQuant[10]={"quali","quali","quanti","quanti","quanti","quanti","quanti","quali","quanti","quanti"};
Data data("D:/OneDrive/Documents/MIGS/Projet_Binary_TreeC++/Pic_Ozone.txt", 1);//Colonne 1 = variable cible
//cout << data;
//data.getTypeVar();

// Diviser les donnees en train et  test
auto [train_set, test_set] = data.Train_Test_Split(0.8);
//cout << train_set; 


cout << endl;

    //Preparation des indices des individus
    vector<int> indice;
    for(int i=0; i<train_set.V.size(); i++){
        indice.push_back(i);
    }


    //Construction de l'arbre de decision
    Arbre A;
    A.creer_Arbre(train_set,indice,"PremierArbre");

    cout << "=== Prédictions sur le test set ===" << endl;

int correct = 0;
for (int i = 0; i < test_set.V.size(); i++) {
    DataIndividu indiv = test_set.V[i];
    // Prédiction
    string prediction = A.predire(indiv);
    // Vérité terrain (valeur réelle de la variable cible Y)
    float vrai_label = indiv.getAtQuant(data.indVarY); // à adapter si c'est une variable quantitative
    
    cout << "Individu " << i+1 << " : Predit = " << prediction << ", Reel = " << vrai_label << endl;
    if (stof(prediction) == vrai_label) correct++;
}

float accuracy = (float)correct / test_set.V.size();
cout << "\nAccuracy sur le test set : " << accuracy * 100 << " %" << endl;

    // Executer le bagging
   // BaggingArbre bagging(5); // 5 arbres
    //bagging.executerBagging(P,P.V[0]);
}
