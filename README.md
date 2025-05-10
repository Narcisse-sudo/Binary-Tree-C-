
# 🌳 Projet : Arbres de décision et prédiction par *bagging* d’arbres en C++

Ce projet implémente un **arbre de décision en C++** avec la méthode du **bagging** pour la classification.  
L’arbre utilise des **critères de Gini** (variables qualitatives) ou de **variance** (variables quantitatives) pour construire les divisions optimales.

Une fonctionnalité de **visualisation** est incluse via l’export `.dot` compatible avec **Graphviz**.

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
