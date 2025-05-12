#include"Data.h"
#include"Arbre.h"

int main()
    {
    //Configuration des types de variables correxpondant aux colonnes du fichier
    string QualQuant[10]={"quanti","quali","quanti","quanti","quanti","quanti","quanti","quali","quanti","quanti"};

    Data data("D:/OneDrive/Documents/MIGS/Projet_Binary_TreeC++/Pic_Ozone.txt", QualQuant, 1);//Colonne 1 = variable cible

    //cout << data;
    //data.getTypeVar();


    // Diviser les donnees en train et  test
    auto [train_set, test_set] = data.Train_Test_Split(0.8);
    //cout << train_set; 

    cout << endl;

    //Preparation des indices des individus du train_set
    vector<int> indice;
    for(int i=0; i<train_set.V.size(); i++){
        indice.push_back(i);
    }

    //Construction de l'arbre de decision
    Arbre A;
    A.creer_Arbre(train_set, indice, "PremierArbre");

    // Prediction sur le test_set et calcule du score de prediction avec un arbre
    auto [prediction, score] = A.evaluerTestSet(test_set);
    for (const auto& pred : prediction) cout << pred <<" ";
    cout <<"\n \n" <<  " score : " << score << "\n\n"; 

    // Executer le bagging 
     int nbr_arbre = 10;   //Nombres d'arbres
     BaggingArbre bagging(nbr_arbre, train_set); 
     bagging.executerBagging();
    // Prediction sur le test_set et calcule du score de prediction avec bagging
     auto [predictions, scor] = bagging.evaluerTestSet(test_set);
     for (const auto& pred : predictions) cout << pred <<" ";
     cout <<"\n \n" <<  " score : " << scor<< "\n\n"; 

}
