# Änderungen/ Anpassungen
## Software Engineering
### Julian Sobott, Tobias Pfisterer, Lukas Mendel, David Sugar

Die folgenden Einträge beschreiben Änderungen, die während der Implementierung
des SE-Projekts aufgetreten sind.

1. Anpassung der Vererbungshierarchie von Arbeit und seinen Unterklassen:
	* Die Oberklasse Arbeit wurde durch die Unterklasse SonstigesProjekt ersetzt.
	* Projektarbeit und Abschlussarbeit erben beide von SonstigesProjekt
2. Die Klasse DB hat für jedes der Objekte eine eigene update() methode erhalten:
	* Updates werden über update() in die Datenbank eingespielt und nicht wie geplant über add
