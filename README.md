
# 🌳 Projet : Arbres de décision et prédiction par *bagging* d’arbres en C++

Ce projet a pour objectif de concevoir et d’impl´ementer en C++ un modèle d’arbre binaire de
décision capable de traiter des données quantitatives et qualitatives, tant en classification qu’en
régression. L’approche repose sur la construction récursive de l’arbre en utilisant des crit`eres
d’homog´en´eit´e tels que la variance pour les variables continues et l’indice de Gini pour les
variables cat´egorielles.
L’architecture du code repose sur des classes hi´erarchis´ees (Noeud, Arbre, Data, Attri-
but, DataIndividu) assurant une modularit´e et une clart´e dans la construction des arbres.
Le rapport d´etaille ´egalement l’ajout d’une fonctionnalit´e de visualisation automatique via la
g´en´eration de fichiers .dot compatibles avec Graphviz, facilitant l’interpr´etation graphique
des arbres g´en´er´es.
Une extension du projet a ´et´e r´ealis´ee `a travers l’int´egration de l’algorithme Bagging (Boots-
trap Aggregating), permettant d’am´eliorer la stabilit´e et la pr´ecision des pr´edictions par la
combinaison de plusieurs arbres construits sur des ´echantillons bootstrap´es.


---

## 📂 Structure du Code

### 🧩 Classes Principales

#### 1. `Arbre`
- **Rôle** : Crée et entraîne l'arbre de décision.
- **Méthodes principales** :
  - `Gini` : Calcule l’hétérogénéité pour une variable qualitative.
  - `gainGini` : Mesure le gain d’information selon l’indice de Gini.
  - `Variance` : Calcule l’hétérogénéité pour une variable quantitative.
  - `gainVariance` : Mesure le gain d’information selon la variance.
  - `creerNoeud`, `diviserNoeud` : Construction récursive des nœuds.
  - `critereArret` : Conditions d’arrêt de croissance de l’arbre.
  - `exporterGraphviz` : Génère un `.dot` pour visualisation.
  - `calculerPredictionsFeuilles` : Affecte une prédiction à chaque feuille.
  - **Public** :
    - `predire` : Prédit la classe d’un individu.
    - `evaluerTestSet` : Évalue l’arbre (renvoie prédictions + score).
    - `creer_Arbre` : Initialise l’arbre.

#### 2. `BaggingArbre`
- **Rôle** : Entraîne plusieurs arbres sur des sous-échantillons bootstrapés.
- **Méthodes** :
  - `entrainer` : Génére plusieurs jeux bootstrap et entraîne un arbre sur chacun.

#### 3. `Noeud` *(classe de base)*
- **Rôle** : Représente tout type de nœud.
- Étendue par :
  - `NoeudFeuille`
  - `NoeudQuantitatif`
  - `NoeudQualitatif`

#### 4. `NoeudFeuille`
- **Rôle** : Nœud terminal avec une prédiction.
- **Attributs** : `prediction`, `vectIndiceInd`.

#### 5. `NoeudQuantitatif`
- **Rôle** : Nœud avec une variable quantitative.
- **Attributs** : `seuilOpti`.

#### 6. `NoeudQualitatif`
- **Rôle** : Nœud avec variable qualitative.
- **Attributs** : `meilleurGroupe`.

#### 7. `Data`
- **Rôle** : Stocke les données d’entrée.
- **Contenu** :
  - `V`, `NomVar`, `TypeVar`, `cat`.
  - `Train_Test_Split` : Séparation entraînement/test.

#### 8. `DataIndividu`
- **Rôle** : Représente un individu avec ses attributs.

---

## 🧠 Méthodes importantes

### 🎯 Critères de division
- **Gini** : Utilisé pour les variables qualitatives.
- **Variance** : Utilisée pour les variables quantitatives.
- Le **gain** (d'information) permet de choisir la variable et la coupure optimale.

### 🛑 Critères d'arrêt
- Nombre minimum d’individus dans un nœud (ex : 10)
- Pureté maximale du nœud

### 🔮 Prédiction
- L’individu est classé en suivant les branches de l’arbre jusqu’à une feuille.

---

## 🖼️ Visualisation

L’arbre est exporté en `.dot` :
```bash
dot -Tpng arbre.dot -o arbre.png
