#include"Arbre.h"
#include <fstream>
#include <algorithm>
#include<set>
#include"Data.h"
#include <random>
#include <iostream>
#include <ctime>
#include<math.h>
#include<vector>
using namespace std;




/** \brief Destructeur de la classe Arbre
 */
Arbre::~Arbre() {
    delete racine;
}



/**
 * \brief
 * \param P Noeud*&, D Data&
 * \return void
 */
void Arbre::creerNoeud(Noeud*& P,Data & D)
{
    // Initialisation des variables pour stocker la meilleure division
    float meilleurGainGiniVI = numeric_limits<float>::max(); // Initialisation avec la pire valeur possible
    int meilleureVar = -1;                                   // Indice de la variable optimale
    float meilleurSeuil = 0.0;                               // Seuil optimal pour variables quantitatives
    set<string> meilleurGroupe;                              // Groupe optimal pour variables qualitatives

    //Vérification des critères d'arrêt
    if (critereArret(*P,D)) {
    return;
    }

    //Exploration des variables pour trouver la meilleure division
    for (int i=0; i<D.nbrVar; i++){

        //Cas d'une variable quantitative
        if (D.TypeVar[i]=="quanti" && i!=D.indVarY){
            //Tester chaque valeur comme seuil potentiel
            for (auto seuil_ind:P->vectIndiceInd){
                float seuil = D.V[seuil_ind].getAtQuant(i); //Valeur du seuil courant
                vector<int> classeGauche, classeDroite;//Sous-ensembles pour la division
                //Diviser les individus selon le seuil
                for (int ind : P->vectIndiceInd){
                    if (D.V[ind].getAtQuant(i) <= seuil){
                        classeGauche.push_back(ind);
                    }
                    else{classeDroite.push_back(ind);}
                }
                //Calculer le gain de Gini ou de variance pour cette division
                float gain=1; //Valeur initiale qui sera écrasée
                if(D.TypeVar[D.indVarY]=="quali"){gain = gainGini(classeDroite,classeGauche,D,*P);}// Gain pour variable cible qualitative
                else if(D.TypeVar[D.indVarY]=="quanti"){gain = gainVI(classeDroite,classeGauche,D,*P);}// Gain pour variable cible quantitative

                // Mettre à jour si ce gain est meilleur
                if (gain < meilleurGainGiniVI){
                    meilleurGainGiniVI = gain;
                    meilleureVar = i;
                    meilleurSeuil = seuil;
                }
            }
        }//Fin if Quanti

        //Cas d'une variable qualitative
        else if (D.TypeVar[i] == "quali"&& i!=D.indVarY)
        {
            vector<string> categories = D.cat[D.NomVar[i]];//Liste des catégories possibles
            int n = categories.size();
            // Générer toutes les combinaisons possibles de regroupements
            for (int combinaison = 1; combinaison < (1 << n-1); ++combinaison){
                set<string> groupe1, groupe2; //Groupes pour la division
                for (int j = 0; j < n; ++j){
                    if (combinaison & (1 << j)){
                        groupe1.insert(categories[j]);
                    }
                    else{groupe2.insert(categories[j]);}
                }
                if (!groupe1.empty() && !groupe2.empty()){
                    vector<int> classeGauche, classeDroite;
                    // Diviser les individus selon les groupes
                    for (auto ind : P->vectIndiceInd){
                        if (groupe1.find(D.V[ind].getAtQual(i)) != groupe1.end()){
                            classeGauche.push_back(ind);
                        }
                        else{classeDroite.push_back(ind);}
                    }
                    // Calculer le gain de Gini ou de variance pour cette division
                    float gain=1;
                    if(D.TypeVar[D.indVarY]=="quali"){gain = gainGini(classeDroite,classeGauche,D,*P);}
                    else if(D.TypeVar[D.indVarY]=="quanti"){ gain = gainVI(classeDroite,classeGauche,D,*P);}
                    // Mettre à jour si ce gain est meilleur
                    if (gain < meilleurGainGiniVI){
                        meilleurGainGiniVI = gain;
                        meilleureVar = i;
                        meilleurGroupe = groupe1;
                    }
                }
            }
        }//else if Quali
    }//Fin de la division

    //Création des nœuds fils basés sur la division optimale
    vector<int> classeGauche, classeDroite;
    diviserNoeud(*P,D,meilleureVar,meilleurSeuil,meilleurGroupe,classeGauche,classeDroite);
    //Créer un nouveau nœud quantitatif ou qualitatif pour remplacer P
    Noeud* nouveauP;
    if (D.TypeVar[meilleureVar] == "quanti") {
        nouveauP = new NoeudQuantitatif(P->vectIndiceInd,meilleureVar,meilleurSeuil);
    } else {
        nouveauP = new NoeudQualitatif(P->vectIndiceInd,meilleureVar,meilleurGroupe);
    }

    //Libérer l'ancien nœud et assigner le nouveau
    delete P;
    P = nouveauP;

    //Initialiser les nœuds fils comme feuilles
    P->Ngauche = new NoeudFeuille(classeGauche);
    P->Ndroite = new NoeudFeuille(classeDroite);

    //Appels récursifs pour construire les sous-arbres
    creerNoeud(P->Ngauche, D);
    creerNoeud(P->Ndroite, D);
}



