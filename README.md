# Arno Durandeau

# Jeu de Tennis en Grapic

## But du projet

Je suis un étudiant qui aime beaucoup jouer au tennis et qui en pratique depuis tout petit, alors avec les thèmes présentés dans l'UE, c'était logique pour moi de programmer un jeu de tennis.  
Mais seulement un jeu de tennis, une balle qui rebondit et deux raquettes, ça aurait été un peu simple.
J'ai donc réfléchit à tout ce que je pouvais ajouter, pour rendre le jeu plus réaliste et plus amusant.

Le jeu est vu de coté.

## Contenu

### Niveaux de difficultés:

Plusieurs niveaux de difficulté, inspirés des 3 principales [surfaces de terrain utilisées dans la compétition](https://fr.wikipedia.org/wiki/Surfaces_de_jeu_au_tennis): 
* [la terre battue](https://fr.wikipedia.org/wiki/Surfaces_de_jeu_au_tennis#Terre_battue)
* [le gazon](https://fr.wikipedia.org/wiki/Surfaces_de_jeu_au_tennis#Gazon)
* [l'asphalte](https://fr.wikipedia.org/wiki/Surfaces_de_jeu_au_tennis#Dur)

![Les surfaces](Surfaces.png "Les différentes surfaces sur de vrais terrains")

Pour le fun, il y a aussi un niveau sur la lune.  
Plusieures choses changent lorsque la difficulté augmente:
* La force initiale de la balle est plus élevée
* La friction de la balle avec le sol augmente 
* La longueur des raquettes diminue  
* Les coups spéciaux (dont je parlerai après) ont plus d'impact sur la balle
* Et évidemment le fond change de couleur selon la difficulté  

Je tiens quand même à préciser que ces "niveaux" ne sont pas représentatif de la difficulté des terrains dans la vraie vie. La terre battue, le gazon et le "dur" (c'est techniquement de l'asphalte) représentent chacun une difficulté, et souvent les joueurs ont leur préférence quant à ces surfaces. Exemple: [Rafael Nadal](https://fr.wikipedia.org/wiki/Rafael_Nadal) est meilleur sur terre battue (rolland garos), [Roger Federer](https://fr.wikipedia.org/wiki/Roger_Federer) est meilleur gazon (wimbledon) et [Novak Djokovic](https://fr.wikipedia.org/wiki/Novak_Djokovic) est polyvalent mais meilleur sur du dur (open d'australie). Ces 3 joueurs représentent le top 3 mondial du tennis en score ATP.

Bref, revenons à notre jeu en C/C++ Grapic.

### Engagement:
L'engagement (service) a lieu au centre du terrain, du côté du joueur qui a perdu le point précédent.  

### Points et échanges:
Les points sont comptés comme un "**SET**" en compétition, on compte d'abord les points dans l'ordre: *0, 15, 30, 40*. 
Lorsqu'un joueur arrivé à *40* marque un point, les deux joueurs repartent à *0*, et le joueur qui a gagné le dernier point gagne 1 "**Jeu**".
Le premier joueur à **6 Jeux** a gagné le SET, ici, la partie.  
En compétition une "partie" ou match, se joue en 3 SET gagnants, mais cela rendrait le jeu moins amusant.  

Un point vous est attribué si votre adversaire ne rattrape pas la balle avec sa raquette, ou, en difficulté "Asphalt", si votre adversaire tire dans le filet.  
Le cas du filet est compté uniquement en 3ème difficulté, pour la simple raison qu'il est plus simple de tirer dans le filet lorsque la balle a moins de vitesse.

Il y a également un compteur d'échanges au centre-haut de la fenêtre, ainsi que le meilleur nombre d'échanges de la partie, pour se mettre au défi de battre son record.

### Coups spéciaux:
Non, on ne peut pas faire de coups comme dans "Mario Tennis Aces" avec des attaques spéciales et des attaques enflammées.  
En revanche, dans ce jeu, on peut éffectuer tous les coups spéciaux du tennis, à savoir:
* **Amorti** : Son but est de ralentir la balle sans que l'adversaire ne s'y attende.
* **Smash** : Son but est d'accélerer la balle pour qu'elle soit plus dure à rattraper.
* **Vollée** : Son but est de se rapprocher du filet pour rattraper un amorti, ou pour surprendre l'adversaire.
* **Lob** : Son but est de rendre la trajectoire de la balle plus parabollique pour, par exemple, contrer la Vollée.  

Un bouton est associé pour chacun de ces coups (voir onglet commande). Rester appuyé sur ce bouton le rend plus puissant (LOB plus haut, SMASH plus rapide, AMORTI plus lent), mais attention, ces coups sont limités et à utiliser avec précaution.  
*Le fait qu'il faille rester appuyer sur le bouton pour accentuer le coup n'était pas prévu et était un bug, mais j'ai trouvé la mécanique de jeu intéressante. J'ai donc laissé cette mécanique et je n'ai pas patché le bug.*

### Physique

La physique de la balle fonctionne selon la seconde loi de Newton, qui dit que la somme des forces sur un objet est égale au produit de sa masse et de son accéleration.  
Ici notre balle a comme masse m=1g, elle a donc, dans le modèle utilisé, comme seule force son accéleration.  
Lors de l'engagement, on donne à la balle une force initiale **horizontale**. Ensuite, par souci de difficulté dans le jeu, la balle n'accelerera pas tout au long des points (sinon sa vitesse augmenterait de façon exponentielle).  
Aussi, on supposera que la balle n'a que très peu de friction avec le sol, pour qu'elle ne touche pas trop vite le filet.  
Pour finir, à chaque contact avec les raquettes, on ajoute un vecteur **vertical** positif à l'acceleration de la balle, cela permet à la balle de conserver une trajectoire réaliste (pour un jeu de tennis).  
J'en convient, toutes ces légères modifications rendent le jeu moins réaliste dans son aspect physique. A l'inverse, ces modifications rendent le jeu plus réaliste dans son aspect tennistique, et c'est ce qui m'intéressait pour de ce projet.

Pour les equations de l'acceleration, de la vitesse, ainsi que de la position **en fonction du temps**, j'ai utilisé celles qui nous étaient proposées dans le cours de LIFAMI  
![Les equations de force](Equations.png "Les equations de force"). 

### Commandes
Les commandes sont plutôt simples, et son expliquées dans le programme, mais un petit schéma réalisé à la main ne fera pas de mal:  
![schéma des commandes du jeu](Commandes.jpg "Commandes")

## Explication du code

### Données
Les principales données utilisées dans ce code sont:  
Une **balle**, constituée de sa masse, sa position, sa vitesse, et son accélération.  
Les **deux Joueurs**, constitués des positions de leurs raquettes et leur longueur, l'information "est ce que le joueur a gagné le point précédent" et la valeur prise par la jauge de smash/amortis/lob effectués.  
Le **filet**, constitué de sa position.  
Ainsi qu'une structure de donnée comportant le **jeu** en entier, c'est à dire:  
tout ce qui a été cité précédemment, mais également les scores, la difficulté, le nombre d'échanges effectués, l'information du dernier joueur qui a frappé la balle, et pour l'état du point (finit ou pas) et de la partie (finie ou pas).  

Toutes ces données serviront au bpn fonctionnement du programme.

### Déroulement du programme

#### Initialisation
D'abord, le programme initialise chaque valeur citée précédemment.  
La balle est placée au centre de la fenêtre, avec une force initale nulle.  
Les raquettes sont placées de chaque cotés de la fenêtre, avec une longueur qui varie selon la difficulté.  
Le filet est placé au centre de la fenêtre, comme sur un vrai terrain.  
Chaque score est mit à 0. Pareil pour le nombre d'échanges.  
Pour le premier point, c'est le joueur à droite qui devra frapper la  balle en premier.

#### Mise à jour (chaque 1/15 ou 1/60 de seconde selon l'ordinateur)

Lors du tout premier coup, le vecteur acceleration de la balle prendra une nouvelle valeur dans sa coordonée x.  
Sinon, le programme applique la seconde loi de newton sur la balle, pour mettre à jour la position de la balle. Il fait en sorte que la balle rebondisse au sol (càd la valeur y=0) et que la balle rebondisse dans chaque raquettes.  
Lorsque la balle est proche de la raquette d'un joueur, et que c'est à son tour de frapper, le programme vérifie si le joueur appuie sur un des boutons associés aux "coups spéciaux" mentionnés précédemment. Si c'est le cas, la balle empruntera une trajectoire différente selon le coup spécial, et la valeur de la jauge de ce coup spécial augmentera. Si cette dite jauge atteint son MAX, le joueur ne pourra plus utiliser le coup spécial. 
