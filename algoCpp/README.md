## Run c++ algorithm

Pré-requis : qt creator
Pour installer qtcreator sur une machine linux 
1. Ouvrir le terminal
2. Passer en mode root avec la commande : su
3. Une fois en mode root, taper les commandes : 
  3.1 Une fois en mode root, suivez ce tutoriel (documentation pas obligatoire) : https://lucidar.me/fr/dev-c-cpp/how-to-install-qt-creator-on-ubuntu-16-04/
4. Voilà

Pré-requis : Git
Pour installer git sur une machine linux 
1. Ouvrir le terminal
2. Passer en mode root avec la commande : su
3. Une fois en mode root, taper la commande : sudo apt-get install git
4. Voilà

Pré-requis : Cloner le projet
Pour cloner le projet sur qt creator
1. Ouvrir qt creator
2. Fichier -> Nouveau fichier ou projet -> import project -> git clone
3. Dans repository coller l'URL que vous trouverez dans le git
4. Voilà

Pré-requis : Cmake
Pour installer cmake sur une machine linux 
1. Ouvrir le terminal
2. Passer en mode root avec la commande : su
3. Une fois en mode root, taper la commande : sudo apt-get install cmake
4. Voilà

Assuming you have installed cmake on your computer :

* Open a terminal in the /build and run "cmake .." . This command will generate a Makefile.
* Then run "make" to generate your exec.
* You can now run your exec named main with "./main".

To clear all the generated files by cmake :

* delete all the files in the build folder

Dernière étape pour ouvrir votre projet dans qt creator : 
1. Ouvrir qt creator
2. Fichier -> Nouveau fichier ou projet -> importer un projet existant -> Olap mod ->algoCPP
3. Suivez les instructions
4. Voilà