/**
 * \brief
 * \param P Noeud&
 * \param D Data&
 * \param meilleureVar int
 * \param meilleurSeuil float
 * \param meilleurGroupe set<string>&
 * \param classeGauche vector<int>&
 * \param classeDroite vector<int>&
 * \return void
 */
void Arbre::diviserNoeud(Noeud& P,Data& D,int meilleureVar,float meilleurSeuil,set<string>& meilleurGroupe,vector<int>& classeGauche,vector<int>& classeDroite){
    if(D.TypeVar[meilleureVar] == "quanti"){
        for (int ind : P.vectIndiceInd){
            if (D.V[ind].getAtQuant(meilleureVar) <= meilleurSeuil){
                classeGauche.push_back(ind);
            }
            else{classeDroite.push_back(ind);}
        }
    }
    else if(D.TypeVar[meilleureVar] == "quali"){
        for (int ind : P.vectIndiceInd){
            if (meilleurGroupe.find(D.V[ind].getAtQual(meilleureVar)) != meilleurGroupe.end()){
                classeGauche.push_back(ind);
            }
            else{classeDroite.push_back(ind);}
        }
    }
}




/**
 * \brief
 * \param P Noeud&, D Data&
 * \return bool
 */
bool Arbre::critereArret(Noeud& P,Data& D) {
    if (P.vectIndiceInd.size() < 10) return true;
    if (D.TypeVar[D.indVarY] == "quali" && Gini(P.vectIndiceInd, D) == 0) return true;
    if (D.TypeVar[D.indVarY] == "quanti" && VI(P.vectIndiceInd, D) == 0) return true;
    return false;
}



/**
 * \brief calcule la variance de la variable cible pour les individus contenus dans le vecteur classeMere.
 * \param classeMere vector<int>&, D Data&
 * \return float
 */
float Arbre::VI(vector<int> & classeMere, Data & D){
    float res=0;
    float moy=0;
    for (int ind : classeMere){
        moy+=D.V[ind].getAtQuant(D.indVarY);
    }
    moy=moy/classeMere.size();
    for (int ind : classeMere){
        res+=pow((D.V[ind].getAtQuant(D.indVarY)-moy),2);
    }
    return res/classeMere.size();
}




/**
 * \brief Cette méthode calcule le gain de variance
 * \param classeDroite vector<int>&
 * \param classeGauche vector<int>&
 * \param D Data&, P Noeud&
 * \return float
 */
float Arbre::gainVI(vector<int> & classeDroite,vector<int> & classeGauche,Data &D,Noeud & P){
    float VIG = VI(classeGauche,D);
    float VID = VI(classeDroite,D);
    return (float) classeDroite.size()*VID/P.vectIndiceInd.size() + classeGauche.size()*VIG/P.vectIndiceInd.size();
}




