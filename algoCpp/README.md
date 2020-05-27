## QueryOptimizer

###Lancement de l'application
Actuellement, QueryOptimizer ne supporte que les ordinateurs d'architecture d'OSX et Linux.
Dans le dossier "build" il y a deux différents d'exécutables : qt_main_osx et qt_main_linux. En fonction de l'architecture de votre ordinateur lancer depuis un terminal un de ces deux fichiers.

###Cloner le projet
Pour cloner le projet, dans un terminal, taper : "git clone https://github.com/EzekielaRakotoarijaona/OlapMod.git"

###Cmake
Le projet compile avec "cmake" ! Veillez à l'installer sur votre machine.

###Librairie qt
Pour installer Qt sur une machine : 
1. Linux, aller sur ce lien : [https://wiki.qt.io/Install_Qt_5_on_Ubuntu] (Qt5)
2. OSX, aller sur ce lien : [https://formulae.brew.sh/formula/qt] (Qt5). Il se peut que vous ayez à configurer les liens permettant à cmake de trouver Qt. La commande à taper ressemblerait à celle-ci : "export CMAKE_PREFIX_PATH=/usr/local/Cellar/qt5/5.7.0/". Veillez à spécifier le bon chemin de votre installation de Qt.

###Librairie openmp
1. Linux : La librairie devrait déjà être installée, sinon veuillez l'installer.
2. OSX : Pour l'installer, lancer la commande : "brew install libomp". Pour compiler il faudra utiliser un compilateur g++. Si vous ne l'avez pas installé, lancer la commande "brew install gcc".

###Compilation
Dans les deux cas (Linux ou OSX), il faut se placer dans le dossier "build".
1. Linux : Taper la commande "cmake .." suivi par la commande "make".
2. OSX : Taper la commande "cmake -DCMAKE_CXX_COMPILER="/usr/local/bin/g++-9" .." suivi par la commande "make".

###Lancement de l'application avec les exécutables générés
Trois exécutables sont générés après compilation. Pour lancer l'application en version interface graphique, lancer "qt_main". Une version du terminal est disponible avec l'exécutable "main" et l'exécutable "main_test" est l'exécutable lancant les tests.

