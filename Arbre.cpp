#include "Arbre.h"
#include <algorithm>
#include <random>
#include <math.h>

//--------------------------------------------------------------------------------------------------------------------

/** \brief Destructeur de la classe Arbre
 */
Arbre::~Arbre() {
    delete racine;
}

//--------------------------------------------------------------------------------------------------------------------


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

    //Verification des criteres d'arret
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
                float gain=1; //Valeur initiale qui sera ecrasee
                if(D.TypeVar[D.indVarY]=="quali"){gain = gainGini(classeDroite,classeGauche,D,*P);}// Gain pour variable cible qualitative
                else  {gain = gainVariance(classeDroite,classeGauche,D,*P);}// Gain pour variable cible quantitative

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
            vector<string> categories = D.cat[D.NomVar[i]];//Liste des categories possibles
            int n = categories.size();
            // Generer toutes les combinaisons possibles de regroupements
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
                    else if(D.TypeVar[D.indVarY]=="quanti"){ gain = gainVariance(classeDroite,classeGauche,D,*P);}
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

    //Creation des nouds fils bases sur la division optimale
    vector<int> classeGauche, classeDroite;
    diviserNoeud(*P,D,meilleureVar,meilleurSeuil,meilleurGroupe,classeGauche,classeDroite);
    //Creer un nouveau noud quantitatif ou qualitatif pour remplacer P
    Noeud* nouveauP;
    if (D.TypeVar[meilleureVar] == "quanti") {
        nouveauP = new NoeudQuantitatif(P->vectIndiceInd,meilleureVar,meilleurSeuil);
    } else {
        nouveauP = new NoeudQualitatif(P->vectIndiceInd,meilleureVar,meilleurGroupe);
    }

    //Liberer l'ancien noud et assigner le nouveau
    delete P;
    P = nouveauP;

    //Initialiser les nouds fils comme feuilles
    P->Ngauche = new NoeudFeuille(classeGauche);
    P->Ndroite = new NoeudFeuille(classeDroite);

    //Appels recursifs pour construire les sous-arbres
    creerNoeud(P->Ngauche, D);
    creerNoeud(P->Ndroite, D);
}

