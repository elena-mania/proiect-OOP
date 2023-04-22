# proiect-OOP
## Implementarea unui Musicplayer
Tema aleasă este reprezentată de o aplicație prin care este implementat un musicplayer.

## Clase
În cadrul aplicației sunt utilizate următoarele clase:
1. String - reprezintă o clasă utilitară, creeată și folosită cu scopul de a evita utilizarea string-ul din STL
2. Song - această clasă reține informații individuale despre fiecare melodie în parte precum:
   titlul, artistul și albumul din care face parte melodia, precum și durata acestea, păstrată în minute și secunde
3. Playlist - clasa playlist permite creearea unei liste de piese, aceasta include următoarele câmpuri:
   Song* songs (reprezentând un vector de melodii), numele playlist-ului precum și numele acestuia
4. User - clasa reține informații cu privire la utilizatorii aplicației precum: numele de utilizator, email-ul și parola,
totodată având funcționalitatea de a-i pemite utilizatorului să-și schimbe parola.

## Funcționalități
Funcționalitățile programului includ:
1. Posibilitatea de a adăuga sau de a șterge o melodie din playlist
2. Afișarea playlist-ului
3. Calcularea lungimii unui playlist în minute și secunde
4. Este posibilă schimbarea parolei de către utilizator în urma introducerii parolei vechi ca măsură de securitate