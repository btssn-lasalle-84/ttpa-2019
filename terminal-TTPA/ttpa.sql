pragma foreign_keys = on;

CREATE TABLE joueurs (
    "idJoueur" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
    "nom" VARCHAR(255) NOT NULL UNIQUE
);

INSERT INTO joueurs(nom) VALUES('LEGOUT Christophe');
INSERT INTO joueurs(nom) VALUES('MARTINEZ Michel');
INSERT INTO joueurs(nom) VALUES('LEBESSON Emmanuel');
INSERT INTO joueurs(nom) VALUES('ELOI Damien');
INSERT INTO joueurs(nom) VALUES('MATTENET Adrien');
INSERT INTO joueurs(nom) VALUES('CHILA Patrick');
INSERT INTO joueurs(nom) VALUES('BEAUMONT Jérôme');

CREATE TABLE parametres (
    "idParametres" INTEGER PRIMARY KEY AUTOINCREMENT,
    "nbBalles" INTEGER NOT NULL,
    "nbBallesMinute" INTEGER NOT NULL,
    "effet" INTEGER NOT NULL,
    "puissance" INTEGER NOT NULL,
    "rotation" INTEGER NOT NULL,
    "zoneObjectif" INTEGER NOT NULL,
    "zoneRobot" INTEGER NOT NULL    
);

INSERT INTO parametres(nbBalles, nbBallesMinute, effet, puissance, rotation, zoneObjectif, zoneRobot) VALUES(10, 20, 1, 0, 1, 4, 5);
INSERT INTO parametres(nbBalles, nbBallesMinute, effet, puissance, rotation, zoneObjectif, zoneRobot) VALUES(20, 30, 0, 20, 0, 6, 3);

CREATE TABLE profils (
    "idProfil" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
    "idJoueur" INTEGER NOT NULL, 
    "idParametres" INTEGER NOT NULL,         
    "nom" VARCHAR(255) NOT NULL UNIQUE,
    CONSTRAINT fk_profils_1 FOREIGN KEY (idJoueur) REFERENCES joueurs (idJoueur) ON DELETE CASCADE,
    CONSTRAINT fk_profils_2 FOREIGN KEY (idParametres) REFERENCES parametres (idParametres) ON DELETE CASCADE
);

INSERT INTO profils(idJoueur, idParametres, nom) VALUES(7, 1, 'Séance balles liftés zone 4');
INSERT INTO profils(idJoueur, idParametres, nom) VALUES(3, 2, 'Séances balles coupées zone 6');

CREATE TABLE seances (
    "idSeance" INTEGER PRIMARY KEY AUTOINCREMENT,
    "idProfil" INTEGER NOT NULL,     
    "reussite" REAL NOT NULL,
    "horodatage" DATETIME NOT NULL,    
    CONSTRAINT fk_seances_1 FOREIGN KEY (idProfil) REFERENCES profils (idProfil) ON DELETE CASCADE
);

INSERT INTO seances(idProfil, reussite, horodatage) VALUES(1, '60', datetime('now', 'localtime'));
INSERT INTO seances(idProfil, reussite, horodatage) VALUES(1, '65', datetime('now', 'localtime'));
INSERT INTO seances(idProfil, reussite, horodatage) VALUES(2, '45', datetime('now', 'localtime'));

CREATE TABLE ttpa (
	"idTTPA" INTEGER PRIMARY KEY CHECK (idTTPA = 1),
	"idProfil" INTEGER NOT NULL,     
	CONSTRAINT fk_ttpa_1 FOREIGN KEY (idProfil) REFERENCES profils (idProfil) ON DELETE CASCADE
);

INSERT INTO ttpa(idTTPA, idProfil) VALUES(1, 1);


-- SELECT joueurs.nom FROM profils INNER JOIN joueurs ON profils.idJoueur = joueurs.idJoueur

--  SELECT effet, nbBalles, nbBallesMinute, puissance, rotation, zoneObjectif, zoneRobot FROM parametres INNER JOIN profils ON profils.idParametres = parametres.idParametres

-- SELECT joueurs.nom FROM joueurs INNER JOIN seances, profils ON seances.idProfil = profils.idProfil WHERE profils.idJoueur = joueurs.idJoueur
