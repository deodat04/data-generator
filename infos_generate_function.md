# Données à générer
- Nombres entiers
- Nombres flottants
- autres données : mots[longueurs variables], images, coordonnees, date, coordonnees GPS

# Désordre
- Niveau de désordre varie entre [0,100]
  En fonction de la valeur entrée le désordre est amplifiée
- Options de désordre : *Aléatoire total*, *Debut*, *Milieu*, *Fin*, *Reverse*

# Paramètre reçu
- Nombre exact d'éléments à générer
- Valeur du niveau de désordre
- Options de désordre
- Choix de type de  données à générer

# Compile and build
`gcc  main.c generate_data.c metrics.c -o main -lm`

`./main nb_element disorder_level option_level  type_data`

i.e : `./main 10 70.0 Debut int`

# Output
`output.csv`

# source
- [Dictionnaire de mots](https://github.com/chrplr/openlexicon/blob/master/datasets-info/Liste-de-mots-francais-Gutenberg/README-liste-francais-Gutenberg.md)
- [TP entropie](`https://www.fil.univ-lille.fr/~salson/portail/cdc/tp/TP-Entropie/enonce.html#mise-en-%C5%93uvre-du-calcul-de-lentropie`)
- [Informations Entropie de Shannon](`https://spip.teluq.ca/inf6460/spip.php?article110`)
- [Règle de la fonction sinus](`https://www.alloprof.qc.ca/fr/eleves/bv/mathematiques/la-recherche-de-la-regle-d-une-fonction-sinus-m1173`)
- [Corrélation de Pearson et test statistique](`http://www.biostat.ulg.ac.be/pages/Site_r/corr_pearson.html`)
- [Génération image aléatoire](`https://github.com/yulvil/gopherjs-examples/blob/master/02-base64img/base64img.go`)
- [Fichier création image](`https://github.com/nothings/stb/blob/master/stb_image_write.h`)


# Entropie
## objectif
Mesurer l'entropie de Shannon sur les permutations réalisées par la fonction de désordre.
- Entropie par permutation proche de 1 : La majorité des éléments ont changé de position par rapport à leur ordre initial(désordre élevé).
- Entropie par permutation proche de 0 : Peu ou aucun élément n'a changé de position(désordre faible).

## procedure
- Identifier les éléments déplacés.
- Calculer la probabilité de déplacement.
- Utiliser cette probabilité pour déterminer l'entropie.

#  Quantification du désordre

## Génération des données
- générer la liste des valeurs suivant la regle f(x) = asin(b(x-h)) + k //etat inital
- application de la fonction de désordre sur la liste générer //etat de desordre

## Mesure
### Corrélation de Pearson et Test statistique
- La corrélation de Pearson mesure à quel point la liste désordonnée ressemble encore à la liste originale.
Une valeur proche de 1 signifie que la structure est conservée, tandis qu’une valeur proche de 0 indique un désordre total.

- Le test statistique permet de vérifier si cette corrélation est significative ou si elle pourrait être due au hasard.

### Corrélation avec la courbe sinusoïdale 
On compare la liste désordonnée avec la liste de référence en calculant : 
- Le coefficient de corrélation de Pearson entre les deux séries de valeurs.
- Le test statistique à partir de la corrélation de Pearson

### Comparaison
Soit r la Corrélation et t le test
- r≈1 ==> *Corrélation forte et positive*. (Peu de désordre introduit)
- r≈0  ==> *Aucune corrélation*. (Désordre chaotique)
- r≈−1  ==> *Liste complètement inversée*.
- Si la valeur de t est élevée, cela signifie que la corrélation observée est *statistiquement significative* et qu’elle ne résulte pas du hasard.
- Si la valeur de t est faible, cela signifie que la corrélation obtenue peut être due au bruit et n’est *pas fiable*.


### Nombre d'inversions (Mesure classique du désordre)
- soit  Une inversion est une paire  (𝑖,𝑗)(i,j) telle que 𝑖<𝑗i<j mais 𝑦[𝑖]>𝑦[𝑗]y[i]>y[j].
- Une liste déjà triée a 0 inversion.
- Une liste totalement aléatoire a environ 𝑁(𝑁−1)/4 inversions.
- Une liste presque triée aura un faible nombre d’inversions.

*Formule du nombre d'inversion*

D = Nombre d'inversion observées / Nombre maximum d'inversion
- Si D est proche de 0, le désordre est faible → La permutation facilite le tri.
- Si D est proche de 1, le désordre est élevé → La permutation complique le tri.