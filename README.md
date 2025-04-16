<<<<<<< HEAD
# **Projet : Arbre de Décision avec Bagging en C++**
=======
# **Projet : Arbre Binaire de Décision avec Bagging en C++**
>>>>>>> ad321c6b164b84bd9cc65a989bcef0c3088699db

Ce projet implémente un arbre de décision en C++ avec la méthode du **bagging** pour la classification. L'arbre de décision construit utilise un algorithme de division basé sur les critères de **Gini** ou de **variance** pour les variables qualitatives et quantitatives. Le projet génère également une visualisation de l'arbre sous forme de fichier `.dot`, que vous pouvez utiliser pour créer une représentation graphique de l'arbre avec Graphviz.

## **Structure du Code**

### **Classes Principales**

1. **Arbre**
   - **Rôle** : Représente l'arbre de décision. Il contient les méthodes pour créer l'arbre, diviser les nœuds, calculer les prédictions et exporter l'arbre en format `.dot`.
   - **Méthodes principales** :
     - `creer_Arbre` : Initialise l'arbre avec la racine, puis appelle la méthode récursive `creerNoeud` pour construire l'arbre à partir des données.
     - `creerNoeud` : Crée un nœud dans l'arbre de manière récursive en divisant les données en sous-ensembles, et détermine la meilleure division basée sur le critère de gain (Gini ou variance).
     - `diviserNoeud` : Divise un nœud en deux en fonction de la meilleure séparation trouvée pour une variable.
     - `critereArret` : Critères d'arrêt pour la construction de l'arbre (par exemple, taille minimale d'un nœud, ou pureté maximale).
     - `exporterGraphviz` et `exporterGraphvizRecursif` : Exportent l'arbre sous forme de fichier `.dot` pour visualisation avec Graphviz.
     - `calculerPredictionsFeuilles` : Calcule les prédictions des nœuds feuilles de l'arbre.
     - `predire` : Prédit la classe d'un individu en suivant le chemin de l'arbre jusqu'à une feuille.

2. **Noeud** (Classe de base pour les nœuds de l'arbre)
   - **Rôle** : Représente un nœud générique de l'arbre (nœud interne ou feuille).
   - **Méthodes principales** : 
     - La classe est une classe de base qui est étendue par des classes spécifiques comme `NoeudFeuille`, `NoeudQuantitatif` et `NoeudQualitatif`.

3. **NoeudFeuille**
   - **Rôle** : Représente un nœud feuille de l'arbre, où une prédiction est effectuée.
   - **Attributs** : `prediction` (classe prédite) et `vectIndiceInd` (indices des individus dans ce nœud).

4. **NoeudQuantitatif**
   - **Rôle** : Représente un nœud interne avec une variable quantitative.
   - **Attributs** : `seuilOpti` (seuil de division).

5. **NoeudQualitatif**
   - **Rôle** : Représente un nœud interne avec une variable qualitative.
   - **Attributs** : `meilleurGroupe` (groupe optimal de valeurs qualitatives pour diviser).

6. **Data**
   - **Rôle** : Représente l'ensemble des données utilisées pour l'apprentissage et la classification.
   - **Méthodes principales** :
     - Contient les données sur les variables (`V`), les noms des variables (`NomVar`), leurs types (`TypeVar`), et les catégories des variables qualitatives (`cat`).

7. **DataIndividu**
   - **Rôle** : Représente un individu dans l'ensemble de données, avec des méthodes pour accéder à ses attributs qualitatifs et quantitatifs.

### **Méthodes Importantes**

- **Calcul du gain de Gini et de variance (VI)** :
  - Le gain de Gini et le gain de variance sont utilisés pour déterminer la qualité des divisions d'un nœud en fonction de la variable utilisée pour la division. Plus le gain est élevé, plus la division est bonne.

- **Critères d'arrêt** :
  - L'arbre cesse de se diviser si la taille du nœud est inférieure à un seuil donné (par exemple, 10 individus), ou si la pureté (mesurée par Gini ou VI) est maximale.

- **Prédiction** :
  - Une fois l'arbre construit, on peut prédire la classe d'un individu en suivant le chemin de l'arbre en fonction des valeurs de ses attributs.

### **Fonctionnalités supplémentaires**

- **Visualisation de l'arbre** : 
  Le fichier `.dot` généré permet de visualiser l'arbre avec Graphviz. Chaque nœud est étiqueté avec la variable utilisée pour la division, les seuils ou groupes, et la prédiction des feuilles.