//--------------------------------------------------------------------------------------------------------------------

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
void Arbre::diviserNoeud(Noeud& P,Data& D,int IndiceMeilleureVar,float meilleurSeuil,set<string>& meilleurGroupe,vector<int>& classeGauche,vector<int>& classeDroite){
    if(D.TypeVar[IndiceMeilleureVar] == "quanti"){
        for (int ind : P.vectIndiceInd){
            if (D.V[ind].getAtQuant(IndiceMeilleureVar) <= meilleurSeuil){
                classeGauche.push_back(ind);
            }
            else{classeDroite.push_back(ind);}
        }
    }
    else if(D.TypeVar[IndiceMeilleureVar] == "quali"){
        for (int ind : P.vectIndiceInd){
            if (meilleurGroupe.find(D.V[ind].getAtQual(IndiceMeilleureVar)) != meilleurGroupe.end()){
                classeGauche.push_back(ind);
            }
            else{classeDroite.push_back(ind);}
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief
 * \param P Noeud&, D Data&
 * \return bool
 */
bool Arbre::critereArret(Noeud& P,Data& D) {
    if (P.vectIndiceInd.size() < 10) return true;
    if (D.TypeVar[D.indVarY] == "quali" && Gini(P.vectIndiceInd, D) == 0) return true;
    if (D.TypeVar[D.indVarY] == "quanti" && Variance(P.vectIndiceInd, D) == 0) return true;
    return false;
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief calcule la variance de la variable cible pour les individus contenus dans le vecteur classeMere.
 * \param classeMere vector<int>&, D Data&
 * \return float
 */
float Arbre::Variance(vector<int> & classeMere, Data & D){
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

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Cette methode calcule le gain de variance
 * \param classeDroite vector<int>&
 * \param classeGauche vector<int>&
 * \param D Data&, P Noeud&
 * \return float
 */
float Arbre::gainVariance(vector<int> & classeDroite,vector<int> & classeGauche,Data &D,Noeud & P){
    float VIG = Variance(classeGauche,D);
    float VID = Variance(classeDroite,D);
    return (float) classeDroite.size()*VID/P.vectIndiceInd.size() + classeGauche.size()*VIG/P.vectIndiceInd.size();
}

//--------------------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief calcule le gain d'impurete de Gini
 * \param classeDroite vector<int>&
 * \param classeGauche vector<int>&
 * \param D Data&, P Noeud&
 * \return float
 */
float Arbre::gainGini(vector<int> & classeDroite, vector<int> & classeGauche, Data &D,Noeud & P){
    float giniG = Gini(classeGauche,D);
    float giniD = Gini(classeDroite,D);
    return (float) (classeDroite.size()*giniD + classeGauche.size()*giniG)/P.vectIndiceInd.size();
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Initialise un arbre de decision en creant un noud racine contenant
 * tous les individus, puis construit l'arbre recursivement en appelant creerNoeud.
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

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief genere un fichier .dot permettant de visualiser l'arbre de decision.
 * \param N Noeud*, D Data&
 * \param nomFichier string
 * \return void
 */
void Arbre::exporterGraphviz( Noeud* N, Data& D, string nomFichier){
    ofstream fichier(nomFichier+".dot");
    if (!fichier) {
        cerr << "Erreur : impossible d'ouvrir le fichier " << "Graphe.dot" << endl;
        return;
    }

    fichier << "digraph Arbre {" << endl;
    fichier << "  node [shape=box];" << endl;

    int id = 0; // Identifiant unique pour chaque noud
    exporterGraphvizRecursif(N, fichier, id, D);

    fichier << "}" << endl;
    fichier.close();
    cout << " Fichier GRAPHVIZ genere : " << nomFichier+".dot" << endl;
}

//--------------------------------------------------------------------------------------------------------------------

 /**
  *\brief Cette methode est appelee recursivement pour parcourir l'arbre de decision et ecrire
  * les informations de chaque noeud
  * \param noeud Noeud*, fichier ofstream&
  * \param id int&, D Data&
  * \return void
  */
 void Arbre::exporterGraphvizRecursif(Noeud* noeud, ofstream& fichier, int& id, Data& D) {
    if (!noeud) return;
    int noeudId = id++; // Identifiant unique pour ce noeud
    // ecrire le noeud actuel avec les informations appropriees
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

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Calcule les predictions pour tous les noeuds feuilles de l'arbre
 * \param noeud Noeud*
 * \param D Data&
 * \return void
 */
void Arbre::calculerPredictionsFeuilles(Noeud* noeud, Data& D) {
    if (!noeud) return;

    // Si c'est une feuille (typeVarOpti = -1)
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
        }
        else {
            // Calcul de la moyenne
            float somme = 0;
            for (int ind : feuille->vectIndiceInd) {
                somme += D.V[ind].getAtQuant(D.indVarY);
            }
            feuille->prediction = to_string(somme/feuille->vectIndiceInd.size());
        }
    } else {
        // Parcourir recursivement les sous-arbres
        calculerPredictionsFeuilles(noeud->Ngauche,D);
        calculerPredictionsFeuilles(noeud->Ndroite,D);
    }
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Predit la classe d'un individu en parcourant l'arbre
 * \param individu DataIndividu&
 * \return string
 */
string Arbre::predire( DataIndividu& individu){
    if (racine == nullptr) {
        throw runtime_error("L'arbre n'a pas été construit.");
    }
    return predireRecursif(racine, individu);
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Predit la classe d'un individu en parcourant de manière recursive l'arbre
 * \param noeud Noeud*,individu DataIndividu&
 * \return string
 */
string Arbre::predireRecursif(Noeud* noeud,  DataIndividu& individu){
    //Noeud Feuille
    if (noeud->typeVarOpti == -1) {
        NoeudFeuille* feuille = static_cast<NoeudFeuille*>(noeud);
        return feuille->prediction;  // Retourne la prediction pre-calculee
    }
    //Noeud Quantitatif
    if (noeud->typeVarOpti == 0){
        if (individu.getAtQuant(noeud->indVarOpti) <= static_cast<NoeudQuantitatif*>(noeud)->seuilOpti) {
            return predireRecursif(noeud->Ngauche,individu);
        } else {
            return predireRecursif(noeud->Ndroite,individu);
        }
    //Noeud Qualitative
    } else  {
        string valeur = individu.getAtQual(noeud->indVarOpti);
        if (static_cast<NoeudQualitatif*>(noeud)->meilleurGroupe.find(valeur) != static_cast<NoeudQualitatif*>(noeud)->meilleurGroupe.end()) {
            return predireRecursif(noeud->Ngauche,individu);
        } else {
            return predireRecursif(noeud->Ndroite,individu);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------

/** \brief Destructeur de la classe BaggingArbre
 */
BaggingArbre::~BaggingArbre(){
    for (Arbre* arbre : arbres){
        delete arbre;
    }
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Genere un echantillon d'indices avec remise de la taille de la base de donnees D
 * \param D Data&
 * \return vector<int>
 */
vector<int> BaggingArbre::genererEchantillon(Data& D){
    vector<int> echantillon;
    static mt19937 gen(random_device{}()^time(nullptr));
    uniform_int_distribution<> dis(0, D.V.size()-1);
    // Generer un echantillon de taille D.V.size() avec remise
    for (size_t i = 0; i < D.V.size(); ++i) {
        echantillon.push_back(dis(gen));
    }
    return echantillon;
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Entraine une foret d'arbres par bagging
 * \param D Data&
 * \return void
 */
void BaggingArbre::entrainer(Data& D){
    // Creer et entrainer nombreux  arbres
    for (int i = 0; i < nbrArbres; ++i){
        Arbre* arbre = new Arbre();
        vector<int> echantillon = genererEchantillon(D);
        arbre->creer_Arbre(D,echantillon,"ArbreNum"+to_string(i)+"Bagging");
        arbres.push_back(arbre);
    }
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Predit la classe/valeur d'un individu par vote majoritaire/moyenne.
 * \param D Data&, individu DataIndividu&
 * \return string
 */
string BaggingArbre::predire(Data& D,DataIndividu& individu){
    if (arbres.empty()) {
        throw runtime_error("Aucun arbre n'a été entrainé.");
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
    } else {
        // Moyenne des predictions
        float somme = 0;
        for (Arbre* arbre : arbres){
            string pred = arbre->predire(individu);
            somme += stof(pred); // Convertir la string en float
        }
        return to_string(somme/nbrArbres);
    }
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * \brief Execute le processus complet de bagging (entrainement + prediction)
 * \param D Data&
 * \param individuTest DataIndividu&
 * \return void
 */
void BaggingArbre::executerBagging(Data& D, DataIndividu& individuTest){
    entrainer(D);
    string prediction = predire(D,individuTest);
    cout << "Prediction pour l'individu de test : " << prediction << endl;
}


// Évalue le modèle sur un test_set, renvoie un vecteur de prédictions et la performance (accuracy ou R²)
pair<vector<string>, float> Arbre::evaluerTestSet(Data& test_set) {
    vector<string> predictions; // Stocke les prédictions
    int indexY = test_set.indVarY; // L’indice de la variable cible
    string typeY = test_set.TypeVar[indexY]; // Type de la variable cible : "quali" ou "quanti"

    float performance = 0.0;

    // Variables pour R² score
    vector<float> y_true; // Valeurs réelles
    vector<float> y_pred; // Valeurs prédites
    float somme = 0.0;

    for (DataIndividu& individu : test_set.V) {
        // Prédiction avec l’arbre de décision
        string prediction = this->predire(individu);
        predictions.push_back(prediction);

        if (typeY == "quali") {
            // Accuracy : compare la prédiction avec la vraie valeur
            string true_val = individu.getAtQual(indexY);
            if (prediction == true_val) {
                performance += 1.0;
            }
        } else if (typeY == "quanti") {
            // R² score : nécessite des valeurs numériques
            float y = individu.getAtQuant(indexY);
            float y_hat = stof(prediction); // Convertit la prédiction en float

            y_true.push_back(y);
            y_pred.push_back(y_hat);
            somme += y;
        }
    }

    if (typeY == "quali") {
        // Accuracy = nombre de prédictions correctes / total
        performance /= test_set.V.size();
    } else if (typeY == "quanti") {
        float y_bar = somme / y_true.size(); // Moyenne de y
        float ss_tot = 0.0;
        float ss_res = 0.0;

        for (size_t i = 0; i < y_true.size(); ++i) {
            ss_tot += pow(y_true[i] - y_bar, 2);
            ss_res += pow(y_true[i] - y_pred[i], 2);
        }

        performance = 1 - (ss_res / ss_tot); // R²
    }

    return {predictions, performance};
}
