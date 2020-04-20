## Run c++ Program

Pré-requis : Librairies OpenMp et Qt Sdk installées (disponible sur le site offciel de Qt)


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
* You can now run the terminal version your exec named main with a parameter with "./main {path_to_csv_file}".
* To run the GUI version just launch "./qt_main" : from there, you can open a csv file and choose your options.


To clear all the generated files by cmake :

* delete all the files in the build folder

Dernière étape pour ouvrir votre projet dans qt creator : 
1. Ouvrir qt creator
2. Fichier -> Nouveau fichier ou projet -> importer un projet existant -> Olap mod ->algoCPP
3. Suivez les instructions
4. Voilà

Utilisation de git pour les personnes qui n'ont jamais utilsié le développement collaboratif
1- Coder vos méthodes vos tests et correction
2- IL faut impérativement à chaque début d'utilisation du projet taper la commande "git pull --rebase" dans le cmd à la racine du projet
3- A fin de vos développement, faites un "git pull --rebase" au cas ou, puis faites un "git commit [US-X] : decrivez vos changement
4- Une fois le commit bien passé faites git push
5 Faites la commande "git status" pour vérifier que votre commit est bien passé 