/**
 * \brief Calcule l'indice de Gini
 * \param classeMere vector<int>&, D Data&
 * \return float
 */
float Arbre::Gini(vector<int> & classeMere, Data & D){
    map<string, int> countModalites;
    for (int ind : classeMere){
        countModalites[D.V[ind].getAtQual(D.indVarY)]++;
    }

    float gini = 1;
    for (const auto& mod : countModalites){
        float proportion = (float) mod.second / classeMere.size();
        gini -= proportion * proportion;
    }
    return gini;
}



/**
 * \brief calcule le gain d'impureté de Gini
 * \param classeDroite vector<int>&
 * \param classeGauche vector<int>&
 * \param D Data&, P Noeud&
 * \return float
 */
float Arbre::gainGini(vector<int> & classeDroite,vector<int> & classeGauche,Data &D,Noeud & P){
    float giniG = Gini(classeGauche,D);
    float giniD = Gini(classeDroite,D);
    return (float) (classeDroite.size()*giniD + classeGauche.size()*giniG)/P.vectIndiceInd.size();
}




/**
 * \brief Initialise un arbre de décision en créant un nœud racine contenant
 * tous les individus, puis construit l'arbre récursivement en appelant creerNoeud.
 * Enfin, elle exporte l'arbre au format .dot.
 * \param D Data&, indice vector<int>
 * \param nomFichier string
 * \return void
 */
void Arbre::creer_Arbre(Data & D,vector<int> indice,string nomFichier){
    racine = new NoeudFeuille(indice);
    creerNoeud(racine,D);
    calculerPredictionsFeuilles(racine,D);
    exporterGraphviz(racine,D,nomFichier);
}




/**
 * \brief génère un fichier .dot permettant de visualiser l'arbre de décision.
 * \param N Noeud*, D Data&
 * \param nomFichier string
 * \return void
 */
void Arbre::exporterGraphviz( Noeud* N, Data& D,string nomFichier){
    ofstream fichier(nomFichier+".dot");
    if (!fichier) {
        cerr << "Erreur : impossible d'ouvrir le fichier " << "Graphe.dot" << endl;
        return;
    }

    fichier << "digraph Arbre {" << endl;
    fichier << "  node [shape=box];" << endl;

    int id = 0; // Identifiant unique pour chaque nœud
    exporterGraphvizRecursif(N, fichier, id, D);

    fichier << "}" << endl;
    fichier.close();
    cout << " Fichier GRAPHVIZ genere : " << nomFichier+".dot" << endl;
}




 /**
  *\brief Cette méthode est appelée récursivement pour parcourir l'arbre de décision et écrire
  * les informations de chaque nœud
  * \param noeud Noeud*, fichier ofstream&
  * \param id int&, D Data&
  * \return void
  */
 void Arbre::exporterGraphvizRecursif(Noeud* noeud, ofstream& fichier, int& id, Data& D) {
    if (!noeud) return;
    int noeudId = id++; // Identifiant unique pour ce nœud

    // Écrire le nœud actuel avec les informations appropriées
    fichier << "  " << noeudId << " [label=\"";

    if (noeud->typeVarOpti == -1) {
        fichier << "Feuille\\n Pred: " << dynamic_cast<NoeudFeuille*>(noeud)->prediction<< "\\n";
    } else {
        fichier << "Var " << D.NomVar[noeud->indVarOpti] << "\\n";

        if (D.TypeVar[noeud->indVarOpti] == "quanti") {
            fichier << "Seuil: " << dynamic_cast<NoeudQuantitatif*>(noeud)->seuilOpti << "\\n";
        } else if (D.TypeVar[noeud->indVarOpti] == "quali") {
            fichier << "Groupes: {";
            for (const auto& groupe : dynamic_cast<NoeudQualitatif*>(noeud)->meilleurGroupe) {
                string groupeSansGuillemets = groupe;
                groupeSansGuillemets.erase(remove(groupeSansGuillemets.begin(), groupeSansGuillemets.end(), '\"'), groupeSansGuillemets.end());
                fichier << groupeSansGuillemets << " ";
            }
            fichier << "}\\n";
        }
    }

    fichier << "Individus: " << noeud->vectIndiceInd.size() << "\"];" << endl;

    // Traiter le fils gauche
    if (noeud->Ngauche) {
        int gaucheId = id;
        exporterGraphvizRecursif(noeud->Ngauche, fichier, id, D);
        if (noeud->typeVarOpti != -1) {
            if (D.TypeVar[noeud->indVarOpti] == "quanti") {
                fichier << "  " << noeudId << " -> " << gaucheId << " [label=\"<= " << dynamic_cast<NoeudQuantitatif*>(noeud)->seuilOpti << "\"];" << endl;
            } else if (D.TypeVar[noeud->indVarOpti] == "quali") {
                fichier << "  " << noeudId << " -> " << gaucheId << " [label=\"dans groupe\"];" << endl;
            }
        } else {
            fichier << "  " << noeudId << " -> " << gaucheId << ";" << endl;
        }
    }

    // Traiter le fils droit
    if (noeud->Ndroite) {
        int droiteId = id;
        exporterGraphvizRecursif(noeud->Ndroite, fichier, id, D);
        if (noeud->typeVarOpti != -1) {
            if (D.TypeVar[noeud->indVarOpti] == "quanti") {
                fichier << "  " << noeudId << " -> " << droiteId << " [label=\"> " << dynamic_cast<NoeudQuantitatif*>(noeud)->seuilOpti << "\"];" << endl;
            } else if (D.TypeVar[noeud->indVarOpti] == "quali") {
                fichier << "  " << noeudId << " -> " << droiteId << " [label=\"hors groupe\"];" << endl;
            }
        } else {
            fichier << "  " << noeudId << " -> " << droiteId << ";" << endl;
        }
    }
}




