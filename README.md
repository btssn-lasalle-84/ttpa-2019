# Le projet TTPA 2019 (Table Tennis Performance Analyser)

Le système TTPA doit permettre une analyse des performances du joueur (côté relanceur). Il doit proposer une phase d'entraînement adaptée au niveau du joueur, puis de détecter l'impact des balles afin d'afficher le rythme de jeu, la précision, le pourcentage de réussite. La zone d'impact (côté distributeur) est identifiée sur un écran de télévision en fin d'exercice. Le pourcentage de balles dans chacune des zones, le rythme de jeu et le pourcentage de réussite sont disponibles en fin d'exercice. Le joueur lance un exercice spécifique et pourra connaître son évolution individuelle.

- [Le projet TTPA 2019 (Table Tennis Performance Analyser)](#le-projet-ttpa-2019-table-tennis-performance-analyser)
  - [Auteurs](#auteurs)
  - [Productions](#productions)
  - [Vidéo](#vidéo)
  - [Base de données SQLite](#base-de-données-sqlite)
  - [Licence GPL](#licence-gpl)

## Auteurs

- Module Écran : Yohann BOTELLA--BROC <botellabroc.yohann@gmail.com>
  - Le système d’exploitation est installé et fonctionnel
  - L'écran est configuré en mode "kiosque"
  - La zone d’impact est identifiée et affichée en temps réel
  - Les données de la séance sont affichées en temps réel
  - Les liaisons sans fil sont opérationnelles
  - Les informations sont affichées en fin de séquence

- Module Application Mobile : Youssef HAMMOUMA <yhammouma@gmail.com>
  - La base de données est fonctionnelle et complétée
  - Le système est paramétrable
  - La liaison Bluetooth est fonctionnelle
  - Les informations de paramétrages sont transmises
  - L'application mobile est déployée

## Productions

- [dossier-ttpa-ir-2019.pdf](dossier-ttpa-ir-2019.pdf)
- [refman-ttpa.pdf](refman-ttpa.pdf)
- [presentation-hammouma-2019.pdf](presentation-hammouma-2019.pdf)
- [presentation-botella-2019.pdf](presentation-botella-2019.pdf)

## Vidéo

Lien : https://www.youtube.com/watch?v=WZrcLZjM4dY

## Base de données SQLite

```sql
CREATE TABLE joueurs ( "idJoueur" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "nom" VARCHAR(255) NOT NULL UNIQUE )

CREATE TABLE parametres ( "idParametres" INTEGER PRIMARY KEY AUTOINCREMENT, "nbBalles" INTEGER NOT NULL, "nbBallesMinute" INTEGER NOT NULL, "effet" INTEGER NOT NULL, "puissance" INTEGER NOT NULL, "rotation" INTEGER NOT NULL, "zoneObjectif" INTEGER NOT NULL, "zoneRobot" INTEGER NOT NULL )

CREATE TABLE profils ( "idProfil" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "idJoueur" INTEGER NOT NULL, "idParametres" INTEGER NOT NULL, "nom" VARCHAR(255) NOT NULL UNIQUE, CONSTRAINT fk_profils_1 FOREIGN KEY (idJoueur) REFERENCES joueurs (idJoueur) ON DELETE CASCADE, CONSTRAINT fk_profils_2 FOREIGN KEY (idParametres) REFERENCES parametres (idParametres) ON DELETE CASCADE )

CREATE TABLE seances ( "idSeance" INTEGER PRIMARY KEY AUTOINCREMENT, "idProfil" INTEGER NOT NULL, "reussite" REAL NOT NULL, "horodatage" DATETIME NOT NULL, CONSTRAINT fk_seances_1 FOREIGN KEY (idProfil) REFERENCES profils (idProfil) ON DELETE CASCADE )

CREATE TABLE ttpa ( "idTTPA" INTEGER PRIMARY KEY CHECK (idTTPA = 1), "idProfil" INTEGER NOT NULL, CONSTRAINT fk_ttpa_1 FOREIGN KEY (idProfil) REFERENCES profils (idProfil) ON DELETE CASCADE )
```

## Licence GPL

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