/**
 * \brief Calcule les prédictions pour tous les noeuds feuilles de l'arbre
 * \param noeud Noeud*
 * \param D Data&
 * \return void
 */
void Arbre::calculerPredictionsFeuilles(Noeud* noeud, Data& D) {
    if (!noeud) return;

    // Si c’est une feuille (typeVarOpti = -1)
    if (noeud->typeVarOpti == -1) {
        NoeudFeuille* feuille = static_cast<NoeudFeuille*>(noeud);
        if ( D.TypeVar[D.indVarY] == "quali") {
            // Calcul de la classe majoritaire
            map<string, int> countModalites;
            for (int ind : feuille->vectIndiceInd) {
                countModalites[D.V[ind].getAtQual(D.indVarY)]++;
            }
            string classeMajoritaire;
            int maxCount = 0;
            for (const auto& mod : countModalites) {
                if (mod.second > maxCount) {
                    maxCount = mod.second;
                    classeMajoritaire = mod.first;
                }
            }
            feuille->prediction = classeMajoritaire;
        } else if (D.TypeVar[D.indVarY]=="quanti") {
            // Calcul de la moyenne
            float somme = 0;
            for (int ind : feuille->vectIndiceInd) {
                somme += D.V[ind].getAtQuant(D.indVarY);
            }
            feuille->prediction = to_string(somme/feuille->vectIndiceInd.size());
        }
    } else {
        // Parcourir récursivement les sous-arbres
        calculerPredictionsFeuilles(noeud->Ngauche,D);
        calculerPredictionsFeuilles(noeud->Ndroite,D);
    }
}




/**
 * \brief Prédit la classe d'un individu en parcourant l'arbre
 * \param individu DataIndividu&
 * \return string
 */
string Arbre::predire(DataIndividu& individu){
    if (racine == nullptr) {
        throw runtime_error("L'arbre n'a pas été construit.");
    }
    return predireRecursif(racine,individu);
}



/**
 * \brief Predit la classe d'un individu en parcourant de manière récursive l'arbre
 * \param noeud Noeud*,individu DataIndividu&
 * \return string
 */
string Arbre::predireRecursif(Noeud* noeud,DataIndividu& individu){
    //Noeud Feuille
    if (noeud->typeVarOpti == -1) {
        NoeudFeuille* feuille = static_cast<NoeudFeuille*>(noeud);
        return feuille->prediction;  // Retourne la prédiction pré-calculée
    }
    //Noeud Quantitatif
    if (noeud->typeVarOpti == 0){
        if (individu.getAtQuant(noeud->indVarOpti) <= static_cast<NoeudQuantitatif*>(noeud)->seuilOpti) {
            return predireRecursif(noeud->Ngauche,individu);
        } else {
            return predireRecursif(noeud->Ndroite,individu);
        }
    //Noeud Qualitative
    } else if (noeud->typeVarOpti == 1){
        string valeur = individu.getAtQual(noeud->indVarOpti);
        if (static_cast<NoeudQualitatif*>(noeud)->meilleurGroupe.find(valeur) != static_cast<NoeudQualitatif*>(noeud)->meilleurGroupe.end()) {
            return predireRecursif(noeud->Ngauche,individu);
        } else {
            return predireRecursif(noeud->Ndroite,individu);
        }
    }
}






















/** \brief Destructeur de la classe BaggingArbre
 */
BaggingArbre::~BaggingArbre(){
    for (Arbre* arbre : arbres){
        delete arbre;
    }
}



/**
 * \brief Génère un échantillon d'indices avec remise de la taille de la base de données D
 * \param D Data&
 * \return vector<int>
 */
vector<int> BaggingArbre::genererEchantillon(Data& D){
    vector<int> echantillon;
    static mt19937 gen(random_device{}()^time(nullptr));
    uniform_int_distribution<> dis(0, D.V.size()-1);
    // Générer un échantillon de taille D.V.size() avec remise
    for (size_t i = 0; i < D.V.size(); ++i) {
        echantillon.push_back(dis(gen));
    }
    return echantillon;
}




/**
 * \brief Entraîne une forêt d'arbres par bagging
 * \param D Data&
 * \return void
 */
void BaggingArbre::entrainer(Data& D){
    // Créer et entraîner nbrArbres arbres
    for (int i = 0; i < nbrArbres; ++i){
        Arbre* arbre = new Arbre();
        vector<int> echantillon = genererEchantillon(D);
        arbre->creer_Arbre(D,echantillon,"ArbreNum"+to_string(i)+"Bagging");
        arbres.push_back(arbre);
    }
}




/**
 * \brief Prédit la classe/valeur d'un individu par vote majoritaire/moyenne.
 * \param D Data&, individu DataIndividu&
 * \return string
 */
string BaggingArbre::predire(Data& D,DataIndividu& individu){
    if (arbres.empty()) {
        throw runtime_error("Aucun arbre n’a été entraîné.");
    }

    if (D.TypeVar[D.indVarY] == "quali") {
        // Vote majoritaire
        map<string, int> votes;
        for (Arbre* arbre : arbres){
            string pred = arbre->predire(individu);
            votes[pred]++;
        }
        string predictionMajoritaire;
        int maxVotes = 0;
        for (const auto& [pred, count] : votes){
            if (count > maxVotes) {
                maxVotes = count;
                predictionMajoritaire = pred;
            }
        }
        return predictionMajoritaire;
    } else if (D.TypeVar[D.indVarY]=="quanti"){
        // Moyenne des prédictions
        float somme = 0;
        for (Arbre* arbre : arbres){
            string pred = arbre->predire(individu);
            somme += stof(pred); // Convertir la string en float
        }
        return to_string(somme/nbrArbres);
    }
}



/**
 * \brief Exécute le processus complet de bagging (entraînement + prédiction)
 * \param D Data&
 * \param individuTest DataIndividu&
 * \return void
 */
void BaggingArbre::executerBagging(Data& D, DataIndividu& individuTest){
    entrainer(D);
    string prediction = predire(D,individuTest);
    cout << "Prediction pour l'individu de test : " << prediction << endl;
}
